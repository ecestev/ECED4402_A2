/*
 * File: svc.cpp
 * Original Source: http://lh.ece.dal.ca/eced4402/
 * Modified By: Stephen Sampson
 * Revised Date: December 5th 2017
 * Purpose: Supervisor call (SVC) handler
 */

#include <stdio.h>
#include "svc.h"
#include "process.h"
#include "queues.h"
#include "KernelCalls.h"
#include "systick.h"
#include "globals.h"
#include "uart.h"
#include "message.h"

/* Supervisor call (trap) entry point */
extern "C" void SVCall(void) {

    /* Save LR for return via MSP or PSP */
    __asm("     PUSH    {LR}");

    /* Trapping source: MSP or PSP? */
    __asm("     TST     LR,#4");        // Bit #4 indicates MSP (0) or PSP (1)
    __asm("     BNE     RtnViaPSP");

    /* Trapping source is MSP - save r4-r11 on stack (default, so just push) */
    __asm("     PUSH    {r4-r11}");
    __asm("     MRS r0,msp");
    __asm("     BL  SVCHandler");       // r0 is MSP
    __asm("     POP {r4-r11}");
    __asm("     POP     {PC}");

    /* Trapping source is PSP - save r4-r11 on psp stack (MSP is active stack) */
    __asm("RtnViaPSP:");
    __asm("     mrs     r0,psp");
    __asm("     stmdb   r0!,{r4-r11}"); // Store multiple, decrement before
    __asm("     msr psp,r0");
    __asm("     BL  SVCHandler");       // r0 Is PSP

    /* Restore r4..r11 from trapping process stack  */
    __asm("     mrs     r0,psp");
    __asm("     ldmia   r0!,{r4-r11}"); // Load multiple, increment after
    __asm("     msr psp,r0");
    __asm("     POP     {PC}");

}




















/* Supervisor call handler */
extern "C" void SVCHandler(struct stack_frame *argptr) {

    if(force_psp == TRUE){              // Force a return using PSP

        /* Ensure PSP points to the address of R0 */
        set_PSP(running -> sp + 8 * sizeof(unsigned int));
        force_psp  = FALSE;             // update flag
        SysTickStart();                 // start systick

        /* Change LR to indicate return to Thread mode using the PSP */
        __asm(" movw    LR,#0xFFFD");   // Lower 16 [and clear top 16]
        __asm(" movt    LR,#0xFFFF");   // Upper 16 only
        __asm(" bx  LR");               // Force return to PSP

    } else {                            // Handle kernel call using args in R7

        kcallargs *kcaptr = (struct kcallargs*)argptr->r7;
        /* Switch on code provided to kernel by pkCall() */
        switch(kcaptr->code){
            /* Bind running process to queue specified in arguments */
            case BIND:
                kcaptr->rtnvalue = KBind(kcaptr->arg1);
                break;
            /* Return value of running PID */
            case GETID:
                kcaptr->rtnvalue = KGetPID();
                break;
            /* Terminate running process and switch running to next process */
            case TERMINATE:
                KTerminateProcess();
                break;
            /* Handle IPC Operation (Send/Receive) */
            struct p_msg *pmsg;         // structure needed in both send and receive
            /* Send specified message to specified message queue (if it has an owner) */
            case SEND:
                pmsg = (struct p_msg *) kcaptr->arg1;
                kcaptr->rtnvalue = KSendMessage(pmsg->queueID, pmsg->msg, pmsg->msg_size);
                break;
            /* Receive message from specified message queue if it exists. Block process if not. */
            case RECEIVE:
                pmsg = (struct p_msg *) kcaptr->arg1;
                kcaptr->rtnvalue = (int) KReceiveMessage(pmsg->queueID, pmsg->msg, pmsg->msg_size);
                break;
            /* Default handler to shut compiler up */
            default:
                kcaptr -> rtnvalue = -1;
                break;
        }
    }
}

/* Signal that the PendSV handler is to be called on exit */
void TriggerPendSV(void) {
    NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
}

/* Save process state, switch to next waiting to run process, and restore that state */
extern "C" void PendSVHandler(void) {
    save_registers();
    next_process();
    restore_registers();
}



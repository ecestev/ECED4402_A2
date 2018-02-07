/*
 * File: process.cpp
*  Original Source: http://lh.ece.dal.ca/eced4402/
 * Modified By: Stephen Sampson and Mason Butler
 * Revised Date: December 5th 2017
 * Purpose: Provides function definitions for registering,
 *          running, and swapping processes including the saving and
 *          restoring of process state.
 */

#include <string>
#include "process.h"
#include "globals.h"
#include "uart.h"
#include "systick.h"
#include "KernelCalls.h"
#include "message.h"

/* Definition for table declared in globals.h */
std::string priorities[] = {"IDLE", "LOW", "MEDIUM", "HIGH", "HIGHEST"};

/* Returns contents of PSP (current process stack */
unsigned long get_PSP(void) {
    __asm(" mrs     r0, psp");
    __asm(" bx  lr");
    return 0;
}

/* Returns contents of MSP (main stack) */
unsigned long get_MSP(void) {
    __asm(" mrs     r0, msp");
    __asm(" bx  lr");
    return 0;
}

/* set PSP to ProcessStack */
void set_PSP(volatile unsigned long ProcessStack) {
    __asm(" msr psp, r0");
}

/* Set MSP to MainStack */
void set_MSP(volatile unsigned long MainStack) {
    __asm(" msr msp, r0");
}

/* Save r4..r11 on process stack */
void save_registers() {
    __asm(" mrs     r0,psp");
    __asm(" stmdb   r0!,{r4-r11}");
    __asm(" msr psp,r0");
}

/* Restore r4..r11 from stack to CPU */
void restore_registers() {
    __asm(" mrs r0,psp");
    __asm(" ldmia   r0!,{r4-r11}");
    __asm(" msr psp,r0");
}

/* Get stack pointer */
unsigned long get_SP() {
    __asm("     mov     r0,SP");
    __asm(" bx  lr");
    return 0;
}




/* Assign 'data' to R7 */
void assignR7(volatile unsigned long data) {
    __asm(" mov r7,r0");
}

/* Swap running process for next in queue */
void next_process(void) {
    running -> sp = get_PSP();              // save current stack pointer
    running = running->next;                // Set running process to next in queue
    set_PSP(running -> sp);                 // Set PSP
    uartformat *processprint = &FormatTable[running->pid];  // find appropriate entry in print format table
    UART0_printf(processprint->cursor);     // update cursor position in console
}

/* Register a new instance of a process with a specified priority and unique PID */
int reg_proc(void (*func_name)(), unsigned pid, unsigned priority) {
    int *stack = new int [STACKSIZE];       // Create Unique Process Stack
      if(stack == NULL)                     // If stack does not point to a valid address
          return ERROR;                     // Stack creation failed, return error

    pcb *temp = new pcb;                    // Create PCB for Process
    temp->pid = pid;                        // set PID field in PCB
    temp->sp = (unsigned long)stack + STACKSIZE - (sizeof(stack_frame));
    temp->priority = priority;              // Set Priority field in PCB
    if(priority >= highest_p)               // If this process is of higher priority than those already registered
        highest_p = priority;               // Update which queue contains highest priority process

    /* create a new stack frame for this process and initialize its registers */
    stack_frame *stack_init = (struct stack_frame*)temp->sp;
    stack_init->r0 = 0x00000000;
    stack_init->r1 = 0x11111111;
    stack_init->r2 = 0x22222222;
    stack_init->r3 = 0x33333333;
    stack_init->r4 = 0x44444444;
    stack_init->r5 = 0x55555555;
    stack_init->r6 = 0x66666666;
    stack_init->r7 = 0x77777777;
    stack_init->r8 = 0x88888888;
    stack_init->r9 = 0x99999999;
    stack_init->r10 = 0x10101010;
    stack_init->r11 = 0x11011011;
    stack_init->r12 = 0x12121212;
    stack_init->psr = 0x01000000;
    stack_init->pc = (unsigned long)func_name;
    stack_init->lr = (unsigned long)PTerminateProcess;

    procqueue[priority].enqueue(temp);      // Enqueue newly created process to proper queue
    uartformat *processprint = &FormatTable[pid];                           // find appropriate entry in print format table
    UART0_printf(processprint->reg.append(priorities[priority]));           // print diagnostic info to console
    next_pid++;                             // Increment value of next PID available to be registered
    return SUCCESS;                         // Process registered successfully
}

/* Dummy Process 1 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process1(void){
    unsigned int my_queue = PBind(running->pid);    // process call to kernel to bind to message queue
    char *txt = "FOR P9";                   // define message to be sent
    msgcontainer *rmsg;                     // create container for message
    _Delay(10);                             // delay X number of systick interrupts
    PSendMessage(9, txt, 6);                // process call to kernel to send message
    _Delay(10);                             // delay X number of systick interrupts
    PReceiveMessage(my_queue, rmsg, 6);     // process call to kernel to receive message
}






/* Dummy Process 2 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process2(void){
    unsigned int my_queue = PBind(running->pid);    // process call to kernel to bind to message queue
    msgcontainer *rmsg;                     // create container for message
    PReceiveMessage(my_queue, rmsg, 6);     // process call to kernel to receive message
}

/* Dummy Process 3 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process3(void){
    char *txt = "FOR ??";                   // define message to be sent
    _Delay(30);                             // delay 30 systick interrupts
    PSendMessage(3, txt, 6);                // process call to kernel to send message

}

/* Dummy Process 4 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process4(void){
    char *txt = "FOR ??";                   // define message to be sent
    _Delay(30);                             // delay 30 systick interrupts
    PSendMessage(3, txt, 6);                // process call to kernel to send message
}

/* Dummy Process 5 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process5(void){
    char *txt = "FOR ??";                   // define message to be sent
    _Delay(30);                             // delay 30 systick interrupts
    PSendMessage(3, txt, 6);                // process call to kernel to send message
}

/* Dummy Process 6 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process6(void){
    char *txt = "FOR ??";                   // define message to be sent
    _Delay(30);                             // delay 30 systick interrupts
    PSendMessage(3, txt, 6);                // process call to kernel to send message
}

/* Dummy Process 7 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process7(void){
    char *txt = "FOR ??";                   // define message to be sent
    _Delay(100);                            // delay 100 systick interrupts
    PSendMessage(3, txt, 6);                // process call to kernel to send message
}

/* Dummy Process 8 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process8(void){
    char *txt = "FOR ??";                   // define message to be sent
    _Delay(80);                             // delay 80 systick interrupts
    PSendMessage(3, txt, 6);                // process call to kernel to send message
}

/* Dummy Process 9 - Modified for various tests. This example is for 'comprehensive' test */
void dummy_process9(void){
    unsigned int my_queue = PBind(running->pid);    // process call to kernel to bind to message queue
    char *txt = "FOR P1";                   // define message to be sent
    msgcontainer *rmsg;                     // create container for message
    _Delay(10);                             // delay 10 systick interrupts
    PSendMessage(1, txt, 6);                // process call to kernel to send message
    _Delay(10);                             // delay 10 systick interrupts
    PReceiveMessage(my_queue, rmsg, 6);     // process call to kernel to receive message
}

/* Idle Process. CPU must be doing 'something'. Effectively a busy wait */
void idle_process(void){
    while(1);
}



/**********************************************************************************************************
 *                           Mason Butler authored the functions below
 *********************************************************************************************************/

/* Process call to kernel to perform specified function with given arguments (if applicable) */
int pkCall(unsigned int code, void *arg){
    volatile struct kcallargs args;         // create an argument structure to be passed to kernel
    args.code = code;                       // set code of kcallargs to that specified in pkCall
    args.arg1 = (unsigned int) arg;         // set arg1 of kcallargs to that specified in pkCall
    assignR7((unsigned long)&args);         // assign arg structure to R7 to be used by kernerl
    SVC();                                  // make a call to SVC
    return args.rtnvalue;                   // return value put into arg.rtnvalue by kernel
}

/* Process call to kernel to terminate process */
void PTerminateProcess(void){
    pkCall(TERMINATE, NULL);                // value returned from process kernel call with specified code/arg(s)
}

/* Process call to kernel to get PID of process */
unsigned int PGetPID(){
    return pkCall(GETID, NULL);             // value returned from process kernel call with specified code/arg(s)
}

/* Process call to kernel to bind to message queue */
signed int PBind(unsigned int queue_num){
    return pkCall(BIND, (void *) queue_num);// return value returned from process kernel call with specified code/arg(s)
}

/* Process call to kernel to send message given parameters */
signed int PSendMessage(unsigned int destQueueID, void *message, unsigned int msgSize){
    volatile struct p_msg pmsg;             // create message structure to store message to be sent
    pmsg.queueID = destQueueID;             // set the destination queue field of message struct
    pmsg.msg = message;                     // set the message field of message struct
    pmsg.msg_size = msgSize;                // set the message size fieled of message struct
    return pkCall(SEND, (void *)&pmsg);     // value returned from process kernel call with specified code/arg(s)
}

/* Process call to kernel to receive messaged given parameters */
signed int PReceiveMessage(unsigned int queueID, void *message, unsigned int msgSize){
    volatile struct p_msg pmsg;             // create message structure to store message to be received
    pmsg.queueID = queueID;                 // queue to receive message from
    pmsg.msg = message;                     // space to store the message to be received
    pmsg.msg_size = msgSize;                // size of the message we expect to receive
    return pkCall(RECEIVE, (void *) &pmsg); // value returned from process kernel call with specified code/arg(s)
}

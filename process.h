/*
 * File: process.h
*  Original Source: http://lh.ece.dal.ca/eced4402/
 * Modified By: Stephen Sampson and Mason Butler
 * Revised Date: December 5th 2017
 * Purpose: Contains definitions, structures, macros, and functions applicable to processes
 *          including but not limited to the ability to save/restore process state, perform
 *          context switches and IPC operations such as bind, send and receive.
 *          Requires access to process, message, and UART queues.
 */

#pragma once                            // ensure file is included only once in compilation

#include "queues.h"                     // allow access to process, message, and UART queue(s)

#define PRIVATE static                  // allow use of PRIVATE keyword in place of static
#define SVC()       __asm(" SVC #0")    // macro for SVC as it can not be called directly
#define STACKSIZE   1024                // size to be reserved for each process stack

/* Cortex default stack frame */
struct stack_frame {
    /* Must be stacked by software (if desired) */
    unsigned long r4;                   // general purpose register
    unsigned long r5;                   // general purpose register
    unsigned long r6;                   // general purpose register
    unsigned long r7;                   // general purpose register
    unsigned long r8;                   // general purpose register
    unsigned long r9;                   // general purpose register
    unsigned long r10;                  // general purpose register
    unsigned long r11;                  // general purpose register
    /* Stacked by hardware (implicit)*/
    unsigned long r0;                   // general purpose register + args
    unsigned long r1;                   // general purpose register + args
    unsigned long r2;                   // general purpose register + args
    unsigned long r3;                   // general purpose register + args
    unsigned long r12;                  //
    unsigned long lr;                   // link register (contains return address)
    unsigned long pc;                   // program counrer (entry point / execution location)
    unsigned long psr;                  // xPSR, process stack register
};

/* Prototypes for provided functions */
void set_LR(volatile unsigned long);        // set link register (return address / mode + stack)
void set_PSP(volatile unsigned long);       // set process stack pointer
void set_MSP(volatile unsigned long);       // set main stack pointer
void assignR7(volatile unsigned long data); // use R7 to pass address of arg structure to kernel
void save_registers();                      // save R4->R11 on process stack
void restore_registers();                   // restore R4->R11 from process stack to CPU
unsigned long get_PSP();                    // return contents of current process stack
unsigned long get_MSP(void);                // return contents of main stack
unsigned long get_SP();                     // return location of stack pointer

/* Prototypes for added functions */
int reg_proc(void (*func_name)(), unsigned pid, unsigned priority); // register and place process in proper queue
void PTerminateProcess(void);               // process call to kernel to terminate process
void next_process(void);                    // get the next waiting to run process (called from PendSVHandler())
void idle_process(void);                    // idle_process (infinite loop, never ends)
void dummy_process1(void);                  // dummy process (for testing)
void dummy_process2(void);                  // dummy process (for testing)
void dummy_process3(void);                  // dummy process (for testing)
void dummy_process4(void);                  // dummy process (for testing)
void dummy_process5(void);                  // dummy process (for testing)
void dummy_process6(void);                  // dummy process (for testing)
void dummy_process7(void);                  // dummy process (for testing)
void dummy_process8(void);                  // dummy process (for testing)
void dummy_process9(void);                  // dummy process (for testing)




/**********************************************************************************************************
 *                           Mason Butler authored the functions below
 *********************************************************************************************************/

int pkCall(unsigned int code, void *arg);   // process call to kernel with code + args (if applicable)
unsigned int PGetPID();                     // process call to kernel to get PID
signed int PBind(unsigned int queue_num);   // process call to kernel to bind process to msgqueue
/* process call to kernel to send message to a specified message queue */
signed int PSendMessage(unsigned int destQueueID, void *message, unsigned int msgSize);
/* process call to kernel to receive message from queue owned by process (ownership set on bind()) */
signed int PReceiveMessage(unsigned int queueID, void *message, unsigned int msgSize);

/*
 * File: KernelCalls.h
 * Author: Stephen Sampson and Mason Butler
 * Original Date: October 2nd 2017
 * Revised Date: December 5th 2017
 * Purpose: Define structure used to send arguements to the kernel as well as
 *          functions called from the kernel, not to be interrupted
 */

#pragma once                            // ensure file is included only once in compilation

#include "queues.h"                     // kernel needs to access process and message queues

/* Enumeration for kernel codes to improve readability and eliminate 'magic' numbers */
enum kernelcallcodes {GETID, BIND, SEND, RECEIVE, TERMINATE};

struct kcallargs {
    unsigned int code;                  // action (from enumeration table above) to perform
    unsigned int arg1;                  // arg1 (if applicable)
    unsigned int arg2;                  // arg2 (if applicable)
    int rtnvalue;                       // value returned by kernel to calling process (neg = error)
};

void KTerminateProcess(void);           // Kernel call to terminate 'running' process
unsigned int KGetPID();                 // Kernel call to get PID of 'runnign' process

/**********************************************************************************************************
 * Mason Butler originally authored the functions below. Testing and modifications by Stephen Sampson
 *********************************************************************************************************/

int KBind(unsigned int queue_num);      // Kernel call to bind process to specified message queue
/*Kernel call to send message to destination queue if bound to a process */
int KSendMessage(unsigned int destQueueID, void *message, unsigned int msgSize);
/* Kernel call to receive message from message queue if one exists, else block until message queue receives a message */
int KReceiveMessage(unsigned int queueID, void *message, unsigned int msgSize);

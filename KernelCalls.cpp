/*
 * File: KernelCalls.cpp
 * Author: Stephen Sampson and Mason Butler
 * Original Date: October 2nd 2017
 * Revised Date: December 5th 2017
 * Purpose: Includes definitions of functions called by the kernel.
 *          These are called while in an SVC and will not be interrupted.
 */

#include "globals.h"
#include "KernelCalls.h"
#include "process.h"
#include "systick.h"
#include "uart.h"
#include "queues.h"

/* Kernel call to terminate 'running' process */
void KTerminateProcess(void){
    int delpriority = running->priority;                        // priority of process being terminated
    uartformat *processprint = &FormatTable[running->pid];      // find appropriate entry in print format table
    for(int i = 0; i < MAX_MSG_QUEUES; i++)                     // iterate through message queues
        if(msgqueue[i].owner == running)                        // if a message queue is owned by process being deleted
            msgqueue[i].clear();                                // clear it and free memory being taken up by messages in queue
    running = running->next;                                    // set running to next process (points back to itself if only item in queue)
    procqueue[delpriority].remove(running->prev);               // remove the process that was previously running
    if (procqueue[delpriority].empty()) {                       // if terminated process's priority queue is empty
        for (int i = HIGHEST; i >= IDLE; i--) {                 // loop from highest to lowest priority
            if (!procqueue[i].empty()) {                        // until a non empty queue is found
                highest_p = i;                                  // set global var highest_p to this priority
                break;                                          // break from loop
            }
        }
        running = procqueue[highest_p].get_front();             // update running process to process at front of highest_p queue
    }
    force_psp = TRUE;                                           // SVCall & SVHandler will now set PSP, restart SysTick, and restore registers
    UART0_printf(processprint->terminate);                      // print diagnostic info to console indicating process has terminated
}

/**********************************************************************************************************
 * Mason Butler originally authored the functions below. Testing, changes, and comments by Stephen Sampson
 *********************************************************************************************************/

/* Kernel call to get PID of 'runnign' process */
unsigned int KGetPID(){
    return running->pid;
}

















/* Kernel call to bind process to specified message queue */
signed int KBind(unsigned int queue_num){
    if(queue_num < MAX_MSG_QUEUES){                             // if valid message queue number
        if(msgqueue[queue_num].owner == NULL){                  // verify queue does not already have an owner
            msgqueue[queue_num].owner = running;                // assign owner of queue to running process
            uartformat *processprint = &FormatTable[running->pid];
            UART0_printf(processprint->bind);                   // print diagnostic information to console
            return queue_num;                                   // return queue number as success message
        } else                                                  // queue has an owner
            return ERROR;                                       // return error
    } else                                                      // invalid queue number
        return ERROR;                                           // return error
}

/* Kernel call to send message to destination queue if bound to a process */
signed int KSendMessage(unsigned int destQueueID, void *message, unsigned int msgSize) {
    pcb *pcb_ptr = msgqueue[destQueueID].owner;                 // create pointer to PCB that owns specified message queue
    if(pcb_ptr != NULL) {                                       // if pointer has a value (queue has an owner)
        msgcontainer *msg = new msgcontainer();                 // create new message container structure
        msg->size = msgSize;                                    // set size field of message container
        msg->msg = (char *)message;                             // set message field of message container
        msgqueue[destQueueID].enqueue(msg);                     // queue message in specified message queue
        if (pcb_ptr->blocked == TRUE) {                         // if the process to receive the message is blocked
            procqueue[BLOCKED].dequeue(pcb_ptr);                // remove PCB from blocked queue
            procqueue[pcb_ptr->priority].enqueue(pcb_ptr);      // place PCB in proper queue (based on priority)
            pcb_ptr->blocked = FALSE;                           // update blocked flag in newly unblocked PCB
        }
        uartformat *processprint = &FormatTable[running->pid];  // find appropriate entry in print format table
        UART0_printf(processprint->send.append(msg->msg));      // print diagnostic information to console
        return SUCCESS;                                         // return success
    }                                                           // otherwise pointer is NULL (queue had no owner)
    return ERROR;                                               // return error
}

































/* Kernel call to receive message from message queue or block if one not available */
signed int KReceiveMessage(unsigned int queueID, void *message, unsigned int msgSize){
    pcb *pcb_ptr = msgqueue[queueID].owner;                     // get owner of specified message queue
    if(pcb_ptr != NULL) {                                       // if specified message queue has an owner (is bound to process)
        uartformat *processprint = &FormatTable[running->pid];  // find appropriate entry in print format table
        msgcontainer *msg = msgqueue[queueID].get_front();      // get message at front of specified message queue
        if(msg != NULL) {                                       // if there exists a message in the message queue (receive process)
            message = (void *) msg;                             // set value of message to be made available to process
            UART0_printf(processprint->receive.append(msg->msg));
            msgqueue[queueID].remove(msg);                      // remove message from message queue and free memory
            return SUCCESS;                                     // message successfully transmitted
        } else {                                                // no message in queue (block process and perform a context switch)
            save_registers();                                   // save registers (push R4->R11 onto the process stack)
            running -> sp = get_PSP();                          // update the stack pointer in the PCB of the process being switched out
            running = pcb_ptr->next;                            // set running to next process (points back to itself if only item in queue)
            set_PSP(running -> sp);                             // update PSP to that saved in the running PCB
            procqueue[pcb_ptr->priority].dequeue(pcb_ptr);      // dequeue process to be blocked
            procqueue[BLOCKED].enqueue(pcb_ptr);                // enqueue dequeued process to blocked queue
            pcb_ptr->blocked = TRUE;                            // set blocked flag in process's PCB
            if (procqueue[pcb_ptr->priority].empty()) {         // if priority queue of newly blocked process is now empty
                for (int i = HIGHEST; i >= IDLE; i--) {         // loop from highest to lowest priority
                    if (!procqueue[i].empty()) {                // until a non empty queue is found
                        highest_p = i;                          // set global var highest_p to this priority
                        break;                                  // break from loop
                    }
                }
                running = procqueue[highest_p].get_front();     // update running process to process at front of highest_p queue
            }
            UART0_printf(processprint->blocked);                // print diagnostic information to console
            return SUCCESS;                                     // message successfully queued
        }
    }
    return ERROR;                                               // specified message queue had no owner (not bound to a process)
}

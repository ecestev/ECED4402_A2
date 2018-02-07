/*
 * File: queues.h
 * Author: Stephen Sampson
 * Original Date: 9/23/2017 (Expanded A#1 Implementation)
 * Revised Date: December 5th 2017
 * Purpose: This file contains the structure of the queue classes used
 *          to create priority process queues, message queues, and
 *          UART0 TX/RX queues.
 *          All queues are circular in nature with UART queues being of
 *          a fixed size (defined in globals.h).
 *          Process and message queues are dynamic therefore their sizes are
 *          free to grow/shrink as needed. These queues also contain a pointer
 *          to both the next and previous element in the queue. If only one element
 *          exists, the circular nature has these pointers pointing to itself.
 *          Memory for PCBs stored is allocated during process registration:
 *              see reg_process() in 'process.cpp'.
 *          Memory for MSGs stored is allocated during message creation:
 *              see KSendMessage() in 'KernelCalls.cpp'.
 *          Memory for a message or PCB is freed on remove() which is a member function
 *          for both pqueue and mqueue.
 */
#pragma once                        // ensure file is included only once in compilation

/**************************************************
 *                  PROCESSES
 *************************************************/

/* Process Control Block Structure */
class pcb {
public:
    pcb* next;                      // pointer to next PCB
    pcb* prev;                      // pointer to previous PCB
    unsigned long sp;               // location of process stack pointer
    unsigned long pid;              // PID of process
    unsigned long priority;         // priority of process
    bool blocked;                   // flag indicating if process is blocked or not (not sure if needed)
    pcb(void);                      // constructor for new PCB
    ~pcb(void);                     // custom destructor for PCB
};

/* Process Queues */
class p_queue {
private:
    pcb* front;                     // pointer to the PCB at the front (head) of the queue
public:
    p_queue(void);                  // constructor of an empty queue
    ~p_queue(void);                 // destructor for the process queue (never called)
    void enqueue(pcb* ptr);         // put x at the back of the list
    void dequeue(pcb* ptr);         // dequeues a PCB without deleting (for swapping queues)
    pcb* get_front(void);           // returns pointer to PCB at the front of the queue
    bool remove(pcb* ptr);          // dequeues a PCB and frees its memory
    bool empty(void) const;         // check for empty queue
};

/**************************************************
 *                  MESSAGES
 *************************************************/

/* Message Structure */
class msgcontainer {
public:
    msgcontainer* next;             // pointer to the next MSG
    msgcontainer* prev;             // pointer to the previous MSG
    int size;                       // size of the message being passed|queued
    char* msg;                      // the message itself
    msgcontainer(void);             // constructor for a message container
    ~msgcontainer(void);            // destructor for a message container
};

/* Message Queues */
class m_queue {
private:
    msgcontainer* front;            // pointer to the MSG at the front (head) of the queue
public:
    pcb* owner;                     // the PCB associated with process bound to queue
    m_queue(void);                  // constructor of an empty queue
    ~m_queue();                     // destructor for the message queue (never called)
    void enqueue(msgcontainer* ptr);// put x at the back of the list
    msgcontainer* get_front(void);  // get the node at the front of the list
    bool remove(msgcontainer* ptr); // dequeues a message container and frees its memory
    bool empty(void) const;         // check for empty queue
    void clear(void);               // empty the message queue and delete any queued messages
};

/**************************************************
 *                    UART
 *************************************************/

/* UART RX and TX Queues */
class u_queue {
private:
    int max;                        // maximum size of the queue
    int items;                      // number of items in the queue
    int insloc;                     // next location to insert
    int remloc;                     // next location to remove
    char* data;                     // vector holding the queue of items
public:
    u_queue(int sz);                // constructor, max = sz
    ~u_queue();                     // destructor
    void enqueue(char UART_RX_CHAR);// put the char onto the queue
    char dequeue(void);             // get char at the head of the queue
    bool empty(void) const;         // check for empty queue
};

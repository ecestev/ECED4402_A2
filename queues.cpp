/*
 * File: queues.cpp
 * Author: Stephen Sampson
 * Original Date: September 23rd 2017 (Expanded A#1 Implementation)
 * Revised Date: December 5th 2017
 * Purpose: See queues.h for detailed explanation. Contains definitions
 *          for member functions of all queues as well as constructors
 *          and destructors for objects stored in these queues.
 */

#include "globals.h"
#include "queues.h"

/**************************************************
 *                  PROCESSES
 *************************************************/

/* constructor for a new PCB */
pcb::pcb(void) {
    sp = NULL;
    pid = NULL;
    priority = NULL;
    next = NULL;
    prev = NULL;
    blocked = FALSE;
}

/* destructor for a PCB freeing any dynamically allocated memory*/
pcb::~pcb(void) {
    // Default constructor sufficient.
    // All message queues bound to PCB are emptied and the
    // memory allocated to the objects freed in m_queue::clear().
    // Next, PCB memory is freed by calling p_queue::remove().
}

/* constructor for a new process queue */
p_queue::p_queue(void) {
    front = NULL;
}

/* destructor for a process queue */
p_queue::~p_queue(void) {
    // NEVER CALLED
    // Process queues are never deleted
}

/* enqueue PCB to back of queue */
void p_queue::enqueue(pcb* ptr) {
    if (front == NULL) {
        front = ptr;
        ptr->next = ptr;
        ptr->prev = ptr;
    } else {
        ptr->next = front;
        ptr->prev = front->prev;
        front->prev->next = ptr;
        front->prev = front->prev->next;
    }
}

/* get PCB at front of process queue */
pcb* p_queue::get_front(void) {
    return front;
}





/* remove PCB from process queue without deleting */
/* used when moving between queues to avoid copying */
void p_queue::dequeue(pcb* ptr){
    if(front->next == NULL){
        front = NULL;
    } else {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        if(ptr==front)
            front = front->next;
    }
}

/* remove PCB from process queue and free its memory */
bool p_queue::remove(pcb* ptr) {
    if (ptr == NULL)
        return false;
    if(front->next == front) {
        front = NULL;
    } else {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        if(ptr == front) {
            front = front->next;
        }
    }
    delete ptr;
    return true;
}

/* return T|F : process queue is empty */
bool p_queue::empty(void) const {
    return (front == NULL);
}

/**************************************************
 *                  MESSAGES
 *************************************************/

/* constructor for a new message container */
msgcontainer::msgcontainer(void) {
    size = 0;
    msg = NULL;
    next = NULL;
    prev = NULL;
}

/* destructor for a message container */
msgcontainer::~msgcontainer(void) {
    // links fixed and memory freed in msgcontainer::remove()
}

/* constructor for a new message queue */
m_queue::m_queue(void) {
    front = NULL;
    owner = NULL;
}

/* destructor for a message queue */
m_queue::~m_queue(void) {
    // NEVER CALLED
    // Message queues are never deleted
}





/* enqueue message container to back of message queue */
void m_queue::enqueue(msgcontainer* ptr) {
    if (front == NULL) {
        front = ptr;
        ptr->next = ptr;
        ptr->prev = ptr;
    } else {
        ptr->next = front;
        ptr->prev = front->prev;
        front->prev->next = ptr;
        front->prev = front->prev->next;
    }
}

/* get message container at front of message queue */
msgcontainer* m_queue::get_front(void) {
    return front;
}

/* remove messagecontainer from message queue and free its memory */
bool m_queue::remove(msgcontainer* ptr) {
    if (ptr == NULL)
        return false;
    if(front->next == front) {
        front = NULL;
    } else {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        if(ptr == front) {
            front = front->next;
        }
    }
    delete ptr;
    return true;
}

/* return T|F : message queue is empty */
bool m_queue::empty(void) const {
   return (front == NULL);
}

/* Clear all entries in message queue freeing memory */
void m_queue::clear(void) {
    while(front != NULL){
        msgcontainer *temp = front;
        if(front->next == front) {
            front = NULL;
        } else {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            if(temp == front) {
                front = front->next;
            }
        }
        delete temp;
    }
}











/******************************************
*          UART RX and TX Queues
******************************************/

/* Constructor for a new UART buffer/queue */
u_queue::u_queue(int sz) : data(new char[sz]) {
    max = sz;
    items = 0;
    insloc = 0;
    remloc = 0;
}

/* destructor for a UART buffer/queue freeing any dynamically allocated memory*/
u_queue::~u_queue(){
    delete [] data;
}

/* enqueue character at tail of queue*/
void u_queue::enqueue(char val) {
    if(items!=max){
        data[insloc] = val;
        items++;
        insloc++;
        if(insloc>=(max-1))
            insloc = 0;
    }
}

/* remove character from head of queue and return it */
char u_queue::dequeue(void) {
        char temp = data[remloc];
        items--;
        remloc++;
         if(remloc>=(max-1))
            remloc = 0;
        return temp;
}

/* return T|F : queue is empty */
bool u_queue::empty(void) const {
    return (items == 0);
}

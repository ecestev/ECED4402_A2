/*
 * File: message.h
 * Author: Mason Butler
 * Original Date: November 28th 2017
 * Revised Date: December 5th 2017
 * Purpose: Define structure used to hold message being sent, received,
 *          or queued during message passing.
 */

#pragma once                            // ensure file is included only once in compilation

/* message structure */
struct p_msg {
    unsigned int queueID;               // message queue for message
    void *msg;                          // the message itself
    unsigned int msg_size;              // size of message
} p_msg;

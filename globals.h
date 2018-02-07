/*
 * File: globals.h
 * Author: Stephen Sampson
 * Original Date: October 2nd 2017
 * Revised Date: December 5th 2017
 * Purpose: To declare 'things' (structs, vars, macros etc..)
 *          accessible within the entire scope of the project.
 */

#pragma once                            // ensure file is included only once in compilation

#include "queues.h"                     // u_queue and p_queue object types
#include <string>                       // std::string (used in priority table)

/* Enumeration of queue priorities to increase readability / avoid 'magic numbers' */
enum pqueuepriorities {IDLE, LOW, MEDIUM, HIGH, HIGHEST, BLOCKED};

/* Table for printing to UART. Eliminates calls to functions such as sprintf
 * as well as use of string streams to increase efficiency */
typedef struct {
    std::string cursor;                 // cursor location (indicates running process)
    std::string reg;                    // prefix for registration message. Appended with priority before printing
    std::string bind;                   // formatted string to print on bind
    std::string send;                   // formatted string to print on send
    std::string blocked;                // formatted string to print on block
    std::string receive;                // formatted string to print on receive
    std::string terminate;              // formatted string to print on terminate
} uartformat;

/* Global Defines and Macros */
#define TRUE    1                       // Global definition of TRUE = 1
#define FALSE   0                       // Global definition of FALSE = 0
#define ERROR   -1                      // Return -1 for errors
#define SUCCESS 1                       // Return 1 for success
#define UART0_BUFF_SZ   512             // Size of UART buffer
#define NUM_PROC_QUEUES 6               // Priorities: 'IDLE', 'LOW'->'HIGHEST', and 'BLOCKED'
#define MAX_MSG_QUEUES  16              // Max number of msg queues
#define MAX_MSG_SIZE    256             // Longest valid message size
#define GIntDisable() __asm(" cpsid i") // Global interrupt disable
#define GIntEnable()  __asm(" cpsie i") // Global interrupt enable

/* Global Variables */
extern int highest_p;                   // Used to identify highest priority queue containing WTR process(es)
extern int next_pid;                    // Used to track next PID assigned by reg_process()
extern unsigned int ticks;              // Tick counter. Used in _Delay() function
extern bool force_psp;                  // Global flag indicating if call to SVC is to load process state

/* Globally Accessible Objects */
/* Queues */
extern u_queue UART0_TX_BUFFER;         // UART Transmit Buffer
extern p_queue procqueue[];             // Process Queue (All priorities, Idle->Highest & Blocked)
extern m_queue msgqueue[];              // Message Queue (size specified by global define MAX_MSG_QUEUES)

/* Other Objects */
extern pcb* running;                    // Pointer to running process's PCB
extern uartformat FormatTable[];        // Table of formatted strings to print to UART (defined in UART.cpp)
extern std::string priorities[];        // Table of priorities (as strings - defined in process.cpp)

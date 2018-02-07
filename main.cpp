/*
 * File: main.cpp
 * Author: Stephen Sampson
 * Original Date: October 2nd 2017
 * Revised Date: December 5th 2017
 * Purpose: Main program execution for Assignment #2 - A Lightweight Kernel
 *          Defines global variables, initializes global structures, configures
 *              devices (systick and UART0).
 *          Registers all processes to run on system allocating necessary memory
 *              and placing in appropriate queue.
 *          Sets the first running process and makes first call to SVC. Execution
 *          never comes back to main therefore it does not return.
 */

#include "globals.h"
#include "systick.h"
#include "queues.h"
#include "process.h"
#include "KernelCalls.h"
#include "kernel.h"
#include "UART.h"

/* Create queues of size specified in globals.h */
u_queue UART0_TX_BUFFER(UART0_BUFF_SZ);
p_queue procqueue[NUM_PROC_QUEUES];
m_queue msgqueue[MAX_MSG_QUEUES];

/* Pointer PCB of running process */
pcb* running;

/* Global Variables */
int highest_p = 0;
int next_pid = 0;
unsigned int ticks = 0;
bool force_psp = TRUE;

void main(void) {

    /*  Initialize UART */
    UART0_Init();                               // Initialize UART0
    InterruptEnable(INT_VEC_UART0);             // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts

    /* Initialize SYSTICK */
    SysTickPeriod(MAX_WAIT);                    // Set SysTick Period
    SysTickIntEnable();                         // Enable SysTick Interrupts

    /* Initialize Kernel */
    KernelInit();                               // Initialize Kernel

    UART0_printf("\033c\033[1;1H");             // Clear Console
    UART0_printf(                               // Print Welcome Banner
            "---------------------------------------------------------------\n\r"
            "                   ECED4402 - Real Time Systems                \n\r"
            "                 Assignment #2 - A Lightweight Kernel          \n\r"
            "    Stephen Sampson (B00568374) and Mason Butler (B00######)   \n\r"
            "                          Fall 2017                            \n\r"
            "---------------------------------------------------------------");

			
			
			
			
			
			
			
			
			
    
	
	/* INIT ALL STACKS AND ALL PCBs */
    reg_proc(idle_process, next_pid, IDLE);
    reg_proc(dummy_process1, next_pid, LOW);
    reg_proc(dummy_process2, next_pid, LOW);
    reg_proc(dummy_process3, next_pid, HIGHEST);
    reg_proc(dummy_process4, next_pid, HIGH);
    reg_proc(dummy_process5, next_pid, MEDIUM);
    reg_proc(dummy_process6, next_pid, MEDIUM);
    reg_proc(dummy_process7, next_pid, LOW);
    reg_proc(dummy_process8, next_pid, LOW);
    reg_proc(dummy_process9, next_pid, LOW);


    /* Set First Running Process */
    running = procqueue[highest_p].get_front();

    /* Enable Interrupts */
    GIntEnable();

    /* Main Program Execution */
    SVC();

}

/*
 * File: systick.cpp
 * Original Source: http://lh.ece.dal.ca/eced4402/SysTick.c
 * Modified By: Stephen Sampson
 * Revised Date: December 5th 2017
 * Purpose: SysTick definitions
 */

#include "systick.h"
#include "svc.h"
#include "globals.h"
#include "uart.h"

/* Set the clock source to internal and enable the counter to interrupt */
void SysTickStart(void) {
    ST_CTRL_R |= ST_CTRL_CLK_SRC | ST_CTRL_ENABLE;
}

/* Clear the enable bit to stop the counter */
void SysTickStop(void) {
    ST_CTRL_R &= ~(ST_CTRL_ENABLE);
}

/* Set systick reload value to specified period (between 2 and 16777216)  */
void SysTickPeriod(unsigned long Period) {
    ST_RELOAD_R = Period - 1;
}

/* Set the interrupt bit in STCTRL */
void SysTickIntEnable(void) {
    ST_CTRL_R |= ST_CTRL_INTEN;
}

/* Clear the interrupt bit in STCTRL */
void SysTickIntDisable(void) {
    ST_CTRL_R &= ~(ST_CTRL_INTEN);
}

/* Trigger PendSV() and increment global counter of 'ticks' */
extern "C" void SysTickHandler(void) {
    ticks++;
    TriggerPendSV();
}

/* A delay function used diagnostically to time process operations and
 * ensure a predictable number of time quantums pass before continuing */
void _Delay(unsigned int cnt){
    cnt += ticks;
    while(ticks < cnt){}
}

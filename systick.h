/*
 * File: systick.h
 * Original Source: http://lh.ece.dal.ca/eced4402/SysTick.c
 * Modified By: Stephen Sampson
 * Revised Date: December 5th 2017
 * Purpose: Systick Defines and Prototypes
 */

#pragma once                                // ensure file is included only once in compilation

/* SysTick Control and Status Register (STCTRL) */
#define ST_CTRL_R   (*((volatile unsigned long *)0xE000E010))
/* Systick Reload Value Register (STRELOAD) */
#define ST_RELOAD_R (*((volatile unsigned long *)0xE000E014))

/* SysTick defines */
#define ST_CTRL_COUNT      0x00010000       // Count Flag for STCTRL
#define ST_CTRL_CLK_SRC    0x00000004       // Clock Source for STCTRL
#define ST_CTRL_INTEN      0x00000002       // Interrupt Enable for STCTRL
#define ST_CTRL_ENABLE     0x00000001       // Enable for STCTRL
#define MAX_WAIT           0x18B820         // Max Period

/* SysTick function prototypes */
void SysTickStart(void);                    // Set the clock source & enable counter to interrupt
void SysTickStop(void);                     // Clear the enable bit to stop the counter
void SysTickPeriod(unsigned long Period);   // Set period (2-->16777216)
void SysTickIntEnable(void);                // Set the interrupt bit in STCTRL
void SysTickIntDisable(void);               // Clear the interrupt bit in STCTRL
extern "C" void SysTickHandler(void);       // The handler called on each systick()
void _Delay(unsigned int cnt);              // Diagnostic delay function



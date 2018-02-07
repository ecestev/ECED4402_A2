/*
 * File: kernel.h
 * Author: Stephen Sampson
 * Original Date: October 2nd 2017
 * Revised Date: December 5th 2017
 * Purpose: Set the priority of PENDSV and define kernel initialization
 *          function.
 */

#pragma once                            // ensure file is included only once in compilation

/* location of register containing PendSV prority */
#define NVIC_SYS_PRI3_R (*((volatile unsigned long *) 0xE000ED20))

/* define lowest priority as 7 as per page 179 in data sheet */
#define PENDSV_LOWEST_PRIORITY 0x00E00000

/* kernel init */
void KernelInit(void);

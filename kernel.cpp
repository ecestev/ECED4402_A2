/*
 * File: kernel.cpp
 * Author: Stephen Sampson
 * Original Date: October 2nd 2017
 * Revised Date: December 5th 2017
 * Purpose: Performs kernel initialization
 */

#include "kernel.h"

/* Ensure PendSV priority is set to lowest (7) */
void KernelInit() {
    NVIC_SYS_PRI3_R |= PENDSV_LOWEST_PRIORITY;
}

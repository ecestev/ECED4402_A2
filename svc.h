/*
 * File: svc.h
 * Original Source: http://lh.ece.dal.ca/eced4402/
 * Modified By: Stephen Sampson
 * Revised Date: December 5th 2017
 * Purpose: Supervisor Call (SVC) Handler
 */

#pragma once                            // ensure file is included only once in compilation

#define NVIC_INT_CTRL_R (*((volatile unsigned long *) 0xE000ED04))
#define TRIGGER_PENDSV 0x10000000
void TriggerPendSV(void);               // trigger pendSV, called on systick
extern "C" void PendSVHandler(void);    // handler for pendSV executed when no higher priority interrupts remain

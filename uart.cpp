/*
 * File: uart.cpp
 * Original Source: http://lh.ece.dal.ca/eced4402/Tiva_UART_EchoV1.c
 * Original Author: Emad Khan
 * Modified By: Stephen Sampson
 * Revised Date: December 5th 2017
 * Purpose: UART function definitions.
 */

#include <queues.h>
#include "uart.h"
#include "globals.h"
#include "systick.h"

/* Definition for table declared in globals.h */
uartformat FormatTable[] =
{
{"\033[8;2H",  "\033[8;1HP0: ",  "\033[8;16HBND",  "\033[8;23HTX:",  "\033[8;36HBLKD",  "\033[8;43HRX:",  "\033[8;54HXXX"},
{"\033[9;2H",  "\033[9;1HP1: ",  "\033[9;16HBND",  "\033[9;23HTX:",  "\033[9;36HBLKD",  "\033[9;43HRX:",  "\033[9;54HXXX"},
{"\033[10;2H", "\033[10;1HP2: ", "\033[10;16HBND", "\033[10;23HTX:", "\033[10;36HBLKD", "\033[10;43HRX:", "\033[10;54HXXX"},
{"\033[11;2H", "\033[11;1HP3: ", "\033[11;16HBND", "\033[11;23HTX:", "\033[11;36HBLKD", "\033[11;43HRX:", "\033[11;54HXXX"},
{"\033[12;2H", "\033[12;1HP4: ", "\033[12;16HBND", "\033[12;23HTX:", "\033[12;36HBLKD", "\033[12;43HRX:", "\033[12;54HXXX"},
{"\033[13;2H", "\033[13;1HP5: ", "\033[13;16HBND", "\033[13;23HTX:", "\033[13;36HBLKD", "\033[13;43HRX:", "\033[13;54HXXX"},
{"\033[14;2H", "\033[14;1HP6: ", "\033[14;16HBND", "\033[14;23HTX:", "\033[14;36HBLKD", "\033[14;43HRX:", "\033[14;54HXXX"},
{"\033[15;2H", "\033[15;1HP7: ", "\033[15;16HBND", "\033[15;23HTX:", "\033[15;36HBLKD", "\033[15;43HRX:", "\033[15;54HXXX"},
{"\033[16;2H", "\033[16;1HP8: ", "\033[16;16HBND", "\033[16;23HTX:", "\033[16;36HBLKD", "\033[16;43HRX:", "\033[16;54HXXX"},
{"\033[17;2H", "\033[17;1HP9: ", "\033[17;16HBND", "\033[17;23HTX:", "\033[17;36HBLKD", "\033[17;43HRX:", "\033[17;54HXXX"},
{0,    0,    0,}
};

/* Initialize UART0 */
void UART0_Init(void) {
    volatile int wait;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCUART_GPIOA;     // Enable Clock Gating for UART0
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCGPIO_UART0;     // Enable Clock Gating for PORTA
    wait = 0;                                       // give time for the clocks to activate
    UART0_CTL_R &= ~UART_CTL_UARTEN;                // Disable the UART
    wait = 0;                                       // wait required before accessing the UART config regs
    UART0_IBRD_R = 8;                               // IBRD = int(16,000,000 / (16 * 115,200)) = 8.680555555555556
    UART0_FBRD_R = 44;                              // FBRD = int(.680555555555556 * 64 + 0.5) = 44.05555555555556
    UART0_LCRH_R = (UART_LCRH_WLEN_8);              // WLEN: 8, no parity, one stop bit, without FIFOs)
    GPIO_PORTA_AFSEL_R = 0x3;                       // Enable Receive and Transmit on PA1-0
    GPIO_PORTA_PCTL_R = (0x01) | ((0x01) << 4);     // Enable UART RX/TX pins on PA1-0
    GPIO_PORTA_DEN_R = EN_DIG_PA0 | EN_DIG_PA1;     // Enable Digital I/O on PA1-0
    UART0_CTL_R = UART_CTL_UARTEN;                  // Enable the UART
    wait = 0;                                       // wait; give UART time to enable itself.
}

/* Indicate to CPU which device is to interrupt */
void InterruptEnable(unsigned long InterruptIndex) {
    if(InterruptIndex < 32)                         // If Interrupt index is less than 32
        NVIC_EN0_R = 1 << InterruptIndex;           // Enable the interrupt in the EN0 Register
    else                                            // Interrupt index is greater than 32
        NVIC_EN1_R = 1 << (InterruptIndex - 32);    // Enable the interrupt in the EN1 Register
}

/* Set specified bits for interrupt */
void UART0_IntEnable(unsigned long flags) {
    UART0_IM_R |= flags;                            // Set UART0 interrupt mask register
}

/* Handles RX and TX Interrupts */
extern "C" void UART0_IntHandler(void) {

    if (UART0_MIS_R & UART_INT_RX) {                // UART0: handle receive operation
        UART0_ICR_R |= UART_INT_RX;                 // RX done - clear interrupt
        Data = UART0_DR_R;                          // Set 'Data' to char received by UART
        GotData = TRUE;                             // set global flag GotData
    }

    if (UART0_MIS_R & UART_INT_TX) {                // UART0: handle transmit operation
        UART0_ICR_R |= UART_INT_TX;                 // XMIT done - clear interrupt
        for(int i = 0; i < 1000; i++){}             // cause a short delay to fix clobbering
        if(!UART0_TX_BUFFER.empty())                // if characters remain in TX buffer
          UART0_DR_R = UART0_TX_BUFFER.dequeue();   // dequeue next character to UART0_DR_R
    }
}


/* Allows string printing to UART0 */
void UART0_printf(std::string toprint) {
    for(unsigned i = 0; i < toprint.length(); i++)  // for length of string
         UART0_TX_BUFFER.enqueue(toprint.at(i));    // queue character at position (i)
    UART0_DR_R = UART0_TX_BUFFER.dequeue();         // when string queued, set UART0_DR_R to first char to be transmitted
}

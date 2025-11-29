/*
 * File:   timer.c
 * Author: jess
 *
 * Created on November 29, 2025, 12:06 PM
 */

#include <xc.h>
#define _XTAL_FREQ 4000000
// T1 for gear shifting timer
void T1_Initialize() {
    // Timer1 initialization 
    T1CONbits.RD16 = 1;
    T1CONbits.T1RUN = 1;
    T1CONbits.T1CKPS = 0b11; // prescalar = 8
    T1CONbits.T1OSCEN = 0;
    T1CONbits.TMR1CS = 0; // internal clock
    T1CONbits.TMR1ON = 1;
    
    // Fosc/4 = 1 MHz
    // 1MHz / 8 = 125 kHz
    // for 0.5s => 65200
    // 65536 - 62500 = 3036 = 0x0bdc
    TMR1H = 0x0b;
    TMR1L = 0xdc;
    
    PIR1bits.TMR1IF = 0;
    IPR1bits.TMR1IP = 0; // low priority
    PIE1bits.TMR1IE = 1;
}

void T2_Initialize() {
    // Set up PR2, CCP to decide PWM period and Duty Cycle
    /* 16 * 125 = 2000us = 2ms => 500Hz

    PR2 = 124 = 0x7c
    PWM period
    = (PR2 + 1) * 4 * Tosc * (TMR2 prescaler)
    = (0x7c + 1) * 4 * 0.25µs * 16
    = 2000us = 2ms
    Duty cycle
    = (CCPRxL:CCPxCON<5:4>) * Tosc * (TMR2 prescaler)
    = duty * 0.25µs * 16
    = 4 * duty us
    => duty = 0 ~ 500*/
    // Timer2 -> On, prescaler -> 16
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b11;
    PR2 = 0x7c;
}

void Timer_Initialize() {
    T1_Initialize();
    T2_Initialize();
}
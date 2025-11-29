/*
 * File:   pin_manager.c
 * Author: hding
 *
 * Created on 2025?11?23?, ?? 2:28
 */


#include <xc.h>

void PIN_MANAGER_Initialize() {
    ADCON1 = 0x0f;

    TRISD = 0x0f;
    LATD = 0;
    
    // Set up PR2, CCP to decide PWM period and Duty Cycle
    /* 16 * 125 = 2000us = 2ms => 500Hz
     * PR2 = 124 = 0x7c
     * PWM period
     * = (PR2 + 1) * 4 * Tosc * (TMR2 prescaler)
     * = (0x7c + 1) * 4 * 0.25µs * 16
     * = 2000us = 2ms
     * 
     * Duty cycle
     * = (CCPRxL:CCPxCON<5:4>) * Tosc * (TMR2 prescaler)
     * = duty * 0.25µs * 16
     * = 4 * duty us
     * => duty = 0 ~ 500
     */
    // Timer2 -> On, prescaler -> 16
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b11;
    PR2 = 0x7c;
    
    
}

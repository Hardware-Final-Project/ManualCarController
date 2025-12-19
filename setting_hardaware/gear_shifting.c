/*
 * File:   gear_shifting.c
 * Author: jess
 *
 * Created on November 29, 2025, 11:40 AM
 */
#include <xc.h>
#define _XTAL_FREQ 4000000

void Gear_Shifting_Initialize(){
    // Initialize stepper motor control pins
    LATD6 = 0;
    LATD7 = 0;
}
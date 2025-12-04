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
    // LATD6: B-1A, LATD7: B-1B
}

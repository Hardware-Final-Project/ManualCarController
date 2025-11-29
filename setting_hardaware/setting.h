#ifndef SETTING_H
#define	SETTING_H
#include <xc.h>
#include <pic18f4520.h>
#include "uart.h"
#include "ccp.h"
#include "adc.h"
#include "interrupt_manager.h"
#include "pin_manager.h"
#include "timer.h"
#include "gear_shifting.h"

void SYSTEM_Initialize();
void OSCILLATOR_Initialize(void);


#endif
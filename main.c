#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include <stdbool.h>
#include "stdio.h"
#include "string.h"

// 1->thruttle, 2->gear shifting, 3-> reset gear
char mode = 0;

char ch;

char read_flag = 0, check_gear_input_flag = 0, reset_gear_flag = 0;

int cnt = 0;

char change_gear = 0;

unsigned int duty1 = 0;
unsigned int duty2 = 0;

char gear_mode = 1, prev_gear_mode = 1;


char direction = 0;

char reset_times = 0;


char delay100ms(int max_cnt) {
    if (cnt >= max_cnt) {
        cnt = 0;
        return 1;
    }
    return 0;
}



bool inputAvailable() {
    char mbuffer[3];
    strcpy(mbuffer, GetString());
    if(strcmp("m1", mbuffer) == 0) {
        mode = 1;
        return true;
    } else if(strcmp("m2", mbuffer) == 0) {
        mode = 2;
        return true;
    } else if(strcmp("m3", mbuffer) == 0) {
        mode = 3;
        return true;
    }
    return false;
}

void thruttle() {
    while(sscanf(GetString(), "%u %u", &duty1, &duty2) != 2);
    CCPR1L = (duty1 >> 2) & 0xff;
    CCP1CONbits.DC1B = duty1 & 0x03;

    CCPR2L = (duty2 >> 2) & 0xff;
    CCP2CONbits.DC2B = duty2 & 0x03;               
    
}

void gearShifting() {
    while(sscanf(GetString(), "g%hhd", &gear_mode) != 1);
    if (gear_mode != prev_gear_mode) {
        change_gear = 1;
    }
    
}

void resetGear() {
    while (sscanf(GetString(), "rd%hhdrt%hhd", &direction, &reset_times) != 2);
    if (direction == 0) {
        LATD6 = 1;
        LATD7 = 0;
        T1CONbits.TMR1ON = 1;
        while (!delay100ms(reset_times)) {}
        T1CONbits.TMR1ON = 0;
        LATD6 = 0;
        LATD7 = 0;
    } else {
        LATD6 = 0;
        LATD7 = 1;
        T1CONbits.TMR1ON = 1;
        while (!delay100ms(reset_times)) {}
        T1CONbits.TMR1ON = 0;
        LATD6 = 0;
        LATD7 = 0;
    }
}

void executeCommand() {
    while (!getEnterFlag()) { }
    if (!inputAvailable()) {
        ClearBuffer();
        return;
    }
    ClearBuffer();
    switch(mode) {
        case 1:
            thruttle();
            break;
        case 2:
            gearShifting();
            break;
        case 3:
            resetGear();
            break;
    }
    mode = 0;
}


void main(void) {
    
    SYSTEM_Initialize();
    
    
    
    
    CCPR1L = (duty1 >> 2) & 0xff;
    CCP1CONbits.DC1B = duty1 & 0x03;
    
    CCPR2L = (duty2 >> 2) & 0xff;
    CCP2CONbits.DC2B = duty2 & 0x03;
    
    
    
    while (1) {
        executeCommand();
        if(getEnterFlag() == 1) ClearBuffer();
        if (change_gear) {
            switch (prev_gear_mode) {
                    case 1:
                        switch (gear_mode) {
                            case 1:
                                break;
                            case 2:
                                LATD6 = 0;
                                LATD7 = 1;
                                T1CONbits.TMR1ON = 1;
                                if (delay100ms(1)) {
                                    T1CONbits.TMR1ON = 0;
                                    LATD6 = 0;
                                    LATD7 = 0;
                                    change_gear = 0;
                                }
                                break;
                            case 3:
                                
                                break;
                            default:
                                
                                break;
                        
                        }
                        break;
                        
                    case 2:
                        switch (gear_mode) {
                            case 1:
                                
                                break;
                            case 2:
                                
                                break;
                            case 3:
                                
                                break;
                            default:
                                
                                break;
                        
                        }
                        break;
                        
                    case 3:
                        switch (gear_mode) {
                            case 1:
                                
                                break;
                            case 2:
                                
                                break;
                            case 3:
                                
                                break;
                            default:
                                
                                break;
                        
                        }
                        break;
                }
                
        }
    }
    
    return;
}


void __interrupt(low_priority) L_ISR(void)
{
    
    
    if(PIR1bits.TMR1IF == 1) {
        TMR1H = 0x3c;
        TMR1L = 0xb0;
        cnt++;
        PIR1bits.TMR1IF = 0;
    }
   // process other interrupt sources here, if required
    return;
}


void __interrupt(high_priority) H_ISR(void) {
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        MyusartRead();
    }   
}
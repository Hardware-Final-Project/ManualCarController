#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"


char ch;

char read_flag = 0, check_gear_input_flag = 0;

int cnt = 0;


char delay500ms(int max_cnt) {
    if (cnt >= max_cnt) {
        return 1;
    }
    return 0;
}


void main(void) {
    
    SYSTEM_Initialize();
    
    unsigned int duty1 = 0;
    unsigned int duty2 = 0;
    
    char gear_mode = 1, prev_gear_mode = 1;
    
    CCPR1L = (duty1 >> 2) & 0xff;
    CCP1CONbits.DC1B = duty1 & 0x03;
    
    CCPR2L = (duty2 >> 2) & 0xff;
    CCP2CONbits.DC2B = duty2 & 0x03;
    
    
    
    while (1) {
        if (read_flag == 1) {
            if (sscanf(GetString(), "%u %u", &duty1, &duty2) == 2) {
                ClearBuffer();
                CCPR1L = (duty1 >> 2) & 0xff;
                CCP1CONbits.DC1B = duty1 & 0x03;

                CCPR2L = (duty2 >> 2) & 0xff;
                CCP2CONbits.DC2B = duty2 & 0x03;               
            }
            read_flag = 0;
        } else if (check_gear_input_flag == 1) {
            if (sscanf(GetString(), "g%hhd", &gear_mode) == 1) {
                switch (gear_mode) {
                    case 1:
                        switch (prev_gear_mode) {
                            case 1:
                                break;
                            case 2:
                                LATD6 = 0;
                                LATD7 = 1;
                                T2CONbits.TMR2ON = 1;
                                if (delay500ms(1)) {
                                    T2CONbits.TMR2ON = 0;
                                    LATD6 = 0;
                                    LATD7 = 0;
                                }
                                break;
                            case 3:
                                
                                break;
                            default:
                                
                                break;
                        
                        }
                        break;
                        
                    case 2:
                        switch (prev_gear_mode) {
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
                        switch (prev_gear_mode) {
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
                
                T1CONbits.TMR1ON = 1;
            }
            check_gear_input_flag = 0;
        }
    }
    
    return;
}


void __interrupt(low_priority) Lo_ISR(void)
{
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
//        ch = RCREG; "g1"
        MyusartRead();
        if (getLenStr() == 7) {
            read_flag = 1;   
        } else if (getLenStr() == 2) {
            check_gear_input_flag = 1;
        }
        
    }
    
    if(PIR1bits.TMR1IF == 1) {
        TMR1H = 0x0b;
        TMR1L = 0xdc;
        cnt++;
        PIR1bits.TMR1IF = 0;
    }
   // process other interrupt sources here, if required
    return;
}
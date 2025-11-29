#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"


char ch;

char read_flag = 0;


void main(void) {
    
    SYSTEM_Initialize();
    
    unsigned int duty1 = 400;
    unsigned int duty2 = 0;
    
    CCPR1L = (duty1 >> 2) & 0xff;
    CCP1CONbits.DC1B = duty1 & 0x03;
    
    CCPR2L = (duty2 >> 2) & 0xff;
    CCP2CONbits.DC2B = duty2 & 0x03;
    
    
    
    while (1) {
        if (read_flag == 1) {
            sscanf(GetString(), "%u %u", &duty1, &duty2);
            ClearBuffer();
            read_flag = 0;
            CCPR1L = (duty1 >> 2) & 0xff;
            CCP1CONbits.DC1B = duty1 & 0x03;

            CCPR2L = (duty2 >> 2) & 0xff;
            CCP2CONbits.DC2B = duty2 & 0x03;
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
        
//        ch = RCREG;
        MyusartRead();
        if (getLenStr() == 7) {
            read_flag = 1;   
        }
        
    }
    
    if(PIR1bits.TMR1IF == 1) {
        // for something
        LATD7 = 0;
        LATD6 = 0;
        PIR1bits.TMR1IF = 0;
    }
   // process other interrupt sources here, if required
    return;
}
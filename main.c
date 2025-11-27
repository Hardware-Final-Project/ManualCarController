#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"


char ch;
char read_flag = 0;


void main(void) {
    
    SYSTEM_Initialize();
    
    while (1) {
        if (read_flag == 1) {
            LATD = (ch & 0x0f) << 4;
            ClearBuffer();
            read_flag = 0;
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
        
        ch = RCREG;
//        MyusartRead();
        read_flag = 1;
    }
    
   // process other interrupt sources here, if required
    return;
}
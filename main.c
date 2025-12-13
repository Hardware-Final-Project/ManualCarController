#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include <stdbool.h>
#include "stdio.h"
#include "string.h"

// 1->thruttle, 2->gear shifting, 3-> reset gear
char mode = 0;

char ch;

int cnt = 0;

unsigned int duty1 = 0;
unsigned int duty2 = 0;

char gear_mode = 1, prev_gear_mode = 1;

char direction = 0;

char reset_times = 0;
const int gear_shift_time_counter = 9;


char delay100ms(int max_cnt) {
    if (cnt >= max_cnt) {
        cnt = 0;
        return 1;
    }
    return 0;
}


void thruttle() {
    CCPR1L = (duty1 >> 2) & 0xff;
    CCP1CONbits.DC1B = duty1 & 0x03;

    CCPR2L = (duty2 >> 2) & 0xff;
    CCP2CONbits.DC2B = duty2 & 0x03;               
    
}


void gearShifting() {
    char oled_buffer[10];
    switch (prev_gear_mode) {
        case 1:
            switch (gear_mode) {
                case 1:
                    break;
                case 2:
                    LATD6 = 0;
                    LATD7 = 1;
                    T1CONbits.TMR1ON = 1;
                    while (!delay100ms(gear_shift_time_counter));
                    T1CONbits.TMR1ON = 0;
                    LATD6 = 0;
                    LATD7 = 0;
                    prev_gear_mode = 2;
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
                    LATD6 = 1;
                    LATD7 = 0;
                    T1CONbits.TMR1ON = 1;
                    while (!delay100ms(gear_shift_time_counter));
                    T1CONbits.TMR1ON = 0;
                    LATD6 = 0;
                    LATD7 = 0;
                    prev_gear_mode = 1;
                    break;
                case 2:

                    break;
                case 3:
                    LATD6 = 0;
                    LATD7 = 1;
                    T1CONbits.TMR1ON = 1;
                    while (!delay100ms(gear_shift_time_counter));
                    T1CONbits.TMR1ON = 0;
                    LATD6 = 0;
                    LATD7 = 0;
                    prev_gear_mode = 3;
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
                    LATD6 = 1;
                    LATD7 = 0;
                    T1CONbits.TMR1ON = 1;
                    while (!delay100ms(gear_shift_time_counter));
                    T1CONbits.TMR1ON = 0;
                    LATD6 = 0;
                    LATD7 = 0;
                    prev_gear_mode = 2;
                    break;
                case 3:

                    break;
                default:

                    break;

            }
            break;
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
    
    if (getLenStr() != 1) return;
    
    mode = GetString()[0];
    ClearBuffer();
    char oled_buffer[10];
    switch(mode) {
        case 'A'...'C':
            gear_mode = mode - 'A' + 1;
            SSD1306_SetCursor(1, 0);
            sprintf(oled_buffer, "Gear: %hhd", gear_mode);
            SSD1306_PutString(oled_buffer);
            gearShifting();
            break;
        // 0 ~ 500, -500 ~ 0
        case 'a'...'u':
            // a ~ k => 0~500
            // l ~ v => -500~0
            char speed_text ;
            if(mode >= 'a' && mode < 'k') {
                duty1 = (mode - 'a') * 50;
                duty2 = 0;
                speed_text = mode - 'a';
                sprintf(oled_buffer, "Speed: %hhd", speed_text);
            } else if(mode >= 'l' && mode < 'v') {
                duty1 = 0;
                duty2 = (mode - 'l') * 50;     
                speed_text = mode - 'l';
                sprintf(oled_buffer, "Speed: -%hhd", speed_text);
            }
            SSD1306_SetCursor(2, 0);
            SSD1306_PutString(oled_buffer);
            thruttle();
            break;
        case 'R':
            SSD1306_SetCursor(3,0);
            SSD1306_PutString("Reset Gear mode");
            resetGear();
            SSD1306_SetCursor(3,0);
            SSD1306_PutString("               ");
            break;
    }
    
    mode = -1;
}


void main(void) {
    
    SYSTEM_Initialize();
    
    
    
    
    CCPR1L = (duty1 >> 2) & 0xff;
    CCP1CONbits.DC1B = duty1 & 0x03;
    
    CCPR2L = (duty2 >> 2) & 0xff;
    CCP2CONbits.DC2B = duty2 & 0x03;
    
    
    SSD1306_PutString("Manual Car Controller");
    while (1) {
        executeCommand();
        if(getEnterFlag() == 1) ClearBuffer();
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

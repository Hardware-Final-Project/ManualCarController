#include "I2C.h"
#include "SSD1306.h"
// --- SSD1306 ???? ---

void SSD1306_Write(unsigned char mode, unsigned char byte) {
    I2C_Start();
    I2C_Write(SSD1306_ADDR); // ???? (Write)
    I2C_Write(mode);         // 0x00 ???, 0x40 ???
    I2C_Write(byte);
    I2C_Stop();
}

void SSD1306_Init(void) {
    __delay_ms(100); // ??????
    SSD1306_Write(CMD_MODE, 0xAE); // Display OFF
    SSD1306_Write(CMD_MODE, 0x20); // Set Memory Addressing Mode
    SSD1306_Write(CMD_MODE, 0x02); // 0x02, Page Addressing Mode 
    SSD1306_Write(CMD_MODE, 0xB0); // Set Page Start Address for Page Addressing Mode,0-7
    SSD1306_Write(CMD_MODE, 0xC8); // Set COM Output Scan Direction
    SSD1306_Write(CMD_MODE, 0x00); // Set low column address
    SSD1306_Write(CMD_MODE, 0x10); // Set high column address
    SSD1306_Write(CMD_MODE, 0x40); // Set start line address
    SSD1306_Write(CMD_MODE, 0x81); // set contrast control
    SSD1306_Write(CMD_MODE, 0xFF);
    SSD1306_Write(CMD_MODE, 0xA1); // set segment re-map 0 to 127
    SSD1306_Write(CMD_MODE, 0xA6); // set normal display
    SSD1306_Write(CMD_MODE, 0xA8); // set multiplex ratio(1 to 64)
    SSD1306_Write(CMD_MODE, 0x3F); //
    SSD1306_Write(CMD_MODE, 0xA4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    SSD1306_Write(CMD_MODE, 0xD3); // -set display offset
    SSD1306_Write(CMD_MODE, 0x00); // -not offset
    SSD1306_Write(CMD_MODE, 0xD5); // set display clock divide ratio/oscillator frequency
    SSD1306_Write(CMD_MODE, 0xF0); // set divide ratio
    SSD1306_Write(CMD_MODE, 0xD9); // set pre-charge period
    SSD1306_Write(CMD_MODE, 0x22); //
    SSD1306_Write(CMD_MODE, 0xDA); // set com pins hardware configuration
    SSD1306_Write(CMD_MODE, 0x12);
    SSD1306_Write(CMD_MODE, 0xDB); // set vcomh
    SSD1306_Write(CMD_MODE, 0x20); // 0x20,0.77xVcc
    SSD1306_Write(CMD_MODE, 0x8D); // set charge pump enable/disable
    SSD1306_Write(CMD_MODE, 0x14); // set(0x10) disable
    SSD1306_Write(CMD_MODE, 0xAF); // turn on SSD1306 panel
}

void SSD1306_Clear(void) {
    unsigned char i, j;
    for (i = 0; i < 8; i++) {
        SSD1306_Write(CMD_MODE, 0xB0 + i); // ?? Page (0-7)
        SSD1306_Write(CMD_MODE, 0x00);     // ?? Column ??
        SSD1306_Write(CMD_MODE, 0x10);     // ?? Column ??
        
        for (j = 0; j < 128; j++) {
            SSD1306_Write(DATA_MODE, 0x00); // ?? 0x00 ????
        }
    }
}

// ??????
void SSD1306_SetCursor(unsigned char page, unsigned char col) {
    if (page > 7) page = 7;
    if (col > 127) col = 127;

    SSD1306_Write(CMD_MODE, 0xB0 + page);             
    SSD1306_Write(CMD_MODE, 0x00 | (col & 0x0F));      
    SSD1306_Write(CMD_MODE, 0x10 | ((col >> 4) & 0x0F)); 
}

// ??????
// ???? unsigned char ??????
void SSD1306_PutChar(unsigned char c) {
    if (c < 32 || c > 126) return; // ???????

    unsigned char i;
    unsigned char char_index = c - 32; 

    for (i = 0; i < 5; i++) {
        SSD1306_Write(DATA_MODE, font5x7[char_index][i]);
    }
    SSD1306_Write(DATA_MODE, 0x00); // ??
}

// ????
void SSD1306_PutString(char *s) {
    // ??????????? *s???????? \0
    // ??? while(s) ???????????? 0???????????????
    while (*s) {
        SSD1306_PutChar(*s);
        s++;
    }
}

void SSD1306_Fill(void) {
    unsigned char i, j;
    for (i = 0; i < 8; i++) {
        SSD1306_Write(CMD_MODE, 0xB0 + i); 
        SSD1306_Write(CMD_MODE, 0x00); 
        SSD1306_Write(CMD_MODE, 0x10); 
        
        for (j = 0; j < 128; j++) {
            SSD1306_Write(DATA_MODE, 0xFF); 
        }
    }
}

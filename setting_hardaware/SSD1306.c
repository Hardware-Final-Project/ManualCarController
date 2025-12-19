#include "I2C.h"
#include "SSD1306.h"

// --- SSD1306 Driver Functions ---

void SSD1306_Write(unsigned char mode, unsigned char byte) {
    I2C_Start();
    I2C_Write(SSD1306_ADDR); // Device Address (Write)
    I2C_Write(mode);         // 0x00 for Command, 0x40 for Data
    I2C_Write(byte);
    I2C_Stop();
}

void SSD1306_Init(void) {
    __delay_ms(100); // Wait for power stabilization
    SSD1306_Write(CMD_MODE, 0xAE); // Display OFF
    SSD1306_Write(CMD_MODE, 0x20); // Set Memory Addressing Mode
    SSD1306_Write(CMD_MODE, 0x02); // 0x02, Page Addressing Mode 
    SSD1306_Write(CMD_MODE, 0xB0); // Set Page Start Address for Page Addressing Mode, 0-7
    SSD1306_Write(CMD_MODE, 0xC8); // Set COM Output Scan Direction
    SSD1306_Write(CMD_MODE, 0x00); // Set low column address
    SSD1306_Write(CMD_MODE, 0x10); // Set high column address
    SSD1306_Write(CMD_MODE, 0x40); // Set start line address
    SSD1306_Write(CMD_MODE, 0x81); // Set contrast control
    SSD1306_Write(CMD_MODE, 0xFF);
    SSD1306_Write(CMD_MODE, 0xA1); // Set segment re-map 0 to 127
    SSD1306_Write(CMD_MODE, 0xA6); // Set normal display
    SSD1306_Write(CMD_MODE, 0xA8); // Set multiplex ratio (1 to 64)
    SSD1306_Write(CMD_MODE, 0x3F); //
    SSD1306_Write(CMD_MODE, 0xA4); // 0xA4, Output follows RAM content; 0xA5, Output ignores RAM content
    SSD1306_Write(CMD_MODE, 0xD3); // Set display offset
    SSD1306_Write(CMD_MODE, 0x00); // No offset
    SSD1306_Write(CMD_MODE, 0xD5); // Set display clock divide ratio/oscillator frequency
    SSD1306_Write(CMD_MODE, 0xF0); // Set divide ratio
    SSD1306_Write(CMD_MODE, 0xD9); // Set pre-charge period
    SSD1306_Write(CMD_MODE, 0x22); //
    SSD1306_Write(CMD_MODE, 0xDA); // Set COM pins hardware configuration
    SSD1306_Write(CMD_MODE, 0x12);
    SSD1306_Write(CMD_MODE, 0xDB); // Set VCOMH
    SSD1306_Write(CMD_MODE, 0x20); // 0x20, 0.77xVcc
    SSD1306_Write(CMD_MODE, 0x8D); // Set charge pump enable/disable
    SSD1306_Write(CMD_MODE, 0x14); // Set (0x10) disable, (0x14) enable
    SSD1306_Write(CMD_MODE, 0xAF); // Turn on SSD1306 panel
}

void SSD1306_Clear(void) {
    unsigned char i, j;
    for (i = 0; i < 8; i++) {
        SSD1306_Write(CMD_MODE, 0xB0 + i); // Set Page Address (0-7)
        SSD1306_Write(CMD_MODE, 0x00);     // Set Lower Column Start Address
        SSD1306_Write(CMD_MODE, 0x10);     // Set Higher Column Start Address
        
        for (j = 0; j < 128; j++) {
            SSD1306_Write(DATA_MODE, 0x00); // Write 0x00 to clear pixel
        }
    }
}

// Set Cursor Position
void SSD1306_SetCursor(unsigned char page, unsigned char col) {
    if (page > 7) page = 7;
    if (col > 127) col = 127;

    SSD1306_Write(CMD_MODE, 0xB0 + page);            // Set Page Address
    SSD1306_Write(CMD_MODE, 0x00 | (col & 0x0F));    // Set Lower Column Start Address
    SSD1306_Write(CMD_MODE, 0x10 | ((col >> 4) & 0x0F)); // Set Higher Column Start Address
}

// Display a single character
// Takes an unsigned char as input
void SSD1306_PutChar(unsigned char c) {
    if (c < 32 || c > 126) return; // Check for valid ASCII range

    unsigned char i;
    unsigned char char_index = c - 32; 

    for (i = 0; i < 5; i++) {
        SSD1306_Write(DATA_MODE, font5x7[char_index][i]);
    }
    SSD1306_Write(DATA_MODE, 0x00); // Add spacing between characters
}

// Display a string
void SSD1306_PutString(char *s) {
    // Loop through the string until the null terminator '\0' is reached
    // We check the content *s, not the pointer address s
    while (*s) {
        SSD1306_PutChar(*s);
        s++;
    }
}

void SSD1306_Fill(void) {
    unsigned char i, j;
    for (i = 0; i < 8; i++) {
        SSD1306_Write(CMD_MODE, 0xB0 + i); // Set Page Address
        SSD1306_Write(CMD_MODE, 0x00);     // Set Lower Column Start Address
        SSD1306_Write(CMD_MODE, 0x10);     // Set Higher Column Start Address
        
        for (j = 0; j < 128; j++) {
            SSD1306_Write(DATA_MODE, 0xFF); // Write 0xFF to fill pixel (turn on)
        }
    }
}
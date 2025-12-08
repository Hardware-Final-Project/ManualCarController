#include "./I2C.h"
// --- I2C ???? ---


void I2C_Init(void) {
    TRISCbits.TRISC3 = 1; // SCL
    TRISCbits.TRISC4 = 1; // SDA
    
    SSPSTAT = 0x80;       // Slew rate disabled (Standard Speed mode 100kHz & 1MHz)
    SSPCON1 = 0x28;       // Enable SSP, Master mode
    SSPCON2 = 0x00;
    
    // ?? I2C ???? (Fosc / (4 * (SSPADD + 1)))
    // ?? Fosc = 20MHz??? I2C = 100kHz
    // 100,000 = 20,000,000 / (4 * (SSPADD + 1))
    // 4 * (SSPADD + 1) = 200
    // SSPADD + 1 = 50
    // SSPADD = 49 (0x31)
    SSPADD = 9;          
}

void I2C_Wait(void) {
    // I2C 
    while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));
}

void I2C_Start(void) {
    I2C_Wait();
    SSPCON2bits.SEN = 1;  // Start
}

void I2C_Stop(void) {
    I2C_Wait();
    SSPCON2bits.PEN = 1; // Stop
}

void I2C_Write(unsigned char data) {
    I2C_Wait();
    SSPBUF = data;
}
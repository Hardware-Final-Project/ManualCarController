#include "./I2C.h"

// --- I2C Driver Functions ---
void I2C_Init(void) {
    TRISCbits.TRISC3 = 1; // Configure SCL pin as input
    TRISCbits.TRISC4 = 1; // Configure SDA pin as input

    SSPSTAT = 0x80;       // Disable Slew Rate Control (Standard Speed mode 100kHz & 1MHz)
    SSPCON1 = 0x28;       // Enable SSP module, Configure as I2C Master mode
    SSPCON2 = 0x00;       // Clear MSSP Control Register 2

    // Calculate I2C Baud Rate formula: (Fosc / (4 * (SSPADD + 1)))
    // Assumption: Fosc = 4MHz, Target I2C Clock = 100kHz
    // 100,000 = 4,000,000 / (4 * (SSPADD + 1))
    // 4 * (SSPADD + 1) = 40
    // SSPADD + 1 = 10
    // SSPADD = 9 (0x09)
    SSPADD = 9;           // Set Baud Rate Generator value
}

void I2C_Wait(void) {
    // Wait for the I2C bus to be idle
    // Loops while any Sequence Enable bit (Start, Stop, Ack, etc.) is set
    // or while a Transmit is in progress (R_W bit)
    while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));
}

void I2C_Start(void) {
    I2C_Wait();           // Ensure bus is idle
    SSPCON2bits.SEN = 1;  // Initiate Start condition
}

void I2C_Stop(void) {
    I2C_Wait();           // Ensure bus is idle
    SSPCON2bits.PEN = 1;  // Initiate Stop condition
}

void I2C_Write(unsigned char data) {
    I2C_Wait();           // Ensure bus is idle
    SSPBUF = data;        // Load data into buffer to start transmission
}
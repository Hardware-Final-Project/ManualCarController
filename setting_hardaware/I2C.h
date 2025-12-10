#ifndef _I2C_H
#define _I2C_H


#include <xc.h>


#define CMD_MODE      0x00
#define DATA_MODE     0x40

void I2C_Init();
void I2C_Wait();
void I2C_Start();
void I2C_Stop();
void I2C_Write(unsigned char data);

#endif
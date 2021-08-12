/*
 * File:   i2c.c
 * Author: Guille Lam
 *
 * Created on 11 de agosto de 2021, 01:35 PM
 */


#include "i2c.h"

void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}


void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start()
{
    I2C_Master_Wait();      
    SSPCON2bits.SEN = 1;    
}


void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      
    SSPCON2bits.RSEN = 1;   
}

void I2C_Master_Stop()
{
    I2C_Master_Wait();      
    SSPCON2bits.PEN = 1;    
}

void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      
    SSPBUF = d;             
}

unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      
    temp = SSPBUF;
    I2C_Master_Wait();      
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          
    return temp;                    
}

void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}
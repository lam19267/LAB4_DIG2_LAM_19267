/* 
 * File:   i2c.h
 * Author: Guille Lam
 *
 * Created on 11 de agosto de 2021, 01:40 PM
 */

#ifndef I2C_H
#define	I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif




void I2C_Master_Init(const unsigned long c);

void I2C_Master_Wait(void);

void I2C_Master_Start(void);

void I2C_Master_RepeatedStart(void);

void I2C_Master_Stop(void);

void I2C_Master_Write(unsigned d);

unsigned short I2C_Master_Read(unsigned short a);

void I2C_Slave_Init(uint8_t address);


#endif	/* I2C_H */


/* 
 * File:   adc.h
 * Author: Guille Lam
 *
 * Created on 11 de agosto de 2021, 01:33 PM
 */

#ifndef ADC_H
#define	ADC_H

#pragma config FOSC = INTRC_NOCLKOUT
#include <xc.h>

void ADC_config(char frec); 

#endif	/* ADC_H */


/*
 * File:   adc.c
 * Author: Guille Lam
 *
 * Created on 11 de agosto de 2021, 01:40 PM
 */


#include <xc.h> 
#include <stdint.h>  
#include "adc.h"

#define _XTAL_FREQ 4000000


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h> 
#include "adc.h"
#define _XTAL_FREQ 4000000

void ADC_config(char frec) { 
    switch(frec){
        case 0: //FOSC/2
            ADCON0bits.CHS = 0;     
            __delay_us(100);
    
            ADCON0bits.ADCS0 = 0;   
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;    
            ADCON1bits.VCFG1 = 0;   
            ADCON1bits.VCFG0 = 0;   
            break;
        case 1: //FOSC/8
            ADCON0bits.CHS = 0;    
            __delay_us(100);

            ADCON0bits.ADCS0 = 1;   
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;   
            ADCON1bits.VCFG1 = 0;  
            ADCON1bits.VCFG0 = 0;   
            break;
        case 2: //FOSC/32
            ADCON0bits.CHS = 0;    
            __delay_us(100);

            ADCON0bits.ADCS0 = 0;  
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;    
            ADCON1bits.VCFG1 = 0;   
            ADCON1bits.VCFG0 = 0;   
            break;
        case 3: //FRC
            ADCON0bits.CHS = 0;    
            __delay_us(100);

            ADCON0bits.ADCS0 = 1;   
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;    
            ADCON1bits.VCFG1 = 0;   
            ADCON1bits.VCFG0 = 0; 
            break;
    }
    return;
}
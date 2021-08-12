/*
 * File:   esclavo14.c
 * Author: Guille Lam
 *
 * Created on 11 de agosto de 2021, 01:41 PM
 */



//                            Librerias 


#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf

// Librerias propias

#include "adc.h"
#include "i2c.h"



//                            Bits de configuracion 

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



//                            Variables 


uint8_t z;
uint8_t dato, volt;


//                            Prototipos 


void setup(void);


//                            Interrupciones

void __interrupt() isr(void){
   
    // Interrupcion del ADC
    
       if(PIR1bits.ADIF == 1)       
       {
           if(ADCON0bits.CHS == 0) { 
               PORTB = ADRESH;
               volt = ADRESH;
//               ADCON0bits.CHS = 1;
//               PIR1bits.ADIF = 0;
           }
           PIR1bits.ADIF = 0;
       }
    
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 
            SSPCONbits.SSPOV = 0;      
            SSPCONbits.WCOL = 0;       
            SSPCONbits.CKP = 1;         
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                
            //__delay_us(2);
            PIR1bits.SSPIF = 0;        
            SSPCONbits.CKP = 1;         
            while(!SSPSTATbits.BF);    
            volt = SSPBUF;            
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = volt;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

//                            Main


void main(void) {
    setup();

    while(1){
        
        if(ADCON0bits.GO == 0){
            __delay_ms(100);
            ADCON0bits.GO = 1;
            
             }
    }
    return;
}

//                            Funciones


void setup(void){
    ANSEL = 0b00000001;
    ANSELH = 0;
    
    TRISAbits.TRISA0 = 1;
    TRISB = 0;
    
    //limpiar puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Configurar reloj interno
    OSCCONbits.IRCF0 = 0;        
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1; 
    
    //configuracion de interrupciones
    PIE1bits.ADIE = 1;     
    PIR1bits.ADIF = 0;      
    INTCONbits.GIE = 1;     
    INTCONbits.PEIE = 1;   
    
    // Configuracion del ADC
    ADC_config(2);
    
    // I2C configuracion esclavo
    I2C_Slave_Init(0x50);   
    
}

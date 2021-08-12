/*
 * File:   lab4.c
 * Author: Guille Lam
 *
 * Created on 11 de agosto de 2021, 01:38 PM
 */



//                            Librerias 
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>

// Librerias propias
#include "i2c.h"
#include "lcd.h"




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

#define RS RE0
#define EN RE1
#define D4 RC2
#define D5 RC1
#define D6 RC0
#define D7 RC5



//                            Variables 

int full;
volatile char temperature = 0;
volatile uint8_t pot = 0;
char adc0[10];
char adc1[10];
char cont[10];
float count = 0;
float conv0 = 0;
float conv1 = 0;
char converted, converted02;
char valor, hundreds, residuo, tens, units;


//                            Prototipos 


void setup(void);
void ADC_convert(char *data,float a, int place); // converter
char division (char valor);

//                            Interrupciones

void __interrupt() isr(void)
{
     
}


//                            Main

void main(void) {
    
    setup();    
    Lcd_Init();
    Lcd_Clear(); 
    
    while(1)    
    {
        
        
        
        I2C_Master_Start();
        I2C_Master_Write(0x51); 
        PORTB = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);
      
        I2C_Master_Start();
        I2C_Master_Write(0x80); 
        I2C_Master_Write(0xF3); 
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x81); 
        PORTD = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x91); 
        PORTA = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);
        
        count = PORTA;
        pot = PORTB;
        temperature = PORTD;

 
    Lcd_Clear();
    Lcd_Set_Cursor(1,2);
    Lcd_Write_String("S1");
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("S2");
    Lcd_Set_Cursor(1,14);
    Lcd_Write_String("S3");
    __delay_ms(1000);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(adc0);
    Lcd_Set_Cursor(2,4);
    Lcd_Write_String("V");
    Lcd_Set_Cursor(2,7);
    Lcd_Write_String(cont);
    Lcd_Set_Cursor(2,11);
    Lcd_Write_String("'");
    Lcd_Set_Cursor(2,14);
    Lcd_Write_String(adc1);
    __delay_ms(2000);
    
//    converted =  ((175.72 * temperature)/65536) - 46.85;
    converted = temperature - 77 ;
  
    conv0 = 0;

      
    conv0 = (pot / (float) 255)*5; //Se pasa a voltaje
                                         
    ADC_convert(adc0, conv0, 2);//se convierte el valor actual a un valor ASCII.


    ADC_convert(adc1, converted, 2);
    
    converted02 = division(count);    
    ADC_convert(cont, converted02, 2); 
    
    }
    return;
}

//-----------------------------------------------------------------------------
//                            Funciones
//-----------------------------------------------------------------------------

void setup(void){
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    
    TRISB = 0;
    TRISD = 0;
    TRISC = 0;
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    

    
    //limpiar puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
//    //Configurar reloj interno
    OSCCONbits.IRCF0 = 0;        //reloj interno de 4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  //internal oscillator is used for system clock

 
    // I2C configuracion Maestro
    I2C_Master_Init(100000);        // Inicializar Comuncaci?n I2C
}


 * Funciones para conversión del ADC 
 
void ADC_convert(char *data,float a, int place) 
{
     int temp=a;
     float x=0.0;
     int digits=0;
     int i=0,mu=1;
     int j=0;
     if(a<0)
     {
            a=a*-1;
            data[i]='-';
            i++;
      }
     //exponent component
     while(temp!=0)
     {
         temp=temp/10;
         digits++;          
     }
     while(digits!=0)
     {
         if(digits==1)mu=1;
         else  for(j=2;j<=digits;j++)mu=mu*10;
         
         x=a/mu;
         a=a-((int)x*mu);
         data[i]=0x30+((int)x);
         i++;
         digits--;
         mu=1;
     }
     //mantissa component
     data[i]='.';
     i++;
     digits=0;
     for(j=1;j<=place;j++)mu=mu*10;
     x=(a-(int)a)*mu; //shift places
     a=x;
     temp=a;
     x=0.0;
     mu=1;
     digits=place;
     while(digits!=0)
     {
         if(digits==1)mu=1;
         else  for(j=2;j<=digits;j++)mu=mu*10;
         
         x=a/mu;
         a=a-((int)x*mu);
         data[i]=0x30+((int)x);
         i++;
         digits--;
         mu=1;
     }   
     
    data[i]='\n';
}

char division (char valor){
    hundreds = valor/100;
    residuo = valor%100; 
    tens = residuo/10; 
    units = residuo%10;
}

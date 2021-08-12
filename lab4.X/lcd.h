/* 
 * File:   lcd.h
 * Author: Guille Lam
 *
 * Created on 11 de agosto de 2021, 01:36 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif

#ifndef D4
#define D4 RC2
#endif

#ifndef D5
#define D5 RC1
#endif

#ifndef D6
#define D6 RC0
#endif

#ifndef D7
#define D7 RC5
#endif

#include <xc.h> // include processor files - each processor file is guarded.  

//LCD Functions Developed by electroSome


void Lcd_Port(char a);

void Lcd_Cmd(char a);

void Lcd_Clear(void);

void Lcd_Set_Cursor(char a, char b);

void Lcd_Init(void);

void Lcd_Write_Char(char a);

void Lcd_Write_String(char *a);

void Lcd_Shift_Right(void);

void Lcd_Shift_Left(void);

#endif	/* LCD_H */


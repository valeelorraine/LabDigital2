/* File: LCD.c  
 * Se utilizaron y adaptaron las librerias de Ligo George, tomadas del siguiente
 * Link: https://electrosome.com/lcd-pic-mplab-xc8/
 * Las funciones fueron desarrolladas por electrosome
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#include "LCD.h"                       // Nombre de la librería

//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************

void Lcd_Port(char a) {
    PORTD = a;                         // Utilizar valor del puerto D
}

void Lcd_Cmd(char a) {
    RS = 0;                            // Puerto para comando
    Lcd_Port(a);
    EN = 1;                            // Activar el enable para realizar comando
    __delay_ms(4);                     // Delay 
    EN = 0;                            // => E = 0
}

void Lcd_Clear(void) {                 // Función para limpiar la LCD
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b){   // Función para el cursor
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;               // Linea 1
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0xF0;               // Linea 2
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
}

void Lcd_Init(void) {
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30);                     //Iniciando en 8 bits
    __delay_ms(5);
    Lcd_Cmd(0x30); 
    __delay_us(200);
    Lcd_Cmd(0x30); 
    Lcd_Cmd(0x3C);                     // Lines and character font
    //Lcd_Cmd(0x02);                   //
    Lcd_Cmd(0x08);                     // Display off
    //Lcd_Cmd(0x00);                   //
    Lcd_Cmd(0x01);                     // Display clear
    //Lcd_Cmd(0x00);                   //
    Lcd_Cmd(0x06);                     //Entry mode set
}

void Lcd_Write_Char(char a) {          // Función para esrcibir en la LCD
    char temp, y; 
    temp = a;                          //8bits 
    y = temp;
    RS = 1;                            // => RS = 1 Activar lectura de caracteres
    Lcd_Port(y);                       // Escribir lo que esta en puerto D
    EN = 1;                            // Encender enable
    __delay_us(40);                    // Delay de 40
    EN = 0;
}

void Lcd_Write_String(char *a) {       // Función para escribir un caracter
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right(void) {           // Función para correr a la derecha
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {            // Función para correr a la derecha
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}
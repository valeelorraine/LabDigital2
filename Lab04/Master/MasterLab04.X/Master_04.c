/* 
 * File:  Laboratorio 4
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: Comunicaión I2C
 * 
 * Hardware: 3 PIC16F887, potenciómetros, 8 LEDS
 * 
 * Created on 28 de julio de 2021
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#include <xc.h>
#include <stdint.h>                // Librería para variables de ancho definido
#include <stdio.h>                 // Para el sprintf
#include <stdlib.h>
#include <string.h>
#include "I2C.h"                   // Librería del I2C
#include "LCD_4.h"                 // Libreria de la LCD

//******************************************************************************
//                      C O N F I G U R A C I Ó N 
//******************************************************************************

// PIC16F887 Configuration Bit Settings
#pragma config FOSC=INTRC_NOCLKOUT // Oscillador interno I/O RA6
#pragma config WDTE=OFF            // WDT disabled (reinicio rep. del pic)
#pragma config PWRTE=OFF           // Power-up Timer (PWRT disabled)
#pragma config MCLRE=OFF           // El pin de MCLR se utiliza como I/O
#pragma config CP=OFF              // Sin protección de código
#pragma config CPD=OFF             // Sin protección de datos

#pragma config BOREN=OFF           // No reinicia cuándo Vin baja de 4v
#pragma config IESO=OFF            // Reinicio sin cambio de reloj inter-exter.
#pragma config FCMEN=OFF           // Cambio de reloj exter-inter en caso falla
#pragma config LVP=OFF             // Progra en bajo voltaje permitido

// CONFIG2
#pragma config BOR4V = BOR40V      // Reinicio a bajo de 4v, (BOR21V=2.1V)
#pragma config WRT = OFF           // Protección de autoescritura x prog. desact.

//******************************************************************************
//             D I R E C T I V A S  del  C O M P I L A D O R
//******************************************************************************
#define _XTAL_FREQ 4000000         // Frecuencia del oscilador
#define RS RD2                     // Definir las variables como nombres de los
#define EN RD3                     // pines
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
unsigned char VOL; 
unsigned char un;
unsigned char dec; 
unsigned char cen;
unsigned char POT;
unsigned char TEMP;
uint8_t sensor1;
uint8_t sensor2;
uint16_t temp;                   // Se utilizan dos dos bytes
float temp1;                     // Porque recibira valores en decimal para el mapeo

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);
void decimal(uint8_t var);
void Temperatura(uint8_t var);
void voltaje(uint8_t var);

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************
void setup(void){
    // CONFIGURACIÓN DE LOS PUERTOS
    ANSELH = 0x00;                  // Puerto B digital
    TRISD = 0X00;                   // Puertos digitales
    TRISE = 0x00;

    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;                   
    PORTC = 0X00;
    PORTD = 0X00;
    
    // Configuración del oscilador
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 4Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 0;
    
    // Inicializar comunicacion I2C con libreria
    I2C_Master_Init(100000);        // Frecuencia recomendada tipica
 }
//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){
    setup();                       // Inicializar el setup
    Lcd_Init();                    // Inicializar LCD
    Lcd_Clear();                   // Limpiar LCD
    Lcd_Set_Cursor(1,1);           // Cursor  uno para la primera posicion
    Lcd_Write_String("S1:   S2:   S3:");  // Mensaje a desplegar
    __delay_us(100);
    
    while(1){
        //Obtener informacion del segundo slave
        I2C_Master_Start();
        I2C_Master_Write(0x61); //51, el 1 para que lea
        sensor2 = I2C_Master_Read(0); //lo escribo en el puerto de leds
        I2C_Master_Stop();
        __delay_ms(200);
    
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        POT = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);

        //Temp
        I2C_Master_Start();
        I2C_Master_Write(0x80);    // Seleccionar el sensor y se escribe
        I2C_Master_Write(0xF3);    // Read temperature
        I2C_Master_Stop();         // Finalizar la obtención de información
        __delay_ms(200);

        I2C_Master_Start();         
        I2C_Master_Write(0x81);     // Para que ahora lea
        TEMP = I2C_Master_Read(0); // Read temperature
        I2C_Master_Stop();          // Finalizar la obtención de info.
        __delay_ms(200);

        //Potenciometro 1
        voltaje(POT);              //Convertir a decimales y a CHAR
        Lcd_Set_Cursor(2,1);        //Llevar el cursor a fila 2 primer espacio
        Lcd_Write_Char(cen);
        Lcd_Write_String(".");
        Lcd_Write_Char(dec);    //Enviar datos
        Lcd_Write_Char(un);
        Lcd_Write_String("  ");
        __delay_ms(200);
    
        //Escritura del contador
        Lcd_Set_Cursor(2,8);
        decimal(sensor2);     
        Lcd_Write_Char(dec);
        Lcd_Set_Cursor(2,9);
        Lcd_Write_Char(un);  
    
        //Contador 2
        Lcd_Set_Cursor(2,13);
        Temperatura(TEMP);
        Lcd_Write_Char(cen);
        Lcd_Write_Char(dec);    //Enviar datos
        Lcd_Write_Char(un);
        Lcd_Write_String("C ");
        __delay_ms(200);
        
        PORTB = POT;
        PORTA = TEMP;
        __delay_ms(200);
}
    }

//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************
void decimal(uint8_t var){        // Función para obtener valor decimal
    uint8_t VAL;
    VAL = var;                    // Guardar el valor del Puerto
    cen = (VAL/100) ;             // Obtener cen dividiendo dentro de 100
    VAL = (VAL - (cen*100));
    dec = (VAL/10);               // Obtener dec dividiendo dentro de 10
    VAL = (VAL - (dec*10));
    un = (VAL);                   // Obtener un dividiendo dentro de 1
    
    cen = cen + 48;               // Se realiza la suma por el 0 en ASCII
    dec = dec + 48;               // que es igual a 48 en decimal
    un = un + 48;
}

void voltaje(uint8_t var){        // Función para obtener valor decimal
    uint8_t VAL;
    VAL = var;                    // Guardar el valor del Puerto
    VAL = (VAL*1.96);             // convertir a Volt
    cen = (VAL/100) ;             // Obtener cen dividiendo dentro de 100
    VAL = (VAL - (cen*100));
    dec = (VAL/10);               // Obtener dec dividiendo dentro de 10
    VAL = (VAL - (dec*10));
    un = (VAL);                   // Obtener un dividiendo dentro de 1
    
    cen = cen + 48;               // Se realiza la suma por el 0 en ASCII
    dec = dec + 48;               // que es igual a 48 en decimal
    un = un + 48;
}

void Temperatura(uint8_t var){        // Función para obtener valor decimal
    uint8_t VAL;
    VAL = (0.7*var)-47;                    // Guardar el valor del Puerto
    cen = (VAL/100) ;             // Obtener cen dividiendo dentro de 100
    VAL = (VAL - (cen*100));
    dec = (VAL/10);               // Obtener dec dividiendo dentro de 10
    VAL = (VAL - (dec*10));
    un = (VAL);                   // Obtener un dividiendo dentro de 1
    
    cen = cen + 48;               // Se realiza la suma por el 0 en ASCII
    dec = dec + 48;               // que es igual a 48 en decimal
    un = un + 48;
}



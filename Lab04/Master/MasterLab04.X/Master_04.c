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
#include <pic16f887.h> 
#include "I2C.h"                   // Librería del I2C

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
#define _XTAL_FREQ 4000000

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
uint8_t VOL; 

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************

void setup(void){
    // CONFIGURACIÓN DE LOS PUERTOS
    ANSELH = 0x00;                  // Puerto B digital
    TRISA = 0X00;                   // Puertos digitales
    TRISB = 0x00;

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
    setup();
    while(1){
        I2C_Master_Start();         // Obtener informacion del primer slave
        I2C_Master_Write(0x51);     // 1 para que lea
        PORTB = I2C_Master_Read(0); // Puerto de leds
        I2C_Master_Stop();          // Finalizar la obtención de info
        __delay_ms(200);            // Delay
        
        // Información del sensor de temperatura
        I2C_Master_Start();
        I2C_Master_Write(0x80);    // Seleccionar el sensor y se escribe
        I2C_Master_Write(0xF3);    // Read temperature
        I2C_Master_Stop();         // Finalizar la obtención de información
        __delay_ms(200);
        
        I2C_Master_Start();         
        I2C_Master_Write(0x81);     // Para que ahora lea
        PORTA = I2C_Master_Read(0); // Read temperature
        I2C_Master_Stop();          // Finalizar la obtención de info.
        __delay_ms(200);
    }
}


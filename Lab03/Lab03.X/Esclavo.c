/* 
 * File:  Laboratorio 3
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: Comunicaión SPI
 * 
 * Hardware: 2 PIC16F887, potenciómetros, 8 LEDS
 * 
 * Created on 28 de julio de 2021
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#include <xc.h>
#include <stdint.h>                // Librería para variables de ancho definido
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "SPI.h"                   // Librería del SPI
#include "config_ADC.h"            // Libreria para configurar el ADC

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
#define _XTAL_FREQ 4000000         // Frecuencia de operación

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
uint8_t vol1 = 0;
uint8_t vol2 = 0;                // Variables para el voltaje
uint8_t temp1 = 0;
uint8_t temp2 = 0;              // Variables temporales

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                  // Configuraciones

//******************************************************************************
//                     F U N C I Ó N   para   I S R
//******************************************************************************
void __interrupt() isr(void){
//    if(PIR1bits.ADIF == 1){        // Obtener valor del potenciómetro
//        if(ADCON0bits.CHS == 0){   // Switcheo de canales
//            vol2 = ADRESH;         // Obtener el valor y guardarlo en vol2
//            }
//        else{
//            vol1 = ADRESH;         // Obtener el valor y guardarlo en vol1
//        }
//        PIR1bits.ADIF = 0;      // Limpiar bandera 
//    }
    if(PIR1bits.ADIF == 1){       // Obtener valor del potenciómetro
        if(ADCON0bits.CHS == 0){  // Switcheo de canales
            ADCON0bits.CHS = 1;
            vol2 = ADRESH;        // Obtener el valor y guardarlo en vol2
            }
        else if(ADCON0bits.CHS == 1){
            ADCON0bits.CHS = 0;
            vol1 = ADRESH;       // Obtener el valor y guardarlo en vol1
        }
    }
    PIR1bits.ADIF = 0;     // Limpiar bandera 
 
    if(SSPIF == 1){
        temp1 = spiRead();         // Leer el voltaje del pot 1
        spiWrite(vol1);            // Escribir el valor del voltaje del pot 1
        temp2 = spiRead();         // Leer el voltaje del pot 2
        spiWrite(vol2);            // Escribir el valor del voltaje del pot 2
        SSPIF = 0;
    }
}

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************
void setup(void){
    ANSEL = 0B00000011;            // Pines digitales en el puerto A
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA5 = 0;          // Slave Select
    TRISB = 0X00;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 0;
    
    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;
    PORTC = 0X00;
    PORTD = 0X00;
    PORTE = 0X00;

    //configuracion de interrupciones
    INTCONbits.GIE = 1;            // GIE Encender interrupción de global
    INTCONbits.PEIE = 1;           // PEIE 
    PIR1bits.SSPIF = 0;            // Limpiar bandera interrupción MSSP
    PIE1bits.SSPIE = 1;            // Habilitar interrupción MSSP
    
    //configuracion del esclavo
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, 
            SPI_IDLE_2_ACTIVE);

    config_ADC(1);                 // Configurar ADC
}
//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){
    setup();
    while(1){
        PORTB--;
        if (ADCON0bits.GO == 0){   // Activar la secuencia de lectura
            __delay_us(100);       // Delay
            ADCON0bits.GO = 1;
        }
    }
}                



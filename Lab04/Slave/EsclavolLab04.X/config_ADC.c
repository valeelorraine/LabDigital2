/* 
 * File:  Laboratorio 4
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * Descripción del programa: Comunicaión I2C
 *
 */

#include <xc.h>
#include <stdint.h>                 // Librería para variables de ancho definido
#include <pic16f887.h> 
#include "config_ADC.h"             // Libreria a utilizar
#define _XTAL_FREQ 4000000          // Frcuencia del Oscilador

void config_ADC(char frec) {        // Seleccionar la frecuencia ADC
    switch(frec){
        case 0:                     //FOSC/2
            ADCON0bits.CHS = 0;     // Canal 0
            __delay_us(100);
    
            ADCON0bits.ADCS0 = 0;   // FOSC
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    // ADC enable bit
            ADCON1bits.ADFM = 0;    // Left justified
            ADCON1bits.VCFG1 = 0;   // 5 voltios
            ADCON1bits.VCFG0 = 0;   // Tierra
            break;
        case 1: //FOSC/8
            ADCON0bits.CHS = 0;     // Canal 0
            __delay_us(100);

            ADCON0bits.ADCS0 = 1;   // FOSC/8
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    // ADC enable bit
            ADCON1bits.ADFM = 0;    // left justified
            ADCON1bits.VCFG1 = 0;   // 5 voltios
            ADCON1bits.VCFG0 = 0;   // Tierra
            break;
        case 2: //FOSC/32
            ADCON0bits.CHS = 0;     // Canal 0
            __delay_us(100);

            ADCON0bits.ADCS0 = 0;   // FOSC/32
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    // ADC enable bit
            ADCON1bits.ADFM = 0;    // left justified
            ADCON1bits.VCFG1 = 0;   // 5 voltios
            ADCON1bits.VCFG0 = 0;   // Tierra
            break;
        case 3: //FRC
            ADCON0bits.CHS = 0;     // Canal 0
            __delay_us(100);

            ADCON0bits.ADCS0 = 1;   // FRC
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    // ADC enable bit
            ADCON1bits.ADFM = 0;    // left justified
            ADCON1bits.VCFG1 = 0;   // 5 voltios
            ADCON1bits.VCFG0 = 0;   // Tierra
            break;
    }
}
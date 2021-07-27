/* 
 * File:   config_ADC.h
 * Author: Valerie Valdez
 *
 * Created on July 22, 2021, 12:15 AM
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#ifndef config_ADC_H
#define	config_ADC_H
#include <xc.h>
#include <stdint.h>

//******************************************************************************
//             D I R E C T I V A S  del  C O M P I L A D O R
//******************************************************************************
#define _XTAL_FREQ  4000000                // Frecuencia de operación


//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************
void initOsc(uint8_t frec){                // Función para las dif. frecuencias
    OSCCONbits.SCS = 1;                    // Oscilador interno
        switch(frec){
            case 8:                        // 8MHz
                OSCCONbits.IRCF0 = 1;
                OSCCONbits.IRCF1 = 1;      
                OSCCONbits.IRCF2 = 1;
                break;  
            case 4:                        // 4MHZ
                OSCCONbits.IRCF0 = 0;
                OSCCONbits.IRCF1 = 1;       
                OSCCONbits.IRCF2 = 1;
                break;
            case 2:                        // 2MHZ
                OSCCONbits.IRCF0 = 1;
                OSCCONbits.IRCF1 = 0;       
                OSCCONbits.IRCF2 = 1;
                break;
             case 1:                       // 1MHZ
                OSCCONbits.IRCF0 = 0;
                OSCCONbits.IRCF1 = 0;       
                OSCCONbits.IRCF2 = 1;
                break;
             case 50:                      // 500kHZ
                OSCCONbits.IRCF0 = 0;
                OSCCONbits.IRCF1 = 1;       
                OSCCONbits.IRCF2 = 1;
                break;
             case 25:                      // 250MHZ
                OSCCONbits.IRCF0 = 0;
                OSCCONbits.IRCF1 = 1;       
                OSCCONbits.IRCF2 = 0;
                break;
              case 12:                     // 125kHZ
                OSCCONbits.IRCF0 = 1;
                OSCCONbits.IRCF1 = 0;       
                OSCCONbits.IRCF2 = 0;
                break;
              case 31:                     // 31kHZ
                OSCCONbits.IRCF0 = 0;
                OSCCONbits.IRCF1 = 1;       
                OSCCONbits.IRCF2 = 1;
                break;
              default:                     // 4MHZ
                OSCCONbits.IRCF0 = 0;
                OSCCONbits.IRCF1 = 1;       
                OSCCONbits.IRCF2 = 1;
                break;
        }
}

void initAN(uint8_t bin, uint8_t just){   // Just. 0 a la izq. y 1 a la der. 
     ANSEL = bin;                         // Apagar analogo
     ANSELH = 0x00;
     TRISA = bin;
     
     ADCON0bits.CHS= 0;
     __delay_us(100);

     ADCON0bits.ADON = 1;                 // Activar modulo
     ADCON0bits.ADCS = 1;                 // ADC clock Fosc/8
     ADCON1bits.ADFM = just;              // Justificado izquierdo
     ADCON1bits.VCFG0 = 0;                // Referencia alta 
     ADCON1bits.VCFG1 = 0;                // Referencia baja
     
     PIE1bits.ADIE= 1;                    // Encender activar interrupcion del ADS
   }


void chselect (uint8_t cant){
    if(ADCON0bits.GO == 0){
      switch (ADCON0bits.CHS){
            
          case 0:
            if (cant == 1){
                ADCON0bits.CHS = 0;        // Si solo hay un canal activo 
            }                              // Se mantiene en 0
            else{
                ADCON0bits.CHS = 1;        // Cambiar a canal 1
            }
            break;
         case 1:
            if (cant == 2){
                ADCON0bits.CHS = 0;        // Si solo se usan dos canales
            }                              // Se regresa a 0
            else{
                ADCON0bits.CHS = 2;        // Cambiar a canal 2
            }
            break;
         case 2:
            if (cant == 3){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 3;         // Cambiar a canal 3
            }
            break;
         case 3:
            if (cant == 4){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 4;         // Cambiar a canal 4
            }
            break;
         case 4:
            if (cant == 5){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 5;         // Cambiar a canal 5
            }
            break;
         case 5:
            if (cant == 6){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 6;         // Cambiar a canal 1
            }
            break;
         case 6:
            if (cant == 7){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 7;         // Cambiar a canal 1
            }
            break;
         case 7:
            if (cant == 8){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 8;         // Cambiar a canal 1
            }
            break;
         case 8:
            if (cant == 9){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 9;         // Cambiar a canal 1
            }
            break;
         case 9:
            if (cant == 10){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 10;        // Cambiar a canal 1
            };
            break;
         case 10:
            if (cant == 11){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 11;        // Cambiar a canal 1
            }
            break;
         case 11:
            if (cant == 12){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 12;        //Cambiar a canal 1
            }
            break;
         case 12:
            if (cant == 13){
                ADCON0bits.CHS = 0;
            }
            else{
                ADCON0bits.CHS = 13;         // Cambiar a canal 1
            }
            break;
         case 13:
                ADCON0bits.CHS = 0;          // Reiniciar el switcheo
            break;
        }
    }   
    __delay_us(150);
    ADCON0bits.GO = 1;
}


uint8_t table(uint8_t val){
    uint8_t tempo;
    
    switch(val){
        case 0:
            tempo = 0b00111111;
            break;
        case 1:
            tempo = 0b00000110;
            break;
        case 2:
            tempo = 0b01011011;
            break;
        case 3:
            tempo = 0b01001111;
            break;
        case 4:
            tempo = 0b01100110;
            break;
        case 5:
            tempo = 0b01101101;
            break;
        case 6:
            tempo = 0b01111101;
            break;
        case 7:
            tempo = 0b00000111;
            break;
        case 8:
            tempo = 0b01111111;
            break;
        case 9:
            tempo = 0b01100111;
            break;
        case 10:    
            tempo = 0b01110111; // A
            break;
        case 11:
            tempo = 0b01111100; //b
            break;
        case 12: 
            tempo = 0b00111001; //C
            break;
        case 13: 
            tempo = 0b01011110; //d
            break;
        case 14:
            tempo = 0b01111001; //E
            break;
        case 15: 
            tempo = 0b01110001; //F
            break;
            default:
                tempo = 0b00111111;
        }
        return(tempo);
}
#endif	/* MY_LIB_H */

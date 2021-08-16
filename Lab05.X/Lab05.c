/* 
 * File:   Lab05 (Laboratorio No. 5)
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: PIC16F887 y ADAFRUIT
 * 
 * Hardware: 
 * 
 * Created on 22 de julio de 2021
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#include <xc.h>
#include <stdint.h>               // Librería para variables de ancho definido
#include <stdio.h>                // Usar el printf

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
#define _XTAL_FREQ      4000000    // Frecuencia de operación
#define _tmr0_value 100            // N de 100 para obtener un overflow de 5ms
//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
unsigned char FLAG = 0X00;
unsigned char FLAG1 = 0X00;

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                  // Configuraciones

//******************************************************************************
//                     F U N C I Ó N   para   I S R
//******************************************************************************
void __interrupt() isr(void){     
    if(T0IF == 1){                 // Bandera del TMR0 encendida
        TMR0 = _tmr0_value;        // Inicializar TMR0 
        INTCONbits.T0IF = 0;     // Apagar la bandera
    }
        
    if(INTCONbits.RBIF == 1){      // Interrupción del IOCB                    
        if(PORTBbits.RB0 == 0){    // Si RB0 se presionó y soltó, encender FLAG 
            FLAG = 1;}           
        else{
            if(FLAG == 1){         // Si la flag está encendida entonces apagarla
                FLAG = 0;        
                PORTA++;           // Incrementar el puerto
            }     
        }
        if(PORTBbits.RB1 == 0){    // Si RB1 se presionó y soltó, encender FLAGS
            FLAG1 = 1;}
        else{
            if(FLAG1 == 1){        // Si FLAGS está encendida entonces apagarla
                FLAG1 = 0;
                PORTA--;           // Decrementar el puerto
            }
        }
        INTCONbits.RBIF = 0;       // RBIF Limpiar la bandera de CHANGE INTERRUPT
    }
}
//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************

void setup(void){
    
    // CONFIGURACIÓN DE LOS PUERTOS
    ANSEL = 0X00;                  // Pines digitales en el puerto A
    ANSELH = 0B00001000;           // Puerto B digital
    
    TRISA = 0X00;                  // Puertos como outputs   
    TRISB = 0B00000011;
    TRISC = 0X00;
    TRISE = 0X00;
    
    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;
    PORTC = 0X00;
    PORTE = 0X00;
    
    // Configuración del oscilador, TMR0 y ADC
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 4Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 0;
    
    // WEAK PULL UP
    IOCB = 0B00000011;             // Habilitar lo del IOCB en pines RB0 y RB1
    WPUB = 0B00000011;
    
    // Configuraciones del módulo ADC
    ADCON0bits.ADON = 1;           // Encender el módulo
    ADCON0bits.CHS = 8;
   // __delay_us(100);             //Delay de 100
    
    // Configuración del TMR0, N = 100 y un overflow de 10ms
    OPTION_REG = 0B00000101;       // Prescaler 1:64
    TMR0 = _tmr0_value;            // Inicializar TMR0
    INTCONbits.GIE = 1;            // GIE Encender interrupción de global
    INTCONbits.PEIE = 1;           // PEIE 
    INTCONbits.T0IE = 1;           // T0IE Encender interrupción de OVERFLOW TMR0 
    INTCONbits.RBIE = 1;           // RBIE Encender interrupción PORTB CHANGE
    INTCONbits.T0IF = 0;           // Limpiar la bandera del overflow TMR0
    INTCONbits.RBIF = 0;           // RBIF Limpiar la bandera de CHANGE INTERRUPT
    }

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
  
  
  
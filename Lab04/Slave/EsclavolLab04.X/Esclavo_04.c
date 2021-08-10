/* 
 * File:  Laboratorio 4
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: Comunicaión I2C
 * 
 * Hardware: 2 PIC16F887, potenciómetros, 8 LEDS
 * 
 * Created on 7 de Agosto de 2021
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include <stdio.h>             // Libreria para mandar str en comunicacion serial
#include <pic16f887.h> 
#include "config_ADC.h"
#include "I2C.h"               // Librería del I2C

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
#define _XTAL_FREQ 4000000         // Frecuencia del reloj

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
uint8_t temp;          // Variables
unsigned char VOL;
unsigned char z;
//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);

//******************************************************************************
//                     F U N C I Ó N   para   I S R
//******************************************************************************
void __interrupt() isr(void){
    if(PIR1bits.ADIF == 1){         // Obtener valor del potenciómetro){
        if(ADCON0bits.CHS == 0){    // Switcheo de canales
            PORTB = ADRESH;         // Probar funcionamiento del ADC
            VOL = ADRESH;
        }
        PIR1bits.ADIF = 0;          // Limpiar bandera 
    }
    
    if(PIR1bits.SSPIF == 1){ 
        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;             // Leer valor previo para limpiar buffer
            SSPCONbits.SSPOV = 0;   // Limpiar bandera del overflow
            SSPCONbits.WCOL = 0;    // Limpiar collision bit
            SSPCONbits.CKP = 1;     // Utilizar el SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;             // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;     // Limpiar bandera de interr. recepción/transmisión SSP
            SSPCONbits.CKP = 1;     // Habilitar entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF); // Esperar a que la recepción se complete
            VOL = SSPBUF;       // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);   
            temp = SSPBUF;
        }
        
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = VOL;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;         // Limpiar bandera
    }
}

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************
void setup(void){
    // CONFIGURACIÓN DE LOS PUERTOS
    ANSEL = 0B00000001;            // Pines digitales en el puerto A           
    TRISAbits.TRISA0 = 1;          // Puertos como outputs 
    TRISB = 0x00; 
    
    //Inicializar puertos
    PORTA = 0X00;
    PORTB = 0x00;
    
    // Configuración del oscilador
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 4Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 0;
    
    //configuracion de interrupciones
    INTCONbits.GIE = 1;            // GIE Encender interrupción de global
    INTCONbits.PEIE = 1;           // PEIE 
    PIE1bits.ADIE = 1;             // ADIE Habilitar para comprobar FLAG -GF
    PIR1bits.ADIF = 0;             // Limpiar bandera de interrupción del ADC
    
    //Inicializar I2C en esclavo
    I2C_Slave_Init(0x50); //se le asigna esta direccion al primer esclavo
        
    config_ADC(1);                 //Configuracion de ADC con libreria
}

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){
    setup();
    
    while(1){
        //Volver a encender ADC
        if(ADCON0bits.GO == 0){
            __delay_ms(100);
            ADCON0bits.GO = 1;
        }
    }
}



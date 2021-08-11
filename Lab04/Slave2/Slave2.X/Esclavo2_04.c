/* 
 * File:  Laboratorio 4
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripci�n del programa: Comunicai�n I2C
 * 
 * Hardware: 3 PIC16F887, potenci�metros, 8 LEDS
 * 
 * Created on 28 de julio de 2021
 */

//******************************************************************************
//                           L I B R E R � A S
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>                 // Libreria para mandar str en comu. serial
#include <pic16f887.h> 
#include "I2C.h"                   // Librer�a del I2C

//******************************************************************************
//                      C O N F I G U R A C I � N 
//******************************************************************************

// PIC16F887 Configuration Bit Settings
#pragma config FOSC=INTRC_NOCLKOUT // Oscillador interno I/O RA6
#pragma config WDTE=OFF            // WDT disabled (reinicio rep. del pic)
#pragma config PWRTE=OFF           // Power-up Timer (PWRT disabled)
#pragma config MCLRE=OFF           // El pin de MCLR se utiliza como I/O
#pragma config CP=OFF              // Sin protecci�n de c�digo
#pragma config CPD=OFF             // Sin protecci�n de datos

#pragma config BOREN=OFF           // No reinicia cu�ndo Vin baja de 4v
#pragma config IESO=OFF            // Reinicio sin cambio de reloj inter-exter.
#pragma config FCMEN=OFF           // Cambio de reloj exter-inter en caso falla
#pragma config LVP=OFF             // Progra en bajo voltaje permitido

// CONFIG2
#pragma config BOR4V = BOR40V      // Reinicio a bajo de 4v, (BOR21V=2.1V)
#pragma config WRT = OFF           // Protecci�n de autoescritura x prog. desact.

//******************************************************************************
//             D I R E C T I V A S  del  C O M P I L A D O R
//******************************************************************************
#define _XTAL_FREQ 4000000         // Frecuencia del oscilador

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
unsigned char contador;            // Variable para el contador
unsigned char z; 
uint8_t inI2C; 

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                  // Inicializaci�n de las configuraciones

//******************************************************************************
//                     F U N C I � N   para   I S R
//******************************************************************************
void __interrupt() isr(void){
     if(INTCONbits.RBIF == 1){     // Si RBIF se enciende...                     
        if(PORTBbits.RB0 == 0){    // Si RB0 se presion� y solt�, inc. PORTD
            contador++;    
        }
        if(PORTBbits.RB1 == 0){    // Si RB1 se presion� y solt�, dec PORTD
            contador--; 
        }
        INTCONbits.RBIF = 0;       // RBIF Limpiar la bandera de CHANGE INTERRUPT
    }
    
    if(PIR1bits.SSPIF == 1){ 
        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;             // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;   // Clear the overflow flag
            SSPCONbits.WCOL = 0;    // Clear the collision bit
            SSPCONbits.CKP = 1;     // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;             // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;     // Limpia bandera de interrupci�n recepci�n/transmisi�n SSP
            SSPCONbits.CKP = 1;     // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF); // Esperar a que la recepci�n se complete
            inI2C = SSPBUF;      // Guardar en el PORTD el valor del buffer de recepci�n
            __delay_us(250);
            }
        
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;             // Variable temporal
            BF = 0;
            SSPBUF = contador;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);           
        }
        PIR1bits.SSPIF = 0;         // Limpiar bandera
    }
}

//******************************************************************************
//                      C O N F I G U R A C I � N
//******************************************************************************
void setup(void){
    // CONFIGURACI�N DE LOS PUERTOS
    ANSELH = 0x00;
    TRISA = 0X00;
    TRISB = 0B00000011;
    TRISCbits.TRISC3 = 0; 
    TRISCbits.TRISC4 = 0; 
    TRISD = 0X00;
    
    PORTA = 0X00;
    PORTB = 0x00;
    PORTC = 0X00;
    PORTD = 0X00;

    //configuracion de internal pullups y iocb
    OPTION_REGbits.nRBPU = 0;      // Internal pull-ups are enabled
    IOCBbits.IOCB0 = 1;            // Boton de inc
    IOCBbits.IOCB1 = 1;            // Boton de dec
    WPUB = 0B00000011;
    
    INTCONbits.GIE = 1;            // GIE Encender interrupci�n de global
    INTCONbits.PEIE = 1;           // PEIE 
    INTCONbits.RBIE = 1;           // RBIE Encender interrupci�n PORTB CHANGE
    INTCONbits.RBIF = 0;           // RBIF Limpiar la bandera de CHANGE INTERRUPT
    
    // Configuraci�n del oscilador
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 4Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 0;
    
    //Inicializar I2C en esclavo
    I2C_Slave_Init(0x60);          // Asignar direccion al primer esclavo
}

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){
    setup();
    while(1){
        PORTD = contador;          // Colocarle el valor del PUERTO a la var.
    }
}




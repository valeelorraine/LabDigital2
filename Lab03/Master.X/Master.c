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
#include <stdio.h>                 // Para el sprintf
#include <stdlib.h>
#include <string.h>
#include <pic16f887.h>
#include "config_USART.h"          // Libreria para configurar USART
#include "SPI.h"                   // Librería del SPI

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
uint8_t vol_a, vol_b;
uint8_t cen, dec, un, residuo;
uint8_t c1, d1, u1;
char s1[10];
char s2[10];
char s3[10];

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                  // Configuraciones
void putch(char dato);
void mensaje1(void);
void mensaje2(void);
void division(char dividendo);
void precaucion(void);
void precaucion1(void);
void precaucion2(void);

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************
void setup(void){
    TRISB = 0X00;                  // Puertos como outputs   
    //TRISCbits.TRISC2 = 0;          //Pin para seleccionar
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC3 = 1;          //Pin para seleccionar
    TRISCbits.TRISC5 = 1;          //Pin para seleccionar
    TRISD = 0X00;
    TRISCbits.TRISC6 = 0;          //Pin para seleccionar
    TRISCbits.TRISC7 = 1;          //Pin para seleccionar

    
    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;
    PORTC = 0X00;
    PORTD = 0X00;
    PORTE = 0X00;
    
    // Configuración del oscilador, TMR0 y ADC
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 4Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 0;
    
    //configuracion de SPI master
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW,
            SPI_IDLE_2_ACTIVE);

    config_USART();                // Configurar uart con la libreria
}

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){
    setup();
    while(1){
        PORTCbits.RC2 = 0;         // Selector del esclavo
       __delay_ms(1);
       
       spiWrite(0x0A);
       vol_a = spiRead();
       __delay_ms(1);
       
       spiWrite(0x0A);
       vol_b = spiRead();
       __delay_ms(1);
       
       PORTCbits.RC2 = 1;          // Selector del esclavo
       __delay_ms(50);
       printf(" Valor actual de los voltajes: ");
       division(vol_a);
       mensaje1();
       __delay_ms(50);
       division(vol_b);
       mensaje2();
       __delay_ms(50);
       
       printf("Ingresar la centena deseada, si es <100 colocar 0.\r");
       while(RCIF == 0);           // Secuencia recibe tres valores seguidos
       c1 = RCREG -48;             // No desplegar nada en ninguna parte 
       
       while(RCREG > '2'){         // Valor mayor a 2, mostrar mensaje de prec.
           precaucion();
       }
       
       printf("Ingresar la decena deseada.\r");
       while(RCIF == 0);           // Al restar -48 lo recibimos como decimal
       d1 = RCREG -48;             // 3 variables que nos serviran para armar
       
       if(c1 == 2){
           while(RCREG > '5'){
               precaucion1();
           }
       }
       
       printf("Ingresar la unidad deseada.\r\r");
       while(RCIF == 0);           // Número completo
       u1 = RCREG -48;
       
       if(c1 == 2 && d1 == 5){
           while(RCREG > '5'){
               precaucion2();
           }
       }
       
       sprintf(s1, "%d", c1);      // conver primero a decimal 
       sprintf(s2, "%d", d1);      // Almacenar como strings en los buffer
       sprintf(s3, "%d", u1);
       strcat(s1, s2);             // Concatenar s2 con s1, ahora estan en s1
       strcat(s1, s3);             // Concatenar s3 con s1 = (s1+s2)
       int completo = atoi(s1);    // Pasar a entero
       division(completo);         // Dividir para verificar su valor en el UART
       __delay_ms(100);
       TXREG = cen;
       __delay_ms(100);
       TXREG = dec;
       __delay_ms(100);
       TXREG = un;
       __delay_ms(100);
       PORTD = completo;           // Desplegar en binario
    }
}

//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************
void putch(char info){          // Trans. de la cadena de caract. a esta funcion                  
    while (TXIF == 0);          // Se espera algo que haya que transmitir
    TXREG = info;               // Lo que hay en data se pasa al reg. de transm.  
                                // para que se depliegue en la terminal virtual.
}

void division(char dividendo){
    cen = (dividendo)/100;      // Dividir entre 100 y se queda con el entero
    residuo = dividendo%100;    // Residuo
    dec = residuo/10; 
    un = residuo%10;            // Unidades de las decenas
    cen += 48;                  // A partir del 0
    dec += 48;
    un += 48;
    return;
} 

void mensaje1(void){            // Para desplegar un mensaje
    printf("\r* V1: ");    
    __delay_ms(50);
    TXREG = cen;                // Ingresarle el valor de las centenas
    __delay_ms(50);
    printf(".");
    __delay_ms(50);
    TXREG = dec;                // Ingresarle el valor de las decenas
    __delay_ms(50);
    TXREG = un;                 // Ingresarle el valor de las uniddes
    __delay_ms(50);             // Colocarle un delay para que no loquee
    printf("\r");
}

void mensaje2(void){
    printf("\r* V2: ");
    __delay_ms(50);
    TXREG = cen;                // Ingresarle el valor de las centenas
    __delay_ms(50);
    printf(".");
    __delay_ms(50);
    TXREG = dec;                // Ingresarle el valor de las decenas
    __delay_ms(50);
    TXREG = un;                 // Ingresarle el valor de las uniddes
    __delay_ms(50);             // Colocarle un delay para que no loquee
    printf("\r\r");
}

void precaucion(void){          // Evitar que metan valores que no son
    if(RCREG > 2){
           printf("Introduzca un valor de 0 a 2\r");   
       }
       while(RCIF == 0);
       c1 = RCREG -48;
}

void precaucion1(void){         // Evitar que metan valores que no son
    if(RCREG > 5){
           printf("Introduzca un valor menor o igual a 5\r");   
       }
       while(RCIF == 0);
       d1 = RCREG -48;
}

void precaucion2(void){         // Evitar que metan valores que no son
    if(RCREG > 5){
           printf("Introduzca un valor menor o igual a 5\r");   
       }
       while(RCIF == 0);
       u1 = RCREG -48;
}



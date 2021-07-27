/* 
 * File:   Main7.c (Laboratorio No. 2)
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: LCD + UART
 * 
 * Hardware: 
 * 
 * Created on 22 de julio de 2021
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include "config_ADC.h"
#include "LCD.h"
#include <stdio.h>

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
                                
//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
uint8_t FLAG;
uint8_t startfinal;
uint8_t start;
uint8_t contador;
uint8_t disp0;
uint8_t disp1;
uint8_t tempo1;
uint8_t VUART;
uint8_t RXREC;
unsigned char VAL1;
unsigned char VAL2;
unsigned char un;                 // Variable unidades
unsigned char dec;                // Variable decenas
unsigned char cen;                // Variable centenas

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                 // Configuraciones
void inicio(void);  
void final (void);  
uint8_t table(uint8_t);           //Funcion tabla
void dispasign(uint8_t , uint8_t );
void hexconv(uint8_t );
void decimal(uint8_t );

//******************************************************************************
//                     F U N C I Ó N   para   I S R
//******************************************************************************
void __interrupt()isr(void){
    if (T0IF==1){                 // Bandera del TMR0 encendida
    INTCONbits.T0IF = 0;          // Apagar la bandera
    }

    if(PIR1bits.ADIF == 1){       // Revisar si el canal AN0 está activo
        if(ADCON0bits.CHS == 0){  //Ver canal 0
            VAL1 = ADRESH;        //Mover a leds
        }
        else{                     //ver canal 1
            VAL2 = ADRESH;        //Guardar en variable para display
        }
        PIR1bits.ADIF = 0;        // Limpiar bandera 
    }    
    
    if(PIR1bits.RCIF == 1){
        RXREC = RCREG;      //Guardar el dato que se recibe en uart 
        if (RXREC == 43){
            contador++;     //Incrementar el valor con RX
        }
        if (RXREC == 45){
            contador--;     //Decrementar el valor con RX
        }
        PIR1bits.RCIF = 0;   //Limpiar la bandera
    }
}

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void) {
    setup();                     // Inicializar el setup
    Lcd_Init();                  // Inicializar LCD
    Lcd_Clear();                 // Limpiar LCD
    Lcd_Set_Cursor(1,1);         // Cursor  uno para la primera posicion 
    Lcd_Write_String("S1:   S2:   S3:");  // Mensaje a desplegar
    
while(1) {
    chselect(2);                 // 2 = solo se usa un canal
 
    decimal(VAL1);               // Potenciómetro 1
    Lcd_Set_Cursor(2,1);         // Cursor dos para la segunda posición
    Lcd_Write_Char(cen);         // Ingresarle el valor de las centenas
    Lcd_Write_Char(dec);         // Ingresarle el valor de las decenas
    Lcd_Write_Char(un);          // Ingresarle el valor de las unidades
    Lcd_Write_String("   ");     // Dejar un espacio
    
    if(PIR1bits.TXIF == 1){
        __delay_ms(150);
        printf("VALOR DE POT 1: \r"); // Desplegar mensaje para identificar val
        __delay_ms(150);
        TXREG = cen;        // Enviar a terminal palabras
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = 46;         // Enviar punto
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = dec;        // Enviar a terminal palabras
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = un;         // Enviar a terminal palabras
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = 13;         // Enviar enter
           }
        __delay_ms(150);
    
        
        
    decimal(VAL2);               // Potenciómetro 2        
    Lcd_Write_Char(cen);         // Ingresarle el valor de las centenas
    Lcd_Write_Char(dec);         // Ingresarle el valor de las decenas
    Lcd_Write_Char(un);          // Ingresarle el valor de las unidades
    __delay_us(1500);            // Colocarle un delay para que no loquee
    
    if(PIR1bits.TXIF == 1){
        __delay_ms(150);
        printf("VALOR DE POT 2: \r"); // Desplegar mensaje para identificar val
        __delay_ms(150);
            TXREG = cen;         // Enviar a terminal palabras
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = 46;          // Enviar punto
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = dec;         // Enviar a terminal palabras
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = un;          // Enviar a terminal palabras
           }
        __delay_ms(10);
        if(PIR1bits.TXIF == 1){
            TXREG = 13;          // Enviar enter
           }
        __delay_ms(150);
        
        
//ContadorRX
    decimal(contador);           // Convertir a decimales y a CHAR
    Lcd_Write_Char(cen);
    Lcd_Write_String(".");       // Escribir un punto
    Lcd_Write_Char(dec);
    Lcd_Write_Char(un);
    __delay_ms(100);
    }
}

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************
void setup(void){
    
// CONFIGURACIÓN DE LOS PUERTOS
  ANSELH = 0x00;                 // Puerto B digital
  ANSEL = 0B00000011;            // Pines digitales en el puerto A
  
  TRISA = 0B00000011;            // Puertos como outputs   
  TRISCbits.TRISC6 = 0;       // RX entrada Y TX salida
  TRISCbits.TRISC7 = 1;       // RX entrada Y TX salida
  TRISD = 0X00;
  TRISE = 0X00;
 
  PORTA = 0X00;                  // Inicializar los puertos
  PORTB = 0X00;
  PORTC = 0X00;
  PORTD = 0X00;
  PORTE = 0X00;
  
  initAN(0b00000011, 0);         // Inicializar AN y justificar a derecha (1)  

// Configuración del oscilador, TMR0 y ADC
  OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
  OSCCONbits.IRCF2 = 1;          // 4Mhz
  OSCCONbits.IRCF1 = 1; 
  OSCCONbits.IRCF0 = 0;

// Configuración del TMR0, N = 100 y un overflow de 10ms
  OPTION_REG = 0B11000100;       // Prescaler 1:64
  INTCONbits.GIE = 1;            // GIE Encender interrupción de global
  INTCONbits.PEIE = 1;           // PEIE 
  INTCONbits.T0IE = 1;           // T0IE Encender interrupción de OVERFLOW TMR0 
  INTCONbits.T0IF = 0;           // Limpiar la bandera del overflow TMR0
  PIE1bits.ADIE = 1;             // ADIE Habilitar para comprobar FLAG -GF
  PIR1bits.ADIF = 0;             // Limpiar bandera de interrupción del ADC
  
 // Configuración UART transmisor y receptor asíncrono
  PIR1bits.RCIF = 0;          // Bandera
  PIE1bits.RCIE = 0;          // Habilitar la interrución por el modo receptor
  PIE1bits.TXIE = 0;          // Habilitar bandera de interrupción
  TXSTAbits.TX9 = 0;          // 8 bits
  TXSTAbits.TXEN = 1;         // Se habilita el transmisor
  TXSTAbits.SYNC = 0;         // Se opera de forma asíncrona y de 8 bits
  TXSTAbits.BRGH = 1; 
  RCSTAbits.RX9 = 0;          // 8 bits
  RCSTAbits.CREN = 1;         // Receptor se habilita
  RCSTAbits.SPEN = 1;         // Módulo ON y el pin TX se config. como salida
                                // y el RX como entrada

// Generador de baudios del USART
  BAUDCTLbits.BRG16 = 0;
  SPBRG = 25;                  // Para una velocidad de transmisión de 9600
  SPBRGH = 1; 
}
    

//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************
void decimal(uint8_t var){       // Función para obtener valor decimal
    uint8_t VAL;
    VAL = var;                   // Guardar el valor del Puerto
    cen = (VAL/100) ;            // Obtener cen dividiendo dentro de 100
    VAL = (VAL - (cen*100));
    dec = (VAL/10);              // Obtener dec dividiendo dentro de 10
    VAL = (VAL - (dec*10));
    un = (VAL);                  // Obtener un dividiendo dentro de 1
    
    cen = cen + 48;              // Se realiza la suma por el 0 en ASCII
    dec = dec + 48;              // que es igual a 48 en decimal
    un = un + 48;
}

void putch(char info){           // Trans. de la cadena de caract. a esta funcion                  
    while (TXIF == 0);           // Se espera algo que haya que transmitir
    TXREG = info;                // Lo que hay en data se pasa al reg. de transm.  
                                 // para que se depliegue en la terminal virtual.
}

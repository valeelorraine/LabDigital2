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
#include <string.h>               // Librería para concatenar
#include <stdlib.h>

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
unsigned char FLAG = 0X00;
unsigned char FLAG1 = 0X00;
unsigned char un;
unsigned char dec; 
unsigned char cen;
int select;
char centena;
unsigned char decena;
unsigned char unidad;
char var, con;
int full;

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                  // Configuraciones
void setup(void);
void Text(void);
void decimal(uint8_t var);

//******************************************************************************
//                     F U N C I Ó N   para   I S R
//******************************************************************************
void __interrupt() isr(void){       
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
    ANSELH = 0x00;                 // Puerto B digital
    
    TRISA = 0X00;                  // Puertos como outputs   
    TRISB = 0B00000011;
    TRISE = 0X00;
    
    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;
    PORTC = 0X00;
    PORTE = 0X00;
    
    // Configuración del oscilador
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 4Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 0;
    
    // WEAK PULL UP
    IOCB = 0B00000011;             // Habilitar lo del IOCB en pines RB0 y RB1
    WPUB = 0B00000011;
    
    // Configuración de los special register
    OPTION_REGbits.nRBPU = 0;   // Desabilitar RBPU para utilizar pullUp en 2 Pa
    INTCONbits.GIE = 1;            // GIE Encender interrupción de global
    INTCONbits.PEIE = 1;           // PEIE 
    INTCONbits.RBIE = 1;           // RBIE Encender interrupción PORTB CHANGE
    INTCONbits.RBIF = 0;           // RBIF Limpiar la bandera de CHANGE INTERRUPT
    
    // Configuración UART transmisor y receptor asíncrono
    // Configuración UART transmisor y receptor asíncrono
    PIR1bits.RCIF = 0;          // Limpiar bandera RX
    PIR1bits.TXIF = 0;          // Limpiar bandera del TX
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
    BAUDCTLbits.BRG16 = 0;      // Activar el generador de baudios
    SPBRG = 25;                 // Para una velocidad de transmisión de 9600
    SPBRGH = 1; 
    
    }

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){  
    setup();                            // Llamar al set up       
    while (1){  
        PORTD = select;
        Text();
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

void Text(void){ 
    __delay_ms(250); //Tiempos para el despliegue de los caracteres
        decimal(PORTA);
        printf("Valor del contador:\r");
        __delay_ms(250);
        TXREG = cen;
        __delay_ms(250);
        TXREG = dec;
        __delay_ms(250);
        TXREG = un;
        __delay_ms(250);
        printf("\r");


         printf("Ingresar valor de la centena que sea menor o igual que 2: \r ");
          defensa1:  
           while(RCIF == 0);
            centena = RCREG -48;  

           while(RCREG > '2'){ 
               goto defensa1;
           }

        printf("Ingresar decena: \r");
          defensa2:
            while(RCIF == 0); 
             decena = RCREG -48; 

            if(centena == 2){
               while(RCREG > '5'){
                   goto defensa2;
               }
           }

        printf("Ingresar unidad: \r");
          defensa3:
           while(RCIF == 0); 
            unidad = RCREG - 48;

           if(centena == 2 && decena == 5){
               while(RCREG > '5'){
                   goto defensa3;
               }
           }
          con = concat(centena, decena);
          full = concat(con, unidad);
          __delay_ms(250);
        printf("El numero elegido es: %d \r", full);
        select = full;
    }

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


    int concat(int a, int b)
    {

        char s1[20];
        char s2[20];


        // Convert both the integers to string
        sprintf(s1, "%d", a);
        sprintf(s2, "%d", b);


        // Concatenate both strings
        strcat(s1, s2);

        // Convert the concatenated string
        // to integer
        int c = atoi(s1);

        // return the formed integer
        return c;
    }

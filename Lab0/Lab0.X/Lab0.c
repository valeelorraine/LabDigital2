/* 
 * File:   Main7.c (Laboratorio No. 7)
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: Juego de Carrera
 * 
 * Hardware: Tres botones colocados en PULL UP, un DISPLAY de 7 segmentos y 21 
 *           LEDS
 * 
 * Created on 10 de julio de 2021
 */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#include <xc.h>
#include <stdint.h>

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
#define _tmr0_value 100            // N de 100 para obtener un overflow de 5ms


//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
unsigned char CONT;
unsigned char FLAG = 0X00;
unsigned char FLAG1 = 0X00;
unsigned char FLAG2 = 0X00;
unsigned char VAL;
unsigned char VAL1;
unsigned char SEM;

//******************************************************************************
//                          Valores del display
//******************************************************************************

char NUMEROS[4] = { 
    0B00111111,                    // 0
    0B00000110,                    // 1
    0B01011011,                    // 2
    0B01001111,                    // 3
    };

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                  // Configuraciones
void SALIDA(void);
void BOTONES(void);

//******************************************************************************
//                     F U N C I Ó N   para   I S R
//******************************************************************************
void __interrupt() isr(void){
    if(T0IF == 1){                 // Bandera del TMR0 encendida
        INTCONbits.T0IF = 0;       // Apagar la bandera
        TMR0 = _tmr0_value;        // Inicializar TMR0
         if(FLAG == 1){            // Si FLAG está encendida entonces...
            CONT++;                // Incrementar contador
                }   
    INTCONbits.T0IF = 0;           // Apagar la bandera del TMR0 
    }
}

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************
void setup(void){
    // CONFIGURACIÓN DE LOS PUERTOS
    ANSEL = 0B00011111;            // Pines digitales en el puerto A
    ANSELH = 0X00;                 // Puerto B digital
    
    TRISA = 0X00;                  // Puertos como outputs   
    TRISB = 0X00;
    TRISC = 0B00111000;
    TRISD = 0X00; 
    TRISE = 0X00;
    
    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;
    PORTC = 0X00;
    PORTD = 0X00;
    PORTE = 0X00;
    
    // Configuración del TMR0, N = 100 y un overflow de 10ms
    OPTION_REG = 0B10000101;       // Prescaler 1:64
    TMR0 = _tmr0_value;            // Inicializar TMR0
    INTCONbits.GIE = 1;            // GIE Encender interrupción de global
    INTCONbits.PEIE = 1;           // PEIE 
    INTCONbits.T0IE = 1;           // T0IE Encender interrupción de OVERFLOW TMR0 
    INTCONbits.T0IF = 0;           // Limpiar la bandera del overflow TMR0
    
    
    VAL = 0B00000001;              // Inicializar las variables
    VAL1 = 0B00000001;
    PORTD = NUMEROS[3];            // Colocar el display en 3
    }

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){  
    setup();                       // Llamar al set up       
    while (1){ 
        if(PORTCbits.RC3 == 0){    // Si RC3 se presionó y soltó, encender FLAG 
            FLAG = 1;
            VAL = 0B00000001;      // Inicializar las variables
            VAL1 = 0B00000001;
            PORTA = 0X00;          // Limpiar los puertos
            PORTB = 0X00;
            PORTCbits.RC6 = 0;     // Apagar los inidicadores de ganado
            PORTCbits.RC7 = 0;
            PORTD = NUMEROS[3];    
        }           
        else{
            if(FLAG == 1){         // Si FLAG está encendida entonces apagarla 
                if(CONT == 200){   // Si se alcanzó 1s incr. SEM
                    SEM++;
                    CONT = 0X00;   // Inicializar contador
                }
                SALIDA();          // Función para el semáforo
            }     
        }
        while(PORTCbits.RC2 == 1){     
            BOTONES();             // Se pueden usar los otros botones
        }   
    }
}
//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************

void SALIDA(void){   
    switch(SEM){                   // Contador del semáforo       
        case 1:                    
            PORTD = NUMEROS[2];    // Encender el color rojo y No. 2
            PORTCbits.RC0 = 1;
            PORTCbits.RC1 = 0;  
            PORTCbits.RC2 = 0;
            break;
        case 2:
            PORTD = NUMEROS[1];    // Encender el color amarillo y No. 1
            PORTCbits.RC0 = 0;
            PORTCbits.RC1 = 1;  
            PORTCbits.RC2 = 0;
            break;
        case 3:
            PORTD = NUMEROS[0];    // Encender el color verde y No. 0
            PORTCbits.RC0 = 0;
            PORTCbits.RC1 = 0;  
            PORTCbits.RC2 = 1;
            SEM = 0X00;            // Inicializar la var de semáforo
            break;     
    }
} 

void BOTONES(void){
    if(PORTCbits.RC4 == 1){        // Si RC4 se presionó y soltó, FLAG1 = 1
            FLAG1 = 1;  
        }
    else{
        if(FLAG1 == 1){            // Si FLAG1 está encendida entonces apagarla
            FLAG1 = 0;

            PORTA = VAL;
            VAL <<= 1;
            PORTCbits.RC6 = 0;
        }
    }
            
    if(PORTCbits.RC5 == 1){        // Si RC5 se presionó y soltó, FLAG2 = 1
        FLAG2 = 1;
    }
    else{
        if(FLAG2 == 1){            // Si FLAG2 está encendida entonces apagarla
            FLAG2 = 0;
            PORTB = VAL1;
            VAL1 <<= 1;
            PORTCbits.RC7 = 0;
        }
    }

    if(PORTAbits.RA7 == 1){        // Ver quien es el ganador
        PORTCbits.RC2 = 0;
        PORTCbits.RC4 = 0;
        PORTCbits.RC6 = 1;
        PORTD = NUMEROS[1];        // Colocar el numero del ganador
        FLAG = 0; 
   }
    else if(PORTBbits.RB7 == 1){ 
        PORTCbits.RC2 = 0;
        PORTCbits.RC4 = 0;
        PORTCbits.RC7 = 1; 
        PORTD = NUMEROS[2];        // Colocar el numero del ganador
        FLAG = 0;
        }
    }
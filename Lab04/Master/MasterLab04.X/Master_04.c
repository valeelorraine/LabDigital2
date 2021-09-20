/* 
 * File:  Laboratorio 4
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: Comunicaión I2C
 * 
 * Hardware: 3 PIC16F887, potenciómetros, 8 LEDS
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
#include "I2C.h"                   // Librería del I2C
#include "LCD_4.h"                 // Libreria de la LCD

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
#define _XTAL_FREQ 4000000         // Frecuencia del oscilador
#define RS RD2                     // Definir las variables como nombres de los
#define EN RD3                     // pines
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
unsigned char VOL; 
unsigned char un;
unsigned char dec; 
unsigned char cen;
unsigned char POT;
unsigned char TEMP;
uint8_t sensor1;
uint8_t sensor2;
uint16_t temp;                   // Se utilizan dos dos bytes
float temp1;                     // Porque recibira valores en decimal para el mapeo

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);
void decimal(uint8_t var);
void Temperatura(uint8_t var);
void voltaje(uint8_t var);

//******************************************************************************
//                      C O N F I G U R A C I Ó N
//******************************************************************************
void setup(void){
    // CONFIGURACIÓN DE LOS PUERTOS
    ANSELH = 0x00;                  // Puerto B digital
    TRISD = 0X00;                   // Puertos digitales
    TRISE = 0x00;

    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;                   
    PORTC = 0X00;
    PORTD = 0X00;
    
    // Configuración del oscilador
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 4Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 0;
    
    // Inicializar comunicacion I2C con libreria
    I2C_Master_Init(100000);        // Frecuencia recomendada tipica
 }
//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){
    setup();                       // Inicializar el setup
    Lcd_Init();                    // Inicializar LCD
    Lcd_Clear();                   // Limpiar LCD
    Lcd_Set_Cursor(1,1);           // Cursor  uno para la primera posicion
    Lcd_Write_String("S1:   S2:   S3:");  // Mensaje a desplegar
    __delay_us(100);
    
    while(1){
        //Obtener informacion del segundo slave
        I2C_Master_Start();
        I2C_Master_Write(0x61); //51, el 1 para que lea
        sensor2 = I2C_Master_Read(0); //lo escribo en el puerto de leds
        I2C_Master_Stop();
        __delay_ms(200);
    
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        POT = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);

        //Temp
        I2C_Master_Start();
        I2C_Master_Write(0x80);    // Seleccionar el sensor y se escribe
        I2C_Master_Write(0xF3);    // Read temperature
        I2C_Master_Stop();         // Finalizar la obtención de información
        __delay_ms(200);

        I2C_Master_Start();         
        I2C_Master_Write(0x81);     // Para que ahora lea
        TEMP = I2C_Master_Read(0);  // Read temperature
        I2C_Master_Stop();          // Finalizar la obtención de info.
        __delay_ms(200);

        //Potenciometro 1
        voltaje(POT);              //Convertir a decimales y a CHAR
        Lcd_Set_Cursor(2,1);        //Llevar el cursor a fila 2 primer espacio
        Lcd_Write_Char(cen);
        Lcd_Write_String(".");
        Lcd_Write_Char(dec);    //Enviar datos
        Lcd_Write_Char(un);
        Lcd_Write_String("  ");
        __delay_ms(200);
    
        //Escritura del contador
        Lcd_Set_Cursor(2,8);
        decimal(sensor2);     
        Lcd_Write_Char(dec);
        Lcd_Set_Cursor(2,9);
        Lcd_Write_Char(un);  
    
        //Contador 2
        Lcd_Set_Cursor(2,13);
        Temperatura(TEMP);
        Lcd_Write_Char(cen);
        Lcd_Write_Char(dec);    //Enviar datos
        Lcd_Write_Char(un);
        Lcd_Write_String("C ");
        __delay_ms(200);
        
        PORTB = POT;
        PORTA = TEMP;
        __delay_ms(200);
}
    }

//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************
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

void voltaje(uint8_t var){        // Función para obtener valor decimal
    uint8_t VAL;
    VAL = var;                    // Guardar el valor del Puerto
    VAL = (VAL*1.96);             // convertir a Volt
    cen = (VAL/100) ;             // Obtener cen dividiendo dentro de 100
    VAL = (VAL - (cen*100));
    dec = (VAL/10);               // Obtener dec dividiendo dentro de 10
    VAL = (VAL - (dec*10));
    un = (VAL);                   // Obtener un dividiendo dentro de 1
    
    cen = cen + 48;               // Se realiza la suma por el 0 en ASCII
    dec = dec + 48;               // que es igual a 48 en decimal
    un = un + 48;
}

void Temperatura(uint8_t var){        // Función para obtener valor decimal
    uint8_t VAL;
    VAL = (0.7*var)-47;                    // Guardar el valor del Puerto
    cen = (VAL/100) ;             // Obtener cen dividiendo dentro de 100
    VAL = (VAL - (cen*100));
    dec = (VAL/10);               // Obtener dec dividiendo dentro de 10
    VAL = (VAL - (dec*10));
    un = (VAL);                   // Obtener un dividiendo dentro de 1
    
    cen = cen + 48;               // Se realiza la suma por el 0 en ASCII
    dec = dec + 48;               // que es igual a 48 en decimal
    un = un + 48;
}






//* File: Libreria C Ultasónico 
// * Author: Valerie Lorraine Sofia Valdez Trujillo
// * Compilador: pic-as (v2.30), MPLABX V5.45
// * 
// * Descripción del programa: Comunicaión I2C
// * 
// * Hardware: 3 PIC16F887, potenciómetros, 8 LEDS
// * 
// * Created on 28 de julio de 2021
// */

//******************************************************************************
//                           L I B R E R Í A S
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "DHT11.h"
//#include "UART.h"
#include "I2C.h"
//#include "LCDD2.h"

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
#define _XTAL_FREQ 8000000         // Frecuencia del oscilador

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
uint8_t VAL;                       // Variable para las fotoresistencias
uint8_t VAL2;                      // Variable para las fotoresistencias
uint8_t ULTR;

//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                   // Configuraciones
void canales(void);                 // Switcheo de pots con servos
void CONFIG(void);

//******************************************************************************
//                     F U N C I Ó N   para   I S R
//******************************************************************************
void __interrupt() isr(void){  
    if(PIR1bits.ADIF == 1){         //INTERRUPCIÓN DEL ADC
        switch(ADCON0bits.CHS){     // Asignación del ADRESH a las variables
            case 0:                 // También es un switcheo con casos
                VAL = ADRESH;       
                break;

            case 1:
                VAL2 = ADRESH; 
                break;
            }        
        PIR1bits.ADIF = 0;          // Limpiar bandera   
       }
        
    PIR1bits.TMR2IF = 0;           // Limpiar la bandera del TMR2

//******************************************************************************
//  Funciones principales y de configuracion
//******************************************************************************
void main(void) {
    CONFIG();
    TMR1H = 0;
    TMR1L = 0;
    __delay_us(100);
    
    while(1){
        // Ultrasónico
        I2C_Master_Start();         // START para el sensor
        I2C_Master_Write(0X51);        // Direccion pero de lectura del ultrasónico
        ULTR = I2C_Master_Read(0);     // Lee el valor y lo mete en ULTR
        I2C_Master_Stop();
         __delay_ms(200);

        PORTA = ULTR;
    }
}

void CONFIG(void){
    ANSEL =     0X00;
    ANSELH =    0X00;
    TRISA =     0X00;
    TRISB =     0X00;
    TRISD =     0X00;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    PORTA =     0X00;
    PORTB =     0X00;
    PORTD =     0X00;

    //Configuracion del oscilador
    OSCCONbits.IRCF = 0b111; //oscilador a 8Mhz
    OSCCONbits.SCS = 0b1;
    
    //interrupciones utilizadas
    INTCONbits.GIE =    1;
    INTCONbits.PEIE =   1; //Permite INT perifericas
    PIR1bits.RCIF =     0;
    PIE1bits.RCIE =     1; //permite interrupciones de recepcion de datos
    
    //Configuracion del timmer1
    T1CON = 0X10; //periodo de 1MHz, ya que fuente es FOSC/4
    TMR1H = 0; //valores en 0 para el conteo de los periodos de pulso
    TMR1L = 0;  
    I2C_Master_Init(100000);
}

                   

//}
//
//
//void setup(void){
// // CONFIGURACIÓN DE LOS PUERTOS
//    ANSEL = 0B00000011;        // Pines digitales en el puerto A
//    ANSELH = 0x00;           // Puerto B digital
//    
//    TRISA = 0B00000011;        // Puertos como outputs 
//    TRISBbits.TRISB2 = 0;          // Led 
//    TRISBbits.TRISB3 = 0;          // Led
//    TRISBbits.TRISB4 = 1;          // ECHO
//    TRISBbits.TRISB5 = 0;          // TRIGGER   
//    
//    TRISCbits.TRISC1 = 0;          // CCP2
//    TRISCbits.TRISC2 = 0;          // CCP1      
////    TRISCbits.TRISC3 = 0;        // Señal del clock SCL
////    TRISCbits.TRISC4 = 0;        // Datos seriales SDA
////TRISD = 0X00;                  // Desplegar valor del puerto
// 
//    PORTA = 0X00;                  // Inicializar los puertos
//    PORTB = 0X00;
//    PORTC = 0X00;
//    PORTD = 0X00;
//    PORTE = 0X00;
//    
//    // Configuración de los special register
//    INTCONbits.GIE = 1;            // GIE Encender interrupción de global
//    INTCONbits.PEIE = 1;           // PEIE 
//
//    
////////    // Configuración del oscilador
////////    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
////////    OSCCONbits.IRCF2 = 1;          // 8Mhz
////////    OSCCONbits.IRCF1 = 1; 
////////    OSCCONbits.IRCF0 = 1;
//    
//    // Configuración del oscilador
//    OSCCONbits.SCS = 1;         // Utilizar el oscilador itnterno
//    OSCCONbits.IRCF2 = 1;       // Oscilador de 8MHz
//    OSCCONbits.IRCF1 = 1;
//    OSCCONbits.IRCF0 = 1;
//    
//    // Configuración TMR1 
//    TMR1 = 0X00;                   // TMR1L y TMR1H = 0
//    T1CONbits.T1CKPS = 0B01;       // Prescaler de 1:2
//    T1CONbits.TMR1GE = 0;          // Contador siempre cuenta
//    T1CONbits.TMR1CS = 0;          // Internal clock (FOCSC/4)
//    
//    // Configuración del TMR2
//    PIE1bits.TMR2IE = 1;        // Habilita el TMR2 to PR2 match interrupt 
//    PIR1bits.TMR2IF = 0;        // Limpiar la bandera del TMR2
//    T2CON = 0X4E;               // Encender TMR2ON, Pre 1:16 y Post 1:10
//    //T2CON = 0X26;               // Encender TMR2ON, Pre 1:16 y  Post 1:5
//    
//    // Configuraciones del módulo ADC
//    ADCON0bits.CHS = 0;         // Usar canal 0
//    ADCON0bits.CHS = 1;         // Usar canal 2
//    __delay_us(100);            // Delay de 100
//    
//    PIE1bits.ADIE = 1;          // ADIE Habilitar para comprobar FLAG -GF
//    PIR1bits.ADIF = 0;          // Limpiar bandera de interrupción del ADC
//    ADCON0bits.ADON = 1;        // Encender el módulo
//    ADCON0bits.ADCS = 1;        // FOSC/8 
//    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
//    ADCON1bits.VCFG0 = 0;       // Voltaje de referencia en VSS y VDD
//    ADCON1bits.VCFG1 = 0;
//    
//    // Configuración del PWM
//    PR2 = 250;                  // Período del pwm 4ms
//    CCP1CON = 0B00001100;       // El CCP1 se encuentra en Modo PWM 
//    CCP2CON = 0B00001111;       // El CCP2 se encuentra en modoo PWM
//    
//    // Asignar esta direccion al esclavo
//    I2C_Slave_Init(0x50);
//    }
// 
//////******************************************************************************
//////                         L O O P   P R I N C I P A L
//////******************************************************************************
//void main(void){  
//    setup();                        // Llamar al set up       
//    while (1){  
//        canales();                  // Swicheo de los canales
//
//        __delay_ms(200);            // Delay para que no loqueen los servos
//        C_distancia(dist);
//         //PORTD = dist;              // Probar el valor en el puerto
//         if(dist <= 4){             // Si el objeto se encuentra a menos de 4cm
//            PORTBbits.RB3 = 1;     // Encender RB1 y apagar RB2
//            PORTBbits.RB2 = 0;
//            __delay_ms(1);
//        }
//        if(dist >= 5){             // Si el objeto se encuentra a más de 5cm
//            PORTBbits.RB3 = 0;     // Encender RB2 y apagar RB1
//            PORTBbits.RB2 = 1;
//            __delay_ms(1);
//        }
//    }
//}
//
////******************************************************************************
////                           F U N C I O N E S 
////******************************************************************************
//// Bit banging se refiere a manejar el PWM por tiempos manuales
//void canales(){                // Switcheo de los canales
//    if(ADCON0bits.GO == 0){
//        switch(ADCON0bits.CHS){           
//            case 0: 
//                CCPR1L = 2*((0.247*VAL)+62);  // Función para el servo
//                ADCON0bits.CHS = 1;         // Canal 2
//                __delay_us(100);            // Delay para activar una medición
//                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
//                break; 
//                              
//            case 1: 
//                CCPR2L = 2*((0.247*VAL2)+62); // Función para el servo     VALOR 62          
//                ADCON0bits.CHS = 0;         // Canal 3
//                __delay_us(100);            // Delay para activar una medición
//                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
//                break; 
//                
//            default:
//                break;
//         }
//    }
//}

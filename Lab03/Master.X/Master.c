///* 
// * File:  Laboratorio 3
// * Author: Valerie Lorraine Sofia Valdez Trujillo
// * Compilador: pic-as (v2.30), MPLABX V5.45
// * 
// * Descripci�n del programa: Comunicai�n SPI
// * 
// * Hardware: 2 PIC16F887, potenci�metros, 8 LEDS
// * 
// * Created on 28 de julio de 2021
// */
//
////******************************************************************************
////                           L I B R E R � A S
////******************************************************************************
//#include <xc.h>
//#include <stdint.h>                // Librer�a para variables de ancho definido
//#include <stdio.h>                 // Para el sprintf
//#include <stdlib.h>
//#include <string.h>
//#include <pic16f887.h>
//#include "config_USART.h"          // Libreria para configurar USART
//#include "SPI.h"                   // Librer�a del SPI
//
////******************************************************************************
////                      C O N F I G U R A C I � N 
////******************************************************************************
//
//// PIC16F887 Configuration Bit Settings
//#pragma config FOSC=INTRC_NOCLKOUT // Oscillador interno I/O RA6
//#pragma config WDTE=OFF            // WDT disabled (reinicio rep. del pic)
//#pragma config PWRTE=OFF           // Power-up Timer (PWRT disabled)
//#pragma config MCLRE=OFF           // El pin de MCLR se utiliza como I/O
//#pragma config CP=OFF              // Sin protecci�n de c�digo
//#pragma config CPD=OFF             // Sin protecci�n de datos
//
//#pragma config BOREN=OFF           // No reinicia cu�ndo Vin baja de 4v
//#pragma config IESO=OFF            // Reinicio sin cambio de reloj inter-exter.
//#pragma config FCMEN=OFF           // Cambio de reloj exter-inter en caso falla
//#pragma config LVP=OFF             // Progra en bajo voltaje permitido
//
//// CONFIG2
//#pragma config BOR4V = BOR40V      // Reinicio a bajo de 4v, (BOR21V=2.1V)
//#pragma config WRT = OFF           // Protecci�n de autoescritura x prog. desact.
//
////******************************************************************************
////             D I R E C T I V A S  del  C O M P I L A D O R
////******************************************************************************
//#define _XTAL_FREQ 4000000         // Frecuencia de operaci�n
//
////******************************************************************************
////                           V A R I A B L E S
////******************************************************************************
//uint8_t vol_a, vol_b;
//uint8_t cen, dec, un, residuo;
//uint8_t c1, d1, u1;
//char s1[10];
//char s2[10];
//char s3[10];
//
////******************************************************************************
////                 P R O T O T I P O S  de  F U N C I O N E S
////******************************************************************************
//void setup(void);                  // Configuraciones
//void putch(char dato);
//void mensaje1(void);
//void mensaje2(void);
//void division(char dividendo);
//void precaucion(void);
//void precaucion1(void);
//void precaucion2(void);
//
////******************************************************************************
////                      C O N F I G U R A C I � N
////******************************************************************************
//void setup(void){
//    TRISB = 0X00;                  // Puertos como outputs   
//    //TRISCbits.TRISC2 = 0;          //Pin para seleccionar
//    TRISCbits.TRISC2 = 1;
//    TRISCbits.TRISC3 = 1;          //Pin para seleccionar
//    TRISCbits.TRISC5 = 1;          //Pin para seleccionar
//    TRISD = 0X00;
//    TRISCbits.TRISC6 = 0;          //Pin para seleccionar
//    TRISCbits.TRISC7 = 1;          //Pin para seleccionar
//
//    
//    PORTA = 0X00;                  // Inicializar los puertos
//    PORTB = 0X00;
//    PORTC = 0X00;
//    PORTD = 0X00;
//    PORTE = 0X00;
//    
//    // Configuraci�n del oscilador, TMR0 y ADC
//    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
//    OSCCONbits.IRCF2 = 1;          // 4Mhz
//    OSCCONbits.IRCF1 = 1; 
//    OSCCONbits.IRCF0 = 0;
//    
//    //configuracion de SPI master
//    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW,
//            SPI_IDLE_2_ACTIVE);
//
//    config_USART();                // Configurar uart con la libreria
//}
//
////******************************************************************************
////                         L O O P   P R I N C I P A L
////******************************************************************************
//void main(void){
//    setup();
//    while(1){
//        PORTCbits.RC2 = 0;         // Selector del esclavo
//       __delay_ms(1);
//       
//       spiWrite(0x0A);
//       vol_a = spiRead();
//       __delay_ms(1);
//       
//       spiWrite(0x0A);
//       vol_b = spiRead();
//       __delay_ms(1);
//       
//       PORTCbits.RC2 = 1;          // Selector del esclavo
//       __delay_ms(50);
//       printf(" Valor actual de los voltajes: ");
//       division(vol_a);
//       mensaje1();
//       __delay_ms(50);
//       division(vol_b);
//       mensaje2();
//       __delay_ms(50);
//       
//       printf("Ingresar la centena deseada, si es <100 colocar 0.\r");
//       while(RCIF == 0);           // Secuencia recibe tres valores seguidos
//       c1 = RCREG -48;             // No desplegar nada en ninguna parte 
//       
//       while(RCREG > '2'){         // Valor mayor a 2, mostrar mensaje de prec.
//           precaucion();
//       }
//       
//       printf("Ingresar la decena deseada.\r");
//       while(RCIF == 0);           // Al restar -48 lo recibimos como decimal
//       d1 = RCREG -48;             // 3 variables que nos serviran para armar
//       
//       if(c1 == 2){
//           while(RCREG > '5'){
//               precaucion1();
//           }
//       }
//       
//       printf("Ingresar la unidad deseada.\r\r");
//       while(RCIF == 0);           // N�mero completo
//       u1 = RCREG -48;
//       
//       if(c1 == 2 && d1 == 5){
//           while(RCREG > '5'){
//               precaucion2();
//           }
//       }
//       
//       sprintf(s1, "%d", c1);      // conver primero a decimal 
//       sprintf(s2, "%d", d1);      // Almacenar como strings en los buffer
//       sprintf(s3, "%d", u1);
//       strcat(s1, s2);             // Concatenar s2 con s1, ahora estan en s1
//       strcat(s1, s3);             // Concatenar s3 con s1 = (s1+s2)
//       int completo = atoi(s1);    // Pasar a entero
//       division(completo);         // Dividir para verificar su valor en el UART
//       __delay_ms(100);
//       TXREG = cen;
//       __delay_ms(100);
//       TXREG = dec;
//       __delay_ms(100);
//       TXREG = un;
//       __delay_ms(100);
//       PORTD = completo;           // Desplegar en binario
//    }
//}
//
////******************************************************************************
////                           F U N C I O N E S 
////******************************************************************************
//void putch(char info){          // Trans. de la cadena de caract. a esta funcion                  
//    while (TXIF == 0);          // Se espera algo que haya que transmitir
//    TXREG = info;               // Lo que hay en data se pasa al reg. de transm.  
//                                // para que se depliegue en la terminal virtual.
//}
//
//void division(char dividendo){
//    cen = (dividendo)/100;      // Dividir entre 100 y se queda con el entero
//    residuo = dividendo%100;    // Residuo
//    dec = residuo/10; 
//    un = residuo%10;            // Unidades de las decenas
//    cen += 48;                  // A partir del 0
//    dec += 48;
//    un += 48;
//    return;
//} 
//
//void mensaje1(void){            // Para desplegar un mensaje
//    printf("\r* V1: ");    
//    __delay_ms(50);
//    TXREG = cen;                // Ingresarle el valor de las centenas
//    __delay_ms(50);
//    printf(".");
//    __delay_ms(50);
//    TXREG = dec;                // Ingresarle el valor de las decenas
//    __delay_ms(50);
//    TXREG = un;                 // Ingresarle el valor de las uniddes
//    __delay_ms(50);             // Colocarle un delay para que no loquee
//    printf("\r");
//}
//
//void mensaje2(void){
//    printf("\r* V2: ");
//    __delay_ms(50);
//    TXREG = cen;                // Ingresarle el valor de las centenas
//    __delay_ms(50);
//    printf(".");
//    __delay_ms(50);
//    TXREG = dec;                // Ingresarle el valor de las decenas
//    __delay_ms(50);
//    TXREG = un;                 // Ingresarle el valor de las uniddes
//    __delay_ms(50);             // Colocarle un delay para que no loquee
//    printf("\r\r");
//}
//
//void precaucion(void){          // Evitar que metan valores que no son
//    if(RCREG > 2){
//           printf("Introduzca un valor de 0 a 2\r");   
//       }
//       while(RCIF == 0);
//       c1 = RCREG -48;
//}
//
//void precaucion1(void){         // Evitar que metan valores que no son
//    if(RCREG > 5){
//           printf("Introduzca un valor menor o igual a 5\r");   
//       }
//       while(RCIF == 0);
//       d1 = RCREG -48;
//}
//
//void precaucion2(void){         // Evitar que metan valores que no son
//    if(RCREG > 5){
//           printf("Introduzca un valor menor o igual a 5\r");   
//       }
//       while(RCIF == 0);
//       u1 = RCREG -48;
//}
//
//

 /* File: Ultas�nico y servos
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
#include <stdint.h>                // Librer�a para variables de ancho definido
#include <stdio.h>                 // Para el sprintf
#include <stdlib.h>
#include <string.h>
#include <pic16f887.h>

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
#define _XTAL_FREQ 8000000         // Frecuencia del oscilador
#define pinTrig RC2;               // Definir las variables como nombres de los 
#define pinEcho RC3;               // pines 

//******************************************************************************
//                           V A R I A B L E S
//******************************************************************************
unsigned char FLAG = 0X00; 
//uint16_t TMR1C = 0X00;             // Variable para el contador
unsigned char dist = 0x00;
//******************************************************************************
//                 P R O T O T I P O S  de  F U N C I O N E S
//******************************************************************************
void setup(void);                  // Prototipo para la configuraci�n
    
//******************************************************************************
//                      C O N F I G U R A C I � N
//******************************************************************************
void setup(void){
    // CONFIGURACI�N DE LOS PUERTOS
    ANSEL = 0X00;                  // Pines digitales en el puerto A
    ANSELH = 0x00;                 // Puerto B digital
    

    TRISCbits.TRISC2 = 0;          // Puertos como otputs
    TRISCbits.TRISC3 = 1;
    TRISBbits.TRISB1 = 0;          // Puertos como otputs
    TRISBbits.TRISB2 = 0;
    TRISD = 0X00;
 
    PORTA = 0X00;                  // Inicializar los puertos
    PORTB = 0X00;
    PORTC = 0X00;
    PORTD = 0X00;
    PORTE = 0X00;
    
    // Configuraci�n del oscilador
    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
    OSCCONbits.IRCF2 = 1;          // 8Mhz
    OSCCONbits.IRCF1 = 1; 
    OSCCONbits.IRCF0 = 1;
    
    // Configuraci�n de los special register
    INTCONbits.GIE = 1;            // GIE Encender interrupci�n de global
    INTCONbits.PEIE = 1;           // PEIE 

    // Configuraci�n TMR1 Prescaler 1:8; TMR1 Preload = 15536; Actual Interrupt 
    TMR1 = 0X00;                   // TMR1L y TMR1H = 0
    T1CONbits.T1CKPS = 0B01;       // Prescaler de 1:2
    T1CONbits.TMR1GE = 0;          // Contador siempre cuenta
    T1CONbits.TMR1CS = 0;          // Internal clock (FOCSC/4)
    }

//******************************************************************************
//                         L O O P   P R I N C I P A L
//******************************************************************************
void main(void){  
    setup();                       // Llamar al set up       
    while (1){  
        __delay_ms(200);
        C_distancia(dist);
        PORTD = dist;
        
        if(dist <= 4){ 
            PORTBbits.RB1 = 1; 
            PORTBbits.RB2 = 0;
            __delay_ms(1);
        }
        if(dist >= 5){ 
            PORTBbits.RB1 = 0;  
            PORTBbits.RB2 = 1;
            __delay_ms(1);
        }
    }
}
        
//******************************************************************************
//                           F U N C I O N E S 
//******************************************************************************
    
int C_distancia(void){ 
    dist = 0x00;                  // Inicializar distancia
    TMR1 = 0X00;                  // Inicializar timer
    PORTCbits.RC2 = 1;            // Enviar se�al al sensor (TRIGGER)
    __delay_us(10);               // Esperar los 10us que debe el sensor
    PORTCbits.RC2 = 0;            // Apagar la se�al (TRIGGER)
    while(PORTCbits.RC3 == 0){};  // Esperar el pulso del sensor (ECHO)
    T1CONbits.TMR1ON = 1;         // Encender el modulo del timer
    while(PORTCbits.RC3 == 1){};  // Esperar a que el pulso termine (ECHO)
    T1CONbits.TMR1ON = 0;         // Apagar el timer
    dist = TMR1/58.82;            // Funci�n para obtener dist. en cm
    return dist;
}  



////////EN este funciona el Ultrasonico
//////
///////* File: Libreria C Ultas�nico 
//////// * Author: Valerie Lorraine Sofia Valdez Trujillo
//////// * Compilador: pic-as (v2.30), MPLABX V5.45
//////// * 
//////// * Descripci�n del programa: Comunicai�n I2C
//////// * 
//////// * Hardware: 3 PIC16F887, potenci�metros, 8 LEDS
//////// * 
//////// * Created on 28 de julio de 2021
//////// */
//////
//////
////////******************************************************************************
////////                           L I B R E R � A S
////////******************************************************************************
//////#include <xc.h>
//////#include <stdint.h>                // Librer�a para variables de ancho definido
//////#include <stdio.h>                 // Para el sprintf funcione
//////#include <stdlib.h>
//////#include <string.h>                // Librer�a para concatenar
//////#include "I2C.h"                   // Librer�a del I2C
//////#include "Ultrasonicoo.h"          // Librer�a para el sensor ultras�nico
//////
////////******************************************************************************
////////                      C O N F I G U R A C I � N 
////////******************************************************************************
//////
//////// PIC16F887 Configuration Bit Settings
//////#pragma config FOSC=INTRC_NOCLKOUT // Oscillador interno I/O RA6
//////#pragma config WDTE=OFF            // WDT disabled (reinicio rep. del pic)
//////#pragma config PWRTE=OFF           // Power-up Timer (PWRT disabled)
//////#pragma config MCLRE=OFF           // El pin de MCLR se utiliza como I/O
//////#pragma config CP=OFF              // Sin protecci�n de c�digo
//////#pragma config CPD=OFF             // Sin protecci�n de datos
//////
//////#pragma config BOREN=OFF           // No reinicia cu�ndo Vin baja de 4v
//////#pragma config IESO=OFF            // Reinicio sin cambio de reloj inter-exter.
//////#pragma config FCMEN=OFF           // Cambio de reloj exter-inter en caso falla
//////#pragma config LVP=OFF             // Progra en bajo voltaje permitido
//////
//////// CONFIG2
//////#pragma config BOR4V = BOR40V      // Reinicio a bajo de 4v, (BOR21V=2.1V)
//////#pragma config WRT = OFF           // Protecci�n de autoescritura x prog. desact.
//////
////////******************************************************************************
////////             D I R E C T I V A S  del  C O M P I L A D O R
////////******************************************************************************
//////#define _XTAL_FREQ 8000000         // Frecuencia del oscilador
//////#define pinTrig RB5;               // Definir las variables como nombres de los 
//////#define pinEcho RB4;               // pines 
//////
////////******************************************************************************
////////                           V A R I A B L E S
////////******************************************************************************
//////uint8_t dist = 0x00;               // Variable de la distancia
////////uint8_t cont = 0x00;               // Variable para probar la comunicaci�n serial
//////unsigned char z;
//////uint8_t PWM1;                      // Variable para el 1er PWM creado
//////uint8_t PWM2;                      // Variable para el 2do PWM creado
////////******************************************************************************
////////                 P R O T O T I P O S  de  F U N C I O N E S
////////******************************************************************************
//////void setup(void);                  // Prototipo para la configuraci�n
////////void canales(void);                 // Switcheo de pots con servos
//////
////////******************************************************************************
////////                     F U N C I � N   para   I S R
////////******************************************************************************
////////void __interrupt() isr(void){
//////    //if(PIR1bits.ADIF == 1){         //INTERRUPCI�N DEL ADC
////////        switch(ADCON0bits.CHS){     // Asignaci�n del ADRESH a las variables
////////            case 0:                 // Tambi�n es un switcheo con casos
////////                VAL = ADRESH;       
////////                break;
////////            case 1: 
////////                VAL1 = ADRESH; 
////////                break;
////////            case 2:
////////                VAL2 = ADRESH; 
////////                break;
////////            case 3:
////////                VAL3 = ADRESH; 
////////                break;
////////            }        
////////        PIR1bits.ADIF = 0;          // Limpiar bandera   
////////       }
//////    
//////    
////////     if(PIR1bits.SSPIF == 1){ 
////////        SSPCONbits.CKP = 0;
////////       
////////        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
////////            z = SSPBUF;             // Leer valor previo para limpiar buffer
////////            SSPCONbits.SSPOV = 0;   // Limpiar bandera del overflow
////////            SSPCONbits.WCOL = 0;    // Limpiar collision bit
////////            SSPCONbits.CKP = 1;     // Utilizar el SCL (Clock)
////////        }
////////
////////        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
////////            z = SSPBUF;             // Lec. del SSBUF para limpiar buffer y flag BF
////////            PIR1bits.SSPIF = 0;     // Limpiar FLAG de interr. recepci�n/transmisi�n SSP
////////            SSPCONbits.CKP = 1;     // Habilitar entrada de pulsos de reloj SCL
////////            while(!SSPSTATbits.BF); // Esperar a que la recepci�n se complete
////////     //       0B00001111 = SSPBUF;          // Guardar val. buffer de recepci�n en PORTD
////////            __delay_us(250);   
////////           // temp = SSPBUF;
////////        }
////////        
////////        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
////////            z = SSPBUF;             // Variable temporal
////////            BF = 0;
////////            SSPBUF = cont;
////////            SSPCONbits.CKP = 1;
////////            __delay_us(250);
////////            while(SSPSTATbits.BF);
////////        }
////////        PIR1bits.SSPIF = 0;         // Limpiar bandera
////////    }
////////}
//////
////////******************************************************************************
////////                      C O N F I G U R A C I � N
////////******************************************************************************
//////void setup(void){
//////    
//////    // CONFIGURACI�N DE LOS PUERTOS
//////    ANSEL = 0X00;                  // Pines digitales en el puerto A
//////    ANSELH = 0x00000001;                 // Puerto B digital
//////    
//////    TRISBbits.TRISB0 = 1;
//////    TRISBbits.TRISB3 = 0;          // Puertos como otputs
//////    TRISBbits.TRISB2 = 0;          // Las del B son las leds
//////    TRISBbits.TRISB4 = 1;          // ECHO
//////    TRISBbits.TRISB5 = 0;          // TRIGGER        
////////    TRISCbits.TRISC3 = 0;          // Se�al del clock SCL
////////    TRISCbits.TRISC4 = 0;          // Datos seriales SDA
//////    TRISD = 0X00;                  // Desplegar valor del puerto
////// 
//////    PORTA = 0X00;                  // Inicializar los puertos
//////    PORTB = 0X00;
//////    PORTC = 0X00;
//////    PORTD = 0X00;
//////    PORTE = 0X00;
//////    
//////    // Configuraci�n del oscilador
//////    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
//////    OSCCONbits.IRCF2 = 1;          // 8Mhz
//////    OSCCONbits.IRCF1 = 1; 
//////    OSCCONbits.IRCF0 = 1;
//////    
//////    // Configuraci�n de los special register
//////    INTCONbits.GIE = 1;            // GIE Encender interrupci�n de global
//////    INTCONbits.PEIE = 1;           // PEIE 
//////
//////    // Configuraci�n TMR1 
//////    TMR1 = 0X00;                   // TMR1L y TMR1H = 0
//////    T1CONbits.T1CKPS = 0B01;       // Prescaler de 1:2
//////    T1CONbits.TMR1GE = 0;          // Contador siempre cuenta
//////    T1CONbits.TMR1CS = 0;          // Internal clock (FOCSC/4)
//////    
////////    // Configuraci�n del TMR2
////////    PIR1bits.TMR2IF = 0;        // Limpiar la bandera del TMR2
////////    T2CON = 0X4D;               // Encender TMR2ON, Pre 1:16 y Post 1:5
////////    
////////        // Configuraci�n del PWM
////////    PR2 = 250;                  // Per�odo del pwm 4ms
////////    CCP1CON = 0B00001100;       // El CCP1 se encuentra en Modo PWM 
////////    CCP2CON = 0B00001111;       // El CCP2 se encuentra en modoo PWM
////////    
////////        // Configuraciones del m�dulo ADC
////////    ADCON0bits.CHS = 0;         // Usar canal 0
////////    ADCON0bits.CHS = 2;         // Usar canal 2
////////    __delay_us(100);            // Delay de 100
////////    
////////    PIE1bits.ADIE = 1;          // ADIE Habilitar para comprobar FLAG -GF
////////    PIR1bits.ADIF = 0;          // Limpiar bandera de interrupci�n del ADC
////////    ADCON0bits.ADON = 1;        // Encender el m�dulo
////////    ADCON0bits.ADCS = 1;        // FOSC/8 
////////    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
////////    ADCON1bits.VCFG0 = 0;       // Voltaje de referencia en VSS y VDD
////////    ADCON1bits.VCFG1 = 0;
//////    
//////    // Asignar esta direccion al esclavo
//////    I2C_Slave_Init(0x50);
//////    }
//////
////////******************************************************************************
////////                         L O O P   P R I N C I P A L
////////******************************************************************************
//////void main(void){  
//////    setup();                       // Llamar al set up       
//////    while (1){  
//////        //cont++;
//////        __delay_ms(200);
//////        C_distancia(dist);
//////        //canales();                  // Swicheo de los canales
//////        PORTD = dist;              // Probar el valor en el puerto
//////        
//////        if(dist <= 4){             // Si el objeto se encuentra a menos de 4cm
//////            PORTBbits.RB3 = 1;     // Encender RB1 y apagar RB2
//////            PORTBbits.RB2 = 0;
//////            __delay_ms(1);
//////        }
//////        if(dist >= 5){             // Si el objeto se encuentra a m�s de 5cm
//////            PORTBbits.RB3 = 0;     // Encender RB2 y apagar RB1
//////            PORTBbits.RB2 = 1;
//////            __delay_ms(1);
//////        }
//////    }
//////}
//////        
////////******************************************************************************
////////                           F U N C I O N E S 
////////******************************************************************************
//////
//////// Bit banging se refiere a manejar el PWM por tiempos manuales
////////void canales(){                // Switcheo de los canales
////////    if(ADCON0bits.GO == 0){
////////        switch(ADCON0bits.CHS){           
////////            case 0: 
////////                CCPR1L = ((0.247*VAL)+62);  // Funci�n para el servo
////////                VALOR1 = CCPR1L;
////////                ADCON0bits.CHS = 1;         // Canal 2
////////                __delay_us(100);            // Delay para activar una medici�n
////////                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
////////                break; 
////////                
////////            case 1:                         // PWM codificado
////////                POT4 = ((0.049*VAL1)+7);
////////                ADCON0bits.CHS = 2;         // Canal 0
////////                __delay_us(250);            // Delay para activar una medici�n
////////                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
////////                break; 
////////                              
////////            case 2: 
////////                CCPR2L = ((0.247*VAL2)+62); // Funci�n para el servo
////////                VALOR2 = CCPR2L;
////////                ADCON0bits.CHS = 3;         // Canal 3
////////                __delay_us(100);            // Delay para activar una medici�n
////////                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
////////                break; 
////////                
////////            case 3:                         // PWM codificado
////////                POT3 = ((0.049*VAL3)+7); 
////////                ADCON0bits.CHS = 0;         // Canal 1
////////                __delay_us(250);            // Delay para activar una medici�n
////////                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
////////                break; 
////////                
////////            default:
////////                break;
////////         }
////////    }
////////}   
////////int C_distancia(void){ 
////////    dist = 0x00;                  // Inicializar distancia
////////    TMR1 = 0X00;                  // Inicializar timer
////////    PORTCbits.RC2 = 1;            // Enviar se�al al sensor (TRIGGER)
////////    __delay_us(10);               // Esperar los 10us que debe el sensor
////////    PORTCbits.RC2 = 0;            // Apagar la se�al (TRIGGER)
////////    while(PORTCbits.RC3 == 0){};  // Esperar el pulso del sensor (ECHO)
////////    T1CONbits.TMR1ON = 1;         // Encender el modulo del timer
////////    while(PORTCbits.RC3 == 1){};  // Esperar a que el pulso termine (ECHO)
////////    T1CONbits.TMR1ON = 0;         // Apagar el timer
////////    dist = TMR1/58.82;            // Funci�n para obtener dist. en cm
////////    return dist;
////////}  


//
//////aqui FUNCIONAN LOS POTS SIN INCLUIR EL ULTRASONICO
////
//////aqui esta lo del MAINNNNSLAVE
////
//////* File: Libreria C Ultas�nico 
////// * Author: Valerie Lorraine Sofia Valdez Trujillo
////// * Compilador: pic-as (v2.30), MPLABX V5.45
////// * 
////// * Descripci�n del programa: Comunicai�n I2C
////// * 
////// * Hardware: 3 PIC16F887, potenci�metros, 8 LEDS
////// * 
////// * Created on 28 de julio de 2021
////// */
////
//////******************************************************************************
//////                           L I B R E R � A S
//////******************************************************************************
//#include <xc.h>
//#include <stdint.h>                // Librer�a para variables de ancho definido
//#include <stdio.h>                 // Para el sprintf funcione
//#include <stdlib.h>
//#include <string.h>                // Librer�a para concatenar
//#include "I2C.h"                   // Librer�a del I2C
//#include "Ultrasonicoo.h"          // Librer�a para el sensor ultras�nico
//
////******************************************************************************
////                      C O N F I G U R A C I � N 
////******************************************************************************
//
//// PIC16F887 Configuration Bit Settings
//#pragma config FOSC=INTRC_NOCLKOUT // Oscillador interno I/O RA6
//#pragma config WDTE=OFF            // WDT disabled (reinicio rep. del pic)
//#pragma config PWRTE=OFF           // Power-up Timer (PWRT disabled)
//#pragma config MCLRE=OFF           // El pin de MCLR se utiliza como I/O
//#pragma config CP=OFF              // Sin protecci�n de c�digo
//#pragma config CPD=OFF             // Sin protecci�n de datos
//
//#pragma config BOREN=OFF           // No reinicia cu�ndo Vin baja de 4v
//#pragma config IESO=OFF            // Reinicio sin cambio de reloj inter-exter.
//#pragma config FCMEN=OFF           // Cambio de reloj exter-inter en caso falla
//#pragma config LVP=OFF             // Progra en bajo voltaje permitido
//
//// CONFIG2
//#pragma config BOR4V = BOR40V      // Reinicio a bajo de 4v, (BOR21V=2.1V)
//#pragma config WRT = OFF           // Protecci�n de autoescritura x prog. desact.
//
////******************************************************************************
////             D I R E C T I V A S  del  C O M P I L A D O R
////******************************************************************************
//#define _XTAL_FREQ 8000000         // Frecuencia del oscilador
//#define pinTrig RB5;               // Definir las variables como nombres de los 
//#define pinEcho RB4;               // pines 
//
////******************************************************************************
////                           V A R I A B L E S
////******************************************************************************
//uint8_t dist = 0x00;               // Variable de la distancia
//uint8_t cont = 0x00;               // Variable para probar la comunicaci�n serial
//unsigned char z;
//uint8_t VAL;                       // Variable para las fotoresistencias
//uint8_t VAL2;                      // Variable para las fotoresistencias
////                      
////******************************************************************************
////                 P R O T O T I P O S  de  F U N C I O N E S
////******************************************************************************
//void setup(void);                   // Configuraciones
//void canales(void);                 // Switcheo de pots con servos
//
//////******************************************************************************
//////                     F U N C I � N   para   I S R
//////******************************************************************************
//void __interrupt() isr(void){  
//    if(PIR1bits.ADIF == 1){         //INTERRUPCI�N DEL ADC
//        switch(ADCON0bits.CHS){     // Asignaci�n del ADRESH a las variables
//            case 0:                 // Tambi�n es un switcheo con casos
//                VAL = ADRESH;       
//                break;
//
//            case 1:
//                VAL2 = ADRESH; 
//                break;
//            }        
//        PIR1bits.ADIF = 0;          // Limpiar bandera   
//       }
//        
//    PIR1bits.TMR2IF = 0;           // Limpiar la bandera del TMR2
//
////    PIR1bits.TMR2IF = 0;            // Limpiar la bandera del TMR2
////
////  
//////     if(PIR1bits.SSPIF == 1){ 
//////        SSPCONbits.CKP = 0;
//////       
//////        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
//////            z = SSPBUF;             // Leer valor previo para limpiar buffer
//////            SSPCONbits.SSPOV = 0;   // Limpiar bandera del overflow
//////            SSPCONbits.WCOL = 0;    // Limpiar collision bit
//////            SSPCONbits.CKP = 1;     // Utilizar el SCL (Clock)
//////        }
//////
//////        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
//////            z = SSPBUF;             // Lec. del SSBUF para limpiar buffer y flag BF
//////            PIR1bits.SSPIF = 0;     // Limpiar FLAG de interr. recepci�n/transmisi�n SSP
//////            SSPCONbits.CKP = 1;     // Habilitar entrada de pulsos de reloj SCL
//////            while(!SSPSTATbits.BF); // Esperar a que la recepci�n se complete
//////            cont = SSPBUF;          // Guardar val. buffer de recepci�n en PORTD
//////            __delay_us(250);   
//////        }
//////        
//////        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
//////            z = SSPBUF;             // Variable temporal
//////            BF = 0;
//////            SSPBUF = cont;
//////            SSPCONbits.CKP = 1;
//////            __delay_us(250);
//////            while(SSPSTATbits.BF);
//////        }
//////        PIR1bits.SSPIF = 0;         // Limpiar bandera
//////    }
//}
//
//
//void setup(void){
// // CONFIGURACI�N DE LOS PUERTOS
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
////    TRISCbits.TRISC3 = 0;        // Se�al del clock SCL
////    TRISCbits.TRISC4 = 0;        // Datos seriales SDA
////TRISD = 0X00;                  // Desplegar valor del puerto
// 
//    PORTA = 0X00;                  // Inicializar los puertos
//    PORTB = 0X00;
//    PORTC = 0X00;
//    PORTD = 0X00;
//    PORTE = 0X00;
//    
//    // Configuraci�n de los special register
//    INTCONbits.GIE = 1;            // GIE Encender interrupci�n de global
//    INTCONbits.PEIE = 1;           // PEIE 
//
//    
////////    // Configuraci�n del oscilador
////////    OSCCONbits.SCS = 1;            // Utilizar el oscilador itnterno
////////    OSCCONbits.IRCF2 = 1;          // 8Mhz
////////    OSCCONbits.IRCF1 = 1; 
////////    OSCCONbits.IRCF0 = 1;
//    
//    // Configuraci�n del oscilador
//    OSCCONbits.SCS = 1;         // Utilizar el oscilador itnterno
//    OSCCONbits.IRCF2 = 1;       // Oscilador de 4MHz
//    OSCCONbits.IRCF1 = 1;
//    OSCCONbits.IRCF0 = 0;
//    
//    // Configuraci�n TMR1 
//    TMR1 = 0X00;                   // TMR1L y TMR1H = 0
//    T1CONbits.T1CKPS = 0B01;       // Prescaler de 1:2
//    T1CONbits.TMR1GE = 0;          // Contador siempre cuenta
//    T1CONbits.TMR1CS = 0;          // Internal clock (FOCSC/4)
//    
//    // Configuraci�n del TMR2
//    PIE1bits.TMR2IE = 1;        // Habilita el TMR2 to PR2 match interrupt 
//    PIR1bits.TMR2IF = 0;        // Limpiar la bandera del TMR2
//    //    T2CON = 0X4D;               // Encender TMR2ON, Pre 1:16 y Post 1:5
//    T2CON = 0X26;               // Encender TMR2ON, Pre 1:16 y  Post 1:5
//    
//    // Configuraciones del m�dulo ADC
//    ADCON0bits.CHS = 0;         // Usar canal 0
//    ADCON0bits.CHS = 1;         // Usar canal 2
//    __delay_us(100);            // Delay de 100
//    
//    PIE1bits.ADIE = 1;          // ADIE Habilitar para comprobar FLAG -GF
//    PIR1bits.ADIF = 0;          // Limpiar bandera de interrupci�n del ADC
//    ADCON0bits.ADON = 1;        // Encender el m�dulo
//    ADCON0bits.ADCS = 1;        // FOSC/8 
//    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
//    ADCON1bits.VCFG0 = 0;       // Voltaje de referencia en VSS y VDD
//    ADCON1bits.VCFG1 = 0;
//    
//    // Configuraci�n del PWM
//    PR2 = 250;                  // Per�odo del pwm 4ms
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
//      //  C_distancia(dist);
//         //PORTD = dist;              // Probar el valor en el puerto
//         if(dist <= 4){             // Si el objeto se encuentra a menos de 4cm
//            PORTBbits.RB3 = 1;     // Encender RB1 y apagar RB2
//            PORTBbits.RB2 = 0;
//            __delay_ms(1);
//        }
//        if(dist >= 5){             // Si el objeto se encuentra a m�s de 5cm
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
//                CCPR1L = ((0.247*VAL)+62);  // Funci�n para el servo
//                //VALOR1 = CCPR1L;
//                ADCON0bits.CHS = 1;         // Canal 2
//                __delay_us(100);            // Delay para activar una medici�n
//                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
//                break; 
//                              
//            case 1: 
//                CCPR2L = ((0.247*VAL2)+62); // Funci�n para el servo
//               //VALOR2 = CCPR2L;
//                ADCON0bits.CHS = 0;         // Canal 3
//                __delay_us(100);            // Delay para activar una medici�n
//                ADCON0bits.GO = 1;          // Comienza el ciclo del ADC
//                break; 
//                
//            default:
//                break;
//         }
//    }
//}

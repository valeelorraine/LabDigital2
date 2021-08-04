/* 
 * File:  Laboratorio 3
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: Comunicaión SPI
 *
 */

#include <xc.h>

void config_USART(void){
//configurar transmisor y receptor asincrono
    SPBRG = 103;              // Baud rate de 9600
    SPBRGH = 0;
    BAUDCTLbits.BRG16 = 1;    // 8bits baud rate generator is used
    TXSTAbits.BRGH = 1;       // High speed
    
    TXSTAbits.SYNC = 0;       // Asincrono
    // serial port enabled (Configures RX/DT and TX/CK pins as serial)
    RCSTAbits.SPEN = 1; 
    RCSTAbits.CREN = 1;       // Habilitar la recepcion
    
    TXSTAbits.TX9 = 0;        // Transmision de 8bits
    TXSTAbits.TXEN = 1;       // Enable the transmission
    RCSTAbits.RX9 = 0;        // Recepcion de 8 bits

    INTCONbits.GIE = 1;       // Enable de global interrupts
    INTCONbits.PEIE = 1;

    PIR1bits.TXIF = 0;        // Limpiar interrupciones
    PIR1bits.RCIF = 0;
}
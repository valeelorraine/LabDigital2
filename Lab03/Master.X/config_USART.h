/* 
 * File:  Laboratorio 3
 * Author: Valerie Lorraine Sofia Valdez Trujillo
 * Compilador: pic-as (v2.30), MPLABX V5.45
 * 
 * Descripción del programa: Comunicaión SPI
 *
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef xc_USART //definir nuevo nombre que el otro header para que no lo llame
#define	xc_USART //dos veces

#include <xc.h> // include processor files - each processor file is guarded.  

void config_USART(void);

#endif	/* XC_HEADER_TEMPLATE_H */

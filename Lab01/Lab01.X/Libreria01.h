/* 
 * File:   Libreria01.h
 * Author: personal01
 *
 * Created on 15 de julio de 2021, 12:27
 */

#ifndef LIBRERIA01_H
#define	LIBRERIA01_H
#define _XTAL_FREQ 4000000
#include <xc.h>

int LECTURA(){
    unsigned char ADCL;
    ADCL = ADRESH;
    return ADCL;
}
#endif	/* LIBRERIA01_H */


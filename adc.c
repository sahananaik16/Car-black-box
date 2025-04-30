/*
 * File:   adc.c
 * Author: DELL
 *
 * Created on December 9, 2024, 5:51 PM
 */


#include "black.h"

/*initializing adc*/
void init_adc() {
    CHS3=0;
    CHS2=1;
    CHS1=0;
    CHS0=0;
    
    ADON=1;
    
    PCFG3=1;
    PCFG2=0;
    PCFG1=1;
    PCFG0=0;
    
    VCFG1=0;
    VCFG0=0;
    
    ADFM=0;
    
    ADCS2=0;
    ADCS1=1;
    ADCS0=0;
    
    ACQT2=1;
    ACQT1=0;
    ACQT0=0;
    
    ADRESH=0X00;
    ADRESL=0X00;
}

/*reading adc value*/
unsigned short read_adc()
{
    GO=1;
    while(GO);
    
    return (ADRESH<<2)|(ADRESL>>6);
}

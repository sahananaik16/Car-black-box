/*
 * File:   internal_eeprom.c
 * Author: vaibh
 *
 * Created on 20 November, 2024, 4:38 PM
 */


#include "black.h"

unsigned char internal_eeprom_read(unsigned char addr)
{
    EEADR=addr;
    EEPGD=0;
    CFGS=0;
    WREN=0;
    RD=1;
    return EEDATA;
}

void internal_eeprom_write(unsigned char addr,unsigned char data)
{
    EEADR=addr;
    EEDATA=data;
    EEPGD=0;
    CFGS=0;
    WREN=1;
    GIE=0;
    EECON2=0x55;
    EECON2=0xAA;
    WR=1;
    GIE=1;
    while(WR);
}
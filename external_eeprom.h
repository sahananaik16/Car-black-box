/* 
 * File:   external_eeprom.h
 * Author: DELL
 *
 * Created on December 24, 2024, 5:56 PM
 */

#ifndef EXTERNAL_EEPROM_H
#define	EXTERNAL_EEPROM_H

void write_external_eeprom(unsigned char address, unsigned char data);

unsigned char read_external_eeprom(unsigned char address);

#define SLAVE_READ_E 0xA1
#define SLAVE_WRITE_E 0xA0

#endif	/* EXTERNAL_EEPROM_H */


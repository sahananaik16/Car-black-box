/*
 * File:   external_eeprom.c
 * Author: DELL
 *
 * Created on December 24, 2024, 5:55 PM
 */



#include "black.h"

void write_external_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();//start
	i2c_write(SLAVE_WRITE_E);//slave address
	i2c_write(address);//address write
	i2c_write(data);//data write
	i2c_stop();//stop
    for(unsigned int wait=3000;wait--; );
}

unsigned char read_external_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();//start
	i2c_write(SLAVE_WRITE_E);//write the address
	i2c_write(address);//writing the address
	i2c_rep_start();//operation has changed so master should hold the bus
	i2c_write(SLAVE_READ_E);//reading operation
	data = i2c_read();//data read
	i2c_stop();

	return data;
}

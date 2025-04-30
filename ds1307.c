
#include "black.h"

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */

void init_ds1307(void)
{
	unsigned char dummy;

	/* Setting the CH bit of the RTC to Stop the Clock */
	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy | 0x80); 

	/* Setting 12 Hr Format */
	dummy = read_ds1307(HOUR_ADDR);
	write_ds1307(HOUR_ADDR, dummy | 0x40); 


	/* Clearing the CH bit of the RTC to Start the Clock */
	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy & 0x7F); 

}

void write_ds1307(unsigned char address, unsigned char data)
{
	i2c_start();//start
	i2c_write(SLAVE_WRITE);//slave address
	i2c_write(address);//address write
	i2c_write(data);//data write
	i2c_stop();//stop
}

unsigned char read_ds1307(unsigned char address)
{
	unsigned char data;

	i2c_start();//start
	i2c_write(SLAVE_WRITE);//write the address
	i2c_write(address);//writing the address
	i2c_rep_start();//operation has changed so master should hold the bus
	i2c_write(SLAVE_READ);//reading operation
	data = i2c_read();//data read
	i2c_stop();

	return data;
}
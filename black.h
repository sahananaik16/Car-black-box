/* 
 * File:   black.h
 * Author: DELL
 *
 * Created on December 13, 2024, 5:22 PM
 */

#ifndef BLACK_H
#define	BLACK_H

#include <xc.h>
#include "external_eeprom.h"

#define LEVEL 0
#define EDGE 1

#define CLCD_PORT			PORTD
#define CLCD_EN				RC2
#define CLCD_RS				RC1
#define CLCD_RW				RC0
#define CLCD_BUSY			RD7
#define PORT_DIR			TRISD7


#define HI												1
#define LO												0

#define INPUT											0xFF
#define OUTPUT											0x00

#define DATA_COMMAND									1
#define INSTRUCTION_COMMAND								0
#define _XTAL_FREQ                  20000000
#define LINE1(x)									(0x80 + (x))
#define LINE2(x)										(0xC0 + (x))

#define TWO_LINE_5x8_MATRIX_8_BIT					clcd_write(0x38, INSTRUCTION_COMMAND)
#define CLEAR_DISP_SCREEN				                clcd_write(0x01, INSTRUCTION_COMMAND)
#define CURSOR_HOME							clcd_write(0x02, INSTRUCTION_COMMAND)
#define DISP_ON_AND_CURSOR_OFF						clcd_write(0x0C, INSTRUCTION_COMMAND)
#define EIGHT_BIT_MODE   0x33


#define SLAVE_READ		0xD1
#define SLAVE_WRITE		0xD0


#define SEC_ADDR		0x00
#define MIN_ADDR		0x01
#define HOUR_ADDR		0x02
#define DAY_ADDR		0x03
#define DATE_ADDR		0x04
#define MONTH_ADDR		0x05
#define YEAR_ADDR		0x06
#define CNTL_ADDR		0x07

unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[9];
unsigned char date[11];

void write_ds1307(unsigned char address1,  unsigned char data);
unsigned char read_ds1307(unsigned char address1);

void init_i2c(void);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
void i2c_write(unsigned char data);
unsigned char i2c_read(void);
void init_ds1307(void);

//UART
#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);
int puts(const char *s);
unsigned char getch(void);
unsigned char getch_with_timeout(unsigned short max_time);
unsigned char getche(void);


unsigned char time[]="00:00:00";
unsigned short speed=0;
unsigned char *gear[9]={"ON","GN","G1","G2","G3","G4","G5","GR","C "};
unsigned char key,event_addr=0x00,event_count=0,data_store[10][10],read_addr=0x00;
int gear_change = 0;
unsigned char events_to_read;
unsigned char state_ind = 0;
unsigned char display_index = 0;
unsigned char scroll = 0,save_exit=0;
unsigned char index_count = 0;
unsigned char hr,min,sec,field=0,upload=0;


/* Enum for maintaining the app state */
typedef enum {
    e_dashboard, e_main_menu, e_view_log, e_set_time, e_download_log, e_clear_log
} State_t;


extern State_t state; // App state

//Function declarations

//Dashboard function declaration
void view_dashboard(void);

//Storing events function declaration
void event_store(void);

//Password function declaration
void password(void);

//main menu function declaration
void display_main_menu(void);

//View log function declaration
void view_log(void);

//Reading events function declaration
void event_reader(void);

//Change password function declaration
void change_password(void);

//Set time function declaration
void set_time(void);

//Download log function _decleration
void download_log(void);

//Clear log function declaration
void clear_log(void);

void clcd_write(unsigned char byte, unsigned char control_bit);

void init_clcd();

unsigned char scan_key();

unsigned char read_matrix_keypad(unsigned char detection);

void clcd_print(const unsigned char *data, unsigned char addr);

void clcd_putch(const unsigned char data, unsigned char addr);

void init_adc();

unsigned short read_adc();

unsigned char internal_eeprom_read(unsigned char addr);

void internal_eeprom_write(unsigned char addr,unsigned char data);

void swap_events_in_eeprom();

#endif	/* BLACK_H */


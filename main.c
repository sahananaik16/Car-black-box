/*
 * File:   main.c
 * Author: DELL
 *
 * Created on December 13, 2024, 5:21 PM
 */

#include "black.h" 

State_t state;

/* Function to initialize system configurations */
void init_config() {
    /* Initialize UART for communication */
    init_uart();
    /* Initialize ADC for analog-to-digital conversion */
    init_adc();
    /* Initialize the character LCD display */
    init_clcd();
    /* Initialize I2C communication */
    init_i2c();
    /* Initialize DS1307 Real-Time Clock */
    init_ds1307();
    /* Set TRISB register for specific pins as input */
    TRISB = 0x1E;
    /* Enable PORTB pull-up resistors */
    RBPU = 0;
    /* Set the initial state to dashboard */
    state = e_dashboard;
}

/* Function to read and format the current time */
static void get_time(void) {
    /* Read the hour,minute,second from RTC */
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    /* Check for 12-hour format */
    if (clock_reg[0] & 0x40) {
        /* Extract tens and unit of hour */
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    } else { /* For 24-hour format */
        /* Extract tens and unit of hour */
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    /* Extract tens and unit of minute */
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    /* Extract tens and unit of second */
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}

/* Main function */
void main(void) {
    /* Calling initialization function */
    init_config();

    /* Infinite loop to continuously run the system */
    while (1) {
        /* Retrieve the current time */
        get_time();

        /* Read input from the keypad */
        key = read_matrix_keypad(EDGE);

        /* Check if key 11 is pressed and state is set_time */
        if (key == 11 && state == e_set_time) {
            /* Reset scroll index and set save_exit flag*/
            scroll = 0;
            save_exit = 1;
        }
            /* Check if key 11 is pressed and state is not view_log */
        else if (key == 11 && state != e_view_log) {
            /* Clear the display screen */
            CLEAR_DISP_SCREEN;
            /* Increment state index and check value is 1 or not */
            if (++state_ind == 1) {
                /* Set state to main menu */
                state = e_main_menu;
            } else if (state_ind == 2) { /* If state index is 2 */
                /* Check scroll value to set state */
                switch (scroll) {
                    case 0:
                        /* Set state to view log */
                        state = e_view_log;
                        break;
                    case 1:
                        /* Set state to clear log */
                        state = e_clear_log;
                        break;
                    case 2:
                        /* Set state to download log */
                        state = e_download_log;
                        break;
                    case 3:
                        /* Set state to set time */
                        state = e_set_time;
                        break;
                }
                /* Reset state index */
                state_ind = 0;
            }
        }
            /* Check if key 12 is pressed and state is set_time */
        else if (key == 12 && state == e_set_time) {
            /* Reset upload flag scroll index*/
            upload = 0;
            scroll = 0;
            /* Clear the display screen */
            CLEAR_DISP_SCREEN;
            /* Set state to dashboard */
            state = e_dashboard;
        }
            /* Check if key 12 is pressed and state is not dashboard or main_menu */
        else if (key == 12 && state != e_dashboard && state != e_main_menu) {
            /* Reset display index */
            display_index = 0;
            /* Clear the display screen */
            CLEAR_DISP_SCREEN;
            /* Set state to main menu */
            state = e_main_menu;
        }
            /* If key 12 is pressed */
        else if (key == 12) {
            /* Clear the display screen */
            CLEAR_DISP_SCREEN;
            /* Reset scroll index */
            scroll = 0;
            /* Set state to dashboard */
            state = e_dashboard;
        }

        /* Execute actions based on the current state */
        switch (state) {
            case e_dashboard:
                /* Display the dashboard */
                view_dashboard();
                break;

            case e_main_menu:
                /* Display the main menu */
                display_main_menu();
                break;

            case e_view_log:
                /* View log data */
                view_log();
                break;

            case e_download_log:
                /* Download log data */
                download_log();
                break;

            case e_clear_log:
                /* Clear log data */
                clear_log();
                break;

            case e_set_time:
                /* Set the system time */
                set_time();
                break;
        }
    }
}


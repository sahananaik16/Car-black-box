/*
 * File:   dashboard.c
 * Author: DELL
 *
 * Created on December 13, 2024, 5:42 PM
 */


#include "black.h"

/* Function to manage the dashboard display and handle gear-related operations. */
void view_dashboard(void) {

    static int collision = 0, gear_store = 0;
    /* Reads the ADC (Analog-to-Digital Converter) value for speed and scales it. */
    speed = read_adc() / 10;
    /* Limits the speed to a maximum of 99. */
    if (speed >= 99) {
        speed = 99;
    }
    /* Checks if the "increment gear" key is pressed. */
    if (key == 1) {
        /* Resets collision state after incrementing gear. */
        if (collision == 1) {
            collision = 0;
            /* Resets the gear value. */
            gear_change = 0;
        }
        /* Increments the gear value. */
        gear_change++;
        /* Caps the gear at 7 if it exceeds the limit. */
        if (gear_change > 7) {
            gear_change = 7;
            /* Indicates that gear increment is restricted. */
            gear_store = 1;
        } else
            /* Allows further increments if within the limit. */
            gear_store = 0;
        /* Logs the gear increment event if within the range. */
        if (gear_store != 1) {
            event_store();
        }
        /* Checks if the "decrement gear" key is pressed. */
    } else if (key == 2) {
        /* Resets collision state after decrementing gear. */
        if (collision == 1) {
            collision = 0;
            /* Sets the gear to the minimum value of 1. */
            gear_change = 1;
        }
        /* Decrements the gear value. */
        gear_change--;
        /* Ensures the gear value does not go below 1. */
        if (gear_change < 1) {
            gear_change = 1;
            /* Indicates that gear decrement is restricted. */
            gear_store = 1;
        } else
            /* Allows further decrements if within the limit. */
            gear_store = 0;
        /* Logs the gear decrement event if within the range. */
        if (gear_store == 0) {
            event_store();
        }
        /* Checks if the "collision event" key is pressed. */
    } else if (key == 3) {
        /* Sets the gear to an invalid value to signify a collision. */
        gear_change = 8;
        /* Activates the collision state. */
        collision = 1;
        /* Logs the collision event. */
        event_store();
    }
    /* Reads the stored events from memory to process them. */
    event_reader();

    /* Updates the display with the time, gear, and speed information. */
    clcd_print("TIME    GEAR  SP", LINE1(0));
    clcd_print(time, LINE2(0));
    clcd_print(gear[gear_change], LINE2(9));
    clcd_putch((speed / 10) + '0', LINE2(14));
    clcd_putch((speed % 10) + '0', LINE2(15));

    return;
}

/* Function to display the main menu and handle navigation. */
void display_main_menu(void) {

    /* Scrolls down the menu options. */
    if (key == 2) {
        scroll++;

        /* Restricts scrolling beyond the last option. */
        if (scroll >= 3) {
            scroll = 3;
        }

        /* Scrolls up the menu options. */
    } else if (key == 1) {
        scroll--;

        /* Restricts scrolling above the first option. */
        if (scroll <= 0) {
            scroll = 0;
        }
    }

    /* Displays menu options based on the current scroll position. */
    if (scroll == 0) {
        clcd_print("* VIEW LOG        ", LINE1(0));
        clcd_print("  CLEAR LOG       ", LINE2(0));
    } else if (scroll == 1) {
        clcd_print("  VIEW LOG       ", LINE1(0));
        clcd_print("* CLEAR LOG      ", LINE2(0));
    } else if (scroll == 2) {
        clcd_print("  CLEAR LOG      ", LINE1(0));
        clcd_print("* DOWNLOAD LOG   ", LINE2(0));
    } else if (scroll == 3) {
        clcd_print("  DOWNLOAD LOG    ", LINE1(0));
        clcd_print("* SET TIME        ", LINE2(0));
    }
}

/* Function to store an event in the EEPROM */
void event_store(void) {
    /* Check if event count exceeds 10 and manage events in EEPROM accordingly */
    if (event_count > 10) {
        swap_events_in_eeprom(); /* Swap the events in EEPROM to free up space */
        event_addr = 90;         /* Reset the event address */
        event_count = 9;         /* Reset the event count */
    }

    int j = 0; 
    /* Store event data (time, gear, speed) in EEPROM */
    for (int i = 0; i < 10; i++) {
        if (i < 6) {
            if (time[j] == ':') { /* Skip colon in time format */
                j++;
            }
            write_external_eeprom(event_addr, time[j++]); /* Write time data */
        } else if (i >= 6 && i < 8)
            write_external_eeprom(event_addr, gear[gear_change][i - 6]); /* Write gear data */
        else if (i == 8)
            write_external_eeprom(event_addr, (speed / 10) + '0'); /* Write speed tens digit */
        else if (i == 9)
            write_external_eeprom(event_addr, (speed % 10) + '0'); /* Write speed units digit */
        event_addr++;
    }
    /* Increment event count */
    event_count++; 
}

/* Function to read events from the EEPROM into a data store */
void event_reader(void) {
    /* Determine the starting address and number of events to read */
    if (event_count > 10) {
        /* Read last 10 events */
        read_addr = event_addr - (10 * 10); 
        events_to_read = 10;
    } else {
        /* Read available events */
        read_addr = event_addr - (event_count * 10); 
        events_to_read = event_count;
    }

    /* Read the events from EEPROM into the data store */
    for (int j = 0; j < events_to_read; j++) {
        for (int i = 0; i < 10; i++) {
            data_store[j][i] = read_external_eeprom(read_addr++);
        }
    }
}

/* Function to display logs on the LCD */
void view_log(void) {
    /* Check if there are logs to display */
    if (events_to_read == 0) {
        clcd_print("NO LOGS         ", LINE1(0));
        clcd_print("   TO DISPLAY :(", LINE2(0));
        __delay_ms(1000);
        state = e_main_menu;
    } else {
        /* Handle navigation through logs */
        if (key == 1) {
            display_index++;
            if (display_index >= events_to_read) {
                /* Limit to the last log */
                display_index = events_to_read - 1; 
            }
        }
        if (key == 2) {
            display_index--;
            if (display_index >= events_to_read) {
                /* Limit to the first log */
                display_index = 0; 
            }
        }

        /* Display the current log on the LCD */
        clcd_print("# TIME   GEAR SP", LINE1(0));
        clcd_putch(display_index % 10 + '0', LINE2(0));
        clcd_putch(' ', LINE2(1));
        clcd_putch(data_store[display_index][0], LINE2(2));
        clcd_putch(data_store[display_index][1], LINE2(3));
        clcd_putch(':', LINE2(4));
        clcd_putch(data_store[display_index][2], LINE2(5));
        clcd_putch(data_store[display_index][3], LINE2(6));
        clcd_putch(':', LINE2(7));
        clcd_putch(data_store[display_index][4], LINE2(8));
        clcd_putch(data_store[display_index][5], LINE2(9));
        clcd_putch(' ', LINE2(10));
        clcd_putch(data_store[display_index][6], LINE2(11));
        clcd_putch(data_store[display_index][7], LINE2(12));
        clcd_putch(' ', LINE2(13));
        clcd_putch(data_store[display_index][8], LINE2(14));
        clcd_putch(data_store[display_index][9], LINE2(15));
    }
}


/* Function to clear all logs stored in EEPROM */
void clear_log(void) {
    /* Reset the number of events to read and event count*/
    events_to_read = 0;  
    event_count = 0;  
    /* Clear the display */
    CLEAR_DISP_SCREEN;   
    /* Show clearing message */
    clcd_print("CLEARING LOGS...", LINE1(0)); 
    clcd_print("JUST A MINUTE   ", LINE2(0)); 
    __delay_ms(1000);  
    /* Go back to the main menu */
    state = e_main_menu;
    /* Clear the display*/
    CLEAR_DISP_SCREEN;   
}

/* Function to download the logs over UART */
void download_log(void) {
    int count = 0, k = 0;
    
    /* Check if there are any logs available for download */
    if (events_to_read == 0) {
        /*printing message indicating no logs available to download*/
        puts("NO LOGS AVAILABLE :(\n\r");
        clcd_print("NO LOGS         ", LINE1(0));
        clcd_print("  TO DOWNLOAD :(", LINE2(0));
        __delay_ms(1000);
         /* Return to the main menu */
        state = e_main_menu; 
        return;
    }
    /* Print header for the log data */
    puts("#  TIME   GEAR SPEED\n\r");  
    
    /* Loop through each log and send it over UART */
    for (int j = 0; j < events_to_read; j++) {
        k = 0;
        for (int i = 0; i < 17; i++) {
            if (i == 0) {
                /* Print the log index */
                putch(count + '0');  
                count++;
            } else if (i == 1 || i == 10 || i == 13 || i == 14) {
                /* Print spaces for formatting */
                putch(' ');  
            } else if (i == 4 || i == 7) {
                /* Print the colon in time format */
                putch(':');  
            } else {
                /* Print the log data */
                putch(data_store[j][k++]);  
            }
        }
        /* New line after each log */
        puts("\n\r");  
    }
    /* Show downloading message */
    clcd_print("DOWNLOADING...  ", LINE1(0)); 
    clcd_print("THROUGH UART... ", LINE2(0)); 
    __delay_ms(1000);  
    /* Return to the main menu */
    state = e_main_menu; 
}

/* Function to set the time (HH:MM:SS format) */
void set_time(void) {
    /* Used to control the display update rate */
    static unsigned char wait = 0;  
    
    /* Display the time format */
    clcd_print("HH:MM:SS        ", LINE1(0));  
    
    /* Upload the initial time if not done yet */
    if (upload == 0) {
        /* Get hour,minute,second from input */
        hr = (time[0] - '0')*10 + (time[1] - '0');  
        min = (time[3] - '0')*10 + (time[4] - '0');  
        sec = (time[6] - '0')*10 + (time[7] - '0');  
        upload = 1;  /* Mark as uploaded */
    }

    /* Allow user to cycle through hour, minute, and second fields */
    if (key == 2) {
        field++;
        if (field > 2) {
            /* Loop back to hour field */
            field = 0;  
        }
    }

    /* Handle changes for the hour field */
    if (field == 0) {
        if (key == 1) {
            /* Increment the hour */
            hr++;  
            /* Wrap around if hour exceeds 23 */
            if (hr > 23) hr = 0;  
        }
        if (wait++ == 80) {
            /* Display hour tens and ones place */
            clcd_putch((hr / 10) + '0', LINE2(0));  
            clcd_putch((hr % 10) + '0', LINE2(1));  
        } else if (wait >= 160) {
            /* Clear hour display after time update */
            clcd_putch(' ', LINE2(0));  
            clcd_putch(' ', LINE2(1));
            /* Reset wait counter */
            wait = 0;  
        }
        /* Display minute tens,ones place */
        clcd_putch((min / 10) + '0', LINE2(3));  
        clcd_putch((min % 10) + '0', LINE2(4));  
        /* Display second tens,ones place */
        clcd_putch((sec / 10) + '0', LINE2(6));  
        clcd_putch((sec % 10) + '0', LINE2(7));  
    }

    /* Handle changes for the minute field */
    if (field == 1) {
        if (key == 1) {
            min++;  /* Increment the minute */
            if (min > 59) min = 0;  /* Wrap around if minute exceeds 59 */
        }
        if (wait++ == 80) {
            /* Display minute tens and ones place */
            clcd_putch((min / 10) + '0', LINE2(3));  
            clcd_putch((min % 10) + '0', LINE2(4));  
        } else if (wait >= 160) {
            /* Clear minute display after time update */
            clcd_putch(' ', LINE2(3));  
            clcd_putch(' ', LINE2(4));
            wait = 0;  /* Reset wait counter */
        }
        /* Display hour tens,ones place */
        clcd_putch((hr / 10) + '0', LINE2(0));  
        clcd_putch((hr % 10) + '0', LINE2(1)); 
        /* Display second tens,ones place */
        clcd_putch((sec / 10) + '0', LINE2(6));  
        clcd_putch((sec % 10) + '0', LINE2(7));  
    }

    /* Handle changes for the second field */
    if (field == 2) {
        if (key == 1) {
            /* Increment the second */
            sec++;  
            /* Wrap around if second exceeds 59 */
            if (sec > 59) sec = 0;  
        }
        if (wait++ == 80) {
            /* Display second tens,ones place */
            clcd_putch((sec / 10) + '0', LINE2(6));  
            clcd_putch((sec % 10) + '0', LINE2(7));  
        } else if (wait >= 160) {
             /* Clear second display after time update */
            clcd_putch(' ', LINE2(6)); 
            clcd_putch(' ', LINE2(7));
            /* Reset wait counter */
            wait = 0;  
        }
        /* Display hour tens,ones place */
        clcd_putch((hr / 10) + '0', LINE2(0));  
        clcd_putch((hr % 10) + '0', LINE2(1));  
        /* Display minute tens,ones place */
        clcd_putch((min / 10) + '0', LINE2(3));  
        clcd_putch((min % 10) + '0', LINE2(4));  
    }

    /* Save the time to DS1307 RTC and exit the time setting */
    if (save_exit == 1) {
        /* Convert hour,minute,second to BCD format */
        hr = ((hr / 10) << 4) | (hr % 10);  
        min = ((min / 10) << 4) | (min % 10);  
        sec = ((sec / 10) << 4) | (sec % 10); 
        /* Write the second,hour,minute to DS1307 */
        write_ds1307(SEC_ADDR, sec);  
        write_ds1307(HOUR_ADDR, hr);  
        write_ds1307(MIN_ADDR, min);  
        /* Reset upload flag and save exit flag */
        upload = 0;  
        save_exit = 0;  
        /* Return to the dashboard state */
        state = e_dashboard;  
    }

    /* Display the time separator */
    clcd_putch(':', LINE2(2));  
    clcd_putch(':', LINE2(5));  
}

/* Function to swap events in EEPROM when event count exceeds 10 */
void swap_events_in_eeprom() {
    /* Loop through the last 9 events */
    for (int i = 0; i < 9; i++) {  
        /* Loop through each byte of the event */
        for (int j = 0; j < 10; j++) {  
            /* Shift event data */
            write_external_eeprom((i * 10) + j, read_external_eeprom(((i + 1) * 10) + j));  
        }
    }
}
 
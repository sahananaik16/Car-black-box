/*
 * File:   matrix_keypad.c
 * Author: DELL
 *
 * Created on December 3, 2024, 4:58 PM
 */


#include "black.h"

/* Function to scan the keys of the keypad */
unsigned char scan_key() {
    /* Set RB5 low to enable scanning. Set RB6 and RB7 high */
    RB5 = 0;
    RB6 = RB7 = 1;

    /* Check each row and return corresponding key */
    if (RB1 == 0)
        return 1;
    else if (RB2 == 0)
        return 4;
    else if (RB3 == 0)
        return 7;
    else if (RB4 == 0)
        return 10;

    /* Set RB6 low to switch to the next row. Set RB5 and RB7 high*/
    RB6 = 0;
    RB5 = RB7 = 1;

    /* Check each row and return corresponding key */
    if (RB1 == 0)
        return 2;
    else if (RB2 == 0)
        return 5;
    else if (RB3 == 0)
        return 8;
    else if (RB4 == 0)
        return 11;

    /* Set RB7 low to switch to the next row. Set RB5 and RB6 high */
    RB7 = 0;
    RB6 = RB5 = 1;

    /* Check each row and return corresponding key */
    if (RB1 == 0)
        return 3;
    else if (RB2 == 0)
        return 6;
    else if (RB3 == 0)
        return 9;
    else if (RB4 == 0)
        return 12;

    /* Return 0xFF if no key is pressed */
    return 0xFF;
}

/* Function to read the matrix keypad with debouncing */
unsigned char read_matrix_keypad(unsigned char detection) {
    /* Static variable to ensure key press is detected only once */
    static unsigned once = 1;
    /* Scan the keypad to detect key press */
    unsigned char key = scan_key();

    /* If detection type is 0, return the scanned key immediately */
    if (detection == 0) {
        return key;
    }/* If detection type is 1, handle debouncing logic */
    else if (detection == 1) {
        /* If a key is pressed and it's the first time after debounce, return the key */
        if ((key != 0xFF) && once) {
            once = 0; /* Mark that a key press has been detected */
            return key; /* Return the pressed key */
        }
            /* If no key is pressed, reset the debounce flag */
        else if (key == 0xFF) {
            once = 1; /* Reset the debounce flag */
        }
        return 0xFF; /* Return 0xFF if no key is pressed or debouncing is needed */
    }
}
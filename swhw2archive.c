/* 
 * File:   swhw2Main.c
 * Author: atowne
 *
 * Created on September 15, 2014, 7:19 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDlib.h"
#include "SDlib_delay.h"
#include "SDlib_LCD.h"
#include "configbitsrev2014vC.h"
#include <xc.h>

/*
 * 
 */
int main(void) {

    // initialize strings
    char* myname;
    char* helloworld;
    myname = "Ashley Towne";
    helloworld = "Hello World!";
    serial_init(57600);
    
    // initialize LCD
    LCD_init();
    LCD_clear();
    LCD_display_on();
    set_output_device(2); // select LCD as output
    LCD_setpos(0,0);
    printf("This goes to LCD \r \n"); // display on LCD (overwritten immediately)

    // output to LCD
    LCD_clear();
    LCD_setpos(0,0);
    printf("%s",myname);
    LCD_setpos(1,0);
    printf("%s",helloworld);

    // output to terminal
    set_output_device(1); // select UART as output device
    printf("This goes to terminal\n"); // write to the screen
    printf("%s\n%s\n", myname, helloworld);

    // loop
    while(1)
    {
        unsigned char myinput;
        myinput = getu(); // get input from keyboard
        set_output_device(2);
        putu(myinput);
        set_output_device(1);
        LCD_char(myinput);
    }
    return (EXIT_SUCCESS);
}


/*
 * File:   board2usbMain.c
 * Author: ashley
 *
 * Created on February 4, 2015, 12:55 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDlib.h"
#include "configbitsrev2014vC.h"

/*
 *
 */
int main(void) {

    // initialize strings
    char* myname;
    char* helloworld;
    myname = "Ashley Towne";
    helloworld = "Hello World!";
    serial_init(9600);

    // initialize LCD
    LCD_init();
    LCD_display_on();
    set_output_device(2); // select LCD as output

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
    int foo = 0;
    unsigned char cfoo;
    while(1)
    {
        unsigned char myinput;
        //myinput = getu(); // get input from keyboard
        foo = foo + 1;
        if (foo==0){myinput='a';}
        else if(foo==1){myinput='b';}
        else if(foo==2){myinput='c';}
        else if(foo==3){myinput='d';}
        else if(foo==4){myinput='e';}
        else if(foo==5){myinput='f';}
        else if(foo==6){myinput='g';}
        else if(foo==7){myinput='h';}
        else if(foo==8){myinput='i';}
        else {myinput='9';foo=-1;}


        set_output_device(2);
        putu(myinput);
        //putu(foo);
        set_output_device(1);
        LCD_char(myinput);
        //LCD_char(foo);
        //LCD_char(cfoo);
    }
    return (EXIT_SUCCESS);
}



/*
 * File:   board2usbMain.c
 * Author: #pragma
 *
 * Created on February 4, 2015, 12:55 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDlib.h"
#include "configbitsrev2014vC.h"
#include <math.h>
/*
 *
 */
void serial_init6(unsigned long rate);
char getu6();
void putu6(char output);


int main(void) {

    // initialize strings
    char* myname;
    char* helloworld;
    myname = "Ashley Towne";
    helloworld = "Hello World!";
    serial_init(57600);

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
    char *myinput = "0123456789";
    int ctr = 0;
    while(1)
    {
        unsigned char myinput;

        set_output_device(2);
        putu(myinput[ctr]);
        //putu6(myinput[ctr]);
        set_output_device(1);
        LCD_char(myinput);

        ctr++;
        if (ctr>strlen(myinput)){ctr = 0;}
    }

    return (EXIT_SUCCESS);
}


void serial_init6(unsigned long rate){
    U6MODEbits.ON = 1;      //Enable UART6
    U6MODEbits.BRGH = 1;    //Enable High Baud Rate
    U6STAbits.URXEN = 1;    //Enable UART6 Receiver
    U6STAbits.UTXEN = 1;    //Enable UART Transmitter

    long PBclock = get_pb_clock(); //Get peripheral bus clock frequency
    U6BRG = floor(PBclock/(4*rate)-1);  //Calculate and set U6BRG
}

char getu6(){
    char input;
    int b = 0;
    while(b == 0)                    //Wait for input
    {
        if(U6STAbits.URXDA == 1)
        {
            input = U6RXREG;         //Set input
            b = 1;                   //Exit loop
        }

    }
    return input;                    //Return input
}

void putu6(char output){
    int b = 0;
    while(b == 0)                     //Wait for output
    {
        if(U6STAbits.URXDA == 1)
        {
            U6TXREG = output;         //Set output
            b = 1;                    //Exit loop
        }
    }
}


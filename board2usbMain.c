/*
 * File:   board2usbMain.c
 * Author: #pragma
 *
 * Created on February 4, 2015, 12:55 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/attribs.h>
#include <cp0defs.h>

#include "SDlib.h"
#include "configbitsrev2014vC.h"
#include "RegisterConstants.h"
#include "SPIFunctions.h"
#include "A2DFunctions.h"
#include "Timer.h"

/*
 *
 */
// UART 6
void serial_init6(unsigned long rate);
char getu6();
void putu6(char output);

// transmit test - proof of concept
int xmitText(void);
void setLCDdefaults(void);

int main(void) {
    int bar = 255;

    asm volatile("ei");
    INTCONbits.MVEC = 1;
    TRISE = 0;
    LATE = bar;
    xmitTest();
    
    //////////////////////////////////////////////////
    unsigned int inval = 0x4145;
    // ADC must be initialized before timer
    init_ADC();
    timer_init();
    timer_enable(10);


    
    return (EXIT_SUCCESS);
}

void setLCDdefaults(void){
    LCD_init();
    LCD_clear();
    LCD_setpos(0,0);
    set_output_device(1);
}

int xmitTest(void){
    // initialize strings
    serial_init(9600);

    // output to terminal
    set_output_device(1); // select UART as output device
    printf("START\n"); // write to the screen
    printf("eof = %u\n",EOF); // 0xFFFFFFFF (4 bytes)
    return 1;
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

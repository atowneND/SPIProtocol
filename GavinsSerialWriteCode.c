/* 
 * File:   SA3main.c
 * Author: ghsu1
 *
 * Created on September 22, 2014, 8:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDlib.h"
#include "configbitsrev2014vC.h"
#include <xc.h>
#include <math.h>

void serial_init6(unsigned long rate);
char getu6();
void putu6(char output);

/*
 * 
 */
int main() {
    char type; //Used for reading input
    serial_init6(57600); //Initialize UART6 and set Baud Rate
    printf("Gavin Hsu\n");
    while(1)
    {
        type = getu6(); //Get input character
        putu6(type);    //Output character
    }
    
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

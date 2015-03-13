/*
 * File:   board2usbMain.c
 * Author: #pragma
 *
 * Created on February 4, 2015, 12:55 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDlib.h"
#include "configbitsrev2014vC.h"
#include "SPIConstants.h"

/*
 *
 */
void serial_init6(unsigned long rate);
char getu6();
void putu6(char output);
int xmitText(void);
int initSPI2MasterPIC(void);
int initSPIFlash(void);
int write2SPI(int address);
int readSPI(int address);
int sendByte2SPI(int data);

int main(void) {
    xmitTest();
    LCD_clear();
    LCD_setpos(0,0);
    set_output_device(2);
    printf("%lu",get_pb_clock());

    int foo = initSPI2Master();
    LCD_setpos(1,0);
    printf("%i",foo);

    foo = 1;
    foo = initSPI2Master();
    printf(" %i",foo);
    
    foo = 1;
    foo = initSPIFlash();
    printf(" %i",foo);

    foo = 1;
    LCD_setpos(2,0);
    printf("foo=%i *** ",foo);
    foo = sendByte2SPI(RDSR);
    printf("0?=%i",foo);
    
    return (EXIT_SUCCESS);
}

int sendByte2SPI(int data){
    SPIREG_Buffer = data;
    int regstat = SPIREG_Buffer;
    return regstat;
}

int initSPI2Master(void){
    int rData;

    // reset
    REG_Interrupt.SPI4EIE = 0; // (bit 8) disable error interrupt
    REG_Interrupt.SPI4RXIE = 0; // (bit 9) disable receive interrupt
    REG_Interrupt.SPI4TXIE = 0; // (bit 10) disable transmit interrupt
    SPIREG_Control.ON = 0; // turn SPI off
    SPIREG_Buffer = 0; // clear buffer
    rData = SPIREG_Buffer; // clear receiving buffer

    // interrupt settings
    REG_Flag.SPI4EIF = 0; // (bit 8) clear error flag
    REG_Flag.SPI4RXIF = 0; // (bit 9) clear receive flag
    REG_Flag.SPI4TXIF = 0; // (bit 10) clear transmit flag
    // add interrupt priority settings?

    // setup
    SPIREG_Baud_Rate = 0; // set BRG
        // pb clock is now 10MHz -> 5MHz baud rate
    SPIREG_Status.SPIROV = 0; // clear overflow
    SPIREG_Control.MSTEN = 1; // set to master

    // settings - data changes on clock's falling edge, takes data from rising edge
    SPIREG_Control.CKE = 1; // data changes on clock edge from active to idle
    SPIREG_Control.CKP = 0; // clock is active high
    SPIREG_Control.SMP = 0; // take data in middle of cycle

    // set to 16-bit data chunks
    SPIREG_Control.MODE32 = 0;
    SPIREG_Control.MODE16 = 1;

    // set pins
    REG_Analog_Digital = 0xFFFF; // sets all to digital
        // needs to account for ADC (B:8,10,11,14 need to be digital)
    REG_JTAG = 0; // disable JTAG on B10, B11, B12

    // enable interrupts
    REG_Interrupt.SPI4EIE = 1;
    REG_Interrupt.SPI4RXIE = 1;
    REG_Interrupt.SPI4TXIE = 1;
    
    // enable SPI operation
    SPIREG_Control.ON = 1;

    return rData; // should be zero
}

int initSPIFlash(void){

    // initialize settings - set AAI to zero
    // clear all
        // write enable
    int foo;
    foo = sendByte2SPI(WREN);
    LCD_setpos(3,0);printf("%i ",foo);
    foo = sendByte2SPI(ERASE_ALL);
    printf("%i ",foo);
        // erase
    return 0;
}

int write2SPI(int address){
    // try AAI later if possible, to optimize speed
    
    // write enable
    // check if memory needs to be erased; erase if necessary and  write-enable
    // byte-program
    // address (3 bytes)
    // data (1 byte)
    // write disable
    return address;
}
int readSPI(int address){
    // read enable
    // address (3 bytes)
    // dummy byte
    return address;
}

int xmitTest(void){
    // initialize strings
    char* myname;
    myname = "Ashley Towne";
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

    // output to terminal
    set_output_device(1); // select UART as output device
    printf("This goes to terminal\n"); // write to the screen
    printf("%s\n", myname);
    // loop
    int foo = 0;
    unsigned char mystr;
    int ctr = 0;
    while(ctr<10)
    {
        ctr = ctr + 1;
        if (foo==0){mystr='0';}
        else if(foo==1){mystr = '1';}
        else if(foo==2){mystr = '2';}
        else if(foo==3){mystr = '3';}
        else if(foo==4){mystr = '4';}
        else if(foo==5){mystr = '5';}
        else if(foo==6){mystr = '6';}
        else if(foo==7){mystr = '7';}
        else if(foo==8){mystr = '8';}
        else if(foo==9){
            mystr = '9';
            foo = -1;
        }

        set_output_device(2);
        putu((char)(100+foo));
        set_output_device(1);
        LCD_char((char)(100+foo));

        foo = foo + 1;
    }

    putu('z');
    LCD_char('z');
    putu(EOF);
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


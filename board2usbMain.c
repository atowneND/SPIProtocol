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
#include "SPIFunctions.h"

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
    TRISE = 0;
    // trigger
    LATE = 255;
    LATE = 0;
    //xmitTest();
    setLCDdefaults();
    printf("hi");
    // initialize and print output
    initSPI2Master();
    write2AllEnable();

    // high while erasing
    unsigned char foo;
    //foo = eraseSPIFlash(); // erase all
    
    // read device ID
    SPI_CE = 0;
    foo = readID();
    SPI_CE = 1;
        
    // read status register
    //SPI_CE = 0;    
    //printStatReg();
    //SPI_CE = 1;

    unsigned char address[2];
    address[0] = 0x00;address[1]=0x00;address[2]=0x00;
    unsigned char data = 'B';
    
    // write and read
    printf("r/w");
    foo = write2SPI(address,data);
    printf("0");
    foo = readSPI(address);
    printf("1");
    printf("%c",foo);
    address[0] = address[0] + 1;
    foo = write2SPI(address,data+1);
    printf("2");
    foo = readSPI(address);
    printf("3");
    printf("%c",foo);
    address[0] = address[0] + 1;

    unsigned int adcData = 0x6869;
    unsigned char adcStr[2];
    memcpy(adcStr,(char*)&adcData,2);

    foo = write2SPI(address,adcStr[0]);
    address[0] = address[0] + 1;
    foo = write2SPI(address,adcStr[1]);
    foo = readSPI(address-1);
    unsigned char foo2 = readSPI(address);
  //  LCD_clear();
    LCD_setpos(0,0);
    printf("%c%c",foo,foo2);
    
    return (EXIT_SUCCESS);
}

void setLCDdefaults(void){
    LCD_init();
    LCD_clear();
    LCD_setpos(0,0);
    set_output_device(2);
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

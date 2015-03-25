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
// UART 6
void serial_init6(unsigned long rate);
char getu6();
void putu6(char output);

// transmit test - proof of concept
int xmitText(void);

// SPI stuff
void initSPI2Master(void);
unsigned short initSPIFlash(void);
unsigned short write2SPI(int address[], int data);
unsigned short readSPI(int address[]);
unsigned short sendByte2SPI(int data);

int main(void) {
// test
    xmitTest();
    LCD_clear();
    LCD_setpos(0,0);
    set_output_device(2);

// initialize and print output
    initSPI2Master();
    unsigned short foo = initSPIFlash();
    printf("%u",foo);
    LCD_setpos(1,0);
    
// read status register
    SPI_SCK = 0;
    foo = sendByte2SPI(RDSR);
    SPI_SCK = 1;
    printf("%u",foo);
    int address[3];
    address[1] = 0x00;address[2]=0x00;address[3]=0x00;
    //address[1] = 0;address[2] = 0;address[3] = 0;
    int data = 0x42;
    // int data = 101;
    foo = write2SPI(address,data);
    printf("%u",foo);
    LCD_setpos(2,0);
    printf("foo=");
    foo = readSPI(address);
    printf("%u",foo);
    return (EXIT_SUCCESS);
}

void initSPI2Master(void){

    // reset
    REG_Interrupt.SPIEIE = 0; // (bit 8) disable error interrupt
    REG_Interrupt.SPIRXIE = 0; // (bit 9) disable receive interrupt
    REG_Interrupt.SPITXIE = 0; // (bit 10) disable transmit interrupt
    SPIREG_Control.ON = 0; // turn SPI off
    SPIREG_Buffer = 0; // clear buffer

    // interrupt settings
    REG_Flag.SPIEIF = 0; // (bit 8) clear error flag
    REG_Flag.SPIRXIF = 0; // (bit 9) clear receive flag
    REG_Flag.SPITXIF = 0; // (bit 10) clear transmit flag
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
    REG_Interrupt.SPIEIE = 1;
    REG_Interrupt.SPIRXIE = 1;
    REG_Interrupt.SPITXIE = 1;
    
    SPI_TRIS = 0; // set I/O
    SPI_SCK = 1; // don't talk to the SPI right now

    // enable SPI operation
    SPIREG_Control.ON = 1;
}

unsigned short sendByte2SPI(int data){
// sends a single command to SPI - back end
// waits for transmit buffer to be empty, sends data, waits for flag, waits for
// return value (buffer register)
    unsigned short regstat;

    while(!SPIREG_Status.SPITBE); // active wait if transmit buffer is not empty
    SPIREG_Buffer = data; // write data to buffer

    while(!REG_Flag.SPIRXIF); // while the interrupt flag does not signal done
    REG_Flag.SPIRXIF = 0; // clear flag
    while(!SPIREG_Status.SPIRBF); // wait for buffer to be full
    regstat = SPIREG_Buffer; // return buffer register
    return regstat;
}

unsigned short initSPIFlash(void){

    // initialize settings - set AAI to zero
    unsigned short foo;
    
    SPI_SCK = 0;
    foo = sendByte2SPI(WREN); // write enable
    SPI_SCK = 1;
    SPI_SCK = 0;
    foo = sendByte2SPI(ERASE_ALL); // erase all
    SPI_SCK = 1;
    SPI_SCK = 0;
    foo = sendByte2SPI(WRDI); // write disable
    SPI_SCK = 1;
    return foo;
}

unsigned short write2SPI(int address[], int data){
    // try AAI later if possible, to optimize speed
    unsigned short foo;
    // SCK4 = 29/RB14
    // B22/RF13,L6
    // AN14/SCK4/U5TX/!U2RTSU2RTS/PMALH/PMA1/RB14

    // write enable
    SPI_SCK = 0;
    foo = sendByte2SPI(WREN);
    SPI_SCK = 1;
    // CHECK if memory needs to be erased; erase if necessary and  write-enable
    // byte-program
    SPI_SCK = 0;
    foo = sendByte2SPI(BYTE_PROGRAM);
    SPI_SCK = 1;
    // address (3 bytes)
    SPI_SCK = 0;
    foo = sendByte2SPI(address[1]);
    foo = sendByte2SPI(address[2]);
    foo = sendByte2SPI(address[3]);
    SPI_SCK = 1;
    // data (1 byte)
    SPI_SCK = 0;
    foo = sendByte2SPI(data);
    SPI_SCK = 1;
    // wait for write
    while(!SPIREG_Status.SPIBUSY);
    // write disable
    SPI_SCK = 0;
    foo = sendByte2SPI(WRDI);
    SPI_SCK = 1;
    return foo;
}

unsigned short readSPI(int address[]){
    // read enable
    unsigned short foo;
    SPI_SCK = 0;
    foo = sendByte2SPI(READ);
    SPI_SCK = 1;
    // address (3 bytes)
    SPI_SCK = 0;
    foo = sendByte2SPI(address[1]);
    foo = sendByte2SPI(address[2]);
    foo = sendByte2SPI(address[3]);
    SPI_SCK = 1;
    // dummy byte
    SPI_SCK = 0;
    foo = sendByte2SPI(0x00);
    SPI_SCK = 1;
    return foo;
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


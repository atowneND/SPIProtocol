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
unsigned char eraseSPIFlash(void);
void write2AllEnable(void);
unsigned char write2SPI(unsigned char address[], unsigned char data);
unsigned char readSPI(unsigned char address[]);
unsigned char sendByte2SPI(unsigned char data);
unsigned char readID(void);
int checkWIP(void);
void printStatReg(void);

int main(void) {
    TRISE = 0;
    // trigger
    LATE = 255;
    LATE = 0;
    xmitTest();
    LCD_clear();
    LCD_setpos(0,0);
    set_output_device(2);

// initialize and print output
    LATE = 255;
   // write2AllEnable();
    initSPI2Master();
    LATE = 0;
    
    // high while erasing
    LATE = 255;
    unsigned char foo;
    foo = eraseSPIFlash(); // erase all
    LCD_setpos(1,0);
    LATE = 0;
    
    // read device ID
    LATE = 255;
    SPI_CE = 0;
    foo = readID();
    SPI_CE = 1;
    LATE = 0;
    
// read status register
    LATE = 255;
    SPI_CE = 0;    
    foo = sendByte2SPI(RDSR);
    foo = sendByte2SPI(0);
    SPI_CE = 1;
    LATE = 0;

    unsigned char address[2];
    address[0] = 0x00;address[1]=0x00;address[2]=0x00;
    unsigned char data = 'B';
    
    // high while writing to SPI
    LATE = 255;
    foo = write2SPI(address,data);
    LATE = 0;

    // high while reading from SPI
    LATE = 255;
    foo = readSPI(address);
    LATE = 0;

    // high for finished
    LATE = 255;
    return (EXIT_SUCCESS);
}

void initSPI2Master(void){

    // reset
    REG_Interrupt.SPIEIE = 0; // (bit 8) disable error interrupt
    REG_Interrupt.SPIRXIE = 0; // (bit 9) disable receive interrupt
    REG_Interrupt.SPITXIE = 0; // (bit 10) disable transmit interrupt
    SPIREG_Control.ON = 0; // turn     //unsigned char foo = sendByte2SPI(RDID);SPI off
    SPIREG_Buffer = 0; // clear buffer

    // interrupt settings
    REG_Flag.SPIEIF = 1; // (bit 8) clear error flag
    REG_Flag.SPIRXIF = 0; // (bit 9) clear receive flag
    REG_Flag.SPITXIF = 0; // (bit 10) clear transmit flag
    
    // interrupt priority settings
    REG_IPC.SPI_Priority = 3; // set priority to 3

    // setup
    SPIREG_Baud_Rate = 255; // set BRG
        // pb clock is now 10MHz -> 5MHz baud rate
    SPIREG_Status.SPIROV = 0; // clear overflow
    SPIREG_Control.MSTEN = 1; // set to master

    // settings - data changes on clock's falling edge, takes data from rising edge
    SPIREG_Control.CKE = 1; // data changes on clock edge from active to idle
    SPIREG_Control.CKP = 0; // clock is active high
    SPIREG_Control.SMP = 0; // take data in middle of cycle

    // set pins
    REG_Analog_Digital = 0xFFFF; // sets all to digital
        // needs to account for ADC (B:8,10,11,14 need to be digital)
    REG_JTAG = 0; // disable JTAG on B10, B11, B12

    // enable interrupts
    REG_Interrupt.SPIEIE = 1;
    REG_Interrupt.SPIRXIE = 1;
    REG_Interrupt.SPITXIE = 1;
    
    SPI_TRIS = 0; // set I/O
    TRISFbits.TRISF4 = 1;
    TRISFbits.TRISF5 = 0;
    SPI_CE = 1; // don't talk to the SPI right now
    
    // enable SPI operation
    SPIREG_Control.ON = 1;
}

unsigned char sendByte2SPI(unsigned char data){
// sends a single command to SPI - back end
// waits for transmit buffer to be empty, sends data, waits for flag, waits for
// return value (buffer register)
    unsigned char regstat;
    while(!SPIREG_Status.SPITBE); // active wait if transmit buffer is not empty
    SPIREG_Buffer = data; // write data to buffer

    while(!REG_Flag.SPIRXIF); // while the interrupt flag does not signal done
    REG_Flag.SPIRXIF = 0; // clear flag
    while(!SPIREG_Status.SPIRBF); // wait for buffer to be full
    regstat = SPIREG_Buffer; // return buffer register

    return regstat;
}

unsigned char eraseSPIFlash(void){

    // initialize settings - set AAI to zero
    unsigned char foo;
    
    SPI_CE = 0;    
    foo = sendByte2SPI(WREN); // write enable
    SPI_CE = 1;    
    SPI_CE = 0;    
    foo = sendByte2SPI(ERASE_ALL); // erase all
    SPI_CE = 1;
   
    return foo;
}

void write2AllEnable(void){
    SPI_CE = 0;
    // enable write status register
    int foo = sendByte2SPI(EWSR);
    SPI_CE = 1;
    SPI_CE = 0;
    // write 0x00 to status register - chmod +w *
    foo = sendByte2SPI(WRSR);
    foo = sendByte2SPI(0x00);
    SPI_CE = 1;
    SPI_CE = 0;
    // disable write
    foo = sendByte2SPI(WRDI);
    SPI_CE = 1;
}

unsigned char write2SPI(unsigned char address[], unsigned char data){
    // try AAI later if possible, to optimize speed
    unsigned char foo;

    // write enable
    SPI_CE = 0;    
    foo = sendByte2SPI(WREN);
    SPI_CE = 1;
    printStatReg();
    SPI_CE = 0;
    // CHECK if memory needs to be erased; erase if necessary and  write-enable
    // byte-program
    foo = sendByte2SPI(BYTE_PROGRAM);
    // address (3 bytes)
    foo = sendByte2SPI(address[0]);
    foo = sendByte2SPI(address[1]);
    foo = sendByte2SPI(address[2]);
    // data (1 byte)
    foo = sendByte2SPI(data);
    SPI_CE = 1;
    SPI_CE = 0;
    // wait for write - check least significant bit of status register
    foo = sendByte2SPI(RDSR);
    int statReg = foo & 1;
    while(statReg){ // while busy
        foo = sendByte2SPI(RDSR);
        statReg = foo & 1;
    }
    
    SPI_CE = 1;
    
    printStatReg();

    foo = SPIREG_Buffer;
    return foo;
}

int checkWIP(void){
    char foo = sendByte2SPI(RDSR);
    int statReg = foo & 1;
    return statReg;
}

void printStatReg(void){
    SPI_CE = 0;
    char foo = sendByte2SPI(RDSR);
    foo = sendByte2SPI(0);
    SPI_CE = 1;
    int i;
    printf("SR:");
    for (i=7;i>=0;i--){
        printf("%i",(foo>>i)&1);
    }
}

unsigned char readSPI(unsigned char address[]){
    // read enable
    unsigned char foo;
    SPI_CE = 0;    
    foo = sendByte2SPI(READ);
    // address (3 bytes)
    foo = sendByte2SPI(address[0]);
    foo = sendByte2SPI(address[1]);
    foo = sendByte2SPI(address[2]);
    
    // dummy byte
    foo = sendByte2SPI(0x00);
    foo = sendByte2SPI(0x00);
    foo = sendByte2SPI(0x00);
    foo = sendByte2SPI(0x00);
    
    SPI_CE = 1;    
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

unsigned char readID(void){
    unsigned char foo2,foo3;
    unsigned char foo = sendByte2SPI(JEDECRDID);
    
    foo = sendByte2SPI(0); // get BF first
    foo2 = sendByte2SPI(0); // expect BF return
    foo3 = sendByte2SPI(0); // expect device ID return

    return foo;
}

/* 
 * File:   SA6main.c
 * Author: ghsu1
 *
 * Created on October 11, 2014, 1:14 PM
 */

#include <xc.h>
#include "configbitsrev2014vC.h"
#include "SDlib.h"
#include <math.h>

void initialize();          //SPI initialization function
unsigned char sendreceive(unsigned char data);//Basic send/receive function
unsigned char SSR(unsigned char data);        //SPI send/receive function

/*
 * 
 */
int main() {
    unsigned char num = 8;
    unsigned char output = 0;
    initialize();
    output = SSR(num);
    printf("output=%c",output);
}

void initialize()
{
    SPI4CONbits.ON = 0;
    SPI4CONbits.CKE = 1;    //Set SPI Mode
    SPI4CONbits.CKP = 0;    //Set SPI Mode
    SPI4CONbits.SMP = 0;    //Set SPI Mode

    SPI4CONbits.MSTEN = 1;  //Set master mode
    IEC1bits.SPI4RXIE = 1;  //Enable interruput flag
    AD1PCFG = 0xFFFF;       //Set AD1PCFG bits to digital input (B8, B10, B11, B14)
    DDPCONbits.JTAGEN = 0;  //Disable JTAG (B10, B11, B12)
    SPI4BRG = 0;            //Set BRG register
    SPI4STATbits.SPIROV = 0;//Clear overflow bit
    SPI4CONbits.ENHBUF = 0; //Clear buffer
    SPI4CONbits.ON = 1;
    TRISBbits.TRISB8 = 0;
    //LATBbits.LATB12 = 1;     //Set HOLD to high
}

unsigned char sendreceive(unsigned char data)
{
    unsigned char output;

    
    while(!SPI4STATbits.SPITBE);
    SPI4BUF = data;         //Send data to buffer
    
    while(!IFS1bits.SPI4RXIF);
    IFS1bits.SPI4RXIF = 0;  //Turn off interrupt flag
    while(!SPI4STATbits.SPIRBF);
    output = SPI4BUF;
    
    return output;          //Return output
}

unsigned char SSR(unsigned char data)
{
    unsigned char a = 0;
    LATBbits.LATB8 = 0;
    a = sendreceive(6);     //Write enable
    LATBbits.LATB8 = 1;
    LATBbits.LATB8 = 0;
    a = sendreceive(2);     //Write
    a = sendreceive(0);     //Address (16 bits total)
    a = sendreceive(0);     //Address
    a = sendreceive(data);  //Send data
    LATBbits.LATB8 = 1;
    LATBbits.LATB8 = 0;
    a = sendreceive(5);
    char random = sendreceive(0);
    while(random!=0)             //Check for WIP (write in progress) bit
    {
        random = sendreceive(0);
    }
    LATBbits.LATB8 = 1;

    LATBbits.LATB8 = 0;
    a = sendreceive(3);     //Read
    a = sendreceive(0);     //Address (16 bits total)
    a = sendreceive(0);     //Address
    a = sendreceive(11);  //Send data, receive original input
    LATBbits.LATB8 = 1;

    return a;
}

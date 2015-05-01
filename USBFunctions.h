#ifndef USB_H_
#define USB_H_

/**********************************************************/
// declare functions
void serial_init6(unsigned long rate);
char getu6();
void putu6(char output);

/**********************************************************/
// define functions

void serial_init6(unsigned long rate){
    USBMODE.ON = 1;      //Enable UART6
    USBMODE.BRGH = 1;    //Enable High Baud Rate
    USBSTA.URXEN = 1;    //Enable UART6 Receiver
    USBSTA.UTXEN = 1;    //Enable UART Transmitter

    long PBclock = get_pb_clock(); //Get peripheral bus clock frequency
    USBBRG = floor(PBclock/(4*rate)-1);  //Calculate and set USBBRG
}

char getu6(){
    char input;
    int b = 0;
    while(b == 0)                    //Wait for input
    {
        if(USBSTA.URXDA == 1)
        {
            input = USBRX;         //Set input
            b = 1;                   //Exit loop
        }
    }
    return input;                    //Return input
}

void putu6(char output){
    int b = 0;
    while(b == 0)                     //Wait for output
    {
        if(USBSTA.URXDA == 1)
        {
            U6TXREG = output;         //Set output
            b = 1;                    //Exit loop
        }
    }
}

#endif

#ifndef SPIFUNCTIONS_H_
#define SPIFUNCTIONS_H_


/**********************************************************/
// declare functions
void initSPI2Master(void); // configure PIC to communicate with SPI
unsigned char eraseSPIFlash(void); // erase entire chip
void write2AllEnable(void); // set permissions to allow write to all memory addresses

unsigned char sendByte2SPI(unsigned char data); // send any byte to the SPI (used in all remaining functions)
unsigned char readID(void); // get device ID - used to debug PIC/SPI communication

unsigned char write2SPI(unsigned char address[], unsigned char data); // write data to memory address
int checkWIP(void); // check for write in progress (used in write2SPI)

unsigned char readSPI(unsigned char address[]); // read memory address, return data

void printStatReg(void); // print status register to LCD - used for debugging

void pageProgram(unsigned char data[]);


/**********************************************************/
// define functions

void initSPI2Master(void){
    // reset
    REG_Interrupt.SPIEIE = 0; // (bit 8) disable error interrupt
    REG_Interrupt.SPIRXIE = 0; // (bit 9) disable receive interrupt
    REG_Interrupt.SPITXIE = 0; // (bit 10) disable transmit interrupt
    SPIREG_Controlbits.ON = 0; // turn     //unsigned char foo = sendByte2SPI(RDID);SPI off
    SPIREG_Control = 0; // reset everything
    SPIREG_Buffer = 0; // clear buffer

    // interrupt settings - clear all of them
    REG_Flag.SPITXIF = 0;
    REG_Flag.SPIEIF = 0;
    REG_Flag.SPIRXIF = 0; // (bit 9) clear receive flag
    
    // interrupt priority settings
    REG_IPC.SPI_Priority = 3; // set priority to 3

    // setup
    SPIREG_Baud_Rate = 255; // set BRG
        // pb clock is now 10MHz -> 5MHz baud rate
    SPIREG_Status.SPIROV = 0; // clear overflow
    SPIREG_Controlbits.MSTEN = 1; // set to master

    // settings - data changes on clock's falling edge, takes data from rising edge
    SPIREG_Controlbits.CKE = 1; // data changes on clock edge from active to idle
    SPIREG_Controlbits.CKP = 0; // clock is active high
    SPIREG_Controlbits.SMP = 0; // take data in middle of cycle
    SPI_HOLD = 1;
    SPI_WP = 0; // set write protect to allow WRSR

    // set pins
    REG_Analog_Digital = 0xFFFF; // sets all to digital
        // needs to account for ADC (B:8,10,11,14 need to be digital)
    REG_JTAG = 0; // disable JTAG on B10, B11, B12

    // enable interrupts - disable tx interrupt as it is useless in this code
    REG_Interrupt.SPIRXIE = 1;
    REG_Interrupt.SPITXIE = 0;
    REG_Interrupt.SPIEIE = 1;
    
    SPI_TRIS = 0; // set I/O
    TRISFbits.TRISF4 = 1;
    TRISFbits.TRISF5 = 0;
    SPI_CE = 1; // don't talk to the SPI right now
    
    // enable SPI operation
    SPIREG_Controlbits.ON = 1;
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
    unsigned char newSRsettings = 0x00;
    SPI_CE = 0;
    // enable write status register
    int foo = sendByte2SPI(EWSR);
    SPI_CE = 1;
    SPI_CE = 0;
    // write 0x00 to status register - chmod +w *
    foo = sendByte2SPI(WRSR);
    foo = sendByte2SPI(newSRsettings);
    SPI_CE = 1;
    SPI_CE = 0;
    // disable write
    foo = sendByte2SPI(WRDI);
    SPI_CE = 1;
}


unsigned char sendByte2SPI(unsigned char data){
// sends a single command to SPI - back end
// waits for transmit buffer to be empty, sends data, waits for flag, waits for
// return value (buffer register)
    REG_Interrupt.SPIRXIE = 0;
    unsigned char regstat;

    // get rid of this and check in ADC done ISR
    while(!SPIREG_Status.SPITBE); // active wait if transmit buffer is not empty
    SPIREG_Buffer = data; // write data to buffer

    // this should occur after all interrupts have been tripped
    while(!SPIREG_Status.SPIRBF); // wait for buffer to be full
    regstat = SPIREG_Buffer; // return buffer register

    return regstat;
}

unsigned char readID(void){
    unsigned char foo2,foo3;
    unsigned char foo = sendByte2SPI(JEDECRDID);
    SPI_CE = 0;
    foo = sendByte2SPI(0); // expect BF
    foo2 = sendByte2SPI(0); // expect 25 return
    foo3 = sendByte2SPI(0); // expect 41 return
    SPI_CE = 1;
    return foo;
}

unsigned char write2SPI(unsigned char address[], unsigned char data){
    // try AAI later if possible, to optimize speed
    unsigned char foo;

    // write enable
    SPI_CE = 0;    
    foo = sendByte2SPI(WREN);
    SPI_CE = 1;
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
    // wait for write - check least significant bit of status register
    int statReg = checkWIP();
    while(statReg){ // while busy
        statReg = checkWIP();
    }

    foo = SPIREG_Buffer;
    return foo;
}

int checkWIP(void){
    SPI_CE = 0;
    char foo = sendByte2SPI(RDSR);
    SPI_CE = 1;
    int statReg = foo & 1;
    return statReg;
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
    
    SPI_CE = 1;    
    return foo;
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
    printf("\n");
}

void pageProgram(unsigned char data[]){
    SPI_CE = 0;
    int foo = sendByte2SPI(WREN); // write enable
    // address
    // hard coded as zeros for now - change later?
    unsigned char address[2];
    address[2] = 0x00;
    address[1] = 0x00;
    address[0] = 0x00;
    foo = sendByte2SPI(address[0]);
    foo = sendByte2SPI(address[1]);
    foo = sendByte2SPI(address[2]);

    int i;
    for (i=0;i<sizeof(data);i++){
        foo = sendByte2SPI(data[i]);
    }
    
    //// for time lag
    //int tester;
    //for (i=0;i<sizeof(data),i++){
    //  foo = sendByte2SPI(data[i]);
    //  for (tester=0;tester<10000;tester++){}
    //}
    
}


/**********************************************************/
// declare ISR
void __ISR(_SPI_Interrupt_Vector,SPI_PL) SPI_RX_ISR(void)
{
    // test which interrupt is immediately tripped
//    if(REG_Flag.SPIRXIF){
//        LATE = 0b10101010;
//        printf("rx\n");
//    }
//    else if(REG_Flag.SPITXIF){
//        printf("tx\n");
//    }
//    else if(REG_Flag.SPIEIF){
//        printf("error\n");
//    }
    
//    printf("here\n");

    while(!SPIREG_Status.SPIRBF);
    REG_Flag.SPIRXIF = 0;
//    REG_Flag.SPITXIF = 0;
    REG_Flag.SPIEIF = 0;
    REG_Interrupt.SPIRXIE = 0;
//    REG_Interrupt.SPITXIE = 0;
    REG_Interrupt.SPIEIE = 0;
    //LATE = 0;
}

#endif

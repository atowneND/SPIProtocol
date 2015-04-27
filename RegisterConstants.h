#ifndef SPICONSTANTS_H_
#define SPICONSTANTS_H_

// define SPI register constants
#define REG_Interrupt IEC1bits // interrupt enable register
#define SPIEIE SPI4EIE // REG_Interrupt.SPIEIE
#define SPIRXIE SPI4RXIE // REG_Interrupt.SPIRXIE
#define SPITXIE SPI4TXIE // REG_Interrupt.SPITXIE

#define REG_Flag IFS1bits // interrupt flag register
#define SPIEIF SPI4EIF // REG_Flag.SPIEIF
#define SPITXIF SPI4TXIF // REG_Flag.SPITXIF
#define SPIRXIF SPI4RXIF // REG_Flag.SPIRXIF

#define REG_IPC IPC8bits // interrupt priority register
#define SPI_Priority SPI4IP // REG_IPC.SPI_Priority

#define SPIREG_Control SPI4CONbits // spi control register
#define SPIREG_Baud_Rate SPI4BRG // spi baud rate generator
#define SPIREG_Status SPI4STATbits // spi status register
#define SPIREG_Buffer SPI4BUF // spi buffer register

#define SPI_CE LATBbits.LATB8 // chip enable
#define SPI_WP LATBbits.LATB10 // write protect
#define SPI_HOLD LATBbits.LATB12 // hold
#define SPI_TRIS TRISB // set I/O

#define REG_Analog_Digital AD1PCFG // set pins to be analog or digital
#define REG_JTAG DDPCONbits.JTAGEN // disable JTAG

// define SPI commands
#define READ 0x03
#define HIGHSPEED_READ 0x0B
#define ERASE_4KB 0x20
#define ERASE_32KB 0x52
#define ERASE_64KB 0xD8
#define ERASE_ALL 0x60 // check 0x60 vx 0xC&
#define BYTE_PROGRAM 0x02
#define AAI_PROGRAM 0xAD
#define RDSR 0x05 // read status register
#define EWSR 0x50 // enable write status register
#define WRSR 0x01 // write status register
#define WREN 0x06 // write enable
#define WRDI 0x04 // write disable
#define RDID 0x90 // read device ID
#define JEDECRDID 0x9F // JEDEC read device ID

// define A2D registers
#define ADCREG_PCFG AD1PCFG // analog input
#define ADCREG_Control1 AD1CON1bits // A2D control register
#define ADCREG_Control2 AD1CON2bits // A2D control register
#define ADCREG_Control3 AD1CON3bits // A2D control register
#define ADCREG_CHS AD1CHSbits // A2D channel
#define ADCREG_Buffer ADC1BUF0 // ADC output

// define Timer registers
#define TREG_Control T2CONbits // timer control register
#define TREG TMR2 // timer register
#define TREG_PR PR2 // timer period register
#define TREG_Interrupt IEC0bits // interrupt enable register
#define TIE T2IE // interrupt enable for timer 2
#define TREG_Flag IFS0bits // interrupt flag register
#define TIF T2IF // TREG_Flag.TIF = IFS0bits.T2IF
#define TREG_Control_SET T2CONSET // start timer

// define Interrupt vector
#define _SAMPLE_TIMER_VECTOR 8

#endif

#ifndef SPICONSTANTS_H_
#define SPICONSTANTS_H_

// define register constants
#define REG_Interrupt IEC1bits // interrupt enable register
#define REG_Flag IFS1bits // interrupt flag register

#define SPIREG_Control SPI4CONbits // spi control register
#define SPIREG_Baud_Rate SPI4BRG // spi baud rate generator
#define SPIREG_Status SPI4STATbits // spi status register
#define SPIREG_Buffer SPI4BUF // spi buffer register

#define REG_Analog_Digital AD1PCFG // set pins to be analog or digital
#define REG_JTAG DDPCONbits.JTAGEN // disable JTAG

// define SPI commands
#define READ 0x03
#define HIGHSPEED_READ 0x0B
#define ERASE_4KB 0x20
#define ERASE_32KB 0x52
#define ERASE_64KB 0XD8
#define ERASE_ALL 0X60 // check 0x60 vx 0xC&
#define BYTE_PROGRAM 0X02
#define AAI_PROGRAM 0XAD
#define RDSR 0x05 // read status register
#define EWSR 0x50 // enable write status register
#define WRSR 0x01 // write status register
#define WREN 0x06 // write enable
#define WRDI 0x04 // write disable

#endif

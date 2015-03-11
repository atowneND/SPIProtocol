#ifndef _SDLIB_H_
#define _SDLIB_H_
#include <xc.h>

/************************************************************
header file for the support routines for the kit board
* At present, included are:
* Delay routines
* serial (spi) LCD (SPI3 with D4 as chip select
* serial port routines (UART6)
* switcher to allow stdout to go to LCD or serial port
*/
/* LCD Select stuff*/

//#define SPILCDCON SPI3CONbits
//#define SPILCDSTAT SPI3STATbits
#define SPILCDBRG SPI3BRG
#define SPILCD_CS LATDbits.LATD4  // chip select  for LCD
#define SPILCD_CS_TRIS TRISDbits.TRISD4
#define SPILCD_INT IFS0bits.SPI3RXIF  // int flag
#define SPILCD_BUF SPI3BUF // data buffer
// LCD Function prototypes

/* specific to spi display */

void LCD_init(void);
void LCD_char(char val);
void LCD_display_on(void);
void LCD_display_off(void);
void LCD_clear(void);
void LCD_backlight(char val);
void LCD_contrast(char val);
void LCD_setpos(char row, char col);

/* simple input routines */
unsigned int getdec(void);
unsigned int gethex(void);
signed int getint(void);

/* serial I/O via usart 6 prototypes*/

unsigned char getu(void);  // get char
void putu(unsigned char val); // put char

void serial_init(unsigned long rate);


void set_output_device(unsigned char device);

/* delay routines*/
void set_sys_clock(unsigned long val);
unsigned long get_sys_clock(void);
void set_pb_clock(unsigned long val);
unsigned long get_pb_clock(void);
void delay_ms(unsigned long val);
void delay_us(unsigned long val);

#endif //SDLIB_H

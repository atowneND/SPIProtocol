#ifndef _DELAY_H_
#define _DELAY_H_
#include <xc.h>

/************************************************************
Delay functions for kit32 board */
/* delay routines*/
void set_sys_clock(unsigned long val);
unsigned long get_sys_clock(void);
void set_pb_clock(unsigned long val);
unsigned long get_pb_clock(void);
void delay_ms(unsigned long val);
void delay_us(unsigned long val);

#endif //DELAY_H

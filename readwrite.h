/* 
 * File:   readwrite.h
 * Author: ghsu1
 *
 * Created on September 22, 2014, 8:01 PM
 */

#ifndef READWRITE_H
#define	READWRITE_H

#ifdef	__cplusplus
extern "C" {
#endif

void serial_init6(unsigned long rate);
char getu6();
void putu6(char output);


#ifdef	__cplusplus
}
#endif

#endif	/* READWRITE_H */


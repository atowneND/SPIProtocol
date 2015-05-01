/*
 * File:   board2usbMain.c
 * Author: #pragma
 *
 * Created on February 4, 2015, 12:55 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/attribs.h>
#include <cp0defs.h>

#include "SDlib.h"
#include "configbitsrev2014vC.h"
#include "RegisterConstants.h"
#include "A2DFunctions.h"
#include "Timer.h"


/*
 *
 */
// transmit test - proof of concept
void initAll(void);

int main(void) {
    int bar = 255;

    initAll();
    LATE = 254;
    
    //////////////////////////////////////////////////
    enable_ADC();
    timer_enable(10);
    printf("here\n");
    putu('n');

    
    return (EXIT_SUCCESS);
}

void initAll(void){
    // global interrupts
    asm volatile("ei");
    INTCONbits.MVEC = 1;

    // LEDs
    TRISE = 0;
    LATE = 0;

    // initialize USB
    serial_init(9600);
    set_output_device(1); // select USB as output device
    
    // initialize timer
    timer_init();
    
    // initialize ADC
    init_ADC();
}


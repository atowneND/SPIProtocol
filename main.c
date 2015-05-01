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
#include "Initialize.h"


/*
 *
 */
// transmit test - proof of concept

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

void __ISR(UP_Vector,UPIPL) UPISR(){
    LATE = 0b11001100;
    TREG_Flag.UP_IF = 0;
}

void __ISR(DOWN_Vector,DOWNIPL) DOWNISR(){
    LATE = 0b110111011;
    TREG_Flag.DOWN_IF = 0;
}

void __ISR(REC_Vector,RECIPL) RECISR(){
    LATE = 0xF0;
    TREG_Flag.REC_IF = 0;
}

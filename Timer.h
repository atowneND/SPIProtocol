#ifndef TIMER_H_
#define TIMER_H_

/**********************************************************/
// declare functions
void timer_init(int length);

/**********************************************************/
// define functions
void timer_init(int length){
    TREG_Interrupt.TIE = 1;
    // from Gavin's code
    TREG_Control.ON = 1; // turn on timer
    // set prescale to 256
    TREG_Control.TCKPS0 = 1;
    TREG_Control.TCKPS1 = 1;
    TREG_Control.TCKPS2 = 1;

    TREG = 0x0; // clear timer register
    TREG_PR = 0x2710; // set period register to 10000
    TREG_Control_SET = 0x8000; // start timer
    TREG_Flag.TIF = 0; // interrupt flag off
}

#endif

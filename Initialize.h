#ifndef INIT_H_
#define INIT_H_

/**********************************************************/
// declare functions
void initAll(void);
void initButtons(void);

/**********************************************************/
// define functions
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

void initButtons(void){
    // disable interrupts
    TREG_Interrupt.DOWN_IE = 0;
    TREG_Interrupt.UP_IE = 0;
    TREG_Interrupt.REC_IE = 0;

    // set interrupt flags to 0
    TREG_Flag.DOWN_IF = 0;
    TREG_Flag.UP_IF = 0;
    TREG_Flag.REC_IF = 0;

    // tripped on falling edge
    UP_Edge = 0;
    DOWN_Edge = 0;
    REC_Edge = 0;

    // set interrupt priority
    UP_Priority = 3;
    DOWN_Priority = 2;
    REC_Priority = 4;

    // enable interrupts
    TREG_Interrupt.DOWN_IE = 1;
    TREG_Interrupt.UP_IE = 1;
    TREG_Interrupt.REC_IE = 1;
}

#endif

#ifndef TIMER_H_
#define TIMER_H_

/**********************************************************/
// declare functions
void timer_init(void);
void timer_enable(int length);
int Nctr = 0xFFFF;
    
/**********************************************************/
// define functions
void timer_init(void){
    TREG_Interrupt.TIE = 0;
    TREG_Control = 0;

    // from Gavin's code
    TREG_Controlbits.ON = 1; // turn on timer
    // set prescale to 256
    TREG_Controlbits.TCKPS0 = 1;
    TREG_Controlbits.TCKPS1 = 1;
    TREG_Controlbits.TCKPS2 = 1;
    INTERRUPT_PRIORITY = 6;
    //INTERRUPT_SUBPRIORITY = 6;

    //TREG_PR = 0xF710; // set period register to 10000
   
}

void timer_enable(int length){
    TREG_Flag.TIF = 0; // interrupt flag off
    TREG_Interrupt.TIE = 1;
    TREG = 0x0; // clear timer register
    TREG_PR = 0xFFFF;
    TREG_Control_SET = 0x8000; // start timer
 
}

void __ISR(_SAMPLE_TIMER_VECTOR,timerIPL) timerISR(void)
{
    Nctr = Nctr - 1;
    LATE = Nctr;
        conv();
    IFS0bits.T2IF = 0;
}

#endif

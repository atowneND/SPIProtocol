#ifndef TIMER_H_
#define TIMER_H_

/**********************************************************/
// declare functions
void timer_init(int length);
int Nctr = 0xFFFF;
    
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
    INTERRUPT_PRIORITY = 6;
    //INTERRUPT_SUBPRIORITY = 6;
    TREG = 0x0; // clear timer register
    TREG_PR = 0xFFFF;
    //TREG_PR = 0xF710; // set period register to 10000
    TREG_Control_SET = 0x8000; // start timer
    TRISE = 0;
    //TREG_Flag.TIF = 0; // interrupt flag off
    LATE = Nctr;
    LATEbits.LATE0 = 0;
//    asm volatile("ei");
    
    // timer test
    while(1){
        if(IFS0bits.T2IF==1){
            Nctr = Nctr - 1;
            LATE = Nctr;
            IFS0bits.T2IF = 0;
        }
    }
}

void __ISR(_SAMPLE_TIMER_VECTOR,timerIPL) timerISR(void)
{
    Nctr = Nctr - 1;
    LATE = Nctr;
    IFS0bits.T2IF = 0;
}

#endif

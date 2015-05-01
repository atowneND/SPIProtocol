#ifndef TIMER_H_
#define TIMER_H_

/**********************************************************/
// declare functions
void timer_init(void);
void timer_enable(int length);
int Nctr = 0xFFFF;
long unsigned int t2Ctr = 0;
long unsigned int PB_CLK;
int MAXITER = 0;
    
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
    PB_CLK = get_pb_clock();
    //INTERRUPT_SUBPRIORITY = 6;
    //TREG_PR = 0xF710; // set period register to 10000
}

void timer_enable(int length){ // length = time
    printf("foo\n");
    if (length>30){
        // ERROR
    }
    else{
        unsigned int ctrperISR = 0x8000;
            TREG_Flag.TIF = 0; // interrupt flag off
        TREG_Interrupt.TIE = 1;
        TREG = 0x0; // clear timer register
        TREG_PR = 0xFFFF;
        TREG_Control_SET = ctrperISR; // start timer
        MAXITER = PB_CLK*length/(ctrperISR*256);
        printf("Maxiter = %i\n",MAXITER);
    }
}

void __ISR(_SAMPLE_TIMER_VECTOR,timerIPL) timerISR(void)
{
    if (t2Ctr<MAXITER){
        //conv();
        t2Ctr = t2Ctr + 1;
    }
    else{
        TREG_Interrupt.TIE = 0;
        LATE = 0b10101010;
    }
    Nctr = Nctr - 1;
    LATE = Nctr;
    TREG_Flag.TIF = 0;
}

#endif

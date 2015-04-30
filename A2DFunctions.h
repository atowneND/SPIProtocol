#ifndef A2DFUNCTIONS_H_
#define A2DFUNCTIONS_H_

/**********************************************************/
// declare functions
void init_ADC(); // initialize A2D settings
void conv();
void conv1();
void conv2();

/**********************************************************/
// define functions
// CHOSA: use pins 17 & 18 (AN6 & AN7)
void init_ADC()
{
    ADCREG_PCFG = 0;            //Set pins to analog input - DOUBLE CHECK PGD & PGC
    //ADCREG_PCFG = 0x00C0;     //Set AN6 & AN7 to analog - all others are digital, including PGD & PGC
    ADCREG_Control1.ADON = 0;   //Turn off ADC
    ADCREG_Control1.FORM = 0;   //Unsigned integer format
    ADCREG_Control1.SSRC = 0;   //Convert when SAMP is cleared
    ADCREG_Control2.SMPI = 0;   // set interrupt to occur after every conversion
    ADCREG_Control2.VCFG = 0;   //Use AVDD and AVSS as Voltage Reference
    ADCREG_Control2.CSCNA = 0;  //Do not scan
    ADCREG_Control2.BUFM = 0;   //One 16-word buffer
    ADCREG_Control2.ALTS = 0;   //Always use MUX A
    ADCREG_Control3.ADRC = 0;   //Use PBClock as Clock Source
    ADCREG_Control3.ADCS = 0;   //for speed
    ADCREG_Control1.ADON = 1;   //Turn on ADC
    ADC_PRIORITY = 5;
    ADC_Interrupt = 0;          // clear ADC flag
    ADC_IE = 1;                 // enable interrupt
}

void conv()
{
    // A2D_F
    unsigned int output;
	LATD = 1;
    ADCREG_CHS.CH0SA = 6;       //Use channel AN0 as input
    ADCREG_Control1.SAMP = 1;       //Enable sampling
    delay_us(1); //Delay
    LATD = 0;
    ADCREG_Control1.SAMP = 0;       //Terminate sampling
}

void conv1()
{
    // A2D_F 
    unsigned int output; 
    ADCREG_CHS.CH0SA = 6;       //Use channel AN2 as input for Mux A 
    ADCREG_Control1.SAMP = 1;       //Enable sampling
    delay_us(1);                //Delay
    ADCREG_Control1.SAMP = 0;       //Terminate sampling
      LATD = 1;
}

void conv2()
{
    // A2D_UF
    unsigned int output;
    ADCREG_CHS.CH0SA = 7;       //Use channel AN2 as input for Mux A
    ADCREG_Control1.SAMP = 1;       //Enable sampling
    delay_us(1);                //Delay
    ADCREG_Control1.SAMP = 0;       //Terminate sampling
      LATD = 1;
}

void __ISR(_ADC_VECTOR,adcIPL) ADC_ISR(void)
{
    unsigned int output;
    if (!ADCREG_Control1.DONE){
        printf("ERROR\n");
        while(!ADCREG_Control1.DONE);
    }
    output = ADCREG_Buffer;
    printf("%i\n",output);
    ADC_Interrupt = 0;
    LATE = 0;
}
#endif

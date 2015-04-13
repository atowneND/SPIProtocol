#ifndef A2DFUNCTIONS_H_
#define A2DFUNCTIONS_H_

/**********************************************************/
// declare functions
void init_ADC(); // initialize A2D settings
unsigned int conv();
unsigned int conv1();
unsigned int conv2();

/**********************************************************/
// define functions
void init_ADC()
{
    AD1PCFG = 0;            //Set pins to analog input
    AD1CON1bits.ADON = 0;   //Turn off ADC
    AD1CON1bits.FORM = 0;   //Unsigned integer format
    AD1CON1bits.SSRC = 0;   //Convert when SAMP is cleared
    AD1CON2bits.VCFG = 0;   //Use AVDD and AVSS as Voltage Reference
    AD1CON2bits.CSCNA = 0;  //Do not scan
    AD1CON2bits.BUFM = 0;   //One 16-word buffer
    AD1CON2bits.ALTS = 0;   //Always use MUX A
    AD1CON3bits.ADRC = 0;   //Use PBClock as Clock Source
    AD1CON3bits.ADCS = 0;   //for speed
    AD1CON1bits.ADON = 1;   //Turn on ADC
}

unsigned int conv()
{
    unsigned int output;
	LATD = 1;
    AD1CHSbits.CH0SA = 2;       //Use channel AN0 as input
    AD1CON1bits.SAMP = 1;       //Enable sampling
    delay_us(1); //Delay
    LATD = 0;
    AD1CON1bits.SAMP = 0;       //Terminate sampling
    while(!AD1CON1bits.DONE);   //Wait for conversion to complete
    output = ADC1BUF0;          //Get ADC result

    return output;
}

unsigned int conv1()
{
    unsigned int output;
    AD1CHSbits.CH0SA = 2;       //Use channel AN2 as input for Mux A
    AD1CON1bits.SAMP = 1;       //Enable sampling
    delay_us(1);                //Delay
    AD1CON1bits.SAMP = 0;       //Terminate sampling
      LATD = 1;
    while(!AD1CON1bits.DONE);   //Wait for conversion to complete
        LATD = 0;
    output = ADC1BUF0;          //Get ADC result
    return output;
}

unsigned int conv2()
{
    unsigned int output;
    AD1CHSbits.CH0SA = 3;       //Use channel AN2 as input for Mux A
    AD1CON1bits.SAMP = 1;       //Enable sampling
    delay_us(1);                //Delay
    AD1CON1bits.SAMP = 0;       //Terminate sampling
      LATD = 1;
    while(!AD1CON1bits.DONE);   //Wait for conversion to complete
        LATD = 0;
    output = ADC1BUF0;          //Get ADC result
    return output;
}
#endif

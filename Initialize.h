
void initAll(void);

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


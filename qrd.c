// This program reads from multiple A/D channels in
// scan mode.

#include <p24F16KA301.h>

// Select oscillator
#pragma config FNOSC = FRC
#pragma config ICS = PGx3

// This function sets up all the analog pins for analog input
void config_pins_for_ad(void)
{
    // Put all relevant registers into a known state
    TRISA = 0;
    TRISB = 0;
    ANSA = 0;
    ANSB = 0;
    
    // Configure the tri state adapter for input for all the analog pins
    TRISAbits.TRISA0 = 0; // Configure pin 2 for input
//    TRISAbits.TRISA1 = 1; // Configure pin 3 for input
//    TRISBbits.TRISB0 = 1; // Configure pin 4 for input
//    TRISBbits.TRISB1 = 1; // Configure pin 5 for input    
//    TRISBbits.TRISB2 = 1; // Configure pin 6 for input
//    TRISAbits.TRISA2 = 1; // Configure pin 7 for input
//    TRISAbits.TRISA3 = 1; // Configure pin 8 for input
//    TRISBbits.TRISB4 = 1; // Configure pin 9 for input
//    TRISBbits.TRISB12 = 1; // Configure pin 15 for input    
//    TRISBbits.TRISB13 = 1; // Configure pin 16 for input
    TRISBbits.TRISB14 = 1; // Configure pin 17 for input
    TRISBbits.TRISB15 = 1; // Configure pin 18 for input
    
    // Configure all the analog pins for analog (as opposed to digital)
    ANSAbits.ANSA0 = 0; // Configure pin 2 for analog
//    ANSAbits.ANSA1 = 1; // Configure pin 3 for analog
//    ANSBbits.ANSB0 = 1; // Configure pin 4 for analog
//    ANSBbits.ANSB1 = 1; // Configure pin 5 for analog    
//    ANSBbits.ANSB2 = 1; // Configure pin 6 for analog
//    ANSAbits.ANSA2 = 1; // Configure pin 7 for analog
//    ANSAbits.ANSA3 = 1; // Configure pin 8 for analog
//    ANSBbits.ANSB4 = 1; // Configure pin 9 for analog
//    ANSBbits.ANSB12 = 1; // Configure pin 15 for analog    
//    ANSBbits.ANSB13 = 1; // Configure pin 16 for analog
    ANSBbits.ANSB14 = 1; // Configure pin 17 for analog
    ANSBbits.ANSB15 = 1; // Configure pin 18 for analog
}

// This function configures the A/D to read from
// multiple channels in scan mode.
void config_ad(void)
{
    
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D during config
    
    // AD1CON1 register
    _ADSIDL = 0;  // AD1CON1<13> -- A/D continues in idle mode
    _MODE12 = 1;  // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;    // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;    // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;    // AD1CON1<2> -- Auto sampling

    // AD1CON2 register
    _PVCFG = 0;   // AD1CON2<15:14> -- Use VDD as positive
                  // ref voltage
    _NVCFG = 0;   // AD1CON2<13> -- Use VSS as negative
                  // ref voltage
    _BUFREGEN = 1;// AD1CON2<11> -- Result appears in buffer
                  // location corresponding to channel
    _CSCNA = 1;   // AD1CON2<10> -- Scans inputs specified
                  // in AD1CSSx registers
    _SMPI = 9;	  // AD1CON2<6:2> -- Every 9th conversion sent (number of channels sampled -1)
                  // to buffer (if sampling 10 channels)
    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only
    
    // AD1CON3 register
    _ADRC = 0;    // AD1CON3<15> -- Use system clock
    _SAMC = 0;    // AD1CON3<12:8> -- Auto sample every A/D
                  // period TAD
    _ADCS = 0b00000011;    // AD1CON3<7:0> -- TAD needs to be at least 600 ns. Thus, _ADCS = 0b00000010 will give us the fastest AD clock given a 4 MHz system clock. 0b00000011 is slower than we could go, but still fast enough for our needs.

    // AD1CSS registers
    AD1CSSL = 0b0011111000011111; // choose A2D channels you'd like to scan here.
    
    _ADON = 1;    // AD1CON1<15> -- Turn on A/D
}

int main()
{
    // Configure A/D
    config_pins_for_ad();
    config_ad();
    
    static double theVoltage[12];

    // Loop
    while(1)
    {
        // Read from the A/D buffer and do something useful
        // with it. The A/D buffer is being updated continuously 
        // behind the scenes.
        // WRITE CODE TO DO SOMETHING INTERESTING WITH THE
        // VALUES IN THE A/D BUFFER HERE
//        theVoltage[0] = (double)ADC1BUF0/4095.0 * 3.3;  // Pin 2
//        theVoltage[1] = (double)ADC1BUF1/4095.0 * 3.3;  // Pin 3
//        theVoltage[2] = (double)ADC1BUF2/4095.0 * 3.3;  // Pin 4
//        theVoltage[3] = (double)ADC1BUF3/4095.0 * 3.3;  // Pin 5
//        theVoltage[4] = (double)ADC1BUF4/4095.0 * 3.3;  // Pin 6
        theVoltage[5] = (double)ADC1BUF9/4095.0 * 3.3;  // Pin 18
        theVoltage[6] = (double)ADC1BUF10/4095.0 * 3.3; // Pin 17
        if ((theVoltage[5] < 1 && theVoltage[6] > 1) || ((theVoltage[5] > 1 && theVoltage[6] < 1))) {
            _LATA0 = 1;
        } else {
            _LATA0 = 0;
        }
//        theVoltage[7] = (double)ADC1BUF11/4095.0 * 3.3; // Pin 16
//        theVoltage[8] = (double)ADC1BUF12/4095.0 * 3.3; // Pin 15
//        theVoltage[9] = (double)ADC1BUF13/4095.0 * 3.3; // Pin 7 
//        theVoltage[10] = (double)ADC1BUF14/4096.0 * 3.3;  // Pin 8 is clock output (need to disable if you want to use)
//        theVoltage[11] = (double)ADC1BUF15/4096.0 * 3.3;  // Pin 9 is used by debugger (need to disable if you want to use)
    }

    return 0;
}


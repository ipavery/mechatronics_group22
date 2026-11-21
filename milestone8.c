#include "xc.h"

#pragma config FNOSC = LPFRC
#pragma config OSCIOFNC = OFF

// pwm periods 
#define SPEED_MED 1500
#define SPEED_HIGH 1000

#define WF ADC1BUF9 //wall front
#define WL ADC1BUF10 //wall left
#define WR ADC1BUF11 //wall right

#define STEPS_90DEG 510 // number of steps to turn 90 deg 
#define THRESHOLD 600
#define FRONT_THRESHOLD 600

int OC3_steps = 0;
int OC2_steps = 0;
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void){
  _OC2IF = 0;
  OC2_steps++;
}


void __attribute__((interrupt, no_auto_psv)) _OC3Interrupt(void){
  _OC3IF = 0;
  OC3_steps++;
}

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
    _CSCNA = 1;   // AD1CON2<10> -- Does not scan inputs
                  // specified in AD1CSSx registers (instead
                  // uses channels specified by CH0SA bits in
                  // AD1CHS register) -- Selecting '0' here
                  // probably makes writing to the AD1CSSL
                  // register unnecessary.
    _SMPI = 2;    // AD1CON2<6:2> -- Each conversion sent to 
                  // buffer
    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only

    // AD1CON3 register
    _ADRC = 0;    // AD1CON3<15> -- Use system clock
    _SAMC = 1;    // AD1CON3<12:8> -- Auto sample every A/D
                  // period TAD
    _ADCS = 0b00000011;    // AD1CON3<7:0> -- TAD needs to be at least 600 ns. Thus, _ADCS = 0b00000010 will give us the fastest AD clock given a 4 MHz system clock. 0b00000011 is slower than we could go, but still fast enough for our needs.

    // AD1CHS register
//    _CH0NA = 0;   // AD1CHS<7:5> -- Use Vss as negative input
//    _CH0SA = 0b00000; // AD1CHS<4:0> -- Use AN0 as positive input (pin 2)

    // AD1CSSL register
    AD1CSSL = 0;  // AD1CSSL<15:0> -- Skip all channels on 
                  // input scan -- see the CSCNA bits in
                  // AD1CON2

   
    _CSS11 = 1;
    _CSS10 = 1;
    _CSS9 = 1;
    _ADON = 1;    // AD1CON1<15> -- Turn on A/D
}


int main(void) {

  _RCDIV = 0;

  OC2CON2bits.SYNCSEL = 0x1F;
  OC2CON1bits.OCTSEL = 0b111;
  OC2CON1bits.OCM = 0b110;
  OC2CON2bits.OCTRIG = 0;
  
  OC1CON2bits.SYNCSEL = 0x1F;
  OC1CON1bits.OCTSEL = 0b111;
  OC1CON1bits.OCM = 0b000;
  OC1CON2bits.OCTRIG = 0;

  _OC2IF = 0;
  _OC2IP = 4;
  _OC2IE = 1;
  
  _OC3IF = 0;
  _OC3IP = 5;
  _OC3IE = 1;

  OC3CON2bits.SYNCSEL = 0x1F;
  OC3CON1bits.OCTSEL = 0b111;
  OC3CON1bits.OCM = 0b110;
  OC3CON2bits.OCTRIG = 0;

  ANSA = 0;
  ANSB = 0;
  TRISA = 0;
  TRISB = 0;
  
  LATA = 0;

  // set pin 2, 3 as the direction pins
  _ANSA0 = 0;
  _ANSA1 = 0;

  // set pins 16,17,18 as analog inputs
  _TRISB13 = 1; 
  _ANSB13 = 1;  
  _TRISB14 = 1;
  _ANSB14 = 1;
  _TRISB15 = 1;
  _ANSB15 = 1;
 
  
  // calls a2d function
  config_ad();
  
  OC2RS = SPEED_HIGH; //OC2: left motor
  OC2R = OC2RS / 2;
  OC3RS = SPEED_HIGH; //OC3: right motor
  OC3R = OC3RS / 2;
  
  
//  _TRISB9 = 0;
//  while(1){
//      if(WL >= THRESHOLD && WF >= THRESHOLD){
//          _LATB9 =1;
//      }
//      else{
//          _LATB9 =0;
//      }
//  }

  // THE FOLLOWING CODE WILL RUN THREE LEDs; WE NEED THESE LEDs TO TROUBLE SHOOT THE CANYON NAVIGATION. THEY WILL BE CONNNECTED TO THE SAME PINS THAT RUN THHE THREE qrdS. THE PINS WILL JUST BE RECONFIGURED TO BE OUTPUT, NOT INPUT, DURING CYN NAVIGATION/
  _ANSB2 = 0; // PIN 6
  _TRISB2 = 0;
  _LATB2 = 0;
  _ANSA2 = 0; // PIN 7
  _TRISA2 = 0;
  _LATA2 = 0;
  _ANSA3 = 0; // PIN 8
  _TRISA3 = 0;
  _LATA3 = 0;
  
  while(1){
      // the following nine lines of code light the leds based on the sharp sensor states. 
    if(WF >= FRONT_THRESHOLD){
    _LATB2 = 1;}
    else{_LATB2 = 0;}
    if(WL >= THRESHOLD){
    _LATA2 = 1;}
    else{_LATA2 = 0;}
    if(WR >= THRESHOLD){
    _LATA3 = 1;}
    else{_LATA3 = 0;}
    
    if (WF >= FRONT_THRESHOLD && WL >= THRESHOLD){
      OC2_steps = 0;
      // turn right
      _LATA0 = 1;
        _LATA1 = 0;
        OC2RS = SPEED_HIGH;
        OC3RS = SPEED_HIGH;
        OC2R = OC2RS / 2;
        OC3R = OC3RS / 2;
        OC3CON1bits.OCM = 0b000;
      while (OC2_steps < STEPS_90DEG) {
      }
        OC2CON1bits.OCM = 0b110;
        OC3CON1bits.OCM = 0b110;
    } 
    if (WF >= FRONT_THRESHOLD && WR >= THRESHOLD){
      OC3_steps = 0;
      // turn left
       _LATA0 = 0;
        _LATA1 = 1;
        OC2RS = SPEED_HIGH;
        OC3RS = SPEED_HIGH;
        OC2R = OC2RS / 2;
        OC3R = OC3RS / 2;
        OC2CON1bits.OCM = 0b000;
      while (OC3_steps < STEPS_90DEG) {
      }
        OC2CON1bits.OCM = 0b110;
        OC3CON1bits.OCM = 0b110;
    }
    if (WF >= FRONT_THRESHOLD && WL <= THRESHOLD && WR <= THRESHOLD) {
        OC2CON1bits.OCM = 0b000;
        OC3CON1bits.OCM = 0b000;
    } else {
      OC2RS = SPEED_HIGH;
      OC3RS = SPEED_HIGH;
      OC2R = OC2RS / 2;
      OC3R = OC3RS / 2;
      _LATA0 = 0;
      _LATA1 = 0;
    }
  } // end while loop
  return 0;
} // end main loop

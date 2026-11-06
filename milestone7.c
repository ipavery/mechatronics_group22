#include "xc.h"

#pragma config FNOSC = LPFRC

// pwm periods 
#define constant = 10000;  
#define slow = 5000; // for slow turn
#define hard = 2500; // for hard turn

enum {FWD, L, R, HL, HR} State;

// pin 2, RA0, DO, Left Motor Direction
// pin 3, RA1, DO, Right Motor Direction

// pin 4, OC2, Left Stepper PWM
// pin 5, OC3, Right Stepper PWM

// pin 6, RB2, DI, Center Line QRD
// pin 7, RA2, DI, Left Line QRD
// pin 8, RA3, DI, Right Line QRD

int main(void) {

  _RCDIV = 0b001;
  State state = FWD;

  OC1CON2bits.SYNCSEL = 0x1F;
  OC1CON1bits.OCTSEL = 0b111;
  OC1CON1bits.OCM = 0b110;
  OC1CON2bits.OCTRIG = 0;

  OC2CON2bits.SYNCSEL = 0x1F;
  OC2CON1bits.OCTSEL = 0b111;
  OC2CON1bits.OCM = 0b110;
  OC2CON2bits.OCTRIG = 0;

  ANSA = 0;
  LATA = 0;
  // set pin 2, 3
  _TRISA0 = 1;
  _ANSA0 = 1;
  // set pins 6,7,8 as digital inputs
  _TRISB2 = 1; 
  _ANSB2 = 0;  
  _TRISA2 = 1;
  _ANSA2 = 0;
  _TRISA3 = 1;
  _ANSA3 = 0;

  OC2RS = constant;
  OC2R = OC2RS / 2;
  OC3RS = constant;
  OC3R = OC3RS / 2;

  while(1){
    switch (state){
      case FWD:
        OC2RS = constant;
        OC2R = OC2RS / 2;
        OC3RS = constant;
        OC3R = OC3RS / 2;
        if (LL == ON){
          state = L;
        }
        if (RL == ON){
          state = R;
        }
        break;
      case L:
        OC2RS = constant;
        OC2R = OC2RS / 2;
        OC3RS = slow;
        OC3R = OC3RS / 2;
        if (LL == OFF){
          state = FWD;
        }
        break;
      case R:
        OC2RS = slow;
        OC2R = OC1RS / 2;
        OC3RS = constant;
        OC3R = OC2RS / 2;
        if (RL == OFF){
          state = FWD;
        }
        break;
    } // end switch state

  } // end while loop
  return 0;
} // end main loop
    
    

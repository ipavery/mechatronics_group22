#include "xc.h"

#pragma config FNOSC = LPFRC

// pwm periods 
#define constant = 10000;  
#define slow = 5000; // for slow turn
#define hard = 2500; // for hard turn

#define THRESHOLD 2048

enum {FWD, L, R, HL, HR} State;

int main(void) {

  _RCDIV = 0;
  State state = FWD;

  OC2CON2bits.SYNCSEL = 0x1F;
  OC2CON1bits.OCTSEL = 0b111;
  OC2CON1bits.OCM = 0b110;
  OC2CON2bits.OCTRIG = 0;

  OC3CON2bits.SYNCSEL = 0x1F;
  OC3CON1bits.OCTSEL = 0b111;
  OC3CON1bits.OCM = 0b110;
  OC3CON2bits.OCTRIG = 0;

  ANSA = 0;
  LATA = 0;
  // set pin 2, 3
  _TRISA0 = 1;
  _ANSA0 = 1;
  _TRISA1 = 1;
  _ANSA1 = 1;
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
        if (CL >= THRESHOLD && LL >= THRESHOLD && RL < THRESHOLD){
          state = L;
        }
        if (CL >= THRESHOLD && LL < THRESHOLD && RL >= THRESHOLD){
          state = R;
        }
        if (CL < THRESHOLD && LL >= THRESHOLD){
          state = HL;
        }
        if (CL < THRESHOLD && RL >= THRESHOLD){
          state = HR;
        }
        break;
      case L:
        OC2RS = constant;
        OC2R = OC2RS / 2;
        OC3RS = slow;
        OC3R = OC3RS / 2;
        if (CL>= THRESHOLD && LL < THRESHOLD && RL < THRESHOLD){
          state = FWD;
        }
        break;
      case R:
        OC2RS = slow;
        OC2R = OC1RS / 2;
        OC3RS = constant;
        OC3R = OC2RS / 2;
        if (CL>= THRESHOLD && LL < THRESHOLD && RL < THRESHOLD){
          state = FWD;
        }
        break;
      case HL:
        OC2RS = constant;
        OC2R = OC2RS / 2;
        OC3RS = hard;
        OC3R = OC3RS / 2;
        if (CL>= THRESHOLD && LL < THRESHOLD && RL < THRESHOLD){
          state = FWD;
        }
        if (CL >= THRESHOLD && LL >= THRESHOLD && RL < THRESHOLD){
          state = L;
        }
        break;
      case HR:
        OC2RS = hard;
        OC2R = OC2RS / 2;
        OC3RS = constant;
        OC3R = OC3RS / 2;
        if (CL>= THRESHOLD && LL < THRESHOLD && RL < THRESHOLD){
          state = FWD;
        }
        if (CL >= THRESHOLD && LL < THRESHOLD && RL >= THRESHOLD){
          state = R;
        }
        break;
    } // end switch state

  } // end while loop
  return 0;
} // end main loop
    
    

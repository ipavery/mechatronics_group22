#include "xc.h"

#pragma config FNOSC = LPFRC

// pwm periods 
#define CONSTANT 1000
#define SLOW 1200 // for slow turn
#define HARD 1400 // for hard turn

#define CL _RB2 //center light
#define LL _RA2 //left light
#define RL _RA3 //right light

#define THRESHOLD 2048

enum {FWD, L, R, HL, HR} state;

int main(void) {

  _RCDIV = 0;
  state = FWD;

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
  LATB = 0;
  // set pin 2, 3 as the direction pins
  _TRISA0 = 1;
  _ANSA0 = 0;
  _RA0 = 0;
  _TRISA1 = 1;
  _ANSA1 = 0;
  _RA1 = 1;
  // set pins 6,7,8 as digital inputs
  _TRISB2 = 1; 
  _ANSB2 = 0;  
  _TRISA2 = 1;
  _ANSA2 = 0;
  _TRISA3 = 1;
  _ANSA3 = 0;

  OC2RS = CONSTANT;
  OC2R = OC2RS / 2;
  OC3RS = CONSTANT;
  OC3R = OC3RS / 2;

  while(1){
    switch (state){
      case FWD:
          if (!RL) {
              OC2CON1bits.OCM = 0b000;
          }
//        if (!CL && !LL && RL){
//            OC2RS = CONSTANT;
//            OC2R = OC2RS / 2;
//            OC3RS = SLOW;
//            OC3R = OC3RS / 2;
//          state = L;
//        }
//        if (!CL && LL && !RL){
//            OC2RS = SLOW;
//            OC2R = OC1RS / 2;
//            OC3RS = CONSTANT;
//            OC3R = OC2RS / 2;
//          state = R;
//        }
//        if (!CL && LL){
//          state = HL;
//        }
//        if (!CL && RL){
//          state = HR;
//        }
        break;
      case L:
        if (!RL){
            OC2RS = CONSTANT;
            OC2R = OC2RS / 2;
            OC3RS = CONSTANT;
            OC3R = OC3RS / 2;
          state = FWD;
        }
        break;
      case R:
        if (!LL){
            OC2RS = CONSTANT;
            OC2R = OC2RS / 2;
            OC3RS = CONSTANT;
            OC3R = OC3RS / 2;
          state = FWD;
        }
        break;
//      case HL:
//        OC2RS = CONSTANT;
//        OC2R = OC2RS / 2;
//        OC3RS = HARD;
//        OC3R = OC3RS / 2;
//        if (CL>= THRESHOLD && LL < THRESHOLD && RL < THRESHOLD){
//          state = FWD;
//        }
//        if (CL >= THRESHOLD && LL >= THRESHOLD && RL < THRESHOLD){
//          state = L;
//        }
//        break;
//      case HR:
//        OC2RS = HARD;
//        OC2R = OC2RS / 2;
//        OC3RS = CONSTANT;
//        OC3R = OC3RS / 2;
//        if (CL>= THRESHOLD && LL < THRESHOLD && RL < THRESHOLD){
//          state = FWD;
//        }
//        if (CL >= THRESHOLD && LL < THRESHOLD && RL >= THRESHOLD){
//          state = R;
//        }
//        break;
    } // end switch state

  } // end while loop
  return 0;
} // end main loop
    
    

#include "xc.h"

#pragma config FNOSC = LPFRC

// pwm periods 
#define CONSTANT 1000

#define WF _RB15 //wall front
#define WL _RB14 //wall left
#define WR _RB13 //wall right

#define STEPS 457 // number of steps to turn 90 deg 

int steps = 0;
void _attribute_((interrupt, no_auto_psv))
_OC2Interrupt(void){
  _OC2IF = 0;
  steps++;
}

int main(void) {

  _RCDIV = 0;

  OC2CON2bits.SYNCSEL = 0x1F;
  OC2CON1bits.OCTSEL = 0b111;
  OC2CON1bits.OCM = 0b110;
  OC2CON2bits.OCTRIG = 0;

  _OC2IF = 0;
  _OC2IP = 4;
  _OC2IE = 1;

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
  // set pins 16,17,18 as analog inputs
  _TRISB13 = 1; 
  _ANSB13 = 1;  
  _TRISB14 = 1;
  _ANSB14 = 1;
  _TRISB15 = 1;
  _ANSB15 = 1;

  OC2RS = CONSTANT;
  OC2R = OC2RS / 2;
  OC3RS = CONSTANT;
  OC3R = OC3RS / 2;

  while(1){
    if (WF && WL){
      steps = 0;
      // turn right
      while (steps < STEPS) {
        OC2RS = CONSTANT; 
        OC2R = OC2RS / 2;
        OC3RS = 0; 
        OC3R = OC3RS / 2;
      }
    } 
    if (WF && WR){
      steps = 0;
      // turn left
      while (steps < STEPS) {
        OC2RS = 0; 
        OC2R = OC2RS / 2;
        OC3RS = CONSTANT; 
        OC3R = OC3RS / 2;
      }
    } else {
      OC2RS = CONSTANT;
      OC2R = OC2RS / 2;
      OC3RS = CONSTANT;
      OC3R = OC3RS / 2;
    }
  } // end while loop
  return 0;
} // end main loop
    
    

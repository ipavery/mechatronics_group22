//Copyright MISC

#include "xc.h"

#pragma config FNOSC = LPFRC

#define PWM_PERIOD 1000

int steps = 0;

enum {WAITING, FWD1, TURN1, FWD2, TURN2, FWD3, DRIFT} state;

void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void){
    //clear flag
    _OC2IF = 0;
    //do stuff
    steps++;
}

int main(void) {
    
    //configures timer
    T1CON = 0;
    T1CONbits.TCKPS = 0b11; //prescaler
    _TON = 1;
    TMR1 = 0;
    PR1 = 4883; //5 secs

    _RCDIV = 0; //sets postscaler to 0
    
    //clears start up stuff
    ANSA = 0;
    TRISA = 0;
    LATA = 0;
    ANSB = 0;
    TRISB = 0;
    LATB = 0;
    
    _TRISA0 = 0; //sets pin 2 to output
    _TRISA1 = 0; //sets pin 3 to output
    
    //sets up pwm pin 4
    OC2CON1 = 0x1C06;
    OC2CON2 = 0x001F;
    OC2RS = PWM_PERIOD;
    OC2R = OC2RS/2;
    
    //sets up pwm pin 5
    OC3CON1 = 0x1C06;
    OC3CON2 = 0x001F;
    OC3RS = PWM_PERIOD;
    OC3R = OC3RS/2;
    
    OC2CON1bits.OCM = 0; //TURN OFF pwm to start
    OC3CON1bits.OCM = 0;
    
    //sets up interrupt
    _OC2IF = 0;
    _OC2IE = 1;
    _OC2IP = 4; 
    
    state = WAITING;
    
    int drift = 0;
   
    while(1) {
        switch (state)
        {
            case WAITING:
                if (_RB2) {
                    _LATA0 = 0; //both forward
                    _LATA1 = 0;
                    OC2CON1bits.OCM = 0b110; //TURN on pwm after button press
                    OC3CON1bits.OCM = 0b110;
                    steps = 0;
                    if (drift) {
                        OC2RS = 300;
                        OC2R = OC2RS/2;
                        OC3RS = 300;
                        OC3R = OC3RS/2;
                        _LATA0 = 0; //drift
                        _LATA1 = 1;
                        state = DRIFT;
                    } else {
                        state = FWD1;
                    }
                }
                break;
            
            case FWD1:
                if (steps >= 700) {
                    _LATA1 = !_LATA1;
                    steps = 0;
                    state = TURN1;
                }
                break;
                
            case TURN1: //90 degree turn
                if (steps >= 250) {
                    _LATA0 = 0; //both forward
                    _LATA1 = 0;
                    steps = 0;
                    state = FWD2;
                }
                break;
                
            case FWD2: // going forward before 180 turn
                if (steps >= 700) {
                    _LATA1 = !_LATA1;
               
                    steps = 0;
                    state = TURN2;
                }
                break;
                
            case TURN2: //180 turn
                if (steps >= 480) {
                    _LATA0 = 0; //both forward
                    _LATA1 = 0;
               
                    steps = 0;
                    state = FWD3;
                }
                break;
                
            case FWD3: // last forward
                if (steps >= 700) {
                    OC2CON1bits.OCM = 0; //TURN OFF pwm while waiting
                    OC3CON1bits.OCM = 0;
                    steps = 0;
                    state = WAITING;
                }           
            break;
            
            case DRIFT:
                if (steps > 2000) {
                    
                    steps = 0;
                    state = WAITING;
                }
        }
    }
    return 0;
}



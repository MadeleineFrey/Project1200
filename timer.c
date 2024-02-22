#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */



void timer2_conf (int setTime){
//TIMER2 
  T2CON = 0x0; //clear the control register
  T2CON = 0x70;  //prescaler 1:256 internal clock bit 15 == ON 0.1 s
  TMR2 = 0x0; //Clear timer register
  PR2 = ((80000000)/256)*setTime;  //set the time
}
void timer2Start (void){
    T2CONSET = 0x8000; //start timer
}

void timer2_int (void){
    //interrupts
  IFS(0) &= ~0x100; //reset the INT2IF flag
  IEC(0) |= 0x100; //enable IEC0 T2IE bit 8
  IPC(2) |= 0x1F; //T2IS bit 0-1 (subpriority), T2IP 2-4 (priority)
}

void timer1_conf (int setTime){
      //Timer1
  T1CON = 0x0; //Clear control register
  T1CON = 0x030; //256 prescale
}

void timer3_conf (int setTime){
  T3CON = 0x0; //clear the control register
  T3CON = 0x70;  //prescaler 1:256 internal clock bit 15 == ON 0.1 s
  TMR3 = 0x0; //Clear timer register
  PR3 = ((80000000)/256)*setTime;  //set the time
}

void timer3Start (void){
    T3CONSET = 0x8000; //start timer
}

void timer3Stop (void){
    T3CONSET = ~0x8000; //start timer
}
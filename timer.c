#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/*Written by Erica Engborg*/

void timer2_conf (float setTime){
  T2CON = 0x0; //stop timer and clear the control register
  T2CON = 0x70;  //prescaler 1:256 internal clock bit 15 == ON 0.1 s
  TMR2 = 0x0; //Clear timer register
  PR2 = ((80000000)/256)*setTime;  //set the time
}
void timer2Start (void){
  T2CONSET = 0x8000; //start timer
  }

void timer2Stop (void){
    T2CONSET = ~0x8000; //stop timer
}

void timer3_conf (float setTime){
    T3CON = 0x0; //clear the control register
    T3CON = 0x70;  //prescaler 1:256 internal clock bit 15 == ON 0.1 s
    TMR3 = 0x0; //Clear timer register
    PR3 = ((80000000)/256) * setTime;  //set the time
    
}

void timer3Start (void){
    T3CONSET = 0x8000; //start timer
}

void timer3Stop (void){
    T3CONSET = ~0x8000; //stop timer
}

void timer4_conf (float setTime){
    T4CON = 0x0; //clear the control register
    T4CON = 0x70;  //prescaler 1:256 internal clock bit 15 == ON 0.1 s
    TMR4 = 0x0; //Clear timer register
    PR4 = ((80000000)/256)*setTime;  //set the time
}

void timer4Start (void){
    T4CONSET = 0x8000; //start timer
}

void timer4Stop (void){
    T5CONSET = ~0x8000; //stop timer
}

void timer5_conf (float setTime){
    T5CON = 0x0; //clear the control register
    T5CON = 0x70;  //prescaler 1:256 internal clock bit 15 == ON 0.1 s
    TMR5 = 0x0; //Clear timer register
    PR5 = ((80000000)/256)*setTime;  //set the time
}

void timer5_interrupt (void){
      IFS(0) &= ~TMR5_FLAG;
      IEC(0) |= (1 << 20);
      IPC(5) |= (0x1F); 
}

void timer5Start (void){
    T5CONSET = 0x8000; //start timer
}

void timer5Stop (void){
    T5CONSET = ~0x8000; //stop timer
}

void wait_0_1 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 1)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}

void wait_0_2 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 2)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}

void wait_0_5 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 5)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}

void wait_1 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 10)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}

void wait_2 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 20)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}

void wait_3 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 30)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}

void wait_4 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 40)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}

void wait_5 (void){
    int i=0;
    timer2_conf(0.1);
    timer2Start();
    while (i < 50)
    {
      if(IFS(0) & TMR2_FLAG){
        IFS(0) &= ~TMR2_FLAG;
        i++;
      }
    }
    timer2Stop();
}
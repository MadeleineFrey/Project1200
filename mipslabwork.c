/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int timeoutcount = 0; //new global counter
int data;
char* s;

int pointer = 1; // Start with the first choice selected
int selected = 0; // No choice selected initially


/* Interrupt Service Routine */
void user_isr( void )
{


  if((IFS(0) & 0x100)){ //Timer 2 interrupts
    IFS(0) &= ~0x100; 
    yaxis_data(&data);
    if(data == 1){//1
      PORTE = 0x0F;
    }
    else if(data == 0xFF){//-1
      PORTE = 0xF0;
    }
    else{
      PORTE = 0;
    }
  }

  //Acknowledge interrupts
  //0x001 IFS(0).T2IF
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  TRISECLR = 0xFF; //Set bit 0-7 at 0
  TRISDSET &= 0xFE0; //Set bit 5-11 as inputs 
  TRISFSET |= 0x1; //enable push button 1


  //Switch interrupt
  //IFS(0) &= ~0x80000;
  //IEC(0) |= 0x80000; //enable interrupt
  //IPC(4) |= 0x1F000000;

  //button interrupt


 


  i2c_init();
  adxl_init();  //PORTE |= 0x4;
  
  timer2_conf(0.01);
  timer2_int();
  timer2Start();

  //timer3_conf(0.01);
  //timer3Start();

  enable_interrupt(); //8.6.5 ei
  struct_init(); //maybe remove later if we save the score to the flash memory

}

/* This function is called repetitively from the main program */
void labwork( void )
{
  
  // select();
 while (!selected) {
         int btns = getbtns();

         if (btns & BTN_DOWN) {
             if (pointer > 1) pointer--;
            displayMenu(pointer);

         } else if (btns & BTN_UP) {
             if (pointer < 2) pointer++;
            displayMenu(pointer);

         } else if (btns & BTN_SELECT) {
             selected = pointer; // Select the current choice
             displayMenu(pointer);
             display_update();
              selected = 1; // No choice selected initially

             break; // Exit the loop
         }
}
}
  

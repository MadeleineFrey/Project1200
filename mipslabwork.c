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


int data;
int b;
int BTN_VALUE = 0;
int random;

char test[10]; //delete later!!

/* Interrupt Service Routine */
void user_isr( void )
{
  if((IFS(0) & TMR5_FLAG)){ //Timer 2 interrupts
    IFS(0) &= ~TMR5_FLAG; 
    yaxis_data(&data);
    random = adxl_rand(TMR3);
    if(data <= 3 && data > 0){//1
      ADXL_VALUE = 1;
    }
    else if(data <=520 && data >500){//-1
      ADXL_VALUE = -1;
    }
    else{
      ADXL_VALUE = 0;
    }

    b = getbtns();
    if(b == BTN_DOWN){//1
      BTN_VALUE = 1;
    }
    else if(b == BTN_UP){//-1
      BTN_VALUE = -1;
    }
    else{
      BTN_VALUE = 0;
    }
  }

}

/* Lab-specific initialization goes here */
void labinit( void )
{
  TRISECLR = 0xFF; //Set bit 0-7 at 0
  TRISDSET &= 0xFE0; //Set bit 5-11 as inputs 
  TRISFSET |= 0x1; //enable push button 1

  i2c_init();
  adxl_init();  //PORTE |= 0x4;
  struct_init(); //may be remove later if we save the score to the flash memory


//random timer
  timer3_conf(0.0013);
  timer3Start();

  //This one can start when the game starts
  timer5_conf(0.01);
  timer5_interrupt();
  enable_interrupt();
  timer5Start();  

}

/* This function is called repetitively from the main program */
void labwork( void )
{ 
 start_menu();
 
}
  
/*By Madeleine Frey*/
void start_menu (void){
  int pointer = 1; // Choice selected
  int selected = 0; // No choice selected initially

//First distlay
    display_string(0, "Flappy Bird");
  	display_string(1, "> Play");
 	  display_string(2, "  Highscore");
  	display_update();
    display_image(96, icon);

    wait_1();//This wait is to prevent dubble click

//Use buttons to navigate through the menu
   while (!selected) {
         int btns = getbtns();

         if ((btns & BTN_DOWN)) {
             if (pointer > 1) pointer--;
            displayMenu(pointer);
            display_image(96, icon);
            wait_0_2();

         } else if ((btns & BTN_UP)) {
             if (pointer < 2) pointer++;
            displayMenu(pointer);
            display_image(96, icon2);
            wait_0_2();

         } else if ((btns & BTN_SELECT)) {
             selected = pointer; // Select the current choice
             displayMenu(pointer);
             display_update();
              selected = 1; // No choice selected initially

             break; // Exit the loop
         }
  } 
}

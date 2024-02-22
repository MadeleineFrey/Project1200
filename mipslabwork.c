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

char textstring[] = "text, more text, and even more text!";
#define BTN_UP 0x1     // Binary 001
#define BTN_DOWN 0x2    // Binary 010
#define BTN_SELECT 0x4  // Binary 100 
int pointer = 1; // Start with the first choice selected
int selected = 0; // No choice selected initially

/* Interrupt Service Routine */
void user_isr( void )
{


  if((IFS(0) & 0x100)){
    IFS(0) &= ~0x100; 
    yaxis_data(&data);

    //det här är för att visa y-värdet
    hexToString(data, textstring);

    display_string(1, textstring);
    display_update();
    //Det här är för att visa y-värdet

    if(data == 1){//1
      PORTE = 0x01;
    }
    else if(data == 0xFF){//-1
      PORTE = 0x08;
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
  TRISD &= 0xFE0; //Set bit 5-11 as inputs 

  //TIMER
  T2CON = 0x0; //clear the control register
  T2CON = 0x70;  //prescaler 1:256 internal clock bit 15 == ON 0.1 s
  TMR2 = 0x0; //Clear timer register
  PR2 = ((80000000)/256)/10;  //set the time, calculate this!

  PORTE = 0;
  //interrupts
  //IFS(0) &= ~0x100; //reset the INT2IF flag
  //IEC(0) |= 0x100; //enable IEC0 T2IE bit 8
  //IPC(2) |= 0x1F; //T2IS bit 0-1 (subpriority), T2IP 2-4 (priority)

  //Switch interrupt
  //IFS(0) &= ~0x80000;
  //IEC(0) |= 0x80000; //enable interrupt
  //IPC(4) |= 0x1F000000;

//  enable_interrupt(); //8.6.5 ei

  T2CONSET = 0x8000; //start timer

  i2c_init();
  adxl_init();  //PORTE |= 0x4;

}

/* This function is called repetitively from the main program */
void labwork( void )
{
  // display_image(96, icon);

  //   if ((getbtns() & 0x2) > 0){
  //   display_string(0, "Flappy Bird");
	//   display_string(2, "Highscore");
	//   display_string(3, "Welcome!");
	//   display_update();
	//   display_string(1, "> Play");
  // }
  
  // if ((getbtns() & 0x1) > 0){
	//   display_string(0, "Flappy Bird");
	//   display_string(1, "Play");
	//   display_string(3, "Welcome!");
	//   display_update();
	//   display_string(2, "> highscore");
  //   highscore();
  //   }
	

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

  //       // Add a small delay to debounce buttons and avoid high CPU usage in this loop
  //       // This delay function is platform-specific and needs to be implemented according to your system requirements
  //       // For example, in a Unix-like environment, you might use usleep(100000); // Wait for 100 milliseconds
     }



  
}

void displayMenu(pointer) {
         int btns = getbtns();

    display_string(0, "Flappy Bird");

    if(pointer == 1) {
        display_string(1, "> Play");
        display_string(2, "  Highscore");
        display_update();
        if(btns & BTN_SELECT) {
        play ();

        }

    } else {
        display_string(1, "  Play");
        display_string(2, "> Highscore");
        display_update();
        if(btns & BTN_SELECT){
          highscore();
        } 

    }
    display_update();
}


// void play_game (void) {
// 	  // display_string(0, "spela!");
//     // display_update();

// }

 void highscore (void) {

      display_clear();
      display_update();

      display_string(0, "Highscore");
      display_string(1, "1: namn1");
      display_string(2, "2: namn2");
      display_string(3, "3: namn3");
      display_update();

         int btns = getbtns();

      // if(btns & BTN_SELECT){
      //     selected = 0;
      //     display_clear();
      //     display_update();
      //     labwork();
      //   } 





 }

 void play (void) {
  display_clear();
  display_update();
  run();


 }


// //    if ((getbtns() & 0x4) > 0) { // Check if right button is pressed (bit 2)
// //   display_clear();
// //   display_string(0, "Bird");
// //   display_update();
// // } else if ((getbtns() & 0x1) > 0) { // Check if left button is pressed (bit 0)
// //   // Handle left button press logic here
// //   display_string(2, "> highscore");
// // }

  
// }
    
    


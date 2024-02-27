#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

void displayMenu(pointer) {
         int btns = getbtns();

    display_string(0, "Flappy Bird");
    if(pointer == 1) {
        display_string(1, "> Play");
        display_string(2, "  Highscore");
        display_update();

        if(btns & BTN_SELECT) {
        // display_clear();
        // display_image(10, icon);
        // display_string(0, "                 READY?");
        // display_update();
        // wait_0_5();
        // display_image(10, icon2);
        // wait_0_5();
        // display_string(1, "                 SET");
        // display_update();
        // display_image(10, icon);
        // wait_0_5();
        // display_image(10, icon2);
        // wait_0_5();
        // display_string(2, "                 GOOO!");
        // display_image(10, icon);
        // wait_1();
        play ();
        }


    } else {
        display_string(1, "  Play");
        display_string(2, "> Highscore");
        display_update();
        if(btns & BTN_SELECT){
          view_highscore();
        } 

    }
    display_update();

}

 void play (void) {
  display_clear();
  display_update();
  run();


 }


void icon_move (void){
  int count = 0;
  display_image(96, icon);
  while (count<4)
  {
    wait_0_5();
    display_image(96, icon2);
    wait_0_5();  
    display_image(96, icon);
    count++;
  }
}
void icon_move_start (void){
  int count = 0;
  display_image(50, icon);
  while (count<5)
  {
    wait_0_5();
    display_image(50, icon2);
    wait_0_5();  
    display_image(50, icon);
    count++;
  }
}

// void play_game (void) {
// 	  // display_string(0, "spela!");
//     // display_update();

// }

/*
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
*/



// //    if ((getbtns() & 0x4) > 0) { // Check if right button is pressed (bit 2)
// //   display_clear();
// //   display_string(0, "Bird");
// //   display_update();
// // } else if ((getbtns() & 0x1) > 0) { // Check if left button is pressed (bit 0)
// //   // Handle left button press logic here
// //   display_string(2, "> highscore");
// // }

  
// }
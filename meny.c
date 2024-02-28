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
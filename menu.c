#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define BTN_UP 0x1     // Binary 001
#define BTN_DOWN 0x2    // Binary 010
#define BTN_SELECT 0x4  // Binary 100 
// int pointer = 1; // Start with the first choice selected
/*int selected = 0; // No choice selected initially
int pointer = 1; // Start with the first choice selected

void select(){
    while (!selected) {
         int btns = getbtns();

         if (btns & BTN_DOWN) {
             if (pointer > 1) pointer--;
            displayMenu(pointer);

         } else if (btns & BTN_UP) {
             if (pointer < 2) pointer++;
            displayMenu(pointer);

         } else if (btns & BTN_SELECT) {
             selected = pointer; 
             displayMenu(pointer);
             display_update();
              selected = 1; 

             break; 
         }
     }
}

int displayMenu(int x) {
         int btns = getbtns();

    display_string(0, "Flappy Bird");

    if(x == 1) {
        display_string(1, "> Play");
        display_string(2, "  Highscore");
        display_update();
        if(btns & BTN_SELECT) {
        timer3_conf (0.1);
        timer3Start ();
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

 void highscore (void) {

      display_clear();
      display_update();

      display_string(0, "Highscore");
      display_string(1, "1: namn1");
      display_string(2, "2: namn2");
      display_string(3, "3: namn3");
      display_update();

      int btns = getbtns();
 }

 void play (void) {
  display_clear();
  display_update();
  run();
 }
 */

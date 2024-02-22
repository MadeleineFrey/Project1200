/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int main(void) {
	start_init();
	display_init();

	
  	display_string(0, "Flappy Bird");
  	display_string(1, "> Play");
 	display_string(2, "  Highscore");
  	display_update();
	
	 display_image(96, icon);
	 display_update();
	 
	labinit();

    // Further code for action based on selected choice
	while( 1 )
	{
	  labwork(); /* Do lab-specific things again and again */
	}

    return 0;
}

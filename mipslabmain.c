/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int main(void) {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;
	// labwork();
	// display_init();
	// display_string(0, "Flappy Bird");
	// display_string(1, "Play");
	// display_string(2, "Highscore");
	// display_string(3, "Welcome!");
	// display_update();
	display_init();
  	display_string(0, "Flappy Bird");
  	display_string(1, "> Play");
 	display_string(2, "  Highscore");
  	display_update();
	// display_image(96, icon);
	
	//labinit(); /* Do any lab-specific initialization */
	// // menu();

	// #define BTN_UP 1      // Binary 001
	// #define BTN_DOWN 2    // Binary 010
	// #define BTN_SELECT 4  // Binary 100 
	// int pointer = 1; // Start with the first choice selected
    // int selected = 0; // No choice selected initially

    // while (!selected) {
    //     displayMenu(pointer);
    //     int btns = getbtns();

    //     if (btns & BTN_UP) {
    //         if (pointer > 1) pointer--;
    //     } else if (btns & BTN_DOWN) {
    //         if (pointer < 2) pointer++;
    //     } else if (btns & BTN_SELECT) {
    //         selected = pointer; // Select the current choice
            
    //         char selectionMsg[20];
    //         snprintf(selectionMsg, sizeof(selectionMsg), "Choice %d selected!", selected);
    //         display_string(1, selectionMsg); // Display selection on the first line
    //         break; // Exit the loop
    //     }

    //     // Add a small delay to debounce buttons and avoid high CPU usage in this loop
    //     // This delay function is platform-specific and needs to be implemented according to your system requirements
    //     // For example, in a Unix-like environment, you might use usleep(100000); // Wait for 100 milliseconds
    // }

    // Further code for action based on selected choice
	while( 1 )
	{
	  labwork(); /* Do lab-specific things again and again */
	}

    return 0;
}

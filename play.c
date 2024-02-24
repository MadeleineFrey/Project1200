#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdio.h>


//Variables
#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

//Height and width of the screen
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 128
//Height and width of the bird
#define BIRDH 4
#define BIRDW 5

//
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32 
#define PAGE_HEIGHT 8 
#define NUM_PAGES (SCREEN_HEIGHT / PAGE_HEIGHT) 





char* s;
int randomPipeNumber = 0;
int score = 0;


uint8_t screen[128*4] = {0}; //Display
extern uint8_t display[32][128];

double xpos = 0; 
double ypos = 12; 
    
void clear() {
    int i;
    for (i = 0; i < sizeof(screen); i++) {
        screen[i] = 0;
    }
}

uint8_t screen[DISPLAY_WIDTH * NUM_PAGES]; 

void draw(uint8_t *arr) {
    int i, j;
    if (xpos + BIRDW > DISPLAY_WIDTH || ypos + BIRDH > DISPLAY_HEIGHT) return;

        uint8_t bird[BIRDH] = {0b1011, 0b1001, 0b0000, 0b1000, 0b0010};


    for(i = ypos; i < ypos + BIRDH; i++) {
        for(j = xpos; j < xpos + BIRDW; j++) {
            int page = i / PAGE_HEIGHT;
            int bit = i % PAGE_HEIGHT;

            arr[page * DISPLAY_WIDTH + j] |= (1 << bit); 
        }
    }  

 
}

//fixa pixlarna uppe och nere (Maxpunkt)
void movement(){
     int btns = getbtns();
     if(ypos >= 28){
                ypos = 27;
            }

    if(ypos <= 0){
        ypos = 1;
    }

          if (btns & BTN_UP) {
            
             ypos = ypos + 0.2;
            draw(screen); 
            render(screen); 
          }

           if (btns & BTN_DOWN) {
             ypos = ypos - 0.2;
            draw(screen); 
            render(screen); 
          }
 }


 void aMove(){
    if(ypos >= 28){
                ypos = 27;
            }

    if(ypos <= 0){
        ypos = 1;
    }

          if (ADXL_VALUE == 1) {
            
             ypos = ypos + 0.2;
            draw(screen); 
            render(screen); 
          }

           if (ADXL_VALUE == -1) {
             ypos = ypos - 0.2;
            draw(screen); 
            render(screen); 
          }


 }
 


random_pipe_number(){

    int randomSeed = 0;
    int randTemp = TMR3;
    
   randTemp &= 0xF;

// Ensure 0 is mapped to 1 since your operation could result in 0,
// which is not within the desired 1-15 range.
if (randTemp == 0) {
    randTemp = 1;
}
      return randTemp;
}



 draw_pipes_under(uint8_t *arr, int x, int y, int w){
    int i, j;
    int h = random_pipe_number();
    y = 32 - h;

    if (x + w > DISPLAY_WIDTH || y + h > DISPLAY_HEIGHT) return;

    for(i = y; i < y + h; i++) {
        for(j = x; j < x + w; j++) {
            int page = i / PAGE_HEIGHT;
            int bit = i % PAGE_HEIGHT;
            arr[page * DISPLAY_WIDTH + j] |= (1 << bit); 
        }
    }
 }

void play_r() {
int playing = 1; //boolean to known when you are playing or not
    while(playing) {
        clear();
        draw(screen); 
        // render(screen); 
        // movement();
        aMove();
        draw_pipes_under(screen, 10, 0, 4);
        render(screen); 

        
    }
    }

//Infinite loop that switches between the menu, high score, game, and game over
void run() {
    while (1) {
        play_r();
    }
}

void render(uint8_t *arr) {
    int i, j;

    for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

//copied from display_image function written by.....
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

        for(j = 0; j < 128; j++)
            spi_send_recv(arr[i*128 + j]); 
    }
}

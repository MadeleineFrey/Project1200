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


//Height and width of the bird
#define BIRDH 4
#define BIRDW 4

//Height and width of the screen
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32 
#define PAGE_HEIGHT 8 
#define NUM_PAGES (DISPLAY_HEIGHT / PAGE_HEIGHT) 

//Bird
double xpos = 0; 
double ypos = 12; 

//Pipes
float pipeW = 4;
float x_one = 124;
float yp = 0; //ta bort?
int score = 0;

//Display variables and screen.
uint8_t screen[128*4] = {0};
uint8_t screen[DISPLAY_WIDTH * NUM_PAGES]; 

// Clear the screen.    
void clear_screen() {
    int i;
    for (i = 0; i < sizeof(screen); i++) {
        screen[i] = 0;
    }
}

//Draw bird 
void draw(uint8_t *arr) {
    int i, j;
    if (xpos + BIRDW > DISPLAY_WIDTH || ypos + BIRDH > DISPLAY_HEIGHT) return;

    for(i = ypos; i < ypos + BIRDH; i++) {
        for(j = xpos; j < xpos + BIRDW; j++) {
            int page = i / PAGE_HEIGHT;
            int bit = i % PAGE_HEIGHT;

            arr[page * DISPLAY_WIDTH + j] |= (1 << bit); 
        }
    }  
}

// Move bird with buttons. 
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

// Move bird with accelerometer
 void aMove(){
    if(ypos >= 27){
                ypos = 26;
            }

    if(ypos <= 0){
        ypos = 1;
    }
          if (ADXL_VALUE == 1) {
            
             ypos = ypos + 0.2;
            draw(screen); 
          }

           if (ADXL_VALUE == -1) {
             ypos = ypos - 0.2;
            draw(screen); 
          }
 }

#define MAX_PIPES 3
#define PIPE_SPACING 55
#define PIPE_GAP 20

float pipe_positions[MAX_PIPES] = {0};
float pipe_top_heights[MAX_PIPES];
int pipe_count = 0;
int pipe_heights[MAX_PIPES];


void draw_pipes_under(uint8_t *arr, int x, int height){
    int i, j;
    int y = 32 - height;

    if (x + pipeW > DISPLAY_WIDTH || y < 0) return;

    for(i = y; i < y + height; i++) {
        for(j = x; j < x + pipeW; j++) {
            int page = i / PAGE_HEIGHT; 
            int bit = i % PAGE_HEIGHT;
            arr[page * DISPLAY_WIDTH + j] |= (1 << bit);
        }
    }
}

void draw_pipes_over(uint8_t *arr, int x, int bottom_pipe_height){

    int overpipe_height = 32 - bottom_pipe_height - PIPE_GAP; 
    int i, j;
    int y = 0;

    if (x + pipeW > DISPLAY_WIDTH || overpipe_height <= 0) return; 

    for(i = y; i < y + overpipe_height; i++) {
        for(j = x; j < x + pipeW; j++) {
            int page = i / PAGE_HEIGHT;
            int bit = i % PAGE_HEIGHT;
            arr[page * DISPLAY_WIDTH + j] |= (1 << bit);
        }
    }
}


void move_pipes() {
   int i = 0;
   int j = 0;

   for (i = 0; i < pipe_count; i++) {
       pipe_positions[i] -= 0.05;
       draw_pipes_under(screen, pipe_positions[i], pipe_heights[i]);
       draw_pipes_over(screen, pipe_positions[i], pipe_heights[i]);
   }

   if (pipe_count == 0 || (pipe_count < MAX_PIPES && pipe_positions[pipe_count - 1] < DISPLAY_WIDTH - PIPE_SPACING)) {
       pipe_positions[pipe_count] = DISPLAY_WIDTH;
       pipe_heights[pipe_count] = random;
       pipe_count++;
   }

   if (pipe_positions[0] < -pipeW) {
       for (j = 0; j < pipe_count - 1; j++) {
           pipe_positions[j] = pipe_positions[j + 1];
           pipe_heights[j] = pipe_heights[j + 1];
       }
       pipe_count--;
       score++;
   }
}

check_collision() {
    int bird_left = xpos;
    int bird_right = xpos + BIRDW;
    int bird_top = ypos;
    int bird_bottom = ypos + BIRDH;
    int i = 0;
     

    for (i = 0; i < pipe_count; i++) {
        int pipe_left = pipe_positions[i];
        int pipe_right = pipe_positions[i] + pipeW;
        int bottom_pipe_top = DISPLAY_HEIGHT - pipe_heights[i];
        int bottom_pipe_bottom = DISPLAY_HEIGHT;
        int top_pipe_bottom = DISPLAY_HEIGHT - pipe_heights[i] - PIPE_GAP;

        // Check collision with bottom pipe
        if (bird_right > pipe_left && bird_left < pipe_right &&
            bird_bottom > bottom_pipe_top && bird_top < bottom_pipe_bottom) {
            return 1; // Collision detected
        }

        // Check collision with top pipe
        if (bird_right > pipe_left && bird_left < pipe_right &&
            bird_bottom > 0 && bird_top < top_pipe_bottom) {
            return 1; // Collision detected
        }
    }

    return 0; // No collision detected
}

int playing = 1;
void play_r() {
    int a = 1; 
    while(playing) {

        clear_screen();
        draw(screen);
        move_pipes();
        aMove();
        
        if (check_collision()) {
            clear_screen();
            new_highscore (score);
            display_update();
            playing = 0;
        }

        render(screen); 
    }
}
    
void run() {
    score = 0;
    xpos = 0;
    ypos = 12;
    playing = 1; 

    while (1) {
        play_r();
    }
}

void render(uint8_t *arr) {
    int i, j;

    for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

//copied from display_image function.
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

        for(j = 0; j < 128; j++)
            spi_send_recv(arr[i*128 + j]); 
    }
}

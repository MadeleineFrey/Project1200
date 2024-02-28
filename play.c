#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdio.h>
#include <math.h>


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

//Game
int score = 1;

//Bird
double xpos = 2; 
double ypos = 12; 

//Pipes
float pipeW = 4;
float x_one = 124;
float pipe_speed = 0.10;

//Display variables and screen.
uint8_t screen[128*4] = {0};
// uint8_t screen[DISPLAY_WIDTH * NUM_PAGES]; 


//Move variables
#define MAX_PIPES 3
#define PIPE_SPACING 55
#define PIPE_GAP 18

// Moving pipe values
float pipe_positions[MAX_PIPES] = {0};
float pipe_top_heights[MAX_PIPES];
int pipe_count = 0;
int pipe_heights[MAX_PIPES];

// Clear the screen pixel by pixel and reset them to 0/black.    
void clear_screen() {
    int i;
    //Iterates through screen.
    for (i = 0; i < sizeof(screen); i++) {
        screen[i] = 0; // Turn pixel off
    }
}

//Draw bird, Drawing bird and calculates and sets the appropriate bits for each pixel's position.
void draw(uint8_t *arr) {
    int i, j;
    if (ypos + BIRDH > DISPLAY_HEIGHT || ypos <= 0) return;

// Loop through each pixel in the bird's height
    for(i = ypos - 0.5; i < ypos - 0.5 + BIRDH; i++) {
         // Loop through each pixel in the bird's width
        for(j = xpos - 0.5; j < xpos -0.5 + BIRDW; j++) {
            //Which page of the display the pixel falls on
            int page = i / PAGE_HEIGHT;
            // The specific bit within the page that needs to be set
            int bit = i % PAGE_HEIGHT;
    // Set the bit in the array to 1 to draw the pixel of the pipe.
            arr[page * DISPLAY_WIDTH + j] |= (1 << bit); 
        }
    }  
}

/*
//Move bird with buttons

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

 */

// Move bird upp and down with accelerometer.
 void aMove(){

    //Display bounderies.
    if(ypos >= 28){
                ypos = 27;
            }
    if(ypos <= 1){
        ypos = 2;
    }
          if (ADXL_VALUE == 1) {
            //move bird + 0.1 pixels
             ypos = ypos + 0.1;
            draw(screen); //Draw bird with new value
          }

           if (ADXL_VALUE == -1) {
            //move bird  0.1 pixels
             ypos = ypos - 0.1;
            draw(screen); //Draw bird with new value
          }
 }

/*

The display's pixel data is stored in a one-dimensional array. 
The function takes the position x and height of the pipe to draw The height
is a random number from the timer and accelerometer. 

Checks if the pipe fits within the display. if not, it exits without drawing the pipe.

it calculates the position in the array for each pixel of the pipe and sets the
corresponding bit to turn that pixel on.

*/
void draw_pipes_under(uint8_t *arr, int x, int height){
    int i, j;
    int y = 32 - height; // y position

//Safety for if pipe the value is outside.
    if (x + pipeW > DISPLAY_WIDTH || y < 0) return;

// Loop over each pixel in the height of the under pipe.
    for(i = y; i < y + height; i++) {
        // Loop over each pixel in the width of the pipe.
        for(j = x; j < x + pipeW; j++) {
            //Page row
            int page = i / PAGE_HEIGHT; 
            //specific bit within the byte that corresponds to the pixel.
            int bit = i % PAGE_HEIGHT;
            // Set the bit in the array to 1 to draw the pixel of the pipe.
            arr[page * DISPLAY_WIDTH + j] |= (1 << bit);
        }
    }
}

//Same logic as under pipe but gets height from the underpipe - pipegap.
void draw_pipes_over(uint8_t *arr, int x, int bottom_pipe_height){

// height of the upper pipe based on the height of the bottom pipe and pipegap.
    int overpipe_height = 32 - bottom_pipe_height - PIPE_GAP; 
    int i, j;
    int y = 0;

//Safety for if pipe the value is outside.
    if (x + pipeW > DISPLAY_WIDTH || overpipe_height <= 0) return; 

// Loop over each pixel in the height of the upper pipe.
    for(i = y; i < y + overpipe_height; i++) {

        // Loop over each pixel in the width of the pipe.
        for(j = x; j < x + pipeW; j++) {
            //Page row
            int page = i / PAGE_HEIGHT;
            //specific bit within the byte that corresponds to the pixel.
            int bit = i % PAGE_HEIGHT;
            // Set the bit in the array to 1 to draw the pixel of the pipe.
            arr[page * DISPLAY_WIDTH + j] |= (1 << bit);
        }
    }
}

// Function to update and manage the movement of pipes across the screen.
void move_pipes() {
   int i = 0; // Loop for pipe
   int j = 0; // Loop for array

// Loop through each pipe to update its position and draw it on the screen.
   for (i = 0; i < pipe_count; i++) {
       pipe_positions[i] -= pipe_speed; // moves the pipe, bigger value for faster movement.
       draw_pipes_under(screen, pipe_positions[i], pipe_heights[i]);
       draw_pipes_over(screen, pipe_positions[i], pipe_heights[i]);
   }

// Check if a new pipe needs to be added to the screen.
   if (pipe_count == 0 || (pipe_count < MAX_PIPES && pipe_positions[pipe_count - 1] < DISPLAY_WIDTH - PIPE_SPACING)) {
       pipe_positions[pipe_count] = DISPLAY_WIDTH;
       pipe_heights[pipe_count] = random;
       pipe_count++;
   }

// Check if the leftmost pipe has completely moved out of the screen and needs to be removed.
   if (pipe_positions[0] < -pipeW) {
    //Shift pipes after removing and height
       for (j = 0; j < pipe_count - 1; j++) {
           pipe_positions[j] = pipe_positions[j + 1];
           pipe_heights[j] = pipe_heights[j + 1];
       }
       pipe_count--; // Decrement the count of pipes.
       score++; // Increment the score when a pipe has passed.
   }
}

check_collision() {
    //Border for the bird.
    int bird_left = xpos;
    int bird_right = xpos + BIRDW;
    int bird_top = ypos;
    int bird_bottom = ypos + BIRDH;
    int i = 0;
     
// Loop through all the pipes to check for any collisions.
    for (i = 0; i < pipe_count; i++) {

        // Set the borders for the pipe.
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
    while(playing) {

        clear_screen(); // Clears the screen for fresh drawing.
        draw(screen); // Draw the bird.
        move_pipes(); // Move the pipes to the left.
        aMove(); // Bird movement.
        
        // Check for collisions with pipes.
        if (check_collision()) {
            clear_screen();
            new_highscore (score - 1);
            playing = 0; // Stop playing.
        }

// Check if score is a multiple of 10 and not zero and increase speed.
        if (score % 10 == 0 && score != 0) { 
        pipe_speed = 0.10 + ((score / 10) * 0.05);
        }
        render(screen);  // Render the updated screen to the display.
    }
}
    
void run() {
    //Reset values to play again.
    xpos = 2;
    ypos = 12;
    playing = 1; 
    score = 1;
    pipe_count = 0;
    pipe_speed = 0.10;

    while (1) {
        play_r(); // Play game
    }
}

//copied from display_image function.
// Function to render on a display using SPI communication.
void render(uint8_t *arr) {
    int i, j;

// Loop through each of the 4 pages of the display
    for(i = 0; i < 4; i++) {
        //Send to the display, command mode
		DISPLAY_CHANGE_TO_COMMAND_MODE;

        //set the page address
        spi_send_recv(0x22);
        //Current page number
        spi_send_recv(i);

        //set the column address to 0. The start of the column.
        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));

        //Send to display, data mode.
		DISPLAY_CHANGE_TO_DATA_MODE;

// Loop through columns of current page
        for(j = 0; j < 128; j++)
        //Send pixeldata
            spi_send_recv(arr[i*128 + j]); 
    }
}

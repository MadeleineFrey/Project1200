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
#define BIRDW 5

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
float yp = 0;

int score = 0;

// Display variables and screen
uint8_t screen[128*4] = {0};
// extern uint8_t display[32][128]; // samma
uint8_t screen[DISPLAY_WIDTH * NUM_PAGES]; //samma

// Clear the screen    
void clear() {
    int i;
    for (i = 0; i < sizeof(screen); i++) {
        screen[i] = 0;
    }
}

//Dra bird 
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

// Move bird with buttons 
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

    int randTemp = 0;
     randTemp = TMR3;
    
   randTemp &= 0xF;
if (randTemp == 0) {
    randTemp = 1;
}
      return randTemp;
}


#define MAX_PIPES 3
#define PIPE_SPACING 55
float pipe_positions[MAX_PIPES] = {0}; // Array to track the x positions of pipes
// skapa eventuellt en array med h som alla är olikt varandra
float pipe_top_heights[MAX_PIPES];
int pipe_count = 0; 


 draw_pipes_under(uint8_t *arr, float x, float he){
    int i, j;
    he = random_pipe_number();
    yp = 32 - he;

    if (x + pipeW > DISPLAY_WIDTH || yp + he > DISPLAY_HEIGHT) return;

    for(i = yp; i < yp + he; i++) {
        for(j = x; j < x + pipeW; j++) {
            int page = i / PAGE_HEIGHT;
            int bit = i % PAGE_HEIGHT;
            arr[page * DISPLAY_WIDTH + j] |= (1 << bit);
        }
    }
 }


// Move all pipes, and draws them at new position (removes pipe that goes off screen)
int pipe_heights[MAX_PIPES]; // Global array to store the height of each pipe

void move_pipes() {
    int i = 0;
    int j = 0;

    for (i = 0; i < pipe_count; i++) {
        pipe_positions[i] -= 0.1;
        draw_pipes_under(screen, pipe_positions[i], pipe_heights[i]); 
    }

    if (pipe_count == 0 || (pipe_count < MAX_PIPES && pipe_positions[pipe_count - 1] < DISPLAY_WIDTH - PIPE_SPACING)) {
        pipe_positions[pipe_count] = DISPLAY_WIDTH;
        // Generate a new height for the new pipe and store it in the array
        pipe_heights[pipe_count] = random_pipe_number(); 
        pipe_count++;
    }

    if (pipe_positions[0] < -pipeW) {
        for (j = 0; j < pipe_count - 1; j++) {
            pipe_positions[j] = pipe_positions[j + 1];
            pipe_heights[j] = pipe_heights[j + 1];
        }

        pipe_count--;
        score ++;
    }
}



// old move
/*  move_pipe(){
         draw_pipes_under(screen, x_one);
        x_one = x_one - 0.1;
     }
*/ 



/*
Koliderar när man rör sig första gången :/ kolla border
*/
int check_collision() {
    // osäker på detta
    float bird_left = xpos;
    float bird_right = xpos + BIRDW - 1;
    float bird_top = ypos;
    float bird_bottom = ypos - BIRDH + 1;

    int i = 0;
    
    for (i = 0; i < pipe_count; i++) {
        // kolla varför man ej kan röra den och fortsätta
        float pipe_left = pipe_positions[i];
        float pipe_right = pipe_positions[i] + pipeW;
        float upper_pipe_bottom = pipe_heights[i];
        float lower_pipe_top = upper_pipe_bottom + pipeW;

        

        // Check for collision with upper pipe
        if (bird_right > pipe_left && bird_left < pipe_right && 
            bird_top < upper_pipe_bottom) {
            return 1; // Collision detected
        }

        // Check for collision with lower pipe
        if (bird_right > pipe_left && bird_left < pipe_right && 
            bird_bottom > lower_pipe_top) {
            return 1; // Collision detected
        }
    }
    return 0; // No collision
}

void play_r() {
    int playing = 1;
    while(playing) {
        clear();
        draw(screen);
        aMove();
        move_pipes();
        
        if (check_collision()) {
            clear();
            new_highscore (score);
            display_update(); //?? varför satte jag den? 
            playing = 0;
        }

        render(screen); 
    }
}

    
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

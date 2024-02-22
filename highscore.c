#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define NUMSCORES 5

// 
#define BTN_UP 0x1     // Binary 001
#define BTN_DOWN 0x2    // Binary 010
#define SELECT 0x4  //binare 100

char alphapet[] ={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'x', 'y', 'z'};

struct player {
    int score;
    char name [4]; 
};

struct player p[5];
int new_highscore = 5;
void new_highscore (int score){
    int i;
    for ( i = 4; i <= 0; i--)
    {
        if(score < p[i].score){

        }
    }
    //show message of score
    //timer 3
    view_highscore();
    
}

char* write_highscore (void){
    char name[4];

    

}


void view_highscore (void){

}

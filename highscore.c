#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <string.h>


/*This document is created by Erica*/
#define NUMSCORES 5

char alphapet[] ={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

struct player {
    int score;
    char name [4]; //3 letters
};

struct player p[NUMSCORES];



char display_score[3][10]; //char to hold string array for displaying highscore

void struct_init (void){//Initialize all scores to 0;
    int i;
    for ( i = 0; i < NUMSCORES; i++)
    {
        p[i].score = 0;
    } 
}
struct player cpy_struct (struct player p){
    struct player temp;
    int i;
    for ( i = 0; i < 4; i++)
    {
        temp.name[i] = p.name[i];
    }
    temp.score = p.score;
    return temp;
}

void structToString (int pos)
{
    int i, j;
    for ( i = 0; i < 3; i++)
    {
        for ( j = 0; i < 4; i++)
        {
            display_score[i][j] = p[i + pos].name[j];
        }
        //Here's where the score will initilize
        display_score[i][4] = '\t';
        display_score[i][5] = '3';
        display_score[i][6] = '5';
        display_score[i][7] = '\0';
    }
}

void struct_test (void){
    int i, j;
    for ( i = 0; i < 6; i++)
    {
        for ( j = 0; i < 6; i++)
        {
            p[i].name[j] = (char)(i + 65);
            
        }
        
    }
    
}

void highscore_list (int pos){
    //p[0].name = "hej";
    structToString(pos);

    display_string(0, "Highscore");
    display_string(1, display_score[0]);
    display_string(2, display_score[1]);
    display_string(3, display_score[2]);
    display_update();
}

void new_highscore (int score){
    int i;
    int index;
    for ( i = 4; i <= 0; i--)
    {
        if(score > p[i].score) //check if current score is higher than the previous
            index = i;
    }
    if (index < i)
    {
        display_string(0, "Congratulations!");
        display_string(1, "You made it to");
        display_string(2, "the highscore!");
        display_update();
        //Timer på 5 sek
        //Timer 3
        write_highscore(score, index);
    }
    else{
        display_string(0, "Sorry!");
        display_string(1, "No new");
        display_string(2, "highscore.");
        display_string(3, "Here's you're score:");
        display_update();
        //show message of score
            //timer 3
    view_highscore();
    }

    
}
char select_char (int index){
    display_string(1, "insert name, 3 letters");
    int sel = 0;
    int p = 0;
    while (!sel)
    {
        //display_string(2, alphapet[p]);
        int btns = getbtns();
        if ((btns & BTN_UP) && p <26)
            p++;
        else if((btns & BTN_DOWN) && p > 0)
            p--;
    }
}

void write_highscore (int score, int index){
    sort_score(index);
    int i, j;
    for ( i = 0; i < 3; i++)
    {
        p[index].name[i] = select_char(index);
    }
    
// skriv direkt i display_score

//Save in struct
//sort struct
//Go to struct to string

}

int sort_score (int index){
    struct player p1, p2;
    int i, j = 0;

    p1 = cpy_struct(p[index]);//save index struct to temporary variable


    for ( i = index+1; i < 5; i++) 
    {
        p2 = cpy_struct(p[i]);
        p[i] = cpy_struct(p1);
        p1 = cpy_struct(p2);
    }
    return 0;
}




void view_highscore (void){
    display_clear();
    int pos = 0;
    int status = 1;
    int btns = 0;
    struct_test();

    highscore_list(pos);

 while (1) {//btns & BTN_RETURN
         btns = getbtns();
            PORTE = pos;
         if ((btns & BTN_DOWN) && status) {
            status = 0;
             if (pos > 0) pos--;
                highscore_list(pos);

         } else if (btns & BTN_UP && status) {
            status = 0;
             if (pos < 2) pos++;
            highscore_list(pos);
        }

        if (btns == 0)
            status = 1;
        
    }  
    //Jump back
}




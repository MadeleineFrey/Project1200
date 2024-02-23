#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <string.h>


/*This document is created by Erica*/


char alphapet[] ={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

struct player {
    char place[3]; 
    int score;
    char name [5]; //3 letters
};

struct player p[5];



char display_score[3][12]; //char to hold string array for displaying highscore


void struct_init (void){//Initialize all scores to 0;
    int i, j;
    char test[10];
    for ( i = 0; i < 5; i++)
    {
        p[i].score = 0;

        for ( j = 0; j < 3; j++)
            p[i].name[j]= 42;  
        p[i].name[3] = '\0';

        p[i].place[0]='1'+i;
        p[i].place[1] = '.';
        p[i].place[2]='\0';    
    } 
}

struct player cpy_struct (struct player p){
    struct player temp;
    int i;
    for ( i = 0; i < 3; i++)
    {
        temp.name[i] = p.name[i];
    }
    temp.name[3] = '\0';
    temp.score = p.score;
    return temp;
}


int intlength (int score){
    int len = 1;
    while (score > 9) //Calculate the lengt of the integer
    {
        len ++;
        score /= 10;
    }
    return len;
}

void scoreToStr(int score, char* str){
    int len = intlength(score);
    int i, temp;

    if (len > 4){
        display_string(0, "Score to high");
        display_update();
        //TIMER!!!!!
    }

    for ( i = len-1; i >= 0; i--) //extract each digit of the integer and store it in the char array.
    {
        temp = score % 10;
        str[i] = temp + '0';
        score /= 10;
    }
    str[len] = '\0';
}


void structToString (int pos)
{
    int i, j=0, k=0;
    char temp[5];
    for ( i = 0; i < 3; i++)
    {
        j = 0;
        k=0;
        while (p[i+pos].place[k] != '\0'){
            display_score[i][j] = p[i+pos].place[k];
            j++;
            k++;
        }
        display_score[i][j] = ' ';
        j++;

        k=0;
        while (p[i+pos].name[k] != '\0')
        {
            display_score[i][j] = p[i+pos].name[k];
            j++;
            k++;
        }
        display_score[i][j] = '\t';
        j++;

        scoreToStr(p[i+pos].score, temp);
        k=0;
        while (temp[k] != '\0')
        {
            display_score[i][j] = temp[k];
            j++;
            k++;
        }   
        display_score[i][j]='\0';
    }
}
  
void highscore_list (int pos){
    structToString(pos);

    display_string(0, "Highscore");
    display_string(1, display_score[0]);
    display_string(2, display_score[1]);
    display_string(3, display_score[2]);
    display_update();
}


int sort_score (int index){
    int i;

    for ( i = 4; i > index; i--) 
    {
        p[i] = cpy_struct(p[i-1]);
    }
    return 0;
}


void write_highscore (int score, int index){
    sort_score(index); //Gives place for the new highscore
    int i=0, j=0, count=0, btns=0, status=0;

    for ( i = 0; i < 3; i++)
        p[index].name[i]= ' ';//remove name
    p[index].name[3]='\0';

    p[index].score = score;//Save the score
    i = 0;
    display_string(0, "Enter your name:");
    display_string(1, p[index].name);
    display_update();

    while (count != 3)//insert three charachers
    {
        btns = getbtns();
        p[index].name[i]=alphapet[j];
        display_string(1, p[index].name);
        if ((btns & BTN_SELECT) && !status)
        {

            display_update();
            status = 1;
                        count++;
            i++;
        }
        else if((btns & BTN_DOWN) && !status){
            if(j < 25) j++;
            display_update();
            status = 1;
        }
        else if((btns & BTN_UP) && !status){
            if(j > 0) j--;
            display_update();
            status = 1;
        }
        else if(btns == 0)
            status = 0;  


    }  
p[index].name[3]='\0';
display_clear();
view_highscore();
}

void test_highscore(void){
    int btns = getbtns(), status = 0;
    int score= 0;
    char str [5];
    display_clear();
    display_string(1, "score:");
    scoreToStr(score, str);
    display_string(2, str);
    display_update();
    while (!(btns & BTN_RETURN))
    {
        btns = getbtns();
         if ((btns & BTN_DOWN) && !status && score > 0) {
            score--;
            scoreToStr(score, str);
            display_string(2, str);
            display_update();
            status = 1;

         } else if (btns & BTN_UP && !status) {
            score++;
            scoreToStr(score, str);
            display_string(2, str);
            display_update();
            status = 1;
        }
        else if(btns == 0)
            status = 0;
    }
    new_highscore(score);
}

void new_highscore (int score){
    int index = -1, i; //-1 as start value fro index
    char temp[5];
    timer3_conf(0.1);
    timer3Start();

    for ( i = 4; i >= 0; i--)
    {
        if(score > p[i].score) //check if current score is higher than the previous
            index = i;
    }
        scoreToStr(score, temp);

    if (index != -1)
    {
        display_string(0, "Congratulations!");
        display_string(1, "You made it to");
        display_string(2, "the highscore!");
        display_string(3, temp);
        display_update();
        

        icon_move();
  
  
        display_clear();
        write_highscore(score, index); 
    }
    else{

        display_string(0, "Sorry!");
        display_string(1, "No new");
        display_string(2, "highscore.");
        display_string(3, temp);
        display_update();

        wait_4();
        display_clear();
        view_highscore();
    }   
}

void view_highscore (void){
    display_clear();
    int pos = 0;
    int status = 1;
    int btns = 0;

    highscore_list(pos);
    wait_1();
 while (!(btns & BTN_SELECT)) {//
         btns = getbtns();
         if ((btns & BTN_DOWN) && status) {
            status = 0;
             if (pos > 0) pos--;
                highscore_list(pos);

         } else if (btns & BTN_UP && status) {
            status = 0;
             if (pos < 2) pos++;
            highscore_list(pos);
        }
        else if (btns == 0)
            status = 1;        
    } 
    display_clear(); 
    start_menu();
}



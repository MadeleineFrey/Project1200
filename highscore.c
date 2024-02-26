#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h" 
#include <string.h>


/*This document is created by Erica*/


/*An array that hold the alphabet for the writing*/
char alphabet[] ={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

/*A struct that hold information from the highscore list. Place is a chare array that hold the position and a dott after that. 
That corresponds to wich position in the struct array the player is and the position on the highscore list. The name stores three letters and a null
terminator. The score holds the score upp to 9999. The limit of the score is set in a char array when the score is displayed*/
struct player {
    char place[3]; 
    int score;
    char name [4]; //3 letters
};

/*Hold the five players on the highscore list*/
struct player p[5];

/*A multidimensional char array (string array) that holds the information that's shown at the highscore list*/
char display_score[3][12]; 

/*Initialize the struct if it's not saved to the EEPROM*/
void struct_init (void){
    int i, j;
    char test[10];
    for ( i = 0; i < 5; i++)
    {
        p[i].score = 0; //Score is set to 0

        for ( j = 0; j < 3; j++)
            p[i].name[j]= '*';  
        p[i].name[3] = '\0';    //Null operator

        p[i].place[0]='1'+i;
        p[i].place[1] = '.';
        p[i].place[2]='\0';    
    } 
}

/*The function copies data from a struct and returns the copied struct*/
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

/*The function checks how many digits it is in an integer*/
int intlength (int score){
    int len = 1;
    while (score > 9) //Calculate the lengt of the integer
    {
        len ++;
        score /= 10;
    }
    return len;
}

/*The function first takes the length from the intlength function, if the score is above the limit och the char array, which is four digits, a messages of that the score is to high
will be shown and then you will return to the main menu. This score will be impossible to reach in the game. After that the score integer is converted into a string by extracting the 
least significant digit first and store it in the last position and so on.*/
void scoreToStr(int score, char* str){
    int len = intlength(score);
    int i, temp;

    if (len > 4){
        display_string(0, "Score to high.");
        display_string(1, "highest score is");
        display_string(2, "9999");
        display_update();
        wait_2();
        displayMenu();
    }

    for ( i = len-1; i >= 0; i--) //extract each digit of the integer and store it in the char array in the oposite order (extracts the lowest digit first).
    {
        temp = score % 10;
        str[i] = temp + '0';
        score /= 10;
    }
    str[len] = '\0';
}

/*To display the score on the screen it needs to be in a single string representation. This function merges each of the three elements in the struct and store it in a 
single string. Depending on the position in the highscore list, different structs are shown. */
void structToString (int pos)
{
    int i, j, k;
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

/*Takes the position as an argument and forwards it to structToString that stores the information in the global string array display_score. Depending on the position
different places in the highscore list will be stored in display_score.*/
void highscore_list (int pos){
    structToString(pos);

    display_string(0, "Highscore");
    display_string(1, display_score[0]);
    display_string(2, display_score[1]);
    display_string(3, display_score[2]);
    display_update();
}

/*If a player makes it to the highscore, the index where the player will be is calculated by another function and sent as an argument in this function. 
This functions frees up that space where the new highscore will be.*/
void sort_score (int index){
    int i;
    for ( i = 4; i > index; i--) 
    {
        p[i] = cpy_struct(p[i-1]);
    }
    for ( i = 0; i < 5; i++)
    {
        p[i].place[0]='1'+i;
        p[i].place[1] = '.';
        p[i].place[2]='\0';  
    }
 
}

/*First of all the position where the new player is going to write his/her name will be made free by the sort function. After that that position's name is replaced with blankspaces.
Then the new score is saved to that position. After that you're asked to write your name. The position of which letter to provid is held by i and which letter from the alphabet
char array you want to provide is held by j. When you've provided 3 letters the highscore will be viewed.*/
void write_highscore (int score, int index){
    sort_score(index); //Gives place for the new highscore
    int i=0, j=0, btns=0;

    for ( i = 0; i < 3; i++)
        p[index].name[i]= ' ';//remove name
    p[index].name[3]='\0';

    p[index].score = score;//Save the score

    i = 0;

    display_string(0, "Enter your name,");
    display_string(1, "use three letters:");
    display_string(2, p[index].name);
    display_update();

    while (i != 3)//insert three charachers
    {
        btns = getbtns();
        p[index].name[i]=alphabet[j];
        wait_0_1();
        display_update();
        display_string(2, p[index].name);
        if ((btns & BTN_SELECT))
        {
            display_update();
            i++;
            p[index].name[i]=alphabet[j];
            wait_0_1();
        }
        else if((btns & BTN_UP)){
            if(j < 25) {
                j++;
                display_update();
                wait_0_1();
                }
        }
        else if((btns & BTN_DOWN)){
            if(j > 0) {
                j--;
                display_update();
                wait_0_1();
                }
        }
    }  
p[index].name[3]='\0';
wait_1();
display_clear();
view_highscore();
}

/*This is a test program to test the highscore list without playing the game*/
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

/*When game over, this function is called. It checks if the score is high enough to be on the top 5 highscore list. Then you either get to input your name or you go direct to
the highscore list. */
void new_highscore (int score){
    int index = -1, i; //-1 as start value fro index
    char temp[5];

    for ( i = 4; i >= 0; i--)
    {
        if(score > p[i].score) //check if current score is higher than the previous
            index = i;
    }
        scoreToStr(score, temp);

    if (index != -1)
    {
        display_string(0, "Congratulations!");
        display_string(1, "You made it");
        display_string(2, "to the highscore!");
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

/*This is the menu for the highscore. Pos is the position that the player is in the highscore list.*/
void view_highscore (void){
    display_clear();
    int pos = 0;
    int btns = 0;

    highscore_list(pos);
    wait_1();
 while (!(btns & BTN_SELECT)) {//
         btns = getbtns();
         if ((btns & BTN_DOWN)) {
            wait_0_2();
             if (pos > 0) pos--;
                highscore_list(pos);

         } else if (btns & BTN_UP) {
            wait_0_2();
             if (pos < 2) pos++;
            highscore_list(pos);
        }
    }
    display_clear(); 
    start_menu();
}



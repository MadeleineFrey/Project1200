#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

/*By Erica Engborg and Madeleine Frey*/
int getsw(void){
    int sw = (PORTD >> 8) & 0x0F; //isolate bit 8-11

    return sw;
}

int getbtns(void){
    //binary 5, 6, 7
    int btn = (PORTD >> 5) & 0x07; //isolate it 5-7
    btn |= (PORTF & 0x2) << 2; //btn 1
    return btn;
}
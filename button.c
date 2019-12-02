#include "button.h"

int readRA5Button(void) {
    firstReadRA5 = secondReadRA5;
    secondReadRA5 = PORTAbits.RA5;
    int check = 0;
    if (firstReadRA5 == secondReadRA5) {
        if (firstReadRA5 == 0) {
            check = 1;
        }
    }
   return check; 
}

int readRB0Button (void) {
    firstReadRB0 = secondReadRB0;
    secondReadRB0 = PORTBbits.RB0;
    int check = 0;
    if (firstReadRB0 == secondReadRB0) {
        if (firstReadRB0 == 0) {
            check = 1;
        }
    }
    return check;
}

void button (void) {
    int checkRA5 = readRA5Button();
    int checkRB0 = readRB0Button();
    if (checkRB0 == 1) {
        RB0Pressed = 1;
        countPressedRB0++;
        if (countPressedRB0 >= 100) {
            skip_state = 1;
            countPressedRB0 = 0;
        }
    }
    else if (checkRA5 == 1) {
        countPressedRA5++;
        RA5Pressed = 1;
        if (countPressedRA5 >= 100) {
            custom_flag = 1;  
            countPressedRA5 = 0;
        }
    }
    else {
        countPressedRA5 = 0;
        countPressedRB0 = 0;
        countAuto = 0;
        RA5Pressed = 0;
        RB0Pressed = 0;
        increaseTime = 0;
    }
}
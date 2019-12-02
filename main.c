#include <xc.h>
#include <pic18f8722.h>
#include "interrupt.h"
#include "DHT.h"
#include "FSM.h"

void main(void) {
    OSCILLATOR_INIT();
    INTERRUPT_TIMER_INIT();
    BUTTON_INIT();
    pin_init();
    LCDInit();
    while (1)
    {  
        FSM1();
        FSM2();
        FSM3();
        //Print temperature and humidity afer 1s
        if (rePrintData >= 100){
            printData();
            rePrintData = 0;//reset time
        }
    }
    return;
} 
        
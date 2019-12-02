#include "interrupt.h"

void __interrupt () deviceInterrupt(void) {
    if (INTCONbits.TMR0IF == 1 && INTCONbits.TMR0IE == 1) {
        ISR();
    }
    else{
        
    }
}

void ISR(void){
        INTCONbits.TMR0IF = 0;
        TMR0H = 0xfd;
        TMR0L = 0x5f;
        count10ms++;
        rePrintData++;
        if (LTDState == UV)
            count_UV++;
        if (LTDState == LHEATER)
            count_heater++;
        if (LTDState == LHEAT_PUMP)
            count_heatpump++;
        if (TIMEOUT > 0) TIMEOUT--;
        button();
}
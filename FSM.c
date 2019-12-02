#include "system.h"
#include "button.h"
#include "interrupt.h"
#include "FSM.h"

void FSM1(void)
{
    if (PreLTDState != LTDState) TIMEOUT = TIMEOUT_ERROR;
    PreLTDState = LTDState;
     switch(LTDState){
         case BEGIN:    
             LATD = 1;
            LCD_Line(1, "FRESH?         ");
            if (TIMEOUT == 0) LTDState = LERROR;
            if (RA5Pressed){
                LTDState = UV;
            }
            if (RB0Pressed){
                CPNState = HEATER;
                LTDState = LHEATER;
            }
            
            break;
         case UV:
             LATD = 2;
             LCD_Line(1, "UV LIGHT: ON     ");
             if (TIMEOUT == 0) LTDState = LERROR;
             if (count_UV >= 1000){
                 count_UV = 0;
                 CPNState = HEATER;
                 LTDState = LHEATER;
             }
             break;
         case LHEATER:
             LATD = 3;
             ht = 1;
             hp = 0;
             f2 = 1;
             if (TIMEOUT == 0) LTDState = LERROR;
             if (realTemp >= MAX_TEMP || count_heater >= 1000){ 
                 if (realHumid <= MIN_HUMID) 
                     LTDState = IDLE;
                 else 
                     LTDState = LHEAT_PUMP;
                 count_heater = 0;
             }
             if (skip_state){
                 LTDState = IDLE;
                 skip_state = 0;
             }
             break;
         case LHEAT_PUMP:
             LATD = 4;
             ht = 0;
             hp = 1;
             f2 = 0;
             if (TIMEOUT == 0) LTDState = LERROR;
             if (realHumid <= MIN_HUMID || count_heatpump >= 1000) 
             { 
                 if (realTemp >= MAX_TEMP) 
                     LTDState = IDLE;
                 else 
                     LTDState = LHEATER;
                 count_heatpump = 0;
             }
             if (skip_state){
                 LTDState = IDLE;
                 skip_state = 0;
             }
             break;
         case IDLE:
             LATD = 5;
             ht = 0;
             hp = 0;
             f2 = 0;
             LCD_Line(1, "WORK DONE!        ");
             CPNState = CPN_IDLE;
             if (RA5Pressed) LTDState = BEGIN;
             if (skip_state){
                 LTDState = LERROR;
                 skip_state = 0;
             }
             break;
         case LERROR:
             LATD = 6;
             ht = 0;
             hp = 0;
             f2 = 0;
             LCD_Line(1, "ERROR!!!!!!        ");
             CPNState = CPN_IDLE;
             if (RA5Pressed) LTDState = BEGIN;
             break;
     }          
}
void FSM2(void)
{
   if (realHumid >= MAX_HUMID){
       if (realHumid < MEDIUM_SPEED_HUMID) FAN1State = LOW;
       else if (realHumid < MAX_SPEED_HUMID) FAN1State = MEDIUM;
       else FAN1State = HIGH;
   }   
   else FAN1State = OFF;
}
void FSM3(void)
{
    mCURSOR_LINE1;
    switch(CPNState){
        case CPN_IDLE:
            break;
        case HEATER:
            if (ht){
                LCDPutStr("HEATER: ON     ");
                curLCD = 0;
            }
            else{
                LCDPutStr("HEATER: OFF    "); 
                curLCD = 0;
            }
            if (RA5Pressed)
                CPNState = HEAT_PUMP;     
            break;
        case HEAT_PUMP:
            if (hp)  {
                LCDPutStr("HEAT_PUMP: ON ");
            }
            else {
                LCDPutStr("HEAT_PUMP: OFF");
            }
            if (RA5Pressed)
                CPNState = FAN1;
            break;
        case FAN1:
            if (FAN1State == OFF){  
                LCDPutStr("FAN1: OFF       ");
            }
            else if (FAN1State == LOW){
                LCDPutStr("FAN1: LOW       "); 
            }
            else if (FAN1State == MEDIUM){
                LCDPutStr("FAN1: MEDIUM    "); 
            }
            else {
                LCDPutStr("FAN1: HIGH      "); 
            }
            if (RA5Pressed)
                CPNState = FAN2;
            break;
        case FAN2:
            if (f2) { 
                LCDPutStr("FAN2: ON         ");
            }
            else{
                LCDPutStr("FAN2: OFF         "); 
            }
            if (RA5Pressed)
                CPNState = CUSTOM;
            break;
         case CUSTOM:
            CUSTOM_FSM();
            if (custom_flag){
                 CPNState = HEATER;
                 custom_flag = 0;
                 CUSTOMState = EXIT;
            }
            
            break;   
        default: 
            break;
    }  
    
}

void CUSTOM_FSM(void){
    switch(CUSTOMState){
        case CMAX_TEMP:
            LCD_Line(1,"MAX_TEMP: ");
            if (RA5Pressed) MAX_TEMP = (MAX_TEMP + 5)%100;
            LCDPutChar(MAX_TEMP/10 +'0');
            LCDPutChar(MAX_TEMP%10 +'0');
            if (RB0Pressed) CUSTOMState = CMIN_HUMI;
            break;
        case CMIN_HUMI:
            LCD_Line(1,"MIN_HUMI: ");
            if (RA5Pressed) MIN_HUMID = (MIN_HUMID + 5)%100;
            LCDPutChar(MIN_HUMID/10 +'0');
            LCDPutChar(MIN_HUMID%10 +'0');
            if (RB0Pressed) CUSTOMState = EXIT;
            break;
        case EXIT:
            LCD_Line(1,"EXIT?            ");
            if (RB0Pressed) CUSTOMState = CMAX_TEMP;
            break;
    }
}
void LCD_Line(int line, const char *str){
    
    if (line == 1){
            mCURSOR_LINE1;
    }
    else {
        mCURSOR_LINE2; 
    }
    LCDPutStr(str);
}

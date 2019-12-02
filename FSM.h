#ifndef FSM_H
#define	FSM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "system.h"
#include "button.h"
#include "interrupt.h"

int ht;
int hp;
int f1;
int f2;


//#define MAX_TEMP 80
//#define MAX_HUMID 40
int MIN_HUMID = 20;
#define MEDIUM_SPEED_HUMID 70
#define MAX_SPEED_HUMID 90
int MAX_TEMP  = 80;
#define MAX_HUMID 60

#define LED1 LATD0
#define LED2 LATD1
#define LED3 LATD2
#define LED4 LATD3
#define LED5 LATD4
#define LED6 LATD5
#define LED7 LATD6
#define LED8 LATD7

enum LTD{BEGIN, UV, LHEATER, LHEAT_PUMP, IDLE, LERROR} ;
enum FAN1_SPEED{OFF, LOW, MEDIUM, HIGH} ;
enum COMPONENT{CPN_IDLE, HEATER, HEAT_PUMP, FAN1, FAN2, CUSTOM};
enum CUSTOM{CMAX_TEMP, CMIN_HUMI, EXIT};
enum LTD LTDState = BEGIN;
enum LTD PreLTDState = LERROR;
enum FAN1_SPEED FAN1State = OFF;
enum COMPONENT CPNState = CPN_IDLE;
enum CUSTOM CUSTOMState = EXIT;
int curLCD = 0;
int PreLCD = -1; 
void FSM1(void);
void FSM2(void);
void FSM3(void);
void CUSTOM_FSM(void);
void LCD_Line(int line, const char *str);

unsigned char Cflag = 0;
#ifdef	__cplusplus
}
#endif

#endif	/* FSM_H */
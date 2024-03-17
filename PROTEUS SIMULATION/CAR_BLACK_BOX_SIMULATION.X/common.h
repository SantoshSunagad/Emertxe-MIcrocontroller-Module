
#ifndef COMMON_H
#define	COMMON_H

#define DASHBOARD 0
#define PASSWORD_SCREEN  1
#define MENU_SCREEN 2
#define VIEW_LOG 3
#define DOWNLOAD_LOG 4
#define CLEAR_LOG 5
#define SER_TIME 6
#define CHANGE_PASSWORD 7

/* Common Declaration */
unsigned int  SCREEN_FLAG = DASHBOARD, MENU_FLAG; 
unsigned char key;

 char gear[][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "G4", "G5", "C_", "CL", "ST", "CP"};
 static unsigned int speed=0, g_index = 0;
 /*Macro For Gears*/
#define ON 0
#define GR 1
#define GN 2
#define G1 3
#define G2 4
#define G3 5
#define G4 6
#define G5 7
#define C_ 8

//unsigned int wait = 0;
static int  chance = 3, count = 0, index = 0, once = 1;
unsigned int sec = 60, M=0, menu = 0, second=0;
unsigned char arr[4], pass[4] = {0,0,0,0} , ER=0;
unsigned long wait = 0;


/* Menu Function variables*/
char diplay[][20] = {"VIEW LOG      ", "DOWNLOAD LOG  ", "CLEAR LOG     ", "SET TIME      ", "CHANGE PASSWORD","->"};


#define L1 0
#define L2 1


/* ADC Declaration  */
#define CHANNEL4		0x04

/* CLCD Declaration*/
#define CLCD_PORT			PORTD
#define CLCD_EN				RC2
#define CLCD_RS				RC1
#define CLCD_RW				RC0
#define CLCD_BUSY			RD7
#define PORT_DIR			TRISD7


#define HI												1
#define LO												0

#define INPUT											0xFF
#define OUTPUT											0x00

#define DATA_COMMAND									1
#define INSTRUCTION_COMMAND								0

#define LINE1(x)									(0x80 + (x))
#define LINE2(x)										(0xC0 + (x))

#define TWO_LINE_5x8_MATRIX_8_BIT					clcd_write(0x38)
#define CLEAR_DISP_SCREEN				                clcd_write(0x01)
#define SHIFT_CURSOR_RIGHT			                  clcd_write(0x14)
#define CURSOR_HOME							clcd_write(0x02)
#define DISP_ON_AND_CURSOR_OFF						clcd_write(0x0C)

void clcd_write(unsigned char command);
void init_clcd(void);
void clcd_print(const unsigned char *str, unsigned char addr);
void clcd_putch(unsigned char data, unsigned char addr);

void init_timer0(void);

/* Fuction Prototypes */
void display_dashboard();
void password_screen();
void view_log();
void download_log();
void clear_log();
//void set_time();
void set_times();
void change_password();
void  display_menu();


#include "uart.h"


/* VIEW EVENT STORED */
void store_event_in_eeprom();
void over_write(unsigned int current_address);

unsigned char time[9]="11:54:44";

#endif	/* COMMON_H */


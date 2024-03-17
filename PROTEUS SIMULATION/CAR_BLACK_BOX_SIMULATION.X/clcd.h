#ifndef LCD_H
#define LCD_H

#define RS RC1
#define RW RC0
#define EN RC5


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
#define CURSOR_HOME							clcd_write(0x02)
#define DISP_ON_AND_CURSOR_OFF						clcd_write(0x0C)
#define EIGHT_BIT_MODE   0x33
#define SHIFT_CURSOR_RIGHT			                  clcd_write(0x14)
#define LCD_CURSOR_BLINK                             clcd_write(0x0F)// curson keeps blinking
#define LCD_CURSOR_LINE2                             clcd_write(0xC3)   // move curson to scond line or second row


void clcd_write(unsigned char command);
void init_clcd(void);
void clcd_print(const unsigned char *str, unsigned char addr);
void clcd_putch(unsigned char data, unsigned char addr);


#endif

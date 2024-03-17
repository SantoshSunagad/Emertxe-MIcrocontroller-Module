#include <xc.h>
#include "clcd.h"
#define _XTAL_FREQ 4000000
void init_clcd()
{
    TRISC = 0X00;
    TRISD = 0X00;
    
     //Write your initialization code here
    clcd_write(0x38); //enable 16x2
    clcd_write(0x06);
    clcd_write(0x0c); //curser on and off
    clcd_write(0x01); //clear screen
 
}

void clcd_write(unsigned char command) {
    PORTD = command;
    RS = 0;
    RW = 0;
    EN = 1;
    for (unsigned int i = 100; i--;);
    EN = 0;
}

void clcd_putch(unsigned char data, unsigned char addr) {
    clcd_write(addr);
    PORTD = data;
    RS = 1;
    RW = 0;
    EN = 1;
    for (unsigned int i = 100; i--;);
    EN = 0;
}

void clcd_print(const unsigned char *str, unsigned char addr) {
    clcd_write(addr); //
    unsigned char i;
    for (i = 0; str[i]!= '\0'; i++) {
        clcd_putch(str[i], addr++);
    }
}

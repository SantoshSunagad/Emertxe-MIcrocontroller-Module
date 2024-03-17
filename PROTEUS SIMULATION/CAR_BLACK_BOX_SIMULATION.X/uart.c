#include <xc.h>
#include "uart.h"

void uartinit(void) {
    TRISCbits.TRISC6 = 0; //config tx pin as output
    TRISCbits.TRISC7 = 1;
    TXSTA = 0b00100000;
    RCSTA = 0b10010000;
    SPBRG = 31;
//    INTCONbits.GIE = 1;
//    INTCONbits.PEIE = 1;
//    PIE1bits.RCIE = 1;
}

void uartsendchar(unsigned char ucdata) {
    TXIF = 0;
    TXREG = ucdata;
    while (TXIF == 0) {
        CLRWDT();
    }
}

void uartsendstr(char *str) {
    while (*str) {
        uartsendchar(*str++);
    }
}

//////////////////////////////////////////////////////////////////////////////////

void putch(unsigned char byte) 
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)
	{
		continue;
	}
	TXREG = byte;
}

int puts(const char *s)
{
	while(*s)		
	{
		putch(*s++);	
	}

	return 0;
}

unsigned char getch(void)
{
	/* Retrieve one byte */
	/* Set when register is not empty */
	while(!RCIF)
	{
		continue;
	}

	return RCREG;
}

unsigned char getche(void)
{
	unsigned char c;

	putch(c = getch());

	return (c);
}

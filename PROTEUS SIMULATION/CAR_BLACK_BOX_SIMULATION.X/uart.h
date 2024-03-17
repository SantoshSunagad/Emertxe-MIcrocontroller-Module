

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif
void uartinit();
void uartISR(void);
void uartsendchar(unsigned char ucdata);
void uartsendstr(char *str);
void delayISR(void);
////////////////////////////////////////////////////////////////////////////////
void putch(unsigned char byte);
int puts(const char *s);
unsigned char getch(void);
unsigned char getch_with_timeout(unsigned short max_time);
unsigned char getche(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */


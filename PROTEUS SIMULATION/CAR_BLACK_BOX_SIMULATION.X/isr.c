#include <xc.h>
#include "common.h"
extern unsigned int sec, SCREEN_FLAG, MENU_FLAG, second, blink_f;
void __interrupt() isr(void)
{
	static unsigned short count;
     
	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;

		if (count++ == 20000)
		{
			count = 0;
            if(SCREEN_FLAG == PASSWORD_SCREEN || MENU_FLAG == SER_TIME)
            {
                --sec;
                //second++;
                blink_f=!blink_f;
            }
            
		}
		TMR0IF = 0;
	}
}
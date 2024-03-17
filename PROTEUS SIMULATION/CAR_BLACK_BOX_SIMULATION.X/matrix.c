#include <xc.h>
#include "matrix_keypad.h"

#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2

#define R1 PORTBbits.RB4
#define R2 PORTBbits.RB5
#define R3 PORTBbits.RB6
#define R4 PORTBbits.RB7
void init_matrix_config()
{
    TRISB = 0xF0;
}

unsigned char scan_key()
{
    C1=1;C2=0;C3=0;
    if(R1 == 1)
    {
        return 1;
    }
    if(R2 == 1)
    {
        return 4;
        
    }
    if(R3 == 1)
    {
        return 7;
       
    }
    if(R4 == 1)
    {
        return 10;
        
    }
    //
     C1=0;C2=1;C3=0;
    if(R1 == 1)
    {
        return 2;
       
    }
    if(R2 == 1)
    {
        return 5;
       
    }
    if(R3 == 1)
    {
        return 8;
        
    }
    if(R4 == 1)
    {
        return 11;
        
    }
     //
       C1=0;C2=0;C3=1;
    if(R1 == 1)
    {
        return 3;
        
    }
    if(R2 == 1)
    {
        return 6;
        
    }
    if(R3 == 1)
    {
        return 9;
   
    }
    if(R4 == 1)
    {
        return 12;
      
    }
       return 0xFF;
}

unsigned char read_switches(unsigned char detection_type)
{
	static unsigned char once = 1, key;

	if (detection_type == STATE_CHANGE)
	{
		key = scan_key();
		if(key != 0xFF && once  )
		{
			once = 0;
			return key;
		}
		else if(key == 0xFF)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL_CHANGE)
	{
		return scan_key();
	}

	return 0xFF;
}
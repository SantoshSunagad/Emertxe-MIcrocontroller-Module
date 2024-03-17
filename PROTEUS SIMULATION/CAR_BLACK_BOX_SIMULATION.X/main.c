#include <xc.h>
#include "matrix_keypad.h"
#include "clcd.h"
#include "adc.h"
#include "common.h"
#include "isr.h"
#include "timer0.h"
#include "eeprom.h"

// PIC18F4580 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 0         // Brown-out Reset Voltage bits (VBOR set to 4.6V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bit (1K words (2K bytes) boot block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2

#define R1 PORTBbits.RB4
#define R2 PORTBbits.RB5
#define R3 PORTBbits.RB6
#define R4 PORTBbits.RB7


void init_config(void) {
    init_matrix_config();
    init_clcd();
    init_adc();
    init_timer0();
    GIE = 1;
    PEIE = 1;
    
    /* FOR One time Write EEPROM */
    write_internal_eeprom(0x64, 0);
    write_internal_eeprom(0x65, 0);
    write_internal_eeprom(0x66, 0);
    write_internal_eeprom(0x67, 0);
    
}
 unsigned char flag=1;
void main(void) {
    CLEAR_DISP_SCREEN;
    init_config();
    store_event_in_eeprom();
    while (1) {
       
        
      key = read_switches(STATE_CHANGE);
      
        if (key == MK_SW5 && flag ) {
            SCREEN_FLAG = PASSWORD_SCREEN;
            flag=0;
            MENU_FLAG = 0;
        } 
        
        if (SCREEN_FLAG == DASHBOARD) 
        {
            display_dashboard();
        } else if (SCREEN_FLAG == PASSWORD_SCREEN) 
        {
            password_screen();
        } else if (SCREEN_FLAG == MENU_SCREEN) 
        {
            if (MENU_FLAG == VIEW_LOG) {
                view_log();
            } else if (MENU_FLAG == DOWNLOAD_LOG) 
            {
                download_log();
            } else if (MENU_FLAG == CLEAR_LOG) 
            {
                clear_log();
            } else if (MENU_FLAG == SER_TIME) 
            {
                //key=read_switches(LEVEL_CHANGE);
               // set_time();
                set_times();
            } else if (MENU_FLAG == CHANGE_PASSWORD) 
            {
                change_password();
            } else
                display_menu();
        }

    }
}









//
//
//init_config();
//    unsigned char key, speed;
//    unsigned int ger_index = -1;
//    while (1) {
//        /* Speed controll */
//        speed = (read_adc(CHANNEL4) / 10.23);
//        if (speed > 99) {
//            speed = 99;
//        }
//        if (ger_index == 2 && (0 >= speed && speed <= 30)) {
//            clcd_putch((speed / 10) + '0', LINE2(14));
//            clcd_putch((speed % 10) + '0', LINE2(15));
//        }
//
//        /* Gear system */
//        key = read_switches(STATE_CHANGE);
//        if (key == MK_SW1) {
//            clcd_print("C_", LINE2(10));
//        } else if (key == MK_SW2) {
//            ger_index++;
//            clcd_print(gear[ger_index], LINE2(10));
//        } else if (key == MK_SW3) {
//            ger_index--;
//            clcd_print(gear[ger_index], LINE2(10));
//        }
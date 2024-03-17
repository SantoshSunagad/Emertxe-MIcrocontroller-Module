#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "common.h"
#include "matrix_keypad.h"
#include "eeprom.h"

unsigned int password = 0, dummy[4];
extern unsigned int MENU_FLAG;

// char gear[][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "G4", "G5", "C_"};
extern unsigned char key, flag;
extern unsigned int MENU_FLAG, SCREEN_FLAG, speed, second;
unsigned char ee = 0, EF = 0, EI = 0;

void display_dashboard() {
    clcd_print(" TIME     EV  SP", LINE1(0));
    clcd_print(time, LINE2(0));
    speed = (read_adc(CHANNEL4) / 10.23);

    /* Speed Controll using Gears*/
    if (g_index == GR && speed >= 20) {
        speed = 20;
    } else if (g_index == GN || g_index == ON || g_index == C_) {
        speed = 0;
    } else if (g_index == G1 && speed >= 30) {
        speed = 30;
    } else if (g_index == G2 && speed >= 40) {
        speed = 40;
    } else if (g_index == G3 && speed >= 60) {
        speed = 60;
    } else if (g_index == G4 && speed >= 80) {
        speed = 80;
    } else if (g_index == G5 && speed >= 99) {
        speed = 99;
    }
    if (ee) {
        ee = 0;
        store_event_in_eeprom();
    }
    /* Gear system */
    if (key == MK_SW1) {
        g_index = C_;
        speed = 0;
        ee = 1;
        EF = 0;
    } else if (key == MK_SW2 && g_index <= 6 || key == MK_SW2 && g_index == C_) {

        (g_index == C_) ? (g_index = GN) : (g_index++);
        ee = 1;
        EF = 0;
    } else if (key == MK_SW3 && g_index > 1 || key == MK_SW3 && g_index == C_) {

        (g_index == C_) ? (g_index = GN) : (g_index--);
        ee = 1;
        EF = 0;
    }
    if (!EF) {
        clcd_print(gear[g_index], LINE2(10));
        clcd_putch((speed / 10) + '0', LINE2(14));
        clcd_putch((speed % 10) + '0', LINE2(15));
    } else {
        clcd_print(gear[EI], LINE2(10));
        clcd_putch('0', LINE2(14));
        clcd_putch('0', LINE2(15));
    }


}

/* FUNCTION DEFFINITION  PASSWORD SCREEN */
void password_screen() {

    if (chance > 0) {
        if (index < 4) {
            if (once) {
                once = 0;
                CLEAR_DISP_SCREEN;
                clcd_print(" ENTER PASSWORD ", LINE1(0));
                LCD_CURSOR_LINE2;
                LCD_CURSOR_BLINK;
                /* Read From EEPROM */
                pass[0] = read_internal_eeprom(0x64);
                pass[1] = read_internal_eeprom(0x65);
                pass[2] = read_internal_eeprom(0x66);
                pass[3] = read_internal_eeprom(0x67);
                sec = 60;
                key = 0;
            }

            if (key == MK_SW5) {
                arr[index] = 0;
                clcd_putch('*', LINE2(index + 3));
                if (arr[index] == pass[index]) {
                    ++count;
                }
                ++index;
                sec = 60;
            } else if (key == MK_SW6) {
                arr[index] = 1;
                clcd_putch('*', LINE2(index + 3));
                if (arr[index] == pass[index]) {
                    ++count;
                }
                ++index;
                sec = 60;
            } else {
                /* wait for 5 sec dealy*/
                if (sec < 54) {
                    DISP_ON_AND_CURSOR_OFF;
                    CLEAR_DISP_SCREEN;
                    SCREEN_FLAG = DASHBOARD;
                    index = 0;
                    count = 0;
                    sec = 60;
                    flag = 1;
                    chance = 3;
                    once = 1;
                }
            }
        } else {
            /* if PASSWORD Match or Not*/
            if (index == count) {
                DISP_ON_AND_CURSOR_OFF;
                CLEAR_DISP_SCREEN;
                clcd_print("   WEL-COME TO ", LINE1(0));
                clcd_print("   MENU SCREEN", LINE2(0));
                for (unsigned long y = 0xFFFFA; --y;);
                CLEAR_DISP_SCREEN;
                flag = 0;
                index = 0;
                count = 0;
                sec = 60;
                chance = 3;
                once = 1;
                /* Update To SCREEN FLAG TO MENU SCREEN */
                SCREEN_FLAG = MENU_SCREEN;
            } else {
                chance--;
                DISP_ON_AND_CURSOR_OFF;
                CLEAR_DISP_SCREEN;
                clcd_print(" WRONG PASSWORD ", LINE1(0));
                clcd_print(" ATTEMPT ", LINE2(0));
                clcd_putch('0' + chance, LINE2(10));
                for (unsigned long y = 0xFFFFA; --y;);
                CLEAR_DISP_SCREEN;
                index = 0;
                count = 0;
                sec = 60;
                once = 1;
            }
        }
    } else {
        DISP_ON_AND_CURSOR_OFF;
        clcd_print("  USER BLOCKED  ", LINE1(0));
        clcd_print(" WAIT FOR   SEC", LINE2(0));
        clcd_putch('0' + sec / 10, LINE2(10));
        clcd_putch('0' + sec % 10, LINE2(11));
        if (!sec) {
            index = 0;
            count = 0;
            sec = 60;
            chance = 3;
            once = 1;
        }
    }
}
unsigned char print[12], NEW_KEY;
static unsigned int f = 0x00;
unsigned int pk = 0, Event = 0;
unsigned long delay = 0;
static unsigned int no_of_event = 0, store_add = 0x00;

void view_log() {
    clcd_print("#   TIME   EV SP", LINE1(0));
    key = read_switches(LEVEL_CHANGE);
    if (key == MK_SW5 || key == MK_SW6) {
        delay++;
        NEW_KEY = key;
    }
    if (delay < 500 && key == ALL_RELEASED) {
        if (NEW_KEY == MK_SW5 && Event <= no_of_event - 1) {

            if (Event == no_of_event - 1) {
                Event = 0;
                f = 0x00;
            } else {
                ++Event;
                f = f + 11;
            }

        } else if (NEW_KEY == MK_SW6 && Event >= 0) {
            if (Event == 0) {
                Event = no_of_event - 1;
                f = 0x0B * (no_of_event - 1);
            } else {
                --Event;
                f = f - 11;
            }
        }
        delay = 0;
        NEW_KEY = 0;
    }
    if (no_of_event > 0) {
        print[0] = read_internal_eeprom(0x00 + f);
        print[1] = read_internal_eeprom(0x01 + f);
        print[2] = read_internal_eeprom(0x02 + f);
        print[3] = read_internal_eeprom(0x03 + f);
        print[4] = read_internal_eeprom(0x04 + f);
        print[5] = read_internal_eeprom(0x05 + f);
        print[6] = read_internal_eeprom(0x06 + f);
        print[7] = read_internal_eeprom(0x07 + f);
        print[8] = ' ';
        print[9] = read_internal_eeprom(0x08 + f);
        print[10] = read_internal_eeprom(0X09 + f);
        print[11] = '\0';
        pk = read_internal_eeprom(0x0A + f);

        clcd_putch(Event + '0', LINE2(0));
        clcd_print(print, LINE2(2));
        clcd_putch((pk / 10) + '0', LINE2(14));
        clcd_putch((pk % 10) + '0', LINE2(15));
    } else {
        clcd_print("EMPTY", LINE2(3));
    }
    if (delay >= 500) {
        CLEAR_DISP_SCREEN;
        SCREEN_FLAG = DASHBOARD;
        delay = 0;
        flag = 1;
        NEW_KEY = 0;
        MENU_FLAG = 0;
    }
}
unsigned char ch;
unsigned int address;
void download_log() {
    uartinit();
    address = 0x00;
    clcd_print("DOWNLOADING...", LINE1(0));
    for (unsigned long y = 0xFFFFA; --y;);
    puts("#  TIME EV SP\n\r");
    puts("\n\r");
    if (no_of_event > 0) {
        for (int i = 0; i < no_of_event; i++) {
            print[0] = read_internal_eeprom(0x00 + address);
            print[1] = read_internal_eeprom(0x01 + address);
            print[2] = read_internal_eeprom(0x02 + address);
            print[3] = read_internal_eeprom(0x03 + address);
            print[4] = read_internal_eeprom(0x04 + address);
            print[5] = read_internal_eeprom(0x05 + address);
            print[6] = read_internal_eeprom(0x06 + address);
            print[7] = read_internal_eeprom(0x07 + address);
            print[8] = ' ';
            print[9] = read_internal_eeprom(0x08 + address);
            print[10] = read_internal_eeprom(0X09 + address);
            print[11] = '\0';
            pk = read_internal_eeprom(0x0A + address);

            putch(i + '0');
            puts("  ");
            puts(print);
            putch(' ');
            putch((pk / 10) + '0');
            putch((pk % 10) + '0');
            puts("\n\r");
            address = address + 11;
        }
        clcd_print("DOWNLOAD COMPLETED", LINE1(0));
        for (unsigned long y = 0xFFFFA; --y;);
    } else {
        puts("DOWNLOAD LOG IS EMPTY\n\r");
    }
    SCREEN_FLAG = DASHBOARD;
    CLEAR_DISP_SCREEN;
    flag = 1;
    NEW_KEY = 0;
    MENU_FLAG = 0;
    address = 0x00;


}

void clear_log() {
    no_of_event = 0;
    MENU_FLAG = 0;
    clcd_print("CLAER LOG", LINE1(3));
    clcd_print("SUCCESSFULLY", LINE2(3));
    for (unsigned long int y = 0xFFFFA; --y;);
    SCREEN_FLAG = DASHBOARD;
    EI = 9;
    EF = 1;
    flag = 1;
    CLEAR_DISP_SCREEN;
}






unsigned int min = 0, hour = 0;
unsigned short int BLINK, FILED = 0, blink_f = 0, main_key;
unsigned char change_Time[9] = "00:00:00", clock_set[4];

void set_times() {
    key = read_switches(LEVEL_CHANGE);
    if (key == MK_SW5 || key == MK_SW6) {
        delay++;
        NEW_KEY = key;
    }
    if (delay < 500 && key == ALL_RELEASED) {
        if (NEW_KEY == MK_SW6) {
            if (FILED == 1) {
                ++second;
                if (second == 60) {
                    second = 0;
                }
                change_Time[6] = second / 10 + '0';
                change_Time[7] = second % 10 + '0';
            }
            if (FILED == 2) {
                ++min;
                if (min == 60) {
                    min = 0;
                }
                change_Time[3] = min / 10 + '0';
                change_Time[4] = min % 10 + '0';
            }
            if (FILED == 3) {
                ++hour;
                if (hour == 13) {
                    hour = 0;
                }
                change_Time[0] = hour / 10 + '0';
                change_Time[1] = hour % 10 + '0';
            }
        }
        if (NEW_KEY == MK_SW5 && FILED < 3) {
            FILED++;
        } else if (NEW_KEY == MK_SW5 && FILED == 3) {
            FILED = 1;
        }
        NEW_KEY = 0;
        delay = 0;
    }
    if (blink_f) {
        wait = 0;
        BLINK = FILED;
    } else {
        BLINK = 0;
    }
    if (BLINK == 1) {
        clcd_print("  ", LINE2(10));
    } else if (BLINK == 2) {
        clcd_print("  ", LINE2(7));
    } else if (BLINK == 3) {
        clcd_print("  ", LINE2(4));
    } else {
        clcd_print(change_Time, LINE2(4));
    }
    clcd_print("HH:MM:SS", LINE1(4));
    if (delay > 500) {
        if (NEW_KEY == MK_SW6) {
            CLEAR_DISP_SCREEN;
            MENU_FLAG = 0;
            NEW_KEY = 0;
            BLINK = 0;
            FILED = 0;
            flag = 1;
            delay = 0;
            hour = 0;
            min = 0;
            second = 0;
        }
        if (NEW_KEY == MK_SW5) {
            //            	clock_set[0] = read_ds1307(HOUR_ADDR);
            //	            clock_set[1] = read_ds1307(MIN_ADDR);
            //	            clock_set[2] = read_ds1307(SEC_ADDR);
            //            write_ds1307(HOUR_ADDR, ((clock_set[0] & 0x00) | ((HH / 10) << 4) | (HH % 10)));
            //            write_ds1307(MIN_ADDR, (((clock_set[1]) & 0x00) | (((MM /10)<< 4) | (MM % 10))));
            //            write_ds1307(SEC_ADDR, ((clock_set[2] & 0x00) | ((Second / 10)<<4) | (Second % 10)));
            CLEAR_DISP_SCREEN;
            clcd_print("TIME  CHANGED", LINE1(2));
            clcd_print("SUCCESSFULLY", LINE2(2));
            for (unsigned long int y = 0xFFFFA; --y;);
            CLEAR_DISP_SCREEN;
            SCREEN_FLAG = DASHBOARD;
            EI = 10;
            EF = 1;
            MENU_FLAG = 0;
            NEW_KEY = 0;
            BLINK = 0;
            FILED = 0;
            flag = 1;
            delay = 0;
            hour = 0;
            min = 0;
            second = 0;
        }

    }
}





//
//void set_time() {
//    clcd_print("HH:MM:SS", LINE1(6));
//    if (second == 59) {
//        second = 0;
//        min++;
//    }
//    if (min == 59) {
//        min = 0;
//        hour++;
//    }
//    key = read_switches(LEVEL_CHANGE);
//    if (key == MK_SW5 || key == MK_SW6) {
//        delay++;
//        NEW_KEY = key;
//    }
//    if (delay < 100 && key == ALL_RELEASED) {
//        if (NEW_KEY == MK_SW5) {
//            if (field_flag < 3)
//                field_flag++;
//            else
//                field_flag = 0;
//            if (NEW_KEY == MK_SW6) {
//                if (field_flag == 1) {
//                    second++;
//                } else if (field_flag == 2) {
//                    min++;
//                } else if (field_flag == 3) {
//                    hour++;
//                }
//            }
//        }
//        delay = 0;
//        NEW_KEY = 0;
//    }
//    blink_field = !blink_field;
//    if (field_flag == 0) {
//        if (blink_field) {
//            clcd_putch(second / 10 + '0', LINE2(12));
//            clcd_putch(second % 10 + '0', LINE2(13));
//        } else {
//            clcd_putch(' ', LINE2(12));
//            clcd_putch(' ', LINE2(13));
//        }
//    } else if (field_flag == 1) {
//        if (blink_field) {
//            clcd_putch(min / 10 + '0', LINE2(12));
//            clcd_putch(min % 10 + '0', LINE2(13));
//        } else {
//            clcd_putch(' ', LINE2(12));
//            clcd_putch(' ', LINE2(13));
//        }
//    } else if (field_flag == 2) {
//        if (blink_field) {
//            clcd_putch(hour / 10 + '0', LINE2(12));
//            clcd_putch(hour % 10 + '0', LINE2(13));
//        } else {
//            clcd_putch(' ', LINE2(12));
//            clcd_putch(' ', LINE2(13));
//        }
//    }
//    clcd_putch(hour / 10 + '0', LINE2(6));
//    clcd_putch(hour % 10 + '0', LINE2(7));
//    clcd_putch(':', LINE2(8));
//    clcd_putch(min / 10 + '0', LINE2(9));
//    clcd_putch(min % 10 + '0', LINE2(10));
//    clcd_putch(':', LINE2(11));
//    clcd_putch(second / 10 + '0', LINE2(12));
//    clcd_putch(second % 10 + '0', LINE2(13));
//}

void change_password() {
    if (index < 4) {
        if (once) {
            once = 0;
            CLEAR_DISP_SCREEN;
            clcd_print(" ENTER NEW PSW ", LINE1(0));
            LCD_CURSOR_LINE2;
            LCD_CURSOR_BLINK;
            key = 0;
        }

        if (key == MK_SW5) {
            arr[index] = 0;
            clcd_putch('*', LINE2(index + 3));
            if (password) {
                if (arr[index] == dummy[index]) {
                    ++count;
                }
            } else {
                dummy[index] = arr[index];
            }
            ++index;
        } else if (key == MK_SW6) {
            arr[index] = 1;
            clcd_putch('*', LINE2(index + 3));
            if (password) {
                if (arr[index] == dummy[index]) {
                    ++count;
                }
            } else {
                dummy[index] = arr[index];
            }
            ++index;
        }
    } else {
        /* if PASSWORD Match or Not*/
        if (index == 4 && !password) {
            CLEAR_DISP_SCREEN;
            clcd_print(" ENTER CNF PSW ", LINE1(0));
            LCD_CURSOR_LINE2;
            LCD_CURSOR_BLINK;
            index = 0;
            count = 0;
            password = 1;
        } else if (index == 4 && index == count) {
            DISP_ON_AND_CURSOR_OFF;
            CLEAR_DISP_SCREEN;
            clcd_print(" PSW CHANGED", LINE1(3));
            clcd_print("SUCCESS...", LINE2(5));
            for (unsigned long y = 0xFFFFA; --y;);
            CLEAR_DISP_SCREEN;
            index = 0;
            count = 0;
            once = 1;
            flag = 1;
            password = 0;
            write_internal_eeprom(0x64, arr[0]);
            write_internal_eeprom(0x65, arr[1]);
            write_internal_eeprom(0x66, arr[2]);
            write_internal_eeprom(0x67, arr[3]);
            SCREEN_FLAG = DASHBOARD;
            EI = 11;
            EF = 1;
        } else {
            DISP_ON_AND_CURSOR_OFF;
            CLEAR_DISP_SCREEN;
            clcd_print(" TRY AGAIN...", LINE1(0));
            for (unsigned long y = 0xFFFFA; --y;);
            CLEAR_DISP_SCREEN;
            index = 0;
            count = 0;
            once = 1;
            password = 0;
        }
    }
}

unsigned int M_index = 0, ARROW = L1;

void display_menu() {
    key = read_switches(LEVEL_CHANGE);
    if (key == MK_SW5 || key == MK_SW6) {
        delay++;
        NEW_KEY = key;
    }
    if (delay < 100 && key == ALL_RELEASED) {
        if (M_index < 3 && NEW_KEY == MK_SW5) {
            (ARROW) ? (M_index++) : (ARROW = L2);
        }
        if (M_index > 0 && NEW_KEY == MK_SW6) {
            (!ARROW) ? (M_index--) : (ARROW = L1);
        } else if (NEW_KEY == MK_SW5 && !ARROW) {
            ARROW = L2;
        } else if (NEW_KEY == MK_SW6 && ARROW) {
            ARROW = L1;
        }
        delay = 0;
        NEW_KEY = 0;
    }

    if (ARROW == L1) {
        clcd_print("->", LINE1(0));
        clcd_print("  ", LINE2(0));
    } else {
        clcd_print("  ", LINE1(0));
        clcd_print("->", LINE2(0));
    }
    clcd_print(diplay[M_index], LINE1(3));
    clcd_print(diplay[M_index + 1], LINE2(3));

    /* LONG Press Operation */
    if (delay >= 100) {
        delay = 0;
        if (NEW_KEY == MK_SW6) {
            CLEAR_DISP_SCREEN;
            SCREEN_FLAG = DASHBOARD;
            delay = 0;
            flag = 1;
            M_index = 0;
            ARROW = L1;
            NEW_KEY = 0;
        } else if (NEW_KEY == MK_SW5) {
            CLEAR_DISP_SCREEN;
            MENU_FLAG = ARROW ? (M_index + 4) : (M_index + 3);
            delay = 0;
            M_index = 0;
            ARROW = L1;
            NEW_KEY = 0;
        }
    }
}

/* OVER WRITE FUNCTION */
static int add = 0x00;

void over_write(unsigned int i) {
    unsigned char ch;

    for (int j = 0; j < 11; j++) {
        ch = read_internal_eeprom(i);
        write_internal_eeprom(add, ch);
        add++;
        i++;
    }
}

/* STORE EEPROM FUCTION DEFFINITION */
void store_event_in_eeprom(void) {

    if (++no_of_event > 9) {
        /* OVER WRITE  */
        /* 0x0B = 11   0X62 = 98 */
        for (unsigned int i = 0x0B; i < 0x62; i += 0x0B) {
            /* Function call  */
            over_write(i);
        }
        /* 0x58 = 88 and make 'add' as zero */
        store_add = 0x58;
        add = 0x00;
        no_of_event = no_of_event - 1;
    }

    /* TIME STORING */
    for (unsigned int i = 0; i < 8; i++) {
        write_internal_eeprom(store_add, time[i]);
        ++store_add;
    }

    /* EVENT STORING */
    for (unsigned int i = 0; i < 2; i++) {
        write_internal_eeprom(store_add, gear[g_index][i]);
        ++store_add;
    }

    /* SPEED STORING */
    write_internal_eeprom(store_add, speed);
    ++store_add;
}
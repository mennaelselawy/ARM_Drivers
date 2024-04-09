/*
 * LCD_Private.h
 *
 *  Created on: Apr 2, 2024
 *      Author: Menna Ashraf
 */

#define       LCD_FOUR_BIT_MODE       0      // bit 5 is 0 and bit 6 is 1
#define       LCD_EIGHT_BIT_MODE      0x10   // bit 5 and bit 6 are 1 , the difference is in bit '5'

#define       LCD_ONE_LINE            0
#define       LCD_TWO_LINE            1

#define       LCD_5x7                 0
#define       LCD_5x10                1

#define       LCD_DISPLAY_OFF         0
#define       LCD_DISPLAY_ON          1

#define       LCD_CURSOR_DISABLE      0
#define       LCD_CURSOR_ENABLE       1

#define       LCD_BLINK_CURSOR_OFF    0
#define       LCD_BLINK_CURSOR_ON     1

#define       LCD_ROW_1               0
#define       LCD_ROW_2               1

#define       LCD_DECREMENT           0
#define       LCD_INCREMENT           1

#define       LCD_SHIFTING_DISABLE    0

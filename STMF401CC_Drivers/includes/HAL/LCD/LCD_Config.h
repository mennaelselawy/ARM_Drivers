/*
 * LCD_Config.h
 *
 *  Created on: Apr 2, 2024
 *      Author: Menna Ashraf
 */

#include "../../LIB/STD_TYPES.h"

/**
 * @brief Periodicity of the LCD Update task in milli seconds
 *
 */
#define LCD_UPDATE_PERIODICITY_MS       2


/*
 * choose number of data pins:
 * EIGHT_PINS_DATA for 8 bit mode
 * FOUR_PINS_DATA for 4 bit mode
 */
#define LCD_DATA_PINS   EIGHT_PINS_DATA

/*
choose the number of lines you want to write on for LCD:
1- LCD_ONE_LINE
2- LCD_TWO_LINE
*/
#define   LCD_NUMBER_OF_LINES     LCD_TWO_LINE

/*
choose the font size for LCD:
1- LCD_5x7
2- LCD_5x10
*/
#define   LCD_FONT_SIZE     LCD_5x7

/*
choose to display on/off control:
1- LCD_DISPLAY_OFF
2- LCD_DISPLAY_ON
*/
#define   LCD_Display_Mode   LCD_DISPLAY_ON

/*
choose cursor mode:
1- LCD_CURSOR_DISABLE
2- LCD_CURSOR_ENABLE
*/
#define  LCD_CURSOR_MODE    LCD_CURSOR_ENABLE

/*
choose cursor blinking mode:
1- LCD_BLINK_CURSOR_OFF
2- LCD_BLINK_CURSOR_ON
*/
#define  LCD_BLINK_MODE     LCD_BLINK_CURSOR_OFF

/* Increment and Decrement the Register Counter after write operation                       */
/* Options:                                                                                 */
/*      - LCD_DECREMENT    =>  The Pointer counter decrements (Moves from right to left)    */
/*      - LCD_INCREMENT    =>  The Pointer counter increments (Moves from left to right)    */
#define LCD_INCREMENT_DECREMENT    LCD_INCREMENT


/* Shifts the entire display when enabled, shifts to the Left when Pointer Counter Decrements   */
/* and to the right when it Increments                                                          */
#define LCD_SHIFTING               LCD_SHIFTING_DISABLE


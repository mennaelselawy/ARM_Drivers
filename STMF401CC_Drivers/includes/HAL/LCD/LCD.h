/*
 * LCD.h
 *
 *  Created on: Apr 2, 2024
 *      Author: Menna Ashraf
 */

#include "../../LIB/STD_TYPES.h"
//#include ""

#define EIGHT_PINS_DATA       8
#define FOUR_PINS_DATA        4

typedef struct
{
	void * PORT;
	u8 PIN;
}LCD_PinConfig_t;

typedef struct
{
	LCD_PinConfig_t DataPins[LCD_DATA_PINS];
	LCD_PinConfig_t R_S_Pin;
	LCD_PinConfig_t R_W_Pin;
	LCD_PinConfig_t E_Pin;
}LCD_Pins_Configurations_t;

typedef void (*CallBack_t)(void);

COM_ErrorStatus_t LCD_Init();
COM_ErrorStatus_t LCD_WriteStringAsynchronous(const u8 * String, u8 Length, CallBack_t CB);
COM_ErrorStatus_t LCD_SetCurosrPositionAsynchronous(u8 Position_ROW, u8 Position_COLUMN, CallBack_t CB);
COM_ErrorStatus_t LCD_ClearScreenAsynchronous(CallBack_t CB);

COM_ErrorStatus_t LCD_GetState();

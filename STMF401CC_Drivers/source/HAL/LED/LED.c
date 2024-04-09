/*
 * LED.c
 *
 *  Created on: Feb 26, 2024
 *      Author: Menna Ashraf
 */
#include "../../include/LIB/STD_TYPES.h"
#include "../../../include/MCAL/GPIO/GPIO.h"
#include "../../../include/HAL/LED/LED.h"

extern const LED_Config_t LEDS[LED_Number];

COM_ErrorStatus_t LED_Init(void)
{
	COM_ErrorStatus_t RET_LedErrorStatus =LED_OK;
	u32 Index;
	GPIO_strConfig_t Led;
	Led.MODE = GPIO_MODE_OUT_PP;
	Led.SPEED = GPIO_SPEED_HIGH;
	for(Index =0; Index < LED_Number; Index++)
	{
		Led.PIN = LEDS[Index].PIN;
		Led.PORT = LEDS[Index].PORT;
		GPIO_Init(& Led);
	}
	return RET_LedErrorStatus;
}
COM_ErrorStatus_t LED_SetState(u32 LED_Name, u8 LED_State)
{
	COM_ErrorStatus_t RET_LedErrorStatus =LED_NOK;
	if( (LED_Name < LED_Alarm || LED_Name > LED_Number)
		|| (LED_State < LED_STATE_OFF || LED_State > LED_STATE_ON) )
	{
		RET_LedErrorStatus = Error_InvalidInput;
	}
	else
	{
		GPIO_SetPinValue(LEDS[LED_Name].PORT, LEDS[LED_Name].PIN, LED_State ^ LEDS[LED_Name].Connection);
		RET_LedErrorStatus = LED_OK;
	}
	return RET_LedErrorStatus;
}



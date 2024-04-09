/*
 * LED_CONFIG.c
 *
 *  Created on: Feb 26, 2024
 *      Author: Menna Ashraf
 */
#include "../../../include/HAL/LED/LED.h"
//#include "GPIO.h"

const LED_Config_t LEDS[LED_Number] =
{
		[LED_Alarm]={
				.PORT = LED_PORTA,
				.PIN = PIN_0,
				.Connection = LED_CONNECTION_FORWARD,
				.InitSTATE = LED_STATE_ON,
		},
		[LED_Power]={
				.PORT = LED_PORTA,
				.PIN = PIN_1,
				.Connection = LED_CONNECTION_REVERSE,
				.InitSTATE = LED_STATE_ON,
		},
};



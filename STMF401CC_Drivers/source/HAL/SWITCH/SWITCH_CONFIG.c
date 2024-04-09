/*
 * SWITCH_CONFIG.c
 *
 *  Created on: Feb 26, 2024
 *      Author: Menna Ashraf
 */
#include"../../../include/HAL/SWITCH/SWITCH.h"

const SWITCH_Config_t Switches[Switch_Number] =
{
		[Switch_Alarm] = {
				.Port = GPIO_PORTB,
				.Pin = PIN_0,
				.Connection = SWITCH_CONNECTION_PULLUP
		},
		[Switch_Power] =
		{
				.Port = GPIO_PORTB,
				.Pin = PIN_1,
				.Connection = SWITCH_CONNECTION_PULLDOWN
		},
};




/*
 * LCD_Config.c
 *
 *  Created on: Apr 2, 2024
 *      Author: Menna Ashraf
 */

#include "../../../include/LIB/STD_TYPES.h"
#include "../../../include/MCAL/GPIO/GPIO.h"

LCD_Pins_Configurations_t LCD_Pins ={
		.DataPins[0].PORT = GPIO_PORTA,
		.DataPins[0].PIN = GPIO_PIN_0,
		.DataPins[1].PORT = GPIO_PORTA,
		.DataPins[1].PIN = GPIO_PIN_1,
		.DataPins[2].PORT = GPIO_PORTA,
		.DataPins[2].PIN = GPIO_PIN_2,
		.DataPins[3].PORT = GPIO_PORTA,
		.DataPins[3].PIN = GPIO_PIN_3,
		.DataPins[4].PORT = GPIO_PORTA,
		.DataPins[4].PIN = GPIO_PIN_4,
		.DataPins[5].PORT = GPIO_PORTA,
		.DataPins[5].PIN = GPIO_PIN_5,
		.DataPins[6].PORT = GPIO_PORTA,
		.DataPins[6].PIN = GPIO_PIN_6,
		.DataPins[7].PORT = GPIO_PORTA,
		.DataPins[7].PIN = GPIO_PIN_7,

		.R_S_Pin.PORT = GPIO_PORTA,
		.R_S_Pin.PIN = GPIO_PIN_8,
		.R_W_Pin.PORT = GPIO_PORTA,
		.R_W_Pin.PIN = GPIO_PIN_9,
		.E_Pin.PORT = GPIO_PORTA,
		.E_Pin.PIN = GPIO_PIN_10,
};



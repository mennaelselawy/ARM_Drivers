#ifndef LED_H
#define LED_H

#include "../../LIB/STD_TYPES.h"
#include "LED_CONFIG.h"

#define LED_STATE_ON    1
#define LED_STATE_OFF   0

#define LED_CONNECTION_FORWARD  0
#define LED_CONNECTION_REVERSE  1

#define	 LED_PORTA               ((void *)0x40020000)
#define	 LED_PORTB               ((void *)0x40020400)
#define  LED_PORTC               ((void *)0x40020800)

typedef enum
{
	PIN_0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15
}LED_PINS_t;

typedef struct
{
	void * PORT;
	LED_PINS_t PIN;
	u32 Connection;
	u8 InitSTATE;
}LED_Config_t;

COM_ErrorStatus_t LED_Init(void);
COM_ErrorStatus_t LED_SetState(u32 LED_Name, u8 LED_State);

#endif

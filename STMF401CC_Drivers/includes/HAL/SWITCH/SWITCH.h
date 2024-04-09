#ifndef SWITCH_H
#define SWITCH_H
#include"../../LIB/STD_TYPES.h"
#include "SWITCH_CONFIG.h"



#define SWITCH_CONNECTION_Floating   0x0
#define SWITCH_CONNECTION_PULLUP     0x8
#define SWITCH_CONNECTION_PULLDOWN   0x10

#define SWITCH_OFFSET_4BITS          0x4

#define	 GPIO_PORTA               ((void *)0x40020000)
#define	 GPIO_PORTB               ((void *)0x40020400)
#define  GPIO_PORTC               ((void *)0x40020800)

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
}SWITCH_PINS_t;

typedef enum
{
	SWITCH_STATE_NOT_PRESSED,
	 SWITCH_STATE_PRESSED
}SWITCH_State_t;

typedef struct
{
	void * Port;
	SWITCH_PINS_t Pin;
	u8 Connection;
}SWITCH_Config_t;

/**
 * brief: Initializes all the switches configured in configure files
 * parameters: none
 * return: error status as switch module ok or not ok
 * note: Must configure the Switches in SWITCH_config.h and SWITCH_config.c first
 */
COM_ErrorStatus_t Switch_Init(void);

/*
 * brief: reads = RETURNS the switch state as pressed or not
 * parameters: switch name as the id in configuration array, address pointer to u8 which the state of switch returns in
 * return: error status as switch module ok or not ok
 * notes: must Initialize the Switches first using SWITCH_Init() function
 */
COM_ErrorStatus_t Switch_GetSwitchState(Switch_Name_t Switch_Name, u8* Add_pState);

#endif

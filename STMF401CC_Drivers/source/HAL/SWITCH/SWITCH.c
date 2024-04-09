/*
 * SWITCH.c
 *
 *  Created on: Feb 26, 2024
 *      Author: Menna Ashraf
 */
#include"../../../include/LIB/STD_TYPES.h"
#include"../../../include/MCAL/GPIO/GPIO.h"
#include"../../../include/HAL/SWITCH/SWITCH.h"

#define SWITCH_PU_PD_SHIFT    0x3
#define SWITCH_THRESHOLD             5

extern const SWITCH_Config_t Switches[Switch_Number];
static u8 G_Switch_State[Switch_Number];

COM_ErrorStatus_t Switch_Init(void)
{
	COM_ErrorStatus_t RET_SwitchErrorState = SWITCH_OK;
	u16 Index;
	GPIO_strConfig_t Switch;
	Switch.SPEED = GPIO_SPEED_HIGH;
	Switch.AF = GPIO_AF_SYSTEM;
	for(Index =0; Index < Switch_Number; Index++)
	{
		if( ( (Switches[Index].Connection) != SWITCH_CONNECTION_Floating)
		  ||( (Switches[Index].Connection) != SWITCH_CONNECTION_PULLUP)
		  ||( (Switches[Index].Connection) != SWITCH_CONNECTION_PULLDOWN ) )
		{
			RET_SwitchErrorState = SWITCH_NOK;
		}
		else
		{
			Switch.PORT = Switches[Index].Port;
			Switch.PIN = Switches[Index].Pin;
			Switch.MODE = Switches[Index].Connection;
			RET_SwitchErrorState = GPIO_Init(&Switch);
		}
	}
	return RET_SwitchErrorState;
}

COM_ErrorStatus_t Switch_GetSwitchState(u8 Switch_Name, u8* Add_pState)
{
	COM_ErrorStatus_t RET_SwitchErrorState = SWITCH_NOK;
	if(Switch_Name > Switch_Number)
	{
		RET_SwitchErrorState = SWITCH_NOK;
	}
	else if(Add_pState == NULL)
	{
		RET_SwitchErrorState = Error_NULLPointer;
	}
	else
	{
		RET_SwitchErrorState = SWITCH_OK;
		*Add_pState = !( G_Switch_State[Switch_Name] ^ (Switches[Switch_Name].Connection >> SWITCH_OFFSET_4BITS) );

	}
	return RET_SwitchErrorState;
}

void Switch_Runnable(void)
{
	u8 LOC_Index;
	u8 LOC_CurrentState;
	static u8 LOC_PreviousState[Switch_Number] = {0};
	static u8 LOC_Counts[Switch_Number] = {0};
	for(LOC_Index =0; LOC_Index <Switch_Number ; LOC_Index++)
	{
		GPIO_GetPinValue(Switches[LOC_Index].Port, Switches[LOC_Index].Pin, &LOC_CurrentState);
		if(LOC_CurrentState == LOC_PreviousState[LOC_Index])
		{
			LOC_Counts[LOC_Index] ++;
		}
		else
		{
			LOC_Counts[LOC_Index] =0;
		}
		if(LOC_Counts == SWITCH_THRESHOLD)
		{
			G_Switch_State[LOC_Index] = LOC_CurrentState;
			LOC_Counts[LOC_Index] =0;
		}
		LOC_PreviousState[LOC_Index] = LOC_CurrentState;
	}
}

/*
 * COM_ErrorStatus_t SWITCH_ReadSwitchState(u8 Switch_Name, SWITCH_State_t* Add_PSwitchState)
{
	COM_ErrorStatus_t swReturn_enumError = SWITCH_NOK;
	u8 LOC_SwitchPinValue;

	if (Switch_Name < Switch_Number)
	{
		swReturn_enumError = DIO_enumGetPinValue(Switches[Switch_Name].PORT, Switches[Switch_Name].PIN, &LOC_SwitchPinValue);

		if (SWITCH_OK **1** == swReturn_enumError)
		{
			switch (Switches[Switch_Name].CONFIGURATION)
			{
				case SWITCH_enumInputExternalPD: break;
				case SWITCH_enumInputInternalPU: LOC_SwitchPinValue = ! LOC_SwitchPinValue; break;
				default: swReturn_enumError = SWITCH_NOK;
			}

			if (SWITCH_enumPressed == (SWITCH_enumState_t)LOC_SwitchPinValue)
			{
				_delay_ms(300); //Switches[Switch_Name].delay_debounce

				*Add_PSwitchState = SWITCH_enumPressed;
			}
			else
			{
				*Add_PSwitchState = SWITCH_enumNotPressed;
			}
		}
		else
		{

		}
	}
	else
	{
		swReturn_enumError = SWITCH_NOK;
	}
	return swReturn_enumError;
}


		RET_SwitchErrorState = GPIO_GetPinValue(Switches[Switch_Name].Port, Switches[Switch_Name].Pin, &Add_pState);
		// for pull down no change but pull up need to be toggled
		RET_SwitchErrorState ^= ((Switches[Switch_Name].Connection & SWITCH_CONNECTION_PULLUP) >> SWITCH_PU_PD_SHIFT);

 */


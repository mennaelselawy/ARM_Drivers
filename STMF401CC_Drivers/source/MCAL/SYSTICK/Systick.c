/*
 * Systick.c
 *
 *  Created on: Mar 23, 2024
 *      Author: Menna Ashraf
 */

#include "../include/LIB/STD_TYPES.h"
#include "../include/Systick.h"
#include "../include/Systick_Config.h"

#define ENABLE_MASK              0x1
#define TICK_INT_MASK            0x2
#define CLK_SOURCE_MASK          0x4
#define COUNT_FLAG_MASK          0x10000
#define MAX_LOAD_VALUE           0xFFFFFF  /*24 bits*/

#define MS_TO_US(ms_Time)        ms_Time/1000
#define US_TO_MS(us_time)        us_time*1000

#define SYSTICK_BASE_ADDRESS     0xE000E010
#define SYSTICK                 ((volatile Systick_Registers_t*)SYSTICK_BASE_ADDRESS)

typedef struct{
	volatile u32 CTRL;
	volatile u32 LOAD;
	volatile u32 VAL;
	volatile u32 CALIB;
}Systick_Registers_t;

static SystickCBF_t GLOBAL_AppCBF;

COM_ErrorStatus_t Systick_Start(u8 MODE) /*Enable Systick*/
{
	COM_ErrorStatus_t RET_ErorrStatus = SYSTICK_NOK;
	if(MODE > SYSTICK_MODE_PERIODIC)
	{
		RET_ErorrStatus = SYSTICK_NOK;
	}
	else
	{
		SYSTICK->CTRL &=~ TICK_INT_MASK;
		SYSTICK->CTRL |= (ENABLE_MASK | MODE);
		RET_ErorrStatus = SYSTICK_OK;
	}
	return RET_ErorrStatus;
}

COM_ErrorStatus_t Systick_ConfigureClockSource(u8 Clock_Source)
{
	COM_ErrorStatus_t RET_ErorrStatus = SYSTICK_NOK;
	if(Clock_Source > SYSTICK_CLOCK_SOURCE_PROCESSOR_AHB)
	{
		RET_ErorrStatus = SYSTICK_NOK;
	}
	else
	{
		switch(Clock_Source)
		{
		case SYSTICK_CLOCK_SOURCE_EXTERNAL_AHBdiv8:
			SYSTICK->CTRL &= ~CLK_SOURCE_MASK;
			RET_ErorrStatus = SYSTICK_OK;
			break;
		case SYSTICK_CLOCK_SOURCE_PROCESSOR_AHB:
			SYSTICK->CTRL |= CLK_SOURCE_MASK;
			RET_ErorrStatus = SYSTICK_OK;
			break;
		default:
			RET_ErorrStatus = SYSTICK_NOK;
		}
	}
	return RET_ErorrStatus;
}

COM_ErrorStatus_t Systick_SetTimeMS(u32 Time)
{
	COM_ErrorStatus_t RET_ErorrStatus = SYSTICK_NOK;
	u32 LOC_Prescaller = 8;
	u32 LOC_SystemClock = 16;
	u32 LOC_TimeTicks_US = LOC_Prescaller/LOC_SystemClock;
	u32 LOC_Time_Overflow_MS = US_TO_MS(MAX_LOAD_VALUE * LOC_TimeTicks_US);

	u32 LOC_ClockSpeed_US;
	if(Time > LOC_Time_Overflow_MS)
	{
		RET_ErorrStatus = SYSTICK_NOK;
	}
	else
	{
		if((SYSTICK->CTRL & CLK_SOURCE_MASK) == SYSTICK_CLOCK_SOURCE_PROCESSOR_AHB)
		{
			LOC_ClockSpeed_US = SYSTICK_CLOCK_PROCESSOR_AHB;
		}
		else
		{
			LOC_ClockSpeed_US = SYSTICK_CLOCK_EXTERNAL_AHBdiv8;
		}
		SYSTICK->LOAD = (MS_TO_US(Time) * LOC_ClockSpeed_US) - 1;
		RET_ErorrStatus = SYSTICK_OK;
	}
	return RET_ErorrStatus;
}

void Systick_IsExbired(void) //disable systick
{
	SYSTICK->CTRL &=~ ENABLE_MASK;
}

void Systick_EnbaleInterrupt(void)
{
	SYSTICK->CTRL |= TICK_INT_MASK;
}
void Systick_DisableInterrupt(void)
{
	SYSTICK->CTRL &=~ TICK_INT_MASK;
}

COM_ErrorStatus_t SetCallBack(SystickCBF_t CBF)
{
	COM_ErrorStatus_t RET_ErorrStatus = SYSTICK_NOK;
	if(CBF == NULL)
	{
		RET_ErorrStatus = Error_NULLPointer;
	}
	else
	{
		GLOBAL_AppCBF = CBF;
		RET_ErorrStatus = SYSTICK_OK;
	}
	return RET_ErorrStatus;
}

void Systick_Handler(void)
{
	if(GLOBAL_AppCBF)
	{
		GLOBAL_AppCBF();
	}
}

/*
 * RCC.c
 *
 *  Created on: Feb 12, 2024
 *      Author: Menna Ashraf
 */

/*    #INCLUDES         */
#include "../../../include/LIB/STD_TYPES.h"
#include "../../../include/MCAL/RCC/RCC.h"

/*    #DEFINES         */

// Three main system clocks
#define SYSCLK_MASK            0x3

//Five Clocks
#define  CR_HSI_READY_BIT      0x2
#define  CR_HSI_ON_BIT         0x1
#define  CR_HSE_READY_BIT      0x20000
#define  CR_HSE_ON_BIT         0x10000
#define  CR_PLL_READY_BIT      0x2000000
#define  CR_PLL_ON_BIT         0x1000000
#define  CR_PLL2_READY_BIT     0x8000000
#define  CR_PLL2_ON_BIT        0x4000000
#define  CSR_LSI_READY_BIT     0x2
#define  CSR_LSI_ON_BIT        0x1
#define  BDCR_LSE_READY_BIT    0x2
#define  BDCR_LSE_ON_BIT       0x1

#define  RCC_READY_STATE       1
#define  RCC_NOT_READY_STATE   0

#define  PLLSRC_MASK           0x400000
#define  PLLM_MASK             0x3F
#define  PLLN_MASK             0x7FC0
#define  PLLP_MASK             0x30000
#define  PLLQ_MASK             0xF000000

#define  PLLM_SHIFT            0
#define  PLLN_SHIFT            6
#define  PLLP_SHIFT            16
#define  PLLQ_SHIFT            24

#define  AHB_PRESCALE_MASK     0xF0
#define  APB1_PRESCALE_MASK    0x1C00
#define  APB2_PRESCALE_MASK    0xE000

#define  AHB_SHIFT             4
#define  APB1_SHIFT            10
#define  APB2_SHIFT            13

#define RCC_BASE_ADDRESS      0x40023800
#define RCC                 ((volatile RCC_t *) RCC_BASE_ADDRESS)

/*    #TYPES         */
typedef struct
{
	volatile u32 CR;
	volatile u32 PLLCFGR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 AHB1RSTR;
	volatile u32 AHB2RSTR;
	volatile u32 Reserved1[2];
	volatile u32 APB1RSTR;
	volatile u32 APB2RSTR;
	volatile u32 Reserved2[2];
	volatile u32 AHB1ENR;
	volatile u32 AHB2ENR;
	volatile u32 Reserved3[2];
	volatile u32 APB1ENR;
	volatile u32 APB2ENR;
	volatile u32 Reserved4[2];
	volatile u32 AHB1LPENR;
	volatile u32 AHB2LPENR;
	volatile u32 Reserved5[2];
	volatile u32 APB1LPENR;
	volatile u32 APB2LPENR;
	volatile u32 Reserved6[2];
	volatile u32 BDCR;
	volatile u32 CSR;
	volatile u32 Reserved7[2];
	volatile u32 SSCGR;
	volatile u32 PLLI2SCFGR;
	volatile u32 DCKCFGR;
}RCC_t;

/*    #VARIABLES         */
u32 TimeOut = 1000;
/*   STATIC API'S           */

/*    #IMPLEMENTATION         */

COM_ErrorStatus_t RCC_SelectSystemClock(u8 SYSCLK)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	u32 LOC_TempReg;
	if(SYSCLK < SYSCLK_HSI || SYSCLK > SYSCLK_PLL)
	{
		LOC_ReturnErrorState = RCC_InvalidSYSCLK;
	}
	else
	{
		switch(SYSCLK)
		{
		case SYSCLK_HSI:
			if(RCC->CR & CR_HSI_ON_BIT)          //check on
			{
				if(RCC->CR & CR_HSI_READY_BIT)    //check ready
				{
					LOC_TempReg = RCC->CFGR;
					LOC_TempReg &=~ SYSCLK_MASK;
					LOC_TempReg |= SYSCLK;
					RCC->CFGR = LOC_TempReg;
				}
				else
				{
					LOC_ReturnErrorState = RCC_ClockNotReady;
				}
			}
			else
			{
				LOC_ReturnErrorState = RCC_ClockNotON;
			}
		break;
		case SYSCLK_HSE:
			if(RCC->CR & CR_HSE_ON_BIT)          //check on
			{
				if(RCC->CR & CR_HSE_READY_BIT)    //check ready
				{
					LOC_TempReg = RCC->CFGR;
					LOC_TempReg &=~ SYSCLK_MASK;
					LOC_TempReg |= SYSCLK;
					RCC->CFGR = LOC_TempReg;
				}
				else
				{
					LOC_ReturnErrorState = RCC_ClockNotReady;
				}
			}
			else
			{
				LOC_ReturnErrorState = RCC_ClockNotON;
			}
		break;
		case SYSCLK_PLL:
			if(RCC->CR & CR_PLL_ON_BIT)           //check on
			{
				if(RCC->CR & CR_PLL_READY_BIT)    //check ready
				{
					LOC_TempReg = RCC->CFGR;
					LOC_TempReg &=~ SYSCLK_MASK;
					LOC_TempReg |= SYSCLK;
					RCC->CFGR = LOC_TempReg;
				}
				else
				{
					LOC_ReturnErrorState = RCC_ClockNotReady;
				}
			}
			else
			{
				LOC_ReturnErrorState = RCC_ClockNotON;
			}
		break;
		default:
			LOC_ReturnErrorState = RCC_InvalidSYSCLK;
	    }
		LOC_ReturnErrorState = RCC_OK;
	}
	return LOC_ReturnErrorState;
}

COM_ErrorStatus_t RCC_EnableClock(RCC_Clock_t CLOCK)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	if(CLOCK < CLOCK_HSI || CLOCK > CLOCK_LSI)
	{
		LOC_ReturnErrorState = RCC_InvalidClock;
	}
	else
	{
		switch(CLOCK)
		{
		case CLOCK_HSI:
			RCC->CR |= CR_HSI_ON_BIT;
			while(!(RCC->CR & CR_HSI_READY_BIT) && TimeOut)
			{
				TimeOut --;
			}
			if(!(RCC->CR & CR_HSI_READY_BIT))
			{
				LOC_ReturnErrorState = RCC_ClockNotReady;
			}
			break;
		case CLOCK_HSE:
			RCC->CR |= CR_HSE_ON_BIT;
			while(!(RCC->CR & CR_HSE_READY_BIT) && TimeOut)
			{
				TimeOut --;
			}
			if(!(RCC->CR & CR_HSE_READY_BIT))
			{
				LOC_ReturnErrorState = RCC_ClockNotReady;
			}
			break;
		case CLOCK_PLL:
			RCC->CR |= CR_PLL_ON_BIT;
			while(!(RCC->CR & CR_PLL_READY_BIT) && TimeOut)
			{
				TimeOut --;
			}
			if(!(RCC->CR & CR_PLL_READY_BIT))
			{
				LOC_ReturnErrorState = RCC_ClockNotReady;
			}
		break;
		case CLOCK_PLL2:
			RCC->CR |= CR_PLL2_ON_BIT;
			while(!(RCC->CR & CR_PLL2_READY_BIT) && TimeOut)
			{
				TimeOut --;
			}
			if(!(RCC->CR & CR_PLL2_READY_BIT))
			{
				LOC_ReturnErrorState = RCC_ClockNotReady;
			}
		break;
		case CLOCK_LSE:
			RCC->BDCR |= BDCR_LSE_ON_BIT;
			while(!(RCC->BDCR & BDCR_LSE_READY_BIT) && TimeOut)
			{
				TimeOut --;
			}
			if(!(RCC->CR & BDCR_LSE_READY_BIT))
			{
				LOC_ReturnErrorState = RCC_ClockNotReady;
			}
		break;
		case CLOCK_LSI:
			RCC->CSR |= CSR_LSI_ON_BIT;
			while(!(RCC->CSR & CSR_LSI_READY_BIT) && TimeOut)
			{
				TimeOut --;
			}
			if(!(RCC->CR & CSR_LSI_READY_BIT))
			{
				LOC_ReturnErrorState = RCC_ClockNotReady;
			}
		break;
		default:
			LOC_ReturnErrorState = RCC_InvalidClock;
		}
		LOC_ReturnErrorState = RCC_OK;
	}
	return LOC_ReturnErrorState;
}

COM_ErrorStatus_t RCC_DisableClock(RCC_Clock_t CLOCK)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	if(CLOCK < CLOCK_HSI || CLOCK > CLOCK_LSI)
	{
		LOC_ReturnErrorState = RCC_InvalidClock;
	}
	else
	{
		switch(CLOCK)
				{
				case CLOCK_HSI:
					RCC->CR &=~ CR_HSI_ON_BIT;
					break;
				case CLOCK_HSE:
					RCC->CR &=~ CR_HSE_ON_BIT;
					break;
				case CLOCK_PLL:
					RCC->CR &=~ CR_PLL_ON_BIT;
				break;
				case CLOCK_PLL2:
					RCC->CR &=~ CR_PLL2_ON_BIT;
				break;
				case CLOCK_LSE:
					RCC->BDCR &=~ BDCR_LSE_ON_BIT;
				break;
				case CLOCK_LSI:
					RCC->CSR &=~ CSR_LSI_ON_BIT;
				break;
				default:
					LOC_ReturnErrorState = RCC_InvalidClock;
				}
		LOC_ReturnErrorState = RCC_OK;
	}
	return LOC_ReturnErrorState;
}

COM_ErrorStatus_t RCC_CheckReady(u8 CLOCK, u8* Add_pReadyState)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;

	if( (CLOCK == CLOCK_HSI && (RCC->CR & CR_HSI_READY_BIT))
		|| (CLOCK == CLOCK_HSE && (RCC->CR & CR_HSE_READY_BIT))
		|| (CLOCK == CLOCK_PLL && (RCC->CR & CR_PLL_READY_BIT))
		|| (CLOCK == CLOCK_PLL2 && (RCC->CR & CR_PLL2_READY_BIT))
		|| (CLOCK == CLOCK_LSE && (RCC->BDCR & BDCR_LSE_READY_BIT))
		|| (CLOCK == CLOCK_LSI && (RCC->CSR & CSR_LSI_READY_BIT))   )
	{
		*Add_pReadyState = RCC_READY_STATE;
		LOC_ReturnErrorState = RCC_OK;
	}
	else
	{
		*Add_pReadyState = RCC_NOT_READY_STATE;
		LOC_ReturnErrorState = RCC_ClockNotReady;
	}
	return LOC_ReturnErrorState;
}

COM_ErrorStatus_t RCC_SetPLLSource(u8 SYSCLK)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	if(SYSCLK == SYSCLK_HSI)
	{
		if(!(RCC->CR & CR_HSI_ON_BIT) && !(RCC->CR & CR_HSI_READY_BIT))
		{
			LOC_ReturnErrorState = RCC_ClockNotReady;
		}
		else
		{
			RCC->PLLCFGR &=~ PLLSRC_MASK;
		}

	}
	else if(SYSCLK == SYSCLK_HSE)
	{
		if(!(RCC->CR & CR_HSE_ON_BIT) && !(RCC->CR & CR_HSE_READY_BIT))
		{
			LOC_ReturnErrorState = RCC_ClockNotReady;
		}
		else
		{
			RCC->PLLCFGR |= PLLSRC_MASK;
		}
	}
	else {
		LOC_ReturnErrorState = RCC_InvalidClock ;
	}
	return LOC_ReturnErrorState;
}
COM_ErrorStatus_t RCC_ConfigurePLL(u8 PLLM, u8 PLLN, u8 PLLP, u8 PLLQ)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	u32 LOC_TempRegister ;

	if( (2 > PLLM || PLLM > 63)
		||(2 > PLLN || PLLN > 510 || PLLN == 433)
		||(PLLP != 2 || PLLP != 4 || PLLP != 6 || PLLP != 8 )
		|| ((RCC->CR & CR_PLL_ON_BIT) && (1 > PLLQ || PLLQ > 16) )
		)
	{
		LOC_ReturnErrorState = RCC_InvalidParameter;
	}
	else
	{
		LOC_TempRegister = RCC->PLLCFGR;
		LOC_TempRegister &=~ PLLM_MASK;
		LOC_TempRegister &=~ PLLN_MASK;
		LOC_TempRegister &=~ PLLP_MASK;
		LOC_TempRegister &=~ PLLQ_MASK;
		LOC_TempRegister |= (PLLM << PLLM_SHIFT) | (PLLN << PLLN_SHIFT) | (PLLP << PLLP_SHIFT) | (PLLQ << PLLQ_SHIFT);
		RCC->PLLCFGR = LOC_TempRegister;
		LOC_ReturnErrorState = RCC_OK;
	}
	return LOC_ReturnErrorState;
}

COM_ErrorStatus_t RCC_SetAHBPrescaler(u32 AHBPrescale)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	u32 LOC_TempRegister;
	if(AHBPrescale < 1 || AHBPrescale > 512)
	{
		LOC_ReturnErrorState = RCC_NOK;
	}
	else
	{
		LOC_TempRegister = RCC->CFGR;
		LOC_TempRegister &=~ AHB_PRESCALE_MASK;
		LOC_TempRegister |= (AHBPrescale << AHB_SHIFT);
		RCC->CFGR = LOC_TempRegister;
		LOC_ReturnErrorState = RCC_OK;
	}
	return LOC_ReturnErrorState;
}
COM_ErrorStatus_t RCC_SetAPB1Prescaler(u32 APB1Prescale)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	u32 LOC_TempRegister;
	if(APB1Prescale < 1 || APB1Prescale > 16)
	{
		LOC_ReturnErrorState = RCC_NOK;
	}
	else
	{
		LOC_TempRegister = RCC->CFGR;
		LOC_TempRegister &=~ APB1_PRESCALE_MASK;
		LOC_TempRegister |= (APB1Prescale << APB1_SHIFT);
		RCC->CFGR = LOC_TempRegister;
		LOC_ReturnErrorState = RCC_OK;
	}
	return LOC_ReturnErrorState;
}
COM_ErrorStatus_t RCC_SetAPB2Prescaler(u32 APB2Prescale)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	u32 LOC_TempRegister;
	if(APB2Prescale < 1 || APB2Prescale > 16)
	{
		LOC_ReturnErrorState = RCC_NOK;
	}
	else
	{
		LOC_TempRegister = RCC->CFGR;
		LOC_TempRegister &=~ APB2_PRESCALE_MASK;
		LOC_TempRegister |= (APB2Prescale << APB2_SHIFT);
		RCC->CFGR = LOC_TempRegister;
		LOC_ReturnErrorState = RCC_OK;
	}
	return LOC_ReturnErrorState;
}


COM_ErrorStatus_t RCC_EnablePeripheral(u32 BUS,u32 Peripheral)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	if(BUS < BUS_AHB1 || BUS > BUS_APB2)
	{
		LOC_ReturnErrorState = RCC_InvalidBus;
	}
	else if (BUS == BUS_AHB1)
	{
		RCC->AHB1ENR |= Peripheral;
	}
	else if(BUS == BUS_AHB2)
	{
		RCC->AHB2ENR |= Peripheral;
	}
	else if(BUS == BUS_APB1)
	{
		RCC->APB1ENR |= Peripheral;
	}
	else if(BUS == BUS_APB2)
	{
		RCC->APB2ENR |= Peripheral;
	}
	else
	{
		LOC_ReturnErrorState = RCC_NOK;
	}
	return LOC_ReturnErrorState;
}
COM_ErrorStatus_t RCC_DisablePeripheral(u32 BUS,u32 Peripheral)
{
	COM_ErrorStatus_t LOC_ReturnErrorState = RCC_NOK;
	if(BUS < BUS_AHB1 || BUS > BUS_APB2)
	{
		LOC_ReturnErrorState = RCC_InvalidBus;
	}
	else if (BUS == BUS_AHB1)
	{
		RCC->AHB1ENR &=~ Peripheral;
	}
	else if(BUS == BUS_AHB2)
	{
		RCC->AHB2ENR &=~ Peripheral;
	}
	else if(BUS == BUS_APB1)
	{
		RCC->APB1ENR &=~ Peripheral;
	}
	else if(BUS == BUS_APB2)
	{
		RCC->APB2ENR &=~ Peripheral;
	}
	else
	{
		LOC_ReturnErrorState = RCC_NOK;
	}
	return LOC_ReturnErrorState;
}

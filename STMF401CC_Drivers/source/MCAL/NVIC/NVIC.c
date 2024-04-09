/*
 * NVIC.c
 *
 *  Created on: Mar 6, 2024
 *      Author: Menna Ashraf
 */

#include"../../../include/LIB/STD_TYPES.h"
#include"../../../include/MCAL/NVIC/STM32F401CC_VectorTable.h"
#include"../../../include/MCAL/NVIC/NVIC.h"

#define NVIC_BaseAddress  0xE000E100
#define SCB_BaseAddress   0xE000ED00

#define SCB_AIRCR  ((u32 *)0xE000ED0C)

#define NVIC              ((volatile NVIC_Registers*)NVIC_BaseAddress)
#define SCB               ((volatile SCB_Registers*)SCB_BaseAddress)

#define  SCB_PRIGROUP_MASK               0x700
#define  SCB_VECTKEY_MASK                0xFFFF0000
#define  SCB_AIRCR_VECTKEY_WRITE         0x5FA
#define  SCB_AIRCR_VECTKEY_READ          0xFA05

#define  PRIORITY_BITS                   4
#define  NON_IMPLEMENTED_PRIORITY_BITS   4

#define  EIGHT_BIT_MASK 0xFF


#define PREEMPTION_0_BYTEOFFSET  0
#define PREEMPTION_1_BYTEOFFSET  1
#define PREEMPTION_2_BYTEOFFSET  2
#define PREEMPTION_3_BYTEOFFSET  3

COM_ErrorStatus_t NVIC_EnableInterrupt(u8 IRQ_ID)
{
	COM_ErrorStatus_t Ret_ErrorStatus = NVIC_NOK;
	u32 LOC_RegNumber;
	u32 LOC_BitNumber;
	if(IRQ_ID >= NVIC_MaxNumber)
	{
		Ret_ErrorStatus =  NVIC_NOK;
	}
	else
	{
		LOC_RegNumber = IRQ_ID / 32;
		LOC_BitNumber = IRQ_ID % 32;
		 NVIC->ISER[LOC_RegNumber] = (1<<LOC_BitNumber);
		Ret_ErrorStatus = NVIC_OK;
	}
	return Ret_ErrorStatus;
}

COM_ErrorStatus_t NVIC_DisableInterrupt(u8 IRQ_ID)
{
	COM_ErrorStatus_t Ret_ErrorStatus = NVIC_NOK;
	u32 LOC_RegNumber;
	u32 LOC_BitNumber;
	if(IRQ_ID >= NVIC_MaxNumber)
	{
		Ret_ErrorStatus =  NVIC_NOK;
	}
	else
	{
		LOC_RegNumber = IRQ_ID / 32;
		LOC_BitNumber = IRQ_ID % 32;
		 NVIC->ICER[LOC_RegNumber] = (1<<LOC_BitNumber);
		Ret_ErrorStatus = NVIC_OK;
	}
	return Ret_ErrorStatus;
}

COM_ErrorStatus_t NVIC_SetPendingInterrupt(u8 IRQ_ID)
{
	COM_ErrorStatus_t Ret_ErrorStatus = NVIC_NOK;
	u32 LOC_RegNumber;
	u32 LOC_BitNumber;
	if(IRQ_ID >= NVIC_MaxNumber)
	{
		Ret_ErrorStatus =  NVIC_NOK;
	}
	else
	{
		LOC_RegNumber = IRQ_ID / 32;
		LOC_BitNumber = IRQ_ID % 32;
		 NVIC->ISPR[LOC_RegNumber] = (1<<LOC_BitNumber);
		Ret_ErrorStatus = NVIC_OK;
	}
	return Ret_ErrorStatus;
}

COM_ErrorStatus_t NVIC_ClearPendingInterrupt(u8 IRQ_ID)
{
	COM_ErrorStatus_t Ret_ErrorStatus = NVIC_NOK;
	u32 LOC_RegNumber;
	u32 LOC_BitNumber;
	if(IRQ_ID >= NVIC_MaxNumber)
	{
		Ret_ErrorStatus =  NVIC_NOK;
	}
	else
	{
		LOC_RegNumber = IRQ_ID / 32;
		LOC_BitNumber = IRQ_ID % 32;
		 NVIC->ICPR[LOC_RegNumber] = (1<<LOC_BitNumber);
		Ret_ErrorStatus = NVIC_OK;
	}
	return Ret_ErrorStatus;
}

COM_ErrorStatus_t NVIC_GetPendingIRQ(u8 IRQ_ID, u8* Add_PendingState)
{
	COM_ErrorStatus_t Ret_ErrorStatus = NVIC_NOK;
	u32 LOC_RegNumber;
	u32 LOC_BitNumber;
	if(IRQ_ID >= NVIC_MaxNumber)
	{
		Ret_ErrorStatus =  NVIC_NOK;
	}
	else
	{
		LOC_RegNumber = IRQ_ID / 32;
		LOC_BitNumber = IRQ_ID % 32;
		*Add_PendingState = NVIC->ISPR[LOC_RegNumber] & (1<<LOC_BitNumber);
	}
	return Ret_ErrorStatus;
}

COM_ErrorStatus_t NVIC_GetActiveState(u8 IRQ_ID, u8* Add_ActiveState)
{
	COM_ErrorStatus_t Ret_ErrorStatus = NVIC_NOK;
	u32 LOC_RegNumber;
	u32 LOC_BitNumber;
	if(IRQ_ID >= NVIC_MaxNumber)
	{
		Ret_ErrorStatus =  NVIC_NOK;
	}
	else
	{
		LOC_RegNumber = IRQ_ID / 32;
		LOC_BitNumber = IRQ_ID % 32;
		*Add_ActiveState = NVIC->IABR[LOC_RegNumber] & (1<<LOC_BitNumber);
	}
	return Ret_ErrorStatus;
}

COM_ErrorStatus_t NVIC_SetPriority(u8 IRQ_ID, u8 PreemptionNumber, u8 SubGroupNumber, u32 SubGroupBitsMask)
{
	COM_ErrorStatus_t Ret_ErrorStatus = NVIC_NOK;
	u32 LOC_RegNumber;
	u32 LOC_ByteOffset;
	if(IRQ_ID <= NVIC_MaxNumber)
	{
		Ret_ErrorStatus =  NVIC_NOK;
	}
	else
	{
		/*Extract Subgroup Bits from Subgroup Mask*/
		 u8 SubGroupBitsNum=(u8)(((SubGroupBitsMask>>8)&0x07)-0x03);

		/*Extract Preemption Bits from Data*/
		 u8 PreemptionBitsNum=PRIORITY_BITS-SubGroupBitsNum;

	    /**************************************************/
        /*	    SubGroup Configuration - SCB Peripheral   */
        /**************************************************/
        u32 Local_AIRCR=SCB->AIRCR;
        /*Apply Write Mask to Register*/
        Local_AIRCR &=~ SCB_VECTKEY_MASK;
        /*Apply Priority Mask to Register*/
        Local_AIRCR &=~ SCB_PRIGROUP_MASK;
        /*Apply Write Value + Priority Value to Register*/
        Local_AIRCR |= (SCB_AIRCR_VECTKEY_WRITE | SubGroupBitsMask);
        SCB->AIRCR=Local_AIRCR;

        /***************************************************/
        /*	    Priority Configuration - NVIC Peripheral   */
        /***************************************************/
        u8 RegisterNumber= IRQ_ID/4;
        u8 ByteOffset= IRQ_ID%4;

        u32 Local_IPR=NVIC->IPR[RegisterNumber];
        Local_IPR &=~ (EIGHT_BIT_MASK<<ByteOffset);
        Local_IPR |= (((PreemptionNumber<<SubGroupBitsNum)|(SubGroupNumber))<<NON_IMPLEMENTED_PRIORITY_BITS);
        NVIC->IPR[RegisterNumber] |= Local_IPR;
	}
	return Ret_ErrorStatus;
}

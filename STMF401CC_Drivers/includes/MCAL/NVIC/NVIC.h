/*
 * NVIC.h
 *
 *  Created on: Mar 6, 2024
 *      Author: Menna Ashraf
 */
#ifndef NVIC_H
#define NVIC_H

#include"../../LIB/STD_TYPES.h"



typedef struct
{
	  volatile u32   ISER[8];
	  volatile u32   Reserved0[24];
	  volatile u32   ICER[8];
	  volatile u32   Reserved1[24];
	  volatile u32   ISPR[8];
	  volatile u32   Reserved2[24];
	  volatile u32   ICPR[8];
	  volatile u32   Reserved3[24];
	  volatile u32   IABR[8];
	  volatile u32   Reserved4[56];
	  volatile u32   IPR[60];
	  volatile u32   Reserved5[580];
	  //volatile u32   IP[240];
	  //volatile u32   Reserved5[644];
	  volatile u32   STIR;
}NVIC_Registers;

typedef struct
{
	volatile u32  CPUID;
	volatile u32  ICSR;
	volatile u32  VTOR;
	volatile u32  AIRCR;
	volatile u32  SCR;
	volatile u32  CCR;
	volatile u32  SHPR1;
	volatile u32  SHPR2;
	volatile u32  SHPR3;
	volatile u32  SHCSR;
	volatile u32  CFSR;
	volatile u32  HFSR;
	volatile u32  RESERVED;
	volatile u32  MMAR;
	volatile u32  BFAR;
	volatile u32  AFSR;
}SCB_Registers;


COM_ErrorStatus_t NVIC_EnableInterrupt(u8 IRQ_ID);
COM_ErrorStatus_t NVIC_DisableInterrupt(u8 IRQ_ID);
COM_ErrorStatus_t NVIC_SetPendingInterrupt(u8 IRQ_ID);
COM_ErrorStatus_t NVIC_ClearPendingInterrupt(u8 IRQ_ID);
COM_ErrorStatus_t NVIC_GetPendingIRQ(u8 IRQ_ID, u8* Add_PendingState);
COM_ErrorStatus_t NVIC_GetActiveState(u8 IRQ_ID, u8* Add_ActiveState); //active or not active
COM_ErrorStatus_t NVIC_SetPriority(u8 IRQ_ID, u8 PreemptionNumber, u8 SubGroupNumber, u32 SubGroupBitsMask);

/*
COM_ErrorStatus_t NVIC_SetPriority(u8 IRQ_ID, u8 PreemptionNumber);
COM_ErrorStatus_t NVIC_GetPriority(u8 IRQ_ID, u8* Add_PreemptionNumber);
COM_ErrorStatus_t NVIC_SetSubGroupNumber(u8 IRQ_ID, u8 PreemptionNumber, u8 SubGroupNumber);
COM_ErrorStatus_t NVIC_GetSubGroupNumber();
*/
#endif

/*
 * GPIO.c
 *
 *  Created on: Feb 20, 2024
 *      Author: Menna Ashraf
 */
#include "../../../include/LIB/STD_TYPES.h"
#include "../../../include/MCAL/GPIO/GPIO.h"

#define	 GPIOA_BASE_ADDRESS                0x40020000
#define	 GPIOB_BASE_ADDRESS                0x40020400
#define  GPIOC_BASE_ADDRESS                0x40020800


#define  GPIO_MODE_MASK                   0x3
#define  GPIO_OUT_TYPE_MASK               0x4
#define  GPIO_PUPD_MASK                   0x18
#define  GPIO_OUT_TYPE_SHIFT              0x2
#define  GPIO_PUPD_SHIFT                  0x3
#define  GPIO_PINOFFSET_2BITS_PER_PIN     0x2
#define  GPIO_PINOFFSET_4BITS_PER_PIN     0x4
#define  GPIO_2_BITS_MASK                 0x3
#define  GPIO_4_BITS_MASK                 0xF   //FOR AFLR , AFHR
#define  GPIO_BSRR_RESET                  0x10  //16 bits for reset

typedef struct
{
	volatile u32 MODER;
	volatile u32 OTYPER;
	volatile u32 OSPEEDR;
	volatile u32 PUPDR;
	volatile u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 LCKR;
	volatile u64 AFR;
	//volatile u32 AFRL;
	//volatile u32 AFRH;
}GPIO_t;

COM_ErrorStatus_t GPIO_Init(GPIO_strConfig_t* PINConfig)
{
	COM_ErrorStatus_t RET_GPIOErrorState = GPIO_NOK;
	u32 LOC_TempRergister;
	u64 LOC_TempRegisterAF;
	if( (PINConfig == NULL) || (PINConfig->PORT == NULL) )
	{
		RET_GPIOErrorState = Error_NULLPointer;
	}
	else if(  (PINConfig->MODE > GPIO_MODE_AF_OD_PD)
		   || (PINConfig->PIN > GPIO_PIN_15)
		   || (PINConfig->PORT > GPIO_PORTC)
		   || (PINConfig->SPEED > GPIO_SPEED_VERY_HIGH) )
	{
		RET_GPIOErrorState = Error_InvalidInput;
	}
	else
	{
		LOC_TempRergister = ((GPIO_t *)PINConfig->PORT)->MODER;
		LOC_TempRergister &=~ GPIO_2_BITS_MASK << (PINConfig->PIN * GPIO_PINOFFSET_2BITS_PER_PIN);
		LOC_TempRergister |= (PINConfig->MODE & GPIO_MODE_MASK) << (PINConfig->PIN * GPIO_PINOFFSET_2BITS_PER_PIN);
	    ((GPIO_t *)PINConfig->PORT)->MODER = LOC_TempRergister;

	    LOC_TempRergister = ((GPIO_t *)PINConfig->PORT)->OTYPER;
	    LOC_TempRergister &=~ (1 << PINConfig->PIN );
	    LOC_TempRergister |= ((PINConfig->MODE & GPIO_OUT_TYPE_MASK) >> GPIO_OUT_TYPE_SHIFT) << (PINConfig->PIN);
	    ((GPIO_t *)PINConfig->PORT)->OTYPER = LOC_TempRergister;

	    LOC_TempRergister = ((GPIO_t *)PINConfig->PORT)->PUPDR;
	    LOC_TempRergister &=~ GPIO_2_BITS_MASK << (PINConfig->PIN * GPIO_PINOFFSET_2BITS_PER_PIN);
	    LOC_TempRergister |= ((PINConfig->MODE & GPIO_PUPD_MASK) >> GPIO_PUPD_SHIFT) << (PINConfig->PIN * GPIO_PINOFFSET_2BITS_PER_PIN);
	    ((GPIO_t *)PINConfig->PORT)->PUPDR = LOC_TempRergister;

	    LOC_TempRergister = ((GPIO_t *)PINConfig->PORT)->OSPEEDR;
	    LOC_TempRergister &=~ GPIO_2_BITS_MASK << (PINConfig->PIN * GPIO_PINOFFSET_2BITS_PER_PIN);
	    LOC_TempRergister |= (PINConfig->SPEED) << (PINConfig->PIN * GPIO_PINOFFSET_2BITS_PER_PIN);
	    ((GPIO_t *)PINConfig->PORT)->OSPEEDR = LOC_TempRergister;

	    LOC_TempRegisterAF = ((GPIO_t *)PINConfig->PORT)->AFR;
	    LOC_TempRegisterAF &=~ ((u64)GPIO_4_BITS_MASK) << (PINConfig->PIN * GPIO_PINOFFSET_4BITS_PER_PIN);
	    LOC_TempRegisterAF |= ((u64)PINConfig->AF) << (PINConfig->PIN * GPIO_PINOFFSET_4BITS_PER_PIN);
	    ((GPIO_t *)PINConfig->PORT)->AFR = LOC_TempRegisterAF;

	    RET_GPIOErrorState = GPIO_OK;
	}
	return RET_GPIOErrorState;
}

COM_ErrorStatus_t GPIO_SetPinValue(void * PORT, u32 PIN , u8 State)
{
	COM_ErrorStatus_t RET_GPIOErrorState = GPIO_NOK;
	if(PORT == NULL)
	{
		RET_GPIOErrorState = Error_NULLPointer;
	}
	else if( (GPIO_PIN_15 < PIN || PIN < GPIO_PIN_0)
		  || (GPIO_STATE_HIGH < State || State < GPIO_STATE_LOW)  )
	{
		RET_GPIOErrorState = GPIO_InvalidParameter;
	}
	else
	{
		switch(State)
		{
		case GPIO_STATE_HIGH:
			((GPIO_t *)PORT)->BSRR = (1 << PIN);
			RET_GPIOErrorState = GPIO_OK;
			break;
		case GPIO_STATE_LOW:
			((GPIO_t *)PORT)->BSRR = (1 << (PIN + GPIO_BSRR_RESET) );
			RET_GPIOErrorState = GPIO_OK;
			break;
		default:
			RET_GPIOErrorState = GPIO_NOK;
		}
	}
	return RET_GPIOErrorState;
}

COM_ErrorStatus_t GPIO_GetPinValue(void * PORT, u32 PIN, u8 * ADD_ptrValue)
{
	COM_ErrorStatus_t RET_GPIOErrorState = GPIO_NOK;
	if( (PORT == NULL) || (ADD_ptrValue == NULL) )
	{
		RET_GPIOErrorState = Error_NULLPointer;
	}
	else if(GPIO_PIN_15 < PIN || PIN < GPIO_PIN_0)
	{
		RET_GPIOErrorState = GPIO_InvalidParameter;
	}
	else
	{
		*ADD_ptrValue = ((((GPIO_t *)PORT)->IDR) >> PIN) & 1;
		RET_GPIOErrorState = GPIO_OK;
	}
	return RET_GPIOErrorState;
}

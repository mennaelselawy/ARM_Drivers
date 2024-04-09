#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

#define NULL    (void*)(0)

typedef unsigned char         u8 ;
typedef unsigned short int    u16;
typedef unsigned long  int    u32;
typedef unsigned long  long   u64;
typedef signed   char         s8 ;
typedef signed   short int    s16;
typedef signed   long  int    s32;
typedef float                 f32;
typedef double                f64;


typedef enum
{
	RCC_OK,
	RCC_InvalidSYSCLK,
	RCC_ClockNotReady,
	RCC_ClockNotON,
	RCC_InvalidClock,
	RCC_NotReady,
	RCC_InvalidParameter,
	RCC_InvalidBus,
	RCC_InvalidPeripheral,
	RCC_NOK,
	GPIO_OK,
	GPIO_InvalidParameter,
	GPIO_NOK,
	LED_OK,
	LED_NOK,
	SWITCH_OK,
	SWITCH_NOK,
	NVIC_OK,
	NVIC_NOK,
	SYSTICK_OK,
	SYSTICK_NOK,
	SCHEDULER_OK,
	SCHEDULER_NOK,
	LCD_OK,
	LCD_NOK,
	Error_InvalidInput,
	Error_NULLPointer
}COM_ErrorStatus_t;

#endif

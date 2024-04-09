#ifndef GPIO_H
#define GPIO_H

/* PORTS */
#define	 GPIO_PORTA               ((void *)0x40020000)
#define	 GPIO_PORTB               ((void *)0x40020400)
#define  GPIO_PORTC               ((void *)0x40020800)
/* PINS */
#define GPIO_PIN_0                0x0
#define GPIO_PIN_1                0x1
#define GPIO_PIN_2                0x2
#define GPIO_PIN_3                0x3
#define GPIO_PIN_4                0x4
#define GPIO_PIN_5                0x5
#define GPIO_PIN_6                0x6
#define GPIO_PIN_7                0x7
#define GPIO_PIN_8                0x8
#define GPIO_PIN_9                0x9
#define GPIO_PIN_10               0xA
#define GPIO_PIN_11               0xB
#define GPIO_PIN_12               0xC
#define GPIO_PIN_13               0xD
#define GPIO_PIN_14               0xE
#define GPIO_PIN_15               0xF

                                   /* MASKS   */
// MODES
//INPUT MODES
#define   GPIO_MODE_IN_FL         0x0
#define   GPIO_MODE_IN_PU         0x8
#define   GPIO_MODE_IN_PD         0x10

//OUTPUT TYPE
#define   GPIO_MODE_OUT_PP        0x1
#define   GPIO_MODE_OUT_PP_PU     0x9
#define   GPIO_MODE_OUT_PP_PD     0x11
#define   GPIO_MODE_OUT_OD        0x5
#define   GPIO_MODE_OUT_OD_PU     0xD
#define   GPIO_MODE_OUT_OD_PD     0x15

//Alternate functions
#define   GPIO_MODE_AF_PP         0x2
#define   GPIO_MODE_AF_PP_PU      0xA
#define   GPIO_MODE_AF_PP_PD      0x12
#define   GPIO_MODE_AF_OD         0x6
#define   GPIO_MODE_AF_OD_PU      0xE
#define   GPIO_MODE_AF_OD_PD      0x16

//Analog
#define   GPIO_MODE_ANALOG        0x3

//speed
#define   GPIO_SPEED_LOW          0x0
#define   GPIO_SPEED_MEDIUM       0x1
#define   GPIO_SPEED_HIGH         0x2
#define   GPIO_SPEED_VERY_HIGH    0x3

#define GPIO_AF_SYSTEM            0x0
#define GPIO_AF_TIM_1_2           0x1
#define GPIO_AF_TIM_3_4_5         0x2
#define GPIO_AF_TIM_9_11          0x3
#define GPIO_AF_I2C1_3            0x4
#define GPIO_AF_SPI1_4            0x5
#define GPIO_AF_SPI3              0x6
#define GPIO_AF_USART_1_2         0x7
#define GPIO_AF_USART6            0x8
#define GPIO_AF_I2C_2_3           0x9
#define GPIO_AF_OTG_FS            0xA
#define GPIO_AF_SDIO              0xC
#define GPIO_AF_EVENTOUT          0xF

#define GPIO_STATE_HIGH           1
#define GPIO_STATE_LOW            0

typedef struct
{
	void* PORT;
	u32 PIN;
	u32 MODE;
	u32 SPEED;
	u32 AF;
}GPIO_strConfig_t;

COM_ErrorStatus_t GPIO_Init(GPIO_strConfig_t* PINConfig);
COM_ErrorStatus_t GPIO_SetPinValue(void * PORT, u32 PIN , u8 State);
COM_ErrorStatus_t GPIO_GetPinValue(void * PORT, u32 PIN, u8 * ADD_ptrValue);

#endif

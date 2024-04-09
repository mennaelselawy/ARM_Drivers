
                                               /*#defines*/

#define SYSCLK_HSI          0x0
#define SYSCLK_HSE          0x1
#define SYSCLK_PLL          0x2

//BUSES
#define   BUS_AHB1        0
#define   BUS_AHB2        1
#define   BUS_APB1        2
#define   BUS_APB2        3

//AHB1 peripherals
#define   PERIPHERAL_GPIOA_AHB1          0x1
#define   PERIPHERAL_GPIOB_AHB1          0x2
#define   PERIPHERAL_GPIOC_AHB1          0x4
#define   PERIPHERAL_GPIOD_AHB1          0x8
#define   PERIPHERAL_GPIOE_AHB1          0x10
#define   PERIPHERAL_GPIOH_AHB1          0x80
#define   PERIPHERAL_CRC_AHB1            0x1000
#define   PERIPHERAL_DMA1_AHB1           0x200000
#define   PERIPHERAL_DMA2_AHB1           0x400000

//AHB2 peripherals
#define   PERIPHERAL_USB_OTGFS_AHB2      0x80

//APB1 peripherals
#define   PERIPHERAL_TIM2_APB1           0x1
#define   PERIPHERAL_TIM3_APB1           0x2
#define   PERIPHERAL_TIM4_APB1           0x4
#define   PERIPHERAL_TIM5_APB1           0x8
#define   PERIPHERAL_WWDG_APB1           0x800
#define   PERIPHERAL_SPI2_APB1           0x4000
#define   PERIPHERAL_SPI3_APB1           0x8000
#define   PERIPHERAL_USART2_APB1         0x20000
#define   PERIPHERAL_I2C1_APB1           0x200000
#define   PERIPHERAL_I2C2_APB1           0x400000
#define   PERIPHERAL_I2C3_APB1           0x800000
#define   PERIPHERAL_PWR_APB1            0x10000000


//APB2 peripherals
#define   PERIPHERAL_TIM1_APB2           0x1
#define   PERIPHERAL_USART1_APB2         0x10
#define   PERIPHERAL_USART6_APB2         0x20
#define   PERIPHERAL_ADC1_APB2           0x100
#define   PERIPHERAL_SDIO_APB2           0x800
#define   PERIPHERAL_SPI1_APB2           0x1000
#define   PERIPHERAL_SPI4_APB2           0x2000
#define   PERIPHERAL_SYSCFG_APB2         0x4000
#define   PERIPHERAL_TIM9_APB2           0x10000
#define   PERIPHERAL_TIM10_APB2          0x20000
#define   PERIPHERAL_TIM11_APB2          0x40000

                                       /*#types*/


typedef enum
{
	CLOCK_HSI,
	CLOCK_HSE,
	CLOCK_PLL,
	CLOCK_PLL2,
	CLOCK_LSE,
	CLOCK_LSI
}RCC_Clock_t;

typedef enum
{
	Disable,
	Enable
}RCC_PeripheralStatus_t;

                                             /*API'S*/
/*
 * Brief: Select which System Clock will provide to the processor
 * Parameters: SYSCLK options: SYSCLK_HSI , SYSCLK_HSE , SYSCLK_PLL
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes: The selected Clock has to be enabled first
 */
COM_ErrorStatus_t RCC_SelectSystemClock(u8 SYSCLK);
/*
 * Brief: Set Clock ON
 * Parameters: six clock options: CLOCK_HSI, CLOCK_HSE, CLOCK_PLL, CLOCK_PLL2, CLOCK_LSE, CLOCK_LSI.
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:Once PLL or PLL2 are enabled, they can't be configured
 */
COM_ErrorStatus_t RCC_EnableClock(RCC_Clock_t CLOCK);
/*
 * Brief: Set Clock OFF
 * Parameters: six clock options: CLOCK_HSI, CLOCK_HSE, CLOCK_PLL, CLOCK_PLL2, CLOCK_LSE, CLOCK_LSI.
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes: Can't set the Clock OFF if it is the System clock
 */
COM_ErrorStatus_t RCC_DisableClock(RCC_Clock_t CLOCK);
/*
 * Brief: Set Source Clock for the PLL
 * Parameters: PLL Source options: SYSCLK_HSI , SYSCLK_HSE
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_SetPLLSource(u8 SYSCLK);
/*
 * Brief: Set the
 * Parameters:
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_ConfigurePLL(u8 PLLM, u8 PLLN,u8 PLLP,u8 PLLQ);
/*
 * Brief:
 * Parameters:
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_SetAHBPrescaler(u32 AHBPrescale);
/*
 * Brief:
 * Parameters:
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_SetAPB1Prescaler(u32 APB1Prescale);
/*
 * Brief:
 * Parameters:
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_SetAPB2Prescaler(u32 APB2Prescale);
/*
 * Brief:
 * Parameters:
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_CheckReady(u8 CLOCK, u8* Add_pReadyState);
/*
 * Brief:
 * Parameters:
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_EnablePeripheral(u32 BUS,u32 Peripheral);
/*
 * Brief:
 * Parameters:
 * Return: Error Status for the output ,NOK not ok if there is a problem.
 * Notes:
 */
COM_ErrorStatus_t RCC_DisablePeripheral(u32 BUS,u32 Peripheral);


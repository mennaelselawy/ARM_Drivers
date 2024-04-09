/*
 * Systick_Config.h
 *
 *  Created on: Mar 23, 2024
 *      Author: Menna Ashraf
 */

#ifndef SYSTICK_CONFIG_H_
#define SYSTICK_CONFIG_H_

/* SYSTEM CLOCKES:
 * HSI: 16 MHZ
 * HSE: 25 MHZ
 **/

#define SYSTICK_CLOCK_PROCESSOR_AHB            16000000   //for HSI default
#define SYSTICK_CLOCK_EXTERNAL_AHBdiv8         2000000    // HSI/8

#endif

/*
 * Scheduler_Config.c
 *
 *  Created on: Mar 31, 2024
 *      Author: Menna Ashraf
 */

#include "../include/LIB/STD_TYPES.h"
#include "../include/Scheduler.h"
#include "../include/Scheduler_Config.h"

extern void Switch_One_CallBack(void);
extern void Switch_Two_CallBack(void);
extern void LCD_CallBack(void);

RUNNABLE_t  RUNNABLES[Num_Of_Runnables] = {
		[Switch_1] = {.name="Switch One", .priority = Switch_1, .priodicity = 500, .FirstDelay = 0, .CallBack = Switch_One_CallBack, .TaskRunState= = Running},
		[Switch_2] = {.name="Switch Two", .priority = Switch_2, .priodicity = 300, .FirstDelay = 0, .CallBack = Switch_Two_CallBack, .TaskRunState= = Running},
		[LCD] = { .name="LCD", .priority = LCD,	priodicity = 2, .FirstDelay = 0, .CallBack = LCD_CallBack, .TaskRunState= = Running},
};

/*
 * Scheduler.h
 *
 *  Created on: Mar 31, 2024
 *      Author: Menna Ashraf
 */
#ifndef SCHEDULER_CONFIG_H
#define SCHEDULER_CONFIG_H

typedef enum{
	Switch_1,
	Switch_2,
	LCD,

	Num_Of_Runnables
}RUNNABLES_t;

#define  SCHEDULER_TickTime_MS  2

#endif

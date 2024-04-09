/*
 * Scheduler.h
 *
 *  Created on: Mar 31, 2024
 *      Author: Menna Ashraf
 */
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

typedef  void (*RunnableCB_t) (void);

typedef struct{
	char * name;
	u32 priority;
	u32 priodicity;
	u32 FirstDelay;
	RunnableCB_t CallBack;
	TaskState_t TaskRunState;
}RUNNABLE_t;

typedef struct{
	const RUNNABLE_t *Runnable;
	u32 TimeTaskMS;
}RunnableInfo_t;

typedef enum{
	Suspend,
	Running
}TaskState_t;

void Scheduler_Init(void);
COM_ErrorStatus_t Scheduler_StartOS();
//COM_ErrorStatus_t Scheduler();

//COM_ErrorStatus_t Scheduler_CreateTask();
COM_ErrorStatus_t Scheduler_DeleteRunnable(u32 Copy_priority);
COM_ErrorStatus_t Scheduler_SuspendRunnable(u32 Copy_priority);
COM_ErrorStatus_t Scheduler_ResumeRunnable(u32 Copy_priority);

#endif

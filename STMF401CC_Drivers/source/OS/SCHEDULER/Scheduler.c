/*
 * Scheduler.h
 *
 *  Created on: Mar 31, 2024
 *      Author: Menna Ashraf
 */

#include "../include/LIB/STD_TYPES.h"
#include "../include/SYSTICK/Systick.h"
#include "../include/SYSTICK/Systick_Config.h"
#include "../include/Scheduler.h"
#include "../include/Scheduler_Config.h"

extern  RUNNABLE_t RUNNABLES[Num_Of_Runnables];

RunnableInfo_t RUNNABLES_INFO[Num_Of_Runnables];

static volatile u32 G_PendingTicks = 1;

RUNNABLE_t EmptyRunnable = {0};

static void SCHEDULER_IncrementTickCB(void){
	G_PendingTicks ++;
}

static void Scheduler(void)
{
	u32 LOC_Index = 0;
	for(LOC_Index = 0; LOC_Index < Num_Of_Runnables; LOC_Index++)
	{
		if(RUNNABLES_INFO[LOC_Index].Runnable->CallBack && RUNNABLES_INFO[LOC_Index].TimeTaskMS == 0 && RUNNABLES_INFO[LOC_Index].Runnable->TaskRunState == Running)
		{
			RUNNABLES_INFO[LOC_Index].Runnable->CallBack();
			RUNNABLES_INFO[LOC_Index].TimeTaskMS = RUNNABLES_INFO[LOC_Index].Runnable->priodicity;
		}
		else
		{
			RUNNABLES_INFO[LOC_Index].TimeTaskMS -= SCHEDULER_TickTime_MS;
		}
	}
}

void Scheduler_Init(void)
{
	u32 LOC_Index = 0;
	for(LOC_Index = 0; LOC_Index < Num_Of_Runnables; LOC_Index++)
	{
		RUNNABLES_INFO[LOC_Index].Runnable = &RUNNABLES[LOC_Index];
		RUNNABLES_INFO[LOC_Index].TimeTaskMS = RUNNABLES_INFO[LOC_Index].Runnable->FirstDelay;
	}
	Systick_SetTimeMS(SCHEDULER_TickTime_MS);
	Systick_SetCallBack(SCHEDULER_IncrementTickCB);
}

void Scheduler_StartOS()
{
	Systick_Start(SYSTICK_MODE_PERIODIC);
	while(1)
	{
		if(G_PendingTicks)
		{
			G_PendingTicks --;
			Scheduler();
		}
	}

}

COM_ErrorStatus_t Scheduler_DeleteRunnable(u32 Copy_priority)
{
	COM_ErrorStatus_t RET_ErrorStatus = SCHEDULER_NOK;
	if(Copy_priority < Num_Of_Runnables && RUNNABLES_INFO[Copy_priority].Runnable->CallBack != 0)
	{
		RUNNABLES_INFO[Copy_priority].Runnable = EmptyRunnable;
		RET_ErrorStatus = SCHEDULER_OK;
	}
	else
	{
		RET_ErrorStatus = SCHEDULER_NOK;
	}
	return RET_ErrorStatus;
}

COM_ErrorStatus_t Scheduler_SuspendRunnable(u32 Copy_priority)
{
	COM_ErrorStatus_t RET_ErrorStatus = SCHEDULER_NOK;
	if(Copy_priority < Num_Of_Runnables && RUNNABLES_INFO[Copy_priority].Runnable->CallBack != 0)
	{
		RUNNABLES_INFO[Copy_priority].Runnable->TaskRunState = Suspend;
		RET_ErrorStatus = SCHEDULER_OK;
	}
	else
	{
		RET_ErrorStatus = SCHEDULER_NOK;
	}
	return RET_ErrorStatus;
}

COM_ErrorStatus_t Scheduler_ResumeRunnable(u32 Copy_priority)
{
	COM_ErrorStatus_t RET_ErrorStatus = SCHEDULER_NOK;
	if(Copy_priority < Num_Of_Runnables && RUNNABLES_INFO[Copy_priority].Runnable->CallBack != 0)
	{
		RUNNABLES_INFO[Copy_priority].Runnable->TaskRunState = Running;
		RET_ErrorStatus = SCHEDULER_OK;
	}
	else
	{
		RET_ErrorStatus = SCHEDULER_NOK;
	}
	return RET_ErrorStatus;
}

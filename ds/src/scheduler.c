/******************************************************************************

		Scheduler
		Reviewer : Daria
		Date: 7 / 1 / 25 

******************************************************************************/



#include <stdlib.h>		/* malloc free 	*/
#include <time.h>		/* time() 		*/
#include <assert.h> 	/* assert		*/
#include <unistd.h> 	/* sleep 		*/

#include "scheduler.h"
#include "pqueue.h"
#include "uid.h"
#include "task.h"
/* -------------------------------------------------------------------------*/
#define UNUSED(x) 	((void)(x)) 

#define REMOVED 	0
#define NOT_FOUND 	1
/* -------------------------------------------------------------------------*/
typedef enum sch_bool
{
    TRUE = 1,
	FALSE = 0
} sch_bool_ty;
/* -------------------------------------------------------------------------*/
typedef enum func_to_exe_ty
{
	NONE = 0,
	REMOVE = 1,
	CLEAN = 2
}func_to_exe_ty;
/* -------------------------------------------------------------------------*/
struct scheduler
{
	pq_ty* pq;	
	sch_bool_ty is_run;
	func_to_exe_ty func_to_exe;
	task_ty* cur_task;
};
/* -------------------------------------------------------------------------*/
static int SchIsMatchID(const void* data, void* param);
static int ComapreTask(const void* data1, const void* data2, void* param);
static void SchedulerClearQueue(scheduler_ty* sch);
/* -------------------------------------------------------------------------*/
scheduler_ty* SchedulerCreate(void)
{
	pq_ty* new_pq = NULL;
	scheduler_ty* new_sch = (scheduler_ty*)malloc(sizeof(scheduler_ty));

	if (NULL == new_sch)
	{
		return NULL;
	}

	new_pq = PQCreate(ComapreTask, NULL);
	if (NULL == new_pq)
	{
		free(new_sch);
		new_sch = NULL;
	}
	new_sch->pq = new_pq;
	new_sch->is_run = FALSE;
	new_sch->func_to_exe = NONE;
	new_sch->cur_task = NULL;

	return new_sch;
}
/* ------------------------------------------------------------------------- */
void SchedulerDestroy(scheduler_ty* sch)
{
	task_ty* cur_task = NULL;

	if (NULL == sch)
	{
		return;
	}

	SchedulerClearQueue(sch);

	PQDestroy(sch->pq);
	sch->pq = NULL;
	free(sch);
	sch = NULL;
}
/* ------------------------------------------------------------------------- */
uid_ty SchedulerAddTask( scheduler_ty* sch, unsigned int interval,
                         scheduler_op_ty op_func, void* op_param,
                         scheduler_cleanup_ty clean_func, void* clean_param )
{
	task_ty* new_task = NULL;
	pq_status_ty enqueue_ret = PQ_FAIL;

	assert(NULL != sch);
	assert(NULL != op_func);
	assert(NULL != clean_func);

	new_task = TaskCreate((task_op_ty)op_func,
						 op_param,
						 (task_cleanup_ty)clean_func,
						 clean_param, interval);
	if (NULL == new_task)
	{
		return invalid_uid_g;
	}

	enqueue_ret = PQEnqueue(sch->pq, new_task);

	if (PQ_FAIL == enqueue_ret)
	{
		TaskDestroy(new_task);
		new_task = NULL;
		return invalid_uid_g;
	}
	
	return TaskGetUID(new_task);
}
/* ------------------------------------------------------------------------- */
int SchedulerRemoveTask(scheduler_ty* sch, uid_ty task_id)
{
	task_ty* task_removed = NULL;
	assert(NULL != sch);

	if(sch->is_run && NONE == sch->func_to_exe && SchIsMatchID(sch->cur_task, &task_id))
	{
		sch->func_to_exe = REMOVE;
		return REMOVED;
	}

	sch->func_to_exe = NONE;

	task_removed = (task_ty*)PQRemove(sch->pq, 
				   					(pq_is_match_ty)SchIsMatchID,
				   			 		 &task_id);
	if (NULL == task_removed)
	{
		return NOT_FOUND;
	}
	
	TaskDestroy(task_removed);
	return REMOVED;
}

/* ------------------------------------------------------------------------- */
size_t SchedulerSize(const scheduler_ty* sch)
{
	assert(NULL != sch);
	return PQCount(sch->pq);
}
/* ------------------------------------------------------------------------- */
sch_run_status_ty SchedulerRun(scheduler_ty* sch)
{
	task_ty* first_task = NULL;
	time_t first_time = (time_t)0;
	time_t curr_time = (time_t)0;
	pq_status_ty enqueue_status = PQ_FAIL;
	int pause = 0;
	task_op_status_ty first_op_status = TSK_NOT_REPEAT;
 
	assert(NULL != sch);

	sch->is_run = TRUE;
	while(sch->is_run && !PQIsEmpty(sch->pq))
	{
		switch(sch->func_to_exe)
		{
			case NONE:
				break;

			case REMOVE:
				SchedulerRemoveTask(sch, TaskGetUID(sch->cur_task));
				continue;

			case CLEAN:
				SchedulerClear(sch);
				continue;
		}

		sch->func_to_exe = NONE;

		first_task = (task_ty*)PQPeek(sch->pq);
		sch->cur_task = first_task;
		first_time = TaskGetTimeToRun(first_task);

		time(&curr_time);
		if ((time_t)-1 == curr_time)
		{
			sch->is_run = FALSE;
			return SCH_FAIL;
		}

		pause = difftime(first_time, curr_time);
		
		while(pause > 0)
		{
			pause = (int)sleep(pause);
		}

		sch->cur_task = first_task;
		first_op_status = TaskExecute(first_task);

		first_task = PQDequeue(sch->pq);

		if (TSK_NOT_REPEAT == first_op_status)
		{
			TaskDestroy(first_task);
			first_task = NULL;
			sch->cur_task = NULL;
			sch->func_to_exe = (sch->func_to_exe == REMOVE) ? NONE : sch->func_to_exe;
			continue;
		}
		
		TaskUpdateTimeToRun(first_task);
		enqueue_status = PQEnqueue(sch->pq, first_task);
		if (PQ_FAIL == enqueue_status)
		{
			return SCH_FAIL;
		}		
	}
	
	if(!sch->is_run)
	{
		return SCH_STOP;
	}

	return SCH_SUCCESS;
}
/* ------------------------------------------------------------------------- */
void SchedulerStop(scheduler_ty* sch)
{
	assert(NULL != sch);
	
	sch->is_run = FALSE;
}
/* ------------------------------------------------------------------------- */
void SchedulerClear(scheduler_ty* sch)
{
	task_ty* cur_task = NULL;
	assert(NULL != sch);

	if(NONE == sch->func_to_exe && sch->is_run)
	{
		sch->func_to_exe = CLEAN;
		return;
	}
	sch->func_to_exe = NONE;
	SchedulerClearQueue(sch);
}
/* ------------------------------------------------------------------------- */
static void SchedulerClearQueue(scheduler_ty* sch)
{
	task_ty* cur_task = NULL;

	assert(NULL != sch);

	while (!PQIsEmpty(sch->pq))
	{
		cur_task = PQDequeue(sch->pq);
		TaskDestroy(cur_task);
	}
}
/* ------------------------------------------------------------------------- */
static int SchIsMatchID(const void* cur_task, void* id_to_remove)
{
	assert(NULL != cur_task);
	assert(NULL != id_to_remove);
	return IsMatchId(TaskGetUID((task_ty*)cur_task), *(uid_ty*)id_to_remove);	
}
/* ------------------------------------------------------------------------- */
static int ComapreTask(const void* data1, const void* data2, void* param)
{
	assert(NULL != data1);
	assert(NULL != data2);
	UNUSED(param);
	
	return (int)difftime(TaskGetTimeToRun((task_ty*)data1),
		    		     TaskGetTimeToRun((task_ty*)data2));
}



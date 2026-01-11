#ifndef __ILRD_SCHEDULER_H__
#define __ILRD_SCHEDULER_H__

#include <stddef.h>	/* size_t */

#include "uid.h"

typedef struct scheduler scheduler_ty;

typedef enum sch_run_status
{
    SCH_SUCCESS = 0,
    SCH_FAIL = 1,
    SCH_STOP = 2
} sch_run_status_ty;

typedef enum sch_op_status
{
    SCH_REPEAT = 0,
	SCH_NOT_REPEAT = 1
} sch_op_status_ty;

/******************************************************************************
*	Description :
*		Performs task operation
*	Return Value:
* 		SCH_REPEAT = 0 (task will repeat in the next interval)
*       SCH_NOT_REPEAT = 1 (task will be removed and cleaned)
******************************************************************************/
typedef sch_op_status_ty (*scheduler_op_ty)(void* param);

/******************************************************************************
*	Description:
*		Performs task cleanup if operation return value is SCH_NOT_REPEAT
******************************************************************************/
typedef void (*scheduler_cleanup_ty)(void* param);

/******************************************************************************
*	Description:
*		Create a new scheduler instance
*	Return Value:
* 		On success, a pointer to the created scheduler
* 		On failure, NULL
* 	O(1)
******************************************************************************/
scheduler_ty* SchedulerCreate(void);

/****************************************************************************** 
* 	Description:
* 		free all memory usage of "sch"
* 		No operations performed if "sch" is NULL
* 	Return value:
* 		None
*   O(n)
******************************************************************************/
void SchedulerDestroy(scheduler_ty* sch);

/****************************************************************************** 
* 	Description:
* 		Insert a new task to scheduler, performed on constant "interval" 
* 	Return value:	
* 		task uid upon success, invalid_uid_g upon failure
* 	Undefined behaviour:
* 		scheduler is NULL
* 		op_func is NULL
* 		clean_func is NULL
*   O(1)
******************************************************************************/
uid_ty SchedulerAddTask( scheduler_ty* sch, unsigned int interval,
                         scheduler_op_ty op_func, void* op_param,
                         scheduler_cleanup_ty clean_func, void* clean_param );

/****************************************************************************** 
*   Description:
* 		Remove task from a scheduler
*   Return value:
*       0 on success, 1 if task_id wasn't found
* 	Undefined behavior: 
* 		task_id is invalid_uid_g
*       sch is NULL
*   O(n)
******************************************************************************/
int SchedulerRemoveTask(scheduler_ty* sch, uid_ty task_id);

/****************************************************************************** 
* 	Description:
* 		Counts the number of tasks in "sch"	  
* 	Return value: 
* 		Count of tasks in "sch"
* 	Undefined behavior:
* 		sch is NULL
*   O(n)
******************************************************************************/
size_t SchedulerSize(const scheduler_ty* sch);

/****************************************************************************** 
* 	Description:
* 		Run sch tasks
* 	Return value: 
* 		SCH_SUCCESS = 0 (scheduler finished running successfully)
*       SCH_FAIL = 1 (one or more tasks failed)
*       SCH_STOP = 2 (sch was stopped, sch may still contain unperformed tasks)
* 	Undefined behavior:
* 		sch is NULL
*   O(n)
******************************************************************************/
sch_run_status_ty SchedulerRun(scheduler_ty* sch);

/****************************************************************************** 
* 	Description:
* 		Change the state of the scheduler to SCH_STOP
* 	Return value: 
* 		None
* 	Undefined behavior:
* 		sch is NULL
*   O(1)
******************************************************************************/
void SchedulerStop(scheduler_ty* sch);

/****************************************************************************** 
* 	Description:
* 		Remove all the tasks in the scheduler
* 	Return value: 
* 		None
* 	Undefined behavior:
* 		sch is NULL
*   O(n)
******************************************************************************/
void SchedulerClear(scheduler_ty* sch);

#endif /* __ILRD_SCHEDULER_H__*/

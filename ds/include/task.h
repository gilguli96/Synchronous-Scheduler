#ifndef __ILRD_TASK_H_
#define __ILRD_TASK_H_

#include <time.h> /* time_t */

#include "uid.h"

typedef enum task_op_status
{
	    TSK_REPEAT = 0,
        TSK_NOT_REPEAT = 1
} task_op_status_ty;

typedef enum task_bool
{
    TSK_FALSE = 0,
    TSK_TRUE = 1
} task_bool_ty;

typedef struct task task_ty;
typedef task_op_status_ty (*task_op_ty) (void* param);
typedef void (*task_cleanup_ty) (void* param);

/******************************************************************************
*	Description:
*		Create a new task
*	Return Value:
* 		On success, new task instance
		On failure, NULL
*   Undefined behaviour:
* 		op or cl are NULL
*   O(1)
******************************************************************************/
task_ty* TaskCreate( task_op_ty op, void* op_param,
                     task_cleanup_ty cl, void* cl_param,
                     unsigned int interval          );

/****************************************************************************** 
* 	Description:
* 		free all memory usage of "tsk"
* 		No operations performed if "tsk" is NULL
* 	Return value:
* 		None
*   O(1)
******************************************************************************/
void TaskDestroy(task_ty* tsk);

/******************************************************************************
*	Description:
*		return task uid
*   Undefined behaviour:
* 		tsk is NULL
*   O(1)
******************************************************************************/
uid_ty TaskGetUID(const task_ty *tsk);

/******************************************************************************
*	Description:
*		Perform task operation
*   Return value:
*       TSK_REPEAT = 0 (task will be rescheduled)
*       TSK_NOT_REPEAT = 1 (task will be removed after completion)
*   Undefined behavior:
* 		tsk is NULL
*   O(1)
******************************************************************************/
task_op_status_ty TaskExecute(task_ty* tsk);

/******************************************************************************
*	Description:
*		return task time_to_run
*   Undefined behaviour:
* 		tsk is NULL
*   O(1)
******************************************************************************/
time_t TaskGetTimeToRun(const task_ty* tsk);

/******************************************************************************
*	Description:
*		Update task time_to_run by adding task interval to current
*		time_to_run
*   Undefined behaviour:
* 		tsk is NULL
*   O(1)
******************************************************************************/
void TaskUpdateTimeToRun(task_ty* tsk);

/****************************************************************************** 
*	Description:
*       Are "tsk1" and "tsk2" equal
*	Return value:
*       TSK_TRUE if tasks are equal, TSK_FALSE otherwise
*   Undefined behavior:
*       tsk1 or tsk2 are NULL 
******************************************************************************/
task_bool_ty TaskIsMatch(const task_ty* tsk1, const task_ty* tsk2);

#endif /* __ILRD_TASK_H_ */

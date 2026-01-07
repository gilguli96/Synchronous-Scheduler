/******************************************************************************

		Task
		Reviewer : Shir
		Date: 7 / 1 / 25 

******************************************************************************/


#include <time.h>   /* time() */
#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc free */

#include "uid.h"
#include "task.h"

struct task
{
    uid_ty uid;
    task_op_ty op_func;
    void* op_param;
    task_cleanup_ty cleanup_func;
    void* clean_param;
    unsigned int interval;
    time_t time_to_run;
};


task_ty* TaskCreate( task_op_ty op, void* op_param,
                     task_cleanup_ty cl, void* cleanup_param,
                     unsigned int interval                          )
{
	task_ty* new_task = NULL;

	assert(NULL != op);
	assert(NULL != cl);

	new_task = (task_ty*)malloc(sizeof(task_ty));

	if (NULL == new_task)
	{
		return NULL;
	}

	new_task->uid = UidCreate();

	if ( IsMatchId(invalid_uid_g, new_task->uid) )
	{
		free(new_task);
		new_task = NULL;
		return NULL;
	}
	
	new_task->op_func = op;
	new_task->op_param = op_param;
	new_task->cleanup_func = cl;
	new_task->clean_param = cleanup_param;
	new_task->interval = interval;
	new_task->time_to_run = time(NULL) + interval;

	return new_task;
}

op_status_ty TaskExecute(task_ty* tsk)
{
	assert(NULL != tsk);

	return tsk->op_func(tsk->op_param);
}

void TaskUpdateTimeToRun(task_ty* tsk)
{
	assert(NULL != tsk);

	tsk->time_to_run += tsk->interval;
}

void TaskDestroy(task_ty* tsk)
{
	assert(NULL != tsk);

	tsk->cleanup_func(tsk->clean_param);
	free(tsk);
	tsk = NULL;
}

uid_ty TaskGetUid(task_ty* tsk)
{
	assert(NULL != tsk);

	return tsk->uid;
}

time_t TaskGetTimeToRun(const task_ty* tsk)
{
	assert(NULL != tsk);

	return tsk->time_to_run;
}

bool_ty TaskIsMatch(const task_ty* tsk1, const task_ty* tsk2)
{
	assert(NULL != tsk1);
	assert(NULL != tsk2);

	return (bool_ty)IsMatchId(tsk1->uid, tsk2->uid);
}


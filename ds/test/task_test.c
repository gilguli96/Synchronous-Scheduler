#include <stdio.h>  	/* 	printf 		 */
#include <stddef.h> 	/* 	size_T 		 */
#include <stdlib.h> 	/* 	malloc free  */
#include <unistd.h>		/*	sleep		*/

#include "task.h"

typedef struct OpParam
{
	size_t num_of_operations;

}op_param_ty;


/* -------------------------------------------------------------------------
						Test Functions Declarations
   ------------------------------------------------------------------------- */
static void TestTaskCreateDestroy(void);
static void TestTaskIsMatch(void);
static void TestTaskUpdateGetExecute(void);


/* -------------------------------------------------------------------------
						Helper Functions Declarations
   ------------------------------------------------------------------------- */
static op_param_ty* CreateOpParam(size_t num_of_operations);
static op_status_ty PrintOp(void* param);
static void CleanOpParam(void* param);

/* -------------------------------------------------------------------------
   									Main
   ------------------------------------------------------------------------- */
int main()
{
	TestTaskCreateDestroy();
	TestTaskIsMatch();
	TestTaskUpdateGetExecute();
	return 0;
}


/* -------------------------------------------------------------------------
						Test Functions Implementations
   ------------------------------------------------------------------------- */
static void TestTaskCreateDestroy(void)
{
	
	op_param_ty* my_op_param = CreateOpParam(3);
	task_ty* my_task = TaskCreate(PrintOp, my_op_param, CleanOpParam, my_op_param, 2);
	if (NULL == my_task)
	{
		printf("TaskCreate Failed! returned NULL!\n\n");
	}
	TaskDestroy(my_task);
	my_task = NULL;
	printf("Finished TestTaskCreateDestroy check vlg! \n\n");
}
/* ------------------------------------------------------------------------- */
static void TestTaskIsMatch(void)
{
	op_param_ty* my_op_param1 = CreateOpParam(3);
	op_param_ty* my_op_param2 = CreateOpParam(3);
	task_ty* my_task1 = TaskCreate(PrintOp, my_op_param1, CleanOpParam, my_op_param1, 2);
	task_ty* my_task2 = TaskCreate(PrintOp, my_op_param2, CleanOpParam, my_op_param2, 2);

	if (TaskIsMatch(my_task1, my_task2))
	{
		printf("Expected IsMatch to Return False task1 != task2\n\n");
	}
	if (!TaskIsMatch(my_task1, my_task1))
	{
		printf("Expected IsMatch to Return True task1 == task1\n\n");
	}
	
	printf("Finished TestTaskIsMatch check vlg! \n\n");
	TaskDestroy(my_task1);
	my_task1 = NULL;
	TaskDestroy(my_task2);
	my_task2 = NULL;
}
/* ------------------------------------------------------------------------- */
static void TestTaskUpdateGetExecute(void)
{
	op_param_ty* my_op_param1 = CreateOpParam(3);
	long get_time_res = 0;
	unsigned int my_interval = 2;
	op_status_ty task_exe_ret = REPEAT;
	task_ty* my_task1 = TaskCreate(PrintOp, my_op_param1, CleanOpParam, 
									my_op_param1, my_interval);

	get_time_res = (long)TaskGetTimeToRun(my_task1);
	if (0 != get_time_res - (time(NULL) + my_interval))
	{
		printf("Got: %ld Expected: 0\n\n", get_time_res - (time(NULL) + 2));
		/* in debugger run this will allways be false! */
	}
	while (NOT_REPEAT != task_exe_ret)
	{
		sleep(get_time_res - time(NULL));
		task_exe_ret = TaskExecute(my_task1);
		TaskUpdateTimeToRun(my_task1);
		get_time_res = (long)TaskGetTimeToRun(my_task1);
	}


	TaskDestroy(my_task1);
	my_task1 = NULL;
}


/* -------------------------------------------------------------------------
						Helper Functions Implementations
   ------------------------------------------------------------------------- */
static op_status_ty PrintOp(void* param)
{
	op_param_ty* my_op_param = (op_param_ty*)param;

	if(NULL == param)
	{
		return NOT_REPEAT;
	}

    if(my_op_param->num_of_operations == 0)
	{
		return NOT_REPEAT;
	}

	printf("Ehud Is Geh!\n\n");
	--(my_op_param->num_of_operations);
	return REPEAT;
}

static op_param_ty* CreateOpParam(size_t num_of_operations)
{
	op_param_ty* new_op_param = (op_param_ty*)malloc(sizeof(op_param_ty));
	/* no check if null returned because its test */
	new_op_param->num_of_operations = num_of_operations;
	return new_op_param;
}

static void CleanOpParam(void* param)
{
	free((op_param_ty*)param);
	param = NULL;
}

















/******************************************************************************

		Task
		Reviewer : Shir
		Date: 7 / 1 / 25 

******************************************************************************/


#include <stdio.h>  	/* 	printf 		 */
#include <stddef.h> 	/* 	size_T 		 */
#include <stdlib.h> 	/* 	malloc free  */

#include "task.h"
#include "scheduler.h"

typedef struct OpParam
{
	size_t num_of_operations;
	scheduler_ty* sch;
	uid_ty task_id;

}op_param_ty;


/* -------------------------------------------------------------------------
						Test Functions Declarations
   ------------------------------------------------------------------------- */
static void TestSchedulerCreateDestory(void);
static void TestSchedulerAddRemove(void);
static void TestSchedulerRunAddStop(void);
static void TestSchedulerRunClear(void);
static void TestSchedulerRunStopClear(void);



/* -------------------------------------------------------------------------
						Helper Functions Declarations
   ------------------------------------------------------------------------- */
static op_param_ty* CreateOpParam(size_t num_of_operations, scheduler_ty* sch ,uid_ty task_id);
static sch_op_status_ty RemoveSelf(void* param);
static sch_op_status_ty RemoveFriend(void* param);
static sch_op_status_ty PrintOp(void* param);
static sch_op_status_ty StopOp(void* param);
static sch_op_status_ty ClearOp(void* param);
static void CleanOpParam(void* param);

/* -------------------------------------------------------------------------
   									Main
   ------------------------------------------------------------------------- */
int main()
{
	/*TestSchedulerCreateDestory();*/
	/*TestSchedulerAddRemove();*/
	TestSchedulerRunAddStop();
	TestSchedulerRunClear();
	
	return 0;
}


/* -------------------------------------------------------------------------
						Test Functions Implementations
   ------------------------------------------------------------------------- */
static void TestSchedulerCreateDestory(void)
{
	
	scheduler_ty* my_sch = SchedulerCreate();
	op_param_ty* my_op_param = CreateOpParam(3, my_sch, invalid_uid_g);
	uid_ty cur_uid = invalid_uid_g;

	if (NULL == my_sch)
	{
		printf("SchedulerCreate Failed! returned NULL!\n\n");
	}
	
	if (0 != SchedulerSize(my_sch))
	{
		printf("SchedulerSize failed! should be 0 first!\n\n");
	}

	cur_uid = SchedulerAddTask(my_sch, 2, RemoveSelf, my_op_param, CleanOpParam, my_op_param);
	if (IsMatchId(invalid_uid_g, cur_uid))
	{
		printf("SchedulerAddTask failed! returned invalid uid!\n\n");
	}

	if (1 != SchedulerSize(my_sch))
	{
		printf("SchedulerSize failed! should be 1 after add!\n\n");
	}
	
	SchedulerRun(my_sch);
	
	if (0 != SchedulerSize(my_sch))
	{
		printf("SchedulerSize failed! should be 1 after run ends!\n\n");
	}

	SchedulerDestroy(my_sch);
	my_sch = NULL;
	printf("Finished TestSchedulerCreateDestory check vlg! \n\n");
}
/* ------------------------------------------------------------------------- */
static void TestSchedulerAddRemove(void)
{
	/* Case 1 task1 removes itself ! print  - remove self print -> end (no print) */
	{
		scheduler_ty* my_sch = SchedulerCreate();
		op_param_ty* my_op_param = CreateOpParam(3, my_sch ,invalid_uid_g);
		my_op_param->task_id = SchedulerAddTask(my_sch, 2, RemoveSelf,
										   my_op_param, CleanOpParam,
										   my_op_param);
		SchedulerRun(my_sch);
		SchedulerDestroy(my_sch);
		my_sch = NULL;
	}

	/* Case 2: task 2 removes task1 PrintOp print 1  -> RemoveOp prints 1 -> PrintOp Prints 2 -> Removed PrintOp! -> RemoveOp print 3 */
	{
		
		scheduler_ty* my_sch = SchedulerCreate();
		size_t intervals_1_amount = 3;
		size_t intervals_2_amount = 2;
		size_t interval_1_len = 2;
		size_t interval_2_len = 3;
		op_param_ty* my_op_param1 = CreateOpParam(intervals_1_amount, my_sch ,invalid_uid_g);
		op_param_ty* my_op_param2 = CreateOpParam(intervals_2_amount, my_sch ,invalid_uid_g);
		my_op_param1->task_id = SchedulerAddTask(my_sch, interval_1_len,
									 			PrintOp, my_op_param1, 
												CleanOpParam, my_op_param1);

		my_op_param2->task_id = my_op_param1->task_id;
		SchedulerAddTask(my_sch, interval_2_len, RemoveFriend, my_op_param2, 
						           CleanOpParam, my_op_param2);
		
		
		SchedulerRun(my_sch);
		SchedulerDestroy(my_sch);
		my_sch = NULL;
	}
	
}
/* ------------------------------------------------------------------------- */
static void TestSchedulerRunAddStop(void)
{
		scheduler_ty* my_sch = SchedulerCreate();
		sch_run_status_ty run_status = SCH_SUCCESS;
		op_param_ty* my_op_param = CreateOpParam(3, my_sch ,invalid_uid_g);
		my_op_param->task_id = SchedulerAddTask(my_sch, 2, StopOp,
										   my_op_param, CleanOpParam,
										   my_op_param);
		run_status = SchedulerRun(my_sch);

		if (SCH_STOP != run_status)
		{
			printf("Expected run_status == %d Got: run_status == %d\n\n",SCH_STOP, run_status);
		}

		if (SchedulerSize(my_sch) != 1)
		{
			printf("Expected Size = %d Got: Size = %lu\n\n",1, SchedulerSize);
		}
		
		SchedulerDestroy(my_sch);
		my_sch = NULL;
		printf("Finished TestSchedulerRunAddStop! \n\n");
}
/* ------------------------------------------------------------------------- */
static void TestSchedulerRunClear(void)
{
	size_t intervals_1 = 3;
	size_t intervals_2 = 2;
	size_t interval_1_len = 2;
	size_t interval_2_len = 3 ;

	scheduler_ty* my_sch = SchedulerCreate();
	sch_run_status_ty run_status = SCH_SUCCESS;

	op_param_ty* my_op_param1 = CreateOpParam(intervals_1, my_sch ,invalid_uid_g);
	op_param_ty* my_op_param2 = CreateOpParam(intervals_2, my_sch ,invalid_uid_g);

	my_op_param1->task_id = SchedulerAddTask(my_sch, interval_1_len,
								 			PrintOp, my_op_param1, 
											CleanOpParam, my_op_param1);
	my_op_param2->task_id = SchedulerAddTask(my_sch, interval_2_len, ClearOp, my_op_param2, 
					           CleanOpParam, my_op_param2);
	
	
	run_status = SchedulerRun(my_sch);
	if (SchedulerSize(my_sch) != 0)
	{
		printf("Expected Size = %d Got: Size = %lu\n\n",0, SchedulerSize);
	}
	
	if (SCH_SUCCESS != run_status)
	{
		printf("Expected run_status == %d Got: run_status == %d",SCH_SUCCESS, run_status);
	}
	
	SchedulerDestroy(my_sch);
	my_sch = NULL;
}

/* -------------------------------------------------------------------------
						Helper Functions Implementations
   ------------------------------------------------------------------------- */
static sch_op_status_ty RemoveSelf(void* param)
{
	op_param_ty* my_op_param = (op_param_ty*)param;

	if(NULL == param)
	{
		return SCH_REPEAT;
	}

    if(my_op_param->num_of_operations == 0)
	{
		return SCH_NOT_REPEAT;
	}

	if(3 == my_op_param->num_of_operations)
	{
		printf("Remove Self First Execution!\n\n");
	}
	else if(2 == my_op_param->num_of_operations)
	{
		SchedulerRemoveTask(my_op_param->sch, my_op_param->task_id);
		printf("Removed self from list here!\n\n");
	}
	else if(1 == my_op_param->num_of_operations)
	{
		printf("Remove Self This Part should not appear! \n\n");
	}
	--(my_op_param->num_of_operations);
	return SCH_REPEAT;
}
/* ------------------------------------------------------------------------- */
static sch_op_status_ty RemoveFriend(void* param)
{
	op_param_ty* my_op_param = (op_param_ty*)param;

	if(NULL == param)
	{
		return SCH_REPEAT;
	}

    if(my_op_param->num_of_operations == 0)
	{
		return SCH_NOT_REPEAT;
	}

	if(3 == my_op_param->num_of_operations)
	{
		
		printf("Remove Friend First Execution!\n\n");
	}
	else if(2 == my_op_param->num_of_operations)
	{
		SchedulerRemoveTask(my_op_param->sch, my_op_param->task_id);
		printf("Removed Friend From List!\n\n");
	}
	else if(1 == my_op_param->num_of_operations)
	{
		printf("Remove Friend Third Execution\n\n");
	}
	--(my_op_param->num_of_operations);
	return SCH_REPEAT;
}
/* ------------------------------------------------------------------------- */
static sch_op_status_ty PrintOp(void* param)
{
	op_param_ty* my_op_param = (op_param_ty*)param;

	if(NULL == param)
	{
		return SCH_REPEAT;
	}

    if(my_op_param->num_of_operations == 0)
	{
		return SCH_NOT_REPEAT;
	}

	printf("PrintOp Execution num %lu\n\n", my_op_param->num_of_operations);
	--(my_op_param->num_of_operations);
	return SCH_REPEAT;
}
/* ------------------------------------------------------------------------- */
static sch_op_status_ty StopOp(void* param)
{
	op_param_ty* my_op_param = (op_param_ty*)param;

	if(NULL == param)
	{
		return SCH_REPEAT;
	}

    if(my_op_param->num_of_operations == 0)
	{
		return SCH_NOT_REPEAT;
	}

	if(3 == my_op_param->num_of_operations)
	{
		printf("StopOp First Execution!\n\n");
	}
	else if(2 == my_op_param->num_of_operations)
	{
		SchedulerStop(my_op_param->sch);
		printf("StopOp Second Execution: Stopped Scheduler !\n\n");
	}
	else if(1 == my_op_param->num_of_operations)
	{
		printf("StopOp Third Execution! should not appear!\n\n");
	}
	--(my_op_param->num_of_operations);
	return SCH_REPEAT;
}
/* ------------------------------------------------------------------------- */
static sch_op_status_ty ClearOp(void* param)
{
	op_param_ty* my_op_param = (op_param_ty*)param;

	if(NULL == param)
	{
		return SCH_REPEAT;
	}

    if(my_op_param->num_of_operations == 0)
	{
		return SCH_NOT_REPEAT;
	}

	if(3 == my_op_param->num_of_operations)
	{
		printf("ClearOp First Execution!\n\n");
	}
	else if(2 == my_op_param->num_of_operations)
	{
		SchedulerClear(my_op_param->sch);
		printf("ClearOp Second Execution: Cleared Scheduler !\n\n");
	}
	else if(1 == my_op_param->num_of_operations)
	{
		printf("ClearedOp Third Execution! should not appear!\n\n");
	}
	--(my_op_param->num_of_operations);
	return SCH_REPEAT;
}
/* ------------------------------------------------------------------------- */
static op_param_ty* CreateOpParam(size_t num_of_operations, scheduler_ty* sch , uid_ty task_id)
{
	op_param_ty* new_op_param = (op_param_ty*)malloc(sizeof(op_param_ty));
	new_op_param->num_of_operations = num_of_operations;
	new_op_param->task_id = task_id;
	new_op_param->sch = sch;
	return new_op_param;
}
/* ------------------------------------------------------------------------- */
static void CleanOpParam(void* param)
{
	free((op_param_ty*)param);
	param = NULL;
}


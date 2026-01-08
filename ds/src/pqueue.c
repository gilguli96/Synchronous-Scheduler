/******************************************************************************

		Priority Queue
		Reviewer : Daria
		Date: 6 / 1 / 25 

******************************************************************************/

#include <stddef.h>		/* size_t */
#include <stdlib.h> 	/* malloc free */
#include <assert.h> 	/* assert */

#include "pqueue.h"
#include "sorted_list.h"
/******************************************************************************/
struct pq 
{
	sorted_list_ty* sorted_list;
};

/******************************************************************************/
pq_ty* PQCreate(pq_cmp_ty cmp, void* param)
{
	pq_ty* new_pq = NULL;
	assert(NULL != cmp);

	new_pq = (pq_ty*)malloc(sizeof(pq_ty));
	if (NULL == new_pq )
	{
		return NULL;
	}

	new_pq->sorted_list = SortedListCreate(cmp, param);
	
	if  (NULL == new_pq->sorted_list)
	{
		free(new_pq);
		return NULL;
	}
	return new_pq;
}
/******************************************************************************/
void PQDestroy(pq_ty* q)
{
	if (NULL == q)
	{
		return ;
	}

	SortedListDestroy(q->sorted_list);
	q->sorted_list = NULL;
	free(q);
	q = NULL;
}
/******************************************************************************/
size_t PQCount(const pq_ty* q)
{
	assert(NULL != q);
	return SortedListCount(q->sorted_list);
}
/******************************************************************************/
pq_bool_ty PQIsEmpty(const pq_ty* q)
{
	assert(NULL != q);
	return (pq_bool_ty)SortedListIsEmpty(q->sorted_list);
}
/******************************************************************************/
pq_status_ty PQEnqueue(pq_ty* q, void* data)
{
	sorted_list_iter_ty sorted_list_iter = {0};

	assert(NULL != q);

	sorted_list_iter = SortedListInsert(q->sorted_list, data);
	if (SortedListIterIsEqual(SortedListEndIter(q->sorted_list), sorted_list_iter))
	{
		return PQ_FAIL;
	}

	return PQ_SUCCESS;	 
}
/******************************************************************************/
void* PQDequeue(pq_ty* q)
{
	void* head_data = NULL;

	assert(NULL != q);
	assert(!SortedListIsEmpty(q->sorted_list));

	head_data = SortedListIterGetData(SortedListBeginIter(q->sorted_list));
	SortedListRemove(SortedListBeginIter(q->sorted_list));
	return head_data;
}
/******************************************************************************/
void* PQRemove(pq_ty* q, pq_is_match_ty is_match, void* param)
{
	sorted_list_iter_ty sorted_iter = {0};
	void* data_removed = NULL;	
	
	assert(NULL != q);
	assert(NULL != is_match);

	sorted_iter = SortedListFindIf(SortedListBeginIter(q->sorted_list),
					 SortedListEndIter(q->sorted_list), 
					 is_match,
					 param);

	if (SortedListIterIsEqual(sorted_iter, SortedListEndIter(q->sorted_list)))
	{
		return NULL;	
	}

	data_removed = SortedListIterGetData(sorted_iter);
	SortedListRemove(sorted_iter);
	return data_removed;
}
/******************************************************************************/
void* PQPeek(const pq_ty* q)
{
	assert(NULL != q);
	assert(!SortedListIsEmpty(q->sorted_list));

	return SortedListIterGetData(SortedListBeginIter(q->sorted_list));
}
/******************************************************************************/



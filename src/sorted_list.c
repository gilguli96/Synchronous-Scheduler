/*******************************************************************************

	Reviewer :  Shir Tankel
	Version : 2
	Date : 28/12/25

*******************************************************************************/
#include <stdlib.h>				/*	malloc free		*/
#include <assert.h>				/*  assert			*/
#include <stddef.h>				/*	size_t	*/

#include "sorted_list.h"		

#define TRUE					1
#define FALSE					0
#define UNUSED(x) 				((x)=(x))

#ifndef NDEBUG									/* in debug we have access to the the list from iter */
#define ITER_GET_LIST(iter)		((iter).lst)
#else
#define	ITER_GET_LIST(iter)		(NULL)
#endif

#ifndef NDEBUG
#define SET_ITER_TO_NULL		{0}				/* in debug we need so set the whole struct to null */
#else											/* in release we need so set ptr to null */
#define	SET_ITER_TO_NULL		NULL
#endif

#define SET_D_ITER_TO_NULL		NULL				

/*******************************************************************************/
struct sorted_list
{
	dlist_ty* dlist;
	sorted_list_cmp_func_ty cmp;
	void* param;
};
/*******************************************************************************/
static sorted_list_iter_ty DIterToSIter(dlist_iter_ty dlist_iter, const sorted_list_ty* slist)
{
#ifndef NDEBUG
	sorted_list_iter_ty new_sorted_list_iter;

	new_sorted_list_iter.d_iter = dlist_iter;
	new_sorted_list_iter.lst = (sorted_list_ty*)slist;
	return new_sorted_list_iter;
#else
	return dlist_iter;
#endif
}
/*******************************************************************************/
static dlist_iter_ty SIterToDIter(sorted_list_iter_ty sorted_list_iter)
{
#ifndef NDEBUG
	return sorted_list_iter.d_iter;
#else
	return (dlist_iter_ty)sorted_list_iter;
#endif
}
/*******************************************************************************/
static int IsValidIter(sorted_list_iter_ty sorted_list_iter)
{
#ifndef NDEBUG
	return (NULL != sorted_list_iter.d_iter) && ( NULL != sorted_list_iter.lst ) ;
#else
	return NULL != sorted_list_iter ;
#endif
}
/*******************************************************************************/
static int ItersOnSameList(sorted_list_iter_ty iter1, sorted_list_iter_ty iter2)
{
#ifndef NDEBUG
	return ITER_GET_LIST(iter1) ==  ITER_GET_LIST(iter2);
#else
	return TRUE ;
#endif
}
/*******************************************************************************/
static sorted_list_iter_ty FindValidLoc(sorted_list_ty* sorted_list ,sorted_list_iter_ty from, sorted_list_iter_ty to, void* data)
{
	sorted_list_cmp_func_ty cmp_func = sorted_list->cmp;
	sorted_list_iter_ty cur_iter = from;
	sorted_list_iter_ty end_iter = to;

	while( !SortedListIterIsEqual(cur_iter, end_iter) )
	{
		if (cmp_func(data, SortedListIterGetData(cur_iter), sorted_list->param) < 0)
		{
			return cur_iter;
		}
		cur_iter = SortedListIterNext(cur_iter);
	}
	return end_iter;
}
/*******************************************************************************/
sorted_list_ty* SortedListCreate(sorted_list_cmp_func_ty cmp, void* param)
{
	sorted_list_ty* new_sorted_list = (sorted_list_ty*)malloc(sizeof(sorted_list_ty));

	if (NULL == new_sorted_list)
	{
		return NULL;
	}
	new_sorted_list->dlist = DListCreate();
	if (NULL == new_sorted_list->dlist)
	{
		free(new_sorted_list);
		new_sorted_list = NULL;
		return NULL;
	}
	new_sorted_list->param = param;
	new_sorted_list->cmp = cmp;
	
	return new_sorted_list;
}
/*******************************************************************************/
void SortedListDestroy(sorted_list_ty* lst)
{
		if (NULL == lst)
		{
			return;
		}

		DListDestroy(lst->dlist);
		lst->dlist = NULL;
		free(lst);
		lst = NULL;
}
/*******************************************************************************/
size_t SortedListCount(const sorted_list_ty* lst)
{
	assert(NULL != lst);

	return DListCount(lst->dlist);
}
/*******************************************************************************/
int SortedListIsEmpty(const sorted_list_ty* lst)
{
	assert(NULL != lst);

	return DListIsEmpty(lst->dlist);
}
/*******************************************************************************/
void* SortedListIterGetData(sorted_list_iter_ty iter)
{
	assert(IsValidIter(iter));
	assert(!SortedListIterIsEqual(iter, SortedListEndIter(ITER_GET_LIST(iter))));

	return DListIterGetData(SIterToDIter(iter));
}

/*******************************************************************************/
void SortedListIterSetData( sorted_list_ty* lst,
                            sorted_list_iter_ty iter,
                            void* new_data            )
{
	dlist_iter_ty d_iter = SET_D_ITER_TO_NULL;
	sorted_list_iter_ty where = SET_ITER_TO_NULL;
	dlist_iter_ty d_where = SET_D_ITER_TO_NULL;

	assert(IsValidIter(iter));
	assert(!SortedListIterIsEqual(SortedListEndIter(lst), iter));
	
	d_iter = SIterToDIter(iter);
	where = FindValidLoc(lst ,SortedListBeginIter(lst), SortedListEndIter(lst), new_data);
	d_where = SIterToDIter(where);

	if (DListIterIsEqual(d_iter, d_where))
	{
		DListIterSetData(d_iter, new_data);
		return;
	}

	DListIterSetData(d_iter, new_data);
	DListSplice(d_iter, DListIterNext(d_iter), d_where);
}
/*******************************************************************************/
sorted_list_iter_ty SortedListBeginIter(const sorted_list_ty* lst)
{
	assert(NULL != lst);
	
	return DIterToSIter(DListBeginIter(lst->dlist), lst);
}
/*******************************************************************************/
sorted_list_iter_ty SortedListEndIter(const sorted_list_ty* lst)
{
	assert(NULL != lst);
	
	return DIterToSIter(DListEndIter(lst->dlist), lst);
}
/*******************************************************************************/
sorted_list_iter_ty SortedListIterNext(sorted_list_iter_ty iter)
{
	assert(IsValidIter(iter));
	assert(!SortedListIterIsEqual(iter, SortedListEndIter(ITER_GET_LIST(iter))));

	return DIterToSIter(DListIterNext(SIterToDIter(iter)), ITER_GET_LIST(iter));
}
/*******************************************************************************/
sorted_list_iter_ty SortedListIterPrev(sorted_list_iter_ty iter)
{
	assert(IsValidIter(iter));
	assert(!SortedListIterIsEqual(iter, SortedListBeginIter(ITER_GET_LIST(iter))));

	return DIterToSIter(DListIterPrev(SIterToDIter(iter)), 
						ITER_GET_LIST(iter));
}
/*******************************************************************************/
int SortedListIterIsEqual( sorted_list_iter_ty iter1, 
						   sorted_list_iter_ty iter2  )
{
	assert(IsValidIter(iter1));
	assert(IsValidIter(iter2));
	assert(ItersOnSameList(iter1, iter2));

	return DListIterIsEqual(SIterToDIter(iter1), SIterToDIter(iter2));
}

/*******************************************************************************/
sorted_list_iter_ty SortedListInsert(sorted_list_ty* lst, void* new_data)
{
	sorted_list_iter_ty iter_where = SET_ITER_TO_NULL ;

	assert (NULL != lst);

	iter_where = FindValidLoc(lst ,SortedListBeginIter(lst), SortedListEndIter(lst) , new_data);

	return DIterToSIter(DListInsertBefore(lst->dlist, 
										SIterToDIter(iter_where),
										new_data), ITER_GET_LIST(iter_where));	
}
/*******************************************************************************/
sorted_list_iter_ty SortedListRemove(sorted_list_iter_ty iter)
{
	assert(IsValidIter(iter));
	assert(!SortedListIterIsEqual(iter ,SortedListEndIter(ITER_GET_LIST(iter))));

	return DIterToSIter(DListRemove(SIterToDIter(iter)), ITER_GET_LIST(iter));
}
/*******************************************************************************/
sorted_list_iter_ty SortedListFind(sorted_list_ty* lst, void* data)
{
	sorted_list_cmp_func_ty cmp_func = NULL;
	sorted_list_iter_ty cur_iter = SET_ITER_TO_NULL;
	sorted_list_iter_ty end_iter = SET_ITER_TO_NULL;

	assert(NULL != lst);

	cmp_func = lst->cmp;
	cur_iter = SortedListBeginIter(lst);
	end_iter = SortedListEndIter(lst);

	while( !SortedListIterIsEqual(cur_iter, end_iter) )
	{
		if (cmp_func(data, SortedListIterGetData(cur_iter), lst->param) == 0)
		{
			return cur_iter;
		}
		cur_iter = SortedListIterNext(cur_iter);
	}
	return end_iter;
}
/******************************************************************************/
sorted_list_iter_ty SortedListFindIf( sorted_list_iter_ty from,
									  sorted_list_iter_ty to, 
					                  sorted_list_is_match_func_ty is_match,
					                  void* param     )
{
	dlist_iter_ty d_from = SET_D_ITER_TO_NULL;
	dlist_iter_ty d_to = SET_D_ITER_TO_NULL;

	assert(IsValidIter(from));
	assert(IsValidIter(to));
	assert(ItersOnSameList(from, to));

	d_from = SIterToDIter(from);
	d_to = SIterToDIter(to);
	
	return DIterToSIter(DListFind(d_from, d_to, is_match, param), ITER_GET_LIST(from));
}
/******************************************************************************/
int SortedListForEach( sorted_list_iter_ty from, sorted_list_iter_ty to, 
					   sorted_list_action_func_ty action, void* param    )
{
	dlist_iter_ty d_from = SET_D_ITER_TO_NULL;
	dlist_iter_ty d_to = SET_D_ITER_TO_NULL;

	assert(IsValidIter(from));
	assert(IsValidIter(to));
	assert(ItersOnSameList(from, to));

	d_from = SIterToDIter(from);
	d_to = SIterToDIter(to);
	
	return DListForEach(d_from, d_to, action, param);
}
/******************************************************************************/
void SortedListMerge(sorted_list_ty* dest, sorted_list_ty* src)
{
	sorted_list_iter_ty end_iter_dest = SET_ITER_TO_NULL;
	sorted_list_iter_ty end_iter_src = SET_ITER_TO_NULL;

	sorted_list_iter_ty from = SET_ITER_TO_NULL;
	sorted_list_iter_ty to = SET_ITER_TO_NULL;
	sorted_list_iter_ty where = SET_ITER_TO_NULL;

	assert(NULL != dest);
	assert(NULL != src);
	assert(dest != src);

	where = SortedListBeginIter(dest);
	from = SortedListBeginIter(src);
	end_iter_dest = SortedListEndIter(dest);
	end_iter_src = SortedListEndIter(src);
	
	while(!SortedListIterIsEqual(from, end_iter_src))
	{
		where = FindValidLoc(dest, where, end_iter_dest, SortedListIterGetData(from));

		/* if where is end_iter we just push the entire src list to the end of dest_list.
		   if I dont have this if I will get seg fault because after this if I will try to access 'where' data */
		if (SortedListIterIsEqual(where, end_iter_dest))
		{
			DListSplice(SIterToDIter(from), SIterToDIter(end_iter_src), SIterToDIter(end_iter_dest));
			return;
		}
	
		to = FindValidLoc(src, from, end_iter_src, SortedListIterGetData(where));
		
		DListSplice(SIterToDIter(from), SIterToDIter(to), SIterToDIter(where));

		from = to;
	}
}


                       

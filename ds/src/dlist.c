#include <stdlib.h>		/*  malloc free 		*/
#include <assert.h>		/*	assert				*/

#include "dlist.h"

#define SUCCESS			0
#define ALOC_FAIL		2
#define NODE_TO_ITER(iter) ((dlist_node_ty*)(iter))

struct dlist_node
{
	dlist_node_ty* next;
	dlist_node_ty* prev;
	void* data;
};

struct dlist
{
	dlist_node_ty dummy_head; 	/* first real node will be dummy_head.next */
	dlist_node_ty dummy_tail; 	/* last real node will be dummy_tail.prev */
};
/*****************************************************************************/
static dlist_node_ty* IterToNode(const dlist_iter_ty iter)
{
	return (dlist_node_ty*)iter ;
}
/*****************************************************************************/
static dlist_iter_ty NodeToIter(const dlist_node_ty* node)
{
	return (dlist_iter_ty)node ;
}
/*****************************************************************************/
static void AttachNodes(dlist_node_ty* node1, dlist_node_ty* node2)
{
	node1->next = node2;
	node2->prev = node1;
}
dlist_ty* DListCreate(void)
{
	dlist_ty* new_list = (dlist_ty*)malloc(sizeof(dlist_ty));
	if (NULL == new_list)
	{
		return NULL;
	}

	new_list->dummy_head.prev = NULL;
	new_list->dummy_head.next = &(new_list->dummy_tail);

	new_list->dummy_tail.prev = &(new_list->dummy_head);
	new_list->dummy_tail.next = NULL;	

	new_list->dummy_tail.data = NULL;
	new_list->dummy_head.data = NULL;
	
	return new_list;
}

/*****************************************************************************/
dlist_iter_ty DListBeginIter(const dlist_ty* dlist)
{
	assert(NULL != dlist);
	return NodeToIter(dlist->dummy_head.next);
}
/*****************************************************************************/
dlist_iter_ty DListEndIter(const dlist_ty* dlist)
{
	assert(NULL != dlist) ;
	return NodeToIter(&(dlist->dummy_tail)) ;
}
/*****************************************************************************/
int DListIsEmpty(const dlist_ty* dlist)
{
	assert(NULL != dlist);
	return DListBeginIter(dlist) == DListEndIter(dlist);
}
/*****************************************************************************/
dlist_iter_ty DListIterNext(const dlist_iter_ty iter)
{
	assert(NULL != iter);
	return NodeToIter(IterToNode(iter)->next);
}
/*****************************************************************************/
dlist_iter_ty DListIterPrev(const dlist_iter_ty iter)
{
	assert(NULL != iter);
	return NodeToIter(IterToNode(iter)->prev);
}
/*****************************************************************************/
void DListIterSetData(dlist_iter_ty iter, void* data_to_set)
{
	assert(NULL != iter);
	IterToNode(iter)->data = data_to_set ;
}
/*****************************************************************************/
void* DListIterGetData(const dlist_iter_ty iter)
{
	assert(NULL != iter);
	return IterToNode(iter)->data;
}
/*****************************************************************************/
int DListIterIsEqual(const dlist_iter_ty iter1, const dlist_iter_ty iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	return IterToNode(iter1) == IterToNode(iter2);	
}
/*****************************************************************************/
size_t DListCount(const dlist_ty* dlist)
{
	dlist_iter_ty cur_iter = DListBeginIter(dlist);
	dlist_iter_ty end_iter = DListEndIter(dlist);
	size_t count = 0 ;

	assert(NULL != dlist) ;


	while(cur_iter != end_iter)
	{
		cur_iter = DListIterNext(cur_iter);
		++count;
	}
	
	return count;
}
/*****************************************************************************/
dlist_iter_ty DListInsertBefore(dlist_ty* dlist, dlist_iter_ty where, void* new_data)
{
	dlist_node_ty* prev_node = NULL ;
	dlist_node_ty* next_node = NULL ;
	dlist_node_ty* new_node = NULL ;

	assert(NULL != dlist);
	assert(NULL != where);

	prev_node = IterToNode(where)->prev;
	new_node = (dlist_node_ty*)malloc(sizeof(dlist_node_ty));
	next_node = IterToNode(where);

	if (NULL == new_node)
	{
		return DListEndIter(dlist);
	}

	AttachNodes(prev_node, new_node);
	AttachNodes(new_node, next_node);

	new_node->data = new_data;

	return NodeToIter(new_node);
}
/*****************************************************************************/
dlist_iter_ty DListRemove(dlist_iter_ty iter)
{
	dlist_node_ty* prev_node = IterToNode(iter)->prev;
	dlist_node_ty* next_node = IterToNode(iter)->next;
	
	prev_node->next = next_node;
	next_node->prev = prev_node;

	free(IterToNode(iter));
	iter = NULL;
	
	return NodeToIter(next_node);
}
/*****************************************************************************/
void DListDestroy(dlist_ty* dlist)
{

	dlist_iter_ty cur_iter = DListBeginIter(dlist);
	dlist_iter_ty end_iter = DListEndIter(dlist);

	if (NULL == dlist)
	{
		return;
	}	

	while (cur_iter != end_iter)
	{
		cur_iter = DListRemove(cur_iter);
	} 

	free(dlist);
	dlist = NULL;
}
/*****************************************************************************/
dlist_iter_ty DListPushFront(dlist_ty* dlist, void* new_data)
{
	assert(NULL != dlist);

	return DListInsertBefore(dlist, DListBeginIter(dlist), new_data);
}
/*****************************************************************************/
void DListPopFront(dlist_ty* dlist)
{
	assert(NULL != dlist);
	assert(!DListIsEmpty(dlist));

	DListRemove(DListBeginIter(dlist));
}
/*****************************************************************************/
dlist_iter_ty DListPushBack(dlist_ty* dlist, void* new_data)
{
	assert(NULL != dlist);

	return DListInsertBefore(dlist, DListEndIter(dlist), new_data);	
}
/*****************************************************************************/
void DListPopBack(dlist_ty* dlist)
{
	assert (NULL != dlist);
	DListRemove(DListIterPrev(DListEndIter(dlist)));
}
/*****************************************************************************/
int DListForEach( dlist_iter_ty from, const dlist_iter_ty to,
				action_func_ty action,  void* param)
{
	dlist_iter_ty cur_iter = from;
	int ret_value = 1;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action);

	while ( !DListIterIsEqual(cur_iter, to) )
	{		
		ret_value = action(IterToNode(cur_iter)->data, param);

		if (SUCCESS != ret_value)
		{
			return ret_value;
		}
		
		cur_iter = DListIterNext(cur_iter);
	}
	return SUCCESS;	
}
/*****************************************************************************/
dlist_iter_ty DListFind(const dlist_iter_ty from, 
						const dlist_iter_ty to, 
						is_match_func_ty is_match, void* param)
{
	dlist_iter_ty cur_iter = from;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match);

	while( !DListIterIsEqual(cur_iter, to) )
	{
		if ( is_match((IterToNode(cur_iter))->data , param) )
		{
			return cur_iter;
		}
		cur_iter = DListIterNext(cur_iter);
	}

	return to;	
}
/*****************************************************************************/
int DListMultiFind(	const dlist_iter_ty from, const dlist_iter_ty to, 
					dlist_ty* dest,
					is_match_func_ty is_match, void* param			)
{
		dlist_iter_ty cur_iter = from;
		dlist_iter_ty ret_push = NULL;

		assert(NULL != from);
		assert(NULL != to);
		assert(NULL != is_match);
		
		cur_iter = DListFind(cur_iter, to, is_match, param);

		while( !DListIterIsEqual(cur_iter, to) )
		{
			ret_push = DListPushBack(dest , DListIterGetData(cur_iter));
			if( DListIterIsEqual(DListEndIter(dest), ret_push) )
  			{
				return ALOC_FAIL;
			}
			
			cur_iter = DListIterNext(cur_iter);
			cur_iter = DListFind(cur_iter, to, is_match, param);			
		}

		return SUCCESS;
}
/******************************************************************************/
void DListSplice(dlist_iter_ty from, dlist_iter_ty to, dlist_iter_ty where)
{
	dlist_node_ty* from_node = NULL;
	dlist_node_ty* where_node = NULL;
	dlist_node_ty* to_node = NULL;
	dlist_node_ty* old_where_prev_node = NULL;
	dlist_node_ty* old_from_prev_node = NULL;

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != where);
	assert(!DListIterIsEqual(from, to));
	assert(IterToNode(from)->next != NULL);

	from_node = IterToNode(from);
	where_node = IterToNode(where);
	to_node = IterToNode(to);
	old_from_prev_node = IterToNode(DListIterPrev(from));
	old_where_prev_node = IterToNode(DListIterPrev(where));
	
	/* attaching where and (the previous of to). */
	where_node->prev = to_node->prev;
	to_node->prev->next = where_node;

	/* attaching (the previous of where) and from */
	old_where_prev_node->next = from_node;
	from_node->prev = old_where_prev_node;

	/* attaching (the old previous of from) and to */
	old_from_prev_node->next = to_node;
	to_node->prev = old_from_prev_node;

}



#ifndef __ILRD_SORTED_LIST_H__
#define __ILRD_SORTED_LIST_H__

#include <stddef.h>	/* size_t */

#include "dlist.h"

typedef struct sorted_list sorted_list_ty;

/* May change - do not touch or use it */
#ifndef NDEBUG
typedef struct sorted_list_iter
{
	dlist_iter_ty d_iter;
	sorted_list_ty* lst;
} sorted_list_iter_ty;
#else
typedef dlist_iter_ty sorted_list_iter_ty;
#endif

/****************************************************************************** 
*   Function compares "data1" and "data2", any other parameter may be
*		passed with "param".
*	Return value:
* 		0 > result if data1 comes before data2,
* 		0 < result if data2 comes before data1,
*		0 == result if data1 and data2 are equal
******************************************************************************/
typedef int (*sorted_list_cmp_func_ty)( const void* data1,
                                        const void* data2,
                                        void* param        );
                                        
/****************************************************************************** 
*   Function receives node's data ("data"), any other parameter may be
*		passed with "param".
*	Function should return 0 on success, any other value will be considered
*		failure.
******************************************************************************/
typedef int (*sorted_list_action_func_ty)(void* data, void* param);

/****************************************************************************** 
*   Function receives node's data ("data"), any other parameter may be
*		passed with "param".
*	Function should return 1 on success and 0 on failure.
******************************************************************************/
typedef int (*sorted_list_is_match_func_ty)(const void* data, void* param);

/****************************************************************************** 
* 	Description:
* 		Creates an empty list
* 	Return value:	
* 		On success, a pointer to the created list
* 		On failure, NULL
*   Undefined behavior:
* 		cmp is NULL
* 	O(1)
******************************************************************************/
sorted_list_ty* SortedListCreate(sorted_list_cmp_func_ty cmp, void* param);

/****************************************************************************** 
* 	Description:
* 		Destroys the lst
* 		No operations performed if lst is NULL
* 	Return value:
* 		None
*   O(n)
******************************************************************************/
void SortedListDestroy(sorted_list_ty* lst);

/****************************************************************************** 
* 	Description:
* 		Counts the size of lst	  
* 	Return value: 
* 		Count of nodes in lst (Not including dummy)
* 	Undefined behavior: 
* 		lst is NULL
*   O(n)
******************************************************************************/
size_t SortedListCount(const sorted_list_ty* lst);

/****************************************************************************** 
* 	Description:
* 		Checks if the list is empty
* 	Return value: 
* 		1 if empty
* 		0 otherwise
* 	Undefined behavior:
* 		lst is NULL
* 	O(1)
******************************************************************************/
int SortedListIsEmpty(const sorted_list_ty* lst);

/****************************************************************************** 
* 	Description:
* 		Insert a new node to the list
* 	Return value:	
* 		On success, iterator to the newly allocated node,
* 		On failure, SortedListEndIter
* 	Undefined behaviour:
* 		lst is NULL
*   O(n)
******************************************************************************/
sorted_list_iter_ty SortedListInsert(sorted_list_ty* lst, void* new_data);

/****************************************************************************** 
*   Description:
* 		Removes a node pointed by iter from the list
* 	Return value: 
* 		iter to next node
* 	Undefined behavior: 
* 		iter is SortedListEndIter
*   O(1)
******************************************************************************/
sorted_list_iter_ty SortedListRemove(sorted_list_iter_ty iter);

/****************************************************************************** 
*   Description:
* 		Returns pointer to the node's data on which iter points
* 	Return value: 
* 		Pointer to the node's data
* 	Undefined behavior:
* 		iter is SortedListEndIter
* 	O(1)
******************************************************************************/
void* SortedListIterGetData(sorted_list_iter_ty iter);

/****************************************************************************** 
* 	Description:
* 		Sets a given data, rearranging lst if necessary
* 	Return value: 
* 		None
* 	Undefined behavior:
* 		iter is SortedListEndIter
* 	O(n)
******************************************************************************/
void SortedListIterSetData( sorted_list_ty* lst,
                            sorted_list_iter_ty iter,
                            void* new_data            );

/****************************************************************************** 
*	Description:
* 		Returns iter to "dummy" node - invalid node
* 	Return value: 
* 		iterator to the "dummy" node
* 	Undefined behavior:
* 		lst is NULL
* 	O(1)
******************************************************************************/
sorted_list_iter_ty SortedListEndIter(const sorted_list_ty* lst);

/****************************************************************************** 
*   Description:
* 		Returns iter to the first node in the list
* 	Return value: 
* 		iterator to the first node
* 		In case of empty list - returns SortedListEndIter, the dummy node 
* 	Undefined behavior:
* 		lst is NULL
* 	O(1)
******************************************************************************/
sorted_list_iter_ty SortedListBeginIter(const sorted_list_ty* lst);

/******************************************************************************
*	Description:
* 		Returns iter to next node
* 	Return value: 
* 		iterator to the next node
*	Undefined behavior:
* 		iter is SortedListEndIter
* 	O(1)
******************************************************************************/
sorted_list_iter_ty SortedListIterNext(sorted_list_iter_ty iter);

/******************************************************************************
*	Description:
* 		Returns iter to prev node
* 	Return value: 
* 		iterator to the prev node
*	Undefined behavior:
* 		iter is the first node
* 	O(1)
******************************************************************************/
sorted_list_iter_ty SortedListIterPrev(sorted_list_iter_ty iter);

/******************************************************************************
*	Description:
* 		Checks if the iters are equal
* 	Return Value: 
* 		1 if iter1 and iter2 are of the same node, 
* 		0 otherwise.   
*	Undefined behavior:
*	 	iter1 or iter2 are invalid
*		iter1 or iter2 are not in the same list
* 	O(1)
******************************************************************************/
int SortedListIterIsEqual( sorted_list_iter_ty iter1, 
						   sorted_list_iter_ty iter2  );

/****************************************************************************** 
* 	Description:
* 		Permorfms the action on each node in range ["from", "to") 
* 		in case of sucess of each action, oterwise stops upon the first fail 
* 	Return Value: 
* 		0 on succes, action return value on failture.
* 	Undefined behaviour:
* 		action is NULL 
* 		from or to are invalid pointer
* 		from > to
* 		from and to are not from the same list
* 	O(n)
******************************************************************************/
int SortedListForEach( sorted_list_iter_ty from, sorted_list_iter_ty to, 
					   sorted_list_action_func_ty action, void* param    );

/****************************************************************************** 
*  	Description:
* 		Searches for a match in lst, according to default cmp func logic
*   Return value:
* 		On success, returns iter to the matching node
* 		On failure, returns SortedListEndIter
*   Undefinded behavior:
*        lst is NULL
*   O(n)
******************************************************************************/
sorted_list_iter_ty SortedListFind(sorted_list_ty* lst, void* data);
							
/****************************************************************************** 
*  	Description:
* 		Searches for a match in range ["from", "to"), using a custom match
*           func
*   Return value:
* 		On success, returns iter to the matching node
* 		On failure, returns SortedListEndIter
*   Undefinded behavior:
*       from or to are invaild iters
*       from and to don't belong to the same list
*       from < to 
*       is_match is NULL
*   O(n)
******************************************************************************/
sorted_list_iter_ty SortedListFindIf( sorted_list_iter_ty from,
                                      sorted_list_iter_ty to, 
					                  sorted_list_is_match_func_ty is_match,
					                  void* param                            );

/******************************************************************************
* 	Description:
* 		Merges src into dest, src left empty
*   Return value:
* 		None
*   Undefinded behavior:
*		dest and src point to the same list
*   O(n+m)
******************************************************************************/
void SortedListMerge(sorted_list_ty* dest, sorted_list_ty* src);

#endif /* __ILRD_SORTED_LIST_H__ */


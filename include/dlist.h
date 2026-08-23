#ifndef __ILRD_DLIST_H_
#define __ILRD_DLIST_H_

#include <stddef.h>	/* size_t */

typedef struct dlist dlist_ty;

typedef struct dlist_node dlist_node_ty;

/* May change - do not touch or use it */
typedef dlist_node_ty* dlist_iter_ty;

/****************************************************************************** 
*   Function receives node's data ("data"), any other parameter may be
*		passed with "param".
*	Function should return 0 on success, any other value will be considered
*		failure.
******************************************************************************/
typedef int (*action_func_ty)(void* data, void* param);

/****************************************************************************** 
*   Function receives node's data ("data"), any other parameter may be
*		passed with "param".
*	Function should return 0 on success and 1 on failure.
******************************************************************************/
typedef int (*is_match_func_ty) (const void* data, void* param);

/****************************************************************************** 
* 	Description:
* 		Creates an empty list,
* 	Return value:	
* 		On success, a pointer to the created list
* 		On failure, NULL
* 	O(1)
******************************************************************************/
dlist_ty* DListCreate(void);

/****************************************************************************** 
* 	Description:
* 		Destroys the dlist
* 		No operations performed if dlist is NULL
* 	Return value:
* 		None
*   O(n)
******************************************************************************/
void DListDestroy(dlist_ty* dlist);

/****************************************************************************** 
* 	Description:
* 		Counts the size of dlist	  
* 	Return value: 
* 		Count of nodes in dlist (Not including dummy)
* 	Undefined behavior: 
* 		dlist is NULL
*   O(n)
******************************************************************************/
size_t DListCount(const dlist_ty* dlist);

/****************************************************************************** 
* 	Description:
* 		Checks if the list is empty
* 	Return value: 
* 		1 if empty
* 		0 otherwise
* 	Undefined behavior:
* 		"dlist" is NULL
* 	O(1)
******************************************************************************/
int DListIsEmpty(const dlist_ty* dlist);

/****************************************************************************** 
* 	Description:
* 		Insert a new node with a passed data before "where"
* 	Return value:	
* 		On success, iterator to the newly allocated node,
* 		On failure, "DListEnd"
* 	Undefined behaviour:
* 		dlist is NULL
* 		where is invalid
*   O(1)
******************************************************************************/
dlist_iter_ty DListInsertBefore(	dlist_ty* dlist, 
									dlist_iter_ty where, 
									void* new_data);

/****************************************************************************** 
* 	Description:
* 		Insert a new node with a passed data to the front of the list
* 	Return value:	
* 		On success, iterator to the newly allocated node,
* 		On failure, "DListEnd"
* 	Undefined behaviour:
* 		dlist is NULL
*   O(1)
******************************************************************************/
dlist_iter_ty DListPushFront(dlist_ty* dlist, void* new_data);

/****************************************************************************** 
*   Description:
* 		Removes a node from the beginning of the list
* 	Return value: 
* 		None
* 	Undefined behavior: 
* 		dlist is NULL
* 		dlist is empty
*   O(1)
******************************************************************************/
void DListPopFront(dlist_ty* dlist);

/****************************************************************************** 
* 	Description:
* 		Insert a new node with a passed data to the back of the list
* 	Return value:	
* 		On success, iterator to the newly allocated node,
* 		On failure, "DListEnd"
* 	Undefined behaviour:
* 		dlist is NULL
*   O(1)
******************************************************************************/
dlist_iter_ty DListPushBack(dlist_ty* dlist, void* new_data);

/****************************************************************************** 
*   Description:
* 		Removes a node from the back of the list
* 	Return value: 
* 		None
* 	Undefined behavior: 
* 		dlist is NULL
* 		dlist is empty
*   O(1)
******************************************************************************/
void DListPopBack(dlist_ty* dlist);
									
/****************************************************************************** 
*   Description:
* 		Removes a node pointed by iter from the list
* 	Return value: 
* 		iter to next node
* 	Undefined behavior: 
* 		iter is "DListEnd"
*   O(1)
******************************************************************************/
dlist_iter_ty DListRemove(dlist_iter_ty iter);

/****************************************************************************** 
*   Description:
* 		Returns pointer to the node's data on which iter points
* 	Return value: 
* 		Pointer to the node's data
* 	Undefined behavior:
* 		"iter" is DListEnd
* 	O(1)
******************************************************************************/
void* DListIterGetData(const dlist_iter_ty iter);

/****************************************************************************** 
* 	Description:
* 		Sets a given data
* 	Return value: 
* 		None
* 	Undefined behavior:
* 		"iter" is DListEnd
* 	O(1)
******************************************************************************/
void DListIterSetData(dlist_iter_ty iter, void* data_to_set);

/****************************************************************************** 
*	Description:
* 		Returns iter to "dummy" node - invalid node
* 	Return value: 
* 		iterator to the "dummy" node
* 	Undefined behavior:
* 		dlist is NULL
* 	O(1)
******************************************************************************/
dlist_iter_ty DListEndIter(const dlist_ty* dlist);

/****************************************************************************** 
*   Description:
* 		Returns iter to the first node in the list
* 	Return value: 
* 		iterator to the first node
* 		In case of empty list - returns "DListEndIter()", the dummy node 
* 	Undefined behavior:
* 		dlist is NULL
* 	O(1)
******************************************************************************/
dlist_iter_ty DListBeginIter(const dlist_ty* dlist);

/******************************************************************************
*	Description:
* 		Returns iter to next node
* 	Return value: 
* 		iterator to the next node
*	Undefined behavior:
* 		"iter" is DListEnd
* 	O(1)
******************************************************************************/
dlist_iter_ty DListIterNext(const dlist_iter_ty iter);

/******************************************************************************
*	Description:
* 		Returns iter to prev node
* 	Return value: 
* 		iterator to the prev node
*	Undefined behavior:
* 		"iter" is the first node
* 	O(1)
******************************************************************************/
dlist_iter_ty DListIterPrev(const dlist_iter_ty iter);

/****************************************************************************** 
* 	Description:
* 		Permorfms the action on each node in range ["from", "to") 
* 		in case of sucess of each action, oterwise stops upon the first fail 
* 	Return Value: 
* 		0 on succes, "action" return value on failture.
* 	Undefined behaviour:
* 		action is NULL 
* 		"from" or "to" are invalid pointer
* 		"from" > "to"
* 		"from" and "to" are not from the same list
* 	O(n)
******************************************************************************/
int DListForEach( dlist_iter_ty from, const dlist_iter_ty to, 
				  action_func_ty action,  void* param           );

/******************************************************************************
*	Description:
* 		Checks if the iters are equal
* 	Return Value: 
* 		1 if iter1 and iter2 are of the same node, 
* 		0 otherwise.   
*	Undefined behavior:
*	 	"iter1"/"iter2" NULL
*		"iter1"/"iter2" not in the same list
* 	O(1)
******************************************************************************/
int DListIterIsEqual(const dlist_iter_ty iter1, const dlist_iter_ty iter2);
		  
/****************************************************************************** 
*  	Description:
* 		Searches for a match in range ["from", "to")
*   Return value:
* 		On success, returns iter to the matching node 
* 		On failure, returns "to"
*   Undefinded behavior:
*       "from" or "to" are invaild iters
*       "from" and "to" don't belong to the same list
*       "from" > "to" 
*       "is_match" is NULL
*   O(n)
******************************************************************************/
dlist_iter_ty DListFind( const dlist_iter_ty from, const dlist_iter_ty to, 
						 is_match_func_ty is_match, void* param	            );
							
/****************************************************************************** 
*  	Description:
* 		Searches for all matching to param nodes in range ["from", "to")
* 		with is_match function and insets them to "dest" list, provided by user
*   Return value:
* 		On success, returns 0
* 		On failure, returns 1
*   Undefinded behavior:
*       "from" or "to" are invaild iters
*       "from" and "to" don't belong to the same list
*       "from" > "to" 
*       "is_match" is NULL
* 		"dest" is NULL
*       "dest" is between "from" and "to"
*   O(n)
******************************************************************************/
int DListMultiFind(	const dlist_iter_ty from, const dlist_iter_ty to, 
					dlist_ty* dest,
					is_match_func_ty is_match, void* param              );

/******************************************************************************
* 	Description:
* 		Copies nodes of the second list from "from" to "to" ("to" not
*       included), to the first list before "where". Nodes remain in their
*       original order.
*   Return value:
* 		None
*   Undefinded behavior:
* 		"from", "to" or "where" are invaild iters
*  		"from" is DListEnd
*       "from" > "to"
*		"from" == "to"
*       "where" is between "from" and "to"
*   O(1)
******************************************************************************/
void DListSplice(dlist_iter_ty from, dlist_iter_ty to, dlist_iter_ty where);


#endif /* __ILRD_DLIST_H_ */
   

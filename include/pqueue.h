#ifndef __ILRD_PRIORITY_Q_H__
#define __ILRD_PRIORITY_Q_H__

typedef struct pq pq_ty;

typedef enum pq_status
{
    PQ_SUCCESS = 0,
    PQ_FAIL = 1
} pq_status_ty;

typedef enum bool
{
    PQ_FALSE = 0,
    PQ_TRUE = 1
} pq_bool_ty;

/****************************************************************************** 
*   Function compares "data1" and "data2", any other parameter may be
*		passed with "param".
*	Return value:
* 		0 > result if data1 comes before data2,
* 		0 < result if data2 comes before data1,
*		0 == result if data1 and data2 are equal
******************************************************************************/
typedef int (*pq_cmp_ty)(const void* data1, const void* data2, void* param);

/****************************************************************************** 
*   Function receives element's data ("data"), any other parameter may be
*		passed with "param".
*	Function should return 1 if match was found, or 0 otherwise.
******************************************************************************/
typedef int (*pq_is_match_ty)(const void* data, void* param);

/****************************************************************************** 
* 	Description:
* 		Creates an empty priority queue
* 	Return value:
* 		On success, a pointer to the created queue
* 		On failure, NULL
*   Undefined behavior:
* 		cmp is NULL
* 	O(1)
******************************************************************************/
pq_ty* PQCreate(pq_cmp_ty cmp, void* param);

/****************************************************************************** 
* 	Description:
* 		Destroys the "q"
* 		No operations performed if "q" is NULL
* 	Return value:
* 		None
*   O(n)
******************************************************************************/
void PQDestroy(pq_ty* q);

/****************************************************************************** 
* 	Description:
* 		Counts the size of "q"	  
* 	Return value: 
* 		Count of elements in "q"
* 	Undefined behavior: 
* 		"q" is NULL
*   O(n)
******************************************************************************/
size_t PQCount(const pq_ty* q);

/****************************************************************************** 
* 	Description:
* 		Checks if the "q" is empty
* 	Return value: 
* 		PQ_TRUE if empty, PQ_FALSE otherwise
* 	Undefined behavior:
* 		"q" is NULL
* 	O(1)
******************************************************************************/
pq_bool_ty PQIsEmpty(const pq_ty* q);

/****************************************************************************** 
* 	Description:
* 		Insert a new element to the queue
* 	Return value:	
* 		PQ_SUCCESS on success, PQ_FAIL on failure
* 	Undefined behaviour:
* 		"q" is NULL
*   O(1)
******************************************************************************/
pq_status_ty PQEnqueue(pq_ty* q, void* data);

/****************************************************************************** 
*   Description:
* 		Remove "q" head
* 	Return value: 
*		pointer to removed element
* 	Undefined behavior: 
* 		"q" is NULL
*       "q" is empty
*   O(1)
******************************************************************************/
void* PQDequeue(pq_ty* q);

/****************************************************************************** 
*   Description:
* 		Removes an element from "q"
* 	Return value: 
*		pointer to removed element's data
*       NULL if element was not found
* 	Undefined behavior: 
* 		"q" is NULL
*		"is_match" is NULL
*   O(n)
******************************************************************************/
void* PQRemove(pq_ty* q, pq_is_match_ty is_match, void* param);

/****************************************************************************** 
*   Description:
* 		Get first element of "q"
* 	Return value: 
*		pointer to "q" head
* 	Undefined behavior: 
* 		"q" is NULL
*       "q" is empty
*   O(1)
******************************************************************************/
void* PQPeek(const pq_ty* q);

#endif /* _ILRD_PRIORITY_Q_H__ */

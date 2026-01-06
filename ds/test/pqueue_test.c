#include <stdio.h>	/*	printf	*/
#include <string.h> /* 	strcmp	*/
#include "sorted_list.h"

#include "pqueue.h"

#define SUCCESS 			0
#define FAILURE				1
#define TRUE				1
#define FALSE				0
#define ARR_SIZE(x)			(sizeof(x)/sizeof(x[0]))
#define UNUSED(x) ((x) = (x))

static void PrintListOfInts(pq_ty* pq);
static void PrintListOfPersons(pq_ty* pq);
static int VerifyListPersons(const pq_ty* pq, int *expected_data, size_t size);
static int VerifyListOfInts(const pq_ty* pq, int *expected_data, size_t size);
static pq_ty* CreateListFromInts(int* arr, size_t size);

void TestPQCreateDestroy(void);
void TestPQEnqueueDequeue(void);
void TestPQRemovePeek(void);


typedef struct person
{
    int id;
    int age;
    char name[50];
} person_ty;

/* -------------------------------------------------------------------------
   					Match & Action Function Declarations
   ------------------------------------------------------------------------- */
int MatchById(const void* data, void* param);
int MatchByAge(const void* data, void* param);
int MatchByName(const void* data, void* param);

int AddToAge(void* data, void* param);
int AppendToName(void* data, void* param);
int ActionFailure(void* data, void* param);
/* -------------------------------------------------------------------------
   						Cmp Functions Implementations
   ------------------------------------------------------------------------- */
int  CmpInts( const void* data1, const void* data2, void* param)
{
	UNUSED(param); 
	return *(int*)data1 - *(int*)data2;
}
int CmpPersonsByAge(const void* data1, const void* data2, void* param)
{
	UNUSED(param); 
	return ((person_ty*)data1)->age - ((person_ty*)data2)->age;
}
int CmpPersonsById(const void* data1, const void* data2, void* param)
{
	UNUSED(param); 
	return ((person_ty*)data1)->id - ((person_ty*)data2)->id;
}

/* -------------------------------------------------------------------------
   							Helper Implementations
   ------------------------------------------------------------------------- */

int MatchById(const void* data, void* param)
{
    return (((person_ty*)data)->id == *(int*)param);
}

int MatchByAge(const void* data, void* param)
{
    return (((person_ty*)data)->age == *(int*)param);
}

int MatchByName(const void* data, void* param)
{
    return (0 == strcmp(((person_ty*)data)->name, (char*)param));
}

int MatchInt(const void* data, void* param)
{
	return *(int*)data == *(int*)param;
}

int AddToAge(void* data, void* param)
{
    ((person_ty*)data)->age += *(int*)param;
    return SUCCESS;
}

int AppendToName(void* data, void* param)
{
    strcat(((person_ty*)data)->name, (char*)param);
    return SUCCESS;
}

int ActionFailure(void* data, void* param)
{
	if (((person_ty*)data)->id == 2)
	{
		return FAILURE;	
	}

	AddToAge(data, param);
	return SUCCESS;
}

/* -------------------------------------------------------------------------
   									Main
   ------------------------------------------------------------------------- */

int main()
{
	TestPQCreateDestroy();
	TestPQEnqueueDequeue();
	TestPQRemovePeek();
	return 0;
}

/* -------------------------------------------------------------------------
						Test Functions Implementantions 
   ------------------------------------------------------------------------- */

void TestPQCreateDestroy(void)
{
	pq_ty* pq = PQCreate(CmpInts, NULL);
	PQDestroy(pq);
	pq = NULL;
	/* check vlg */
	printf("Finished TestPQCreateDestroy\n\n");
}
/* -------------------------------------------------------------------------*/
void TestPQEnqueueDequeue(void)
{
	int arr_test[] = {3,1,2};
	int arr_expected[] = {1,2,3};
	size_t i = 0;
	size_t expected_size = 3;
	pq_ty* pq = CreateListFromInts(arr_test, ARR_SIZE(arr_test));
	VerifyListOfInts(pq, arr_expected, ARR_SIZE(arr_expected));

	for (i = 0; i < ARR_SIZE(arr_expected); ++i, --expected_size)
	{
		if (expected_size != PQCount(pq))
		{
			printf("PQ Count failed! Expected %lu, Got: %lu\n\n",expected_size , PQCount(pq));
		}

		if (PQIsEmpty(pq))
		{
			printf("PQ IsEmpty failed! Expected False, Got: True\n\n");
		}
		
		if (*(int*)PQDequeue(pq) != arr_expected[i])
		{
			printf("Dequeue failed! Expected: %d, Got: %d\n\n",arr_expected[i], *(int*)PQDequeue(pq));
		}
		
	}

	if (!PQIsEmpty(pq))
	{
		printf("PQ IsEmpty failed! Expected True, Got: False\n\n");
	}
	
	PQDestroy(pq);
	pq = NULL;
	
	printf("Finished TestPQEnqueueDequeue\n\n");
}
/* -------------------------------------------------------------------------*/
void TestPQRemovePeek(void)
{
	int arr_test[] = {3,1,2};
	int arr_expected[] = {1,2,3};
	size_t i = 0;
	size_t expected_size = 3;
	pq_ty* pq = CreateListFromInts(arr_test, ARR_SIZE(arr_test));
	
	/* remove 2 and expected to see 1 at the head */
	PQRemove(pq, MatchInt, &arr_test[2]);
	if (*(int*)PQPeek(pq) != 1)
	{
		printf("PQ remove failed! Expected %d, Got: %d \n\n",1 ,*(int*)PQPeek(pq));
	}

	/* remove 1 and expected to see 3 at the head */
	PQRemove(pq, MatchInt, &arr_test[1]);
	if (*(int*)PQPeek(pq) != 3)
	{
		printf("PQ remove failed! Expected %d, Got: %d \n\n",3 ,*(int*)PQPeek(pq));
	}
	
	/* remove 3 and expected to see empty queue */
	PQRemove(pq, MatchInt, &arr_test[0]);
	if (!PQIsEmpty(pq))
	{
		printf("PQ remove! Expected empty list\n\n");
	}
	
	PQDestroy(pq);
	printf("Finished TestPQRemovePeek\n\n");
}


/* -------------------------------------------------------------------------*/
/* Helper to check if a list matches an array of expected integers */
static int VerifyListPersons(const pq_ty* pq, int *expected_data, size_t size)
{
	sorted_list_ty* list = *(sorted_list_ty**)pq;

    sorted_list_iter_ty iter = SortedListBeginIter(list);
    sorted_list_iter_ty end = SortedListEndIter(list);
	person_ty* cur_person = NULL;
    size_t i = 0;

	
    while (!SortedListIterIsEqual(iter, end) && i < size)
    {
		cur_person = (person_ty*)SortedListIterGetData(iter);
        if (cur_person->id != expected_data[i])
        {
            printf("Mismatch at index %lu: Expected %d, Found %d\n", 
                   i, expected_data[i], cur_person->id);
            return FALSE;
        }
        iter = SortedListIterNext(iter);
        i++;
    }

    /* Check if lengths match (list shouldn't have extra nodes, nor array extra data) */
    if (!SortedListIterIsEqual(iter, end) || i < size)
    {
        printf("Length mismatch. Iterated %lu items, expected %lu.\n", i, size);
        return FALSE;
    }

    return TRUE; 
}
/*****************************************************************************/
static int VerifyListOfInts(const pq_ty* pq, int *expected_data, size_t size)
{
	sorted_list_ty* list = *(sorted_list_ty**)pq;

    sorted_list_iter_ty iter = SortedListBeginIter(list);
    sorted_list_iter_ty end = SortedListEndIter(list);
	int* cur_int = NULL;
    size_t i = 0;

	
    while (!SortedListIterIsEqual(iter, end) && i < size)
    {
		cur_int = (int*)SortedListIterGetData(iter);
        if (*cur_int != expected_data[i])
        {
            printf("Mismatch at index %lu: Expected %d, Found %d\n", 
                   i, expected_data[i], *cur_int);
            return FALSE;
        }
        iter = SortedListIterNext(iter);
        i++;
    }

    /* Check if lengths match (list shouldn't have extra nodes, nor array extra data) */
    if (!SortedListIterIsEqual(iter, end) || i < size)
    {
        printf("Length mismatch. Iterated %lu items, expected %lu.\n", i, size);
        return FALSE;
    }

    return TRUE; 
}
/*****************************************************************************/
static pq_ty* CreateListFromInts(int* arr, size_t size)
{
    size_t i = 0;
    pq_ty* list = PQCreate(CmpInts, NULL);

    if (NULL == list)
    {
        return NULL;
    }

    for (i = 0; i < size; ++i)
    {
        PQEnqueue(list, &arr[i]);
    }

    return list;
}
/*****************************************************************************/
static void PrintListOfInts(pq_ty* pq)
{
	sorted_list_ty* list  = *(sorted_list_ty**)pq;
	sorted_list_iter_ty cur_iter = SortedListBeginIter(list);

	while(!SortedListIterIsEqual(cur_iter, SortedListEndIter(list)))
	{
		printf("%d --> ", *(int*)SortedListIterGetData(cur_iter));
		cur_iter = SortedListIterNext(cur_iter);
	}
	printf("END\n\n");
}
/*****************************************************************************/
static void PrintListOfPersons(pq_ty* pq)
{
	sorted_list_ty* list = *(sorted_list_ty**)pq;
	sorted_list_iter_ty cur_iter = SortedListBeginIter(list);
	person_ty* cur_person = NULL;

	while(!SortedListIterIsEqual(cur_iter, SortedListEndIter(list)))
	{
		cur_person = (person_ty*)SortedListIterGetData(cur_iter);
		printf("ID: %d Age: %d Name: %s --> ",cur_person->id ,cur_person->age ,cur_person->name);
		cur_iter = SortedListIterNext(cur_iter);
	}
	printf("END\n\n");
}
/*****************************************************************************/



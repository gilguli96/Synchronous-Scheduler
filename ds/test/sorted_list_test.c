#include <stdio.h>	/*	printf	*/
#include <string.h> /* 	strcmp	*/

#include "sorted_list.h"

#define SUCCESS 			0
#define FAILURE				1
#define TRUE				1
#define FALSE				0
#define ARR_SIZE(x)			(sizeof(x)/sizeof(x[0]))
#define UNUSED(x) ((x) = (x))

static void PrintListOfInts(sorted_list_ty* list);
static void PrintListOfPersons(sorted_list_ty* list);
static int VerifyListPersons(const sorted_list_ty* list, int *expected_data, size_t size);
static int VerifyListInts(const sorted_list_ty* list, int *expected_data, size_t size);
static sorted_list_ty* CreateListFromInts(int* arr, size_t size);

void TestSortedListCreateDestroy(void);
void TestSortedListInsertRemove(void);
void TestSortedListFind(void);
void TestSortedListSet(void);
void TestSortedListMerge(void);

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
	TestSortedListCreateDestroy();
	TestSortedListInsertRemove();
	TestSortedListFind();
	TestSortedListSet();
	TestSortedListMerge();
	return 0;
}
/* -------------------------------------------------------------------------
						Test Functions Implementantions 
   ------------------------------------------------------------------------- */
void TestSortedListCreateDestroy(void)
{
    sorted_list_ty* list = SortedListCreate(CmpInts, NULL);
    if (NULL == list)
    {
        printf("TestSortedListCreateDestroy failed! List creation returned NULL.\n");
    }
	
	if(0 != SortedListCount(list))
	{
		printf("count should be 0 at creation\n\n");
	}
	
	SortedListDestroy(list);
    
	list = NULL;
	/*check valgrind here*/
}
/*****************************************************************************/
void TestSortedListInsertRemove(void)
{
	int num3 = 9;
	int num2 = 3;
	int num1 = -3;
	sorted_list_iter_ty ret_value;

    sorted_list_ty* list = SortedListCreate(CmpInts, NULL);
	
	SortedListInsert(list, &num3);
	SortedListInsert(list , &num1);
	SortedListInsert(list, &num2);
	/* list is 1 --> 2 --> 3 --> END */

	if(num1 != *(int*)SortedListIterGetData(SortedListBeginIter(list)))
	{
		printf(" TestSortedListInsert 1 failed Expected: %d Got: %d\n\n",
			  num1, *(int*)SortedListIterGetData(SortedListBeginIter(list)));
	}

	if (num2 != *(int*)(SortedListIterGetData(SortedListIterNext(SortedListBeginIter(list)))))
	{
		printf("TestSortedListInsert 2 failed Expected: %d Got: %d\n\n",
		num2, *(int*)(SortedListIterGetData(SortedListIterNext(SortedListBeginIter(list)))));
	}
	
	SortedListRemove(SortedListIterNext(SortedListBeginIter(list)));
	if (num3 != *(int*)(SortedListIterGetData(SortedListIterNext(SortedListBeginIter(list)))))
	{
		printf("TestSortedListInsert 3 failed failed Expected: %d Got: %d\n\n",
		num3, *(int*)(SortedListIterGetData(SortedListIterNext(SortedListBeginIter(list)))));
	}

	SortedListDestroy(list);
	list = NULL;
}
/*****************************************************************************/
void TestSortedListFind(void)
{
    person_ty p1 = {101, 25, "Alice"};
    person_ty p2 = {102, 30, "Bob"};
    person_ty p3 = {103, 35, "Charlie"};
	person_ty p4 = {104, 40, "David"};
    person_ty* found_person = NULL;
    
    int target_id = 102;          /* Bob's ID */
    int target_age = 35;          /* Charlie's Age */
    char target_name[] = "Alice"; /* Alice's Name */
    int missing_id = 999;         /* Non-existent ID */

    sorted_list_ty* list = SortedListCreate(CmpPersonsByAge, NULL);
    sorted_list_iter_ty end_iter = SortedListEndIter(list);
    sorted_list_iter_ty found_iter= {0};

    SortedListInsert(list, &p3); /* Charlie */
    SortedListInsert(list, &p2); /* Bob */
    SortedListInsert(list, &p1); /* Alice */

	found_person = (person_ty*)SortedListIterGetData(SortedListFind(list, &p2));
	
	if (found_person->id != 102)
	{
		printf("TestSortedListFind 1 Expected: %d Got: %d\n\n",102, found_person->id);
	}
	
	if (!SortedListIterIsEqual(SortedListFind(list, &p4), end_iter))
	{
		printf("TestSortedListFind 2 expected to get EndIter when Find not find\n\n");
	}

}
/*****************************************************************************/
void TestSortedListSet(void)
{
    person_ty p1 = {101, 25, "Alice"};
    person_ty p2 = {103, 30, "Bob"};
    person_ty p3 = {105, 35, "Charlie"};
	person_ty p4 = {107, 40, "David"};

	person_ty p5 = {106, 35, "Bob"}; 	/* adjusted Bob with id 106   */
	person_ty p6 = {108, 25, "Alice"};	/* adjusted Alice with id 108 */
    person_ty* found_person = NULL;
    
	int expected_data[] = {105, 106, 107, 108};

    sorted_list_ty* list = SortedListCreate(CmpPersonsById, NULL);
    sorted_list_iter_ty end_iter = SortedListEndIter(list);
    sorted_list_iter_ty found_iter= {0};
	
    SortedListInsert(list, &p3); /* Charlie */
    SortedListInsert(list, &p2); /* Bob */
    SortedListInsert(list, &p1); /* Alice */
	SortedListInsert(list, &p4); /* David */


	SortedListIterSetData(list, SortedListIterNext(SortedListBeginIter(list)), &p5); /* set bob to id 106 */
	found_person = (person_ty*)SortedListIterGetData(SortedListIterPrev(SortedListIterPrev(SortedListEndIter(list))));


	if (0 != strcmp(found_person->name, "Bob"))
	{
		printf("TestSortedListFind 1 Expected: %s Got: %s\n\n","Bob", found_person->name);
	}

	SortedListIterSetData(list, SortedListBeginIter(list), &p6); /* set Alice to id 108 */
	VerifyListPersons(list, expected_data, 4);

}
/*****************************************************************************/
void TestSortedListMerge(void)
{
    sorted_list_ty* dest = NULL;
    sorted_list_ty* src = NULL;
    

    /* Case 1: (1,3,5 + 2,4,6) */
    {
        int arr1[] = {1, 3, 5};
        int arr2[] = {2, 4, 6};
        int exp[] =  {1, 2, 3, 4, 5, 6};
        
        dest = CreateListFromInts(arr1, ARR_SIZE(arr1));
        src = CreateListFromInts(arr2, ARR_SIZE(arr2));
        
        SortedListMerge(dest, src);
        
        if (FALSE == VerifyListInts(dest, exp, ARR_SIZE(exp)) && SortedListIsEmpty(src))
		{
             printf("Case 1 (Interleaved): FAIL\n");
		}	
         
        SortedListDestroy(dest);
        SortedListDestroy(src);
    }

    /* Case 2: Duplicates */
    {
        int arr1[] = {2, 2, 5};
        int arr2[] = {2, 5, 5};
        int exp[] =  {2, 2, 2, 5, 5, 5};
        
        dest = CreateListFromInts(arr1, ARR_SIZE(arr1));
        src = CreateListFromInts(arr2, ARR_SIZE(arr2));
        
        SortedListMerge(dest, src);
        
        if (FALSE == VerifyListInts(dest, exp, ARR_SIZE(exp)) && SortedListIsEmpty(src))
		{
            printf("Case 2 (Duplicates): FAIL\n");
		}
        SortedListDestroy(dest);
        SortedListDestroy(src);
    }

    /* Case 3: Src Larger than Dest */
    {
        int arr1[] = {1, 2};
        int arr2[] = {5, 6, 7};
        int exp[] =  {1, 2, 5, 6, 7};
        
        dest = CreateListFromInts(arr1, ARR_SIZE(arr1));
        src = CreateListFromInts(arr2, ARR_SIZE(arr2));
        
        SortedListMerge(dest, src);
        
        if (FALSE == VerifyListInts(dest, exp, ARR_SIZE(exp)) && SortedListIsEmpty(src))
		{
            printf("Case 3 (Src All Larger): FAIL\n");
		}
        SortedListDestroy(dest);
        SortedListDestroy(src);
    }
}
/*****************************************************************************/
/* Helper to check if a list matches an array of expected integers */
static int VerifyListPersons(const sorted_list_ty* list, int *expected_data, size_t size)
{
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
static int VerifyListInts(const sorted_list_ty* list, int *expected_data, size_t size)
{
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
static sorted_list_ty* CreateListFromInts(int* arr, size_t size)
{
    size_t i = 0;
    sorted_list_ty* list = SortedListCreate(CmpInts, NULL);

    if (NULL == list)
    {
        return NULL;
    }

    for (i = 0; i < size; ++i)
    {
        SortedListInsert(list, &arr[i]);
    }

    return list;
}
/*****************************************************************************/
static void PrintListOfInts(sorted_list_ty* list)
{
	sorted_list_iter_ty cur_iter = SortedListBeginIter(list);

	while(!SortedListIterIsEqual(cur_iter, SortedListEndIter(list)))
	{
		printf("%d --> ", *(int*)SortedListIterGetData(cur_iter));
		cur_iter = SortedListIterNext(cur_iter);
	}
	printf("END\n\n");
}
/*****************************************************************************/
static void PrintListOfPersons(sorted_list_ty* list)
{
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

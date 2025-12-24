#include <stdio.h>	/*	printf	*/
#include <string.h> /* 	strcmp	*/

#include "dlist.h"

#define SUCCESS 	0
#define FAILURE		1
#define TRUE		1
#define FALSE		0
#define ARR_SIZE(x)			(sizeof(x)/sizeof(x[0]))

static void PrintListOfInts(dlist_ty* list);
static void PrintListOfPersons(dlist_ty* list);
static int VerifyListContent(const dlist_ty *list, int *expected_data, size_t size);

void TestDListCreateDestroy(void);
void TestDListInsertRemove(void);
void TestDListPushPop(void);
void TestDListForEach(void);
void TestDListFind(void);
void TestDListMultiFind(void);
void TestDListSplice(void);

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
/*****************************************************************************/
static void PrintListOfInts(dlist_ty* list)
{
	dlist_iter_ty cur_iter = DListBeginIter(list);

	while(cur_iter != DListEndIter(list))
	{
		printf("%d --> ", *(int*)DListIterGetData(cur_iter));
		cur_iter = DListIterNext(cur_iter);
	}
	printf("END\n\n");
}
/*****************************************************************************/
static void PrintListOfPersons(dlist_ty* list)
{
	dlist_iter_ty cur_iter = DListBeginIter(list);
	person_ty* cur_person = NULL;

	while(cur_iter != DListEndIter(list))
	{
		cur_person = (person_ty*)DListIterGetData(cur_iter);
		printf("ID: %d Age: %d Name: %s --> ",cur_person->id ,cur_person->age ,cur_person->name);
		cur_iter = DListIterNext(cur_iter);
	}
	printf("END\n\n");
}
/*****************************************************************************/
int main()
{
	TestDListCreateDestroy();
	TestDListInsertRemove();
	TestDListPushPop();
	TestDListForEach();
	TestDListFind();
	TestDListMultiFind();
	TestDListSplice();
	return 0;
}
/*****************************************************************************/
void TestDListCreateDestroy(void)
{
	int data = 7;
    dlist_ty* list = DListCreate();
    if (NULL == list)
    {
        printf("TestDListCreateDestroy failed! List creation returned NULL.\n");
    }
	
	DListInsertBefore(list ,DListEndIter(list), &data);

	if(DListCount(list) != 1 || DListIsEmpty(list))
	{
		printf("(DlistCount) Expected: 1 Got: %lu (DListIsEmpty) Expected: 1 Got: %d\n\n",DListCount(list) ,DListIsEmpty(list));
	}
		
	DListRemove(DListBeginIter(list));

	if(DListCount(list) != 0 || !DListIsEmpty(list))
	{
		printf("(DlistCount) Expected: 0 Got: %lu (DListIsEmpty) Expected: 1 Got: %d\n\n",DListCount(list) ,DListIsEmpty(list));
	}
	
    DListDestroy(list);
	list = NULL;
	/*check valgrind here*/
}
/*****************************************************************************/
void TestDListInsertRemove(void)
{
	int num3 = 3;
	int num2 = 2;
	int num1 = 1;
	dlist_iter_ty ret_value = NULL;

    dlist_ty* list = DListCreate();
	
	DListInsertBefore(list ,DListEndIter(list), &num3);
	/* list is 3 - > END */

	if(num3 != *(int*)DListIterGetData(DListBeginIter(list)))
	{
		printf(" 58 DListIterGetData(DListBeginIter(list) failed Expected: %d Got: %d\n\n",
			  num3, *(int*)DListIterGetData(DListBeginIter(list)));
	}

	DListInsertBefore(list ,DListIterPrev(DListEndIter(list)), &num1);
	/* list is 1 -> 3 - > END */

	if (num1 != *(int*)DListIterGetData(DListBeginIter(list)))
	{
		printf("66 DListIterGetData(DListBeginIter(list) failed Expected: %d Got: %d\n\n",
		num1, *(int*)DListIterGetData(DListBeginIter(list)));
	}

	ret_value = DListInsertBefore(list ,DListIterPrev(DListEndIter(list)), &num2);
	/* list is 1 -> 2 -> 3 - > END */

	if (num2 != *(int*)DListIterGetData(ret_value))
	{
		printf("75: Expected %d Got %d\n\n"
		, num2 , *(int*)DListIterGetData(ret_value));
	}

	ret_value = DListRemove(ret_value);
	/* list is 1 -> 3 - > END */
	
	if ( num3 != *(int*)DListIterGetData(ret_value))
	{
		printf("84: Expected %d Got %d\n\n"
		, num2 , *(int*)DListIterGetData(ret_value));
	}

	DListDestroy(list);
	list = NULL;
}
/*****************************************************************************/
void TestDListPushPop(void)
{
	int arr_test[] = {1,2,3};
    dlist_ty* list = DListCreate();
	dlist_iter_ty cur_iter = NULL;
	int i = 0;
	
	/* push back and check last item in the list */
	for (i = 0; i < (int)ARR_SIZE(arr_test); ++i)
	{
		DListPushBack(list, &arr_test[i]);
		cur_iter = DListIterPrev(DListEndIter(list));
		if (arr_test[i] != *(int*)DListIterGetData(cur_iter))
		{
				printf("TestDListPushPop 1: Expected %d Got %d\n\n"
		   ,arr_test[i],*(int*)DListIterGetData(cur_iter));
		}		
	}
	/*  check last item in the list -> pop last -> check again last item*/
	for (i = ARR_SIZE(arr_test) - 1; i >= 0 ; --i)
	{

		cur_iter = DListIterPrev(DListEndIter(list));
		if (arr_test[i] != *(int*)DListIterGetData(cur_iter))
		{
				printf("TestDListPushPop 2: Expected %d Got %d\n\n"
		   ,arr_test[i],*(int*)DListIterGetData(cur_iter));
		}
		DListPopBack(list);
	}
	/* push front and check first item in the list */
	for (i = (int)ARR_SIZE(arr_test) - 1; i >= 0; --i)
	{
		DListPushFront(list, &arr_test[i]);
		cur_iter = DListBeginIter(list);
		if (arr_test[i] != *(int*)DListIterGetData(cur_iter))
		{
				printf("TestDListPushPop 3: Expected %d Got %d\n\n"
		   ,arr_test[i],*(int*)DListIterGetData(cur_iter));
		}		
	}
	/*  check first item in the list -> pop first -> check again first item*/
	for (i = 0 ; i < (int)ARR_SIZE(arr_test) ; ++i)
	{

		cur_iter = DListBeginIter(list);
		if (arr_test[i] != *(int*)DListIterGetData(cur_iter))
		{
				printf("TestDListPushPop 4: Expected %d Got %d\n\n"
		   ,arr_test[i],*(int*)DListIterGetData(cur_iter));
		}
		DListPopFront(list);
	}
	DListDestroy(list);
}

void TestDListForEach(void)
{
    person_ty p1 = {1, 20, "Alice"};
    person_ty p2 = {2, 20, "Bob"};
    person_ty p3 = {3, 20, "Charlie"};
    
    int age_increment = 5;
    char suffix[] = "_AddedSuffix";
    int status = -1;

    dlist_ty* list = DListCreate();
    dlist_iter_ty iter = NULL;
    person_ty* p_data = NULL;

    /* Create List: p1 -> p2 -> p3 */
    iter = DListInsertBefore(list, DListBeginIter(list), &p3);
    iter = DListInsertBefore(list, iter, &p2);
    iter = DListInsertBefore(list, iter, &p1);

    /* TEST 1: Add Age  */
    /* Add 5 to everyone's age. Expected result: 25 for all. */
    status = DListForEach(DListBeginIter(list), DListEndIter(list), AddToAge, &age_increment);
    
    if (SUCCESS != status)
    {
        printf("TestDListForEach (AddAge) failed! Function returned FAILURE status.\n");
    }

    /* Verify p1 */
    iter = DListBeginIter(list);
    p_data = (person_ty*)DListIterGetData(iter);
    if (p_data->age != 25)
    {
        printf("TestDListForEach (AddAge) failed! Node 1: Expected 25, Got %d\n", p_data->age);
    }
	if (0 != strcmp(p_data->name, "Alice"))
	{
		printf("TestDListForEach: (Insertion) Expected: %s, Got: %s","Alice" ,p_data->name);
	}

    /* Verify p2 */
    iter = DListIterNext(iter);
    p_data = (person_ty*)DListIterGetData(iter);
    if (p_data->age != 25)
    {
        printf("TestDListForEach (AddAge) failed! Node 2: Expected 25, Got %d\n", p_data->age);
    }

	if (0 != strcmp(p_data->name, "Bob"))
	{
		printf("TestDListForEach: (Insertion) Expected: %s, Got: %s", "Bob" ,p_data->name);
	}

    /* TEST 2: Modify String (Name) */
    /* Append "_AddedSuffix" to all users name. Expected: "User_AddedSuffix */
    status = DListForEach(DListBeginIter(list), DListEndIter(list), AppendToName, suffix);

    if (SUCCESS != status)
    {
        printf("TestDListForEach (AppendName) failed! Function returned failure status.\n");
    }

    /* Verify p1 Name */
    iter = DListBeginIter(list);
    p_data = (person_ty*)DListIterGetData(iter);
    if (strcmp(p_data->name, "Alice_AddedSuffix") != 0)
    {
        printf("TestDListForEach (AppendName) failed! Node 1: Expected 'Alice_AddedSuffix', Got '%s'\n", p_data->name);
    }

    /* Verify p2 Name */
    iter = DListIterNext(iter);
    p_data = (person_ty*)DListIterGetData(iter);
    if (strcmp(p_data->name, "Bob_AddedSuffix") != 0)
    {
        printf("TestDListForEach (AppendName) failed! Node 1: Expected 'Bob_AddedSuffix', Got '%s'\n", p_data->name);
    }

    /* TEST 3: Failed Action Return Value */
    /* activate an action function that returns FAILURE */
    status = DListForEach(
	DListBeginIter(list), DListEndIter(list), ActionFailure, &age_increment);

    if (FAILURE != status)
    {
        printf("TestDListForEach (action failure) failed! sreturned success status.\n");
    }
	
	iter = DListBeginIter(list);
	p_data = (person_ty*)DListIterGetData(iter);
	if( p_data->age != 30 )
	{
	 	printf("TestDListForEach (AddAge) failed! Node 1: Expected 30, Got %d\n", p_data->age);
	}

	iter = DListIterNext(iter);
	p_data = (person_ty*)DListIterGetData(iter);
	if( p_data->age != 25 )
	{
	 	printf("TestDListForEach (AddAge) failed! Node 2: Expected 25, Got %d\n", p_data->age);
	}
	
    DListDestroy(list);
}
/*****************************************************************************/
void TestDListFind(void)
{
	
    person_ty p1 = {101, 25, "Alice"};
    person_ty p2 = {102, 30, "Bob"};
    person_ty p3 = {102, 35, "Charlie"};
    
    int target_id = 102;          /* Bob's ID */
    int target_age = 35;          /* Charlie's Age */
    char target_name[] = "Alice"; /* Alice's Name */
    int missing_id = 999;         /* Non-existent ID */

    dlist_ty* list = DListCreate();
    dlist_iter_ty start_iter = DListEndIter(list);
    dlist_iter_ty found_iter = NULL;
    person_ty* found_data = NULL;

    /* Create list: Alice -> Bob -> Charlie -> [Dummy] */
    start_iter = DListInsertBefore(list, start_iter, &p3); /* Charlie */
    start_iter = DListInsertBefore(list, start_iter, &p2); /* Bob */
    start_iter = DListInsertBefore(list, start_iter, &p1); /* Alice */

    /* TEST 1: Find by ID (Middle Element: Bob) */
    found_iter = DListFind(DListBeginIter(list), DListEndIter(list), MatchById, &target_id);
    found_data = (person_ty*)DListIterGetData(found_iter);

    if (found_data->id != target_id)
    {
        printf("TestDListFind (ID) failed! Found ID %d instead of %d\n", 
               found_data->id, target_id);
    }
    /* TEST 2: Find by Age (Last Element: Charlie)*/
    found_iter = DListFind(DListBeginIter(list), DListEndIter(list), MatchByAge, &target_age);
    found_data = (person_ty*)DListIterGetData(found_iter);

    if (found_data->age != target_age)
    {
        printf("TestDListFind (Age) failed! Found Age %d instead of %d\n", 
               found_data->age, target_age);
    }

    /* TEST 3: Find by Name (Head Element: Alice) */
    found_iter = DListFind(DListBeginIter(list), DListEndIter(list), MatchByName, target_name);
    found_data = (person_ty*)DListIterGetData(found_iter);

    if (strcmp(found_data->name, target_name) != 0)
    {
        printf("TestDListFind (Name) failed! Found Name %s instead of %s\n", 
               found_data->name, target_name);
    }
    /* TEST 4: Find Missing Element (Should Return Bob ('to')) */
    found_iter = DListFind(
	DListBeginIter(list), DListIterNext(DListBeginIter(list)), MatchById, &missing_id);

    if (!DListIterIsEqual(found_iter,DListIterNext(DListBeginIter(list))))
    {
        printf("TestDListFind (Missing) failed! expected to receive: %s instead got: %s\n\n",
		((person_ty*)DListIterGetData(DListIterNext(DListBeginIter(list))))->name, 		((person_ty*)DListIterGetData(found_iter))->name);
    }

    /* Cleanup */
    DListDestroy(list);
}
/*****************************************************************************/
void TestDListMultiFind(void)
{
	person_ty p1 = {101, 25, "Alice"};
    person_ty p2 = {102, 30, "Alice"};
    person_ty p3 = {102, 25, "Charlie"};
    
    int target_id = 102;          /* Alice And Charlie's ID */
    char target_name[] = "Alice"; /* Alice's Name */
	int ret_value_multi = 0;

    dlist_ty* list = DListCreate();
	dlist_ty* list_res = DListCreate();
    dlist_iter_ty start_iter = DListEndIter(list);
	dlist_iter_ty iter = NULL;
    person_ty* found_data = NULL;

    /* Create list: Alice -> Alice -> Charlie -> [Dummy] */
    start_iter = DListInsertBefore(list, start_iter, &p3); /* Charlie */
    start_iter = DListInsertBefore(list, start_iter, &p2); /* Alice */
    start_iter = DListInsertBefore(list, start_iter, &p1); /* Alice */
	
    /* TEST 1: Find by ID (Supposed to find p2 only) */
  	ret_value_multi = DListMultiFind(DListBeginIter(list), DListIterPrev(DListEndIter(list)), list_res, MatchById, &target_id);

	if (SUCCESS != ret_value_multi)
	{
		printf("expected ret_value_multi to be SUCCESS!\n\n");
	}

	
	if (1 != DListCount(list_res))
	{
		printf("TestDListMultiFind 1 expected to find only 1 item in list_res! \n\n");
	}

	found_data = (person_ty*)DListIterGetData(DListBeginIter(list_res));
	
	if (found_data->age != 30)
	{
		printf("TestDListMultiFind 2 Expected: %d Got: %d\n\n",30, found_data->age);
	}

	/* TEST 2: Find by Name (Supposed to find p1 and p2) */
	DListDestroy(list_res);
	list_res = DListCreate();

    ret_value_multi = DListMultiFind(DListBeginIter(list), DListIterPrev(DListEndIter(list)) ,list_res,  MatchByName, &target_name);

	if (2 != DListCount(list_res))
	{
		printf("TestDListMultiFind 3 expected to find 2 items in list_res! \n\n");
	}

	found_data = (person_ty*)DListIterGetData(DListBeginIter(list_res));

	if (101 != found_data->id)
	{
		printf("TestDListMultiFind 4 Expected: %d Got: %d\n\n",101, found_data->id);
	}

	found_data = (person_ty*)DListIterGetData(DListIterPrev(DListEndIter(list_res)));

	if (102 != found_data->id)
	{
		printf("TestDListMultiFind 5 Expected: %d Got: %d\n\n",102, found_data->id);
	}

    DListDestroy(list);
	DListDestroy(list_res);
}


void TestDListSplice(void)
{
    dlist_ty *dest_list = DListCreate();
    dlist_ty *src_list = DListCreate();
    
    /* Iterators */
    dlist_iter_ty where_iter;
    dlist_iter_ty from_iter;
    dlist_iter_ty to_iter;

    /* Test Data */
    int data_dest[] = {1, 2, 5, 6};      /* We will insert between 2 and 6 */
    int data_src[] = {3, 4, 8};          /* 3 - 4 will be injected */
    int expected_result[] = {1, 2, 3, 4, 5, 6, 7};
    
    size_t i = 0;

    /* Destination List: {1, 2, 5, 6} */
    for (i = 0; i < sizeof(data_dest)/sizeof(int); ++i)
    {
        DListPushBack(dest_list, &data_dest[i]);
    }

    /* Source List: {3, 4, 8} */
    for (i = 0; i < sizeof(data_src)/sizeof(int); ++i)
    {
        DListPushBack(src_list, &data_src[i]);
    }

    
    /* Find 'where' (the node containing 6) */
    where_iter = DListBeginIter(dest_list);  	/* points to 1  */
    where_iter = DListIterNext(where_iter); 	/* points to 2  */
    where_iter = DListIterNext(where_iter); 	/* points to 5  */
    
    /* moving the whole source list */
    from_iter = DListBeginIter(src_list);
    to_iter = DListIterPrev(DListEndIter(src_list));

    /* Perform Splice */
    /* moves {3, 4, 5} to before {6} */
    DListSplice(from_iter, to_iter, where_iter);
    /* 5. Verify Destination List contains {1, 2, 3, 4, 5, 6} */
    if (!VerifyListContent(dest_list, expected_result, 6))
    {
        printf("List 1 Content: FAILED\n");
    }

    /* Verify Source List size is 1  */
    if (1 != DListCount(src_list))
    {
        printf("List 2 Count:   FAILED (Size is %lu)\n", DListCount(src_list));
    }

    /* Cleanup */
    DListDestroy(dest_list);
    DListDestroy(src_list);
}

/* Helper to check if a list matches an array of expected integers */
static int VerifyListContent(const dlist_ty *list, int *expected_data, size_t size)
{
    dlist_iter_ty iter = DListBeginIter(list);
    dlist_iter_ty end = DListEndIter(list);
    size_t i = 0;

    while (!DListIterIsEqual(iter, end) && i < size)
    {
        if (*(int *)DListIterGetData(iter) != expected_data[i])
        {
            printf("Mismatch at index %lu: Expected %d, Found %d\n", 
                   i, expected_data[i], *(int *)DListIterGetData(iter));
            return FALSE;
        }
        iter = DListIterNext(iter);
        i++;
    }

    /* Check if lengths match (list shouldn't have extra nodes, nor array extra data) */
    if (!DListIterIsEqual(iter, end) || i < size)
    {
        printf("Length mismatch. Iterated %lu items, expected %lu.\n", i, size);
        return FALSE;
    }

    return TRUE; 
}


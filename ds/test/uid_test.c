#include <stdio.h> /* printf */

#define TRUE 	1
#define FALSE 	0

#include "uid.h"

int main ()
{
	uid_ty id_1 = UidCreate();
	uid_ty id_2 = UidCreate();
	
	if (IsMatchId(id_1, id_2) == TRUE)
	{
		printf("1 Uid IsMatch failed ! expected id_1 != id_2\n\n");
		return 1;
	}

	if (IsMatchId(id_1, id_1) == FALSE)
	{
		printf("2 Uid IsMatch failed ! expected id_1 == id_1\n\n");
		return 1;
	}

	if (id_1.counter != 1 || id_2.counter != 2)
	{
		printf("counter assignment failed!\n\n");
	}

	printf("my IP is: %s\n\n", id_1.ip);
	printf("All tests have passed ! \n\n");
	 
	return 0;
}

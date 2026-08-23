#ifndef __UID_H_
#define __UID_H_

#include <time.h>           /*  time_t              */
#include <sys/types.h>      /*  pid_t               */
#include <arpa/inet.h>		/*  INET_ADDRSTRLEN	    */

typedef struct uid
{
    time_t time;
    size_t counter;
    pid_t pid;
    char ip[INET_ADDRSTRLEN];
}uid_ty;

extern const uid_ty invalid_uid_g;

/******************************************************************************
*	Description:
*		Create a id
*	Return Value:
* 		uid on success, invalid_uid_g on failure
* 	O(1)
******************************************************************************/
uid_ty UidCreate(void);

/******************************************************************************
*	Description:
*		Compare id_1 and id_2
*	Return Value:
* 		1 if are equal, 0 otherwise
* 	O(1)
******************************************************************************/
int IsMatchId(uid_ty id_1, uid_ty id_2);

#endif  /* __UID_H_ */

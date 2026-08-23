#define _DEFAULT_SOURCE /* unlocks IFF_LOOPBACK */

#include <unistd.h> 	/* getpid */
#include <time.h>   	/* time */
#include <ifaddrs.h> 	/* getifaddrs, struct ifaddrs,  */
#include <net/if.h>  	/* IFF_LOOPBACK  */
#include <arpa/inet.h>  /* inet_ntop  */
#include <netinet/in.h> /* struct sockaddr_in */
#include <string.h> 	/* strncpy */
#include "uid.h"

#define stringify(x)	(#x)
#define DEFAULT_IP	stringify(127.0.0.1)
#define CHAR_BUFF_LEN(x)	sizeof(x)

static void GetIP(char *buffer, size_t buf_size);

const uid_ty invalid_uid_g = {0};

uid_ty UidCreate(void)
{
    static size_t count = 0;
    uid_ty new_uid;

    new_uid.time = time(NULL);
	if (new_uid.time == (time_t)-1)
	{
		return invalid_uid_g;
	}
    new_uid.counter = ++count; 
    new_uid.pid = getpid();
	GetIP(new_uid.ip, CHAR_BUFF_LEN(new_uid.ip));

    return new_uid;
}

int IsMatchId(uid_ty id_1, uid_ty id_2)
{
    return ( 0 == difftime(id_1.time, id_1.time)) && 
           (id_1.counter == id_2.counter) && 
           (id_1.pid == id_2.pid);
}

static void GetIP(char *buffer, size_t buf_size)
{
    struct ifaddrs* ifaddr = {0};
    struct ifaddrs* ifa = {0};
    struct sockaddr_in* ipv4 = {0};

    /* Set default ip if it wont work */
    strncpy(buffer, DEFAULT_IP, buf_size);

	if (getifaddrs(&ifaddr) == -1)
	{
		return;
	}

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
	{
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET || ifa->ifa_flags & IFF_LOOPBACK) 
		{
            continue;
        }
            
        ipv4 = (struct sockaddr_in *)ifa->ifa_addr;

        /* Convert binary IP to string */
        inet_ntop(AF_INET, &(ipv4->sin_addr), buffer, buf_size);
		freeifaddrs(ifaddr);
        return;
	}
}

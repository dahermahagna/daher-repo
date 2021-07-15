/******************************************************************************
*				System Programing: network ping pong (UDP)                          
*				code by: Kira												  
* 				review by: 
*               test by: Kira
*				last edit: 5/1/21 					 					  
******************************************************************************/

#include <stdio.h>              /* printf() */
#include <stdlib.h>             /* atoi() */
#include <unistd.h>             /* sleep() */
#include <string.h>             /* strlen() */
#include <sys/types.h> 
#include <sys/socket.h>         /* socket(), */
#include <arpa/inet.h> 
#include <netinet/in.h>         /* sockaddr_in struct */
#include <errno.h>              /*  errno, perror() */

#include "udp_api.h"

#define STD_SERVER_ANS ("wazzzzZZZZUUUP!")

int main(int argc, char *argv[])
{
    int status = 0;
    fd_ty sock_fd = 0; 
    sockaddr_in_ty broadc_addr;
    int message_num = 0;
    int broad_perm = 1;
    
    status = BroadcastSockCreate(&sock_fd);
    status = SetBroadcasterSocket(sock_fd, &broad_perm);    
    BroadCAddrInit(&broadc_addr, argv[1], argv[2]);

    while(5 > message_num && SUCCESS == status)
    {
        status = MmgSendTo(sock_fd, (void *)&broadc_addr, STD_SERVER_ANS, BROADCAST);
        ++message_num;
        sleep(3);
    }

    close(sock_fd);

    (void)argc;
    (void)argv;

    return status;
}
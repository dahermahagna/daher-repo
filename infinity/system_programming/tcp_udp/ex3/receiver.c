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

#include "../udp_tcp.h"


int main(int argc, char *argv[])
{
    int status = 0;
    int sock_fd = 0; 
    char msg_buff[MAXLINE] = {0};
    struct sockaddr_in broadc_addr;
    unsigned int len = 0;
    ssize_t msg_end = 0;

    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (0 > sock_fd)
    {
        perror("socket creation failed");
        return SOCKET_ERR;
    }

    memset(&broadc_addr, 0, sizeof(broadc_addr));

    broadc_addr.sin_family = AF_INET;                          /* Internet address family */
    broadc_addr.sin_addr.s_addr = htonl(INADDR_ANY);           /* Address to accept any incoming messages. */
    broadc_addr.sin_port = htons(PORT);                        /* Broadcast port */


    if(0 > bind(sock_fd,(const struct sockaddr*)&broadc_addr,sizeof(broadc_addr)))
    {
        perror("bind failed");
        return BIND_ERR;
    }
    if (0 > (msg_end = recvfrom(sock_fd, msg_buff, MAXLINE, MSG_WAITALL, NULL, &len)))
    {
        perror("message receive error:");
        return RECV_ERR;
    }
    msg_buff[msg_end] = '\0';
    printf("Message: %s\n", msg_buff);
    /* memset(input_buff, 0, msg_end); */

    close(sock_fd);

    (void)argc;
    (void)argv;

    return status;
}
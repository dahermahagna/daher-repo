/******************************************************************************
*				System Programing: network ping pong (UDP)							   
*				code by: Kira											    
* 				review by: 
* 				test by: kira                                               
*				last edit: 5/1/21						 				 
******************************************************************************/
#include <stdio.h>              /* printf() */
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>             /* strlen(), memset() */
#include <sys/types.h> 
#include <sys/socket.h>         /* socket(), */
#include <arpa/inet.h> 
#include <netinet/in.h>         /* sockaddr_in struct */
#include <errno.h>              /*  errno, perror() */

#include "udp_api.h"

/*#define CONFIRM_SENT  ("message sent.")*/

int DgramSockCreate(fd_ty *fd)
{
    if(0 > (*fd = socket(AF_INET, SOCK_DGRAM, 0))) 
    { 
        perror("socket creation failed"); 
        return SOCKET_ERR;
    } 
    return SUCCESS;
}

int BroadcastSockCreate(fd_ty *fd)
{
    if(0 > (*fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) 
    { 
        perror("socket creation failed"); 
        return SOCKET_ERR;
    } 
    return SUCCESS;
}

int SetBroadcasterSocket(fd_ty sock_fd, int *perm)
{
    if(0 > setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, (const void *)perm, sizeof(*perm)))
    {
        perror("socketset failed"); 
        return SOCKET_ERR;
    }
    return SUCCESS;
}

void BroadCAddrInit(sockaddr_in_ty *addr, char *broad_IP, char *port)
{
    memset(addr, 0, sizeof(*addr));

    addr->sin_family = AF_INET;                     /* Internet address family */
    addr->sin_addr.s_addr = inet_addr(broad_IP);    /* Broadcast IP address */
    addr->sin_port = htons(atoi(port));             /* Broadcast port */
}

void AddressInit(sockaddr_in_ty *addr)
{
    memset(addr, 0, sizeof(*addr)); 
}

void ServerAddressInit(sockaddr_in_ty *addr, char *port)
{
    addr->sin_family = AF_INET;  
    addr->sin_addr.s_addr = INADDR_ANY; 
    addr->sin_port = htons(atoi(port)); 
}

int SockBind(fd_ty sock_fd, const sockaddr_ty *sock_addr)
{
    if(0 > bind(sock_fd, sock_addr, sizeof(*sock_addr))) 
    { 
        perror("bind failed"); 
        return BIND_ERR; 
    }
    return SUCCESS; 
}

int MmgSendTo(fd_ty sock_fd, void *dest_addr, const char* msg_str, int flag)
{
    size_t msg_len = strlen(msg_str);
    socklen_t len = sizeof(*(sockaddr_in_ty*)dest_addr);
    int flg = MSG_CONFIRM * (flag);

    if(0 > sendto(sock_fd, msg_str, msg_len, flg, (const sockaddr_ty*)dest_addr, len))
    {
        return SEND_ERR;
    }

    return SUCCESS;
}

int MsgRecvFrom(fd_ty sock_fd, void *input_buff, size_t max_recv, void *src_addr)
{
    int msg_end = 0;
    socklen_t len = 0;

    if(NULL != src_addr)
    {
        len = sizeof(*(sockaddr_in_ty*)src_addr);
    }

    if(0 > (msg_end = recvfrom(sock_fd, input_buff, max_recv, MSG_WAITALL,
                                            (sockaddr_ty*)src_addr, &len)))
    {
        perror("message receive error:");
        return RECV_ERR;
    }
    ((char *)input_buff)[msg_end] = '\0';
    printf("Message: %s\n", (char*)input_buff);
    memset(input_buff, 0, sizeof(*(char*)input_buff));

    return SUCCESS; 
}

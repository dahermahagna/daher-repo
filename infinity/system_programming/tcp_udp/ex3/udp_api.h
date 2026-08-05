/******************************************************************************
*				System Programing: network ping pong (UDP)							   
*				code by: Kira											    
* 				review by: 
* 				test by: kira                                               
*				last edit: 5/1/21						 				 
******************************************************************************/
#ifndef __UDP_API_H__
#define __UDP_API_H__

enum exit
{
    RECV_ERR = -4,
    SEND_ERR,
    BIND_ERR,
    SOCKET_ERR,
    SUCCESS
};

enum send_flag
{
   BROADCAST = 0,
   REG_UDP
};

typedef struct sockaddr_in sockaddr_in_ty;
typedef struct sockaddr sockaddr_ty;
typedef int fd_ty;

/** 
 * @DESCRIPTION: 
 *
 * @param:
 * 
 * @return: 
 * 
 */
int DgramSockCreate(fd_ty *fd);

int BroadcastSockCreate(fd_ty *fd);

void BroadCAddrInit(sockaddr_in_ty *addr, char *broad_IP, char *port);

int SetBroadcasterSocket(fd_ty sock_fd, int *perm);

void AddressInit(sockaddr_in_ty *addr);

void ServerAddressInit(sockaddr_in_ty *addr, char *port);

int SockBind(fd_ty sock_fd, const sockaddr_ty *sock_addr);

int MsgRecvFrom(fd_ty sock_fd, void *input_buff, size_t max_recv, void *src_addr);

int MmgSendTo(fd_ty sock_fd, void *dest_addr, const char* msg_str, int flag);

#endif /*__UDP_API_H__*/
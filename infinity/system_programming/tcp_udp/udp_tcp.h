#ifndef __UDP_TCP_H__
#define __UDP_TCP__

#define PORT 8080
#define MAXLINE 1024
#define SA struct sockaddr
enum exit
{
    RECV_ERR = -4,
    SEND_ERR,
    BIND_ERR,
    SOCKET_ERR,
    SUCCESS
};

#include <sys/socket.h>
/* DESCRIPTION: 
 * update server address
*/
void FillServerInfo(struct sockaddr_in *servaddr);

#endif /*__UDP_TCP__*/
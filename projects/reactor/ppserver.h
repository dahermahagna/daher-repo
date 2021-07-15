#ifndef __PPSERVER_H__
#define __PPSERVER_H__

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#define PORT 8080
#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MAXLINE (1024)

typedef enum exit_status
{
    ACCEPT_ERR = -6,
    SELECT_ERR,
    RECV_ERR,
    SEND_ERR,
    BIND_ERR,
    SOCKET_ERR,
    SUCCESS
} Exit_Status;

typedef enum stdin_result
{
    QUIT = 1,
    PING
    
} Stdin_Result;
int RunServer();
Exit_Status TCPSetBindAndlisten(struct sockaddr_in *servaddr, int *listenfd);
Exit_Status UDPSetAndBind(struct sockaddr_in *servaddr, int *udp_fd);
Exit_Status TCPHandler(int listenfd, fd_set *master, int *fdmax);
Exit_Status UDPHandler(int udp_fd);
Stdin_Result StdInHandler();
Exit_Status RespondHandler(int sockfd, fd_set *master, int *fdmax);

/* Exit_Status  */

#endif /*__PPSERVER_H__*/
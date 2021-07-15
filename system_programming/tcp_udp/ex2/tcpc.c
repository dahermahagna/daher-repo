#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "../udp_tcp.h"

void play(int sockfd)
{
    char buff[MAXLINE];
    int i = 0;
    while(10 > i)
    {
        buff[0] = 'p';
        buff[1] = 'i';
        buff[2] = 'n';
        buff[3] = 'g';
        buff[4] = '\0';
        write(sockfd, buff, sizeof(buff));
        sleep(1);
        memset(buff, 0, sizeof(buff));

        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s\n", buff);
        ++i;

    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    /* socket create and varification*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..(sockfd:%d)\n",sockfd);
    
    memset(&servaddr,0,sizeof(servaddr));
    /* assign IP, PORT*/
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    /* connect the client socket to server socket*/
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    play(sockfd);
    /* close the socket*/
    close(sockfd);
    return 0;
}
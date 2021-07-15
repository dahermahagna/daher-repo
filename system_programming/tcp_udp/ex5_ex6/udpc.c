#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../udp_tcp.h"

int main(int argc, char const *argv[])
{
    int i = 0;
    int sockfd;
    char buffer[MAXLINE];
    char *pong = "pong";
    struct sockaddr_in servaddr;

    int n;
    unsigned int len;

    /* Creating socket file descriptor*/
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Filling server information*/
    FillServerInfo(&servaddr);
    if (argc > 1)
    {
        servaddr.sin_port = htons(atoi(argv[1]));
    }
    while(10 > i)
    {
        sendto(sockfd, (const char *)pong, strlen(pong), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        printf("i received ping from :%d and sending pong to it.\n", servaddr.sin_port);

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
        ++i;
        sleep(1);
    }
    close(sockfd);
    return 0;
}
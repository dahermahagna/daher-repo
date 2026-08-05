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
    char *ping = "ping";
    struct sockaddr_in servaddr, cliaddr;

    int n;
    unsigned int len;

    /* Creating socket file descriptor*/
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("sockfd is: %d\n", sockfd);
    memset(&cliaddr, 0, sizeof(cliaddr));

    /* Filling server information*/

    FillServerInfo(&servaddr);
    if (argc > 1)
    {
        servaddr.sin_port = htons(atoi(argv[1]));
    }
    printf("servaddr.sin_port is: %d\n", servaddr.sin_port);
    /* Bind the socket with the server address*/
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    

    len = sizeof(cliaddr); /*len is value/resuslt*/
    printf("lets start to play ping pong, i'm sending ping.\n");
    while(10 > i)
    {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("the Client respond: %s\n", buffer);
        sendto(sockfd, (const char *)ping, strlen(ping), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("i'm sending Ping\n");
        ++i;
    }
    return 0;
}
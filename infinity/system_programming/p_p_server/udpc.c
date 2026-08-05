

#include "ppserver.h"

int main()
{
    int i = 0, rnd,rnd2;
    int sockfd;
    char buffer[MAXLINE];
    char *ping = "ping";
    struct sockaddr_in servaddr;

    int n;
    unsigned int len;
    srand(time(0));
    /* Creating socket file descriptor*/
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Filling server information*/
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;   /* IPv4*/
    servaddr.sin_port = htons(PORT); /* function converts the unsigned short integer netshort from network byte order to host byte order.*/
    servaddr.sin_addr.s_addr = INADDR_ANY;
    len = sizeof(servaddr);
    rnd = rand() % 8 + 3; 
    printf("%d pings to do\n",rnd);
    while (rnd > i)
    {
    
    sendto(sockfd, (const char *)ping, strlen(ping), 0, (const struct sockaddr *)&servaddr, len);
    printf("i received ping from :%d and sending pong to it.\n", servaddr.sin_port);

    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
    rnd2 = rand() % 8 + 3;
    printf("wait %d seconds before the next ping\n",rnd2);
    sleep(rnd2);
    ++i;
    }
    close(sockfd);
    return 0;
}
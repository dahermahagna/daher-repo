#include "ppserver.h"

int RunServer()
{
    fd_set master;    /* master file descriptor list*/
    fd_set read_fds;  /* temp file descriptor list for select()*/
    int fdmax = 0;    /* maximum file descriptor number*/
    int listened = 0; /* listening socket descriptor*/
    int udp_fd = 0;   /* udp server descriptor*/
    int status = 0, stdin_result = 0, select_result = 0, i = 0;
    struct sockaddr_in tcp_serv, udp_serv;
    struct timeval tv;

    status = UDPSetAndBind(&udp_serv, &udp_fd);
    if (SUCCESS != status)
    {
        return status;
    }
    status = TCPSetBindAndlisten(&tcp_serv, &listened);
    if (SUCCESS != status)
    {
        return status;
    }

    FD_ZERO(&master); /* clear the master and temp sets*/
    FD_ZERO(&read_fds);

    /* add the listened to the master set*/
    FD_SET(listened, &master);
    FD_SET(udp_fd, &master);
    FD_SET(STDIN_FILENO, &master);

    /* keep track of the biggest file descriptor*/
    fdmax = MAX(listened, udp_fd); /* so far, it's this one*/

    /* main loop*/
    while (1)
    {
        read_fds = master; /* copy it*/
        tv.tv_sec = 7;
        tv.tv_usec = 0;
        /*the update of fdmax will be in the loop*/
        select_result = select(fdmax + 1, &read_fds, NULL, NULL, &tv);
        if (0 > select_result)
        {
            perror("select");
            exit(4);
        }
        else if (0 == select_result)
        {
            printf("time out\n");
            continue;
        }

        /* run through the existing connections looking for data to read*/
        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            { /* we got one!!*/

                if (i == listened)
                {
                    status = TCPHandler(i, &master, &fdmax);
                }
                else if (i == udp_fd)
                {
                    status = UDPHandler(i);

                } /* END handle data from client*/
                else if (i == STDIN_FILENO)
                {
                    stdin_result = StdInHandler();
                    if (QUIT == stdin_result)
                    {

                        /*close file descriptor*/
                        return SUCCESS;
                    }
                }
                else
                {
                    status = RespondHandler(i, &master, &fdmax);
                }
            }
        }
    }

    return 0;
}

Exit_Status TCPSetBindAndlisten(struct sockaddr_in *servaddr, int *listenfd)
{
    
    *listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(0 > *listenfd)
    {
        return SOCKET_ERR;
    }
    
    memset(servaddr, 0, sizeof(*servaddr));
    servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr->sin_port = htons(PORT);
    if(0 > bind(*listenfd,(const struct sockaddr*)servaddr,sizeof(*servaddr)))
    {
        perror("socket bind failed");
        return BIND_ERR;
    }
    if (0 > (listen((*listenfd), 10)))
    {
        perror("Listen failed...\n");
        return (Exit_Status)-8;
    }
    return SUCCESS;
}
Exit_Status UDPSetAndBind(struct sockaddr_in *servaddr, int *udp_fd)
{
    *udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > *udp_fd)
    {
        perror("socket creation failed");
        return SOCKET_ERR;
    }
    memset(servaddr, 0, sizeof(*servaddr));
    servaddr->sin_family = AF_INET;   /* IPv4*/
    servaddr->sin_addr.s_addr = INADDR_ANY;
    servaddr->sin_port = htons(PORT); /* function converts the unsigned short integer netshort from network byte order to host byte order.*/
    if (0 > bind(*udp_fd, (const struct sockaddr *)servaddr, sizeof(*servaddr)))
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    return SUCCESS;
}
Exit_Status TCPHandler(int listenfd, fd_set *master, int *fdmax)
{
    struct sockaddr_in cli;
    socklen_t addrlen;
    int connfd = 0;
    char remoteIP[INET6_ADDRSTRLEN];

    addrlen = sizeof(cli);
    connfd = accept(listenfd, (struct sockaddr *)&cli, &addrlen);
    if (connfd < 0)
    {
        printf("server acccept failed...\n");
        return ACCEPT_ERR;
    }
    printf("selectserver: new connection from %s on "
           "socket %d\n",
           inet_ntop(cli.sin_family, &cli.sin_addr, remoteIP, INET6_ADDRSTRLEN), connfd);

    *fdmax = MAX(*fdmax,connfd);
    FD_SET(connfd, master);
    return SUCCESS;
}
Exit_Status UDPHandler(int udp_fd)
{
    char buffer[MAXLINE];
    struct sockaddr_in  cliaddr;
    socklen_t addrlen;
    int n = 0;
    char *pong = "pong";

    addrlen = sizeof(cliaddr); /*len is value/resuslt*/
    n = recvfrom(udp_fd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &addrlen);
    if(0 > n)
    {
        return RECV_ERR;
    }
    buffer[n] = '\0';

    printf("the Client sent: %s\n", buffer);
    sendto(udp_fd, (const char *)pong, strlen(pong), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, addrlen);
    printf("in response i'm sending Pong\n");
    return SUCCESS;
}
Stdin_Result StdInHandler()
{
    char buffer[MAXLINE] = {'\0'};
    int amount_read = read(STDIN_FILENO, buffer, sizeof(buffer));

    if (0 == strncmp(buffer, "quit", amount_read - 1))
    {
        printf("killing server,time to say good bye\n");
        return QUIT;
    }
    if (0 == strncmp(buffer, "ping", amount_read - 1))
    {
        printf("pong\n");
        return PING;
    }
    memset(buffer, 0, MAXLINE);

    return (Stdin_Result)SUCCESS;
}
Exit_Status RespondHandler(int sockfd, fd_set *master, int *fdmax)
{
    char buff[MAXLINE];
    char *pong = "pong";
    int nbytes = 0;

    nbytes = read(sockfd, buff, sizeof buff);
 
    if (0 == nbytes)
    {
        /* connection closed*/
        printf("selectserver: socket %d hung up\n", sockfd);
        close(sockfd);           /* bye! */
        FD_CLR(sockfd, master);  /* remove from master set */
        return SUCCESS;

    }
    else if(0 > nbytes)
    {
        return RECV_ERR;
    }
    printf("received from client: %s\n", buff);
    
    if(0 > write(sockfd, pong, sizeof pong))
    {
        return SEND_ERR;
    }
    return SUCCESS;
    (void)fdmax;

}

    

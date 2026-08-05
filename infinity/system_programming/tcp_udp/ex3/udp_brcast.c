#include <stdio.h>  /* printf() */
#include <stdlib.h> /* atoi() */
#include <unistd.h> /* sleep() */
#include <string.h> /* strlen() */
#include <sys/types.h>
#include <sys/socket.h> /* socket(), */
#include <arpa/inet.h>
#include <netinet/in.h> /* sockaddr_in struct */
#include <errno.h>      /*  errno, perror() */

#include "../udp_tcp.h"

int main(int argc, char *argv[])
{
    int status = 0;
    int sock_fd = 0;
    struct sockaddr_in broadc_addr;
    int message_num = 0;
    int broad_perm = 1;
    char *msg_str = "ping";

        sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (0 > sock_fd)
    {
        perror("socket creation failed");
        return SOCKET_ERR;
    }
    if (0 > setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, (const void *)&broad_perm, sizeof(broad_perm)))
    {
        perror("socketset failed");
        return SOCKET_ERR;
    }

    memset(&broadc_addr, 0, sizeof(broadc_addr));

    broadc_addr.sin_family = AF_INET;                   /* Internet address family */
    broadc_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);     /* Broadcast IP address */
    broadc_addr.sin_port = htons(PORT);                        /* Broadcast port */

    while (5 > message_num)
    {
        if (0 > sendto(sock_fd, msg_str, strlen(msg_str) + 1, 0, (const struct sockaddr *)&broadc_addr, sizeof(broadc_addr)))
        {
            return SEND_ERR;
        }
        ++message_num;
        sleep(3);
    }

    close(sock_fd);

    (void)argc;
    (void)argv;

    return status;
}
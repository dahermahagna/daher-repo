#include <string.h>
#include <netinet/in.h>
#include "udp_tcp.h"

void FillServerInfo(struct sockaddr_in *servaddr)
{
    memset(servaddr, 0, sizeof(*servaddr));
    servaddr->sin_family = AF_INET;   /* IPv4*/
    servaddr->sin_port = htons(PORT); /* function converts the unsigned short integer netshort from network byte order to host byte order.*/
    servaddr->sin_addr.s_addr = INADDR_ANY;
}
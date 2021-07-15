#define _GNU_SOURCE

#include <stdio.h>     /*printf*/
#include <stdlib.h>    /*atoi*/
#include <unistd.h>    /*getpid, getpid, pid_t ,sleep*/
#include <sys/wait.h>  /*wait*/
#include <signal.h>    /*signals*/
#include <string.h>    /*memset*/
#include <sys/types.h> /* */

#define PING (SIGUSR1)
#define PONG (SIGUSR2)

void PingHandler(int sig)
{
    puts("ping");
    sleep(1);
}

int main(int argc, char *argv[])
{
    struct sigaction s_pong;

    pid_t ppid = 0;

    s_pong.sa_handler = PingHandler;
    s_pong.sa_flags = SA_NODEFER;
    sigaction(PING, &s_pong, NULL);

    ppid = atoi(argv[1]);
    while (1)
    {
        kill(ppid, PONG);
        pause();
    }
    return 0;
}

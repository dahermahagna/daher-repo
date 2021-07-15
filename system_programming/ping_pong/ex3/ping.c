#define _GNU_SOURCE

#include <stdio.h>     /*printf*/
#include <stdlib.h>    /*system*/
#include <unistd.h>    /*getpid, getpid, pid_t ,sleep*/
#include <sys/wait.h>  /*wait*/
#include <signal.h>    /*signals*/
#include <string.h>    /*memset*/
#include <sys/types.h> /* */

#define PING (SIGUSR1)
#define PONG (SIGUSR2)

static pid_t pong_pid = 0;

void PongHandler(int sig, siginfo_t *siginfo, void *context)
{
    pong_pid = siginfo->si_pid;
    puts("pong");
    sleep(1);
}

int main(int argc, char *argv[])
{
    struct sigaction s_ping;

    /* this is to call pong.out from here not manually

     char command_line[200] ={0};
     pid_t pid2 = 0; 
     sprintf(command_line,"%s %d", argv[1],getpid()); 
     pid2 = system(command_line); /*  ./pong.out pid  */

    printf("ping pid -> %d\n", getpid());

    s_ping.sa_flags = SA_SIGINFO; /* flag to use sa_sigaction*/

    s_ping.sa_sigaction = PongHandler;

    sigemptyset(&s_ping.sa_mask);

    sigaction(PONG, &s_ping, NULL);

    while (1)
    {
        pause();
        kill(pong_pid, PING);
    }
    return 0;
}

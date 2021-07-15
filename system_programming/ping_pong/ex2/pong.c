#include <stdio.h>     /*printf*/
#include <unistd.h>    /*getpid, getpid, pid_t ,sleep*/
#include <sys/wait.h>  /*wait*/
#include <signal.h>    /*signals*/
#include <string.h>    /*memset*/
#include <sys/types.h> /* */

#define PING (SIGUSR1)
#define PONG (SIGUSR2)

void PingHandler(int sig)
{
    puts("pong");
    sleep(1);
}

int main()
{
    struct sigaction s_child;


    int i = 0;
    pid_t pid2 = 0;
    memset(&s_child, 0, sizeof(s_child));

    s_child.sa_handler = PingHandler;

    sigaction(PING, &s_child, NULL);

    kill(getppid(),PONG);
    pause();

    return 0;
}

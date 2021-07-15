#include <stdio.h>     /*printf*/
#include <unistd.h>    /*getpid, getpid, pid_t ,sleep*/
#include <sys/wait.h>  /*wait*/
#include <signal.h>    /*signals*/
#include <string.h>    /*memset*/
#include <sys/types.h> /* */

#define PING (SIGUSR1)
#define PONG (SIGUSR2)


void PongHandler(int sig)
{
    puts("ping");
    sleep(1);
}

int main(int argc, char *argv[])
{
    
    struct sigaction s_parent;

    int i = 0;
    pid_t pid2 = 0;
    int status = 124;
    
    memset(&s_parent, 0, sizeof(s_parent));

    
    s_parent.sa_handler = PongHandler;

    sigaction(PONG, &s_parent, NULL);
    
    while (10 > i++)
    {
        pid2 = fork();
        if (pid2 < 0)
        {
            printf("Can't create child process\n");
        }
        else if (pid2 == 0)
        {
            /*child*/
            execv(argv[1], argv + 1);
            
        }
        else
        {
            sleep(1);
            kill(pid2,PING);
            
        }
        wait(&status);
    }
    return 0;
}

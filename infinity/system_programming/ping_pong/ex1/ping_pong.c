#include <stdio.h>    /*printf*/
#include <unistd.h>   /*getpid, getpid, pid_t ,sleep*/
#include <sys/wait.h> /*wait*/
#include <signal.h> /*signals*/
#include <string.h> /*memset*/
#include <sys/types.h> /* */

#define PING (SIGUSR1)
#define PONG (SIGUSR2)




void PingHandler(int sig)
{
    puts("ping");
    sleep(1);
}

void PongHandler(int sig)
{
    puts("pong");
    sleep(1);
}

int main()
{
    struct sigaction s_child;
    struct sigaction s_parent;

    int i = 0;
    pid_t pid2 = 0;
    memset(&s_child,0,sizeof(s_child));
    memset(&s_parent, 0, sizeof(s_child));

    s_child.sa_handler = PingHandler;
    s_parent.sa_handler = PongHandler;

    sigaction(PONG, &s_parent, NULL);
    sigaction(PING,&s_child,NULL);
    pid2 = fork();
    while(10 > i++)
    {
        if (pid2 < 0)
        {
            printf("Can't create child process\n");
        }
        else if (pid2 == 0)
        {
            /*child*/
            kill(getppid(),PONG);
            pause();
            
        }
        else
        {
            /*parent*/
            kill(pid2, PING);
            pause(); 
        }
        
       
    }

    return 0;
}


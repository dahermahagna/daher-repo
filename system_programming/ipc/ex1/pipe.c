#include <stdio.h>      /*printf*/
#include <stdlib.h>     /**/
#include <unistd.h>     /*pipe*/
#include <sys/types.h>  /**/
#include <string.h>     /**/
#include <sys/wait.h>   /**/

enum {READ = 0,WRITE};

int main()
{
    int fd1[2];
    int fd2[2];
    char *ping = "ping";
    char *pong = "pong";
    char receive1[5] = {0};
    char receive2[5] = {0};
    pid_t p;

    if(-1 == pipe(fd1))
    {
        printf("pipe failed");
        return 1;
    }
    if(-1 == pipe(fd2))
    {
        printf("pipe failed");
        /*close() clos the first pipe*/
        return 1;
    }
    p = fork();
    if(0 > p)
    {
        printf("failed to fork");
        return 1;
    }
    else if(0 < p)
    {
        /*paren*/
        close(fd1[READ]);
        write(fd1[WRITE], ping, strlen(ping) + 1);
        close(fd1[WRITE]);
        wait(NULL);
        close(fd2[WRITE]);
        read(fd2[READ],receive1,5);
        printf("parent received: %s\n",receive1);
        close(fd2[READ]);
    }
    else
    {
        /*child*/
        close(fd1[WRITE]);

        read(fd1[READ], receive2, 100);
        printf("child received: %s\n",receive2);
        
        close(fd1[READ]);
        close(fd2[READ]);

        write(fd2[WRITE], pong, strlen(pong) + 1);
        close(fd2[WRITE]);

        exit(0);
    }
    
    
    return 0;
}
#include <stdlib.h>   /*system*/
#include <stdio.h>    /*printf*/
#include <unistd.h>   /*getpid, getpid, pid_t */
#include <sys/wait.h> /*wait*/


int main(int argc, char *argv[])
{
    int child_status = 0;
    pid_t wait_result = 0;

    while (0 == child_status)
    {
        printf("\nThis is the parent process with pid: %d\n", (int)getpid());

        if (1 == argc)
        {
            child_status = system("/home/daher/git/system_programming/smpl_wd/dummy.out");
        }
        else
        {
            child_status = system(argv[1]);
        }
    }

    printf("child creation failed with status %d\n", child_status);

    return 0;
}
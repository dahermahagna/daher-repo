#include <stdlib.h>   /*system*/
#include <stdio.h>    /*printf*/
#include <unistd.h>   /*getpid, getפpid, pid_t */
#include <sys/wait.h> /*wait*/

#define SUCCESS (0)

int main(int argc, char *argv[])
{
    int child_status = SUCCESS;

    while (SUCCESS == child_status)
    {
        printf("\nThis is the parent process with pid: %d\n", (int)getpid());

        if (1 == argc)
        {
            child_status = system("/home/kitayez/git/system_programming/smpl_wd/dummy.out");
        }
        else
        {
            child_status = system(argv[1]); 
        }
    }

    printf("child creation failed with status %d\n", child_status);

    return 0;
}
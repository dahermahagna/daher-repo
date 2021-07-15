#include <stdio.h>    /*printf*/
#include <unistd.h>   /*getpid, getpid, pid_t */
#include <sys/wait.h> /*wait*/
enum 
{
    SUCCESS,
    TRUE_COND,
    FORK_ERR,
    EXECV_ERR
};



int
main(int argc, char *argv[])
{
    pid_t child_pid = 0;
    pid_t wait_result = 0;
    int child_status = -1;
    int execv_status = 0;

    child_pid = fork();
    if (0 > child_pid)
    {
        printf("fork failed");
        return FORK_ERR;
    }

    if (0 == child_pid)
    {
        if (1 == argc)
        {
            execv_status = execv("/home/daher/git/system_programming/smpl_wd/dummy.out", argv);
        }
        else
        {
            execv_status = execv(argv[1], argv);
        }

        if (-1 == execv_status)
        {
            printf("execv failed");
            return EXECV_ERR;
        }
    }
    else
    {
        printf("This is the parent process with pid: %d\n", (int)getpid());
        wait_result = wait(&child_status);
        printf("Child Process with pid %d returned result: %d\n\n", (int)wait_result, child_status);
        execvp(argv[0],argv);
    }


    (void)argc;

    return SUCCESS;
}

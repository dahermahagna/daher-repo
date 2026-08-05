#define _GNU_SOURCE

#include <pthread.h>   /* pthread create       */
#include <unistd.h>    /*getpid, getpid ,sleep */
#include <sys/types.h> /*pid_t                 */
#include <stdlib.h>    /* getenv , setenv      */
#include <stdatomic.h> /* atomic variables     */
#include <signal.h>    /*signals               */
#include <stdio.h>     /*printf                */
#include <sys/wait.h>  /*wait                  */
#include <time.h>

#include "../include/scheduler.h"
#include "../include/uid.h"
#include "../include/ssemaphore.h"

/*------------------------MACRO---------------------------*/

#define SIGNAL_MAX_REPEAT (2)

/*---------------FUNCTION DECLERATION---------------------*/
static void SetUp(void);
static int SendSigusr1Task(void *data);
static int CheckCounterTask(void *data);
void Sigusr1Handler(int sig);
void Sigusr2Handler(int sig);
/*-----------------------Globals--------------------------*/
atomic_int sig_counter = 0;
atomic_int end_flag = 0;
pid_t thread_pid = 0;
int sem_id1 = 0;
int sem_id2 = 0;
Scheduler_t *wd_sch = NULL;


/*-------------------------MAIN---------------------------*/

int main(int argc, char *argv[])
{
   
    (void)argc;

    SetUp();

    thread_pid = getppid();

    SchedulerAdd(wd_sch, SendSigusr1Task, NULL, 1);

    SchedulerAdd(wd_sch, CheckCounterTask, (void *)argv, SIGNAL_MAX_REPEAT);

    IncDecSem(sem_id2, 1, 0);

    IncDecSem(sem_id1, -1, 0);

    SchedulerRun(wd_sch);

    return 0;
}

static void SetUp(void)
{
    struct sigaction still_receaving = {0};
    struct sigaction stop_program = {0};
    char str_pid[12] = {'\0'};

    sprintf(str_pid, "%d", getpid());
    setenv("WD_PID", str_pid, 1);

    still_receaving.sa_handler = Sigusr1Handler;
    sigaction(SIGUSR1, &still_receaving, NULL);

    stop_program.sa_handler = Sigusr2Handler;
    sigaction(SIGUSR2, &stop_program, NULL);

    sem_id1 = SemIdCreate(KeyCreate(1), 0);
    sem_id2 = SemIdCreate(KeyCreate(2), 0);

    wd_sch = SchedulerCreate();
}
/*----------     TASKS FUNCTIONS DEFINITION     ----------*/

static int SendSigusr1Task(void *data)
{
    (void)data;
    kill(thread_pid, SIGUSR1);
    ++sig_counter;
    return end_flag;
}

static int CheckCounterTask(void *data)
{
    pid_t wd_pid = 0;
    char **argv = (char **)data;
    if (1 == end_flag)
    {
        SchedulerDestroy(wd_sch);
        wd_sch = NULL;
    }

    if (SIGNAL_MAX_REPEAT < sig_counter)
    {
        wd_pid = fork();
        if (wd_pid < 0)
        {
            printf("Can't create WD_app process\n");
        }
        else if (0 == wd_pid)
        {

            execv(argv[0], argv);
        }
        else
        {
            IncDecSem(sem_id2, 1, 0);
            thread_pid = wd_pid;
            sig_counter = 0;
        }
    }
    return end_flag;
}
/*----------     SIGNAL HANDLERS DEFINITION     ----------*/

void Sigusr1Handler(int sig)
{
    (void)sig;
    sig_counter = 0;
}
void Sigusr2Handler(int sig)
{
    (void)sig;
    end_flag = 1;
    kill(thread_pid, SIGUSR2);
    SchedulerPause(wd_sch);
}

#define _GNU_SOURCE

#include <pthread.h>        /* pthread create       */
#include <unistd.h>         /*getpid, getpid ,sleep */
#include <sys/types.h>      /*pid_t                 */
#include <stdlib.h>         /* getenv , setenv      */
#include <stdatomic.h>      /* atomic variables     */
#include <signal.h>         /*signals               */
#include <stdio.h>          /*printf                */
#include <sys/wait.h>       /*wait                  */
#include <string.h>         /*memset                */

#include "watch_dog.h"
#include "include/scheduler.h"
#include "include/uid.h"
#include "include/ssemaphore.h"

/*------------------------MACRO---------------------------*/

#define SIGNAL_MAX_REPEAT (2)

/*---------------FUNCTION DECLERATION---------------------*/

static void SetUp(void);
static void CreateWDProcess(void *argv);
static void *WatchDogThread(void *argv);
static void CleanUp(void);

static void Sigusr2Handler(int sig);
static void Sigusr1Handler(int sig);

static int SendSigusr1Task(void *data);
static int CheckCounterTask(void *data);


/*-----------------------Globals--------------------------*/

atomic_int sig_counter = 0;
atomic_int end_flag = 0;
Scheduler_t *thread_sch = NULL;
pid_t wd_pid_g = 0;
int sem_id1 = 0;
int sem_id2 = 0;

/*--------------------------------------------------------*/

void StartWatchdog(char *argv[])
{
    pthread_t WD_thread = 0;

    SetUp();
    while(0 != pthread_create(&WD_thread,NULL,&WatchDogThread,(void*)argv));
}
/*--------------------------------------------------------*/

void StopWatchdog()
{
    end_flag = 1;
    kill(wd_pid_g,SIGUSR2);
    
    IncDecSem(sem_id1,-1,0);
    IncDecSem(sem_id2,-1,0);
    CleanUp();
}
/*----------     STATIC FUNCTION DEFINITION     ----------*/

static void *WatchDogThread(void *argv)
{
    pid_t wd_pid = 0;
    char *str_pid = NULL;
 
    str_pid = getenv("WD_PID");
    if (NULL != str_pid)
    {
        wd_pid = atoi(str_pid);
    }
    wd_pid_g = wd_pid;

    if (wd_pid != getppid())
    {
        CreateWDProcess(argv);
    }

    SchedulerAdd(thread_sch, SendSigusr1Task,NULL, 1);
    SchedulerAdd(thread_sch, CheckCounterTask, (void*)argv, SIGNAL_MAX_REPEAT);

    IncDecSem(sem_id1, 1, 0);
    IncDecSem(sem_id2, -1, 0);

    SchedulerRun(thread_sch);
    SchedulerDestroy(thread_sch);

    pthread_detach(pthread_self());
    IncDecSem(sem_id1, 1, 0);

    return NULL;
}

static void SetUp(void)
{
    struct sigaction still_receaving = {0};
    struct sigaction stop_program = {0};

    still_receaving.sa_handler = Sigusr1Handler;
    sigaction(SIGUSR1, &still_receaving, NULL);

    stop_program.sa_handler = Sigusr2Handler;
    sigaction(SIGUSR2, &stop_program, NULL);

    sem_id1 = SemIdCreate(KeyCreate(1), 0);
    sem_id2 = SemIdCreate(KeyCreate(2), 0);

    thread_sch = SchedulerCreate();
}

static void CreateWDProcess(void *argv)
{
    pid_t wd_pid = 0;
    wd_pid = fork();
    if (0 > wd_pid)
    {
        printf("Can't create WD_app process\n");
    }
    else if (0 == wd_pid)
    {
        execv("watch_dog_app.out", argv);
    }
    else
    {
        wd_pid_g = wd_pid;
    }
}
static void CleanUp(void)
{
    semctl(sem_id1, 0, IPC_RMID, 0);
    semctl(sem_id2, 0, IPC_RMID, 0);
    unsetenv("WD_PID");
}
/*----------     TASKS FUNCTIONS DEFINITION     ----------*/

static int SendSigusr1Task(void *data)
{
    (void)data;
    kill(wd_pid_g, SIGUSR1);
    if (1 == end_flag)
    {
        SchedulerPause(thread_sch);
    }
    ++sig_counter;
    return end_flag;
}
static int CheckCounterTask(void *argv)
{
    pid_t wd_pid = 0;
    if (1 == end_flag)
    {
        SchedulerPause(thread_sch);
    }
    if (SIGNAL_MAX_REPEAT <= sig_counter)
    {
        CreateWDProcess(argv);
    }
    return end_flag;
}

/*----------     SIGNAL HANDLERS DEFINITION     ----------*/

static void Sigusr1Handler(int sig)
{
    (void)sig;
    sig_counter = 0;
}
static void Sigusr2Handler(int sig)
{
    (void)sig;
    IncDecSem(sem_id2, 1, 0);
}

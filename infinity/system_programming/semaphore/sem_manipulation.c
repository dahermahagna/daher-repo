#include <stdlib.h> /*atoi*/
#include <stdio.h> /*printf*/
#include <string.h> /* strstr*/
#include <errno.h> /*errno*/

#include <fcntl.h> 
#include <sys/stat.h> 
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#define ERROR (-1)

static key_t KeyCreate(char const *argv[]);
static void PrintInstruction();
static int SemIdCreate(key_t key);
static void IncDecSem(int sem_id, struct sembuf sb, char *command, int minus_flag);

union semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

int main(int argc, char const *argv[])
{
    key_t key = 0;
    int sem_id = 0;
    int sem_val = 0;
    struct sembuf sb = {0};
    char command[100] = {'\0'};

    key = KeyCreate(argv);
    
    sem_id = SemIdCreate(key);

    PrintInstruction();

    while (1)
    {
        fgets(command,30,stdin);
        
        if('V' == *command)
        {
            sem_val = semctl(sem_id, 0, GETVAL);
            printf("semaphore id: %d \nsemaphore value: %d\n",sem_id,sem_val);
        }

        if ('D' == *command)
        {
            IncDecSem(sem_id,sb,command,-1);
            
        }

        if ('I' == *command)
        {
            IncDecSem(sem_id, sb, command, 1);  
        }

        if ('X' == *command)
        {
            return 0;
        }
       
        if ('E' == *command)
        {
            semctl(sem_id, 0, IPC_RMID, 0);
            return 0;
        }
    }

    (void) argc;

    return 0;
}

static key_t KeyCreate(char const *argv[])
{
    key_t key = 0;
    key = ftok(argv[1], atoi(argv[2]));
    if (ERROR == key)
    {
        perror("ftok");
        exit(1);
    }
    return key;
}

static void PrintInstruction()
{
    system("clear");
    puts("D number [undo] - decrements the value of semaphore by number");
    puts("I number [undo] - increments the value of semaphore by number");
    puts("V - view the value");
    puts("*if the optional paramater undo exists, the operation will\n be automatically undone when the process terminates");
    puts("X - to exit");
    puts("E - to remove the semaphore and exit\n\n");
}

static int SemIdCreate(key_t key)
{
    int sem_id = 0;
    union semun a;
    a.val = 1;
    sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (errno == EEXIST)
    {
        sem_id = semget(key, 1, 0);
    }
    else if (0 <= sem_id)
    {
        a.val = 1;
        if (-1 == semctl(sem_id, 0, SETVAL, a))
        {
            perror("semget");
            exit(1);
        }
    }
    return sem_id;
}

static void IncDecSem(int sem_id, struct sembuf sb, char *command, int minus_flag)
{
    sb.sem_op = minus_flag * atoi(command + 2);
    if (NULL != strstr(command, "undo"))
    {

        sb.sem_flg = SEM_UNDO;
        semop(sem_id, &sb, 1);
        sb.sem_flg = 0;
    }
    else
    {
        semop(sem_id, &sb, 1);
    }
}

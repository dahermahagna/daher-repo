#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "include/ssemaphore.h"
#include <errno.h>

int main()
{
    int fd;
    int i = 0;
    char *myfifo = "/home/daher/git/system_programming/ipc/myfifo";


    char *ping = "ping";
    char received[5];
    key_t t = KeyCreate(5);
    int sem = SemIdCreate(t, 0);
    /* Creating the named file(FIFO)*/
    /* mkfifo(<pathname>, <permission>)*/
    if(-1 == mkfifo(myfifo, 0666))
    {
        /* print("%s\n",strerror(errno)); */
        write(STDOUT_FILENO, strerror(errno), strlen(strerror(errno))+1);
    }

    IncDecSem(sem,1,0);
    
    while (10 > i)
    {
        fd = open(myfifo, O_WRONLY);
        if(0 > fd)
        {
            write(STDOUT_FILENO,"failed open file",17);
            exit(1);
        }
        if(0 > write(fd, ping, strlen(ping) + 1))
        {
            write(STDOUT_FILENO, "failed write ping", 18);
            exit(1);
        }
        close(fd);

        /* Open FIFO for Read only*/
        fd = open(myfifo, O_RDONLY);
        if(0 > fd)
        {
            write(STDOUT_FILENO, "failed open file", 17);
            exit(1);
        }
        /* Read from FIFO*/
        if(0 > read(fd, received, 5))
        {
            write(STDOUT_FILENO, "failed read ping", 17);
            exit(1);
        }

        /* Print the read message*/
        write(STDOUT_FILENO, "ping received: ", 16);
        write(STDOUT_FILENO, received, 5);
        write(STDOUT_FILENO,"\n",2);
        close(fd);

        sleep(1);
        ++i;
    }
    unlink(myfifo);
    SemDestroy(sem);
    return 0;
}
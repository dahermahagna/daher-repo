#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "include/ssemaphore.h"

int main()
{
    int fd;
    int i = 0;
    char *myfifo = "/home/daher/git/system_programming/ipc/myfifo";

    char *pong = "pong";
    char received[5];
    key_t t = KeyCreate(5);
    int sem = SemIdCreate(t,0);
    IncDecSem(sem,-1,0);
    while (1)
    {
        
        /* Open FIFO for read only*/
        fd = open(myfifo, O_RDONLY);
        if(0 > fd)
        {
            write(STDOUT_FILENO, "failed open fifo", 17);
            exit(1);
        }
        /* read the ping on FIFO*/
        /* and close it*/
        if(0 > read(fd, received, 5))
        {
            write(STDOUT_FILENO, "failed write pong", 18);
            exit(1);
        }
        write(STDOUT_FILENO, "pong received: ", 16);
        write(STDOUT_FILENO,received,5);
        write(STDOUT_FILENO, "\n", 2);

        close(fd);
        
        

        /* Open FIFO for Read only*/
        fd = open(myfifo, O_WRONLY);
        if (0 > fd)
        {
            write(STDOUT_FILENO, "failed open fifo", 17);
            exit(1);
        }
        if(0 > write(fd, pong, strlen(pong) + 1))
        {
            write(STDOUT_FILENO, "failed write pong", 18);
            exit(1);
        }
        
            

            /* Print the read message*/
            close(fd);
        
        
            sleep(1);
    }
    return 0;
}
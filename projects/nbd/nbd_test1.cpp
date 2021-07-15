#include <stdio.h>
#include <stdlib.h>             /* system() */
#include <sys/types.h>          /* AF_UNIX */
#include <sys/socket.h>         // socketpair
#include <cassert>
#include <sys/ioctl.h>          //ioctl
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>             //strerror
#include <linux/nbd.h>        //NBD_SET
#include <unistd.h>     //sleep()
#include <linux/fs.h>
#include <sys/wait.h>

#include "nbd.hpp"

#define PATH            ("/home/daher/git/projects/user_dir")
#define DEV_PATH        ("/dev/nbd0")

using namespace ilrd;

int main(int argc, char *argv[])
{
    NbdProxy np1(PATH, 4, 1024);
    
    return EXIT_SUCCESS;
}

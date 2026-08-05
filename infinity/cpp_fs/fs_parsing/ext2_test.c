#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
/* #include <linux/ext2_fs.h> */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* #include <sys/mount.h> */

#include "ext2.h"

int main(int argc, char const *argv[])
{
    /*assume that i received /dev/ram0 from the command line*/
    /*check if the path is availbale*/
    Find("/dev/ram0",argv[1]);

    return 0;
}

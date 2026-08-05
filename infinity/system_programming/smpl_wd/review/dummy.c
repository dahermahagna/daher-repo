#include <stdio.h> /*printf*/
#include <unistd.h> /*sleep, getpid*/

int main(int argc, char *argv[])
{
    unsigned int time = 4;
    printf("  The child process pid:%d\n", (int)getpid());

    while (0 < time)
	{
		time = (long)sleep(time);		
	}

    printf("    The child process finished his life period.\n");

    (void)argc;
	(void)argv;

    return 0;
}
/* #include "../utils/test.h" */
#include <unistd.h>
#include "watch_dog.h"
#include <stdio.h>

/*-------------------------MAIN---------------------------*/


int main(int argc, char *argv[])
{
	int i = 0;
	(void)argc;
	StartWatchdog(argv);

	for(i = 0; 60 > i; ++i)
	{
		sleep(1);
		printf("Daher is the king %d\n",i);
	}

	StopWatchdog();
	return 0; 
}

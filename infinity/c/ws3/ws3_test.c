
#include <stdio.h>
#include "ws3.h"



 char **duplicate(char *envp[]);
 void print_envp(char *envp[]);
 int free_env(char *envp[]);
 int array_size(char *envp[]);
 
int main(int argc, char **argv, char **envp)
{
    
	 
  	size_t josephus_win =0;

    (void)argv[0];
    (void)argc;
  	josephus_win = Josephus(5);
  	
  	printf("\nthe winner player -> %ld\n",josephus_win);
  	
	 PrintLowerEnv(envp);

	
  	
   	PrintTypeSizes();

	
  return 0;
}

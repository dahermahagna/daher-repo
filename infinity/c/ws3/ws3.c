
#include <stdio.h>/*printf*/
#include <unistd.h>
#include <stddef.h> /* size_t */
#include <assert.h> /*assert*/
#include <string.h>/*strchr strdup*/
#include <ctype.h>/*tolowe*/
#include <stdlib.h>/*malloc free*/
#include "ws3.h"


 char *strdup(const char *str);

 char **Duplicate(char *envp[]);
 void PrintEnvp(char *envp[]);
 int FreeEnv(char *envp[]);
 int PoinPoinSize(char *envp[]);
 char *DupLowerStr(char * envariable);

int PrintLowerEnv(char *envp[])
{
	
	char **run_envp=NULL;
	int check = 0;
	assert(envp!=NULL);	
	
  	run_envp = Duplicate(envp);
  	PrintEnvp(run_envp);
   	FreeEnv(run_envp);
  	return check;
}


size_t Josephus(size_t length_of_soldiers)
{
	size_t *players;
	size_t i = 0;
	players = (size_t *) malloc ((length_of_soldiers)*sizeof(size_t));
	
	for(i = 0; i <= (length_of_soldiers-2) ; ++i)
	{
		players[i] = i + 1;
	}
	players[i] = 0;
	i=0;

	while(i != players[i])
	{
		players[i] = players[players[i]];
		i = players[i];
	}
	free(players);
	players = '\0';
	return i+1;

}


char **Duplicate(char *envp[])
{
	
	char **dest_env=NULL;
	char **run_envp=NULL;
	char **dest_dup_start=NULL;
	assert(envp != NULL);
	run_envp = envp;

	dest_env = (char **) malloc ((PoinPoinSize(envp) + 1)*sizeof(char*));
	dest_dup_start = dest_env;
	
	while(NULL != *run_envp)
	{
		
		*dest_env = DupLowerStr(*run_envp);
		++dest_env;
		++run_envp;
	}
	*dest_env = NULL;
	return dest_dup_start;

	
}
char *DupLowerStr(char *envariable)
{
	char *dest_dup_var = NULL;
	char *dest_dup_start = NULL;
	char varchar;

	dest_dup_var = (char*)malloc((strlen(envariable)+1)*sizeof(*envariable));/*i have to add the TERMINATOR difine*/

	dest_dup_start = dest_dup_var;

	do
	{
		varchar = (char)(tolower(*envariable));
		*dest_dup_var =  varchar;
		++dest_dup_var;
		++envariable;
	}while('=' != *envariable && '\0' != *envariable);
	*dest_dup_var = '\0';

	return dest_dup_start;
}

int PoinPoinSize(char *envp[])
{
	int count =0;
	while (NULL != *envp)
	{
		++count;
		++envp;
	}
	return count;
}

void PrintEnvp(char **envp)
{

	assert(envp != NULL);
	
	while(NULL != *envp)
	{
		printf("%s\n", *envp);
		++envp;
	}


}

int FreeEnv(char **envp)
{
	char ** start_envp = envp;
	assert(NULL != envp);
	
	while(NULL != *envp)
	{
		free(*envp);
		*envp = NULL;
		++envp;	
	}

	free(start_envp);
	start_envp = NULL;
	return 1;
}



void PrintTypeSizes()
{
	printf("the type is: char,            its size is: %lu byts\n",sizeof(unsigned char));
    printf("the type is: short,           its size is: %lu byts\n",sizeof(short));
    printf("the type is: unsigned short,  its size is: %lu byts\n",sizeof(unsigned short));
    printf("the type is: int,             its size is: %lu byts\n",sizeof(int));
    printf("the type is: unsigned int,    its size is: %lu byts\n",sizeof(unsigned int));
    printf("the type is: long ,           its size is: %lu byts\n",sizeof(long));
    printf("the type is: unsigned long,   its size is: %lu byts\n",sizeof(unsigned long));
    /*printf("the type is: long long,       its size is: %lu byts\n",sizeof(long long));
    printf("the type is: unsi long long,  its size is: %lu byts\n",sizeof(unsigned long long));*/
    printf("the type is: float,           its size is: %lu byts\n",sizeof(float));
    printf("the type is: double,          its size is: %lu byts\n",sizeof(double));
    printf("the type is: long double,     its size is: %lu byts\n",sizeof(long double));
    printf("the type is: size_t,          its size is: %lu byts\n",sizeof(size_t));
}

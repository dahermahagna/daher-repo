#include <stdio.h>/*printf*/
#include <assert.h>/*assert*/
#include <stdlib.h>/*fgets*/
#include <string.h>/*strcmp*/

int strcmp (const char* str1, const char* str2);
void exercise1();
void AppenOnDemand(char *file_name);
int ComprationStartAppend(char *user_command, char *struct_command);
int DefaultChek();	
int RemoveFile(char *text_to_add , char *file_name);
int LineCounter(char *text_to_add , char *file_name);
int AppendToStart(char *text_to_add , char *file_name);
int AppendOneLineToEnd(char *text_to_add , char *file_name);

enum State {CONTINEWORKING = 1, EXITFILE = 0, FILEREMOVED = 0, FAILEDAPPENDING = 0 , TRUECHECK = 10 , FALSECHECK = 15}; 


#define MAX_TEXT_TO_ADD (256)

typedef void (*ptr_print)(int);
struct print_me 
{
	int num;
	ptr_print ptr;
	
};

void PrintNum(int num)
{
	printf("%d\n",num);
}

typedef int (*comperation_opereation)();

struct file_edit
{
	char *command;
	comperation_opereation comperation_pointer;
	comperation_opereation operation_pointer;
};





int main ()
{
	Exercise1();
	AppenOnDemand("ws5_textfile");
	return 0;
}

void Exercise1()
{
	struct print_me arr_print[10];
	int i = 0;
	for (i = 0; i < 10; ++i)
	{
		arr_print[i].num = i;

	}
	for(i = 0; i < 10; ++i)
	{
		arr_print[i].ptr = PrintNum;
		arr_print[i].ptr(arr_print[i].num);
	}
}

void AppenOnDemand(char *file_name)
{

	char text_to_add[MAX_TEXT_TO_ADD];
	int i = 1,j = 0,commant_test;
	struct file_edit special_commands[5];
	char navigator;

	special_commands[0].command = "-remove";
	special_commands[0].comperation_pointer = strcmp;
	special_commands[0].operation_pointer = RemoveFile;

	special_commands[1].command = "-count";
	special_commands[1].comperation_pointer = strcmp;
	special_commands[1].operation_pointer = LineCounter;

	special_commands[2].command = "-exit";
	special_commands[2].comperation_pointer = strcmp;
	special_commands[2].operation_pointer = ExitFile;

	special_commands[3].command = "<";
	special_commands[3].comperation_pointer = ComprationStartAppend;
	special_commands[3].operation_pointer = AppendToStart;

	special_commands[4].command = "default";
	special_commands[4].comperation_pointer = DefaultChek;
	special_commands[4].operation_pointer = AppendOneLineToEnd;

	printf("welcome to appedning function, if you want to read the istruction press \"i\", else press any key then enter\n");
	scanf(" %c", &navigator);
	if('i' == navigator)
	{
		printf("start adding to %s file by the these instrutions -> \ntype ane text to add.\ntype < before the text to append it in the star.\ntype -count to know how many lines there are in your file.\ntype -remove if the file is annoyin to you and you want to delete it.\nto exit just type -exit.\n", file_name);

	}
	while(1 == i)
	{
		fgets(text_to_add, MAX_TEXT_TO_ADD, stdin);

		j = 0;
		while(j<5)
		{
			commant_test = special_commands[j].comperation_pointer(text_to_add,special_commands[j].command);
			if(commant_test == TRUECHECK)
			{	
				i = special_commands[j].operation_pointer(text_to_add,file_name);
				j = 4;

			}
			j++;
			
		}

	}
}

int RemoveFile(char *text_to_add , char *file_name)
{
	
	int ret;
	(void)text_to_add;
	assert(NULL != file_name);
	ret = remove(file_name);
	if(ret == 0) 
	{
      printf("File deleted successfully");
      return FILEREMOVED;
   	} 
   	else 
   	{
      printf("Error: unable to delete the file");
      return (CONTINEWORKING);
   	}

}

int LineCounter(char *text_to_add , char *file_name)
{
	FILE *file_pointer;
	char check;
	unsigned int line_counter = 0;
	(void)text_to_add;
	file_pointer = fopen(file_name, "a+");
	

	for (check = getc(file_pointer); check != EOF; check = getc(file_pointer)) 
	{
        if (check == '\n') 
        {
            line_counter = line_counter + 1;
        }
	}
	fclose(file_pointer);

	printf("the are %d lines in the file\n", line_counter);
	return CONTINEWORKING;
}

int ExitFile()
{
	return EXITFILE;

}

int ComprationStartAppend(char *user_command, char *struct_command)
{	
	(void)struct_command;

	assert(NULL != user_command);
	if('<' == *user_command)
		return TRUECHECK;
	return FALSECHECK;
}

int AppendToStart(char *text_to_add , char *file_name)
{
	FILE* file_pointer;
	char copy_char;
	FILE *new_file_pointer;

	file_pointer = fopen(file_name, "a+");
	if(NULL == file_pointer)
	{
		printf("failed to open file\n");
		return FAILEDAPPENDING;
	}
	new_file_pointer = fopen("/home/daher/git/c/ws5/temporaryfilejustforthisfunction", "w");
	
	if(NULL == new_file_pointer)
	{
		printf("failed to open file\n");
		return FAILEDAPPENDING;
	}
	++text_to_add;
	fputs(text_to_add,new_file_pointer);
	/*fseek to the first place*/
	fseek(file_pointer, 0, SEEK_SET);

	copy_char = fgetc(file_pointer); 

    while (copy_char != EOF) 
    { 
        fputc(copy_char, new_file_pointer); 
        copy_char = fgetc(file_pointer); 
    }

    fclose(file_pointer);
    fclose(new_file_pointer);
    remove(file_name);
    rename("/home/daher/git/c/ws5/temporaryfilejustforthisfunction",  file_name);
    

	return CONTINEWORKING;
	

}


int DefaultChek()
{
	return TRUECHECK;
}

int AppendOneLineToEnd(char *text_to_add , char *file_name)
{
	FILE* file_pointer;
	file_pointer = fopen(file_name, "a+");
		if(NULL == file_pointer)
		{
			printf("failed to open file\n");
			return FAILEDAPPENDING;
		}
	fseek(file_pointer, 0, SEEK_END);
	fputs(text_to_add,file_pointer);
	fclose(file_pointer);

	return CONTINEWORKING;


}
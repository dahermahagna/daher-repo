#include <stdio.h>
#include <stdlib.h>
#include "ws4.h"

void printa();
void printt();
void ExitFunction();
void Nothing();
void TaLut ();

void main1()
{
}
void TaIf ()
{
  int i = 0;
  char command;
  
  printf("press a to type a\npress t to type t\npress esc to return to the main\n");
  system("stty -icanon -echo");
  
	
  while(i == 0)	
  {
    scanf(" %c", &command);
  
    if(command == 27)
      i=1;
  
    if(command == 'a')
     system("echo a");
  
    if(command == 't')
     system("echo t");
  
  }
  
system("stty -icanon echo");

}


void TaSwitch ()
{
	int i = 0;
	char command;

  printf("press a to type a\npress t to type t\npress esc to return to the main\n");
	system("stty -icanon -echo");
	
		while(i == 0)
	{

	 	scanf(" %c", &command);
        switch(command)
        {
            case 27:
            i=1;
            break;
            case 'a':
   			system("echo a");
   			break;
   			case 't':
   			system("echo t");
   			break;
   			case 26:
   			system("echo exit");
        }
    }
	system("stty -icanon echo");

}


void printa()
{
  system("echo a");
}

void printt()
{
  system("echo t");
}
void ExitFunction()
{
  system("stty -icanon echo");
  exit(0);


}
void Nothing()
{

}
void TaLut ()
{

    char command;

    static size_t i = 0;
    static void (*lookup[256])();
    {
        for(; i < 256 ; ++i)
        {
            lookup[i] = &Nothing;
        }

        lookup[97] = &printa;
        lookup[116] = &printt;
        lookup[27] = &ExitFunction;
    }
    i = 0;
    printf("press a to type a\npress t to type t\npress esc to exit\n");
    system("stty -icanon -echo");
    while(i == 0)
    {

       
        scanf(" %c", &command);
        lookup[(int)command]();
    }

}


#include <stdio.h>
#include <stdlib.h>
#include "ws4.h"

int main()
{
	int i = 4;
	char c;

	while(i != 0)
	{
		system("stty -icanon -echo");
		printf("chose f for if finction\nchose s for switch finction\nchose l for lut finctionthen\nchose esc to exit\n");
		scanf(" %c", &c);
		
		switch (c)
		{
			case 'f':
			TaIf();
			break;
		
			case 's':
			TaSwitch();
			break;
		
			case 'l':
			TaLut();
			break;

			case 27:
			system("stty -icanon echo");
			exit(0);

			break;

			default:
			i=1;

			

		}
		if(1 == i)
		{
			printf("wrong chois ");
		}
			system("stty -icanon echo");
			printf("if you want to chose another function press 1. \nif you want to exit press 0 \nthen press enter\n");
			scanf("%d", &i);

		}
		
		
	
	system("stty -icanon echo");
	return 0;
}
#include <stdio.h>/*printf*/
#include <string.h> /*strlen*/

void ConvertHex(char *str);
double Pow_ten(int n);
int FlipInt(int n);
void SwapInt(int *x, int *y);


int main()
{
	int swap1=10,swap2=20;
	char str[]="2268656C6C6F20776F726C642122";
	int flip_check=0;
	/*exercise 3*/
	ConvertHex(str);
	
	/*exercise 4*/
	printf("\n\n%f\n\n",Pow_ten(-3));
	
	/*exercise 5*/
	printf("%d\n\n",FlipInt(234));
	flip_check=(321==FlipInt(123)&&-321==FlipInt(-123)&&321==FlipInt(1230));
	printf("%d\n", flip_check);
	/*exercise 6*/
	SwapInt(&swap1,&swap2);
	
	return 0;
}

void ConvertHex(char *str)
{
	/*printf("\n\nexercise 3 hello world by hex\n");*/
	int rightchar = 0;
	size_t i = 0;
	
	for(i = 0; i <= strlen(str)-2; i+=2)
	{
		rightchar = str[i+1]-48;
		
		if ((str[i+1]) >= 65)
			rightchar-=7;
			
		printf("%c",(str[i]-48)*16+rightchar);
	}	
}

double Pow_ten(int num)
{
	/*printf("\n\nexercise 4 power of 10\n");*/
	double base=10,result=1;
	int i = 0;
	
	if(num<0)
	{
		num*=-1;
		base=0.1;
	}
	
	for(i = 0; i < num; i++)
		result=result*base;
	
	return result;
}

int FlipInt(int num)
{
	/*printf("\n\nexercise 5 flip the number\n");*/
	int result=0,is_pos=1;
	/*while(n%10==0)
		n/=10;
	
	if(num<0)
	{
		num*=-1;
		is_pos=-1;
	}*/
	
	if(num<10)
		return num*is_pos;
		
	while(num!=0)
	{
		result=result*10+num%10;
		num/=10;
	}
	
	return result*is_pos;
}

void SwapInt(int *firstnum, int *secundnum)
{
	int tmp=*firstnum;
	*firstnum=*secundnum;
	*secundnum=tmp;
}




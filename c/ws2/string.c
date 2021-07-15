#include <stdio.h>
#include "string.h"
#include <string.h>
#include <ctype.h> /*toupper*/
#include <stdlib.h>/*malloc*/
#include <assert.h>/*assert*/

int Case_Char(const char c1,const char c2);
size_t Strlen(const char *s);
int Strcmp(const char *str1, const char *str2);
int DigitSeven(int num);
char *within_Space_remove(char *str);



void check_h_file()
{
	printf("h file and c file is working");
}


char *Strcpy(char *dest, const char *src)
{
	size_t i=0;
	
	for(i=0;src[i]!='\0';++i)
	{
		dest[i]=src[i];
	}
	dest[i] ='\0';
	return dest;
}
char *Strncpy(char *dest, const char *src, size_t n)
{
	size_t i=0;
	
	for(i=0;i<n && src[i]!='\0';++i)
	{	
		dest[i]=src[i];
	}
		

		dest[i]='\0';

	return dest;
}

int Strcasecmp(const char *str1, const char *str2)

{
	int result=0;
	while('\0' != *str1 && Case_Char(*str1, *str2)==0)
	{
		str1++;
		str2++;
	}
	
	if(*str1 != '\0')
		result = (int) (tolower(*str1) - tolower(*str2));
	return result;
}


char *Strchr(const char *str, int c)
{
	if(c=='\0')
		return (char *)str;
	while('\0' != *str)
	{
		if(*str==c)
			return (char *)str;
		++str;
	}
	return NULL;
}

char *Strdup(const char *str)
{
	char *ptr_new_arr = NULL;
	int i = 0;

	ptr_new_arr=malloc(Strlen(str)*sizeof(*str)+1);
	
	for(i = 0; str[i] != '\0'; i++)
		ptr_new_arr[i]=str[i];

	return ptr_new_arr;
}

char *Strcat(char *dest, const char *src)
{
	/*int i = 0, j = 0;*/
	char * tmp_c1=dest;
	
	while('\0' != *dest)
	{
		++dest;
	}
	while(*src != '\0')
	{
		*dest = *src;
		++dest;
		++src;
	}
	*(++dest) = '\0';
	return tmp_c1;

}

char *Strncat(char *dest, const char *src, size_t n)
{
	size_t i = 0, j = 0;
	for(i = 0; '\0' != dest[i]; ++i)
	{}
	while('\0' != src[j] && j<n)
	{
		dest[i]=src[j];
		i++;
		j++;
	}
	dest[i+1]='\0';
	return dest;

}

char *Strstr(const char *haystack, const char *needle)
{
	const char *tmp_c1, *tmp_c2;
	while('\0' != *haystack)
	{
		tmp_c1 = needle;
		tmp_c2 =haystack;
		while('\0' != *tmp_c1 && '\0' != *tmp_c2  && *tmp_c1 == *tmp_c2)
		{
				++tmp_c1;
				++tmp_c2;

		}
		if(*tmp_c1 == '\0')
			return (char *) haystack;
		++haystack;
	}
	return NULL;
}
size_t Strspn(const char *str, const char *accept)
{
	size_t count = 0;
	const char *ptr_accept;

	while('\0' != *str)
	{
		ptr_accept=accept;
		while('\0' != *ptr_accept)
			{
				if(*ptr_accept == *str)
				{
					++count;
					break;
				}
				else
				{
					++ptr_accept;
				}
			}
			if(*ptr_accept == '\0')
			{
				return count;
			}
			++str;
	}
	return 0;
}

int IsPalindrom(const char *str)
{
	const char *right = &str[Strlen(str)-1];
	
	while(right>str && *right==*str)
	{
		++str;
		--right;		
	}

	if(right<=str)
	{
		return 1;
	}
	return 0;

}

void SevenBoom(int num1, int num2)
{
	assert(num2 > num1);
	while(num1 <= num2)
	{

		if(0 == num1%7 ||1 == DigitSeven(num1))
		{
			printf("BOOM,");
		}
		else
		{
			printf("%d,",num1);
		}
		++num1;
	}

	printf("\n");
}

char * RemoveExtraWS(char *str)
{
	char *str_end = str + Strlen(str)-1;
	while(str < str_end &&  (*str == ' ' || *str == '\t'))
	{
		++str;
		
	}

	while(*str_end == ' '||*str_end == '\t')
	{
		--str_end;		
	}
	
	

	if(str_end<=str)
		return NULL;

	++str_end;
	*str_end = '\0';
	
	within_Space_remove(str);
	
	return str;
}

int Case_Char(const char c1,const char c2)
{
	if(c1==c2)
		return 0;
	else
	{
		if(c1>=65&&c1<=90)
			if(c1==c2-32)
				return 0;
		if(c1>=97&&c1<=122)
			if(c1==c2+32)
				return 0;
		return 1;	
	}
}
size_t Strlen(const char *s)
{
	size_t i = 0;
	for(;'\0' != *s; ++s)
	{
		++i;
		
	}
	return i;
}
int Strcmp(const char *str1, const char *str2)
{
	while('\0' != *str1 && *str1 == *str2)
	{
		++str1;
		++str2;
	}
	return *str1-*str2;
}
int DigitSeven(int num)
{
	while(0 != num)
	{
		if(7 == num%10)
			return 1;
		num/=10;
	}
	return 0;
}
char *within_Space_remove(char *str)
{
	
	int count = 0;
	char *tmp_c, *tmp_c2=str;
	while('\0' != *str)
	{
		if(*str == ' ' || *str == '\t')
		{
			count = 0;
			tmp_c = str+1;

			while(*tmp_c == ' ')
			{
				++count;
				++tmp_c;
			}
			tmp_c=str+1;
			++str;
			while(*(tmp_c+count) != '\0')
			{
				*tmp_c=*(tmp_c+count);
				++tmp_c;
			}

			while(*(tmp_c) != '\0')
			{
				*tmp_c='\0';
				++tmp_c;
			}

			
		}
		++str;
	}
	return tmp_c2;
}

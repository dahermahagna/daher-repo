#include <stdio.h> /*printf*/
#include <string.h>/* memcpy, memset, memmove */
#include <assert.h>/*assert*/

#define SIZE_OF_WORD (sizeof(size_t))
#define DEST_AND_N *(char *)((size_t )dest+n-1)
#define SRC_AND_N *(char *)((size_t)src+n-1)
#define DEST_AND_N_MINUS_8 *(size_t *)((size_t )dest+n-8)
#define SRC_AND_N_MINUS_8 *(size_t *)((size_t )src+n-8)
#define BYTE_SIZE (8)

void *Memset(void *s, int c, size_t n);
void *Memcpy(void *dest, const void *src, size_t n);
void *Memmove(void *dest, const void *src, size_t n);

void TestMemset();
void TestMemcpy();
void TestMemmove();



int main()
{

	TestMemset();
	TestMemcpy();
	TestMemmove();

    return 0;
}
#define SIZE_OF_WORD (sizeof(size_t))
#define DEST_AND_N *(char *)((size_t )dest+n-1)
#define SRC_AND_N *(char *)((size_t)src+n-1)
#define DEST_AND_N_MINUS_8 *(size_t *)((size_t )dest+n-8)
#define SRC_AND_N_MINUS_8 *(size_t *)((size_t )src+n-8)
#define BYTE_SIZE (8)
void *Memset(void *s, int c, size_t n)
{
	size_t i = 0;
	size_t word_c = 0;
	size_t *run_over = (size_t *)s;

	while(0 != (size_t)run_over % SIZE_OF_WORD && 0 < n)
	{		
		*(char *)run_over = (c & 0xFF);
		++*(char *)&run_over;
		--n;
	}


	for(i = 0; i < SIZE_OF_WORD && 0 != n; ++i)
	{
		word_c <<= BYTE_SIZE;
		word_c = (word_c | (c & 0xFF));

	}

	while(8 <= n)
	{
		*(size_t *)run_over = word_c;
		++run_over;
		n -= SIZE_OF_WORD;
	}

	while(0 < n) 
	{		
		*(char *)run_over = (char)c;
		++*(char *)&run_over;
		--n;
	}

	return s;
}

void TestMemset()
{
	char str1[50], str2[50];

   strcpy(str1,"This is string.h library function");
   
   memset(str1,'$',13);

   strcpy(str2,"This is string.h library function");


   Memset(str2,'$',13);

  
   if (0 == memcmp(str1,str2,36))
   	{
        printf("Memset Success\n");
    }

    else
    {
        printf("Memset Fail\n");
    }
}


void *Memcpy(void *dest, const void *src, size_t n)
{
    size_t *run_over_dest =(size_t *)dest;
    size_t *run_over_src =(size_t *)src; 

    while(0 != ((size_t)run_over_dest % SIZE_OF_WORD))
    {
        *(char *)run_over_dest = *(char *)run_over_src; /*i should make a function that copies one byte to use it before and after the word copy*/
        ++*(char *)&run_over_dest;
        ++*(char *)&run_over_src;
        n -= 1;
    }
    while (SIZE_OF_WORD <= n)
    {
        *run_over_dest = *run_over_src;
        ++run_over_dest;
        ++run_over_src;
        n -= SIZE_OF_WORD;
    }

    while(0 < n)
    {
       
        *(char *)run_over_dest = *(char *)run_over_src;
        ++*(char *)&run_over_dest;
        ++*(char *)&run_over_src;
        n-=1;
    }

    return dest;
}

void TestMemcpy()
{

	int str1[20] = {1,2,3,4,5,6,7,8,9,0,6,7,5,6,3};
    char str2[20] = "Daher";

    int str3[20]  = {1,2,3,4,5,6,7,8,9,0,6,7,5,6,3};
    char str4[20] = "Daher"; 

    if (0 == memcmp(memcpy(str1, str2, 5), Memcpy(str3, str4, 5),9))
    {
        printf("Memcpy Success\n");
    }

    else
    {
        printf("Memcpy Fail\n");
    }
}

void *Memmove(void *dest, const void *src, size_t n)
{
	if(dest < src || *(size_t *)&dest > (*(size_t *)&src+n))
	{
		memcpy(dest,src,n);
	}
	
	else
	{
		while(0 != ((size_t )dest + n) % 8)
		{
			DEST_AND_N = SRC_AND_N;
			--n;
		}
		while(8 <= n)
		{
			DEST_AND_N_MINUS_8 = SRC_AND_N_MINUS_8;
			n -= 8;
		}
		while(0 < n)
		{
			DEST_AND_N = SRC_AND_N;
			--n;
		}
	}
	return dest;


}

void TestMemmove()
{
    char  src1[52], src2[52], src3[80], src4[80];
    strcpy(src1, "dont worry be happy life shining all above you 2020");
    strcpy(src2, "dont worry be happy life shining all above you 2020");
    strcpy(src3, "how are you today all good?");
    strcpy(src4, "how are you today all good?");

    if (0 == memcmp(Memmove(src1 + 4, src1, 6), memmove(src2 + 4, src2, 6),10) &&
    	0 == memcmp(Memmove(src1, src1 + 5, 7), memmove(src2, src2 + 5, 7),10) &&
        0 == memcmp(Memmove(src4, src4, 8), memmove(src3, src3, 8),10))
    {
        printf("Memmove Success\n");
    }
    else
    {
        printf("Memmove Fail\n");
    }
} 
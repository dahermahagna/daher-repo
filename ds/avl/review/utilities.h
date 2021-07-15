#ifndef NDEBUG
    #include <stdio.h>            /* printf()   */
    #include <string.h>           /* strerror() */
	#include <errno.h>            /* errno,     */
    #define ERR_PRINT() fprintf(stderr, "errno: %s\n", strerror(errno));\
                fprintf(stderr, "error in %s file, in line: %d\n", __FILE__, __LINE__)
    #define ERR_MALLOC() fprintf(stderr,"failed to allocate memory %s file, in line: %d\n", __FILE__, __LINE__)
#else
    #define ERR_PRINT()
    #define ERR_MALLOC() 
#endif


#define TEST(condition, func_name) ((1) == (condition))?\
                                    printf("%-30s:\t\x1b[32m SUCCESS \x1b[0m | Line: %d\n", #func_name, __LINE__) : \
                                    printf("%-30s:\t\x1b[31m FAILURE \x1b[0m | Line: %d\n", #func_name, __LINE__)

#define DEADBEEF ((void *) 0xdeadbeef)

#define WORD (sizeof(void *))

#define INT_BITS (32)

#define ZERO (0)
#define ONE (1)

#define ABS(num) num < 0 ? -num : num

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MASK_SINGLE (0x5555555555555555u)
#define MASK_PAIR (0x3333333333333333u)
#define MASK_NIBBLE (0x0f0f0f0f0f0f0f0fu)
#define MASK_TWO_NIBBLES (0x00ff00ff00ff00ffu)
#define MASK_FOUR_NIBBLES (0x0000ffff0000ffffu)
#define MASK_EIGHT_NIBBLES (0x00000000ffffffffu)

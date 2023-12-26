#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h> /*printf*/
#include <stddef.h> /* size_t */

typedef enum {PASSED, FAILED} test_status_t;

#define YELLOW "\033[0;33m"
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

#define FUNC_TO_TEST(func)	do { if (0 == func_call) 	{ printf (" "#func "\n"); func_call = 1; }  else { printf ("                  "#func "\n"); } } while(0)

#define REQUIRE(condition)	do { if (0 == (condition))	{ printf(RED "\n   Failed to check: " RESET "("#condition")\n"); printf(MAGENTA "   File: %s\n   Line: %d" RESET "\n________________________________\n" , __FILE__, __LINE__); return FAILED; } } while(0)

#define RUNTEST(test)		do{	printf ("\n"BLUE UNDERLINE #test CLOSEUNDERLINE RESET "\nTested functions:" );	if (PASSED == test()) {	if (0 == func_call)		{ printf(GREEN " SUCCESS" RESET"\n________________________________\n"); }	else { printf("\n                  -------\n" GREEN "                  SUCCESS" RESET "\n________________________________\n");		}	} func_call = 0; } while(0)

size_t func_call = 0;

#endif /*__TEST_H__*/

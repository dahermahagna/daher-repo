#include <stdlib.h> /*malloc , free*/
#include "../utils/test.h"
#include "fsa.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseTthreeTest(void);

/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(CaseOneTest);
	RUNTEST(CaseTwoTest);
	RUNTEST(CaseTthreeTest);

	return PASSED; 
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate case test */
/*
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{

	FUNC_TO_TEST(FSASuggestSize);

	REQUIRE(104 == FSASuggestSize(5,10));
	REQUIRE(104 == FSASuggestSize(8,10));
	REQUIRE(336 == FSASuggestSize(100,3));
	REQUIRE(184 == FSASuggestSize(14,10));



	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	FSA_t *memory = NULL;
	size_t *first_block = NULL;
	size_t *second_block = NULL;
	size_t *third_block = NULL;
	size_t *fourth_block = NULL;
	size_t *fifth_block = NULL;
	size_t *sixth_block = NULL;
	size_t *seventh_block = NULL;
	size_t *eighth_block = NULL;
	size_t *ninth_block = NULL;
	size_t *tenth_block = NULL;
	size_t *eleventh_block = NULL;


	FUNC_TO_TEST(FSAInit);
	FUNC_TO_TEST(FSAAlloc);
	FUNC_TO_TEST(FSAFree);

	memory = malloc(FSASuggestSize(5,10));
	FSAInit(memory, FSASuggestSize(5,10),5);

	first_block = FSAAlloc(memory);
	*first_block = 1122334455667788;
	REQUIRE(1122334455667788 == *first_block);

	second_block = FSAAlloc(memory);
	*second_block = 1122334455667788;
	REQUIRE(1122334455667788 == *second_block);

	third_block = FSAAlloc(memory);
	*third_block = 1122334455667788;
	REQUIRE(1122334455667788 == *third_block);

	fourth_block = FSAAlloc(memory);
	*fourth_block = 1122334455667788;
	REQUIRE(1122334455667788 == *fourth_block);

	fifth_block = FSAAlloc(memory);
	*fifth_block = 1122334455667788;
	REQUIRE(1122334455667788 == *fifth_block);

	sixth_block = FSAAlloc(memory);
	*sixth_block = 1122334455667788;
	REQUIRE(1122334455667788 == *sixth_block);

	seventh_block = FSAAlloc(memory);
	*seventh_block = 1122334455667788;
	REQUIRE(1122334455667788 == *seventh_block);

	eighth_block = FSAAlloc(memory);
	*eighth_block = 1122334455667788;
	REQUIRE(1122334455667788 == *eighth_block);

	ninth_block = FSAAlloc(memory);
	*ninth_block = 1122334455667788;
	REQUIRE(1122334455667788 == *ninth_block);

	tenth_block = FSAAlloc(memory);
	*tenth_block = 1122334455667788;
	REQUIRE(1122334455667788 == *tenth_block);

	eleventh_block = FSAAlloc(memory);
	REQUIRE(NULL == eleventh_block);

	FSAFree(memory, tenth_block);


	eleventh_block = FSAAlloc(memory);
	REQUIRE(NULL != eleventh_block);



	free(memory);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTthreeTest(void)
{
	
	FSA_t *memory = NULL;
	size_t *first_block = NULL;
	size_t *second_block = NULL;
	size_t *third_block = NULL;
	size_t *fourth_block = NULL;
	size_t *fifth_block = NULL;
	size_t *sixth_block = NULL;
	size_t *seventh_block = NULL;
	size_t *eighth_block = NULL;
	size_t *ninth_block = NULL;
	size_t *tenth_block = NULL;
	size_t *eleventh_block = NULL;

	FUNC_TO_TEST(FSACountFree);

	memory = malloc(FSASuggestSize(5,10));
	FSAInit(memory, FSASuggestSize(5,10),5);

	first_block = FSAAlloc(memory);
	*first_block = 1122334455667788;
	REQUIRE(9 == FSACountFree(memory));

	second_block = FSAAlloc(memory);
	*second_block = 1122334455667788;
	REQUIRE(8 == FSACountFree(memory));

	third_block = FSAAlloc(memory);
	*third_block = 1122334455667788;
	REQUIRE(7 == FSACountFree(memory));

	fourth_block = FSAAlloc(memory);
	*fourth_block = 1122334455667788;
	REQUIRE(6 == FSACountFree(memory));

	fifth_block = FSAAlloc(memory);
	*fifth_block = 1122334455667788;
	REQUIRE(5 == FSACountFree(memory));

	sixth_block = FSAAlloc(memory);
	*sixth_block = 1122334455667788;
	REQUIRE(4 == FSACountFree(memory));

	seventh_block = FSAAlloc(memory);
	*seventh_block = 1122334455667788;
	REQUIRE(3 == FSACountFree(memory));

	eighth_block = FSAAlloc(memory);
	*eighth_block = 1122334455667788;
	REQUIRE(2 == FSACountFree(memory));

	ninth_block = FSAAlloc(memory);
	*ninth_block = 1122334455667788;
	REQUIRE(1 == FSACountFree(memory));

	tenth_block = FSAAlloc(memory);
	*tenth_block = 1122334455667788;
	REQUIRE(0 == FSACountFree(memory));

	
	FSAFree(memory, tenth_block);
	REQUIRE(1 == FSACountFree(memory));

	FSAFree(memory, first_block);
	REQUIRE(2 == FSACountFree(memory));

	FSAFree(memory, third_block);
	REQUIRE(3 == FSACountFree(memory));

	FSAFree(memory, seventh_block);
	REQUIRE(4 == FSACountFree(memory));

	seventh_block = FSAAlloc(memory);
	*seventh_block = 112233445566778899;

	REQUIRE(3 == FSACountFree(memory));
	

	eleventh_block = FSAAlloc(memory);
	REQUIRE(NULL != eleventh_block);



	free(memory);
	return PASSED;
}

#include <stdlib.h> /*malloc*/
#include "../utils/test.h"
#include "vsa.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseThreeTest(void);
static test_status_t CaseFourTest(void);

/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(CaseOneTest);
	RUNTEST(CaseTwoTest);
	RUNTEST(CaseThreeTest);
	RUNTEST(CaseFourTest);

	return PASSED; 
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate stage test */
/*
FUNC_TO_TEST(VSAAlloc);
FUNC_TO_TEST(VSAFree);
FUNC_TO_TEST(VSALargestChunkAvailable);
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{
	
	VSA_t *vsa = NULL;
	void *memory = NULL;
	size_t memory_size = 200;

	void *first_block = NULL;
	void *second_block = NULL;
	void *third_block = NULL;
	void *fourth_block = NULL;
	void *fifth_block = NULL;

	memory = calloc(1, sizeof(char) * memory_size);

	vsa = VSAInit(memory, memory_size); 
	REQUIRE(NULL != vsa);
	

	first_block = VSAAlloc(vsa, 19);
	REQUIRE(NULL != first_block);
	*(int *)first_block = 1;

	second_block = VSAAlloc(vsa, 20);
	REQUIRE(NULL != second_block);
	*(int *)second_block = 2;

	third_block = VSAAlloc(vsa, 20);
	REQUIRE(NULL != third_block);
	*(int *)third_block = 3;
	fourth_block = VSAAlloc(vsa, 40);
	REQUIRE(NULL != fourth_block);
	*(int *)fourth_block = 4;
	fifth_block = VSAAlloc(vsa, 20);
	REQUIRE(NULL == fifth_block);
	REQUIRE(36 > VSALargestChunkAvailable(vsa));

	REQUIRE(1 == *(int *)first_block);
	REQUIRE(2 == *(int *)second_block);
	REQUIRE(3 == *(int *)third_block);
	REQUIRE(4 == *(int *)fourth_block);

	free(memory);

	return PASSED; 
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	VSA_t *vsa = NULL;
	void *memory = NULL;
	size_t memory_size = 200;

	void *first_block = NULL;
	void *second_block = NULL;
	void *third_block = NULL;
	void *fourth_block = NULL;
	void *fifth_block = NULL;

	memory = calloc(1, sizeof(char) * memory_size);

	vsa = VSAInit(memory, memory_size); 
	REQUIRE(NULL != vsa);
	REQUIRE(168 == VSALargestChunkAvailable(vsa));
	first_block = VSAAlloc(vsa, 20);
	REQUIRE(NULL != first_block);
	*(int *)first_block = 1;

	REQUIRE(132 == VSALargestChunkAvailable(vsa));
	VSAFree(first_block);
	REQUIRE(168 == VSALargestChunkAvailable(vsa));
	first_block = VSAAlloc(vsa, 20);
	REQUIRE(NULL != first_block);
	*(int *)first_block = 1;

	second_block = VSAAlloc(vsa, 20);
	REQUIRE(NULL != second_block);
	*(int *)second_block = 2;
	third_block = VSAAlloc(vsa, 20);
	REQUIRE(NULL != third_block);
	*(int *)third_block = 3;
	fourth_block = VSAAlloc(vsa, 40);
	REQUIRE(NULL != fourth_block);
	*(int *)fourth_block = 4;
	REQUIRE(4 == VSALargestChunkAvailable(vsa));
	fifth_block = VSAAlloc(vsa, 4);
	REQUIRE(NULL != fifth_block);

	VSAFree(second_block);
	VSAFree(third_block);
	
	second_block = VSAAlloc(vsa, 40);
	REQUIRE(NULL != second_block);
	*(int *)second_block = 2;
	third_block = VSAAlloc(vsa, 1);
	REQUIRE(NULL == third_block);

	REQUIRE(1 == *(int *)first_block);
	REQUIRE(2 == *(int *)second_block);
	REQUIRE(4 == *(int *)fourth_block);

	free(memory);

	return PASSED; 
}

/*--------------------------------------------------------*/

static test_status_t CaseThreeTest(void)
{
	VSA_t *vsa = NULL;
	void *memory = NULL;
	size_t memory_size = 10;

	memory = calloc(1, sizeof(char) * memory_size);

	vsa = VSAInit(memory, memory_size); 
	REQUIRE(NULL == vsa);

	free(memory);

	return PASSED; 
}

/*--------------------------------------------------------*/

static test_status_t CaseFourTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

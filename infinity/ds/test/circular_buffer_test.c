#include <string.h> /*strcmp*/
#include "../utils/test.h"
#include "circular_buffer.h"



static test_status_t CBufferCreateTest();
static test_status_t CBufferDestroyTest();
static test_status_t CBufferSizeTest();
static test_status_t CBufferFreeSpaceTest();
static test_status_t CBufferIsEmptyTest();
static test_status_t CBufferReadTest();
static test_status_t CBufferWriteTest();



int main()
{
	RUNTEST(CBufferCreateTest);
	RUNTEST(CBufferDestroyTest);
	RUNTEST(CBufferSizeTest);
	RUNTEST(CBufferFreeSpaceTest);
	RUNTEST(CBufferIsEmptyTest);
	RUNTEST(CBufferReadTest);
	RUNTEST(CBufferWriteTest);



return 0; 
}

static test_status_t CBufferCreateTest()
{
	CBuffer_t *new_cbuffer = CBufferCreate(5);
	REQUIRE(NULL != new_cbuffer);
	CBufferDestroy(new_cbuffer);
	return PASSED;
}

static test_status_t CBufferDestroyTest()
{
	return PASSED;
}

static test_status_t CBufferSizeTest()
{
	CBuffer_t *new_cbuffer = CBufferCreate(5);
	REQUIRE(0 == CBufferSize(new_cbuffer));
	CBufferDestroy(new_cbuffer);
	return PASSED;
}

static test_status_t CBufferFreeSpaceTest()
{
	CBuffer_t *new_cbuffer = CBufferCreate(5);
	REQUIRE(5 == CBufferFreeSpace(new_cbuffer));
	CBufferDestroy(new_cbuffer);
	return PASSED;
}

static test_status_t CBufferIsEmptyTest()
{
	CBuffer_t *new_cbuffer = CBufferCreate(5);
	REQUIRE(1 == CBufferIsEmpty(new_cbuffer));
	CBufferDestroy(new_cbuffer);
	return PASSED;
}

static test_status_t CBufferWriteTest()
{
/*	CBuffer_t *new_cbuffer = CBufferCreate(15);

	REQUIRE(10 == CBufferWrite(new_cbuffer,(const void*)"daher mahagna daher mahagna",10));

	REQUIRE(5 == CBufferWrite(new_cbuffer,(const void*)"daher mahagna daher mahagna",10));

	CBufferDestroy(new_cbuffer);

    return PASSED;*/

    char *output = "Let me take you down becuase";
    char words[15] = "aaaaa aaaaaaaa";
    char blank[20] = {0};
    CBuffer_t *cbuffer = CBufferCreate(20);
    CBuffer_t *new_cbuffer = CBufferCreate(30);
    REQUIRE(NULL != cbuffer);
    REQUIRE(10 == CBufferWrite(cbuffer, (void *)words, 10)); 
    REQUIRE(5 == CBufferRead(cbuffer, (void *)blank, 5));

    REQUIRE(15 == CBufferWrite(cbuffer, (void *)words, 15));

    REQUIRE(0 == CBufferWrite(cbuffer, (void *)words, 15));
    REQUIRE(0 == CBufferWrite(cbuffer, (void *)words, 15));
    REQUIRE(5 == CBufferRead(cbuffer, (void *)blank, 5));

    REQUIRE(15 == CBufferRead(cbuffer, (void *)blank, 15));

    REQUIRE(6 == CBufferWrite(new_cbuffer, output, 6));
    REQUIRE(24 == CBufferWrite(new_cbuffer, output, 30));
    
    CBufferDestroy(new_cbuffer);
  
    CBufferDestroy(cbuffer);
    
    return PASSED;
}

static test_status_t CBufferReadTest()
{
	CBuffer_t *new_cbuffer = CBufferCreate(15);

	char test2[30] = {0};

	REQUIRE(10 == CBufferWrite(new_cbuffer,(const void*)"daher mahagna daher mahagna",10));

	REQUIRE(10 == CBufferRead(new_cbuffer,(void*)test2,10));

	REQUIRE(0 == strcmp("daher maha",test2));

	REQUIRE(10 == CBufferWrite(new_cbuffer,(const void*)"daher mahagna daher mahagna",10));

	REQUIRE(10 == CBufferRead(new_cbuffer,(void*)test2,10));

	REQUIRE(0 == strcmp("daher maha",test2));

	CBufferDestroy(new_cbuffer);

    return PASSED;

}

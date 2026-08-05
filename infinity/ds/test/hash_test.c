#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include <string.h>
#include "../utils/test.h"
#include "hash.h"



/*------------------------MACRO ---------------------------*/

#define ARRAY_LENGTH (10)
#define INDEX_LENGH (65)
/*------------------------STRUCT---------------------------*/

typedef struct student
{
	char *name;
	int id;
}student_t;


/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);


size_t HashCode(const void *data);
int CmpFunc(const void *data, const void *data_to_compare);
int PrintStudent(void *data, void *param);


/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(CaseOneTest);


	return PASSED; 
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate stage test */
/*
FUNC_TO_TEST(HashCreate);
FUNC_TO_TEST(HashDestroy);
FUNC_TO_TEST(HashIsEmpty);
FUNC_TO_TEST(HashSize);
FUNC_TO_TEST(HashInsert);
FUNC_TO_TEST(HashRemove);
FUNC_TO_TEST(HashFind);
FUNC_TO_TEST(HashForEach);
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{
	hash_table_ty *user_htable = HashCreate(HashCode,CmpFunc,ARRAY_LENGTH);

	student_t yoni = {"yoni", 2};
	student_t daher = {"daher",20};
	student_t ameer = {"ameer", 5};
	student_t ahmad = {"ahmad", 11};
	student_t mohamad = {"mohamad", 8};
	student_t sagi = {"sagi", 22};
	student_t *find_res = NULL;
	REQUIRE(NULL != user_htable);
	REQUIRE(1 == HashIsEmpty(user_htable));

	HashInsert(user_htable, (void *)&daher);
	HashInsert(user_htable, (void *)&yoni);
	HashInsert(user_htable, (void *)&ameer);
	HashInsert(user_htable, (void *)&ahmad);
	HashInsert(user_htable, (void *)&mohamad);
	HashInsert(user_htable, (void *)&sagi);

	REQUIRE(0 == HashIsEmpty(user_htable));
	find_res = (student_t*)HashFind(user_htable, (void *)&daher);
	printf("%d\n", find_res->id);
	HashForEach(user_htable,PrintStudent,NULL);

	HashRemove(user_htable, (void*)&sagi);
/* 	HashForEach(user_htable, PrintStudent, NULL); */

	FUNC_TO_TEST(HashCreate->HashSize);
	
	HashDestroy(user_htable);


	return PASSED;
}



size_t HashCode(const void *data)
{
	student_t *st = (student_t*) data;
	return st->id % (ARRAY_LENGTH);
}

int CmpFunc(const void *data, const void *data_to_compare)
{

	return data == data_to_compare;
}

int PrintStudent(void *data, void *param)
{
	student_t * st = data;
	printf("\nthe name is: %s\nthe id is: %d",st->name,st->id);
	return 0;
}


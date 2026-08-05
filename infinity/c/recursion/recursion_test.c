#include <string.h>
#include "test.h"
#include "recursion.h"
#include "../../ds/include/stack.h"

/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t StrlenTest(void);
static test_status_t StrcmpTest(void);
static test_status_t StrcpyTest(void);
static test_status_t StrcatTest(void);
static test_status_t StrstrTest(void);
static test_status_t FibonacciIterativelyTest(void);
static test_status_t FibonacciRecursivelyTest(void);
static test_status_t FlipListTest(void);
static test_status_t TestStrstr(void);
static test_status_t SortStackTest(void);

	/*-------------------------MAIN---------------------------*/

	int main()
{
	RUNTEST(StrlenTest);
	RUNTEST(StrcmpTest);
	RUNTEST(StrcpyTest);
	RUNTEST(StrcatTest);
	RUNTEST(StrstrTest);
	RUNTEST(FibonacciIterativelyTest);
	RUNTEST(FibonacciRecursivelyTest);
	RUNTEST(FlipListTest);
	RUNTEST(SortStackTest);

	RUNTEST(TestStrstr);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t StrlenTest(void)
{

	char *str1 = "aaaaaaa";
	char *str2 = "bbbbbb";
	char *str3 = "daher";
	char *str4 = "";

	REQUIRE(7 == Strlen(str1));
	REQUIRE(6 == Strlen(str2));
	REQUIRE(5 == Strlen(str3));
	REQUIRE(0 == Strlen(str4));


	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t StrcmpTest(void)
{
	char *str1 = "aaaaaaa";
	char *str2 = "bbbbb";
	char *str3 = "daher";
	char *str4 = "daher";
	char *str5 = "daheb";
	char *str6 = ""; 
	
	REQUIRE(-1 == Strcmp(str1, str2));
	REQUIRE(-2 == Strcmp(str2, str3));
	REQUIRE(0 == Strcmp(str3, str4));
	REQUIRE(0 == Strcmp(str3, str3));
	REQUIRE(16 == Strcmp(str4, str5));
	REQUIRE(100 == strcmp(str5, str6));
	REQUIRE(-100 == Strcmp(str6, str5));

	REQUIRE(strcmp(str1, str2) == Strcmp(str1, str2));
	REQUIRE(Strcmp(str2, str3) == Strcmp(str2, str3));
	REQUIRE(Strcmp(str3, str4) == Strcmp(str3, str4));
	REQUIRE(Strcmp(str3, str3) == Strcmp(str3, str3));
	REQUIRE(Strcmp(str4, str5) == Strcmp(str4, str5));
	REQUIRE(Strcmp(str5, str6) == strcmp(str5, str6));
	REQUIRE(Strcmp(str6, str5) == Strcmp(str6, str5));
	printf("\nthis is the result of strcmp %d\n", strcmp("", "daher"));

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t StrcpyTest(void)
{
	char *str1 = "aaaaaaa";
	char *str2 = "bbbbb";
	char *str3 = "daher";
	char *str4 = "daheb";
	char *str5 = NULL;
	char str6[10] = {0};
	str5 = Strcpy(str6,str3);
	
	REQUIRE(0 == Strcmp(str3, str6));
	REQUIRE(0 == Strcmp(str3, str5));
	str4 = Strcpy(str5,str1);
	REQUIRE(0 == Strcmp(str1, str5));
	REQUIRE(0 == Strcmp(str1, str4));
	strcpy(str5,str2);
	REQUIRE(0 == Strcmp(str5, str2));

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t StrcatTest(void)
{
	char *str1 = "aaaaaaa";
	char str2[30] = "daher";
	char *str3 = "daheb";
	char *str4 = NULL;
	char str5[10] = {0};
	str4 = Strcat(str2, str1);
	REQUIRE(0 == Strcmp("daheraaaaaaa", str2));
	REQUIRE(0 == Strcmp("daheraaaaaaa", str4));
	str4 = Strcat(str5,str3);
	REQUIRE(0 == Strcmp(str5, str3));
	REQUIRE(0 == Strcmp(str4, str3));
	

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t StrstrTest(void)
{
	char *str1 = "aaaaaaadaher22";
	char str2[30] = "daher";
	char *str3 = "zdaher";
	char *str4;
	char *str5 = "daherz";
	str4 = Strstr(str1,str2);
	REQUIRE(0 == Strcmp(str4, "daher22"));
	str4 = Strstr(str1,str3);
	REQUIRE(NULL == str4);
	str4 = Strstr(str1, str5);
	REQUIRE(NULL == str4);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t FibonacciIterativelyTest(void)
{
	int n = 1;
	REQUIRE(1 == FibonacciIteratively(n));
	n = 2;
	REQUIRE(1 == FibonacciIteratively(n));
	n = 10;
	REQUIRE(55 == FibonacciIteratively(n));
	n = 40;
	REQUIRE(102334155 == FibonacciIteratively(n));

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t FibonacciRecursivelyTest(void)
{
	int n = 1;
	REQUIRE(1 == FibonacciRecursively(n));
	n = 2;
	REQUIRE(1 == FibonacciRecursively(n));
	n = 10;
	REQUIRE(55 == FibonacciRecursively(n));
	n = 40;
	REQUIRE(102334155 == FibonacciRecursively(n));

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t FlipListTest(void)
{
	node_t *head;
	node_t node1, node2, node3, node4, node5;
	int res = 0;

	node1.data = (void *)1;
	node1.next = &node2;
	node2.data = (void *)2;
	node2.next = &node3;
	node3.data = (void *)3;
	node3.next = &node4;
	node4.data = (void *)4;
	node4.next = &node5;
	node5.data = (void *)5;
	node5.next = NULL;

	head = &node1;
	FlipList(head);
	head = &node5;

	while (NULL != head)
	{
		res = res * 10 + *(int *)&head->data;
		head = head->next;
	}
	/* printf("%d\n", *(int *)&node5.next->data);
	printf("%d\n", *(int *)&node4.next->data);
	printf("%d\n", *(int *)&node3.next->data);
	printf("%d\n", *(int *)&node2.next->data); */
	

	REQUIRE(res == 54321);
	return PASSED;
}

static test_status_t TestStrstr(void)
{
	const char *haystack1 = "Hellohel";
	const char *needle1 = "hel";
	const char *haystack2 = "hellohel";
	const char *needle2 = "hel";
	const char *haystack3 = "hellohel";
	const char *needle3 = "heoh";
	const char *haystack4 = "";
	const char *needle4 = "";


	REQUIRE(0 == strcmp(strstr(haystack1, needle1), Strstr(haystack1, needle1)));
	REQUIRE(0 == strcmp(strstr(haystack2, needle2), Strstr(haystack2, needle2)));
	/* printf("%s\n",Strstr(haystack3, needle3)); */
	REQUIRE(NULL == Strstr(haystack3, needle3));
	REQUIRE(haystack4 == Strstr(haystack4, needle4));

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortStackTest(void)
{
	stack_t *stack = StackCreate(10);
	StackPush(stack, (void *)10);
	StackPush(stack, (void *)100);
	StackPush(stack, (void *)1);
	StackPush(stack, (void *)15);
	StackPush(stack, (void *)16);
	StackPush(stack, (void *)15);
	StackPush(stack, (void *)1);
	StackPush(stack, (void *)101);
	SortStack(stack);
	REQUIRE((void *)101 == StackPeek(stack));
	StackPop(stack);
	REQUIRE((void *)100 == StackPeek(stack));
	StackPop(stack);
	REQUIRE((void *)16 == StackPeek(stack));
	StackPop(stack);
	REQUIRE((void *)15 == StackPeek(stack));
	StackPop(stack);
	REQUIRE((void *)15 == StackPeek(stack));
	StackPop(stack);
	REQUIRE((void *)10 == StackPeek(stack));
	StackPop(stack);
	REQUIRE((void *)1 == StackPeek(stack));
	StackPop(stack);
	REQUIRE((void *)1 == StackPeek(stack));
	StackDestroy(stack);
	return PASSED;
}
#include <time.h> /*time, clock*/
#include "../test.h"
#include "calc.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseThreeTest(void);
static test_status_t CaseFourTest(void);
static test_status_t CaseFiveTest(void);
static test_status_t CaseSixTest(void);
static test_status_t CaseSevenTest(void);

	/*-------------------------MAIN---------------------------*/

	int main()
{
	clock_t start, end;
	int x = 5, y = 5;
	start = clock();
	RUNTEST(CaseOneTest);
	RUNTEST(CaseTwoTest);
	RUNTEST(CaseThreeTest);
	RUNTEST(CaseFourTest);
	RUNTEST(CaseFiveTest);
	RUNTEST(CaseSixTest);
	RUNTEST(CaseSevenTest);
	end = clock();
	printf("\ntime exe: %ld\n", end - start);

	start = clock();
	x+=y;
	end = clock();
	printf("\ntime for x+y: %ld\n", end - start);

	return PASSED; 
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate stage test */
/*
FUNC_TO_TEST(Calculate);
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{

	int exit_status = 0;
	double res = 0;
	FUNC_TO_TEST(Plus);
	res = Calculate("(12)", &exit_status);
	printf("%f\n",res);
	REQUIRE(res == 12);
	res = Calculate("2+3+2", &exit_status);
	REQUIRE(res == 7);
	res = Calculate("20+30+20", &exit_status);
	REQUIRE(res == 70);
	res = Calculate("-20+30+-20", &exit_status);
	REQUIRE(res == -10);
	res = Calculate("+30+20", &exit_status);
	REQUIRE(res == 50);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	int exit_status = 0;
	double res = 0;
	FUNC_TO_TEST(Minus);
	res = Calculate("2-3-2", &exit_status);
	REQUIRE(res == -3);
	res = Calculate("20-30-20", &exit_status);
	REQUIRE(res == -30);
	res = Calculate("-20-30-20", &exit_status);
	REQUIRE(res == -70);
	res = Calculate("-30-20", &exit_status);
	REQUIRE(res == -50);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseThreeTest(void)
{
	int exit_status = 0;
	double res = 0;
	FUNC_TO_TEST(Multiply);
	res = Calculate("2*3*5*9", &exit_status);
	REQUIRE(res == 270);
	res = Calculate("20*30*2", &exit_status);
	REQUIRE(res == 1200);
	res = Calculate("-5*2*-2", &exit_status);
	REQUIRE(res == 20);
	res = Calculate("2*-50*0", &exit_status);
	REQUIRE(res == 0);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFourTest(void)
{
	int exit_status = 0;
	double res = 0;
	FUNC_TO_TEST(divide);
	res = Calculate("9/3", &exit_status);
	REQUIRE(res == 3);
	res = Calculate("20/0", &exit_status);
	REQUIRE(exit_status == MATH_ERROR);
	res = Calculate("18/2/3", &exit_status);
	REQUIRE(res == 3);
	

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFiveTest(void)
{
	int exit_status = 0;
	double res = 0;
	FUNC_TO_TEST(Power);
	res = Calculate("2^3", &exit_status);
	REQUIRE(res == 8);
	res = Calculate("3^2^2^2", &exit_status);
	REQUIRE(res == 43046721);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseSixTest(void)
{
	int exit_status = 0;
	double res = 0;
	FUNC_TO_TEST(parantheses);
	res = Calculate("2^([(3+1*5)*2]/4+5)", &exit_status);
	
	REQUIRE(res == 512);
	res = Calculate("2^{[(3+1)*2+2]/2+5}", &exit_status);
	
	REQUIRE(res == 1024);
	
	res = Calculate("-5^1.2", &exit_status);

	REQUIRE(exit_status == MATH_ERROR);
	res = Calculate("(5+3}", &exit_status);
	REQUIRE(exit_status == SYNTAX_ERROR);
	res = Calculate("((3+1)", &exit_status);
	REQUIRE(exit_status == SYNTAX_ERROR);

	return PASSED;
}
static test_status_t CaseSevenTest(void)
{
	int error = 0;
	
	Calculate("(1+", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	Calculate("(})", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	Calculate("1&", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	Calculate("1+$$", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	Calculate("1+*", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	Calculate("1+)", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	Calculate("1 +5", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	Calculate("(5+3}", &error);
	REQUIRE(SYNTAX_ERROR == error);
	error = 0;

	FUNC_TO_TEST(Calculate_syntax_error);
	return PASSED;
}
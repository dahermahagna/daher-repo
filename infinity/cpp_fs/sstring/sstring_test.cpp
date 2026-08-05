#include <cstring>   //strcmp
#include <iostream>  //cout

#include "sstring.hpp"
#include "../utils/test.hpp"

using namespace std;
using namespace ilrd;

static test_status_t TestMemberFunction(void);
static test_status_t TestOperators(void);
static test_status_t TestSelfAssign(void);

int main()
{
    RUNTEST(TestMemberFunction);
    RUNTEST(TestOperators);
    RUNTEST(TestSelfAssign);

    return 0;
}

static test_status_t TestMemberFunction(void)
{
    // Length, Cstr, ctor, cCtor, assign
    String first("hello");

    REQUIRE(5 == first.Length());
    REQUIRE(0 == strcmp("hello", first.CStr()));

    String second("second");
    first = second;
    REQUIRE(0 == strcmp("second", first.CStr()));

    return PASSED;
}

static test_status_t TestOperators(void)
{
    // ==, >, <, << , >>  
    String first("hello");
    String second("hello");
    String third("bla");

    REQUIRE(first == second);
    REQUIRE(third < first);
    REQUIRE((first > third));

    cout << "test << " << first << endl;
    cin >> second;
    cout << "after >> " << second << endl;
   
    return PASSED;
}

static test_status_t TestSelfAssign(void)
{
    // self assign, potential overflow
    
    String first("hello");
    String second("bla");

    first = first;
    REQUIRE(0 == strcmp("hello", first.CStr()));
    
    second = first;
    REQUIRE(first == second);
      
   
    return PASSED;
}

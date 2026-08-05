#include "test.hpp"
#include "framework.hpp"

//----------- Functions Decleration --------------

test_status_t FirstCaseTest(void);
test_status_t SecondCaseTest(void);

//-------------------- Main ----------------------

int main()
{
    RUNTEST(FirstCaseTest);
    RUNTEST(SecondCaseTest);
    return 0;
}

//-------------Functions Definetion --------------

test_status_t FirstCaseTest()
{
    FUNC_TO_TEST(write the case here);
    REQUIRE(1==1);
    return PASSED;
}
test_status_t SecondCaseTest()
{
    FUNC_TO_TEST(write the case here);
    REQUIRE(1==1);
    return PASSED;
}

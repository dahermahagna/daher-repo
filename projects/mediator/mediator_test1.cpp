#include "test.hpp"
#include "mediator.hpp"

//----------- Functions Decleration --------------

test_status_t FirstCaseTest(void);

//-------------------- Main ----------------------
using namespace ilrd;
int main()
{
    RUNTEST(FirstCaseTest);
    return 0;
}

//-------------Functions Definetion --------------

class Iproxy : public ProxyBase
{

};

test_status_t FirstCaseTest()
{
    FUNC_TO_TEST(write the case here);
    ThreadPool th;
    
    Mediator m1();
    
    REQUIRE(1==1);
    return PASSED;
}


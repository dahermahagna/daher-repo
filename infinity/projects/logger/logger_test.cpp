#include "../utils/test.hpp"
#include "logger.hpp"

//----------- Functions Decleration --------------

test_status_t FirstCaseTest(void);
test_status_t SecondCaseTest(void);

//-------------------- Main ----------------------
using namespace ilrd;
int main()
{
    RUNTEST(FirstCaseTest);
    RUNTEST(SecondCaseTest);
    return 0;
}

//-------------Functions Definetion --------------

test_status_t FirstCaseTest()
{
    setenv("LOG_PATH", "/home/daher/git/projects/logger/test/log_file" , 1);
    Logger *logger = Singleton<Logger>::GetInstance();
    logger->SetLevel(logger->INFORMATION);
    LOG_INFO("test num 1");
    sleep(2);
    LOG_WARNING("not exist in logger");
    sleep(2);
    logger->SetLevel(logger->WARNING);
    LOG_WARNING("test num 2");
    sleep(2);
    
    return PASSED;
}
test_status_t SecondCaseTest()
{
    FUNC_TO_TEST(write the case here);
    REQUIRE(1==1);
    return PASSED;
}



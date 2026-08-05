#include <test.hpp>
#include "dir_monitor.hpp"

//----------- Functions Decleration --------------
using namespace ilrd;
test_status_t FirstCaseTest(void);

//-------------------- Main ----------------------

int main()
{
    RUNTEST(FirstCaseTest);

    return 0;
}

//-------------Functions Definetion --------------

test_status_t FirstCaseTest()
{
    FUNC_TO_TEST(write the case here);

    const char *PATH = "/home/daher/git/projects/dir_monitor/test/";
    DirMonitor dm(PATH);
    DllLoader aa(&dm);
    sleep(30);
    dm.StopMonitor();

    return PASSED;
}


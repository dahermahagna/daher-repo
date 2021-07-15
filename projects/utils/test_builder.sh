if [ -f "$1"_test.cpp ]; then
echo "$1_test.cpp exists."
else
touch "$1"_test.cpp
echo  '#include "../utils/test.hpp"
#include "'"$1"'.hpp"

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

' >> "$1"_test.cpp
fi

#include <iostream>

#include "../utils/test.hpp"

#include "bit_array_impl.hpp"

//----------- functions decleration --------------

test_status_t FirstPhaseTest(void);
test_status_t SecondphaseTest(void);

//-------------------- main ----------------------
using namespace ilrd;
int main()
{
    RUNTEST(FirstPhaseTest);
    RUNTEST(SecondphaseTest);
    return 0;
}

//-------------functions definetion --------------

test_status_t FirstPhaseTest()
{

    BitArray<100> b;


    b[95] = true;
    REQUIRE(true == b[95]);
    REQUIRE(false != b[95]);

    BitArray <100> b1;

    b1[60] = b[95];
    REQUIRE(true == b1[60]);
    REQUIRE(false == b[32]);
    b1[30] = !b[32];
    REQUIRE(true == b1[30]);

    REQUIRE(b1[60] && b[95] && b1[30] && !b[32]);

    const size_t size = 100;
    BitArray<size> b2;
    
    return PASSED;
}
test_status_t SecondphaseTest()
{
    BitArray<100> b1;
    b1.Set();
    REQUIRE(true == b1[1]);
    REQUIRE(false == !b1[1]);

    REQUIRE(true == b1[97]);
    BitArray<100> b2;
    REQUIRE(b1 != b2);
    b2 &= b1;
    REQUIRE(false == b2[1]);
    b2 |= b1;
    REQUIRE(b1 == b2);

    REQUIRE(true == b2[10]);
    b2.Set(10,false);
    REQUIRE(false == b2[10]);
    b2.Flip();
    BitArray<100> b3;
    b3.Set(10);
    REQUIRE(b2 == b3);
    b3.Flip(11);
    REQUIRE(true == b3[11]);

    REQUIRE(2 == b3.Count());
    b3[4] = true;
    b3[99] = true;
    b3.Set(68);
    REQUIRE(5 == b3.Count());
    REQUIRE(100 == b1.Count());
    REQUIRE(1 == b2.Count());
    std::cout << "\nb1: "<<b1.ToString() <<std::endl;
    std::cout << "b2: "<< b2.ToString() << std::endl;
    std::cout << "b3: "  << b3.ToString() << std::endl;
    BitArray<10> b4;
    b4.Set();
    std::cout << "b4: " << b4.ToString() << std::endl;
    return PASSED;
}



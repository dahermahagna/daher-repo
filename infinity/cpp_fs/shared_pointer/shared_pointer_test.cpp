#include <iostream>
#include "shared_pointer_impl.hpp"
#include "../utils/test.hpp"

using namespace ilrd;

/*----------------- classes ---------------*/
class A
{
    public:
    A(int x_=5):m_x(x_)
    {}
    void PrintX()
    {
        std::cout << m_x <<std::endl;
    }
    void IncX()
    {
        ++m_x;
    }
    int GetX() const
    {
        return m_x;
    }
    virtual ~A()
    {

    }
    private:
    int m_x;

};
class B :public A 
{
    public:
    virtual ~B()
    {}
};


test_status_t BasicRCTest(void);
test_status_t UpCastingTest(void);

/*------------------ main -----------------*/
int main()
{

    RUNTEST(BasicRCTest);
    RUNTEST(UpCastingTest);


    return 0;
}
void foo(SharedPtr<int> p)
{
    std::cout << *p << std::endl;
}
test_status_t BasicRCTest()
{

    SharedPtr<int> ptr1(new int(500));
    std::cout << *ptr1 << std::endl;
    *ptr1 = 5;
    int a = *ptr1;
    std::cout << a << std::endl;
    SharedPtr<A> pta(new A);
    SharedPtr<A> ptb(new A);
    pta->PrintX();
    pta = ptb;
    ptb->IncX();
    pta->PrintX();
    ptb->PrintX();

    
    SharedPtr<int> paa(new int(2));
    foo(paa);

    return PASSED;
}

test_status_t UpCastingTest(void)
{
    SharedPtr<A> pta(new A);
    SharedPtr<B> ptb(new B);
    pta->PrintX();
    ptb->PrintX();

    pta = ptb;    /*1. tmp = SharedPtr<T>(const SharedPtr<D> &ptb);
                    2.  pta = tmp*/


    pta->PrintX();
    ptb->PrintX();

    SharedPtr<A> ptc;
    ptc = pta;
    ptc->PrintX();

    SharedPtr<A> p_A(new A(2));
    p_A = p_A;
    REQUIRE(2 == p_A->GetX());

    return PASSED;
}
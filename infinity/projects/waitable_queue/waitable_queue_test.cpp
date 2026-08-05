#include <iostream>
#include <boost/thread.hpp>
#include "../utils/test.hpp"
#include "waitable_queue_impl.hpp"
#include "pq_wrapper_impl.hpp"

//----------- Functions Decleration --------------

test_status_t FirstCaseTest(void);
test_status_t SecondCaseTest(void);
void PushThread(int num);
void PopTimeOutThread(int *num);
void PushThreadPq(int num);
void PopTimeOutThreadPq(int *num);


//-------------------- Main ----------------------
using namespace ilrd;
WaitableQueue<int> q;
WaitableQueue<int, PQWrapper<int>> pq;
int main()
{
    RUNTEST(FirstCaseTest);
    RUNTEST(SecondCaseTest);
    return 0;
}

//-------------Functions Definition --------------
void PushThread(int num)
{
    sleep(2);
    q.Push(num);
}
void PopTimeOutThread(int *num)
{
    boost::chrono::seconds to(5);
    bool status = q.Pop(num,to);
    if (status)
    {
        std::cout << "succeed to pop " << *num << std::endl;
    }
    else
    {
        std::cout << "failed to pop " << std::endl;
    }
}


test_status_t FirstCaseTest()
{
    FUNC_TO_TEST(write the case here);
    int data = 0;
    boost::thread t (PopTimeOutThread,&data);
    t.join();
    boost::thread t0(PopTimeOutThread, &data);
    boost::thread t1(PushThread, 4);
    boost::thread t2(PushThread, 3);
    boost::thread t3(PushThread, 2);
    boost::thread t4(PushThread, 1);
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    q.Pop(&data);
    std::cout << data << std::endl;
    q.Pop(&data);
    std::cout << data << std::endl;
    q.Pop(&data);
    std::cout << data << std::endl;
    /* q.Pop(&data);
    std::cout << data << std::endl; */

    return PASSED;
}

void PushThreadPq(int num)
{
    sleep(2);
    pq.Push(num);
}
void PopTimeOutThreadPq(int *num)
{
    boost::chrono::seconds to(5);
    bool status = pq.Pop(num, to);
    if(status)
    {
        std::cout << "succeed to pop " << *num << std::endl;

    }
    else
    {
        std::cout << "failed to pop "<< std::endl;
    }
    
}
test_status_t SecondCaseTest()
{
    FUNC_TO_TEST(write the case here);
    int data = 0;
    boost::thread t(PopTimeOutThreadPq, &data);
    t.join();
    boost::thread t0(PopTimeOutThreadPq, &data);
    boost::thread t1(PushThreadPq, 4);
    boost::thread t2(PushThreadPq, 3);
    boost::thread t3(PushThreadPq, 2);
    boost::thread t4(PushThreadPq, 1);
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    pq.Pop(&data);
    std::cout << data << std::endl;
    pq.Pop(&data);
    std::cout << data << std::endl;
    pq.Pop(&data);
    std::cout << data << std::endl;

    return PASSED;
}



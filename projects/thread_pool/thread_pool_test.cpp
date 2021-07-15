#include <iostream>
#include "../utils/test.hpp"
#include "thread_pool.hpp"
#include "FuturTask_impl.hpp"

//----------- Functions Decleration --------------

static test_status_t FirstCaseTest(void);
static test_status_t TestFunctionExist(void);
static test_status_t TestFuture(void);

//-------------------- Main ----------------------
using namespace ilrd;
int main()
{
    RUNTEST(FirstCaseTest);
    RUNTEST(TestFunctionExist);
    RUNTEST(TestFuture);
    return 0;
}

//-------------Functions Definetion --------------

class ExecTask: public ThreadPool::Task
{
public:
    virtual void Do();
    virtual ~ExecTask();
};
void ExecTask::Do()
{
    
    write(STDOUT_FILENO,"Do somthing !!\n",16);
    //std::cout << "Do somthing !!!!!!!!" << std::endl;
}
ExecTask::~ExecTask()
{

}
class ExecTaskFunc : public ThreadPool::Task
{
public:
    explicit ExecTaskFunc(std::function<void(size_t)> func_,size_t count);
    virtual void Do();
    virtual ~ExecTaskFunc();
private:
std::function<void(size_t)> m_func;
size_t m_count;
};
ExecTaskFunc::ExecTaskFunc(std::function<void(size_t)> func_,size_t count)
:m_func(func_),m_count(count)
{

}
void ExecTaskFunc::Do()
{
   m_func(m_count);
}
ExecTaskFunc::~ExecTaskFunc()
{
}

void PrintMyName(size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        /* code */
        write(STDOUT_FILENO,"daher\n",7);
    }
    
}
void PrintMyLastName(size_t count)
{
    for (size_t i = 0; i < count; i++)
    //while(true)
    {
        /* code */
        write(STDOUT_FILENO, "mahagna\n", 9);
    }
}

static test_status_t FirstCaseTest()
{
    //FUNC_TO_TEST(write the case here);
    ThreadPool t(5,5,2);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr1(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr2(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr3(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr4(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr5(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr6(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr7(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr8(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr9(new ExecTask);
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr10(new ExecTaskFunc(PrintMyName,4));
    boost::shared_ptr<ilrd::ThreadPool::Task> task_ptr11(new ExecTaskFunc(PrintMyLastName, 4));

    t.Add(task_ptr1);
    t.Add(task_ptr2);
    t.Add(task_ptr3);
    t.Add(task_ptr4);
    t.Add(task_ptr5);
    t.Add(task_ptr6);
    t.Add(task_ptr7);
    t.Add(task_ptr8);
    t.Add(task_ptr9); 
    t.Add(task_ptr10,ThreadPool::HIGH);
    t.Run();
    sleep(1);

    t.Pause();
    t.Run();
    sleep(1);
    t.Add(task_ptr1);
    t.Add(task_ptr11,ThreadPool::LOW);
    sleep(1);
    t.Resize(3);
    t.Resize(10);
    t.Resize(6);
    t.Add(task_ptr11);
    t.Add(task_ptr1);
    t.Add(task_ptr2);
    t.Add(task_ptr3);
    sleep(1);
    t.Stop();
    sleep(1);

    REQUIRE(1==1);
    return PASSED;
}
void PrintMyNameOnce()
{
    std::cout << "\n Daher" << std::endl;
}

static test_status_t TestFunctionExist(void)
{
    ThreadPool th_pool;

    boost::function<void()> exist_fun = PrintMyNameOnce;

    boost::shared_ptr<FunctionTask> fun_ptr(new FunctionTask(exist_fun));

    th_pool.Add(fun_ptr, ThreadPool::HIGH);

    th_pool.Run();

    sleep(2);

    REQUIRE(1 == 1);

    return PASSED;
}

double PrintMyNameAndReturn15_1()
{
    std::cout << "Daher" << std::endl;

    return 15.1;
}

static test_status_t TestFuture(void)
{
    ThreadPool th_pool;

    boost::function<double()> future_fun = PrintMyNameAndReturn15_1;

    boost::shared_ptr<FutureTask<double>> fun_ptr(new FutureTask<double>(future_fun));

    th_pool.Add(fun_ptr, ThreadPool::HIGH);

    th_pool.Run();

    sleep(2);

    if (fun_ptr->IsReady())
    {
        double res = fun_ptr->GetValue();

        std::cout << "res is " << res << std::endl;
    }
    else
    {
        std::cout << "not ready!" << std::endl;
    }

    REQUIRE(1 == 1);

    return PASSED;
}

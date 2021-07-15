#ifndef __RD94_FUTURE_TASK_HPP__
#define __RD94_FUTURE_TASK_HPP__

#include <boost/thread/future.hpp>// promise future
#include <boost/function.hpp> // function
#include "thread_pool.hpp"

namespace ilrd
{

class FunctionTask : public ThreadPool::Task
{
public:
    explicit FunctionTask(boost::function<void ()> func_);
    virtual ~FunctionTask();
    virtual void Do();
    
private:
    boost::function<void ()> m_func;
};


template <typename T>
class FutureTask : public ThreadPool::Task
{
public:
    explicit FutureTask(boost::function<T ()> func_);
    virtual void Do();
    bool IsReady() const;
    T GetValue();

private:
   boost::promise<T> m_promise;
   boost::unique_future<T> m_future_obj;
   boost::function<T ()> m_func;

};


}// ilrd







#endif // __RD94_FUTURE_TASK_HPP__
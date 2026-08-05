#ifndef __FUTURE_TASK_IMPL_HPP__
#define __FUTURE_TASK_IMPL_HPP__

#include "FuturTask.hpp"//future 

namespace ilrd
{

FunctionTask::FunctionTask(boost::function<void ()> func_)
:m_func(func_)
{}
void FunctionTask::Do()
{
   m_func();
}
FunctionTask::~FunctionTask()
{}

template <typename T>
FutureTask<T>::FutureTask(boost::function<T ()> func_)
:m_func(func_)
,m_promise()
,m_future_obj(m_promise.get_future())
{}

template <typename T>
void FutureTask<T>::Do()
{
    m_promise.set_value(m_func());
}

template <typename T>
bool FutureTask<T>::IsReady() const
{
    return m_future_obj.is_ready();
}

template <typename T>
T FutureTask<T>::GetValue()
{
    return m_future_obj.get();
}

}// ilrd

#endif // __FUTURE_TASK_IMPL_HPP__
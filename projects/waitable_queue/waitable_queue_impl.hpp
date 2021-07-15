#ifndef __WAITABLE_Q_IMPL_HPP
#define __WAITABLE_Q_IMPL_HPP

#include <cassert> // assert

#include "waitable_queue.hpp"

namespace ilrd
{

template <typename T, typename CONTAINER>
void  WaitableQueue<T,CONTAINER>::Push(const T& data_)
{
    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_queue.push(data_);
    }
    m_condition.notify_one();
}

template <typename T, typename CONTAINER>
void WaitableQueue<T, CONTAINER>::Pop(T *data_)
{
    assert(NULL != data_);
    boost::unique_lock<boost::mutex> lock(m_mutex);
    while(m_queue.empty())
    {
        m_condition.wait(lock);
    }
    *data_ = m_queue.front();
    m_queue.pop();
    
}

template <typename T, typename CONTAINER>
bool WaitableQueue<T, CONTAINER>::Pop(T *data_, boost::chrono::seconds timeout_)
{
    assert(NULL != data_);
    boost::unique_lock<boost::mutex> lock(m_mutex);
    
    while (m_queue.empty())
    {
        if(m_condition.wait_for(lock,timeout_) == boost::cv_status::timeout)
        {

            return false;
        }
    }
    *data_ = m_queue.front();
    m_queue.pop();
    return true;
}
template <typename T, typename CONTAINER>
bool WaitableQueue<T, CONTAINER>::IsEmpty() const noexcept
{
    boost::unique_lock<boost::mutex> lock(m_mutex);
    return m_queue.empty();
}

} //ilrd

#endif //__WAITABLE_Q_IMPL_HPP
#ifndef __ILRD_RD94__WAITABLE_Q_HPP
#define __ILRD_RD94__WAITABLE_Q_HPP

#include <boost/core/noncopyable.hpp>   //noncopyable
#include <boost/chrono.hpp> // seconds
#include <boost/thread/mutex.hpp> // mutex
#include <boost/thread/condition.hpp> // cond var
#include <queue> //queue
#include "../utils/utils.hpp"

namespace ilrd
{

template <typename T, typename CONTAINER = std::queue<T> >
class WaitableQueue : private boost::noncopyable
{
public:
    // generated ctor, dtor is good enough
    void Push(const T& data_); //throws bad_alloc
    void Pop(T * data_);
    bool Pop(T *data_, boost::chrono::seconds timeout_); // true if success, false if timeout
    bool IsEmpty() const noexcept;

private:
    CONTAINER m_queue;
    mutable boost::mutex m_mutex;
    boost::condition_variable m_condition;
};

} //ilrd

#endif //__ILRD_RD94__WAITABLE_Q_HPP
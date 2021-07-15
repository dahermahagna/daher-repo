#ifndef __SINGLETON_IMPL_HPP__
#define __SINGLETON_IMPL_HPP__

#include<boost/thread/mutex.hpp>    //lock guard
#include <boost/thread.hpp>         //threads
#include <cstdlib>                  //atexit
#include <boost/atomic.hpp>
#include "singleton.hpp"
namespace ilrd
{
template <typename T>    
boost::atomic<T *>Singleton<T>::m_Instance(0);

template <typename T>
boost::mutex Singleton<T>::m_mutex;

template <typename T>
T* Singleton<T>::GetInstance()
{
    T *tmp = m_Instance.load(boost::memory_order_consume);
    if(!tmp)
    {   
        boost::lock_guard<boost::mutex> lock(m_mutex);
        tmp = m_Instance.load(boost::memory_order_consume);
        if (!tmp)
        {
            tmp = new T;

            m_Instance.store(tmp,boost::memory_order_release);
            atexit(DeleteInstance);
        }
    }
    return tmp;
}

template <typename T>
void Singleton<T>::DeleteInstance()
{
    /* T *tmp = m_Instance.load(boost::memory_order_relaxed);
    delete tmp; */
    delete m_Instance.load(boost::memory_order_relaxed);
    m_Instance.store(NULL, boost::memory_order_relaxed);
}
   


}// ilrd

#endif // __SINGLETON_HPP__
#include <iostream>
#include <pthread.h>
#include <boost/thread/mutex.hpp>
#include "scope_lock.hpp"

namespace ilrd
{

template <>
ScopeLock<pthread_mutex_t>::ScopeLock(pthread_mutex_t &lock_)
:m_lock(lock_)
{
  pthread_mutex_lock(&m_lock);
}

template <>
ScopeLock<boost::mutex>::ScopeLock(boost::mutex &lock_)
    : m_lock(lock_)
{
  m_lock.lock();
}

template <>
ScopeLock<pthread_mutex_t>::~ScopeLock() noexcept
{
    pthread_mutex_unlock(&m_lock);
}

template <>
ScopeLock<boost::mutex>::~ScopeLock() noexcept
{
  try
  {
    m_lock.unlock();
  }
  catch(...)
  {
    //swallow the exceptions
  }    
}

} // namespace ilrd

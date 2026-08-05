#ifndef __RD94_SINGLETON_HPP__
#define __RD94_SINGLETON_HPP__

#include <boost/thread/mutex.hpp> //mutex
#include <boost/atomic.hpp>
#include <boost/core/noncopyable.hpp>
#include "../utils/utils.hpp"

namespace ilrd
{


template<typename T>
class Singleton: private boost::noncopyable
{
public:
    
    static T* GetInstance();

private:
    Singleton();
    static void DeleteInstance();
    static boost::atomic<T*> m_Instance;
    static boost::mutex m_mutex; 

};
    
}// ilrd

#endif // __RD94_SINGLETON_HPP__
/******************************************************************************
*				C++: Scope lock                					      
*				code by: 												  
* 				review by: 
*               test by: 
*				last edit: 11/1/21						 					  
******************************************************************************/

#ifndef ILRD_94_SCOPE_LOCK_HPP
#define ILRD_94_SCOPE_LOCK_HPP

#if cplusplus < 201104
#define noexcept throw()
#endif //#if cplusplus<201104

#include <boost/core/noncopyable.hpp>   //uncopialbe

namespace ilrd
{

template <typename T>
class ScopeLock : private boost::noncopyable
{
public:
    explicit ScopeLock(T &lock_);  //may throw
    ~ScopeLock() noexcept;                  

private:
    T& m_lock;
};

}//ilrd


#endif // ILRD_94_SCOPE_LOCK_HPP


//#define UNUSED(arg) {if(&arg) /* null */}; if someone wants

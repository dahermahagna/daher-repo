#ifndef SharedPtr_IMPL_HPP
#define SharedPtr_IMPL_HPP

#include "shared_pointer.hpp"
namespace ilrd
{

template <typename T>
SharedPtr<T>::SharedPtr(T *ptr_) 
: m_ptr(ptr_), 
m_counter()
{
    try
    {
        m_counter = new size_t(1);
    }
    catch(const std::bad_alloc & e)
    {
        delete m_ptr;
        std::cerr << e.what() << '\n';
        throw e;
    }
    
}

template <typename T>
SharedPtr<T>::~SharedPtr() noexcept
{
    if(0 == --*m_counter)
    {
        delete m_ptr;
        m_ptr = Badptr<T>();
        delete m_counter;
        m_counter = Badptr<size_t>();
    }
    
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other_)
:m_ptr(other_.m_ptr),
m_counter(other_.m_counter)
{
    ++*m_counter;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T> &other_)
{

    T* tmp_ptr = m_ptr;
    size_t* tmp_counter = m_counter;

    --*m_counter;

    m_ptr = other_.m_ptr;
    m_counter = other_.m_counter;

    ++*m_counter;

    if (0 == *tmp_counter)
    {
        delete tmp_ptr;
        tmp_ptr = Badptr<T>();
        delete tmp_counter;
        tmp_counter = Badptr<size_t>();
    }
    
    return *this;
}

template <typename T>
T &SharedPtr<T>:: operator*() const
{
    return *m_ptr;
}
template <typename T>
T *SharedPtr<T>::operator->() const
{
    return m_ptr;
}

template <typename T>
template <typename D>
SharedPtr<T>::SharedPtr(const SharedPtr<D> &other_)
:m_ptr(static_cast<T*>(other_.m_ptr)),
m_counter(other_.m_counter)
{
    ++*m_counter;
}

} // namespace ilrd
#endif // SharedPtr_IMPL_HPP
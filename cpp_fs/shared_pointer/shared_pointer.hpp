#ifndef ILRD_94_SharedPtr_HPP
#define ILRD_94_SharedPtr_HPP

#include "../utils/utils.hpp" //noexcept
#include <iosfwd> //size_t
namespace ilrd
{

template <typename T>
T *Badptr()
{
    return reinterpret_cast<T *>(0xBADDCAFE);
}
template <typename T>
class SharedPtr;

template <typename T>
T *GetMPTR(SharedPtr<T> &Ptr_);

template <typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T *ptr_ = NULL); // pointer must point to memory allocated by
                                        //new (and not new[], malloc etc.),
                                        //otherwise undefined behavior
                                        //throws bad alloc

    ~SharedPtr() noexcept;
    SharedPtr(const SharedPtr<T> &other_);
    SharedPtr<T> &operator=(const SharedPtr<T> &other_);

    template <typename D>
    SharedPtr(const SharedPtr<D> &other_); // conversion constructor
                                            // D should be derived from TS

    T &operator*() const; //The behavior is undefined if the stored pointer is null.
    T *operator->() const;

private:
    T *m_ptr;
    size_t *m_counter;

    template <typename D>
    friend class SharedPtr;

    void* operator new(size_t);
};


} // namespace ilrd

#endif // ILRD_94_SharedPtr_HPP
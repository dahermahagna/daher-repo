#ifndef __PQWRAPPER_Q_impl_HPP__
#define __PQWRAPPER_Q_impl_HPP__

#include "pq_wrapper.hpp"

namespace ilrd
{
template <typename T, typename CONTAINER, typename COMPARE>
inline typename PQWrapper<T, CONTAINER, COMPARE>::const_value_ref_ty PQWrapper<T, CONTAINER, COMPARE>::front() const
{
    return PQWrapper::pqueue::top();
}

} // namespace ilrd

#endif //___PQWRAPPER_Q_impl_HPP__
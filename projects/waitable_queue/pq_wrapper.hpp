#ifndef __ILRD_RD94_PQWRAPPER_Q_HPP__
#define __ILRD_RD94_PQWRAPPER_Q_HPP__

#include <queue> //priotity_queue

#include "../utils/utils.hpp"

namespace ilrd
{

    template <typename T,
              typename CONTAINER = std::vector<T>,
              typename COMPARE = std::less<typename CONTAINER::value_type> >

    class PQWrapper : private std::priority_queue<T, CONTAINER, COMPARE>
    {
    public:
        typedef typename CONTAINER::const_reference const_value_ref_ty;
        typedef std::priority_queue<T, CONTAINER, COMPARE> pqueue;
        // generated ctor, dtor is good enough
        inline const_value_ref_ty front() const;

        using pqueue::empty;
        using pqueue::pop;
        using pqueue::push;
    };

} // namespace ilrd

#endif //__ILRD_RD94__PQWRAPPER_Q_HPP__
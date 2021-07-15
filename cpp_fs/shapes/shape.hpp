#ifndef ILRD_94_SHAPES_HPP
#define ILRD_94_SHAPES_HPP
#include <iosfwd> //size_t
#include "../utils/utils.hpp"

namespace ilrd
{

class Shape
{  
public:
    explicit Shape() noexcept;
    virtual ~Shape() noexcept;
    //  cctor, op= genrated are good enough
   virtual void Draw() const = 0;
   inline Shape& Move(size_t steps_ = 1);

protected:
   size_t m_offset;
   void PrintSpaces() const;
};

} //namespace ilrd

#endif  // ILRD_94_SHAPES_HPP
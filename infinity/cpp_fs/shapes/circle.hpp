#ifndef ILRD_94_CIRCLE_HPP
#define ILRD_94_CIRCLE_HPP

#include "shape.hpp"

namespace ilrd
{

class Circle: public Shape
{
public: 
    explicit Circle() noexcept;
    virtual ~Circle() noexcept;
    virtual void Draw() const noexcept;
};
} //namespace ilrd
#endif //ILRD_94_CIRCLE_HPP
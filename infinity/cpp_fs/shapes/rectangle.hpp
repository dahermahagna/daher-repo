#ifndef ILRD_94_RECTANGLE_HPP
#define ILRD_94_RECTANGLE_HPP

#include "shape.hpp"


namespace ilrd
{

class Rectangle: public Shape
{
public:
    explicit Rectangle() noexcept;
    virtual ~Rectangle() noexcept;
    virtual void Draw() const noexcept;

};
} //namespace ilrd

#endif //ILRD_94_RECTANGLE_HPP
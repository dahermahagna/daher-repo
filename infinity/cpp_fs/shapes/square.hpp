#ifndef ILRD_94_SQUARE_HPP
#define ILRD_94_SQUARE_HPP

#include "shape.hpp"

namespace ilrd
{

class Square: public Shape
{
public:
    explicit Square() noexcept;
    virtual ~Square() noexcept;
    virtual void Draw() const noexcept;

};
} //namespace ilrd
#endif //ILRD_94_SQUARE_HPP
#ifndef SHAPE_IMPL_HPP
#define SHAPE_IMPL_HPP

#include <iosfwd> //size_t
#include <iostream> //cout
#include "shape.hpp"

namespace ilrd
{

Shape::Shape() noexcept
:m_offset(0)
{}

Shape::~Shape() noexcept
{}

inline Shape& Shape::Move(size_t steps_)
{
   m_offset = steps_;
   return *this;
}
void Shape::PrintSpaces() const
{
   size_t spaces = m_offset;
   while (spaces--)
   {
      std::cout << ' ';
   }
}
} //namespace ilrd

#endif  // SHAPE_IMPL_HPP
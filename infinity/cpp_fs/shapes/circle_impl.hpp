#include <iostream>

#include "circle.hpp"

namespace ilrd
{
  
Circle::Circle() noexcept
{}

Circle::~Circle() noexcept
{}

void Circle::Draw() const noexcept
{
  PrintSpaces();
  std::cout << "circle" << std::endl;
}
} //namespace ilrd
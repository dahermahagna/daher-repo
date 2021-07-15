#include <iostream>

#include "rectangle.hpp"

namespace ilrd
{
Rectangle::Rectangle() noexcept
{}

Rectangle::~Rectangle() noexcept
{}

void Rectangle::Draw() const noexcept
{
  PrintSpaces();
  std::cout << "Rectangle" << std::endl;
}
} //namespace ilrd

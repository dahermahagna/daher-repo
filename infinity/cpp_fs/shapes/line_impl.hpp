#include <iostream>

#include "line.hpp"

namespace ilrd
{
Line::Line() noexcept
{}

Line::~Line() noexcept
{}

void Line::Draw() const noexcept
{
  PrintSpaces();
  std::cout << "line" << std::endl;
}
} //namespace ilrd

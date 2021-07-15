#include <iostream>

#include "square.hpp"

namespace ilrd
{
Square::Square() noexcept
{}

Square::~Square() noexcept
{}

void Square::Draw() const noexcept
{
  PrintSpaces();
  std::cout << "square" << std::endl;
}
} //namespace ilrd

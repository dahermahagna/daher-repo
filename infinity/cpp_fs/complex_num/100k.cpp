#include <iostream>

#include "complex_num_impl.hpp"
int main()
{
  ilrd::Complex x(1.0,1.0);
  ilrd::Complex y(1.0,1.1);
  for (int i = 0; 100000 > i; ++i)
  {
    x += y;
  }
  std::cout << x << std::endl;
  /* std::cout << "daher" << std::endl;
  return 0; */
}

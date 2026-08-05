#include "complex_num_impl.hpp"
#include "../utils/test.hpp"

static test_status_t TestSetGet(void);
static test_status_t TestFirstOps(void);
static test_status_t TestSecondOps(void);
static test_status_t TestThirdOps(void);

int main()
{
  RUNTEST(TestSetGet);
  RUNTEST(TestFirstOps);
  RUNTEST(TestSecondOps);
  RUNTEST(TestThirdOps);
}

static test_status_t TestSetGet()
{
  FUNC_TO_TEST(Ctor->geters->seters "==" "!=");
  ilrd::Complex cmplex(1.0, 2.0);
  ilrd::Complex cmplex2(2.0, 3.0);
  ilrd::Complex cmplex3(2.0, 3.0);
  REQUIRE(1 == cmplex.GetReal());
  REQUIRE(2 == cmplex.GetImaginary());
  cmplex.SetReal(3);
  cmplex.SetImaginary(4);
  REQUIRE(3 == cmplex.GetReal());
  REQUIRE(4 == cmplex.GetImaginary());

  REQUIRE(1 == (cmplex3 == cmplex2));
  REQUIRE(1 == (cmplex != cmplex2));

  REQUIRE(0 == (cmplex == cmplex2));
  REQUIRE(0 == (cmplex3 != cmplex2));

  return PASSED;
}

static test_status_t TestFirstOps(void)
{
  FUNC_TO_TEST(Ctor-> "+"  "-"  "+=" "-=");
  ilrd::Complex cmplex(1.0, 2.0);
  ilrd::Complex cmplex2(2.0, 3.0);
  ilrd::Complex sum(0);
  REQUIRE(0 == sum.GetReal());
  REQUIRE(0 == sum.GetImaginary());
  sum = cmplex + cmplex2;

  REQUIRE(3 == sum.GetReal());
  REQUIRE(5 == sum.GetImaginary());
  cmplex = sum - cmplex;
  REQUIRE(1 == (cmplex == cmplex2));
  REQUIRE(1 == (cmplex != sum));

  sum += cmplex2;
  REQUIRE(5 == sum.GetReal());
  REQUIRE(8 == sum.GetImaginary());

  sum-= cmplex2;
  REQUIRE(3 == sum.GetReal());
  REQUIRE(5 == sum.GetImaginary());

  return PASSED;
}

static test_status_t TestSecondOps(void)
{
  FUNC_TO_TEST(Ctor-> "*"  "/"  "*=" "!=");
  ilrd::Complex cmplex(1.0, 2.0);
  ilrd::Complex cmplex2(2.0, 3.0);
  ilrd::Complex mult(0);

  REQUIRE(0 == mult.GetReal());
  REQUIRE(0 == mult.GetImaginary());
  mult = cmplex * cmplex2;

  REQUIRE(-4 == mult.GetReal());
  REQUIRE(7 == mult.GetImaginary());
  mult.SetReal(2);
  mult.SetImaginary(16);
  cmplex = mult / cmplex2;
  REQUIRE(4 == cmplex.GetReal());
  REQUIRE(2 == cmplex.GetImaginary());

  cmplex *= cmplex2;
  REQUIRE(2 == cmplex.GetReal());
  REQUIRE(16 == cmplex.GetImaginary());

  cmplex /= cmplex2;
  REQUIRE(4 == cmplex.GetReal());
  REQUIRE(2 == cmplex.GetImaginary());

  return PASSED;
}

static test_status_t TestThirdOps()
{
  FUNC_TO_TEST(Ctor-> "std::cout<<"  "std::cin >>");
  ilrd::Complex cmplex(1.0, 2.0);
  std::cout << cmplex << std::endl;
  std::cin >> cmplex;
  std::cout << cmplex << std::endl;

  return PASSED;
}


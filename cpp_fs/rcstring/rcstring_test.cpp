#include <iostream>
#include "rcstring.hpp"
#include "../utils/test.hpp"
#include <cstring>

using ilrd::RCString;

static test_status_t FirstCaseTest(void);
static test_status_t SecondCaseTest(void);
static test_status_t ThirdCaseTest(void);
static test_status_t ForthCaseTest(void);
static test_status_t FifthCaseTest(void);

int main()
{
  RUNTEST(FirstCaseTest);
  RUNTEST(SecondCaseTest);
  RUNTEST(ThirdCaseTest);
  RUNTEST(ForthCaseTest);
  RUNTEST(FifthCaseTest);
  return 0;
}

static test_status_t FirstCaseTest(void)
{
  // Length, Cstr, ctor, cCtor, assign
  FUNC_TO_TEST(ctor -> IsShared -> assingment operator);
  RCString first("hello");

  REQUIRE(5 == first.Length());
  REQUIRE(0 == strcmp("hello", first.ToCStr()));

  RCString second("second");
  REQUIRE(0 == first.IsShared());
  REQUIRE(0 == second.IsShared());
  first = second;

  REQUIRE(0 == strcmp("second", first.ToCStr()));
  REQUIRE(1 == first.IsShared());
  REQUIRE(1 == second.IsShared());


  return PASSED;
}

static test_status_t SecondCaseTest(void)
{
  FUNC_TO_TEST(ctors->Isshared->cin / cout  ->Isshared );
  RCString first("hello");
  RCString second("second");
  first = second;
  RCString third(first);

  REQUIRE(1 == first.IsShared());
  REQUIRE(1 == second.IsShared());
  REQUIRE(1 == third.IsShared());

  std::cin >> first;
  std::cout << first << std::endl;
  third = "daher";

  REQUIRE(0 == first.IsShared());
  REQUIRE(0 == second.IsShared());
  REQUIRE(0 == third.IsShared());
  return PASSED;
}

static test_status_t ThirdCaseTest(void)
{
  FUNC_TO_TEST(ctors->concat-> operator +);
  RCString first("hello");
  RCString second("second");
  RCString third("third");
  RCString forth("forth");
  RCString fifth("fifth");

  first.Concat(second);

  third = forth + first;
  forth = second + fifth;
  REQUIRE(0 == strcmp("hellosecond", first.ToCStr()));
  REQUIRE(0 == strcmp("second", second.ToCStr()));
  REQUIRE(0 == strcmp("forthhellosecond", third.ToCStr()));
  REQUIRE(0 == strcmp("secondfifth", forth.ToCStr()));
  REQUIRE(0 == strcmp("fifth", fifth.ToCStr()));

  return PASSED;
}

static test_status_t ForthCaseTest(void)
{
  FUNC_TO_TEST(ctors-> []);
  const RCString first("hello");
  RCString second(first);

  REQUIRE(1 == first.IsShared());
  REQUIRE(1 == second.IsShared());

  REQUIRE('o' == first[4]);

  REQUIRE(1 == first.IsShared());
  REQUIRE(1 == second.IsShared());

  try
  {
    char &r = second[4];
    r = 'f';
    REQUIRE(0 == first.IsShared());
    REQUIRE(0 == second.IsShared());
    REQUIRE(0 == strcmp("hellf", second.ToCStr()));
  }
  catch(const std::out_of_range& e)
  {
    std::cerr << e.what() << '\n';
  }

  return PASSED;
}

static test_status_t FifthCaseTest(void)
{
  // ==, >, <, << , >>
  FUNC_TO_TEST(ctors-> < / > / ==);
  RCString first("hello");
  RCString second("hello");
  RCString third("bla");

  REQUIRE(first == second);
  REQUIRE(third < first);
  REQUIRE((first > third));

  return PASSED;
}

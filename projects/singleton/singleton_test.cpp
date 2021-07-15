#include <boost/thread.hpp>  //thread

#include "singleton_impl.hpp"
#include "../utils/test.hpp"

using namespace ilrd;

static test_status_t TestGetInstance(void);
static test_status_t TestGetInstanceThread(void);
static void ThreadInstance(void);


class Test
{
public:
  Test():m_int(10) {}
  int Get() { return m_int;}

  private:
  int m_int;
};

boost::atomic<size_t> g_counter(0);
Test *g_instance = NULL;

int main()
{
  RUNTEST(TestGetInstance);
  RUNTEST(TestGetInstanceThread);

  return 0;
}

static test_status_t TestGetInstance(void)
{
  Test *myTest = Singleton<Test>::GetInstance();
  Test *myTest2 = Singleton<Test>::GetInstance();

  REQUIRE(10 == myTest2->Get());
  REQUIRE(10 == myTest->Get());
  REQUIRE(myTest2 == myTest);

  return PASSED;
}

static test_status_t TestGetInstanceThread(void)
{
  g_instance = Singleton<Test>::GetInstance();
  for (int i = 0; i < 10; ++i)
  {
    boost::thread instance(ThreadInstance);
    instance.join();
  }

  REQUIRE(10 == g_counter);

  return PASSED;
}

static void ThreadInstance(void)
{
  Test *tmp_instance = Singleton<Test>::GetInstance();
  if (tmp_instance == g_instance)
  {
    ++g_counter;
  }
}
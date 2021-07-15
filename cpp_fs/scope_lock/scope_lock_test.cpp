#include <iostream>
#include <pthread.h>
#include <thread>
#include <boost/thread/mutex.hpp>
#include "scope_lock.hpp"

int counter = 0;
int counter1 = 0;
int counter2 = 0;
pthread_mutex_t lock;
boost::mutex lock1;

void *IncreasCounter(void* data)
{
  for (size_t i = 0; 100000 > i; ++i)
  {

    ilrd::ScopeLock<pthread_mutex_t> a(lock);
    ++counter;
  }
  (void) data;
  return NULL;
}
void IncreasCounter1()
{
  for (size_t i = 0; 100000 > i; ++i)
  {
    ilrd::ScopeLock<boost::mutex> a(lock1);
    ++counter1;
  }

}
void IncreasCounter2()
{
  for (size_t i = 0; 100000 > i; ++i)
  {
    ++counter2;
  }
}

int main()
{
  pthread_t t1;
  pthread_t t2;
  pthread_create(&t1,NULL,IncreasCounter,NULL);
  pthread_create(&t2, NULL, IncreasCounter, NULL);
  pthread_join(t1,NULL);
  pthread_join(t2, NULL);

  std::thread th1(IncreasCounter1);
  std::thread th2(IncreasCounter1);
  th1.join();
  th2.join();

  std::thread th3(IncreasCounter2);
  std::thread th4(IncreasCounter2);
  th3.join();
  th4.join();

  std::cout << "conter increased with pthread mutex: " << counter << std::endl;
  std::cout << "conter increased with boost mutex: " << counter1 << std::endl;
  std::cout << "conter increased without mutex: " << counter2 << std::endl;

  if(counter == counter1 && counter1 > counter2)
  {
    std::cout << "mutex lock succeeded" <<std::endl;
  }
  return 0;
}


#include <boost/core/noncopyable.hpp> //noncopyable
#include <boost/chrono/chrono.hpp> // chrono seconds
#include <boost/thread.hpp>       //mutex
#include <vector>

#include <iostream>

template <typename T>
class Waitable_FSQ : private boost::noncopyable
{
public:
  explicit Waitable_FSQ(size_t size_ = 10);
  //~Waitable_FSQ();
  void Push(const T& data_);
  bool Push(const T& data_, boost::chrono::seconds timeout_);
  void Pop(T* data_);
  bool Pop(T *data_, boost::chrono::seconds timeout_);

private:
  std::vector<T> m_fsq;
  size_t m_size;
  size_t m_write;
  size_t m_read;
  boost::mutex m_write_mutex;
  boost::mutex m_read_mutex;
  boost::condition_variable m_cv_to_read;
  boost::condition_variable m_cv_to_write;

  bool IsEmpty();
  bool IsFull();

};

template <typename T>
Waitable_FSQ<T>::Waitable_FSQ(size_t size_)
:m_fsq(size_)
,m_size(size_)
,m_write(0)
,m_read(0)
{
}
template <typename T>
void Waitable_FSQ<T>::Push(const T& data_)
{
  boost::unique_lock<boost::mutex> lock(m_write_mutex);
  while(true == IsFull())
  {
    m_cv_to_write.wait(lock);
  }
  m_fsq[m_write] = data_;
  m_write = (m_write + 1) % m_size;
  m_cv_to_read.notify_one();
}

template <typename T>
bool Waitable_FSQ<T>::Push(const T &data_, boost::chrono::seconds timeout_)
{
  boost::unique_lock<boost::mutex> lock(m_write_mutex);
  while(true == IsFull())
  {
    if (m_cv_to_write.wait_for(lock, timeout_) == boost::cv_status::timeout) // TODO
    {
      return false;
    }
  }
  m_fsq[m_write] = data_;
  m_write = (m_write + 1) % m_size;
  m_cv_to_read.notify_one();

  return true;
}

template <typename T>
void Waitable_FSQ<T>::Pop(T *data_)
{
  boost::unique_lock<boost::mutex> lock(m_read_mutex);
  while (true == IsEmpty())
  {
    m_cv_to_read.wait(lock);
  }
  *data_ = m_fsq[m_read];
  m_read = (m_read + 1) % m_size;
  m_cv_to_write.notify_one();
}

template <typename T>
bool Waitable_FSQ<T>::Pop(T *data_, boost::chrono::seconds timeout_)
{
  boost::unique_lock<boost::mutex> lock(m_read_mutex);
  while (true == IsEmpty())
  {
    if (m_cv_to_read.wait_for(lock, timeout_) == boost::cv_status::timeout) // TODO
    {
      return false;
    }
  }
  *data_ = m_fsq[m_read];
  m_read = (m_read + 1) % m_size;
  m_cv_to_write.notify_one();

  return true;
}

template <typename T>
bool Waitable_FSQ<T>::IsEmpty()
{
  return m_read == m_write;
}

template <typename T>
bool Waitable_FSQ<T>::IsFull()
{
  return m_read == (m_write + 1) % m_size;
}

int main()
{
  Waitable_FSQ<int> fsq1(3);
  fsq1.Push(5);
  fsq1.Push(6);
  std::cout << fsq1.Push(7,boost::chrono::seconds(3));
  sleep(3);
  int return_Value;
  fsq1.Pop(&return_Value);

}
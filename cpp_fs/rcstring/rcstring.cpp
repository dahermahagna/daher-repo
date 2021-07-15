#include <cstring>  // strlen
#include <cassert>  // assert
#include <iomanip>  //setw
#include <iostream> //os_ << , is_ >>
#include <exception> //exception
#include "rcstring.hpp"


namespace ilrd
{
  static const size_t g_buffer_size = 1024;
  RCString::SharedRef *RCString::SharedInit(const char *str) 
  {
    assert(NULL != str);
    size_t str_size = strlen(str) + NULL_BYTE;      
    SharedRef *data = reinterpret_cast<SharedRef *>(new char[sizeof(size_t) + str_size]);
    memcpy(data->m_ref_str, str, str_size);
    data->m_count = 1;
    return data;
  }

  RCString::SharedRef *RCString::ConcatTwoRcstings(const RCString& other_)
  {
    size_t str_size = strlen(m_data->m_ref_str);
    size_t other_str_size = other_.Length() + NULL_BYTE;

    void *data_ptr = new char[sizeof(size_t) + str_size + other_str_size];

    SharedRef *data = reinterpret_cast<SharedRef *>(data_ptr);
    memcpy(data->m_ref_str, m_data->m_ref_str, str_size);
    memcpy(data->m_ref_str + Length(), other_.ToCStr(), other_str_size);
    data->m_count = 1;
    return data;
  }

  RCString::RCString(const char *str_)
      : m_data(SharedInit(str_)) 
  {
    //empty
  }

  RCString::RCString(const RCString& other_)
  :m_data(other_.m_data)
  {
    ++m_data->m_count;
  }

  RCString &RCString::operator=(const RCString &other_)
  {
    SharedRef *buffer_data = this->m_data;
    this->m_data = other_.m_data;
    ++this->m_data->m_count;

    --buffer_data->m_count;
    if (0 == buffer_data->m_count)
    {
      delete[] buffer_data;
    }

    return *this;

  }

  RCString::~RCString()
  {
    if(!IsShared())
    {
      delete[] m_data;
    }
    else
    {
      --m_data->m_count;
    }
    
    m_data = NULL;  
  }

  char &RCString::operator[](size_t pos_)
  {
    if(pos_ >= Length())
    {
      throw(std::out_of_range("The requested index is out of range"));
    }
    if(IsShared())
    {
      --m_data->m_count;
      const char *str = ToCStr();
      m_data = SharedInit(str);
    }
    return m_data->m_ref_str[pos_];
  }

  char RCString::operator[](size_t pos_) const
  {
    if (pos_ >= Length())
    {
      throw(std::out_of_range("The requested index is out of range"));
    }
    return m_data->m_ref_str[pos_];
  }

  RCString &RCString::Concat(const RCString &other_)
  {
    SharedRef *data = ConcatTwoRcstings(other_);

    if(!IsShared())
    {
      delete[] m_data;
    }
    else
    {
      --m_data->m_count;
    }
    m_data = data;
    return *this;
  }
  
  const char *RCString::ToCStr() const
  {
    return m_data->m_ref_str;
  }

  std::ostream &operator<<(std::ostream &os_, const RCString &str_)
  {
    return os_ << str_.ToCStr();
  }
  std::istream &operator>>(std::istream &is_, RCString &rhs_str_)
  {
    char buffer[g_buffer_size] = {0};
    is_ >> std::setw(g_buffer_size) >> buffer;
    
    rhs_str_ = buffer;

    return is_;
  }

  bool RCString::IsShared() const
  {
    return 1 < m_data->m_count;
  }

  size_t RCString::Length() const
  {
    return strlen(ToCStr());
  }
  bool operator==(const RCString &lhs_rcstr_, const RCString &rhs_str_)
  {
    return strcmp(lhs_rcstr_.ToCStr(), rhs_str_.ToCStr()) == 0;
  }

  bool operator>(const RCString &lhs_rcstr_, const RCString &rhs_str_)
  {
    return strcmp(lhs_rcstr_.ToCStr(), rhs_str_.ToCStr()) > 0;
  }

  bool operator<(const RCString &lhs_rcstr_, const RCString &rhs_str_)
  {
    return strcmp(lhs_rcstr_.ToCStr(), rhs_str_.ToCStr()) < 0;
  }

  RCString operator+(const RCString &lhs_rcstr_, const RCString &rhs_str_)
  {
    RCString result = RCString(lhs_rcstr_);
    result.Concat(rhs_str_);
    return result;
  }
  
}//namespace ilrd
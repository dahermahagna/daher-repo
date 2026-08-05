#ifndef BIT_ARRAY_IMPL_HPP
#define BIT_ARRAY_IMPL_HPP

#include "bit_array.hpp"
#include <stdexcept> //out of rang exception
#include <algorithm> //transform
#include <numeric> //accumulate


namespace ilrd //BitArray
{
static const size_t WORD_BIT_SIZE = (sizeof(size_t) * __CHAR_BIT__);
static const size_t BYTE_BITS = __CHAR_BIT__;
static const size_t ALL_ONE = 0xffffffffffffffff;
static const size_t ALL_NIBBLE_ON = 0Xf;
static const size_t ALL_BYTE_ON = 0Xff;


inline size_t GetIndex(size_t pos);
inline size_t GetShiftNum(size_t pos);
size_t CountIdx(size_t init, size_t idx);


template <size_t N>
BitArray<N>::BitArray()
: m_bitarray(std::vector<size_t>(GetSize()))
{}

template <size_t N>
bool BitArray<N>::operator[](size_t pos_) const
{
  CheckPos(pos_);
  return (1 == m_bitarray[GetIndex(pos_)] >> GetShiftNum(pos_) & 1LU);
}

template <size_t N>
typename BitArray<N>::BitProxy BitArray<N>::operator[](size_t pos_)
{
  CheckPos(pos_);

  return BitProxy(*this, pos_);
  
}

template <size_t N>
typename BitArray<N>::BitProxy BitArray<N>::Get(size_t pos_)
{
  CheckPos(pos_);

  return BitProxy(*this, pos_);

}

template <size_t N>  
BitArray<N> &BitArray<N>::operator&=(const BitArray<N> &other_) noexcept
{
  std::transform(other_.m_bitarray.begin(), other_.m_bitarray.end(), m_bitarray.begin(), m_bitarray.begin(), std::bit_and<size_t>());
  return *this;
}


template <size_t N>
BitArray<N> &BitArray<N>::operator|=(const BitArray<N> &other_) noexcept
{
  std::transform(other_.m_bitarray.begin(), other_.m_bitarray.end(), m_bitarray.begin(), m_bitarray.begin(), std::bit_or<size_t>());
  return *this;
}


template <size_t N>
BitArray<N> &BitArray<N>::operator^=(const BitArray<N> &other_) noexcept
{
  std::transform(other_.m_bitarray.begin(), other_.m_bitarray.end(), m_bitarray.begin(), m_bitarray.begin(), std::bit_xor<size_t>());
}


template <size_t N>
bool BitArray<N>::operator==(const BitArray<N> &rhs_) const noexcept
{
  return m_bitarray == rhs_.m_bitarray; 
}

template <size_t N>
bool BitArray<N>::operator!=(const BitArray<N> &rhs_) const noexcept
{
  return m_bitarray != rhs_.m_bitarray;
}

template <size_t N>
BitArray<N> &BitArray<N>::Set() noexcept
{
  std::fill(m_bitarray.begin(), m_bitarray.end(), ALL_ONE);
  SetLastIndex();
  return *this;
}

template <size_t N>
BitArray<N> &BitArray<N>::Set(size_t pos_, bool value_)
{
  CheckPos(pos_);

  BitProxy setter(*this,pos_);
  setter = value_;
  return *this;
}
struct FlipIdx
{
  size_t operator()(size_t num_);
};
size_t FlipIdx::operator()(size_t num_)
{
  return ~num_;
}


template <size_t N>
BitArray<N> &BitArray<N>::Flip() noexcept
{
  std::transform(m_bitarray.begin(),m_bitarray.end(),m_bitarray.begin(),FlipIdx());
  SetLastIndex();
  return *this;
}

template <size_t N>
BitArray<N> &BitArray<N>::Flip(size_t pos_)
{
  CheckPos(pos_);

  BitProxy flipper(*this,pos_);
  
  this->Set(pos_,!flipper);
  return *this;

}

struct CountIndx
{
  size_t operator()(size_t init ,unsigned char byte)
  {
    static const int table[256] =
        {
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

    return table[byte] + init; 
  }
};

struct Count_Bits
{
  size_t operator()(size_t init_, size_t idx_)
  {
    size_t count = 0;
    size_t iner_init = 0;
    unsigned char *ch = (unsigned char *)&idx_;
    count = std::accumulate(ch, ch + BYTE_BITS, iner_init, CountIndx());

    return count + init_;
  }
};


template <size_t N>
size_t BitArray<N>::Count() const noexcept
{
  size_t count = 0;
  size_t init = 0;
  count = std::accumulate(m_bitarray.begin(),m_bitarray.end(),init,Count_Bits());
  return count;
}

class To_String 
{
  public:
  explicit To_String(size_t size_ = WORD_BIT_SIZE);
  std::string operator()(std::string dest, size_t num);
  private:
  size_t m_bit_array_size;
  size_t m_counter; 
};

To_String::To_String(size_t size_)
:m_bit_array_size(size_),
m_counter(0)
{}

std::string To_String::operator()(std::string dest, size_t num)
{
  size_t i = 0;

  while ((i < WORD_BIT_SIZE) && (m_counter < m_bit_array_size))
  {
    dest[m_counter++] = char((num & 1) + '0');
    num >>= 1;
    ++i;
  }

  return dest;
}

template <size_t N>
std::string BitArray<N>::ToString() const
{
  std::string str_bitarr(N,0);
  str_bitarr = std::accumulate(m_bitarray.begin(), m_bitarray.end(), str_bitarr, To_String(N));
  std::string return_string = str_bitarr.substr(0,N);
  
  return return_string;
}

template <size_t N>
void BitArray<N>::SetLastIndex()
{
  size_t mask = ALL_ONE;
  mask >>= (WORD_BIT_SIZE - GetShiftNum(N));
  m_bitarray[GetIndex(N)] &= mask;
}

template <size_t N>
size_t BitArray<N>::GetSize() const
{
  return GetIndex(N) + !!(N % WORD_BIT_SIZE);
}

template <size_t N>
void BitArray<N>::CheckPos(size_t pos_) const
{
  if (N < pos_)
  {
    throw(std::out_of_range("illegal pos"));
  }
}

inline size_t GetIndex(size_t pos)
{
  return pos / WORD_BIT_SIZE;
}
inline size_t GetShiftNum(size_t pos)
{
  return pos % WORD_BIT_SIZE;
}

}//ilrd BitArray

namespace ilrd//BitProxy
{

template <size_t N>
BitArray<N>::BitProxy::BitProxy(BitArray<N> &bit_arr_, size_t pos_)
:m_bitsarray_ref(bit_arr_),m_pos(pos_)
{}

template <size_t N>
BitArray<N>::BitProxy::~BitProxy() noexcept
{}

template <size_t N>
typename BitArray<N>::BitProxy &BitArray<N>::BitProxy::operator=(bool other_) noexcept
{
  size_t index = GetIndex(m_pos);
  size_t ShiftNum = GetShiftNum(m_pos);
  size_t mask = 1lu;
  mask <<= ShiftNum;
  size_t value = other_;
  m_bitsarray_ref.m_bitarray[index] = (m_bitsarray_ref.m_bitarray[index] & ~mask) | value << ShiftNum; 
  return *this;
}

template <size_t N>
typename BitArray<N>::BitProxy &BitArray<N>::BitProxy::operator=(const BitProxy &other_) noexcept
{
  bool val = other_;
  *this = val;
  return *this;
}

template <size_t N>
BitArray<N>::BitProxy::operator bool() const noexcept
{
  size_t shift_num = GetShiftNum(m_pos);
  size_t index_value = m_bitsarray_ref.m_bitarray[GetIndex(m_pos)];

  return (1 == (index_value >> shift_num & 1LU));
}

template <size_t N>
bool BitArray<N>::BitProxy::operator!() const noexcept
{
  size_t shift_num = GetShiftNum(m_pos);
  size_t index_value = m_bitsarray_ref.m_bitarray[GetIndex(m_pos)];

  return (0 == (index_value >> shift_num & 1LU));
}

template <size_t N>
typename BitArray<N>::BitProxy &BitArray<N>::BitProxy::Flip() noexcept
{
  m_bitsarray_ref.m_bitarray[m_pos] = !m_bitsarray_ref.m_bitarray[m_pos];
  return *this;
}

} // ilrd BitProxy

#endif //__BIT_ARRAY_IMPL_HPP__
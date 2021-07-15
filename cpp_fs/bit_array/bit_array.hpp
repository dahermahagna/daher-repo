#ifndef ILRD_94_BIT_ARRAY_HPP 
#define ILRD_94_BIT_ARRAY_HPP

#include <vector> //vector
#include <string> //string
#include <iosfwd> //size_t
#include "../utils/utils.hpp"

namespace ilrd
{

template<size_t N = sizeof(size_t) * __CHAR_BIT__>
class BitArray
{
private:
    class BitProxy;

public:
    explicit BitArray();
    // ~BitArray() noexcept;
    // BitArray(const BitArray<N>& other_) noexcept; //shallow copy OK
    // BitArray<N>& operator=(const BitArray<N>& other_) noexcept;

    bool operator[](size_t pos_) const; //throws std::out_of_range
    BitProxy operator[](size_t pos_); //throws std::out_of_range
    BitProxy Get(size_t pos_); //throws std::out_of_range

    BitArray<N>& operator&=(const BitArray<N>& other_) noexcept;
    BitArray<N>& operator|=(const BitArray<N>& other_) noexcept;
    BitArray<N>& operator^=(const BitArray<N>& other_) noexcept;

    BitArray<N>& operator<<=(size_t pos_) noexcept;
    BitArray<N>& operator>>=(size_t pos_) noexcept;

    bool operator==(const BitArray<N>& rhs_) const noexcept;
    bool operator!=(const BitArray<N>& rhs_) const noexcept;

    BitArray<N>& Set() noexcept;
    BitArray<N>& Set(size_t pos_, bool value_ = true); //throws std::out_of_range

    size_t Count() const noexcept;

    BitArray<N>& Flip() noexcept;
    BitArray<N>& Flip(size_t pos_); //throws std::out_of_range

    std::string ToString() const; //throws std::bad_alloc

private:
    class BitProxy
    {
    public:
        BitProxy(BitArray<N>& bit_arr_, size_t pos_); //used only by BitArray
        ~BitProxy() noexcept;
        BitProxy& operator=(bool other_) noexcept;
        BitProxy& operator=(const BitProxy& other_) noexcept;
        operator bool() const noexcept;
        bool operator!() const noexcept;
        BitProxy& Flip() noexcept;

    private:
        BitProxy(); //undefined on purpose
        BitArray<N>& m_bitsarray_ref;
        size_t m_pos;
    }; // BitProxy

    std::vector<size_t> m_bitarray;
    inline size_t GetSize() const;
    inline void SetLastIndex();
    inline void CheckPos(size_t pos_) const;

}; // BitArray

} // ilrd

#endif //__BIT_ARRAY_HPP__
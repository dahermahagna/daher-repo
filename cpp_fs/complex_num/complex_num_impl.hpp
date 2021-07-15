
/******************************************************************************
*				C++: Complex NUM                					      
*				code by     : Daher												  
*               reviewed by : Rotem
*				last edit   : 14/12/20	
*               V 0.1					 					  
******************************************************************************/
#ifndef ILRD_94_COMPLEX_IMPL_HPP
#define ILRD_94_COMPLEX_IMPL_HPP

#include <iosfwd>       // std:: ostream, std:: isrteam
#include <cassert>      //assert
#include <iostream>     // cin cout

#include "complex_num.hpp"

namespace ilrd
{
    ilrd::Complex::Complex(double real_, double imaginary_)
        : m_real(real_),
          m_imaginary(imaginary_)
    {
        //empty
    }

    inline Complex &Complex::operator+=(const Complex &other_)
    {
        *this = *this + other_;
        return *this; 
    }
    inline Complex &Complex::operator-=(const Complex &other_)
    {
        *this = *this - other_;
        return *this;
    }
    inline Complex &Complex::operator*=(const Complex &other_)
    {
        *this = *this * other_;
        return *this;
    }
    inline Complex &Complex::operator/=(const Complex &other_)
    {
        *this = *this / other_;
        return *this;
    }

    inline double Complex::GetReal() const
    {
        return this->m_real;
    }

    inline double Complex::GetImaginary() const
    {
        return this->m_imaginary;
    }

    inline void Complex::SetReal(double val)
    {
        this->m_real = val;
    }

    inline void Complex::SetImaginary(double val)
    {
        this->m_imaginary = val;
    }

    inline std::ostream &operator<<(std::ostream &os_, const Complex &cmplx_)   // print as "a + ib": "4 + i8"
    {
        os_ << cmplx_.GetReal() << " + i" << cmplx_.GetImaginary();

        return os_;
    }

    inline std::istream &operator>>(std::istream &is, Complex &cmplx_)
    {
        double real = 0;
        double imaginary = 0;
        
        is >> real;
        is >> imaginary;

        cmplx_.SetReal(real);
        cmplx_.SetImaginary(imaginary);

        return is;
    }

    inline bool operator==(const Complex &cmplx_, const Complex &cmplx_to_compare_)
    {
        bool same_real      = cmplx_.GetReal() == cmplx_to_compare_.GetReal();
        bool same_imaginary = cmplx_.GetImaginary() == cmplx_to_compare_.GetImaginary();

        return same_real & same_imaginary;
    }

    inline bool operator!=(const Complex &cmplx_, const Complex &cmplx_to_compare_)
    {
        bool same_real      = cmplx_.GetReal() != cmplx_to_compare_.GetReal();
        bool same_imaginary = cmplx_.GetImaginary() != cmplx_to_compare_.GetImaginary();

        return same_real & same_imaginary;
    }

    inline Complex operator+(const Complex &cmplx_, const Complex &cmplx_to_compare_)
    {
        double real_sum      = (cmplx_.GetReal() + cmplx_to_compare_.GetReal());
        double imaginary_sum = (cmplx_.GetImaginary() + cmplx_to_compare_.GetImaginary());

        return Complex(real_sum, imaginary_sum);
    }

    inline Complex operator-(const Complex &cmplx_, const Complex &cmplx_to_compare_)
    {
        double real_dec      = (cmplx_.GetReal() - cmplx_to_compare_.GetReal());
        double imaginary_dec = (cmplx_.GetImaginary() - cmplx_to_compare_.GetImaginary());

        return Complex(real_dec, imaginary_dec);
    }

    inline Complex operator*(const Complex &cmplx_, const Complex &cmplx_to_compare_)
    {
        double a = cmplx_.GetReal();
        double b = cmplx_.GetImaginary();
        double c = cmplx_to_compare_.GetReal();
        double d = cmplx_to_compare_.GetImaginary();

        return Complex(a * c - b * d, a * d + b * c);
    }

    inline Complex operator/(const Complex &cmplx_, const Complex &cmplx_to_compare_)
    {
        
        double c = cmplx_to_compare_.GetReal();
        double d = cmplx_to_compare_.GetImaginary();
        if(0 == c && 0 == d)
        {
            std::cout << "math error" <<std::endl;
            return Complex(0,0);
        }
        double a = cmplx_.GetReal();
        double b = cmplx_.GetImaginary();
        double denominator = (1 / ((c * c) + (d * d)));

        return Complex(denominator * ((a * c) + (d * b)), denominator * ((b * c) - (a * d)));
    }

} //namespace ilrd


#endif // ILRD_94_COMPLEX_IMPL_HPP

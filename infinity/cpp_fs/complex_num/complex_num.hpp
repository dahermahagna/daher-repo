
/******************************************************************************
*				C++: Complex Class                					      
*				code by: RD-94												  
* 
*				last edit: 13/12/20	
*               V 0.1					 					  
******************************************************************************/
#ifndef ILRD_94_COMPLEX_HPP
#define ILRD_94_COMPLEX_HPP

#include <iosfwd> // std:: ostream, std:: isrteam


namespace ilrd
{

class Complex
{
public:
    Complex(double real_, double imaginary_ = 0);    // non explicit
    
    inline Complex& operator+=(const Complex& other_);
    inline Complex& operator-=(const Complex& other_);
    inline Complex& operator*=(const Complex& other_);
    inline Complex& operator/=(const Complex& other_);
    
    inline double GetReal() const;          
    inline double GetImaginary() const;         

    inline void SetReal(double val);
    inline void SetImaginary(double val);

private:
    double m_real;
    double m_imaginary;
};

inline std::ostream& operator<<(std::ostream& os_, const Complex& cmplx_);   // print as "a + ib": "4 + i8"
inline std::istream& operator>>(std::istream& is, Complex& cmplx_);

inline bool operator==(const Complex& cmplx_, const Complex& cmplx_to_compare_);
inline bool operator!=(const Complex& cmplx_, const Complex& cmplx_to_compare_);

inline Complex operator+(const Complex& cmplx_, const Complex& cmplx_to_compare_);
inline Complex operator-(const Complex& cmplx_, const Complex& cmplx_to_compare_);
inline Complex operator*(const Complex& cmplx_, const Complex& cmplx_to_compare_);
inline Complex operator/(const Complex& cmplx_, const Complex& cmplx_to_compare_);
} //namespace ilrd


#endif  // ILRD_94_COMPLEX_HPP
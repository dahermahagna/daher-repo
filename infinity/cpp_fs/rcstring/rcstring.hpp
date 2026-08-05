/******************************************************************************
*				C++: String Resource counting String                					      
*				code by: 												  
* 				review by: 
*               test by: 
*				last edit: 18/12/20						 					  
******************************************************************************/
#ifndef ILRD_94_RCSTRING_HPP
#define ILRD_94_RCSTRING_HPP

#include <iosfwd> // size_t

#if cplusplus >= 201103L 
static_assert(1 == sizeof(char) , "Only works if 1==sizeof(char)");
#else
namespace{ typedef int Stupid[ 1 / (1==sizeof(char)) ]; }// compilation error if  1!=sizeof(char), otherwise does nothing
#define noexcept throw()
#endif // #if cplusplus >= 201103L 

namespace ilrd
{

class RCString
{
public:
    RCString(const char *str_ = "");  // non explicit on purpose, throws bad_alloc
    RCString(const RCString& other_); // throws bad_alloc ??
    RCString& operator=(const RCString& other_); // throws bad_alloc
    ~RCString();

    char& operator[](size_t pos_);
    char operator[](size_t pos_) const;
    
    RCString& Concat(const RCString& other_);   // throws bad_alloc
    const char* ToCStr() const;  
    
    bool IsShared() const;
    size_t Length() const;


private:
    static const size_t NULL_BYTE = 1;
    struct SharedRef;
    SharedRef *SharedInit(const char *str);
    SharedRef *ConcatTwoRcstings(const RCString &other_);
    struct SharedRef
    {
        size_t m_count; 
        char m_ref_str[1];   
         
    };//end SharedRef

    SharedRef *m_data;
};

std::ostream& operator<<(std::ostream& os_, const RCString& rhs_rcstr_);
std::istream& operator>>(std::istream& is_, RCString& rhs_str_);
bool operator==(const RCString& lhs_rcstr_, const RCString& rhs_str_);
bool operator>(const RCString& lhs_rcstr_, const RCString& rhs_str_);
bool operator<(const RCString& lhs_rcstr_, const RCString& rhs_str_);
RCString operator+(const RCString& lhs_rcstr_, const RCString& rhs_str_);

} //namespace ilrd

#endif  // ILRD_94_RCSTRING_HPP



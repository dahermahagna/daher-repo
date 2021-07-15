
/******************************************************************************
*				C++: String Class                					      
*				code by: RD-94												  
* 
*				last edit: 9/12/20	
*               V 0.3					 					  
******************************************************************************/

#ifndef __ILRD_RD94_STRING_HPP__
#define __ILRD_RD94_STRING_HPP__

#include <iosfwd>  // size_t

namespace ilrd
{

class String
{
public:
    String(const char *str_);       // non explicit on purpose
    String(const String& other_); 
    ~String(); 
    String& operator=(const String& other_);
    
    size_t Length() const;          // length of string
    const char* CStr() const;       // get raw string  

private:
    char *StringDup(const char *str);
    static const size_t NULL_CHAR = 1; 
    char *m_cstr;
};

std::ostream& operator<<(std::ostream& os_, const String& str_);
std::istream& operator>>(std::istream& is_, String& str_);
bool operator==(const String& str_, const String& str_to_compare_);
bool operator>(const String& str_, const String& str_to_compare_);
bool operator<(const String& str_, const String& str_to_compare_);

} //namespace ilrd

#endif  //__ILRD_RD94_STRING_HPP__








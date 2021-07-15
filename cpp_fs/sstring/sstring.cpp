#include <cstring>      //memcpy
#include <cassert>      //assert
#include <iomanip>      //setw
#include "sstring.hpp"

namespace ilrd
{
static const size_t g_buffer_size = 1024; 
char *String::StringDup(const char *str)
{
    assert(NULL != str);

    size_t str_size = strlen(str) + NULL_CHAR; 
    char *new_str = new char[str_size];
    memcpy(new_str,str,str_size);

    return new_str;
}
String::String(const char *str_) : m_cstr(StringDup(str_))
{
    //empty    
}
String::String(const String &other_) : m_cstr(StringDup(other_.CStr()))
{
    //empty
}

String& String::operator=(const String& other_)
{
    char* new_str = StringDup(other_.CStr());
    delete[] m_cstr;
    m_cstr = new_str;

    return *this;
}

size_t String::Length() const
{
    return strlen(m_cstr);
}

const char* String::CStr() const
{
    return const_cast<const char *> (m_cstr);
}

String::~String()
{
    delete[] String::CStr();
}

std::ostream& operator<<(std::ostream& os_, const String& str_)
{
    return os_ << str_.CStr();
}

std::istream& operator>>(std::istream &is_, String& str_)
{
    char buffer[g_buffer_size] = {0};
    is_ >> std::setw(g_buffer_size) >> buffer; // to prevent buffer overflaw
    str_ = buffer; 
       
    return is_;
}
bool operator==(const String &str_, const String &str_to_compare_)
{
    return strcmp(str_.CStr(),str_to_compare_.CStr()) == 0;
}

bool operator>(const String &str_, const String &str_to_compare_)
{
    return strcmp(str_.CStr(), str_to_compare_.CStr()) > 0;
}

bool operator<(const String &str_, const String &str_to_compare_)
{
    return strcmp(str_.CStr(), str_to_compare_.CStr()) < 0;
}
}
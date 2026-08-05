#include <iostream>
#include <optional>

using namespace std;
class optional_cpp
{

public:
    optional_cpp(/* args */){};
    ~optional_cpp(){};
    void print_optional_cpp(){std::cout << "optional_cpp "<< std::endl;};
    std::optional<int> a;
    std::optional<int> b;
    void operator= (const optional_cpp& other)
    {
        // if(this == &other)
        // {
        //     return;
        // }

        // if (other.a.has_value())
            a = other.a;
        // else
        //     a.reset();

        // if (other.b.has_value())
            b = other.b;
        // else
        //     b.reset();
    }
    bool operator== (const optional_cpp& other)
    {
        return (a == other.a && b == other.b);
    }

};

int main()
{
    optional_cpp t1;
    optional_cpp t2;

    t1.a = 10;
    t1.b = 20;

    t2.a = 10;

    if( t1 == t2 )
    {
        std::cout << "t1 and t2 are equal" << std::endl;
    }
    else
    {
        t1 = t1; // Copy assignment operator
        std::cout << "t1 and t2 are not equal" << std::endl;
    }
    if( t1 == t2 )
    {
        std::cout << "t1 and t2 are now equal" << std::endl;
    }
    else
    {
        std::cout << "t1 and t2 are still not equal" << std::endl;
    }
    std::cout << "a: " << (t1.a.has_value() ? t1.a.value(): -1) << " b: " << (t1.b.has_value() ? t1.b.value():-1) << std::endl;
    // t.print_optional_cpp();

    optional<int> a , b;
    a = 5;
    b = a;
    a.reset();

    std::cout << "a: " << (a.has_value() ? a.value(): -1) << " b: " << (b.has_value() ? b.value():-1) << std::endl;
    
    b = a;
    std::cout << "a: " << (a.has_value() ? a.value(): -1) << " b: " << (b.has_value() ? b.value():-1) << std::endl;

    optional<bool> c;
    if(c)
    {
        std::cout << "c has value" << std::endl;
    }
    else
    {
        std::cout << "c does not have value" << std::endl;
    }
    std::cout << "c: " << (c.has_value() ? c.value() : false) << std::endl;
    c = true;
    if(c)
    {
        std::cout << "c has value" << std::endl;
    }
    else
    {
        std::cout << "c does not have value" << std::endl;
    }
    std::cout << "c: " << (c.has_value() ? c.value() : false) << std::endl;
    c.reset();
    std::cout << "c: " << (c.has_value() ? c.value() : false) << std::endl;

    std::optional<std::pair<bool, bool>> p1;

    p1.emplace(true, true);
    std::cout << "p1: " << (p1.has_value() && p1->second ?"print":"no print") << std::endl;
    p1.value().second = false;
    std::cout << "p1: " << (p1.has_value() && p1->second ?"print":"no print") << std::endl;
    return 0;
}

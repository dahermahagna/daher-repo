#include <iostream>

class enum_class
{
private:
    /* data */
public:
    enum_class(/* args */){};
    ~enum_class(){};
    void print_enum_class(){std::cout << "enum_class "<< std::endl;};

};
struct A
{
    int a;
    int b;
    // A() : a(0), b(0) {std::cout << "Default constructor called" << std::endl;} // Default constructor
    // A(int x, int y) : a(x), b(y) {std::cout << "Parameterized called" << std::endl;} // Parameterized
    // A(const A& other) : a(other.a), b(other.b) {std::cout << "Copy constructor called" << std::endl; } // Copy constructor
    // A& operator=(const A& other) // Copy assignment operator
    // {
    //     std::cout << "Copy assignment operator called" << std::endl;
    //     if (this != &other) // Check for self-assignment
    //     {
    //         a = other.a;
    //         b = other.b;
    //     } 
    //     return *this; // Return the current object
    // }

};


int main()
{
    enum_class t;
    t.print_enum_class();

    A a1 = {1, 2};
    A a2;
    a2 = a1; // Copy assignment operator
    A a3(a1); // Copy constructor
    std::cout << "a2.a: " << a2.a << ", a2.b: " << a2.b << std::endl;
    return 0;
}

#include <iostream>

class enum_in_class_args
{
private:
    /* data */
public:
    enum_in_class_args(/* args */){};
    ~enum_in_class_args(){};
    void print_enum_in_class_args(){std::cout << "enum_in_class_args "<< std::endl;};

};

int main()
{
    enum_in_class_args t;
    t.print_enum_in_class_args();
    return 0;
}

#include <iostream>

class first_try
{
private:
    /* data */
public:
    first_try(/* args */){};
    ~first_try(){};
    void print_first_try(){std::cout << "first_try "<< std::endl;};

};

int main()
{
    first_try t;
    t.print_first_try();
    return 0;
}

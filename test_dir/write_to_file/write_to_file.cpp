#include <iostream>
#include <fstream>
#include <string>

class write_to_file
{
private:
    /* data */
public:
    write_to_file(/* args */){};
    ~write_to_file(){};
    void print_write_to_file(){std::cout << "write_to_file "<< std::endl;};

};

auto foo()
{
    int a;

    { // block substituent for temporary variables

    // Tr{a}
    int& tr_xr = a;

    // T{24}
    int t_x = 24;

    // = (asignement)
    tr_xr = t_x;
    }

    return a; // 24
}

int main()
{
    write_to_file t;
    t.print_write_to_file();
    std::ofstream file;
    file.open("example.txt");
    file << "Hello, World!" << std::endl;
    file.close();

    std::cout << foo() << std::endl;
    
    return 0;
}

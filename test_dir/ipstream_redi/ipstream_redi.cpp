#include <iostream>
#include <fstream>
#include <string>
// #include <unique_ptr.h>
#include <memory>
#include <pstreams/pstream.h>
#include <sstream>

class ipstream_redi
{
private:
    /* data */
public:
    ipstream_redi(/* args */){};
    ~ipstream_redi(){};
    void print_ipstream_redi(){std::cout << "ipstream_redi "<< std::endl;};

};

int main()
{
    ipstream_redi t;
    std::unique_ptr<redi::pstream> ipstream = std::make_unique<redi::pstream>("ls -l");
    std::string line;
    // while (std::getline(*ipstream, line))
    // {
        // std::getline(*ipstream, line);
        // std::cout << line << std::endl;
    // }
    std::stringstream ss;
    ss << ipstream->rdbuf();
    *ipstream << redi::peof;
    std::string s = ss.str();
    std::cout << s << std::endl;
    const unsigned int n = 10;
    // std::cout << ipstream->rdbuf()->.str() << std::endl;

    s += (std::string("\n[DAHER]\n") + "ipstream_redi::print_ipstream_redi() called\n" + "[DAHER]\n")
        + "ipstream_redi::print_ipstream_redi() called\n"
        + "ipstream_redi::print_ipstream_redi() called\n"
        + "ipstream_redi::print_ipstream_redi() called\n"
        + std::to_string(n) + " times\n";   

    std::cout << s << std::endl;
    return 0;
}

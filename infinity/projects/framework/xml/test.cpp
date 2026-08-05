#include <iostream>
#include "xml.hpp"
using namespace ilrd;
int main()
{
    Xml xml1("configurations.xml");
    std::cout << xml1.GetNumOfThreads() << std::endl;
    std::cout << xml1.GetLoggerPath() << std::endl;
    std::cout << xml1.GetNiceness() << std::endl;
    std::cout << xml1.GetPolicy() << std::endl;
    std::cout << xml1.GetSharedFolderPath() << std::endl;
}
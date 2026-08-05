#include <iostream>
#include "singleton_impl.hpp"

using namespace std;
using namespace ilrd;
extern "C"
{
    void Put5()
    {
        int *single_int = Singleton<int>::GetInstance();

        *single_int = 5;

        cout << "singleton in exp.cpp is: " << *single_int << endl;
    }
}

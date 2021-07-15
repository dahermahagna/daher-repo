#include <iostream>
#include <dlfcn.h>
#include "singleton_impl.hpp"

using namespace std;
using namespace ilrd;

int main()
{
    int *int_ptr = Singleton<int>::GetInstance();

    *int_ptr = 100;

    void* handle = dlopen("./libexp.so", RTLD_LAZY);

      if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    typedef void (*exp_t)();
    exp_t put5_fun = (exp_t) dlsym(handle, "Put5");
    const char *dlsym_error = dlerror();

    if (dlsym_error) 
    {
        cerr << "Cannot load symbol 'hello': " << dlerror() << '\n';
        dlclose(handle);
        return 1;
    }

    put5_fun();

    cout << *int_ptr << endl;
    cout << "singleton instance is: " << *(Singleton<int>::GetInstance()) << endl;

    dlclose(handle);

    return 0;
}
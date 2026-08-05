#include <iostream>
#include <thread>
#include <memory>

#include <chrono>
#include <string>
class thread_cpp
{
private:
    /* data */
public:
    thread_cpp(/* args */){};
    ~thread_cpp(){};
    void print_thread_cpp(){std::cout << "thread_cpp "<< std::endl;};

};

void thread_function()
{
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Thread function is running: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void thread_function2()
{
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Thread function2 is running: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

// int function()
// {
//     int res = 0;
//     std::cout << "Thread function is running." << std::endl;
//     std::jthread([&](int) {
//         for (int i = 0; i < 5; ++i)
//         {
//             std::cout << "Thread function2 is running: " << i << std::endl;
//             std::this_thread::sleep_for(std::chrono::seconds(3));
//             if (i == 3)
//             {
//                 std::cout << "Thread function2 is doneaaaaaaaa." << std::endl;
//                 res = 6; // Exit the thread
//                 return;
//             }
//         }
//         std::cout << "Thread function2 is done." << std::endl;
//         return; // Exit the thread
//     })/* .detach() */;
//     return res;
// }

int main()
{
    thread_cpp t;
    t.print_thread_cpp();
    std::jthread t1(thread_function);
    // t1.detach();
    // int res = function();
    
    // t1.join();
    std::cout << "Thread function is done.bbbbbbb" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(15));
    std::cout << "Main thread is done." << std::endl;
    // std::cout <<"return values is: " << res << std::endl;
    return 0;
}

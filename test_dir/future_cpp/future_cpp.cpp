#include <iostream>
#include <future>
#include <stdexcept>
#include <thread>
#include <chrono>

// Simulate a long-running task that may throw an exception
int LongComputation(int x) {
    std::this_thread::sleep_for(std::chrono::seconds(10)); // Simulate work
    if (x < 0) {
        throw std::runtime_error("Negative input is not allowed");
    }
    return x * x;
}
int LongComputation2(int x) {

    for (size_t i = 0; i < 10; i++)
    {
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
        if (x < 0) {
            throw std::runtime_error("Negative input is not allowed");
        }
        std::cout << i <<": LongComputation2...\n";
    }
    return x * x;
}
void funct8()
{
    std::cout << "func8 is running...\n";
    // std::cout << "func8 resultFuture.get(): " << resultFuture.get() << "\n";
}
void funct7(const std::shared_future<int>& resultFuture)
{
    std::cout << "func7 is running...\n";
    funct8();

}
void funct6(const std::shared_future<int>& resultFuture) {
    std::cout << "func6 is running...\n";
    funct7(resultFuture);
}
void funct5(const std::shared_future<int>& resultFuture) {
    std::cout << "func5 is running...\n";
    funct6(resultFuture);
}
void funct4(const std::shared_future<int>& resultFuture)
{
    std::cout << "func4 is running...\n";
    funct5(resultFuture);
}
void funct3(const std::shared_future<int>& resultFuture)
{
    std::cout << "func3 is running...\n";
    funct4(resultFuture);

}
void funct2(const std::shared_future<int>& resultFuture) {
    std::cout << "func2 is running...\n";
    funct3(resultFuture);
}
void func1(const std::shared_future<int>& resultFuture) {
    std::cout << "func1 is running...\n";
    funct2(resultFuture);
}
std::pair<std::shared_future<int>,int> func0() {
    std::cout << "func0 is running...\n";
    std::jthread(LongComputation2, 5).detach();
    std::shared_future<int> resultFuture = std::async(std::launch::async, LongComputation, 5);

    // Do other work while the task runs
    std::cout << "Doing other work...\n";
    func1(resultFuture);

    return std::make_pair<>(resultFuture,5);
}
int main() {
    std::cout << "Main function is running...\n";
    auto resultFuture = func0();
    std::cout <<"second"  << resultFuture.second << std::endl;
    std::cout << "Main function is doing other work...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Simulate work
    std::cout << "Main function is doing other work...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Simulate work
    std::cout << "Main function is doing other work...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
    std::cout << "Main function is doing other work...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Simulate work
    std::cout << resultFuture.first.get() << std::endl;
    return 0;
}
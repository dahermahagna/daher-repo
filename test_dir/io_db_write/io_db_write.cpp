#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <atomic>
#include <chrono>
#include <memory>

std::atomic<bool> stop_flag{false};

void write_to_postgres(const std::string& data) {
    for (int i = 0; i < 100; ++i) {
        if (stop_flag.load()) {
            std::cout << "[DB Thread] Write cancelled for: " << data << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Small chunk
        std::cout << "[DB Thread] Writing: " << data << " chunk " << i << std::endl;
    }
    std::cout << "[DB Thread] Write complete: " << data << std::endl;
}


int main() {
    boost::asio::io_context io_context;
    std::unique_ptr<boost::asio::io_context::work> work = std::make_unique<boost::asio::io_context::work>(io_context); // Prevent io_context from running out of work
    std::thread db_thread([&io_context]() {
        io_context.run();
    });

    io_context.post([]() { write_to_postgres("record_1"); });

    // Simulate receiving a new update after 200ms
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    stop_flag = true;
    std::cout << "[Main Thread] Stopping io_context..." << std::endl;
    work.release(); // Allow io_context to stop
    io_context.stop();

    db_thread.join();
    std::cout << "Main thread done." << std::endl;
    return 0;

}
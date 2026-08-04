#include <iostream>
#include <thread>
#include <chrono>
#include "circle_q.hpp"

void producer(CircularQueue<int, 3>& q) {
    for (int i = 0; i < 20; ++i) {
        while (!q.enqueue(i)) {
            std::this_thread::yield(); 
        }
        std::cout << "[Producer] Enqueued: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void consumer(CircularQueue<int, 3>& q) {
    for (int i = 0; i < 20; ++i) {
        int val;
        while (!q.dequeue(val)) {
            std::this_thread::yield();
        }
        std::cout << "[Consumer] Dequeued: " << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    CircularQueue<int, 3> q;
    std::thread t1(producer, std::ref(q));
    std::thread t2(consumer, std::ref(q));
    t1.join();
    t2.join();
    std::cout << "Done!" << std::endl;
    q.printVarInfo();
    return 0;
}
#pragma once
#include <atomic>
#include <new>
#include <cstddef> // for size_t
#include <thread> // for std::this_thread::yield
constexpr size_t CacheLineSize = 64;

template<typename T, size_t Capacity>
class CircularQueue {
private:
    alignas(CacheLineSize) std::atomic<size_t> head{0};
    alignas(CacheLineSize) std::atomic<size_t> tail{0};
    // std::atomic<size_t> head{0};
    // std::atomic<size_t> tail{0};

    T buffer[Capacity];

public:
    bool enqueue(T item) {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t next_tail = (t + 1) % Capacity;
        if (next_tail == head.load(std::memory_order_acquire)) {
            return false;
        }
        buffer[t] = item;
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    bool dequeue(T& item) {
        size_t h = head.load(std::memory_order_relaxed);
        if (h == tail.load(std::memory_order_acquire)) {
            return false;
        }
        item = buffer[h];
        head.store((h + 1) % Capacity, std::memory_order_release);
        return true;
    }
    void printVarInfo() {
        std::cout << "Size of CircularQueue: " << sizeof(*this) << " bytes" << std::endl;
        std::cout << "Size of head: " << sizeof(head) << " bytes" << std::endl;
        std::cout << "Size of tail: " << sizeof(tail) << " bytes" << std::endl;
        std::cout << "Size of buffer: " << sizeof(buffer) << " bytes" << std::endl;
        std::cout << "Size of T: " << sizeof(T) << " bytes" << std::endl;
        std::cout << "Capacity: " << Capacity << std::endl;
        std::cout << "Cache line size: " << CacheLineSize << " bytes" << std::endl;
        std::cout << "Alignment of CircularQueue: " << alignof(*this) << " bytes" << std::endl;
        std::cout << "Alignment of head: " << alignof(head) << " bytes" << std::endl;
        std::cout << "Alignment of tail: " << alignof(tail) << " bytes" << std::endl;
        std::cout << "Alignment of buffer: " << alignof(buffer) << " bytes" << std::endl;
        std::cout << "Alignment of T: " << alignof(T) << " bytes" << std::endl;
        std::cout << "Address of head: " << &head << std::endl;
        std::cout << "Address of tail: " << &tail << std::endl;
        std::cout << "Address of buffer: " << &buffer << std::endl;

    }
};
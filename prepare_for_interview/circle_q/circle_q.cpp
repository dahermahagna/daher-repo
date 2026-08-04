// #include <iostream>
// #include <vector>
// #include <atomic>
// #include <new> // for hardware_destructive_interference_size
// #include <thread>
// // #include "circle_q.cpp"

// template<typename T, size_t Capacity>
// class CircularQueue {
// private:
//     // Align to 64 bytes (typical cache line size) to prevent false sharing
//     alignas(64) std::atomic<size_t> head{0};
//     alignas(64) std::atomic<size_t> tail{0};
    
//     T buffer[Capacity];

// public:
//     bool enqueue(T item) {
//         size_t t = tail.load(std::memory_order_relaxed);
//         size_t next_tail = (t + 1) % Capacity;
        
//         if (next_tail == head.load(std::memory_order_acquire)) {
//             return false; // Queue full
//         }
        
//         buffer[t] = item;
//         tail.store(next_tail, std::memory_order_release);
//         return true;
//     }

//     bool dequeue(T& item) {
//         size_t h = head.load(std::memory_order_relaxed);
        
//         if (h == tail.load(std::memory_order_acquire)) {
//             return false; // Queue empty
//         }
        
//         item = buffer[h];
//         head.store((h + 1) % Capacity, std::memory_order_release);
//         return true;
//     }
// };



// void producer(CircularQueue<int, 10>& q) {
//     for (int i = 0; i < 20; ++i) {
//         while (!q.enqueue(i)) {
//             // Wait for space
//         }
//         std::cout << "[Producer] Enqueued: " << i << std::endl;
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }
// }

// void consumer(CircularQueue<int, 10>& q) {
//     for (int i = 0; i < 20; ++i) {
//         int val;
//         while (!q.dequeue(val)) {
//             // Wait for data
//         }
//         std::cout << "[Consumer] Dequeued: " << val << std::endl;
//         std::this_thread::sleep_for(std::chrono::milliseconds(150));
//     }
// }

// // int main() {
// //     CircularQueue<int, 10> q;

// //     std::thread t1(producer, std::ref(q));
// //     std::thread t2(consumer, std::ref(q));

// //     t1.join();
// //     t2.join();

// //     std::cout << "Done!" << std::endl;
// //     return 0;
// // }
// // main() {
// //     CircularQueue<int, 10> cq;

// //     // Enqueue some items
// //     for (int i = 0; i < 10; ++i) {
// //         if (!cq.enqueue(i)) {
// //             std::cout << "Queue is full at item: " << i << "\n";
// //         }
// //     }

// //     // Dequeue and print items
// //     int item;
// //     while (cq.dequeue(item)) {
// //         std::cout << "Dequeued: " << item << "\n";
// //     }

// //     return 0;
// // }
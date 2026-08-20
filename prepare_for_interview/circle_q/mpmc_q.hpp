#include <atomic>
#include <vector>
#include <new>

template<typename T, size_t Capacity>
class MPMCQueue {
private:
    struct Cell {
        std::atomic<size_t> sequence;
        T data;
    };

    // Cache line alignment to prevent false sharing
    static constexpr size_t CacheLineSize = 64;
    
    alignas(CacheLineSize) Cell buffer[Capacity];
    alignas(CacheLineSize) std::atomic<size_t> enqueue_pos{0};
    alignas(CacheLineSize) std::atomic<size_t> dequeue_pos{0};

public:
    MPMCQueue() {
        for (size_t i = 0; i < Capacity; ++i) {
            buffer[i].sequence.store(i, std::memory_order_relaxed);
        }
    }

    bool enqueue(T const& item) {
        Cell* cell;
        size_t pos = enqueue_pos.load(std::memory_order_relaxed);
        for (;;) {
            cell = &buffer[pos % Capacity];
            size_t seq = cell->sequence.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)seq - (intptr_t)pos;

            if (diff == 0) {
                // Slot is empty, try to claim it
                if (enqueue_pos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    break;
                }
            } else if (diff < 0) {
                // Queue is full
                return false;
            } else {
                // Another producer beat us, update pos and try again
                pos = enqueue_pos.load(std::memory_order_relaxed);
            }
        }

        cell->data = item;
        // Signal that the data is ready for consumers
        cell->sequence.store(pos + 1, std::memory_order_release);
        return true;
    }

    bool dequeue(T& item) {
        Cell* cell;
        size_t pos = dequeue_pos.load(std::memory_order_relaxed);
        for (;;) {
            cell = &buffer[pos % Capacity];
            size_t seq = cell->sequence.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)seq - (intptr_t)(pos + 1);

            if (diff == 0) {
                // Slot has data, try to claim it
                if (dequeue_pos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    break;
                }
            } else if (diff < 0) {
                // Queue is empty
                return false;
            } else {
                // Another consumer beat us, update pos and try again
                pos = dequeue_pos.load(std::memory_order_relaxed);
            }
        }

        item = cell->data;
        // Reset the sequence to signal that this slot is empty for producers
        cell->sequence.store(pos + Capacity, std::memory_order_release);
        return true;
    }
};
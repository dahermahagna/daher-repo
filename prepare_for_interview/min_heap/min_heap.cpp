#include <iostream> // for std::cout, std::endl
#include <vector> // for std::vector
#include <stdexcept> // for std::runtime_error
#include <utility> // for std::swap
#include <cmath> // for log2 and pow
#include <iomanip> // for std::setw

#define LINE_INFO std::cout << "[line " << __LINE__ << "] "

class MinHeap {
private:
    std::vector<int> heap;

    // Get parent index
    int parent(int i) { return (i - 1) / 2; }

    // Get left child index
    int leftChild(int i) { return (2 * i) + 1; }

    // Get right child index
    int rightChild(int i) { return (2 * i) + 2; }

    // Maintain heap property upwards after insertion
    void heapifyUp(int index) {
        std::cout << "[line " << __LINE__ << "] Heapify up called for index: " << index << std::endl;
        while (index > 0 && heap[parent(index)] > heap[index]) {
            std::swap(heap[parent(index)], heap[index]);
            if (index == 11) {
                printPrettyHeap(__LINE__);
            }
            std::cout << "[line " << __LINE__ << "] Swapped ";
            printHeap(__LINE__);
            index = parent(index);
        }
    }
    void siftUp(int index) {
        int currentValue = heap[index];
        while (index > 0) {
            int parentIndex = parent(index);
            if (heap[parentIndex] <= currentValue) {
                break;
            }
            heap[index] = heap[parentIndex];
            index = parentIndex;
        }
        heap[index] = currentValue;
    }

    // Maintain heap property downwards after extraction
    void heapifyDown(int index) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < heap.size() && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < heap.size() && heap[right] < heap[smallest]) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }
    void heapifyDownNoRecursive(int index) {
        printPrettyHeap(__LINE__); // Debugging: Print the heap before starting the down-heap process
        while (true) {
            int smallest = index;
            int left = leftChild(index);
            int right = rightChild(index);

            if (left < heap.size() && heap[left] < heap[smallest]) {
                smallest = left;
            }
            if (right < heap.size() && heap[right] < heap[smallest]) {
                smallest = right;
            }

            if (smallest == index) {
                break;
            }

            std::swap(heap[index], heap[smallest]);
            index = smallest;
            std::cout << "[line " << __LINE__ << "] ------------------------------------\n";
            printPrettyHeap(__LINE__); // Debugging: Print the heap after each swap
            std::cout << "[line " << __LINE__ << "] ------------------------------------\n";
        }
    }


public:
    // Insert a new key into the heap
    void insert(int key) {
        heap.push_back(key);
        // debugging: Print the heap after insertion
        printHeap(__LINE__);
        heapifyUp(heap.size() - 1);
    }

    void printHeap(int fromLine = 0) {
        if (fromLine > 0) {
            std::cout << "[line " << fromLine << "] ";
        }
        std::cout << "Heap elements: ";
        for (int val : heap) {
            // aligned output for better readability 2 characters wide
            std::cout << std::setw(2) << val << " ";
        }
        std::cout << std::endl;
    }

    // Return the minimum element (root)
    int getMin() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    // Remove and return the minimum element
    int extractMin() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }

        int minVal = heap[0];
        std::cout << "[line " << __LINE__ << "] Extracting min: " << minVal << std::endl;
        printHeap(__LINE__);
        heap[0] = heap.back();
        heap.pop_back();
        std::cout << "[line " << __LINE__ << "] After removing min, new root: " << heap[0] << std::endl;
        printHeap(__LINE__);

        if (!heap.empty()) {
            heapifyDownNoRecursive(0);
        }

        return minVal;
    }

    // Get the current size of the heap
    size_t size() {
        return heap.size();
    }
    void printHeapAsTree(int fromLine = 0)
    {
        if (fromLine > 0) {
            std::cout << "[line " << fromLine << "] ";
        }
        int n = heap.size();
        int levels = 0;
        while ((1 << levels) - 1 < n)
        {
            levels++;
        }
        std::cout << "Heap as tree: " << levels << " levels\n";

        for (int i = 0; i < levels; ++i)
        {
            int levelStart = (1 << i) - 1;
            int levelEnd = std::min((1 << (i + 1)) - 1, n);
            for (int j = levelStart; j < levelEnd; ++j)
            {
                std::cout << heap[j] << " ";
            }
            std::cout << "\n";
        }
    }

    // Helper to print a specific number of spaces
    void printSpaces(int count)
    {
        for (int i = 0; i < count; i++)
            printf(" ");
    }

    void printRecursiveTree(int fromLine = 0)
    {
        if (fromLine > 0) {
            std::cout << "[line " << fromLine << "] Recursive tree:\n";
        }
        printTree(heap.data(), heap.size(), 0, 0, 3);
    }

    void printTree(int *data, int size, int index, int level, int indent)
    {
        if (index >= size)
            return;

        // 1. Process Right Child first (to print top of tree first)
        printTree(data, size, 2 * index + 2, level + 1, indent);

        // 2. Print current node with indentation based on level
        printSpaces(level * indent);
        printf("%d\n", data[index]);
        // 3. Process Left Child
        printTree(data, size, 2 * index + 1, level + 1, indent);
    }

    // Better version: Calculating spacing to look like a real tree
    void printPrettyHeap(int fromLine = 0)
    {
        if (fromLine > 0) {
            std::cout << "[line " << fromLine << "] Pretty heap:\n";
        }
        int height = (int)(log2(size())) + 1;
        for (int i = 0; i < size(); i++)
        {
            int level = (int)log2(i + 1);
            int spaces = (int)(pow(2, height - level) - 1);

            printSpaces(spaces);
            printf("%d", heap[i]);
            printSpaces(spaces);

            // Newline at the end of a level
            if (((i + 2) & (i + 1)) == 0)
                printf("\n\n");
        }
        printf("\n");
    }
    void printHeapAsVector(int fromLine = 0)
    {
        if (fromLine > 0) {
            std::cout << "[line " << fromLine << "] ";
        }
        std::cout << "Heap as vector: ";
        for (int i = 0; i < heap.size(); ++i)
        {
            std::cout << heap[i] << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    MinHeap mh;
    mh.insert(3);
    mh.insert(2);
    mh.insert(15);
    mh.insert(5);
    mh.insert(4);
    mh.insert(45);
    mh.insert(6);
    mh.insert(7);
    mh.insert(8); 
    mh.insert(9);
    mh.insert(50);
    mh.insert(1);
    mh.insert(2);

    mh.printHeap(__LINE__);
    std::cout << "[line " << __LINE__ << "] Current min: " << mh.getMin() << "\n";       // Outputs 3
    // mh.printHeapAsTree(__LINE__);
    std::cout << "[line " << __LINE__ << "] Pretty print of the heap:\n";
    mh.printPrettyHeap(__LINE__);
    // std::cout << "\nrecursive print of the heap:\n";
    // mh.printRecursiveTree(__LINE__);
    std::cout << "[line " << __LINE__ << "] Extracted min: " << mh.extractMin() << "\n"; // Outputs 2
    mh.printPrettyHeap(__LINE__);
    std::cout << std::endl;

    return 0;
}

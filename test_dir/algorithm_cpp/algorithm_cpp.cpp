#include <iostream>
#include <algorithm>
#include <vector>

class algorithm_cpp
{
private:
    int a;
    int b;
public:
    algorithm_cpp(int _a, int _b):a(_a),b(_b){};
    ~algorithm_cpp(){};
    void print_algorithm_cpp(){std::cout << "algorithm_cpp "<< std::endl;};
    
    // Example method using algorithm library to check if vector contains number divisible by 10
    bool hasNumberDivisibleBy10(const std::vector<int>& numbers) {
        return std::any_of(numbers.begin(), numbers.end(), 
                          [](int n) { return n % 10 == 0; });
    }
    bool isNumEqualsB(int num) const {
        return num == b;
    }
    
    void demonstrateAlgorithm() {
        // Example vectors
        std::vector<int> vec1 = {3, 7, 15, 22, 30, 45};
        std::vector<int> vec2 = {3, 7, 15, 22, 33, 45};
        std::vector<int> vec3 = {100, 200, 300};
        
        std::cout << "\nDemonstrating std::any_of to check for numbers divisible by 10:\n";
        std::cout << "----------------------------------------\n";
        
        // Check vec1
        std::cout << "Vector 1: ";
        for (int n : vec1) std::cout << n << " ";
        std::cout << "\nContains number divisible by 10: " 
                  << (hasNumberDivisibleBy10(vec1) ? "YES" : "NO") << "\n\n";
        
        // Check vec2
        std::cout << "Vector 2: ";
        for (int n : vec2) std::cout << n << " ";
        std::cout << "\nContains number divisible by 10: " 
                  << (hasNumberDivisibleBy10(vec2) ? "YES" : "NO") << "\n\n";
        
        // Check vec3
        std::cout << "Vector 3: ";
        for (int n : vec3) std::cout << n << " ";
        std::cout << "\nContains number divisible by 10: " 
                  << (hasNumberDivisibleBy10(vec3) ? "YES" : "NO") << "\n\n";
        
        // Alternative: Using std::find_if
        std::cout << "Alternative approach using std::find_if:\n";
        auto it = std::find_if(vec1.begin(), vec1.end(), 
                              [](int n) { return n % 10 == 0; });
        
        if (it != vec1.end()) {
            std::cout << "First number divisible by 10 in vec1: " << *it << "\n";
        } else {
            std::cout << "No number divisible by 10 found in vec1\n";
        }
    }

};
algorithm_cpp& search_if_inclueds(std::vector<algorithm_cpp>& vec, int t)
{
    for (auto& item : vec) {
        if (item.isNumEqualsB(t)) {
            return item;
        }
    }
    static algorithm_cpp dummy(0, 0); // Return reference to static dummy if not found
    return dummy;
}


int main()
{
    algorithm_cpp t(10, 20);
    // t.print_algorithm_cpp();
    // t.demonstrateAlgorithm();

    std::vector<algorithm_cpp> vec; 
    vec.push_back(algorithm_cpp(1, 20));
    vec.push_back(algorithm_cpp(2, 30));
    vec.push_back(algorithm_cpp(3, 40));
    int searchValue = 30;
    algorithm_cpp found = search_if_inclueds(vec, searchValue);
    if (found.isNumEqualsB(searchValue)) {
        std::cout << "Found an object with b equal to " << searchValue << "\n";
    } else {
        std::cout << "No object found with b equal to " << searchValue << "\n";
    }

    return 0;
}

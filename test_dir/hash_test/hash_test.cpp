#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class hash_test
{
private:
    /* data */
public:
    hash_test(/* args */){};
    ~hash_test(){};
    void print_hash_test(){std::cout << "hash_test "<< std::endl;};

};

size_t calculateMetricHash(const std::string& oltIfName, int state, const std::string& oltName, const std::string& oltMacAddr, const std::string& moduleName, const std::string& moduleId) {
    // Calculate a hash value based on the Olt's operational status and other relevant fields
    size_t hashValue = 0;
    std::hash<std::string> strHash;
    std::hash<int> intHash;

    // Combine the hashes of relevant fields
    hashValue ^= strHash(moduleName) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= strHash(moduleId) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= strHash(oltIfName) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= intHash(state) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= strHash(oltName) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= strHash(oltMacAddr) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);

    return hashValue;
}

int main()
{
    hash_test t;
    std::vector<std::string> str = {"Hello World"," C++17", "Hash Test"};
    size_t value = 511;
    auto t1 = std::chrono::high_resolution_clock::now();
    size_t hash_value = std::hash<std::string>{}(str[0]+str[1]+str[2]+std::to_string(value));
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Hash value: " << hash_value << std::endl;
    std::cout << "String concat hash time: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
              << " ns" << std::endl;
    t.print_hash_test();

    auto t3 = std::chrono::high_resolution_clock::now();
    size_t metric_hash = calculateMetricHash("eth0", 1, "olt-1", "AA:BB:CC:DD:EE:FF", "module-A", "mod-key-1");
    auto t4 = std::chrono::high_resolution_clock::now();
    std::cout << "Metric hash: " << metric_hash << std::endl;
    std::cout << "calculateMetricHash time: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count()
              << " ns" << std::endl;
    return 0;
}

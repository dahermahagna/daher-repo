#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <functional>
#include <sstream>
#include <iomanip>

// #define RNG_VERBOSE
class random_num
{
private:
    std::vector<uint8_t> secret;
public:
    random_num(/* args */){};
    ~random_num(){};
    void print_random_num(){std::cout << "random_num "<< std::endl;};
    void GenerateSharedSecret(uint8_t size) {
        uint64_t seed = (static_cast<uint64_t>(std::random_device{}()) << 32)
                      ^ static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
#ifdef RNG_VERBOSE
        std::cout << "[GenerateSharedSecret] Requested size: " << static_cast<int>(size) << std::endl;
        std::cout << "[GenerateSharedSecret] Combined seed: " << seed << std::endl;
#endif
        std::mt19937_64 engine(seed);
        std::uniform_int_distribution<int> distribution(1, 0xFF);
        auto generator = [&engine, &distribution]() {
            return static_cast<uint8_t>(distribution(engine));
        };

        std::vector<uint8_t> dynamicSecret;
        dynamicSecret.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            auto val = generator();
            dynamicSecret.push_back(val);
#ifdef RNG_VERBOSE
            std::cout << "[GenerateSharedSecret] Byte " << i << ": 0x" << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(val) << std::dec << std::endl;
#endif
        }
        secret = std::move(dynamicSecret);
#ifdef RNG_VERBOSE
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (auto b : secret) {
            oss << std::setw(2) << static_cast<int>(b) << ' ';
        }
        std::cout << "[GenerateSharedSecret] Final secret (" << secret.size() << " bytes): " << oss.str() << std::dec << std::endl;
#endif
    }
    std::vector<uint8_t> GetSharedSecret() {
#ifdef RNG_VERBOSE
        std::cout << "[GetSharedSecret] Returning secret of size: " << secret.size() << std::endl;
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (size_t i = 0; i < secret.size(); ++i) {
            oss << std::setw(2) << static_cast<int>(secret[i]) << ' ';
        }
        std::cout << "[GetSharedSecret] Secret bytes: " << oss.str() << std::dec << std::endl;
#endif
        return secret;
    }

};

int main()
{
    random_num t;
    t.print_random_num();
    t.GenerateSharedSecret(16);
    auto secret = t.GetSharedSecret();
    std::cout << "Generated Secret: ";
    for (auto byte : secret) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
    return 0;
}

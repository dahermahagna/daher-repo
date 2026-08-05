#include <iostream>
#include <string>
#include <map>
#include <set>

class cpp_map
{
private:
    std::string _name;
public:
    cpp_map() : _name("") {}
    cpp_map(std::string name):_name(name){};
    ~cpp_map(){};
    void print_cpp_map_name() const {std::cout << "name: "<< _name << std::endl;};
    bool operator<(const cpp_map& other) const {
        return _name < other._name;
    }
};
std::tuple<uint32_t,uint32_t,uint32_t> calculate_ucid(uint32_t a)
{
    return {(a / 3 + 60 + 0), (a / 3 + 60 + 1), (a / 3 + 60 + 2)};
}
int main()
{
    // cpp_map t("test");
    // t.print_cpp_map_name();
    std::map<cpp_map,cpp_map> mymap;
    std::cout<<"size of map: "<< mymap.empty() << std::endl;
    cpp_map map1("map1");
    cpp_map map2("map2");
    cpp_map map3("map3");
    cpp_map map4("map4");
    cpp_map map5("map5");
    cpp_map map6("map6");

    mymap[map1] = map2;
    mymap[map2] = map3;
    mymap[map3] = map4;
    mymap[map4] = map5;
    mymap[map5] = map6;
    mymap[map6] = map1;

    for (const auto& pair : mymap) {
        // pair.first.print_cpp_map_name();
        pair.first.print_cpp_map_name();
        pair.second.print_cpp_map_name();
        std::cout << "----" << std::endl;
    }
    uint32_t num1 = 1;
    uint32_t num2 = 9;
    auto [a, b, c] = calculate_ucid(num1);
    std::cout << "For num1 = " << num1 << ": a = " << a << ", b = " << b << ", c = " << c << std::endl;
    auto [d, e, f] = calculate_ucid(num2);
    std::cout << "For num2 = " << num2 << ": d = " << d << ", e = " << e << ", f = " << f << std::endl;

    std::map<std::string, std::set<int>> string_map;
    string_map["first"] = {1, 2, 3};
    string_map["second"] = {4, 5, 6};
    for (const auto& [key, value_set] : string_map) {
        std::cout << "Key: " << key << " Values: ";
        for (const auto& val : value_set) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    string_map["first"].insert(10);
    std::cout << "After insertion:" << std::endl;
    for (const auto& [key, value_set] : string_map) {
        std::cout << "Key: " << key << " Values: ";
        for (const auto& val : value_set) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "sizeof first element in map: " << string_map["first"].size() << std::endl;
    return 0;

}

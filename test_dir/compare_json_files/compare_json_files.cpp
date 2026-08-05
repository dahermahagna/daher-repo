#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

class compare_json_files
{
private:
    /* data */
public:
    compare_json_files(/* args */){};
    ~compare_json_files(){};
    void print_compare_json_files(){std::cout << "compare_json_files "<< std::endl;};

};

// i want function that compares two json files and prints the differences
// for now, just print the names of the files being compared

// void compare_json_files_function(const std::string& file1, const std::string& file2)
// {
//         std::cout << "Comparing JSON files: " << file1 << " and " << file2 << std::endl;

//     // Read the first JSON file
//     std::ifstream f1(file1);
//     if (!f1) {
//         std::cerr << "Failed to open " << file1 << std::endl;
//         return;
//     }
//     nlohmann::json j1;
//     f1 >> j1;

//     // Read the second JSON file
//     std::ifstream f2(file2);
//     if (!f2) {
//         std::cerr << "Failed to open " << file2 << std::endl;
//         return;
//     }
//     nlohmann::json j2;
//     f2 >> j2;

//     // Compare the JSON objects and print differences
//     if (j1 == j2) {
//         std::cout << "The JSON files are identical." << std::endl;
//     } else {
//         std::cout << "The JSON files are different." << std::endl;
//         std::cout << "File 1 content:\n" << j1.dump(4) << std::endl;
//         std::cout << "File 2 content:\n" << j2.dump(4) << std::endl;
//     }
// }

void print_json_diff(const nlohmann::json& j1, const nlohmann::json& j2, const std::string& path = "") {
    if (j1.type() != j2.type()) {
        std::cout << "Type mismatch at " << path << ": "
                  << j1.type_name() << " vs " << j2.type_name() << std::endl;
        return;
    }
    if (j1.is_object()) {
        for (auto it = j1.begin(); it != j1.end(); ++it) {
            std::string current_path = path.empty() ? it.key() : path + "." + it.key();
            if (j2.contains(it.key())) {
                print_json_diff(it.value(), j2.at(it.key()), current_path);
            } else {
                std::cout << "Key only in first file: " << current_path << std::endl;
            }
        }
        for (auto it = j2.begin(); it != j2.end(); ++it) {
            std::string current_path = path.empty() ? it.key() : path + "." + it.key();
            if (!j1.contains(it.key())) {
                std::cout << "Key only in second file: " << current_path << std::endl;
            }
        }
    } else if (j1.is_array()) {
        size_t min_size = std::min(j1.size(), j2.size());
        for (size_t i = 0; i < min_size; ++i) {
            print_json_diff(j1[i], j2[i], path + "[" + std::to_string(i) + "]");
        }
        if (j1.size() > j2.size()) {
            for (size_t i = min_size; i < j1.size(); ++i)
                std::cout << "Element only in first file: " << path << "[" << i << "]" << std::endl;
        } else if (j2.size() > j1.size()) {
            for (size_t i = min_size; i < j2.size(); ++i)
                std::cout << "Element only in second file: " << path << "[" << i << "]" << std::endl;
        }
    } else {
        if (j1 != j2) {
            std::cout << "Value mismatch at " << path << ": "
                      << j1 << " vs " << j2 << std::endl;
        }
    }
}

void compare_json_files_function(const std::string& file1, const std::string& file2)
{
    std::cout << "Comparing JSON files: " << file1 << " and " << file2 << std::endl;

    std::ifstream f1(file1);
    if (!f1) {
        std::cerr << "Failed to open " << file1 << std::endl;
        return;
    }
    nlohmann::json j1;
    f1 >> j1;

    std::ifstream f2(file2);
    if (!f2) {
        std::cerr << "Failed to open " << file2 << std::endl;
        return;
    }
    nlohmann::json j2;
    f2 >> j2;

    if (j1 == j2) {
        std::cout << "The JSON files are identical." << std::endl;
    } else {
        std::cout << "Differences found:" << std::endl;
        print_json_diff(j1, j2);
    }
}

int main()
{
    compare_json_files t;
    t.print_compare_json_files();
    // Example usage
    std::string file1 = "json_files/cli_json_config_2";
    std::string file2 = "json_files/confd_json_config_2";
    compare_json_files_function(file1, file2);
    return 0;
}

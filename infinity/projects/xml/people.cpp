#include <string>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <vector>
#include <utility>   // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream>   // std::stringstream
#include <vector>
#include <iostream>

enum variables 
{   
    PATH = 0
};

class ConfigFile
{
public:
    explicit ConfigFile(std::string path_);
    std::string GetSOPath();
    std::vector<std::vector<std::string> > GetData();

private:
    std::string m_config_path;
    //std::ifstream m_config_file;
};

std::vector<std::vector<std::string> > ConfigFile::GetData()
{
    std::ifstream file(m_config_path);
    std::vector<std::vector<std::string> > dataList;
    std::string line = "";
    // Iterate through each line and split the content using delimeter
    while (getline(file, line))
    {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(","));
        dataList.push_back(vec);
    }
    // Close the File
    file.close();
    return dataList;
}

ConfigFile::ConfigFile(std::string path_)
: m_config_path(path_)
{
    
}
std::string  ConfigFile::GetSOPath()
{
    //path_to_return_ = std::getline(m_config_file,PATH);

    std::vector<std::vector<std::string> > dataList = GetData();
    return dataList.at(PATH).at(1);
}


int main()
{
    ConfigFile cf("config.csv");
    //char path[1024] = {0};
    std::cout <<  cf.GetSOPath() << std::endl;

    return 0;
}


/* std::ifstream people_file("config.csv", std::ifstream::binary);
people_file >> people;

cout << people; //This will print the entire json object.

//The following lines will let you access the indexed objects.
cout << people["Anna"];               //Prints the value for "Anna"
cout << people["ben"];                //Prints the value for "Ben"
cout << people["Anna"]["profession"]; //Prints the value corresponding to "profession" in the json for "Anna"
 */
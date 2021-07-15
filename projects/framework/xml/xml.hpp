#ifndef __RD94_XML_HPP__
#define __RD94_XML_HPP__

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <string>

namespace ilrd
{
class Xml
{
    public:
    explicit Xml(const char *xml_path_);
    //~Xml() = default
    std::string GetSharedFolderPath() const;
    std::string GetLoggerPath() const;
    int GetPolicy() const;
    size_t GetNumOfThreads() const;
    int GetNiceness() const;

    private:
    void WriteToXml();
    void ReadIntoPTree();

    boost::property_tree::ptree m_pt;
    std::string m_shared_folder_path;
    std::string m_logger_path;
    int m_policy;
    size_t m_num_thread;
    int m_niceness;
};



} //namespace

#endif //__RD94_FRAMWORK_HPP__

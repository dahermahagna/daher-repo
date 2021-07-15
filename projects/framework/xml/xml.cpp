#include <cassert>  //assert

#include "xml.hpp"

namespace ilrd
{

Xml::Xml(const char *xml_path_)
: m_pt()
, m_shared_folder_path()
, m_logger_path()
, m_policy(0)
, m_num_thread(2)
, m_niceness(0)
{
  assert(NULL != xml_path_);
  //boost::property_tree::ptree m_pt;
  boost::property_tree::read_xml(xml_path_, m_pt);
  m_shared_folder_path = m_pt.get<std::string>("framework.folder_path");
  m_logger_path = m_pt.get<std::string>("framework.logger_path");
  m_policy = m_pt.get<int>("framework.thread_pool_arg.policy");
  m_num_thread = m_pt.get<size_t>("framework.thread_pool_arg.num_threads");
  m_niceness = m_pt.get<int>("framework.thread_pool_arg.niceness");
}

std::string Xml::GetSharedFolderPath() const
{
  return m_shared_folder_path;
}

std::string Xml::GetLoggerPath() const
{
  return m_logger_path;
}

int Xml::GetPolicy() const
{
  return m_policy;
}

size_t Xml::GetNumOfThreads() const
{
  return m_num_thread;
}
int Xml::GetNiceness() const
{
  return m_niceness;
}

} //namespace
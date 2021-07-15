
#include "framework.hpp"

namespace ilrd
{

/* static const size_t THREAD_POLICY = 4;   // can be used as user demand
static const size_t NUM_OF_THREAD = 4;
static const size_t THREAD_NICENESS = 4; */

FrameWork::FrameWork() //:
:m_log(Singleton<Logger>::GetInstance())
,m_xml("xml/configurations.xml");
,m_factory()
,m_thrdp(boost::make_shared<ThreadPool>(ThreadPool(m_xml.GetPolicy(),m_xml.GetNumOfThreads(),m_xml.GetNiceness())))
,m_mediator(m_thrdp, boost::bind(&Factory<Command, std::vector<char>, std::string>::Create, &m_factory, _1, _2))
,m_plug_play(xml.GetSharedFolderPath().c_str())
{
    LOG_INFO("FW start");
    m_thrdp->Run();
}

FrameWork::~FrameWork() noexcept
{
    LOG_INFO("FW stop");
    m_thrdp->Stop();
}

void FrameWork::RegisterProxy(int fd_, boost::shared_ptr<ProxyBase> proxy_)
{
    m_mediator.RegisterProxy(fd_,proxy_);
}

void FrameWork::RegisterCmd(std::string id_, create_cmd_func_ty func_)
{
    m_factory.Add(id_,func_);
}

PlugPlay::PlugPlay(const char *dir_path_)
:m_dir_monitor(dir_path_)
,m_loader(&m_dir_monitor)
{
    
}


} // namespace ilrd
    

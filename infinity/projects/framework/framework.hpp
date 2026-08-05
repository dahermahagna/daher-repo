#ifndef __RD94_FRAMWORK_HPP__
#define __RD94_FRAMWORK_HPP__

#include <iosfwd> // vector string

#include <boost/core/noncopyable.hpp> // noncopyalbe
#include <boost/function.hpp>   // boost::function
#include <boost/shared_ptr.hpp> // boost shared_ptr

#include "mediator.hpp"    // mediator
#include "singleton.hpp"   // singleton
#include "dir_monitor.hpp" // dir_monitor
#include "factory.hpp"     // factory
#include "logger.hpp"      // logger
#include "xml.hpp"     // Xml 

namespace ilrd
{

class PlugPlay : private boost::noncopyable
{
public:
    explicit PlugPlay(const char *dir_path_); //may throw bed_alloc, bad_resource_error, MonitorFail

private:
    DirMonitor m_dir_monitor;
    DllLoader m_loader;
};

class FrameWork : private boost::noncopyable // of course
{
public:
    typedef boost::shared_ptr<Command> cmd_ptr_ty;
    typedef boost::function<cmd_ptr_ty(std::vector<char>)> create_cmd_func_ty;

    void RegisterProxy(int fd_, boost::shared_ptr<ProxyBase> proxy_); //may throw bad_alloc
    void RegisterCmd(std::string id_, create_cmd_func_ty func_);

private:
    typedef Factory<Command, std::vector<char>, std::string> FrameWork_factory_ty;
    friend class Singleton<FrameWork>;

    explicit FrameWork(); //may throw bad_alloc, bad_resource_error
    ~FrameWork();

    Logger *m_log;
    Xml m_xml;
    FrameWork_factory_ty m_factory;
    Mediator::thread_pool_ptr m_thrdp; // maybe need namespace
    Mediator m_mediator;
    PlugPlay m_plug_play;


    
};

} // namespace ilrd

#endif //__RD94_FRAMWORK_HPP__
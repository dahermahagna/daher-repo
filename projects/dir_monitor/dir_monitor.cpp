#include <cassert>          //assert
#include <iostream>         // cout
#include <fstream>          // ofstream

#include "dir_monitor.hpp"  // DirMonitor

namespace ilrd
{

void DirMonitor::InitMonitor(const char *dir_path_)
{
    m_fd = inotify_init();
    if (0 > m_fd)
    {
        throw MonitorFailException();
    }

    m_watch_fd = inotify_add_watch(m_fd, dir_path_, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE);
    if(0 > m_watch_fd)
    {
        close(m_fd);
        throw MonitorFailException();
    }

    m_dispatcher = new Dispatcher<DirEvent_t>;

    try
    {
        shr_ptr_ty new_th(new boost::thread(boost::bind(&DirMonitor::Monitor, this)));
        m_monitor_thread = new_th;
    }
    catch(const std::exception& e)
    {
        inotify_rm_watch(m_fd, m_watch_fd);
        close(m_fd);

        delete m_dispatcher;

        throw MonitorFailException();
    }
}

DirMonitor::DirMonitor(const char *dir_path_)
:m_fd(0)
,m_watch_fd(0)
,m_to_run_monitor(true)
{
    assert(NULL != dir_path_);
    m_dir_path = dir_path_;
    InitMonitor(dir_path_);
}

void DirMonitor::Monitor()
{
    static const size_t EVENT_QUEUE_SIZE = 1024;
    static const size_t EVENT_SIZE = sizeof(struct inotify_event);
    static const size_t EVENT_BUF_LEN = EVENT_QUEUE_SIZE * (EVENT_SIZE + NAME_MAX);

    while (true == m_to_run_monitor)
    {
        char buf[EVENT_BUF_LEN] = {0};
        ssize_t length = read(m_fd,buf,EVENT_BUF_LEN);
        if(0 > length)
        {
            inotify_rm_watch(m_fd, m_watch_fd);
            close(m_fd);

            delete m_dispatcher;

            throw MonitorFailException();
        }

        struct inotify_event *event;

        for (size_t i = 0; (size_t)length > i && true == m_to_run_monitor; i += (EVENT_SIZE + event->len))
        {
            event = (struct inotify_event*)&buf[i];
            DirEvent_t dir_event = TranslateINotifyToEvent(event);

            m_dispatcher->Broadcast(dir_event);
        } 
    }   
}

void DirMonitor::Subscribe(CallbackBase<DirEvent_t> *cb_)
{
    assert(NULL != cb_);
    m_dispatcher->Subscribe(cb_);
}

DirMonitor::~DirMonitor() noexcept
{
    if(true == m_to_run_monitor)
    {
        StopMonitor();
    }
}

DirEvent_t DirMonitor::TranslateINotifyToEvent(struct inotify_event *buffer)
{
    assert(NULL != buffer);
    DirEvent_t return_event;

    return_event.filename = buffer->name;
    
    if (buffer->mask & IN_MODIFY)
    {
        return_event.m_event_type = MODIFIED;
    }
    else if (buffer->mask & IN_DELETE || buffer->mask & IN_MOVED_FROM)
    {
        return_event.m_event_type = REMOVE;
    }
    else if(buffer->mask & IN_CREATE || buffer->mask & IN_MOVED_TO)
    {
        return_event.m_event_type = ADD;
    }

    return return_event;
}

void DirMonitor::StopMonitor()
{
    m_to_run_monitor = false;
    std::string stop_path(m_dir_path.append("/stop.txt"));
    std::ofstream stopper(stop_path.c_str());
    std::remove(stop_path.c_str());
    m_monitor_thread->join();
    delete m_dispatcher;
}

/*------------------ dlloader ---------------*/

DllLoader::DllLoader(DirMonitor *monitor_)
:m_callback(new Callback<DirEvent_t,DllLoader>(this,&DllLoader::Notify, &DllLoader::DirMonitorDeath))
{
    assert(NULL != monitor_);
    monitor_->Subscribe(m_callback);
}

void DllLoader::Notify(DirEvent &dir_event)
{
    std::cout << dir_event.filename <<" " << dir_event.m_event_type << std::endl;
}

void DllLoader::DirMonitorDeath()
{
    write(STDOUT_FILENO, "Death\n", 7);
}

DllLoader::~DllLoader()
{
    delete m_callback;
}

}
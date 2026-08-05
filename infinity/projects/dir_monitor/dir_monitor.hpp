#ifndef __RD94_DIR_MONITOR_HPP__
#define __RD94_DIR_MONITOR_HPP__

#include <sys/inotify.h>            // inotify API
//#include <linux/inotify.h>          // struct inotify_event
#include <string>                   //std::string
#include <boost/noncopyable.hpp>    // noncopyable
#include <boost/shared_ptr.hpp>     // shared ptr
#include <boost/thread.hpp>         // thread

#include "../dispatcher/dispatcher_callback_impl.hpp"

namespace ilrd
{


typedef enum DirEventType
{
    ADD = 0,
    MODIFIED = 1,
    REMOVE = 2
}DirEventType_t;

typedef struct DirEvent
{
    std::string filename;
    DirEventType_t m_event_type;

} DirEvent_t;



class DirMonitor: private boost::noncopyable
{
public:

    explicit DirMonitor(const char *dir_path); // may throw MonitorFailException, bad_alloc
    ~DirMonitor() noexcept;
    void StopMonitor();
    void Subscribe(CallbackBase<DirEvent_t> *cb_);

private:
    typedef boost::shared_ptr<boost::thread> shr_ptr_ty;

        void Monitor(); // may throw MonitorFailException
    DirEvent_t TranslateINotifyToEvent(struct inotify_event *buffer);

    Dispatcher<DirEvent_t> *m_dispatcher; 
    
    struct MonitorFailException : public std::exception
    {
        const char * what () const throw ()
        {
            return "Monitor Fail";
        }
    }; 
    int m_fd;
    int m_watch_fd;
    bool m_to_run_monitor;
    shr_ptr_ty m_monitor_thread;
    std::string m_dir_path;

    void InitMonitor(const char *dir_path_);
};


class DllLoader: private boost::noncopyable
{
public: 
    explicit DllLoader(DirMonitor *monitor_);
    ~DllLoader();
    
private:
    void DirMonitorDeath();
    void Notify(DirEvent& dir_event);
    //boost::shared_ptr<Callback<DirEvent_t, DllLoader>> m_callback;
    Callback<DirEvent_t,DllLoader> *m_callback;
    
};

} //ilrd


#endif //__RD94_DIR_MONITOR_HPP__
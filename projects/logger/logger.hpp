#ifndef __RD94_LOGGER_HPP__
#define __RD94_LOGGER_HPP__

#include <fstream>                  //ofstream    
#include <boost/atomic.hpp>         //boost::atomic
#include <boost/thread.hpp>          // thread

#include "../waitable_queue/waitable_queue_impl.hpp" //waitable_q
#include "../singleton/singleton_impl.hpp"

namespace ilrd
{

#define LOG (Singleton<Logger>::GetInstance()->Log)

#define LOG_ERROR(message_) (LOG(Logger::ERROR, message_))
#define LOG_WARNING(message_) (LOG(Logger::WARNING, message_))
#define LOG_INFO(message_) (LOG(Logger::INFORMATION, message_))

class Logger
{
public:
    //envarment variable LOG_PATH is revserved for log file path may throw LogException

    typedef enum MessageLevel
    {
        NONE,
        ERROR,
        WARNING,
        INFORMATION,
        DEBUG 
    } MessageLevel_t;

    MessageLevel_t GetLevel() const;
    void SetLevel(MessageLevel_t level_);

    void Log(MessageLevel_t level_, const std::string& message_); // may throw Bad_alloc, LogException

    struct LogException : public std::runtime_error
    {
        LogException(): std::runtime_error(""){}
        const char * what () const throw ()
        {
            return "Log Fail";
        }
    };

private:
    friend class Singleton<Logger>;
    explicit Logger(); //envarment variable LOG_PATH is revserved for log file path may throw LogException
    ~Logger() noexcept;

    WaitableQueue< std::string > m_message_q;
    boost::atomic< MessageLevel_t > m_current_level;
    boost::atomic< bool > m_stop_flag;
    void PopAndLog();
    /* boost::mutex::mutex m_message_lock;*/
    boost::thread m_logging_thread;
    
    std::ofstream m_log_fd;

};

}// ilrd

#endif //__RD94_LOGGER_HPP__
#include "logger.hpp"
// reviewed by Moriya
namespace ilrd
{

Logger::Logger() 
:m_current_level(NONE)
,m_stop_flag(false)
,m_logging_thread(boost::thread(boost::bind(&Logger::PopAndLog,this)))

,m_log_fd()
{
    char *logger_path = std::getenv("LOG_PATH");
    if(NULL == logger_path)
    {
        m_logging_thread.interrupt();
        m_logging_thread.join();
        throw LogException();
    }
    m_log_fd.open(logger_path);
}

Logger::~Logger() noexcept
{
    m_stop_flag = true;
    m_log_fd.close();
    m_logging_thread.interrupt();
    m_logging_thread.join();
}
void Logger::Log(MessageLevel_t level_, const std::string& message_)
{
    if(level_ <= m_current_level.load(boost::memory_order_relaxed))
    {
        m_message_q.Push(message_);
    }
}

Logger::MessageLevel_t Logger::GetLevel() const
{

    return m_current_level.load(boost::memory_order_relaxed);
}

void Logger::SetLevel(MessageLevel_t level_)
{
    m_current_level.store(level_,boost::memory_order_relaxed);
    //m_current_level = level_;
}

void Logger::PopAndLog()
{
    std::string str;
    while (false == m_stop_flag.load(boost::memory_order_relaxed))
    {
        m_message_q.Pop(&str);
        m_log_fd << str;
        m_log_fd << "\n";
    }
    
}

}
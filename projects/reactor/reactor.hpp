#ifndef __RD94_REACTOR_HPP__
#define __RD94_REACTOR_HPP__

#include <map>                          // map
#include <boost/unordered_map.hpp>      //unorderdmap
#include <utility>                      // pair
#include <boost/noncopyable.hpp>        // noncopyable
#include <boost/function.hpp>           // function

#include "../utils/utils.hpp"

namespace ilrd
{



class Reactor : private boost::noncopyable
{
public:
    typedef enum ModeType
    {
        READ = 0,
        WRITE,
        EXCEPTION
    } ModeType_ty;

    explicit Reactor();
    //~Reactor() = default
    void Add(int fd_, ModeType_ty mode_, boost::function<void ()> handler_); //overrites, throws
    void Remove(int fd_, ModeType_ty mode_); //throw logical_error exception
    void Run(); //blocking, until all FD's are removed or Stop() called //throws
    void Stop(); // not thread safe, non reentrant

private:
    typedef std::pair<int, ModeType_ty> fd_mode_ty;
    //typedef std::map<fd_mode_ty, boost::function<void ()> > HandlerMap_ty;
    typedef boost::unordered_map<fd_mode_ty, boost::function<void ()> > HandlerMap_ty;

    class Listener;
    class Select_Listener;
    friend class Select_Listener;
    HandlerMap_ty m_map;
    Listener *m_listener;
    bool m_stop_flag;
    
    class Listener // abstract 
    {
    public:
        explicit Listener(Reactor *reactor_);
        virtual void AddFd(int fd_, ModeType_ty mode_) = 0;
        virtual void RemoveFd(int fd_, ModeType_ty mode_) = 0;
        virtual void RunFd(std::vector<Reactor::fd_mode_ty> *ready_fd) = 0;
        virtual ~Listener();
    private:
        Reactor* m_reactor;

    };

    class Select_Listener : public Listener
    {
    public:
        explicit Select_Listener(Reactor *reactor_);
        void AddFd(int fd_, ModeType_ty mode_);
        void RemoveFd(int fd_, ModeType_ty mode_);
        void RunFd(std::vector < Reactor::fd_mode_ty> *ready_fd);
        void SetFd(int fd_, ModeType_ty mode_);
        void ClrFd(int fd_, ModeType_ty mode_);

    private:
        fd_set m_master_read_set;
        fd_set m_master_write_set;
        fd_set m_master_except_set;
        int m_max_fd;
    };


};


} // namespace ilrd

#endif // __RD94_REACTOR_HPP__
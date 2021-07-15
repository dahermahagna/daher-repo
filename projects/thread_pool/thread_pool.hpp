#ifndef __RD94_THREAD_POOL_HPP__
#define __RD94_THREAD_POOL_HPP__

#include <boost/thread/future.hpp> // promise
#include <map> // map
#include <boost/atomic.hpp> //atomic

#include "../waitable_queue/waitable_queue_impl.hpp" //WaitableQueue
#include "../waitable_queue/pq_wrapper_impl.hpp" //PQWrapper
#include "../utils/utils.hpp"

namespace ilrd
{

    
class ThreadPool : private boost::noncopyable
{
public:
    class Task  /* : private boost::noncopyable */
    {
    public:
        //generated OK
        virtual ~Task();
        virtual void Do() = 0;
    };

    enum Priority
    {
        LOW = 1,
        NORMAL = 2,
        HIGH = 3,
        FIRST = 4 //do not use this Priority
    };
    enum State
    {
        RUN = 1,
        PAUSE = 2,
        STOP =3
    };

    explicit ThreadPool(int policy_ = 1, size_t thread_count_ = 4, int niceness_ = 0); //throws bad_alloc
    ~ThreadPool() noexcept;
    void Add(boost::shared_ptr<Task> new_task_, Priority priority_ = NORMAL);
    void Run();
    void Pause();
    void Stop(); //destroys thread pool gracefully
    void Resize(size_t new_thread_count_); // throws bad_alloc

private:
    typedef std::pair<Priority, boost::shared_ptr<Task> > p_task_ty;
    typedef boost::unique_lock<boost::mutex> Lock_ty; 
    typedef boost::shared_ptr<boost::thread> thr_shr_ptr_ty;
    bool CmpTasks(const Task& lhs, const Task& rhs) const;


    WaitableQueue<p_task_ty, PQWrapper<p_task_ty, std::vector<p_task_ty> > > m_tasks;
    std::map<boost::thread::id, boost::shared_ptr<boost::thread> > m_threads;

    WaitableQueue<boost::shared_ptr<boost::thread> > m_threads_to_kill;
    int m_niceness;
    int m_policy;

    boost::mutex m_mutex;

    boost::condition_variable m_paused;
    size_t m_num_of_threads;
    boost::atomic<ThreadPool::State> m_state_of_process;
    
    void ThreadInit(size_t thread_count_);
    void ThreadTaskConnect();
    void ThreadKiller();
    void AddPoisonApple(size_t difference);

    class PoisonApple : public ThreadPool::Task
    {
    public:
        PoisonApple(ThreadPool *threads_);
        virtual void Do();
        virtual ~PoisonApple();

    private:
        ThreadPool *m_threads_ref;
    };
};

} // ilrd

#endif // __RD94_THREAD_POOL_HPP__
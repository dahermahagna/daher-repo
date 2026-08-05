#ifndef __THREAD_POOL_IMPL_HPP__
#define __THREAD_POOL_IMPL_HPP__


#include "thread_pool.hpp" //threadpool

namespace ilrd
{

ThreadPool::Task::~Task()
{}

ThreadPool::PoisonApple::PoisonApple(ThreadPool *threads_)
:m_threads_ref(threads_)
{}

ThreadPool::PoisonApple::~PoisonApple()
{}

void ThreadPool::PoisonApple::Do()
{
    thr_shr_ptr_ty thr_to_kill;
    while (0 == thr_to_kill)
    {
        thr_to_kill = m_threads_ref->m_threads[boost::this_thread::get_id()];
    }

    m_threads_ref->m_threads[boost::this_thread::get_id()]->interrupt();
    m_threads_ref->m_threads_to_kill.Push(thr_to_kill);
}

void ThreadPool::ThreadInit(size_t thread_count_)
{
    for (size_t i = 0; thread_count_ > i; ++i)
    {
        thr_shr_ptr_ty th(new boost::thread(boost::bind(&ThreadPool::ThreadTaskConnect, this)));
        m_threads[th->get_id()] = th;
    }
}

ThreadPool::ThreadPool(int policy_, size_t thread_count_, int niceness_)
: m_niceness(niceness_)
, m_policy(policy_)
, m_num_of_threads(thread_count_)
, m_state_of_process(PAUSE)
{
    thr_shr_ptr_ty thr_killer(new boost::thread(boost::bind(&ThreadPool::ThreadKiller, this)));
    m_threads[thr_killer->get_id()] = thr_killer;
    ThreadInit(thread_count_);
}

void ThreadPool::Add(boost::shared_ptr<ThreadPool::Task> new_task_, ThreadPool::Priority priority_)
{
    p_task_ty p_task(priority_, new_task_);
    m_tasks.Push(p_task);
    m_paused.notify_one();
}

ThreadPool::~ThreadPool() noexcept
{
    if(STOP != m_state_of_process)
    {
        Stop();
    }
}

void ThreadPool::Run()
{
    m_state_of_process = RUN;
    m_paused.notify_all();
}

void ThreadPool::ThreadTaskConnect()
{
    while (false == boost::this_thread::interruption_requested())
    {
        {
            Lock_ty lock(m_mutex);
            while (PAUSE == m_state_of_process || (m_tasks.IsEmpty() && RUN == m_state_of_process))
            {
                m_paused.wait(lock);
            }
        }
        p_task_ty task;
        m_tasks.Pop(&task);
        task.second->Do();
    }
} 

void ThreadPool::Pause()
{
    m_state_of_process = PAUSE;
}

void ThreadPool::Stop()
{
    m_state_of_process = STOP;
    m_paused.notify_all();

    std::map<boost::thread::id, thr_shr_ptr_ty>::iterator iter;
    for (iter = m_threads.begin(); m_threads.end() != iter; iter++)
    {
        iter->second->interrupt();
    }
    for (iter = m_threads.begin(); m_threads.end() != iter; iter++)
    {
        iter->second->join();
    }
}

void ThreadPool::ThreadKiller()
{
    thr_shr_ptr_ty thr_to_kill;
    while (false == boost::this_thread::interruption_requested())
    {
        m_threads_to_kill.Pop(&thr_to_kill);
        thr_to_kill->join();
        m_threads.erase(thr_to_kill->get_id());
    }
}

void ThreadPool::AddPoisonApple(size_t difference)
{
    for (size_t i = 0; difference > i; ++i)
    {
        boost::shared_ptr<ilrd::ThreadPool::Task> poison_ptr(new PoisonApple(this));
        Add(poison_ptr, FIRST);
    }
}

void ThreadPool::Resize(size_t new_thread_count_)
{
    int difference = new_thread_count_ - m_num_of_threads;
    if(difference > 0)
    {
        ThreadInit(difference);
    } 
    else
    {
        AddPoisonApple(-difference);
    }
    m_num_of_threads = new_thread_count_;  
}

} // ilrd

#endif // __RD94_THREAD_POOL_HPP__
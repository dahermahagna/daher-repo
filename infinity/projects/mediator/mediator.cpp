
#include <boost/make_shared.hpp> //make share 
#include "mediator.hpp"          // mediator

//reviewed by Ran

namespace ilrd
{


/*----------------- mediator -------------------*/ 

Mediator::Mediator(thread_pool_ptr thread_pool_, factory_create_func_ty create_func_) //:
: m_thread_pool(thread_pool_)
, m_create_func_(create_func_)
, m_reactor()
,m_reactor_run(boost::thread(boost::bind(&Reactor::Run,&m_reactor)))

{
  // empty fix the test: run threadpool from test
}

Mediator::~Mediator() noexcept
{
    m_reactor.Stop();
    m_reactor_run.join();
}

void Mediator::RegisterProxy(fd_ty fd_, proxy_ptr proxy_)
{
    m_proxy_map[fd_] = proxy_;
    m_reactor.Add(fd_, Reactor::READ, boost::bind(&Mediator::GetTaskAddToPool, this, fd_));
}

void Mediator::GetTaskAddToPool(fd_ty fd_)
{
    AddTaskToPool( GetTask( GetCmdData(fd_)));
}

boost::shared_ptr<CmdData> Mediator::GetCmdData(fd_ty fd_)
{
    return m_proxy_map[fd_]->ParseData(fd_);
}

boost::shared_ptr<ThreadPool::Task> Mediator::GetTask(boost::shared_ptr<CmdData> data_)
{
    return boost::make_shared<CmdFactoryTask>(CmdFactoryTask(*data_, m_create_func_));
}

void Mediator::AddTaskToPool(boost::shared_ptr<ThreadPool::Task> cmd_fact_task_)
{
    m_thread_pool->Add(cmd_fact_task_);
}

void Mediator::UnRegisterProxy(fd_ty fd_)
{
    m_proxy_map.at(fd_); // for throwing exception
    m_proxy_map.erase(fd_);
    m_reactor.Remove(fd_,Reactor::READ);
}
/*------------------- proxy base -------------------*/

ProxyBase::~ProxyBase() noexcept
{}

/*---------------  CmdFactory Task -----------------*/

Mediator::CmdFactoryTask::CmdFactoryTask(CmdData& data_, factory_create_func_ty create_func_)
:m_data(data_)
, m_create_func(create_func_)
{
}
Mediator::CmdFactoryTask::~CmdFactoryTask() noexcept
{
}
void Mediator::CmdFactoryTask::Do()
{
    m_create_func(m_data.m_key,m_data.m_args)->Run();
    //write(STDOUT_FILENO, "test100\n",9);
    //std::cout << this->m_data.m_key << std::endl; // this lines were here for the test
    
}

/*------------------ CmdData ---------------------*/ 

CmdData::CmdData(std::string id_, std::vector<char> args_)
:m_key(id_)
,m_args(args_)
{

}


} // namespace ilrd

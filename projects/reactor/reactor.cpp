#include <vector>
#include "reactor.hpp"
#include <iostream>

namespace ilrd
{

Reactor::Reactor()
:m_stop_flag(false)
{
  m_listener = new Select_Listener(this);
}


void Reactor::Add(int fd_, ModeType_ty mode_,boost::function<void()> handler_)
{
  m_listener->AddFd(fd_,mode_);
  fd_mode_ty fd_mode_pair(fd_,mode_);
  m_map[fd_mode_pair] = handler_;
}

void Reactor::Remove(int fd_, ModeType_ty mode_)
{
  m_listener->RemoveFd(fd_,mode_);
  fd_mode_ty fd_mode_pair_to_remove(fd_, mode_);
 
  m_map.at(fd_mode_pair_to_remove); //throwing
  m_map.erase(fd_mode_pair_to_remove); 
}

void Reactor::Run()
{
  m_stop_flag = false;
  //blocking, until all FD's are removed or Stop() called //throws
  std::vector<fd_mode_ty> ready_fd;

  while(false == m_stop_flag)
  {
    m_listener->RunFd(&ready_fd);
    
    while(!ready_fd.empty())
    {

      m_map.find(ready_fd.back())->second();
      ready_fd.pop_back();
    }
  }
}

void Reactor::Stop()
{
  m_stop_flag = true;
}

Reactor::Listener::Listener(Reactor *reactor_)
:m_reactor(reactor_)
{
  assert(NULL != reactor_);
}
Reactor::Listener::~Listener()
{
  //empty
}

void Reactor::Select_Listener::SetFd(int fd_, ModeType_ty mode_)
{
  switch (mode_)
  {
  case READ:
    FD_SET(fd_, &m_master_read_set);
    break;
  case WRITE:
    FD_SET(fd_, &m_master_write_set);
    break;
  case EXCEPTION:
    FD_SET(fd_, &m_master_except_set);
    break;
  }
}

Reactor::Select_Listener::Select_Listener(Reactor *reactor_)
:Listener(reactor_)
,m_max_fd(0)
{
  FD_ZERO(&m_master_read_set);
  FD_ZERO(&m_master_write_set);
  FD_ZERO(&m_master_except_set);
  FD_SET(STDIN_FILENO,&m_master_read_set); // it is used in the stop function
}

inline int MAX(int a, int b)
{
  return a > b ? a : b;
}

void Reactor::Select_Listener::AddFd(int fd_, ModeType_ty mode_)
{
  SetFd(fd_, mode_);
  m_max_fd = MAX(m_max_fd,fd_);
}

void Reactor::Select_Listener::ClrFd(int fd_, ModeType_ty mode_)
{
  switch (mode_)
  {
  case READ:
    FD_CLR(fd_, &m_master_read_set);
    break;
  case WRITE:
    FD_CLR(fd_, &m_master_write_set);
    break;
  case EXCEPTION:
    FD_CLR(fd_, &m_master_except_set);
    break;
  }
}

void Reactor::Select_Listener::RemoveFd(int fd_, Reactor::ModeType_ty mode_)
{
  ClrFd(fd_, mode_);
  
}

void Reactor::Select_Listener::RunFd(std::vector<Reactor::fd_mode_ty> *ready_fd)
{
  int select_result = 0;
  fd_set m_fd_read_set = m_master_read_set;
  fd_set m_fd_write_set = m_master_write_set;
  fd_set m_fd_except_set = m_master_except_set;
  struct timeval tv={5,0};

  select_result = select(m_max_fd + 1,&m_fd_read_set,&m_fd_write_set,&m_fd_except_set,&tv);

  if(0 == select_result)
  {
    write(STDOUT_FILENO, "no fd is ready for 5 second\n",29);
  }
  else if(0 > select_result)
  {
    write(STDOUT_FILENO, "Select fail\n",13);
  }

  else
  {
    
  
  for(int i = 0; m_max_fd + 1 > i; ++i)
  {

    if (FD_ISSET(i, &m_fd_read_set))
    {
      //std::cout << i << std::endl;

      fd_mode_ty fd_to_run(i, READ);
      //m_reactor->m_map.find(fd_to_run)->second();

      ready_fd->push_back(fd_to_run);
    }
    else if (FD_ISSET(i, &m_fd_write_set))
    {
      fd_mode_ty fd_to_run(i, WRITE);
      //m_reactor->m_map.find(fd_to_run)->second();
      ready_fd->push_back(fd_to_run);
    }
    else if (FD_ISSET(i, &m_fd_except_set))
    {
      fd_mode_ty fd_to_run(i, EXCEPTION);
      //m_reactor->m_map.find(fd_to_run)->second();
      ready_fd->push_back(fd_to_run);
    }
  }
  }

}

}

#include <iostream>

#include "mediator.hpp"
#include "factory_impl.hpp"
using namespace ilrd;

class Proxy : public ProxyBase
{
public:
  ~Proxy() noexcept{}
  boost::shared_ptr<CmdData> ParseData(fd_ty fd_)
  {
    (void)fd_;
    return boost::make_shared<CmdData>(CmdData(std::string("it's working!!\n"),std::vector<char>()));
  }
};

int main()
{
  boost::shared_ptr<Proxy> p(new Proxy());
  boost::shared_ptr<ThreadPool> tp(new ThreadPool());
  Factory<Command, std::vector<char>, std::string> factory;
  Mediator m(tp, boost::bind(&Factory<Command, std::vector<char>, std::string>::Create, &factory, _1, _2));

  m.RegisterProxy(0,p);

  sleep(10);

  return 0;
}




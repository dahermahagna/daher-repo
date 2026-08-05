#ifndef __RD94_NBDPROXY_HPP__
#define __RD94_NBDPROXY_HPP__

#include <boost/shared_ptr.hpp> // shared_ptr
#include "../include/framework.hpp"
#include "mediator.hpp"

namespace ilrd
{

class NbdProxy: public ProxyBase
{
public:
    
    NbdProxy(const char *path_, size_t block_size_, size_t number_of_blocks_, int socket_fd, int fd); // may throw bad_alloc
    virtual ~NbdProxy() noexcept;
    
    virtual boost::shared_ptr<CmdData> ParseData(fd_ty fd_);
    static boost::shared_ptr<Command> CreateWriteCmd(std::vector<char> args);
    static boost::shared_ptr<Command> CreateReadCmd(std::vector<char> args);
    static void NbdReply(int fd, const void * block, size_t size);
   
    typedef struct CmdArgs
    {
        int fd;
        boost::function < void (int, const char *, size_t) > reply_func;
        size_t key;
        struct nbd_reply rep;
        char block[4096];
    }CmdArgs;

private:
    enum cmd_type
    {
        READ = 0,
        WRITE
    };
    const char *m_path;
    size_t m_bsize;
    size_t m_bnum;
    int nbd_socket_fd;
    int m_nbd;
    boost::thread run_thread;

    void Connect();

    boost::shared_ptr<CmdData>  CreateData(u_int32_t type, size_t len, size_t key, int socket);

};

}

#endif //__RD94_NBDPROXY_HPP__
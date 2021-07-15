#ifndef __RD94_NBDPROXY_HPP__
#define __RD94_NBDPROXY_HPP__

#include <boost/shared_ptr.hpp> // shared_ptr
#include "../include/framework.hpp"

namespace ilrd
{

    class NbdProxy : public Mediator::ProxyBase
    {
    public:
        NbdProxy(const char *path_, size_t block_size_, size_t number_of_blocks_); // may throw bad_alloc
        void Connect();                                                            // may throw ConnectionFailed exception,
        virtual ~NbdProxy() noexcept;

        const char *GetPath();
        void SetPath(const char *path_);
        size_t GetBlockSize();
        void SetBlockSize(size_t block_size_);
        size_t GetNumOfBlocks();
        void SetNumOfBlocks(size_t number_of_blocks_);

        virtual boost::shared_ptr<CmdData> ParseData(fd_ty fd_);

    private:
        const char *m_path;
        
        size_t m_bsize;
        size_t m_block_num;
        boost::thread m_run_thread;
        int m_nbd_fd;
        FrameWork* m_fw; // to get instance of it must use singleton

        void RunThread();
        void NBDConfig();

        void RegisterProxy();
        void RegisterCmd(std::string id, create_cmd_func_ty func);

        static boost::shared_ptr<Command> CreateWriteCmd(std::vector<char> args);
        static boost::shared_ptr<Command> CreateReadCmd(std::vector<char> args);
    };

}

#endif //__RD94_NBDPROXY_HPP__
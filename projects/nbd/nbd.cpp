#include <cassert>        //assert
#include <string>         //string
#include <string.h>       //strerror
#include <sys/ioctl.h>    //ioctl
#include <sys/types.h>    //socketpair
#include <sys/socket.h>

#include "nbd.hpp"

namespace ilrd
{

NbdProxy::NbdProxy(const char *path_, size_t block_size_, size_t number_of_blocks_)
: m_path(path_)
, m_bsize(block_size_)
, m_block_num(number_of_blocks_)
, m_run_thread()    //boost::bind
, m_nbd_fd(0)
{
    m_fw = Singleton<FrameWork>::GetInstance();
    NBDConfig();
}

void NbdProxy::Connect()
{
    int fds[2];
    int nbd, err;
    const char *DEV_PATH = "/dev/nbd0";
    size_t size = m_bsize * m_block_num; //128 MB

    err = socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    assert(!err);

    m_nbd_fd = open(DEV_PATH, O_CREAT | O_RDWR);
    if (m_nbd_fd == -1)
    {
        std::cout << "Failed to open:" << DEV_PATH
                  << "Is kernel module `nbd' loaded and you have permissions to access the device?\n";
        return ERROR;
    }

    err = ioctl(m_nbd_fd, NBD_SET_SIZE, size);
    assert(err != -1);

    err = ioctl(m_nbd_fd, NBD_CLEAR_SOCK);
    assert(err != -1);

    ServeNbd(fds[0]);

    int sk = fds[1];
    if (ioctl(m_nbd_fd, NBD_SET_SOCK, sk) == -1)
    {
        perror("ioctl(nbd, NBD_SET_SOCK, sk) failed\n");
        exit(EXIT_FAILURE);
    }

    RegisterProxy();

    m_run_thread = boost::thread(boost::bind(RunThread, this));
}

NbdProxy::~NbdProxy() noexcept
{
    m_stop = true;
    m_run_thread.interrupt(); //??
}

const char *NbdProxy::GetPath()
{
    return m_path;
}

size_t NbdProxy::GetBlockSize()
{
    return m_bsize;
}

void NbdProxy::SetBlockSize(size_t block_size_)
{
    m_bsize = block_size_;
}

size_t NbdProxy::GetNumOfBlocks()
{
    return m_block_num;
}

void NbdProxy::SetNumOfBlocks(size_t number_of_blocks_)
{
    m_block_num = number_of_blocks_;
}

void NbdProxy::RunThread()
{
    int err = ioctl(m_nbd_fd, NBD_DO_IT);
    if (err == -1)
    {
        perror("NBD_DO_IT terminated with error");
        exit(EXIT_FAILURE);
    }
}

void NbdProxy::NBDConfig()
{
    system("sudo modprobe nbd");
    system("echo 4 | sudo tee /sys/block/nbd0/queue/max_sectors_kb");

    system("sudo mkfs.ext4 /dev/nbd0");

    Connect();

    std::string mount_cmd("sudo mount /dev/nbd0 ");
    mount_cmd += m_path;
    system(mount_cmd.c_str());
}

void NbdProxy::RegisterProxy()
{
    m_fw->RegisterProxy(m_nbd_fd, this);
}

void NbdProxy::RegisterCmd(std::string id, create_cmd_func_ty func)
{
    m_fw->RegisterCmd(id, func);
}

static boost::shared_ptr<Command> CreateWriteCmd(std::vector<char> args)
{
    /* int bytes_written;

    while (count > 0)
    {
        bytes_written = write(fd, buf, count);
        assert(bytes_written > 0);
        buf += bytes_written;
        count -= bytes_written;
    }
    assert(count == 0);

    return 0; */
}
static boost::shared_ptr<Command> CreateReadCmd(std::vector<char> args)
{
    /* int bytes_read;

    while (count > 0)
    {
        bytes_read = read(fd, buf, count);
        assert(bytes_read > 0);
        buf += bytes_read;
        count -= bytes_read;
    }
    assert(count == 0);

    return 0; */
}


} //end ilrd



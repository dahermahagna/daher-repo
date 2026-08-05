#include <stdio.h>
#include <stdlib.h>     /* system() */
#include <sys/types.h>  /* AF_UNIX */
#include <sys/socket.h> // socketpair
#include <cassert>
#include <sys/ioctl.h> //ioctl
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>    //strerror
#include <linux/nbd.h> //NBD_SET
#include <unistd.h>    //sleep()
#include <boost/thread.hpp>  //boost::thread
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <iostream> 
#define NBD "nbd2"
#define DEV_PATH "/dev/"NBD

void DoIt(int *nbd);


void DoIt(int *nbd)
{
  int err = ioctl(*nbd, NBD_DO_IT);
  if (err == -1)
  {
    perror("NBD_DO_IT terminated with error");
    exit(EXIT_FAILURE);
  }
  std::cout << "do it ends" << std::endl; 
}

static int read_all(int fd, char* buf, size_t count)
{
  int bytes_read;

  while (count > 0) 
  {
    bytes_read = read(fd, buf, count);
    buf += bytes_read;
    count -= bytes_read;
  }


  return 0;
}
static int write_all(int fd, char* buf, size_t count)
{
  int bytes_written;

  while (count > 0) 
  {
    bytes_written = write(fd, buf, count);
  
    buf += bytes_written;
    count -= bytes_written;
  }


  return 0;
}



static void react_nbd(int sk)
{
  u_int64_t from;
  u_int32_t len;
  ssize_t bytes_read;
  struct nbd_request request;
  struct nbd_reply reply;
  void *chunk;
  reply.magic = htonl(NBD_REPLY_MAGIC);
  reply.error = htonl(0);
  std::map < u_int64_t, void *> mesgs;
  while ((bytes_read = read(sk, &request, sizeof(request))) > 0) 
  {
    memcpy(reply.handle, request.handle, sizeof(reply.handle));
    len = ntohl(request.len);
    from = ntohl(request.from);
    
    switch(ntohl(request.type))
    {
      case NBD_CMD_READ:
       // std::cout << "read request of size: "<< len << std::endl;
        chunk = malloc(len);
        if(mesgs.find(from) == mesgs.end())
        {
          memset(chunk, 0 ,len);
          write_all(sk, (char*)&reply, sizeof(struct nbd_reply));
          write_all(sk, (char*)chunk, len);
        }
        else
        {
          write_all(sk, (char*)&reply, sizeof(struct nbd_reply));
          write_all(sk, (char*)mesgs[from], len);
        }

        free(chunk);
        break;
      case NBD_CMD_WRITE:
          //std::cout << "write request of size: "<< len << std::endl;
          chunk = malloc(len);
          read_all(sk, (char*)chunk, len);
          mesgs[from] = chunk;
          write_all(sk, (char*)&reply, sizeof(struct nbd_reply));
          break;

      default:
        std::cout << "default" << std::endl;
          assert(0);
    }
  }
   std::cout << "react ends" << std::endl;  
}

int main(int argc, char *argv[])
{
  system("sudo modprobe nbd");
  system("echo 4 | sudo tee /sys/block/"NBD"/queue/max_sectors_kb");

  int fds[2];
  int nbd, err;
  //size_t block_size_ = 4096UL; // set block size in bytes
  //size_t number_of_blocks_ = 14000;
  size_t size = 134217728UL; //128 MB

  err = socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
  assert(!err);

  
  nbd = open(DEV_PATH, O_CREAT | O_RDWR);
  if (nbd == -1)
  {
    printf(
    "Failed to open `%s':\n"
    "Is kernel module `nbd' loaded and you have permissions "
    "to access the device?\n",
    DEV_PATH);
    return 1;
  }

  
  err = ioctl(nbd, NBD_SET_SIZE, size);
  assert(err != -1);

  err = ioctl(nbd, NBD_CLEAR_SOCK);
  assert(err != -1);

  boost::thread react_thread = boost::thread(react_nbd, fds[0]);

  int sk = fds[1];
  if (ioctl(nbd, NBD_SET_SOCK, sk) == -1)
  {
    perror("ioctl(nbd, NBD_SET_SOCK, sk) failed");
    exit(EXIT_FAILURE);
  }

  boost::thread do_it = boost::thread(DoIt, &nbd);

  system("sudo mkfs.ext4 " DEV_PATH);
  std::cout << "mkfs" << std::endl;

  std::cout << "sudo mount /dev/nbd2 /home/daher/Desktop/user_dir" << std::endl;
  system("sudo mount /dev/nbd2 /home/daher/Desktop/user_dir");
  std::cout << "mount" << std::endl;
  sleep(10);


  close(fds[0]);
  close(fds[1]);
  do_it.join();
  std::cout << "do it join" << std::endl;
  react_thread.join();
  std::cout << "react join" << std::endl;
  (void)argc;
  (void)argv;
  return 0;
}
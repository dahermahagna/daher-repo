#ifndef __RD94_MINION_MANAGER_HPP__
#define __RD94_MINION_MANAGER_HPP__

#include <boost/noncopyable.hpp> // boost::noncopyabl
#include <stdint.h> // uint64_t 
#include <vector> // std::vector
#include <map> // std::map
#include <protocol.hpp> // Request_t, Reply_t

namespace ilrd
{


class Master : private boost::noncopyable
{
public:
    Request_t WrapRequest(uint64_t key,char type, void *block);
    int GetMinion(const Request_t& request);
    

private:
    friend class Singleton<Master>;
    explicit Master(); 
    ~Master() noexcept;
};
    typedef enum error
    {
        NONE = 0,
        NOT_ENOUGH_SPACE,
        CORRUPTION,
        BLOCK_NOT_FOUNED 

    }error_t;

    #pragma pack(1)

    typedef struct Request 
    {
        Request(char type_, uint64_t m_key, uint64_t m_id);
        Request(char type_, uint64_t m_key, uint64_t m_id, void *block);
        unsigned char m_type;
        uint64_t m_key;
        uint64_t m_id;
        char m_block[4096];
    }Request_t;

    typedef struct Reply
    {
        Reply(Request_t& req);
        uint64_t m_id;
        unsigned char m_type;
        error_t m_error;
        char m_block[4096]; 
    }Reply_t;

    #pragma pack()

} // ilrd

#endif //__RD94_MASTER_HPP_
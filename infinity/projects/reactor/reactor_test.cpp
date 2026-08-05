
#include "../../system_programming/p_p_server/ppserver.h"
#include <boost/function.hpp> 
#include <boost/bind.hpp>       //bind
#include "../utils/test.hpp"
#include "reactor.hpp"
#include <cstdio>

//----------- Functions Decleration --------------

test_status_t FirstCaseTest(void);
test_status_t SecondCaseTest(void);

//-------------------- Main ----------------------
using namespace ilrd;
int main()
{
    RUNTEST(FirstCaseTest);
    RUNTEST(SecondCaseTest);
    return 0;
}

//-------------Functions Definetion --------------
void stopF(Reactor *r)
{
    r->Stop();
}
test_status_t FirstCaseTest()
{
    FUNC_TO_TEST(type quit to exit this test);
    
    Reactor ra;
    ra.Add(STDIN_FILENO,Reactor::READ,boost::bind(stopF,&ra));
    
    ra.Run();
    REQUIRE(1==1);
    return PASSED;
}
void TCPHandlerF()
{
    write(STDOUT_FILENO,"i received from tcp client\n",28);
    sleep(1);
}
/* void UDPHandlerF()
{
    write(STDOUT_FILENO, "i received from udp client\n", 28);
    sleep(1);
} */

test_status_t SecondCaseTest()
{
    FUNC_TO_TEST(write the case here);

    Reactor ra;
    struct sockaddr_in tcp_serv, udp_serv;
    int listened = 0;
    int udp_fd = 0;
    int status;
    status = TCPSetBindAndlisten(&tcp_serv, &listened);
    if (SUCCESS != status)
    {
        return FAILED;
    }
    status = UDPSetAndBind(&udp_serv, &udp_fd);
    if (SUCCESS != status)
    {
        return FAILED;
    }

     
    //boost::function<void()> f = &TCPHandler;

    //ra.Add(udp_fd, Reactor::READ, boost::function<void()>(&UDPHandlerF));
    ra.Add(udp_fd, Reactor::READ, boost::bind(UDPHandler, udp_fd));
    //ra.Add(listened, Reactor::READ, boost::function<void()>(&TCPHandlerF));

    ra.Add(STDIN_FILENO, Reactor::READ, boost::bind(stopF, &ra));

    ra.Run();
    sleep(3);
    ra.Run();

    REQUIRE(1 == 1);
    return PASSED;
}

#include <string.h>
#include "dhcp.h"
#include "../utils/test.h"

#define SUCCSESS    	(0)
#define FAILURE         (1)

typedef enum relatives
{
    ZERO,
    ONE,
    NUM_OF_RELATIVES
} relatives;

struct dhcp
{
    Dhcp_node_ty *root;
    size_t num_of_bits_for_network;
    unsigned int sub_net;
};

struct dhcp_node
{
    Dhcp_node_ty *relatives[NUM_OF_RELATIVES];
    int is_full;
};

static test_status_t Test_StageOne(void);
static test_status_t Test_StageTwo(void);
static test_status_t Test_StageThree(void);
static test_status_t Test_StageFour(void);

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	RUNTEST(Test_StageOne);
	printf("\n\n");

	RUNTEST(Test_StageTwo);
	printf("\n\n");

	RUNTEST(Test_StageThree);
	printf("\n\n");

	RUNTEST(Test_StageFour);
	printf("\n\n");
	
    return SUCCSESS;
}


static test_status_t Test_StageOne(void)
{
	char string_ip[13] = {0};
	ip_ty ip = 3288350719;

	REQUIRE(string_ip == IPToString(string_ip, ip));
	REQUIRE(0 == strcmp(string_ip, "196.0.63.255"));

	REQUIRE(ip == StringToIp(string_ip));

    printf("IPToString passed the tests\n");
	printf("StringToIp passed the tests\n");
	printf("------------------------------\n");
    
    return PASSED;
}



static test_status_t Test_StageTwo(void)
{
	Dhcp_node_ty *one_runner = NULL;
	Dhcp_node_ty *zero_runner = NULL;
	ip_ty sub_net = 205521919; 
	size_t num_of_bits_for_network = 28;
	Dhcp_ty *new_dhcp = DhcpCreate(sub_net,num_of_bits_for_network);

	REQUIRE(NULL != new_dhcp);
	REQUIRE(NULL != new_dhcp->root);
	REQUIRE(28 == new_dhcp->num_of_bits_for_network);
	REQUIRE(205521919 == new_dhcp->sub_net);

	REQUIRE(0 == new_dhcp->root->is_full);
	
	one_runner = new_dhcp->root->relatives[ONE];
	zero_runner = new_dhcp->root->relatives[ZERO];
	REQUIRE(NULL != one_runner);
	REQUIRE(0 == one_runner->is_full);
	REQUIRE(NULL != zero_runner);
	REQUIRE(0 == zero_runner->is_full);

	one_runner = one_runner->relatives[ONE];
	zero_runner = zero_runner->relatives[ZERO];
	REQUIRE(NULL != one_runner);
	REQUIRE(0 == one_runner->is_full);
	REQUIRE(NULL != zero_runner);
	REQUIRE(0 == zero_runner->is_full);	

	one_runner = one_runner->relatives[ONE];
	zero_runner = zero_runner->relatives[ZERO];
	REQUIRE(NULL != one_runner);
	REQUIRE(0 == one_runner->is_full);
	REQUIRE(NULL != zero_runner);
	REQUIRE(0 == zero_runner->is_full);

	one_runner = one_runner->relatives[ONE];
	zero_runner = zero_runner->relatives[ZERO];
	REQUIRE(NULL != one_runner);
	REQUIRE(1 == one_runner->is_full);
	REQUIRE(NULL != zero_runner);
	REQUIRE(1 == zero_runner->is_full);		
		
	DhcpDestroy(new_dhcp);
	new_dhcp = NULL;
	
    printf("DhcpCreate passed the tests\n");
	printf("DhcpDestroy passed the tests\n");
	printf("------------------------------\n");
    return PASSED;
}



static test_status_t Test_StageThree(void)
{
	ip_ty sub_net = 3288350704;
	size_t num_of_bits_for_network = 28;
	Dhcp_ty *new_dhcp = NULL;
	Dhcp_ty *new_dhcp2 = NULL;
	ip_ty alloc_ip1 = 0;
	ip_ty alloc_ip2 = 0;
	ip_ty alloc_ip3 = 0;
	ip_ty alloc_ip4 = 0;
	ip_ty alloc_ip5 = 0;
	ip_ty alloc_ip6 = 0;
	ip_ty alloc_ip7 = 0;
	ip_ty alloc_ip8 = 0;
	ip_ty alloc_ip9 = 0;
	ip_ty alloc_ip10 = 0;
	ip_ty alloc_ip11 = 0;
	ip_ty alloc_ip12 = 0;
	ip_ty alloc_ip13 = 0;
	ip_ty alloc_ip14 = 0;

	new_dhcp = DhcpCreate(sub_net,num_of_bits_for_network);
	REQUIRE(NULL != new_dhcp);
	REQUIRE(14 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip1, 0));
	REQUIRE(3288350705 == alloc_ip1);
	REQUIRE(13 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip2, 0));
	REQUIRE(3288350706 == alloc_ip2);
	REQUIRE(12 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip3, 0));
	REQUIRE(3288350707 == alloc_ip3);
	REQUIRE(11 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip4, 0));
	REQUIRE(3288350708 == alloc_ip4);
	REQUIRE(10 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip5, 0));
	REQUIRE(3288350709 == alloc_ip5);
	REQUIRE(9 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip6, 0));
	REQUIRE(3288350710 == alloc_ip6);
	REQUIRE(8 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip7, 0));
	REQUIRE(3288350711 == alloc_ip7);
	REQUIRE(7 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip8, 0));
	REQUIRE(3288350712 == alloc_ip8);
	REQUIRE(6 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip9, 0));
	REQUIRE(3288350713 == alloc_ip9);
	REQUIRE(5 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip10, 0));
	REQUIRE(3288350714 == alloc_ip10);
	REQUIRE(4 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip11, 0));
	REQUIRE(3288350715 == alloc_ip11);
	REQUIRE(3 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip12, 0));
	REQUIRE(3288350716 == alloc_ip12);
	REQUIRE(2 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip13, 0));
	REQUIRE(3288350717 == alloc_ip13);
	REQUIRE(1 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip14, 0));
	REQUIRE(3288350718 == alloc_ip14);
	REQUIRE(0 == DhcpCountFree(new_dhcp));

	REQUIRE(MALLOC_FAILURE == DhcpAllocateIp(new_dhcp, &alloc_ip14, 0));
	REQUIRE(0 == DhcpCountFree(new_dhcp));

	new_dhcp2 = DhcpCreate(sub_net,num_of_bits_for_network);
	REQUIRE(NULL != new_dhcp2);
	REQUIRE(14 == DhcpCountFree(new_dhcp2));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp2, &alloc_ip14,3288350705));
	REQUIRE(3288350705 == alloc_ip14);
	REQUIRE(13 == DhcpCountFree(new_dhcp2));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp2, &alloc_ip13,3288350707));
	REQUIRE(3288350707 == alloc_ip13);
	REQUIRE(12 == DhcpCountFree(new_dhcp2));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp2, &alloc_ip12,3288350707));
	REQUIRE(3288350706 == alloc_ip12);
	REQUIRE(11 == DhcpCountFree(new_dhcp2));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp2, &alloc_ip11,3288350707));
	REQUIRE(3288350708 == alloc_ip11);
	REQUIRE(10 == DhcpCountFree(new_dhcp2));

	DhcpDestroy(new_dhcp);
	new_dhcp = NULL;
	DhcpDestroy(new_dhcp2);
	new_dhcp2 = NULL;

    printf("DhcpAllocateIp passed the tests\n");
	printf("DhcpCountFree passed the tests\n");
	printf("------------------------------\n");
    
    return PASSED;
}


static test_status_t Test_StageFour(void)
{
	ip_ty sub_net = 3288350704;
	size_t num_of_bits_for_network = 28;
	Dhcp_ty *new_dhcp = NULL;
	ip_ty alloc_ip1 = 0;
	ip_ty alloc_ip2 = 0;
	ip_ty alloc_ip3 = 0;
	ip_ty alloc_ip4 = 0;
	ip_ty alloc_ip5 = 0;
	ip_ty alloc_ip6 = 0;
	ip_ty alloc_ip7 = 0;
	ip_ty alloc_ip8 = 0;
	ip_ty alloc_ip9 = 0;
	ip_ty alloc_ip10 = 0;
	ip_ty alloc_ip11 = 0;
	ip_ty alloc_ip12 = 0;
	ip_ty alloc_ip13 = 0;
	ip_ty alloc_ip14 = 0;

	new_dhcp = DhcpCreate(sub_net,num_of_bits_for_network);
	REQUIRE(NULL != new_dhcp);
	REQUIRE(14 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip1, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip2, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip3, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip4, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip5, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip6, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip7, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip8, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip9, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip10, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip11, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip12, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip13, 0));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp, &alloc_ip14, 0));

	REQUIRE(0 == DhcpCountFree(new_dhcp));

	REQUIRE(FREE_FAILURE == DhcpFreeIp(new_dhcp, 0));
	REQUIRE(0 == DhcpCountFree(new_dhcp));
	REQUIRE(FREE_FAILURE == DhcpFreeIp(new_dhcp, 0xF));
	REQUIRE(0 == DhcpCountFree(new_dhcp));

	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip1));
	REQUIRE(1 == DhcpCountFree(new_dhcp));
	REQUIRE(DOUBLE_FREE == DhcpFreeIp(new_dhcp, alloc_ip1));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip2));
	REQUIRE(2 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip3));
	REQUIRE(3 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip4));
	REQUIRE(4 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip5));
	REQUIRE(5 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip6));
	REQUIRE(6 == DhcpCountFree(new_dhcp));
	REQUIRE(DOUBLE_FREE == DhcpFreeIp(new_dhcp, alloc_ip6));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip7));
	REQUIRE(7 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip8));
	REQUIRE(8 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip9));
	REQUIRE(9 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip10));
	REQUIRE(10 == DhcpCountFree(new_dhcp));
	REQUIRE(DOUBLE_FREE == DhcpFreeIp(new_dhcp, alloc_ip10));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip11));
	REQUIRE(11 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip12));
	REQUIRE(12 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip13));
	REQUIRE(13 == DhcpCountFree(new_dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp, alloc_ip14));
	REQUIRE(14 == DhcpCountFree(new_dhcp));

	DhcpDestroy(new_dhcp);
	new_dhcp = NULL;

    printf("DhcpFreeIp passed the tests\n");
	printf("------------------------------\n");
    
    return PASSED;
}



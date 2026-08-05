#include <stdlib.h>
#include "../utils/test.h"
#include "dhcp.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseThreeTest(void);
static test_status_t CaseFourTest(void);
static test_status_t CaseFiveTest(void);

/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(CaseOneTest);
	RUNTEST(CaseTwoTest);
	RUNTEST(CaseThreeTest);
	RUNTEST(CaseFourTest);
	RUNTEST(CaseFiveTest);

	return PASSED; 
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate stage test */
/*
FUNC_TO_TEST(enum dhcp_status
);
FUNC_TO_TEST(dhcp_status);
FUNC_TO_TEST(DhcpCreate);
FUNC_TO_TEST(DhcpDestroy);
FUNC_TO_TEST(DhcpAllocateIp);
FUNC_TO_TEST(IPToString);
FUNC_TO_TEST(StringToIp);
FUNC_TO_TEST(DhcpFreeIp);
FUNC_TO_TEST(DhcpCountFree);
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	char sub_net_str[] = "196.0.0.0";
	size_t num_of_bits_for_network = 29;
	ip_ty sub_net_int = StringToIp(sub_net_str); /* 3288334336 */
	size_t i = 0;

	char ip_str[] = "196.0.0.1";
	ip_ty ip_int = StringToIp(ip_str); /* 3288334337 */
	ip_ty allocated_ip = 0;

	Dhcp_ty *dhcp = DhcpCreate(sub_net_int, num_of_bits_for_network);

	FUNC_TO_TEST(DhcpCreate);
	FUNC_TO_TEST(DhcpDestroy);
	/* DhcpAllocateIp(dhcp, &allocated_ip, 0); */
	REQUIRE(6 == DhcpCountFree(dhcp));
	for (i = 0; i < 5; ++i)
	{
		REQUIRE(SUCCESS == DhcpAllocateIp(dhcp, &allocated_ip, ip_int));
		REQUIRE(ip_int + i == allocated_ip);
	}
	REQUIRE(1 == DhcpCountFree(dhcp));
	REQUIRE(SUCCESS == DhcpAllocateIp(dhcp, &allocated_ip, ip_int + i));
	REQUIRE(ip_int + i == allocated_ip);
	REQUIRE(0 == DhcpCountFree(dhcp));
	REQUIRE(SUCCESS == DhcpFreeIp(dhcp,ip_int));
	REQUIRE(1 == DhcpCountFree(dhcp));
	DhcpDestroy(dhcp);
	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseThreeTest(void)
{
	ip_ty sub_net = 3288350464;
	char *string_ip1 = (char *)malloc(18);
	char *string_ip2 = (char *)malloc(18);
	ip_ty ip1 = 0;
	ip_ty ip2 = 0;
	size_t num_of_bits_for_network = 24;
	Dhcp_ty *new_dhcp1 = DhcpCreate(sub_net, num_of_bits_for_network);
	Dhcp_ty *new_dhcp2 = DhcpCreate(sub_net, 30);

	REQUIRE(NULL != new_dhcp1);
	REQUIRE(NULL != new_dhcp2);

	printf("%s\n", IPToString(string_ip1, sub_net));
	REQUIRE(254 == DhcpCountFree(new_dhcp1));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp1, &sub_net, 0));
	printf("%s\n", IPToString(string_ip1, sub_net));
	ip1 = StringToIp(string_ip1);

	printf("%ld\n", DhcpCountFree(new_dhcp1));

	REQUIRE(253 == DhcpCountFree(new_dhcp1));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp1, &sub_net, 0));
	printf("%s\n", IPToString(string_ip2, sub_net));
	REQUIRE(252 == DhcpCountFree(new_dhcp1));
	ip2 = StringToIp(string_ip2);


	REQUIRE(2 == DhcpCountFree(new_dhcp2));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp2, &sub_net, 0));
	REQUIRE(1 == DhcpCountFree(new_dhcp2));
	REQUIRE(SUCCESS == DhcpAllocateIp(new_dhcp2, &sub_net, 0));
	REQUIRE(0 == DhcpCountFree(new_dhcp2));
	REQUIRE(MALLOC_FAILURE == DhcpAllocateIp(new_dhcp2, &sub_net, 0));

	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp1, ip1));
	printf("126-> %ld\n", DhcpCountFree(new_dhcp2));
	REQUIRE(253 == DhcpCountFree(new_dhcp1));

	REQUIRE(SUCCESS == DhcpFreeIp(new_dhcp1, ip2));
	REQUIRE(254 == DhcpCountFree(new_dhcp1));

	DhcpDestroy(new_dhcp1);
	DhcpDestroy(new_dhcp2);
	free(string_ip1);
	string_ip1 = NULL;
	free(string_ip2);
	string_ip2 = NULL;

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFourTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFiveTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

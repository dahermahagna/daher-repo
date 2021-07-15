#include <stdlib.h> /* malloc   */
#include <assert.h> /* assert   */
#include <stdio.h>  /* sprintf  */
#include <limits.h> /* CHAR_BIT */
#include <math.h>   /* pow      */
#include <string.h> /* memset   */
#include "dhcp.h"

/*------------------------MACRO---------------------------*/

#define IP_LENGHT (32)
#define INDEX_ADJUSTMENT (1)
#define ALL_BITS_ON (0XFFFFFFFF)
#define SUBNET_LENGTH dhcp->num_of_bits_for_network
#define ROOT dhcp->root

/*---------------FUNCTION DECLERATION---------------------*/

static Dhcp_node_ty *DhcpInit(Dhcp_ty *dhcp);
static Dhcp_node_ty *NodeCreate();
static void DestroyByElemnt(Dhcp_node_ty *node);
static int AllocateSpecificIp(Dhcp_node_ty *node, ip_ty req_ip, size_t num_of_bits);
static int IsOccupied(Dhcp_ty *dhcp, ip_ty req_ip, size_t num_of_bits);
static dhcp_status AllocateSmallestIp(Dhcp_node_ty *node, ip_ty *allocated_ip, size_t num_of_bits);
static int GetDirection(Dhcp_node_ty *node);
static void FreeIp(Dhcp_node_ty *node, ip_ty req_ip, size_t num_of_bits);
static void CountOccupied(Dhcp_node_ty *node, size_t num_of_bits, size_t *count);

/*-----------------------TYPEDEF--------------------------*/

typedef enum relatives
{
    ZERO,
    ONE,
    NUM_OF_RELATIVES
} relatives;

typedef enum full
{
    NOT_FULL,
    FULL
} full;
typedef enum eccupide
{
    NOT_OCCUPIED,
    OCCUPIED
} eccupide;

/*-----------------------STRUCTS--------------------------*/

struct dhcp
{
    Dhcp_node_ty *root;
    size_t num_of_bits_for_network;
    ip_ty sub_net;
};
struct dhcp_node
{
    Dhcp_node_ty *relatives[NUM_OF_RELATIVES];
    int is_full;
};

/*--------------------------------------------------------*/

Dhcp_ty *DhcpCreate(ip_ty sub_net, size_t num_of_bits_for_network)
{
    Dhcp_ty *new_dhcp = (Dhcp_ty *)malloc(sizeof(Dhcp_ty));
    if (NULL == new_dhcp)
    {
        return NULL;
    }

    new_dhcp->root = NodeCreate();
    if (NULL == new_dhcp->root)
    {
        free(new_dhcp);
        new_dhcp = NULL;
        return NULL;
    }

    new_dhcp->num_of_bits_for_network = num_of_bits_for_network;
    new_dhcp->sub_net = sub_net;
    DhcpInit(new_dhcp); /*DhcpInit can fail so you should to take care of it here*/

    return new_dhcp;
}

static Dhcp_node_ty *DhcpInit(Dhcp_ty *dhcp)
{
    unsigned int i = 0;
    Dhcp_node_ty *zero_runner = NULL;
    Dhcp_node_ty *one_runner = NULL;
    size_t num_of_bits = 0;
    assert(NULL != dhcp);

    num_of_bits = IP_LENGHT - SUBNET_LENGTH; /*please install a spell checker LENGHT should be length*/

    zero_runner = ROOT;
    one_runner = ROOT;

    while (num_of_bits > i)
    {
        zero_runner->relatives[ZERO] = NodeCreate();
        one_runner->relatives[ONE] = NodeCreate();

        if (NULL == zero_runner || NULL == one_runner)
        {
            DhcpDestroy(dhcp);
            return NULL;
        }
        zero_runner = zero_runner->relatives[ZERO];
        one_runner = one_runner->relatives[ONE];

        /*you dont need it here your NodeCreat function put zero inside is_full, and you need to put 1 just in the last couple, so do it outside of the while loop */
        zero_runner->is_full = !(num_of_bits - i - INDEX_ADJUSTMENT);
        one_runner->is_full = !(num_of_bits - i - INDEX_ADJUSTMENT);
        ++i;
    }

    return ROOT;
}

static Dhcp_node_ty *NodeCreate()
{
    Dhcp_node_ty *new_node = (Dhcp_node_ty *)malloc(sizeof(Dhcp_node_ty));
    if (NULL == new_node)
    {
        return NULL;
    }
    memset(new_node, 0, (sizeof(Dhcp_node_ty)));
    return new_node;
}
/*--------------------------------------------------------*/

void DhcpDestroy(Dhcp_ty *dhcp)
{
    assert(NULL != dhcp);
    DestroyByElemnt(ROOT); /*spell checker Elemnt should be written as*/
    free(dhcp);
    dhcp = NULL;
}

static void DestroyByElemnt(Dhcp_node_ty *node)
{
    if (NULL == node)
    {
        return;
    }
    DestroyByElemnt(node->relatives[ZERO]);
    DestroyByElemnt(node->relatives[ONE]);
    free(node);
    node = NULL;
}
/*--------------------------------------------------------*/

dhcp_status DhcpAllocateIp(Dhcp_ty *dhcp, ip_ty *allocated_ip, ip_ty requested_ip)
{
    size_t num_of_bits = IP_LENGHT - SUBNET_LENGTH;
    ip_ty req_ip = 0;

    int return_status = 0;
    req_ip = requested_ip << SUBNET_LENGTH;
    req_ip >>= SUBNET_LENGTH;

    if (0 == requested_ip || OCCUPIED == IsOccupied(dhcp, req_ip, num_of_bits))
    {
        return_status = AllocateSmallestIp(ROOT, allocated_ip, num_of_bits);
        *allocated_ip >>= (int)SUBNET_LENGTH;
        *allocated_ip |= dhcp->sub_net;
    }
    else
    {
        return_status = AllocateSpecificIp(ROOT, req_ip, num_of_bits);
        if (SUCCESS == return_status)
        {
            *allocated_ip = dhcp->sub_net | req_ip;
        }
    }
    return return_status;
}

static int AllocateSpecificIp(Dhcp_node_ty *node, ip_ty req_ip, size_t num_of_bits)
{
    int direction = req_ip >> (num_of_bits - INDEX_ADJUSTMENT) & 1;
    if (0 == num_of_bits)
    {
        node->is_full = FULL;
        return SUCCESS;
    }
    if (NULL == node->relatives[direction])
    {
        node->relatives[direction] = NodeCreate();
        if (NULL == node->relatives[direction])
        {
            return MALLOC_FAILURE;
        }
    }
    if (SUCCESS != AllocateSpecificIp(node->relatives[direction], req_ip, num_of_bits - INDEX_ADJUSTMENT))
    {
        return MALLOC_FAILURE;
    }

    /*take this "IF" out to function, to make it more readable, as well to reuse it in AllocateSmallestIp */
    if ((NULL != node->relatives[ZERO] && FULL == node->relatives[ZERO]->is_full) &&
        (NULL != node->relatives[ONE] && FULL == node->relatives[ONE]->is_full))
    {
        node->is_full = FULL;
    }
    return SUCCESS;
}

static int IsOccupied(Dhcp_ty *dhcp, ip_ty req_ip, size_t num_of_bits)
{
    Dhcp_node_ty *runner = NULL;

    assert(NULL != dhcp);

    runner = ROOT;
    while (0 < num_of_bits && NULL != runner)
    {
        if (FULL == runner->is_full)
        {
            return OCCUPIED;
        }
        runner = runner->relatives[req_ip >> (num_of_bits - INDEX_ADJUSTMENT) & 1];
        --num_of_bits;
    }
    return num_of_bits == 0 ? OCCUPIED : NOT_OCCUPIED;
}

static dhcp_status AllocateSmallestIp(Dhcp_node_ty *node, ip_ty *allocated_ip, size_t num_of_bits)
{
    int direction = 0;

    if (FULL == node->is_full)
    {
        return MALLOC_FAILURE;
    }

    if (0 == num_of_bits)
    {
        node->is_full = FULL;
        return SUCCESS;
    }

    direction = GetDirection(node);

    if (NULL == node->relatives[direction])
    {
        node->relatives[direction] = NodeCreate();
        if (NULL == node->relatives[direction])
        {
            return MALLOC_FAILURE;
        }
    }

    if (SUCCESS == AllocateSmallestIp(node->relatives[direction], allocated_ip, num_of_bits - INDEX_ADJUSTMENT))
    {
        *allocated_ip >>= 1;
        *allocated_ip |= (direction << (IP_LENGHT - INDEX_ADJUSTMENT));
        if (FULL == node->relatives[ZERO]->is_full && (NULL != node->relatives[ONE] && FULL == node->relatives[ONE]->is_full))
        {
            node->is_full = FULL;
        }
        return SUCCESS;
    }

    return MALLOC_FAILURE;
}

static int GetDirection(Dhcp_node_ty *node)
{
    assert(node != NULL);

    if (NULL == node->relatives[ZERO] || 0 == node->relatives[ZERO]->is_full)
    {
        return ZERO;
    }
    else
    {
        return ONE;
    }
}

/*--------------------------------------------------------*/

char *IPToString(char *str_ip, ip_ty ip)
{
    unsigned int byte1 = ip >> 3 * 8 & 0xff;
    unsigned int byte2 = ip >> 2 * 8 & 0xff;
    unsigned int byte3 = ip >> 8 & 0xff;
    unsigned int byte4 = ip & 0xff;

    assert(NULL != str_ip);

    sprintf(str_ip, "%u.%u.%u.%u", byte1, byte2, byte3, byte4);
    return str_ip;
}

/*--------------------------------------------------------*/

ip_ty StringToIp(char *ip)
{
    ip_ty ip_num = 0, byte_num = 0;

    assert(NULL != ip);

    while (*ip != '\0')
    {
        byte_num = 0;

        while (*ip != '.' && *ip != '\0')
        {
            byte_num = byte_num * 10 + (*ip - '0');
            ++ip;
        }

        ip_num <<= 8;
        ip_num += byte_num;
        if (*ip != '\0')
        {
            ++ip;
        }
    }
    return ip_num;
}

/*--------------------------------------------------------*/

dhcp_status DhcpFreeIp(Dhcp_ty *dhcp, ip_ty ip)
{
    size_t num_of_bits = 0;
    ip_ty req_ip = 0;
    unsigned int brodcast = 0;

    assert(NULL != dhcp);

    num_of_bits = IP_LENGHT - SUBNET_LENGTH;
    brodcast = ALL_BITS_ON >> SUBNET_LENGTH;

    req_ip = ip << SUBNET_LENGTH;
    req_ip >>= SUBNET_LENGTH;

    if (req_ip == 0 || req_ip == brodcast)
    {
        return FREE_FAILURE;
    }
    if (!IsOccupied(dhcp, req_ip, num_of_bits))
    {
        return DOUBLE_FREE;
    }
    FreeIp(ROOT, req_ip, num_of_bits);
    return SUCCESS;
}

static void FreeIp(Dhcp_node_ty *node, ip_ty req_ip, size_t num_of_bits)
{
    /*assert*/
    int direction = req_ip >> (num_of_bits - INDEX_ADJUSTMENT) & 1;
    if (0 == num_of_bits)
    {
        node->is_full = NOT_FULL;
        return;
    }

    FreeIp(node->relatives[direction], req_ip, num_of_bits - INDEX_ADJUSTMENT);
    node->is_full = NOT_FULL;
}
/*--------------------------------------------------------*/

size_t DhcpCountFree(const Dhcp_ty *dhcp)
{
    size_t num_of_bits = 0;
    size_t num_of_total_ips = 0;
    size_t count = 0;

    assert(NULL != dhcp);
    num_of_bits = IP_LENGHT - SUBNET_LENGTH;
    num_of_total_ips = pow(2, num_of_bits);

    CountOccupied(ROOT, num_of_bits, &count);
    return num_of_total_ips - count;
}

static void CountOccupied(Dhcp_node_ty *node, size_t num_of_bits, size_t *count)
{
    
    if (NULL == node)
    {
        return;
    }
    if (0 == num_of_bits && FULL == node->is_full)
    {
        ++*count;
        return;
    }
    CountOccupied(node->relatives[ZERO], num_of_bits - INDEX_ADJUSTMENT, count);
    CountOccupied(node->relatives[ONE], num_of_bits - INDEX_ADJUSTMENT, count);
}
/*--------------------------------------------------------*/

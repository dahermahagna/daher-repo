/******************************************************************************			 
 *                 Exercise: dhcp                                               *
 *                 Developer: Eli                                             *
 *                 Reviwer:                                                   *
 *                 Date:                                                      *
 ******************************************************************************/

/******************************************************************************
 *                                 HEADERS                                    * 
 ******************************************************************************/
#include <assert.h> /* assert                 */
#include <stdlib.h> /* malloc, atoi           */
#include <string.h> /* memset, strtok, strdup */
#include <stdio.h>  /* sprintf                */

#include "dhcp.h"


/******************************************************************************
 *                             MACROS & ENUMS                                 * 
 ******************************************************************************/
#define BYTE_SIZE (8)
#define HAS_TWO_CHILDREN(node) (NULL != node->relatives[ZERO] && NULL != node->relatives[ONE])

/******************************************************************************
 *                        TYPE DEFINITIONS & STRUCTS                          * 
 ******************************************************************************/

typedef enum relatives
{
    ZERO,
    ONE,
    NUM_OF_RELATIVES
} relatives_ty;

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

/******************************************************************************
 *                         FUNCTION DECLARATIONS                              * 
 ******************************************************************************/
static Dhcp_node_ty *CreateNodeImp(void);
static void DestroyNodeImp(Dhcp_node_ty *node);

static int InitSavedAddressesImp(Dhcp_ty *dhcp);
static void RecDestoryImp(Dhcp_node_ty *node);

static int AllocRequestedIpImp(Dhcp_ty *dhcp, Dhcp_node_ty *node, ip_ty *allocated_ip, ip_ty requested_ip, size_t height);
static int AllocFirstFreeIpImp(Dhcp_ty *dhcp, Dhcp_node_ty *node, ip_ty *allocated_ip, size_t height);
static relatives_ty GetNextStepImp(Dhcp_node_ty *node);

static dhcp_status FreeIpImp(Dhcp_ty *dhcp, Dhcp_node_ty *node, ip_ty ip_to_free, size_t height);
static size_t CountTakenHostsImp(Dhcp_node_ty *node, size_t height);

static void UpdateFullnessImp(Dhcp_node_ty *node);
static char *StrDupImp(const char *src);

/******************************************************************************
 *                         FUNCTIONS DEFINITIONS                              * 
 ******************************************************************************/
/**************************** DhcpCreate ****************************/
Dhcp_ty *DhcpCreate(ip_ty sub_net, size_t num_of_bits_for_network)
{
    int res = 0;
    Dhcp_ty *new_dhcp = (Dhcp_ty *)malloc(sizeof(Dhcp_ty));
    if (NULL == new_dhcp)
    {
        ERR_MALLOC();
        return NULL;
    }

    new_dhcp->num_of_bits_for_network = num_of_bits_for_network;
    new_dhcp->sub_net = sub_net;

    new_dhcp->root = CreateNodeImp();
    if (NULL == new_dhcp->root)
    {
        DhcpDestroy(new_dhcp);
        return NULL;
    }

    res = InitSavedAddressesImp(new_dhcp);
    if (0 != res)
    {
        DhcpDestroy(new_dhcp);
        return NULL;
    }

    return new_dhcp;
}

/**************************** DhcpDestroy ***************************/
void DhcpDestroy(Dhcp_ty *dhcp)
{
    assert(NULL != dhcp);

    RecDestoryImp(dhcp->root);
    memset(dhcp, 0, sizeof(Dhcp_ty));
    free(dhcp);
    dhcp = NULL;
}

/**************************** DhcpAllocateIp ************************/
dhcp_status DhcpAllocateIp(Dhcp_ty *dhcp, ip_ty *allocated_ip, ip_ty requested_ip)
{
    int res = 0;

    assert(NULL != dhcp);
    assert(NULL != allocated_ip);
    assert(0 == dhcp->root->is_full);

    *allocated_ip = 0;

    if (0 != requested_ip)
    {
        res = AllocRequestedIpImp(dhcp, dhcp->root, allocated_ip, requested_ip, INT_BITS - dhcp->num_of_bits_for_network);
        if (0 == res)
        {
            return SUCCESS;
        }
    }

    res = AllocFirstFreeIpImp(dhcp, dhcp->root, allocated_ip, INT_BITS - dhcp->num_of_bits_for_network);
    if (0 == res)
    {
        return SUCCESS;
    }

    return MALLOC_FAILURE;
}

/**************************** IPToString ****************************/
char *IPToString(char *str_ip, ip_ty ip)
{
    ip_ty nibble = 0;
    int curr_byte = 3;
    char *runner = NULL;

    assert(NULL != str_ip);

    runner = str_ip;

    while (0 <= curr_byte)
    {
        nibble = (ip >> BYTE_SIZE * curr_byte--) & ONE_BYTE_MASK;

        sprintf(runner, "%d.", nibble);

        while ('.' != *runner)
        {
            ++runner;
        }
        ++runner;
    }

    *(--runner) = '\0';

    return str_ip;
}

/**************************** StringToIp ****************************/
ip_ty StringToIp(char *ip)
{
    ip_ty ip_num = 0;
    ip_ty nibble = 0;
    int curr_byte = 3;
    char *token = NULL;
    char *dup_ip = StrDupImp(ip); /* DM the idea of using strtok is good, but to use it you have tou make DUP for the string so you can do it without */

    assert(NULL != ip);

    token = strtok(dup_ip, ".");

    while (NULL != token)
    {
        nibble = atoi(token);
        nibble <<= BYTE_SIZE * curr_byte--;
        ip_num |= nibble;
        token = strtok(NULL, ".");
    }

    free(dup_ip);
    dup_ip = NULL;

    return ip_num;
}

/**************************** DhcpRFreeIp ***************************/
dhcp_status DhcpFreeIp(Dhcp_ty *dhcp, ip_ty ip)
{
    dhcp_status res;

    assert(NULL != dhcp);

    res = FreeIpImp(dhcp, dhcp->root, ip, INT_BITS - dhcp->num_of_bits_for_network);

    return res;
}

/**************************** DhcpCountFree *************************/
size_t DhcpCountFree(const Dhcp_ty *dhcp)
{
    size_t total_hosts = 0;
    size_t taken_hosts = 0;

    assert(NULL != dhcp);

    total_hosts = 1 << (INT_BITS - dhcp->num_of_bits_for_network);

    taken_hosts = CountTakenHostsImp(dhcp->root, INT_BITS - dhcp->num_of_bits_for_network);

    return total_hosts - taken_hosts;
}

/******************************************************************************
 *                          AUXILIARY FUNCTIONS                               * 
 ******************************************************************************/
/**************************** CreateNodeImp *************************/
static Dhcp_node_ty *CreateNodeImp(void)
{
    Dhcp_node_ty *new_node = (Dhcp_node_ty *)malloc(sizeof(Dhcp_node_ty));
    if (NULL == new_node)
    {
        ERR_MALLOC();
        return NULL;
    }

    memset(new_node, 0, sizeof(Dhcp_ty));

    return new_node;
}

/**************************** DestroyNodeImp ************************/
static void DestroyNodeImp(Dhcp_node_ty *node)
{
    assert(NULL != node);

    memset(node, 0, sizeof(Dhcp_node_ty)); /* DM why do you memset the node before the free?*/ 
    free(node);
    node = NULL;
}

/**************************** InitSavedAddressesImp *****************/
static int InitSavedAddressesImp(Dhcp_ty *dhcp)
{
    size_t heigt = 0;
    Dhcp_node_ty *left_runner = NULL;
    Dhcp_node_ty *right_runner = NULL;

    assert(NULL != dhcp);

    heigt = INT_BITS - dhcp->num_of_bits_for_network;
    left_runner = dhcp->root;
    right_runner = dhcp->root;

    while (0 < heigt--)
    {
        /* DM you can do it with one if
        left_runner->relatives[ZERO] = CreateNodeImp();
        right_runner->relatives[ONE] = CreateNodeImp();
        if(NULL == left_runner->relatives[ZERO] || right_runner->relatives[ONE]) 
        {
            return -1;
        }
        left_runner = left_runner->relatives[ZERO];
        right_runner = right_runner->relatives[ONE]
        */

        left_runner->relatives[ZERO] = CreateNodeImp();
        if (NULL != left_runner->relatives[ZERO])
        {
            left_runner = left_runner->relatives[ZERO];

            right_runner->relatives[ONE] = CreateNodeImp();
            if (NULL != right_runner->relatives[ONE])
            {
                right_runner = right_runner->relatives[ONE];
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }

    left_runner->is_full = 1;
    right_runner->is_full = 1;

    return 0;
}

/**************************** RecDestoryImp *************************/
static void RecDestoryImp(Dhcp_node_ty *node)
{
    if (NULL == node)
    {
        return;
    }

    RecDestoryImp(node->relatives[ZERO]);
    RecDestoryImp(node->relatives[ONE]);
    DestroyNodeImp(node);
}

/**************************** AllocRequestedIpImp *******************/
static int AllocRequestedIpImp(Dhcp_ty *dhcp, Dhcp_node_ty *node, ip_ty *allocated_ip, ip_ty requested_ip, size_t height)
{
    int res = -1;
    relatives_ty next_step = 0;

    if (NULL == node)
    {
        return res;
    }

    if (0 == height)
    {
        if (0 == node->is_full)
        {
            node->is_full = 1;
            *allocated_ip = requested_ip;

            res = 0;
        }
        return res;
    }

    next_step = (requested_ip >> height) & ONE_MASK;
    if (NULL == node->relatives[next_step])
    {
        node->relatives[next_step] = CreateNodeImp();
        if (NULL == node->relatives[next_step])
        {
            return -1;
        }
    }

    res = AllocRequestedIpImp(dhcp, node->relatives[next_step], allocated_ip, requested_ip, height - 1);
    UpdateFullnessImp(node);

    return res;
}

/**************************** AllocFirstFreeIpImp *******************/
static int AllocFirstFreeIpImp(Dhcp_ty *dhcp, Dhcp_node_ty *node, ip_ty *allocated_ip, size_t height)
{
    int res = -1;
    relatives_ty next_step = 0;

    if (NULL == node)
    {
        return res;
    }

    if (0 == height)
    {
        if (0 == node->is_full)
        {
            node->is_full = 1;
            *allocated_ip |= dhcp->sub_net;

            res = 0;
        }
        return res;
    }

    next_step = GetNextStepImp(node);
    *allocated_ip <<= 1;
    *allocated_ip |= next_step;

    if (NULL == node->relatives[next_step])
    {
        node->relatives[next_step] = CreateNodeImp();
        if (NULL == node->relatives[next_step])
        {
            return res;
        }
    }

    res = AllocFirstFreeIpImp(dhcp, node->relatives[next_step], allocated_ip, height - 1);
    UpdateFullnessImp(node);

    return res;
}

/**************************** GetNextStepImp ************************/
static relatives_ty GetNextStepImp(Dhcp_node_ty *node)
{
    assert(NULL != node);

    if (NULL == node->relatives[ZERO] || 0 == node->relatives[ZERO]->is_full)
    {
        return ZERO;
    }

    return ONE;
}

/**************************** GetNextStepImp ************************/
static dhcp_status FreeIpImp(Dhcp_ty *dhcp, Dhcp_node_ty *node, ip_ty ip_to_free, size_t height)
{
    dhcp_status res;
    relatives_ty next_step;

    assert(ip_to_free != dhcp->sub_net);
    assert(ip_to_free != (dhcp->sub_net | (-1U >> dhcp->num_of_bits_for_network)));

    if (NULL == node)
    {
        return FREE_FAILURE;
    }

    if (0 == height)
    {
        if (1 == node->is_full)
        {
            node->is_full = 0;

            return SUCCESS;
        }

        return DOUBLE_FREE;
    }

    next_step = (ip_to_free >> height) & ONE_MASK;
    res = FreeIpImp(dhcp, node->relatives[next_step], ip_to_free, height - 1);

    UpdateFullnessImp(node);

    return res;
}

/**************************** CountTakenHostsImp ********************/
static size_t CountTakenHostsImp(Dhcp_node_ty *node, size_t height)
{
    if (NULL == node)
    {
        return 0;
    }

    if (1 == node->is_full)
    {
        return 1 << height; /* wooow its soo good */ 
    }

    return CountTakenHostsImp(node->relatives[ZERO], height - 1) + CountTakenHostsImp(node->relatives[ONE], height - 1);
}

/**************************** UpdateFullnessImp *********************/
static void UpdateFullnessImp(Dhcp_node_ty *node)
{
    if (!HAS_TWO_CHILDREN(node))
    {
        node->is_full = 0;
        return;
    }

    if (0 == node->relatives[ZERO]->is_full || 0 == node->relatives[ONE]->is_full) /* this may cause segmentation fault if the relative[zero] or [one] is null*/
    {
        node->is_full = 0;
        return;
    }

    node->is_full = 1;
}

/**************************** StrDupImp *****************************/
static char *StrDupImp(const char *src)
{
    char *dst = malloc(strlen(src) + 1); /*this malloc will be freed in another function, because of that i think that better not to use strtok*/ 
    if (dst == NULL)
        return NULL;
    strcpy(dst, src);
    return dst;
}
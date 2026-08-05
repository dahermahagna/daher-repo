/******************************************************************************			 
 *                 Exercise: dhcp                                               *
 *                 Developer: Eli                                             *
 *                 Reviwer:                                                   *
 *                 Date:                                                      *
 ******************************************************************************/
#ifndef __DHCP_H__
#define __DHCP_H__

/******************************************************************************
 *                                 HEADERS                                    * 
 ******************************************************************************/
#include <stddef.h>      /* size_t */

/******************************************************************************
 *                     TYPE DEFINITIONS, STRUCTS & ENUMS                      * 
 ******************************************************************************/
typedef struct dhcp Dhcp_ty;
typedef struct dhcp_node Dhcp_node_ty;
typedef unsigned int ip_ty;

typedef enum dhcp_status
{
    SUCCESS,
    DOUBLE_FREE,
    FREE_FAILURE,
    MALLOC_FAILURE
} dhcp_status;

/******************************************************************************
 *                          FUNCTION DECLARATIONS                             * 
 ******************************************************************************/
/* DESCRIPTION: 
 * creates new dhcp
 * 
 * @complexity: O(k), k is the number of not occupied bits. 
 *  
 * @param
 * sub_net - represents the network ip
 * num_of_bits_for_network - number represents the occupied bits by network ip
 * 
 * @return
 * pointer to the new dhcp or NULL if failed         
 */
Dhcp_ty *DhcpCreate(ip_ty sub_net, size_t num_of_bits_for_network);

/* DESCRIPTION: 
 * destroys given dhcp - free all allocated memory
 * 
 * @complexity: O(n)
 *  
 * @param
 * dhcp - pointer to the dhcp to delete
 * 
 * @return          
 */
void DhcpDestroy(Dhcp_ty *dhcp);

/* DESCRIPTION: 
 * allocates new ip for the device.
 * may allocate specific requested ip,
 * if taken will find the smallest ip address
 * if requested ip not specified ('0') or already occupied 
 * or not part of network address - the user will get the smallest ip address
 * 
 * Undefined behaviour when DhcpCountFree() returns zero
 *
 * @complexity: O(k) - k is the number of not occupied bits.
 *  
 * @param
 * dhcp - pointer to the dhcp
 * requested_ip - the requested ip 
 * allocated_ip - the ip that been allocated
 *
 * @return 
 * SUCCESS - for success allocate.
 * MALLOC_FAILURE - failed to allocate new ip     
 */
dhcp_status DhcpAllocateIp(Dhcp_ty *dhcp, ip_ty *allocated_ip, ip_ty requested_ip);

/*DESCRIPTION: 
 * converts ip to string
 * undifiend behavior if not enough space in str_ip:
 * at least 16 chars.
 *
 * @complexity: O(1)
 * 
 * @param
 * str_ip - pointer to allocated string
 * ip - ip to convert
 *
 * @return
 * pointer to str with converted ip
 */
char *IPToString(char *str_ip, ip_ty ip);

/*DESCRIPTION: 
 * converts string to ip
 * undifiend behavior if ip is invalid
 *
 * @complexity: O(1)
 * 
 * @param
 * ip - pointer to allocated string
 *       
 * @return
 * ip_ty - converted ip
 */
ip_ty StringToIp(char *ip);

/*DESCRIPTION: 
 * free given ip from dhcp, 
 * Undifiend behavior if ip to free is reserved IP
 *
 * @complexity: O(k) - k is the number of not occupied bits.
 * 
 * @param
 * Dhcp - pointer to Dhcp
 * ip - ip to free
 *
 * @return
 * SUCCESS - for success free.
 * DOUBLE_FREE - if the ip allready free
 * FREE_FAILURE - if not exists or reserved
 */
dhcp_status DhcpFreeIp(Dhcp_ty *dhcp, ip_ty ip);

/*DESCRIPTION: 
 * return the number of free ips in the dhcp the not located yet
 *
 * @complexity: O(n)
 * 
 * @param
 * dhcp - pointer to dhcp
 * 
 * @return
 * the number of free ips
 */
size_t DhcpCountFree(const Dhcp_ty *dhcp);

#endif /*__DHCP_H__*/
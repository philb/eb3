/**
 * \addtogroup apps
 * @{
 */

/**
 * \defgroup INTERNET
 * @{
 *
 */

/**
 * \file
 *         Header file for Internet
 *
 * \author
 *         Phil Blundel
 */

#ifndef __INTERNET_H__
#define __INTERNET_H__


#include <stdint.h>


/*---- Definitions -------------------------------------------------*/
#define EcCb_ARP			0xa1
#define EcCb_ARPreply		0xa2
#define EcCb_Frame		0x81

#define MY_SERVER_TYPE "INTERNET"
#define MY_SERVER_NAME "TCP/IP Gateway"
#define WILDCARD_SERVER_TYPE "        "




/*---- Functions ---------------------------------------------------*/

extern void internet_init(void);
extern void handle_ip_packet(uint8_t cb, uint16_t length, uint8_t offset);
uint8_t forward_to_econet (void);
extern void handle_port_b0(void);

#endif /* __INTERNET_H__ */
/** @} */
/** @} */

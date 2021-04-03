 /*
 *
 * This file is part of the Econet<>Ethernet firmware.
 *
 * $Id: internet.c,v 1.20 2010-01-23 13:26:04 markusher Exp $
 *
 */

/*---- Includes ----------------------------------------------------*/
#include "internet.h"
#include "nic.h"
#include "serial.h"
#include "adlc.h"
#include "globals.h"
#include "uip_arp.h"
#include <string.h>


/*---- Variables ---------------------------------------------------*/
static uip_ipaddr_t econet_subnet, econet_netmask;

/*---- Structures --------------------------------------------------*/
struct ethip_hdr {
  struct uip_eth_hdr ethhdr;
  /* IP header. */
  u8_t vhl,
    tos,
    len[2],
    ipid[2],
    ipoffset[2],
    ttl,
    proto;
  u16_t ipchksum;
  u16_t srcipaddr[2],
    destipaddr[2];
};

struct ec_arp {
  u16_t srcipaddr[2],
    dstipaddr[2];
};

#define IPBUF ((struct ethip_hdr *)&uip_buf[0])


/*******************************************************************
*
* NAME 	:	internet_init          
*
* DESCRIPTION :
*
* INPUTS 	:	Nothing
*
* OUTPUTS 	:	Nothing
*
* NOTES 	:   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL/
*
*/

void 
internet_init(void)
{
  uip_ipaddr(econet_subnet, eeprom.EconetIP[0], eeprom.EconetIP[1], eeprom.EconetIP[2], eeprom.EconetIP[3]);
  uip_ipaddr(econet_netmask, eeprom.EconetMask[0], eeprom.EconetMask[1], eeprom.EconetMask[2], eeprom.EconetMask[3]);
}

/*******************************************************************
*
* NAME 	:	do_send_mbuf          
*
* DESCRIPTION :
*
* INPUTS 	:	Pointer to a memory buffer
*
* OUTPUTS 	:	Nothing
*
* NOTES 	:   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL/
*
*/

void 
do_send_mbuf(struct mbuf *mb)
{
  mb->data[2] = eeprom.Station;
  mb->data[3] = 0;
  mb->data[5] = IP_PORT;
  enqueue_tx (mb);
}

/*******************************************************************
*
* NAME	:	handle_ip_packet            
*
* DESCRIPTION :
*
* INPUTS 	:	control byte, length, offset 
*
* OUTPUTS 	:
*
* NOTES 	:   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL/
*
*/

void 
handle_ip_packet(uint8_t cb, uint16_t length, uint8_t offset)
{
  struct arp_entry *tabptr;
  struct ec_arp *arpbuf = (struct ec_arp *)ECONET_RX_BUF + offset;
  switch (cb) {
  case EcCb_Frame:
    length -= offset;
    if (length <= (UIP_BUFSIZE - UIP_LLH_LEN))
    {
      memcpy (uip_buf + UIP_LLH_LEN, ECONET_RX_BUF + offset, length);
      uip_len = length;
      uip_arp_out ();
      nic_send (NULL);
    }
    break;
  case EcCb_ARP:
    if (uip_ipaddr_cmp(econet_subnet, arpbuf->dstipaddr)) {
      serial_tx_str ("arp me\r\n");
      struct mbuf *mb = mbuf_alloc ();
      struct ec_arp *arpbuf2 = (struct ec_arp *)&mb->data[6];
      uip_ipaddr_copy (arpbuf2->dstipaddr, arpbuf->srcipaddr);
      uip_ipaddr_copy (arpbuf2->srcipaddr, arpbuf->dstipaddr);
      mb->data[0] = ECONET_RX_BUF[2];
      mb->data[1] = ECONET_RX_BUF[3];
      mb->data[4] = EcCb_ARPreply;
      do_send_mbuf (mb);
    }
    break;
  case EcCb_ARPreply: 
    tabptr = find_arp_entry (arpbuf->srcipaddr);
    if (tabptr == NULL) {
      tabptr = find_arp_victim ();
      memcpy(tabptr->ipaddr, arpbuf->srcipaddr, 4);
    }
    tabptr->ethaddr.addr[0] = ECONET_RX_BUF[2];
    tabptr->ethaddr.addr[1] = ECONET_RX_BUF[3];
    tabptr->time = arptime;
    break;
  }
}

/*******************************************************************
*
* NAME :            
*
* DESCRIPTION :
*
* INPUTS :
*
* OUTPUTS :
*
* NOTES :   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL/
*
*/

uint8_t 
forward_to_econet (void)
{
  /* Check if the destination address is on the local network. */
  if (eeprom.EconetMask[0] && uip_ipaddr_maskcmp(IPBUF->destipaddr, econet_subnet, econet_netmask)) {
    struct arp_entry *arp = find_arp_entry (IPBUF->destipaddr);
    if (arp) {
      struct mbuf *mb = copy_to_mbufs (uip_buf + UIP_LLH_LEN - 6, uip_len - UIP_LLH_LEN + 6);
      mb->data[0] = arp->ethaddr.addr[0];
      mb->data[1] = arp->ethaddr.addr[1];
      mb->data[4] = EcCb_ARP;
      do_send_mbuf (mb);
    } else {
      struct mbuf *mb = mbuf_alloc();
      uint16_t *p = (uint16_t *)(mb->data + 6);
      uip_ipaddr_copy (p + 0, econet_subnet);
      uip_ipaddr_copy (p + 2, IPBUF->destipaddr);
      mb->data[0] = mb->data[1] = 0xff;
      mb->data[4] = EcCb_ARP;
      mb->length = 14;
      do_send_mbuf (mb);
    }
    return 1;
  }

  return 0;
}

/*******************************************************************
*
* NAME 	:	handle_port_b0            
*
* DESCRIPTION :	handle an incoming broadcast for Find Server Port
*			This will return the defined IP_PORT to the requestor
*
* INPUTS 	:	Nothing
*
* OUTPUTS 	:	Nothing
*
* NOTES :   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL/
*
*/

void 
handle_port_b0(void)
{
  unsigned char *bcast_buf = ECONET_RX_BUF + 6;

  if (memcmp (bcast_buf, MY_SERVER_TYPE, 8) == 0
      || memcmp (bcast_buf, WILDCARD_SERVER_TYPE, 8) == 0)
    {
      struct mbuf *mb = mbuf_alloc();	// get a mbuf

      unsigned char *response_buffer = &mb->data[0];
      response_buffer[0] = ECONET_RX_BUF[2];
      response_buffer[1] = ECONET_RX_BUF[3];
      response_buffer[2] = eeprom.Station;
      response_buffer[3] = 0;
      response_buffer[4] = 0x80;
      response_buffer[5] = FIND_SERVER_REPLY_PORT;
      response_buffer[6] = 0;
      response_buffer[7] = IP_PORT;
      response_buffer[8] = 1;
      strcpy ((char *)response_buffer + 9, MY_SERVER_TYPE);	// Server Type
      response_buffer[17] = strlen(MY_SERVER_NAME);		// lenght of Server Name
      strcpy ((char *)response_buffer + 18, MY_SERVER_NAME);// Server Name

      mb->length = 18 + strlen(MY_SERVER_NAME);			// set mbuf data length
      enqueue_tx (mb);				// add to the Tx queue
    }
}

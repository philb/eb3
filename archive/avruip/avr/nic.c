/*****************************************************************************
*  Module Name:       NIC Driver Interface for uIP-AVR Port
*
*  Created By:        Louis Beaudoin (www.embedded-creations.com)
*
*  Original Release:  November 16, 2003
*
*  Module Description:
*  Provides three functions to interface with a NIC driver
*  These functions can be called directly from the main uIP control loop
*  to send packets from uip_buf and uip_appbuf, and store incoming packets to
*  uip_buf
*
*
*****************************************************************************/

#include "nic.h"
#include "serial.h"
#include "mbuf.h"
#include "aun.h"		// for declaration of MNSDATAPORT in debugging
#include <string.h>

#define IP_TCP_HEADER_LENGTH 		40
#define ETHERNET_HEADER_LENGTH	0x0E
#define TOTAL_HEADER_LENGTH (IP_TCP_HEADER_LENGTH + ETHERNET_HEADER_LENGTH)

//#define DEBUG 1
#define IP_MF   0x20


static struct frag_mbuf frag_mbuf;

extern uint8_t cs8900_poll_init (void);
extern void cs8900_poll_send (uint16_t length);
extern void cs8900SendPacketData (struct mbuf *mb, uint16_t length);
extern void cs8900_poll_send_end (void);
extern unsigned int cs8900_poll_retrieve (void);
extern void cs8900RetreivePacketData (struct mbuf *mb, uint16_t length);


void nic_init(void)
{
	NICInit();
}

/******************************************************************************
*
* NAME	: uip_to_mbufs          
*
* DESCRIPTION : 
*
* INPUTS	: Nothing
*
* OUTPUTS	: Pointer to an mbuf
*
* NOTES	:   
*
* CHANGES	:
* REF NO	DATE		WHO	DETAIL
*					
*/

struct mbuf *uip_to_mbufs(void)
{
	struct mbuf *mb;
	
	if (uip_len <= TOTAL_HEADER_LENGTH) {
		mb = copy_to_mbufs (uip_buf, uip_len);
	} 
	else 
	{
  		mb = copy_to_mbufs (uip_buf, TOTAL_HEADER_LENGTH);

		struct mbuf *mb2 = copy_to_mbufs (uip_appdata, uip_len - TOTAL_HEADER_LENGTH);
		struct mbuf *mbp = mb;

		while (mbp->next) {
			mbp = mbp->next;
		}
		mbp->next = mb2;
		mb2->prev = mbp;
	}
	return mb;
}

/******************************************************************************
*
* NAME	:	send_frag        
*
* DESCRIPTION : 
*
* INPUTS	: 
*
* OUTPUTS	: 	Nothing
*
* NOTES	:   
*
* CHANGES	:
* REF NO	DATE		WHO	DETAIL
*					
*/

static void 
send_frag(struct mbuf *mb, uint16_t length)
{


#ifdef DEBUG
/*
	serial_tx_str ("xmit ");
	serial_tx_hex (length >> 8);
	serial_tx_hex (length);
	serial_crlf ();
*/
if (*(mb->data+0x22) == HTONS(MNSDATAPORT)){	// only output AUN packets
	serial_tx_str ("eth tx ");
	serial_packet((unsigned short)&mb->data+42, length-42);	// write out the payload
	serial_crlf ();					// add extra CR LF for spacing
	}
//	memset (&mb->data[0], 0xff, 6);
#endif

//	memset (&mb->data[0], 0xff, 6);

	NICBeginPacketSend(length);
	while (length) {
		uint16_t this_length = mb->length;
		if (this_length > length)
			this_length = length;
            NICSendPacketData((struct mbuf *) &mb->data, this_length);
		length -= this_length;
            mb = mb->next;
      }
	NICEndPacketSend();
}


/******************************************************************************
*
* NAME	:	nic_send	          
*
* DESCRIPTION :	Sends data from the memory buffer and sends on the NIC
*
* INPUTS	:	pointer to mbuf
*
* OUTPUTS	:	Nothing
*
* NOTES	:   
*
* CHANGES	:
* REF NO	DATE		WHO	DETAIL
*					
*/

void 
nic_send(struct mbuf *mb)
{
	if (mb == NULL) {
		mb = uip_to_mbufs();
	}

	memcpy (&frag_mbuf.data[0], mb->data, UIP_IPH_LEN + UIP_LLH_LEN);

      uint16_t length = 0, offset = 0;
      struct mbuf *mbp = mb, *this_mb = mb;
      while (mbp) {
		if ((length + mbp->length) >= UIP_BUFSIZE) {
			/* packet needs fragmenting */
			struct uip_tcpip_hdr *BUF = (struct uip_tcpip_hdr *)&this_mb->data[UIP_LLH_LEN];
			uint16_t payload_length = length - (UIP_IPH_LEN + UIP_LLH_LEN);
			uint8_t overdone = payload_length & 7;
			length -= overdone;
			send_frag (this_mb, length);
			memcpy (&frag_mbuf.data[UIP_IPH_LEN + UIP_LLH_LEN], mb->prev->data + mb->prev->length - overdone, overdone);
			length = UIP_IPH_LEN + UIP_LLH_LEN + overdone;
			frag_mbuf.length = length;
			frag_mbuf.next = mbp;
			this_mb = (struct mbuf *)&frag_mbuf;
			offset += (payload_length - overdone);
			BUF->ipoffset[0] = ((offset / 8) >> 8) | IP_MF;
			BUF->ipoffset[1] = (offset / 8);
		}
		length += mbp->length;
		mbp = mbp->next;
        }

	struct uip_tcpip_hdr *BUF = (struct uip_tcpip_hdr *)&this_mb->data[UIP_LLH_LEN];
	BUF->ipoffset[0] &= ~IP_MF;
	send_frag (this_mb, length);

	mbuf_free_chain (mb);
}


/******************************************************************************
*
* NAME	:	nic_poll	          
*
* DESCRIPTION : 
*
* INPUTS	: 
*
* OUTPUTS	:	Nothing
*
* NOTES	:   
*
* CHANGES	:
* REF NO	DATE		WHO	DETAIL
*					
*/

#if UIP_BUFSIZE > 255
unsigned short nic_poll(void)
#else
unsigned char nic_poll(void)
#endif /* UIP_BUFSIZE > 255 */
{
	unsigned int packetLength;

	packetLength = NICBeginPacketRetreive();

	// if there's no packet or an error - exit without ending the operation
	if( !packetLength )
		return 0;

	// drop anything too big for the buffer
	if( packetLength > UIP_BUFSIZE )
	{
		NICEndPacketRetreive();
		return 0;
	}

	// copy the packet data into the uIP packet buffer
	NICRetreivePacketData((struct mbuf *) &uip_buf, packetLength );

#ifdef DEBUG

if (*(uip_buf+0x22) == HTONS(MNSDATAPORT)){		// only output AUN packets
	serial_tx_str ("eth rx ");
	serial_packet((unsigned short)&uip_buf+42, packetLength-42);	// write out the payload
}
#endif

	NICEndPacketRetreive();

#if UIP_BUFSIZE > 255
	return packetLength;
#else
	return (unsigned char)packetLength;
#endif /* UIP_BUFSIZE > 255 */

}

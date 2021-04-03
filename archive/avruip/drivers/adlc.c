#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "adlc.h"
#include "aun.h"
#include "serial.h"
#include "uip.h"
#include "mbuf.h"
#include "internet.h"
#include "bridge.h"

#define BUF ((struct uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])

#define MAX_TX			8
#define ADLC_TX_RETRY_COUNT	16
#define ADLC_TX_RETRY_DELAY	128

#define ECONET_DATA_FRAME	0x00
#define SCOUT_FRAME		0x01

#define NORMAL_PACKET		0
#define BROADCAST			1
#define IMMEDIATE			2

#define NO_BUFFER			0xFF

#define DEBUG 			1

unsigned char ECONET_RX_BUF[ECONET_RX_BUF_SIZE];
volatile register unsigned char adlc_state asm ("r15");

static struct stats stats;

/*---- Set up an array of transmit records ----*/
struct tx_record tx_buf[MAX_TX];

static struct scout_mbuf scout_mbuf;

extern uint16_t get_adlc_rx_ptr (void);

extern uint8_t get_adlc_state (void);
extern uint8_t adlc_await_idle (void);
extern void adlc_tx_frame (struct mbuf *mb, unsigned char type);

static uip_ipaddr_t ip_target;
static uint8_t aun_cb, aun_port;


/*******************************************************************
*
* NAME : get_tx_buf           
*
* DESCRIPTION :
*
* INPUTS : Nothing
*
* OUTPUTS : counter of tx_buf
*
* NOTES :   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL
*
*/

static uint8_t get_tx_buf (void)
{
  uint8_t i;
  for (i = 0; i < MAX_TX; i++)
    {
      if (tx_buf[i].mb == NULL)
	return i;
    }
  return NO_BUFFER;
}

/******************************************************************************
*
* NAME	: do_tx_packet          
*
* DESCRIPTION : Send a data packet on the Econet network. This is passed by a 
*			pointer to a tx_record. A scout packet is created and sent.
*
* INPUTS	: Pointer to a tx_record
*
* OUTPUTS	: counter of do_tx_packet
*
* NOTES	:   
*
* CHANGES	:
* REF NO	DATE		WHO	DETAIL
*					
*/

static uint8_t do_tx_packet (struct tx_record *tx) __attribute__ ((noinline));

static uint8_t do_tx_packet (struct tx_record *tx)
{
  unsigned char *buf = tx->mb->data;
  unsigned char type = NORMAL_PACKET;

/*---- set the tx type to BROADCAST or IMMEDIATE  ---------------------------*/

  if (buf[0] == ANY_NETWORK)
    type = BROADCAST;
  else if (buf[5] == 0)
    type = IMMEDIATE;

  stats.tx_attempts++;					// update tx stats

/*---- wait for Econet to become idle ----*/

  if (adlc_await_idle ())
    {
      stats.tx_line_jammed++;				// update line jammed stats
      return LINE_JAMMED;				// return status LINE_JAMMED
    }

/* if it is a BROADCAST Tx, transmit a scout frame, await a reply ----
  Sends eight bytes direct from transmit control block to every station with a 
  RECEIVE block set for station &00, and the appropriate port number */

  if (type == BROADCAST)
    {
      adlc_tx_frame (tx->mb, SCOUT_FRAME);	// transmit a scout frame
      adlc_ready_to_receive_scout ();		// receive a scout frame
      return TX_OK;					// return status Tx OK
    }								// END type == BROADCAST


/*---- handle IMMEDIATE Tx packets  ----*/

  uint8_t extra_len = 0, do_4way = 0;		// Set up extra_length & 4way handshake flag

  // immediate OP with length>=14 has 8 extra bytes
  // also control byte 2 POKE, 3 JSR, 4 UPC, 5 ??? requires a 4 way handshake
  
  if (type == IMMEDIATE && tx->mb->length >= 14)
    {								
      extra_len = 8;					// set the extra packet length
      uint8_t cb = buf[4] & RXCB;			// get the Control Byte & RXCB (removes the top bit)
      if (cb >= Econet_Peek && cb <= Econet_Imm05)			// Poke, JSR, UPC or 5?
	do_4way = 1;					// set 4way handshake flag
    }

  memcpy (scout_mbuf.data, tx->mb->data, 6 + extra_len);
  scout_mbuf.length = 6 + extra_len;

/*
#ifdef DEBUG
  serial_tx_str ("eco2");
  serial_tx_str ("tx ");
  serial_packet (&scout_mbuf.data, scout_mbuf.length);

#endif
*/

  adlc_tx_frame ((struct mbuf *) &scout_mbuf, SCOUT_FRAME);	// transmit a scout frame


  unsigned char state;
  do
    {
      state = get_adlc_state ();
    }
  while (state != RX_IDLE && state != (RX_SCOUT_ACK | FRAME_COMPLETE));

  if (state == RX_IDLE)
    {
      adlc_ready_to_receive_scout ();
      return NOT_LISTENING;
    }

  struct mbuf *mb = tx->mb;

  if (do_4way)
    {
      memcpy (scout_mbuf.data + 4, mb->data + 14, mb->length - 14);
      scout_mbuf.length = mb->length - 10;
      scout_mbuf.next = mb->next;
      mb = (struct mbuf *)&scout_mbuf;
    }

  if (type == NORMAL_PACKET || do_4way)
    {

/*
#ifdef DEBUG
      serial_tx_str ("eco3");
      serial_tx_str ("tx ");
      serial_packet (mb->data, scout_mbuf.length);
#endif
*/
      adlc_tx_frame (mb, ECONET_DATA_FRAME);

      do
	{
	  state = get_adlc_state ();
	}
      while (state != RX_IDLE && state != (RX_DATA_ACK | FRAME_COMPLETE));
    }

  scout_mbuf.next = NULL;

  if (type == IMMEDIATE && state != RX_IDLE)
    {
      uint16_t frame_length = get_adlc_rx_ptr () - (int) ECONET_RX_BUF;
      if (frame_length > 4)
	{
	  tx->r_mb = copy_to_mbufs (ECONET_RX_BUF + 4, frame_length - 4);
	}
    }

  adlc_ready_to_receive_scout ();

  return ((state & 0xf) == FRAME_COMPLETE) ? TX_OK : NET_ERROR;
}

/******************************************************************************
*
* NAME :            
*
* DESCRIPTION :
*
* INPUTS : Pointer to a mbuf
*
* OUTPUTS :
*
* NOTES :   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL
*
*/

uint8_t
enqueue_tx (struct mbuf * mb)
{
  uint8_t i = get_tx_buf ();
  if (i != NO_BUFFER)
    {
      struct tx_record *tx = &tx_buf[i];
      tx->retry_count = ADLC_TX_RETRY_COUNT;
      tx->retry_timer = 0;
      tx->mb = mb;
      tx->is_aun = 0;
      return i + 1;
    }

#ifdef DEBUG
  serial_tx_str ("no bufs!\n");
#endif

  return 0;
}

/*******************************************************************
*
* NAME :            
*
* DESCRIPTION :
*
* INPUTS : Pointer to a mbuf, Pointer to a TCP/IP header, handle
*
* OUTPUTS :
*
* NOTES :   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL
*
*/

uint8_t
enqueue_aun_tx (struct mbuf * mb, struct uip_tcpip_hdr * hdr, uint32_t handle)
{
  uint8_t i = enqueue_tx (mb);
  if (i)
    {
      struct tx_record *tx = &tx_buf[i - 1];
      tx->is_aun = 1;
      tx->requestor_handle = handle;
      uip_ipaddr_copy (tx->requestor_ip, hdr->srcipaddr);
      uip_ipaddr_copy (tx->target_ip, hdr->destipaddr);
      tx->retry_count = 0;
    }
  return i;
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
* REF NO    DATE    WHO     DETAIL
*
*/

static uint8_t should_bridge (struct scout_packet *s, uint32_t * ip_targetp)
{

  /* if destination is reachable fill ip_target address */

  if (rTableEthType[s->DNet] == ROUTABLE)	// is the destination Network routable on Ethernet
    {
      *ip_targetp = rTableEthIP[s->DNet] | ((unsigned long) (s->DStn) << 24); // set ip target address
      return 1;	// is routable
    }
  else
    {
      return 0;
    }

}

/*******************************************************************
*
* NAME : make_scout_acknowledge           
*
* DESCRIPTION : Sets the source station.net and dest station.net in 
*               a scout packet from the received packet
*
* INPUTS : None
*
* OUTPUTS : None
*
* NOTES :  scout_mbuf is a static, global to adlc.c
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL
*
*/

static void make_scout_acknowledge (void) __attribute__ ((noinline));

static void
make_scout_acknowledge (void)
{
  scout_mbuf.data[0] = ECONET_RX_BUF[2];	// destination station = source station
  scout_mbuf.data[1] = ECONET_RX_BUF[3];	// destination network = source network
  scout_mbuf.data[2] = ECONET_RX_BUF[0];	// source station = destination station
  scout_mbuf.data[3] = ECONET_RX_BUF[1];	// source network = destination network
}

/*******************************************************************
*
* NAME : make_and_send_scout           
*
* DESCRIPTION : create a scout packet and send it
*
* INPUTS :	None
*
* OUTPUTS : None
*
* NOTES :	scout_mbuf is a static, global to adlc.c  
*		The control byte and port byte for the scout packet are 
*		added in the adlc_tx_frame routine
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL
*
*/

static void
make_and_send_scout (void)
{
  make_scout_acknowledge ();	// make the ack packet
  scout_mbuf.length = 4;	// set the current length

  adlc_tx_frame ((struct mbuf *) &scout_mbuf, SCOUT_FRAME);	// transmit buffer, type
}

/*******************************************************************
*
* NAME 	:	do_local_immediate          
*
* DESCRIPTION :	do an immediate operation
*
* INPUTS 	:	control byte
*
* OUTPUTS	:	nothing
*
* NOTES	:	Only Machine Type reply currently implemented
*
* CHANGES	:
* REF NO    DATE    WHO     DETAIL
*
*/

static void do_local_immediate (uint8_t cb)
{
  switch (cb & RXCB)			// Mask the top bit
    {
    case Econet_MachineType:		// Returns code to indicate machine type and NFS
						// version of remote station. 
      make_scout_acknowledge ();
      scout_mbuf.data[4] = (unsigned char)((MACHINE_TYPE_IGATEWAY & 0xFF00) >> 8);	// MACHINE_TYPE
      scout_mbuf.data[5] = (unsigned char) (MACHINE_TYPE_IGATEWAY & 0x00FF);	// MACHINE_VENDOR
      scout_mbuf.data[6] = MACHINE_VER_LOW;
      scout_mbuf.data[7] = MACHINE_VER_HIGH;
      scout_mbuf.length = AUNHDRSIZE;

      adlc_tx_frame ((struct mbuf *) &scout_mbuf, SCOUT_FRAME);
      break;
    }
}

/*******************************************************************
*
* NAME	:	adlc_poller           
*
* DESCRIPTION :	Checks the ADLC status to see if any packets have been
*			received or any are due for transmission
*
* INPUTS	:	None
*
* OUTPUTS	:	None
*
* NOTES 	:   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL
*
*/

void adlc_poller (void)
{
  if (adlc_state == RX_IDLE)				// if the ADLC is idle (not receiving)
    {

    /*---- loop through the Tx records held in the tx_buf array ----*/

      uint8_t i;
      for (i = 0; i < MAX_TX; i++)
	{
	  struct tx_record *tx = &tx_buf[i];	// get a pointer to the next record
	  if (tx->mb)					// is a mbuf assigned to the tx record?
	    {
	      if (tx->retry_timer == 0)		// check the tx record retry timer is 0
								// otherwise subtract 1
		{
		  uint8_t state = do_tx_packet (tx);// send the tx record to the transmit routine
		  switch (state)				// check the transmission result
		    {
		    default:
		      if (tx->retry_count--)
			{
			  tx->retry_timer = ADLC_TX_RETRY_DELAY;	// set the delay timer
			  break;
			}
		    case TX_OK:						// Tx operation was a success
		    case LINE_JAMMED:					// The line was jammed
		      if (tx->is_aun)					// was Tx an AUN operation?
			{
			  aun_tx_complete (state, tx);		
			}
		      mbuf_free_chain (tx->mb);			// clear the mbuf chain of the data
		      tx->mb = NULL;					// clear the Tx record
		      if (tx->r_mb)					
			{
			  mbuf_free_chain (tx->r_mb);
			  tx->r_mb = NULL;
			}
		      break;
		    }
		}
	      else
		tx->retry_timer--;				// subtract 1 from the retry timer
	    }
	}
    }
  else if ((adlc_state & 0x0F) == FRAME_COMPLETE)	// The ADLC has picked up a Rx packet
    {
      uint16_t frame_length = get_adlc_rx_ptr () - (int) ECONET_RX_BUF; // get the frame length
      stats.frames_in++;					// update Rx stats

/*
#ifdef DEBUG
      serial_tx_str ("eco ");
      serial_tx_str ("rx ");
      serial_packet (ECONET_RX_BUF, frame_length);
#endif
*/

      if (adlc_state == (RX_SCOUT | FRAME_COMPLETE))	// Scout packet received?
	{
	  if (frame_length < sizeof(struct scout_packet))// shorter than a scout packet should be?
	    {
	      stats.short_scouts++;			// update stats
	      goto out;					// quit
	    }

	  struct scout_packet *s = (struct scout_packet *) ECONET_RX_BUF;

	  if (s->DStn == ANY_STATION)			// if broadcast scout received
	    {
	      stats.rx_bcast++;				// update stats

	      if (s->Port == FIND_SERVER_PORT)	// incoming request for find server port?
		handle_port_b0 ();			// return the IP_PORT

	      if (s->Port == IP_PORT)			// incoming request on IP_PORT?
		handle_ip_packet (ECONET_RX_BUF[4], frame_length, 6);

	      if (s->Port == BRIDGE_PORT)		// incoming request for Bridge Port?
		handle_port_9c (frame_length);

	      else						// Otherwise treat as a data packet and broadcast
		{						// to AUN without the first 6 bytes
		  memcpy (uip_appdata + AUNHDRSIZE, ECONET_RX_BUF + 6, frame_length - 6);
//		  aun_send_broadcast (s, frame_length - 6);	// does nothing. Empty procedure
		}
	    }
	  else if (should_bridge (s, (uint32_t *) &ip_target[0]))	// check if the packet should be forwarded
	    {
	      if (s->Port == IMMEDIATE_PORT)		// If Port 00 for an immediate OP
		{
		  aun_send_immediate (s, (uint32_t *) (ECONET_RX_BUF + 6), frame_length - 6);	// forward data to AUN
		  return;
		}
		// otherwise it wasn't the IMMEDIATE_PORT
	    do_rx_data:
	      aun_cb = s->ControlByte;		// set Control Byte
	      aun_port = s->Port;			// set Port
	      make_and_send_scout ();			// send Scout ACK on Econet
	      adlc_ready_to_receive (RX_DATA);	// set ADLC ready for data
	      return;
	    }
	  else if (s->DStn == eeprom.Station && s->DNet == LOCAL_NETWORK) // packet for this device
	    {
	      if (s->Port == IMMEDIATE_PORT)	// immediate OP ?
		{
		  do_local_immediate (s->ControlByte);
		}
	      else if (s->Port == IP_PORT)		// operation on IP_PORT ?
		{
		  goto do_rx_data;
		}
	    }
	}
      else if (adlc_state == (RX_DATA | FRAME_COMPLETE))	// data Rx completed
	{
	  if (aun_port == IP_PORT)				// data for IP_PORT ?
	    {
	      handle_ip_packet (aun_cb, frame_length, 4);
	      make_and_send_scout ();
	      goto out;
	    }
	  if ((frame_length - 4) >
	      (UIP_BUFSIZE - ((char *) uip_appdata - (char *) uip_buf)))	// too big for buffer?
	    {
	      serial_tx_str ("too big!");
	      serial_crlf ();
	    }
	  else
	    {
	      memcpy (uip_appdata + AUNHDRSIZE, ECONET_RX_BUF + 4,
		      frame_length - 4);				// copy data from Econet Buffer to Ethernet
	      aun_send_packet (aun_cb, aun_port,
			       *((uint16_t *) (ECONET_RX_BUF + 2)), ip_target,
			       frame_length - 4);			// forward data on AUN
	      adlc_state = BUSY_FORWARDING;				// flag adlc as busy
	      return;
	    }
	}
    out:
      adlc_ready_to_receive_scout ();				// set ADLC as ready to Rx Scout
    }
}

/*******************************************************************
*
* NAME	:	adlc_forwarding_complete            
*
* DESCRIPTION :	If the result is TX_OK, send a scout ACK.
*
* INPUTS	:	result, ptr to buffer, length
*
* OUTPUTS	:	nothing
*
* NOTES	:	   
*
* CHANGES :
* REF NO    DATE    WHO     DETAIL
*
*/

void
adlc_forwarding_complete (uint8_t result, uint8_t * buffer, uint8_t length)
{
  if (result == TX_OK)
    {
      make_scout_acknowledge ();			// make scout ACK
      if (length > 12)					// trim length to max 12 bytes
	length = 12;
      memcpy (&scout_mbuf.data[4], buffer, length);	// copy buffer to scout buffer +4
      scout_mbuf.length = 4 + length;		// set length of packet

/*
#ifdef DEBUG
      serial_tx_str ("eco6");
      serial_tx_str ("tx ");
      serial_packet (&scout_mbuf.data, scout_mbuf.length + 2);
#endif
*/
      adlc_tx_frame ((struct mbuf *) &scout_mbuf, SCOUT_FRAME);	// Tx Scout ACK
    }

  adlc_ready_to_receive_scout ();
}

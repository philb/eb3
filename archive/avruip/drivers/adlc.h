/**
 * \addtogroup apps
 * @{
 */

/**
 * \defgroup ADLC ADLC_ wrapper
 * @{
 *
 */

/**
 * \file
 *         Header file for ADLC driver
 *
 * \author
 *         Mark Usher
 */

#ifndef __ADLC_H__
#define __ADLC_H__

#ifndef __ASSEMBLER__

	#include <stdint.h>
	#include "globals.h"
	#include "mbuf.h"
	#include "uip.h"

	struct scout_packet
	{
	  unsigned char DStn;
	  unsigned char DNet;
	  unsigned char SStn;
	  unsigned char SNet;
	  unsigned char ControlByte;
	  unsigned char Port;
	};

	struct scout_mbuf
	{
	  struct mbuf *next, *prev;
	  uint8_t length;
	  uint8_t pad;
	  uint8_t data[120];
	};

	struct stats
	{
	  int frames_in;
	  int short_scouts;
	  int rx_bcast;
	  int tx_attempts;
	  int tx_collision;
	  int tx_not_listening;
	  int tx_net_error;
	  int tx_line_jammed;
	};

	struct rx_control
	{
	  uint8_t stn, net, cb, port;
	};

	struct rx_record
	{
	  uint8_t state, port, cb, stn, net, rx_port;
	  int len;
	  union
	  {
	    unsigned char *buf;
	    void (*callback) (int);
	  }
	  d;
	};

	struct tx_record
	{
	  struct mbuf *mb;			// the first mbuf for the Tx data
	  unsigned char retry_count;		// Tx Retry count
	  unsigned char retry_timer;		// Tx Retry timer
	  unsigned char is_aun;			// AUN operation flag
	  uip_ipaddr_t requestor_ip;		// source IP
	  uip_ipaddr_t target_ip;		// destination IP
	  uint32_t requestor_handle;		// handle
	  struct mbuf *r_mb;			// requestor mbuf ?
	};

	extern void adlc_poller(void);
	extern void adlc_ready_to_receive(uint8_t what);
	extern void adlc_ready_to_receive_scout(void);
	extern uint8_t setup_rx(uint8_t port, uint8_t stn, uint8_t net, unsigned char *ptr, unsigned int length);
	extern uint8_t setup_sync_rx(uint8_t port, uint8_t stn, uint8_t net, void (*callback)(int));
	extern uint8_t poll_rx(uint8_t i, struct rx_control *rxc);
	extern void close_rx(uint8_t i);
	extern uint8_t enqueue_tx(struct mbuf *mb);
	extern uint8_t enqueue_aun_tx(struct mbuf *mb, struct uip_tcpip_hdr *hdr, uint32_t handle);
	extern volatile short adlc_rx_ptr;
	extern void adlc_forwarding_complete(uint8_t result, uint8_t *buffer, uint8_t length);
	extern uint16_t my_station;

#endif /* __ASSEMBLER__ */

#define TX_OK		0
#define LINE_JAMMED	1
#define NOT_LISTENING	2
#define NET_ERROR		3
#define COLLIDED        4
#define NO_CLOCK        5

#define RX_IDLE		0
#define RX_CHECK_NET1	1
#define RX_CHECK_NET2	2
#define RX_PAYLOAD	3
#define FRAME_COMPLETE  15

#define RX_SCOUT        (0 << 4)
#define RX_SCOUT_ACK    (1 << 4)
#define RX_DATA         (2 << 4)
#define RX_DATA_ACK     (3 << 4)

#define DISCONTINUED    64
#define BUSY_FORWARDING	65

#define RXCB_INVALID	0
#define RXCB_READY	1
#define RXCB_RECEIVING	2
#define RXCB_RECEIVED	3
#define RXCB		0x7F
#define TXCB		0x80

#define IMMEDIATE_PORT	0x00
#define BRIDGE_PORT    	0x9C
#define FIND_SERVER_PORT	0xB0
#define FIND_SERVER_REPLY_PORT	0xB1
#define IP_PORT		0xD2



// pin definitions
#define ADLC_RS1		0			// PD0 Register Select 1			Econet Pin6
#define ADLC_RnW		1			// PD1 Read/Write Control			Econet Pin2
#define ADLC_RS0		4			// PD4 Register Select 0			Econet Pin5
#define ADLC_nCE		0			// PE0						Econet Pin3
#define ADLC_D0		2			// PE2						Econet Pin7

#define ECONET_RX_BUF_SIZE	(4096+4)

#ifndef __ASSEMBLER__
	extern unsigned char ECONET_RX_BUF[ECONET_RX_BUF_SIZE];
#endif /* __ASSEMBLER__ */

#endif /* __ADLC_H__ */
/** @} */
/** @} */

/*
 * Copyright (c) 2001, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Adam Dunkels.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: main.c,v 1.31 2010-01-23 16:30:25 markusher Exp $
 *
 */

#include <string.h>
#include "compiler.h"

#include "globals.h"


#include "uip.h"
#include "uip_arp.h"
#include "nic.h"

#include "timer.h"
#include "clock.h"

#include "egpio.h"
#include "serial.h"
#include "adlc.h"
#include "internet.h"
#include "bridge.h"
#include "mbuf.h"
#include "eeprom.h"
#include "telnetd.h"
#include "aun.h"


extern void adlc_irq(void);
extern void adlc_access(void);
extern void adlc_init(void);


#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */


void AVR_init(void)
{
	// select 8MHz clock
	CLKPR = 0x80;
	CLKPR = 0x0;

	// set up I/O
	DDRB = 0xFF;
	DDRE = (1 << ADLC_D0 | 1 << ADLC_nCE);
	DDRD = 0xF3;
	PORTD = 0xFF;
	PORTE = (1 << ADLC_nCE);

	// set up timer 0 to generate ADLC clock waveform
	OCR0 = 4;
	TCCR0 = (1 << WGM01) | (1 << COM00) | (1 << CS00);

	// set up timer 1 to generate Econet clock output
	/* to do - add the multiplier parameter in CMOS to configure speed */
	TCCR1A = (1 << WGM11) | (1 << COM1A1);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10); // no prescaling

	// select 1 wait state for upper region at 0x8000-0xffff
	EMCUCR = (1 << SRL2);

	// set the MicroController Control Register
	MCUCR =  (1 << SRE) | (1 << ISC11) | (1 << SRW10);

}

static void maybe_send(void) __attribute__((noinline));
static void maybe_send(void)
{
  /* If the above function invocation resulted in data that
     should be sent out on the network, the global variable
     uip_len is set to a value > 0. */
  if(uip_len > 0) {
    uip_arp_out();
    nic_send(NULL);
  }
}

//int main(void) __attribute__ ((noreturn));

/*---------------------------------------------------------------------------*/
int
main(void)
{
  EEPROM_main();

  int i;
  uip_ipaddr_t ipaddr;
  struct timer periodic_timer, arp_timer;

  memcpy (&uip_ethaddr.addr[0], &eeprom.MAC[0], 6);

  AVR_init();
  egpio_init();

  clock_init();
  mbuf_init();
  adlc_init();
  GICR = (1 << INT0);

  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  timer_set(&arp_timer, CLOCK_SECOND * 10);

  nic_init();


  uip_ipaddr(ipaddr, eeprom.IPAddr[0],eeprom.IPAddr[1],eeprom.IPAddr[2],eeprom.IPAddr[3]);
  uip_sethostaddr(ipaddr);
  uip_ipaddr(ipaddr, eeprom.Gateway[0],eeprom.Gateway[1],eeprom.Gateway[2],eeprom.Gateway[3]);
  uip_setdraddr(ipaddr);
  uip_ipaddr(ipaddr, eeprom.Subnet[0],eeprom.Subnet[1],eeprom.Subnet[2],eeprom.Subnet[3]);
  uip_setnetmask(ipaddr);

  telnetd_init();
  aun_init();
  internet_init();

  egpio_write (EGPIO_STATUS_GREEN);

  while(1) {

// check the econet for complete packets
    adlc_poller();
    aun_poller ();

    uip_len = nic_poll();

    if(uip_len > 0) {

      if(BUF->type == htons(UIP_ETHTYPE_IP)) {
	uip_arp_ipin();
	uip_input();
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */
	maybe_send();
      } else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {
	uip_arp_arpin();
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */
	if(uip_len > 0) {
	  nic_send(NULL);
	}
      }

    } else if(timer_expired(&periodic_timer)) {
      timer_reset(&periodic_timer);
      for(i = 0; i < UIP_CONNS; i++) {
	uip_periodic(i);
	maybe_send();
      }

#if UIP_UDP
      for(i = 0; i < UIP_UDP_CONNS; i++) {
	uip_udp_periodic(i);
	maybe_send();
      }
#endif /* UIP_UDP */

      /* Call the ARP timer function every 10 seconds. */
      if(timer_expired(&arp_timer)) {
	timer_reset(&arp_timer);
	uip_arp_timer();
      }
    }
  }
}

#ifdef __DHCPC_H__

void
dhcpc_configured(const struct dhcpc_state *s)
{
  uip_sethostaddr(s->ipaddr);
  uip_setnetmask(s->netmask);
  uip_setdraddr(s->default_router);
  resolv_conf(s->dnsaddr);
}
#endif /* __DHCPC_H__ */


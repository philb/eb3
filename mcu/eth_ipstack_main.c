/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include <atmel_start.h>
#include <eth_ipstack_main.h>

#include <hal_mac_async.h>
#include <lwip_macif_config.h>
#include <ethif_mac.h>
#include <netif/etharp.h>
#include <lwip/dhcp.h>
#include <string.h>

void TCPIP_STACK_init(void)
{
	lwip_init();
}

struct netif TCPIP_STACK_INTERFACE_0_desc;
static u8_t  TCPIP_STACK_INTERFACE_0_hwaddr[6];

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function mac_low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK  if the loopif is initialized
 */
err_t TCPIP_STACK_INTERFACE_0_stack_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));
	LWIP_ASSERT("netif->state != NULL", (netif->state != NULL));

	netif->output     = etharp_output;
	netif->linkoutput = mac_low_level_output;

	/* device capabilities */
	TCPIP_STACK_INTERFACE_0_desc.flags = CONF_TCPIP_STACK_INTERFACE_0_FLAG;
	TCPIP_STACK_INTERFACE_0_desc.mtu   = CONF_TCPIP_STACK_INTERFACE_0_MTU;

	/* set MAC hardware address length */
	memcpy(TCPIP_STACK_INTERFACE_0_desc.hwaddr, TCPIP_STACK_INTERFACE_0_hwaddr, NETIF_MAX_HWADDR_LEN);
	TCPIP_STACK_INTERFACE_0_desc.hwaddr_len = ETHARP_HWADDR_LEN;

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	LWIP_MACIF_desc.hostname = CONF_TCPIP_STACK_INTERFACE_0_HOSTNAME;
#endif
	memcpy(TCPIP_STACK_INTERFACE_0_desc.name, CONF_TCPIP_STACK_INTERFACE_0_HOSTNAME_ABBR, 2);

	/* initialize the mac hardware */
	mac_low_level_init(netif);

	return ERR_OK;
}
void TCPIP_STACK_INTERFACE_0_init(u8_t hwaddr[6])
{
	struct ip_addr ip;
	struct ip_addr nm;
	struct ip_addr gw;
#if CONF_TCPIP_STACK_INTERFACE_0_DHCP
	ip_addr_set_zero(&ip);
	ip_addr_set_zero(&nm);
	ip_addr_set_zero(&gw);
#else
	ipaddr_aton(CONF_TCPIP_STACK_INTERFACE_0_IP, &ip);
	ipaddr_aton(CONF_TCPIP_STACK_INTERFACE_0_NETMASK, &nm);
	ipaddr_aton(CONF_TCPIP_STACK_INTERFACE_0_GATEWAY, &gw);
#endif
	memcpy(TCPIP_STACK_INTERFACE_0_hwaddr, hwaddr, 6);

	netif_add(&TCPIP_STACK_INTERFACE_0_desc,
	          &ip,
	          &nm,
	          &gw,
	          (void *)&ETHERNET_MAC_0,
	          TCPIP_STACK_INTERFACE_0_stack_init,
	          ethernet_input);
}

void eth_ipstack_init(void)
{
	TCPIP_STACK_init();
}

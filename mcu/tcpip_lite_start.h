/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#ifndef TCPIP_LITE_START_H
#define TCPIP_LITE_START_H

#ifdef __cplusplus
extern "C" {
#endif

#include <tcpip_types.h>
#include <mac_address.h>
#include <arpv4.h>
#include <dhcp_client.h>
#include <icmp.h>
#include <ipv4.h>
#include <network.h>
#include <syslog.h>
#include <tcpv4.h>
#include <udpv4.h>
#include <time.h>

/**
 * \brief Initialize IP Stack
 */
void tcpip_lite_stack_init(void);

#ifdef __cplusplus
}
#endif

#endif /* TCPIP_LITE_START_H */

/**
 * \addtogroup drivers
 * @{
 */

/**
 * \defgroup Serial
 * @{
 *
 */

/**
 * \file
 *         Header file for Serial routines
 *         
 * \author
 *         Mark Usher
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>

void serial_packet(unsigned short pktbuff, unsigned short pktlen);
extern void serial_short(unsigned short);
extern void serial_shortLH(unsigned short);
extern void serial_crlf(void);
extern void serial_tx_str(char *msg);


extern void serial_tx(uint8_t mask);
extern void serial_tx_hex(uint8_t mask);

#endif /* __ADLC_H__ */
/** @} */
/** @} */

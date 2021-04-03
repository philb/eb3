// =======================================================================
// == External GPIO ======================================================
// =======================================================================

#define	EGPIO_ECONET_RED		0x0			// Econet Red LED
#define	EGPIO_ECONET_GREEN	0x2			// Econet Green LED
#define	EGPIO_STATUS_RED		0x4			// Status Red LED
#define	EGPIO_ADLC_RESET		0x6			// ADLC Reset
#define	EGPIO_CLOCK_ENABLE	0x8			// Econet Clock Enable
#define	EGPIO_STATUS_GREEN	0xa			// Status Green LED
#define	EGPIO_ETHER_RESET		0xc			// Ethernet Reset
#define	EGPIO_TP1			0xe			// Serial TX Output

#define	EGPIO_SET			0x1			// flag to set

#ifndef __ASSEMBLER__
extern void egpio_init(void);
extern void egpio_write(u8_t);
#endif

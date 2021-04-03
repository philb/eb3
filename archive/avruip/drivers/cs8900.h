/*==========================================================================

      dev/cs8900.h

      Cirrus Logic CS8900A Ethernet chip

==========================================================================
*/

#ifndef _CS8900_H_
#define _CS8900_H_


#define CS8900_BASE 0x8000 // Default IO Port Address

#define	CS8900_RTDATA	((CS8900_BASE + 0x00))	// R/W	Rx / Tx Data Port 0
#define	CS8900_RTDATA1	((CS8900_BASE + 0x02))	// R/W	Rx / Tx Data Port 1
#define	CS8900_TxCMD	((CS8900_BASE + 0x04))	// Write	TxCMD (Transmit Command)
#define	CS8900_TxLEN	((CS8900_BASE + 0x06))	// Write	TxLength (Transmit Length)
#define	CS8900_ISQ		((CS8900_BASE + 0x08))	// Read	Interrupt Status Queue
#define	CS8900_PPTR		((CS8900_BASE + 0x0A))	// R/W	Packet Page Pointer
#define	CS8900_PDATA	((CS8900_BASE + 0x0C))	// R/W	Packet Page Data (Port 0)
#define	CS8900_PDATA1	((CS8900_BASE + 0x0E))	// R/W	Packet Page Data (Port 1)


// ISQ Events
#define ISQ_RxEvent			0x04
#define ISQ_TxEvent			0x08
#define ISQ_BufEvent			0x0C
#define ISQ_RxMissEvent 		0x10
#define ISQ_TxColEvent			0x12
#define ISQ_EventMask			0x3F

#define EVENT_MASK			0xFFC0

#define PP_IntReg				0x0022		// Interrupt configuration 
#define PP_IntReg_IRQ0			0x0000		// Use INTR0 pin 
#define PP_IntReg_IRQ1			0x0001		// Use INTR1 pin 
#define PP_IntReg_IRQ2			0x0002		// Use INTR2 pin 
#define PP_IntReg_IRQ3			0x0003		// Use INTR3 pin 


// Cirrus Logic CS8900a Packet Page registers

// 0x0000 - 0x0045 Bus Interface Registers

#define	PP_ChipID			0x0000 		// Read Only // Chip identifier - must be0x630E
#define	PP_ChipRev			0x0002 		// Chip revision, model codes
#define	PP_IO_BASE			0x0020		// Default 0300h		I/O Base Address	
#define	PP_INT_NUM			0x0022		// XXXX XXXX XXXX X100		Interrupt Number
#define	PP_DMA_CHAN			0x0024		// XXXX XXXX XXXX XX11		DMA Channel
#define	PP_DMA_SOF			0x0026		// 0000h				DMA Start of Frame Offset
#define	PP_DMA_FCNT			0x0028		// X000h				DMA Frame Count
#define	PP_DMA_RXCNT		0x002A		// 0000h				DMA Byte Count
#define	PP_MEM_BASE			0x002C		// XXX0 0000h			Memory Base Address
#define	PP_BOOT_BASE		0x0030		// XXX0 0000h			Boot PROM Base Address
#define	PP_BOOT_MASK		0x0034		// XXX0 0000h			Boot PROM Address Mask
#define	PP_EE_CMD			0x0040	
#define	PP_EE_DATA			0x0042
#define	PP_RX_FRM_CNT		0x0050

// 0x0100 - 0x013F Status and Control Registers

// configuration and control registers
#define	PP_RxCFG			0x0102		// Register 03 - Receiver Configuration
#define	PP_RxCTL			0x0104		// Register 05 - Receiver Control
#define	PP_TxCFG			0x0106		// Register 07 - Transmit Configuration
#define	PP_TxCmd			0x0108		// Register 09 - Transmit Command Status
#define	PP_BufCFG			0x010A		// Register 0B - Buffer Configuration
					   // 0x010C		// Reserved
					   // 0x010E		// Reserved
					   // 0x0110		// Reserved
#define	PP_LineCTL			0x0112		// Register 13 - Line Control
#define	PP_SelfCTL			0x0114		// Register 15 - Self Control
#define	PP_BusCTL			0x0116		// Register 17 - Bus Control
#define	PP_TestCTL			0x0118		// Register 19 - Test Control

// status and event registers
#define	PP_ISQ			0x0120		// Register 00 - Interrupt Status Queue				
#define	PP_RER			0x0124		// Register 04 - Receiver Event
#define	PP_TER			0x0128		// Register 08 - Transmitter Event
#define	PP_BufEvent			0x012C		// Register 0C - Buffer Event
#define	PP_RxMISS			0x0130		// Register 10 - Receiver Miss Counter
#define	PP_TxCOLL			0x0132		// Register 12 - Transmit Collision Counter
#define	PP_LineStat			0x0134		// Register 14 - Line Status
#define	PP_SelfStat			0x0136		// Register 16 - Self Status
#define	PP_BusStat			0x0138		// Register 18 - Bus Status
#define	PP_AUITDR			0x013C		// Register 1C - AUI Time Domain Reflectometer

// 0x0140 - 0x014F Initiate Transmit Registers

#define	CS_PP_TX_CMD		0x0144		// Transmit Command Request
#define	CS_PP_TX_LEN		0x0146		// Transmit Length

// 0x0150 - 0x015D Address Filter Registers

#define	PP_LAF			0x0150		// Logical Address Filter (hash table, 6 bytes)
#define	PP_IA				0x0158		// Individual Address (MAC)


// Frame location
#define	PP_Rx_STATUS		0x0400		// Receive Status
#define	PP_Rx_LENGTH		0x0402		// Receive Length in bytes
#define	PP_Rx_Location		0x0404		// Receive Frame Location
#define	PP_Tx_Location		0x0A00		// Transmit Frame Location

// ISQ Events
#define	ISQ_RxEvent			0x04
#define	ISQ_TxEvent     		0x08
#define	ISQ_BufEvent    		0x0C
#define	ISQ_RxMissEvent 		0x10
#define	ISQ_TxColEvent  		0x12
#define	ISQ_EventMask   		0x3F


// Register 3, Reeceiver Configuration - PP_RxCFG 0x0102
#define	PP_RxCFG_Skip1    	0x0040		// Skip (i.e. discard) current frame
#define	PP_RxCFG_Stream   	0x0080		// Enable streaming mode
#define	PP_RxCFG_RxOK     	0x0100		// RxOK interrupt enable
#define	PP_RxCFG_RxDMAonly	0x0200		// Use RxDMA for all frames
#define	PP_RxCFG_AutoRxDMA	0x0400		// Select RxDMA automatically 
#define	PP_RxCFG_BufferCRC	0x0800		// Include CRC characters in frame 
#define	PP_RxCFG_CRC		0x1000		// Enable interrupt on CRC error
#define	PP_RxCFG_RUNT		0x2000		// Enable interrupt on RUNT frames
#define	PP_RxCFG_EXTRA		0x4000		// Enable interrupt on frames with extra data


// Register 4, Receiver Event - PP_RER 0x0124
#define	PP_RER_IAHash		0x0040 		// Frame hash match
#define	PP_RER_Dribble		0x0080 		// Frame had 1-7 extra bits after last byte
#define	PP_RER_RxOK			0x0100 		// Frame received with no errors
#define	PP_RER_Hashed		0x0200 		// Frame address hashed OK
#define	PP_RER_IA			0x0400 		// Frame address matched IA
#define	PP_RER_Broadcast		0x0800 		// Broadcast frame
#define	PP_RER_CRC			0x1000 		// Frame had CRC error
#define	PP_RER_RUNT			0x2000 		// Runt frame
#define	PP_RER_EXTRA		0x4000 		// Frame was too long


// Register 5, Receiver Control - PP_RxCTL	0x0104
#define	PP_RxCTL_IAHash		0x0040 		// Accept frames that match hash
#define	PP_RxCTL_Promiscuous	0x0080 		// Accept any frame
#define	PP_RxCTL_RxOK		0x0100 		// Accept well formed frames
#define	PP_RxCTL_Multicast	0x0200 		// Accept multicast frames
#define	PP_RxCTL_IA			0x0400 		// Accept frame that matches IA
#define	PP_RxCTL_Broadcast	0x0800 		// Accept broadcast frames
#define	PP_RxCTL_CRC		0x1000 		// Accept frames with bad CRC
#define	PP_RxCTL_RUNT		0x2000 		// Accept runt frames
#define	PP_RxCTL_EXTRA		0x4000 		// Accept frames that are too long


// Register 7, Transmit Configuration - PP_TxCFG	0x0106
#define	PP_TxCFG_CRS		0x0040 		// Enable interrupt on loss of carrier
#define	PP_TxCFG_SQE		0x0080 		// Enable interrupt on Signal Quality Error
#define	PP_TxCFG_TxOK		0x0100 		// Enable interrupt on successful xmits
#define	PP_TxCFG_Late		0x0200 		// Enable interrupt on "out of window" 
#define	PP_TxCFG_Jabber		0x0400 		// Enable interrupt on jabber detect
#define	PP_TxCFG_Collision	0x0800 		// Enable interrupt if collision
#define	PP_TxCFG_16Collisions	0x8000 		// Enable interrupt if > 16 collisions
#define	PP_TxCFG_ALL_IE		0x87C0 		// Mask for all Tx events


// Register 8, Transmit Event - PP_TER 0x0128
#define	PP_TER_CRS			0x0040 		// Carrier lost
#define	PP_TER_SQE			0x0080 		// Signal Quality Error
#define	PP_TER_TxOK			0x0100 		// Packet sent without error
#define	PP_TER_Late			0x0200 		// Out of window
#define	PP_TER_Jabber		0x0400 		// Stuck transmit?
#define	PP_TER_NumCollisions	0x7800 		// Number of collisions
#define	PP_TER_16Collisions	0x8000 		// > 16 collisions


// Register 9, Transmitter Command Status - PP_TxCmd 0x0108
#define	PP_TxCmd_TxStart_5	0x0000 		// Start after 5 bytes in buffer
#define	PP_TxCmd_TxStart_381	0x0040 		// Start after 381 bytes in buffer
#define	PP_TxCmd_TxStart_1021	0x0080 		// Start after 1021 bytes in buffer
#define	PP_TxCmd_TxStart_Full	0x00C0 		// Start after all bytes loaded
#define	PP_TxCmd_Force		0x0100 		// Discard any pending packets
#define	PP_TxCmd_OneCollision	0x0200 		// Abort after a single collision
#define	PP_TxCmd_NoCRC		0x1000 		// Do not add CRC
#define	PP_TxCmd_NoPad		0x2000 		// Do not pad short packets


// Register B, Buffer Configuration - PP_BufCFG 0x010A
#define	PP_BufCFG_SWI		0x0040 		// Force interrupt via software
#define	PP_BufCFG_RxDMA		0x0080 		// Enable interrupt on Rx DMA
#define	PP_BufCFG_TxRDY		0x0100 		// Enable interrupt when ready for Tx
#define	PP_BufCFG_TxUE		0x0200 		// Enable interrupt in Tx underrun
#define	PP_BufCFG_RxMiss		0x0400 		// Enable interrupt on missed Rx packets
#define	PP_BufCFG_Rx128		0x0800 		// Enable Rx interrupt after 128 bytes
#define	PP_BufCFG_TxCol		0x1000 		// Enable int on Tx collision ctr overflow
#define	PP_BufCFG_Miss		0x2000 		// Enable int on Rx miss ctr overflow
#define	PP_BufCFG_RxDest		0x8000 		// Enable int on Rx dest addr match


// Register C, Buffer Event - PP_BufEvent 0x012C
#define	PP_BufEvent_SWInt		0x0040
#define	PP_BufEvent_Rdy4Tx	0x0100
#define	PP_BufEvent_TxUnder	0x0200
#define	PP_BufEvent_RxMiss	0x0400


// Register 13, Line Control - PP_LineCTL 0x0112
#define	PP_LineCTL_Rx		0x0040 		// Enable receiver
#define	PP_LineCTL_Tx		0x0080 		// Enable transmitter
#define	PP_LineCTL_AUI		0x0100		// bit 8 AUIonly When set, AUI selected (takes precedence over 
									// 	 AutoAUI/10BT).
#define	PP_LineCTL_Auto		0x0200		// bit 9 AutoAUI/10BT When set, automatic interface selection 
									// 	 enabled. When both bits 8 and 9 are clear, 10BASE-T selected.
#define	PP_LineCTL_BackOff	0x0800		// bit B When set, the modified backoff algorithm is used. When clear,
									// 	 the standard backoff algorithm is used.
#define	PP_LineCTL_Polarity	0x1000		// bit C
#define	PP_LineCTL_2PartDef	0x2000		// bit D When set, two-part deferral is disabled.
#define	PP_LineCTL_LoRx		0x4000		// bit E LoRx Squelch When set, receiver squelch level reduced 
									// 	 by approximately 6 dB.
		
// Register 14, Line Status - PP_LineStat 0x0134
#define	PP_LineStat_LinkOK	0x0080 		// Line is connected and working
#define	PP_LineStat_AUI		0x0100 		// Connected via AUI
#define	PP_LineStat_10BT		0x0200 		// Connected via twisted pair
#define	PP_LineStat_Polarity	0x1000 		// Line polarity OK (10BT only)
#define	PP_LineStat_CRS		0x4000 		// Frame being received


// Register 15, Self Control - PP_SelfCtl 0x0114
#define	PP_SelfCtl_Reset		0x0040 		// Self-clearing reset


// Register 16, Self Status - PP_SelfStat 0x0136
#define	PP_SelfStat_InitD		0x0080 		// Chip initialization complete
#define	PP_SelfStat_SIBSY		0x0100 		// EEPROM is busy
#define	PP_SelfStat_EEPROM	0x0200 		// EEPROM present
#define	PP_SelfStat_EEPROM_OK	0x0400 		// EEPROM checks out
#define	PP_SelfStat_ELPresent	0x0800 		// External address latch logic available
#define	PP_SelfStat_EEsize	0x1000 		// Size of EEPROM


// Register 17, Bus Control - PP_BusCtl 0x0116
#define	PP_BusCtl_ResetRxDMA	0x0040 		// Reset receiver DMA engine
#define	PP_BusCtl_DMAextend	0x0100
#define	PP_BusCtl_UseSA		0x0200
#define	PP_BusCtl_MemoryE		0x0400 		// Enable "memory mode"
#define	PP_BusCtl_DMAburst	0x0800
#define	PP_BusCtl_IOCH_RDYE	0x1000
#define	PP_BusCtl_RxDMAsize	0x2000
#define	PP_BusCtl_EnableIRQ	0x8000		// Enable interrupts


// Register 18, Bus Status - PP_BusStat 0x0138
#define	PP_BusStat_TxBid		0x0080 		// Tx error
#define	PP_BusStat_TxRDY		0x0100 		// Ready for Tx data


// Register 19, Test Control - PP_TestCTL 0x0118
#define	PP_TestCTL_DisableLT	0x0080 		// Disable LT
#define	PP_TestCTL_FDX		0x4000		// Full Dulpex Mode
#define	PP_TestCTL_HDX		0x0000		// Half Duplex Mode





#endif /* _CS8900_H_ */

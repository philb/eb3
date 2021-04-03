
; Cirrus Logic CS8900a I/O Registers


.equ	CS8900_RTDATA	= CS8900_BASE + 0x00					; R/W	Rx / Tx Data Port 0
.equ	CS8900_RTDATA1	= CS8900_BASE + 0x02					; R/W	Rx / Tx Data Port 1
.equ	CS8900_TxCMD	= CS8900_BASE + 0x04					; Write	TxCMD (Transmit Command)
.equ	CS8900_TxLEN	= CS8900_BASE + 0x06					; Write	TxLength (Transmit Length)
.equ	CS8900_ISQ		= CS8900_BASE + 0x08					; Read	Interrupt Status Queue
.equ	CS8900_PPTR		= CS8900_BASE + 0x0A					; R/W	Packet Page Pointer
.equ	CS8900_PDATA	= CS8900_BASE + 0x0C					; R/W	Packet Page Data (Port 0)
.equ	CS8900_PDATA1	= CS8900_BASE + 0x0E					; R/W	Packet Page Data (Port 1)

; Cirrus Logic CS8900a Packet Page registers

; 0x0000 - 0x0045 Bus Interface Registers

.equ	PP_ChipID		= 0x0000 		; Read Only ; Chip identifier - must be= 0x630E
.equ	PP_ChipRev		= 0x0002 		; Chip revision, model codes
.equ	PP_IO_BASE		= 0x0020		; Default = 0300h			I/O Base Address	
.equ	PP_INT_NUM		= 0x0022		; XXXX XXXX XXXX X100		Interrupt Number
.equ	PP_DMA_CHAN		= 0x0024		; XXXX XXXX XXXX XX11		DMA Channel
.equ	PP_DMA_SOF		= 0x0026		; 0000h				DMA Start of Frame Offset
.equ	PP_DMA_FCNT		= 0x0028		; X000h				DMA Frame Count
.equ	PP_DMA_RXCNT	= 0x002A		; 0000h				DMA Byte Count
.equ	PP_MEM_BASE		= 0x002C		; XXX0 0000h			Memory Base Address
.equ	PP_BOOT_BASE	= 0x0030		; XXX0 0000h			Boot PROM Base Address
.equ	PP_BOOT_MASK	= 0x0034		; XXX0 0000h			Boot PROM Address Mask
.equ	PP_EE_CMD		= 0x0040	
.equ	PP_EE_DATA		= 0x0042
.equ	PP_RX_FRM_CNT	= 0x0050

; 0x0100 - 0x013F Status and Control Registers

; configuration and control registers
.equ	PP_RxCFG		= 0x0102		; Register 03 - Receiver Configuration
.equ	PP_RxCTL		= 0x0104		; Register 05 - Receiver Control
.equ	PP_TxCFG		= 0x0106		; Register 07 - Transmit Configuration
.equ	PP_TxCmd		= 0x0108		; Register 09 - Transmit Command Status
.equ	PP_BufCFG		= 0x010A		; Register 0B - Buffer Configuration
				; 0x010C		; Reserved
				; 0x010E		; Reserved
				; 0x0110		; Reserved
.equ	PP_LineCTL		= 0x0112		; Register 13 - Line Control
.equ	PP_SelfCTL		= 0x0114		; Register 15 - Self Control
.equ	PP_BusCTL		= 0x0116		; Register 17 - Bus Control
.equ	PP_TestCTL		= 0x0118		; Register 19 - Test Control

;status and event registers
.equ	PP_ISQ		= 0x0120		; Register 00 - Interrupt Status Queue				
.equ	PP_RER		= 0x0124		; Register 04 - Receiver Event
.equ	PP_TER		= 0x0128		; Register 08 - Transmitter Event
.equ	PP_BufEvent		= 0x012C		; Register 0C - Buffer Event
.equ	PP_RxMISS		= 0x0130		; Register 10 - Receiver Miss Counter
.equ	PP_TxCOLL		= 0x0132		; Register 12 - Transmit Collision Counter
.equ	PP_LineStat		= 0x0134		; Register 14 - Line Status
.equ	PP_SelfStat		= 0x0136		; Register 16 - Self Status
.equ	PP_BusStat		= 0x0138		; Register 18 - Bus Status
.equ	PP_AUITDR		= 0x013C		; Register 1C - AUI Time Domain Reflectometer

; 0x0140 - 0x014F Initiate Transmit Registers

.equ	CS_PP_TX_CMD	= 0x0144		; Transmit Command Request
.equ	CS_PP_TX_LEN	= 0x0146		; Transmit Length

; 0x0150 - 0x015D Address Filter Registers

.equ	PP_LAF		= 0x0150		; Logical Address Filter (hash table, 6 bytes)
.equ	PP_IA			= 0x0158		; Individual Address (MAC)


; Frame location
.equ	PP_Rx_STATUS	= 0x0400		; Receive Status
.equ	PP_Rx_LENGTH	= 0x0402		; Receive Length in bytes
.equ	PP_Rx_Location	= 0x0404		; Receive Frame Location
.equ	PP_Tx_Location	= 0x0A00		; Transmit Frame Location

; ISQ Events
.equ	ISQ_RxEvent		= 0x04
.equ	ISQ_TxEvent		= 0x08
.equ	ISQ_BufEvent	= 0x0C
.equ	ISQ_RxMissEvent	= 0x10
.equ	ISQ_TxColEvent	= 0x12
.equ	ISQ_EventMask	= 0x3F


; Register 3, Reeceiver Configuration - PP_RxCFG 0x0102
.equ	PP_RxCFG_Skip1    	= 0x40	; Skip (i.e. discard) current frame
.equ 	PP_RxCFG_Stream   	= 0x80	; Enable streaming mode
.equ	PP_RxCFG_RxOK     	= 0x01	; RxOK interrupt enable
.equ	PP_RxCFG_RxDMAonly	= 0x02	; Use RxDMA for all frames
.equ	PP_RxCFG_AutoRxDMA	= 0x04	; Select RxDMA automatically 
.equ	PP_RxCFG_BufferCRC	= 0x08	; Include CRC characters in frame 
.equ	PP_RxCFG_CRC		= 0x10	; Enable interrupt on CRC error
.equ 	PP_RxCFG_RUNT		= 0x20	; Enable interrupt on RUNT frames
.equ	PP_RxCFG_EXTRA		= 0x40	; Enable interrupt on frames with extra data


; Register 4, Receiver Event - PP_RER 0x0124
.equ	PP_RER_IAHash		= 0x40 	; Frame hash match
.equ	PP_RER_Dribble		= 0x80 	; Frame had 1-7 extra bits after last byte
.equ	PP_RER_RxOK			= 0x01 	; Frame received with no errors
.equ	PP_RER_Hashed		= 0x02 	; Frame address hashed OK
.equ	PP_RER_IA			= 0x04 	; Frame address matched IA
.equ	PP_RER_Broadcast		= 0x08 	; Broadcast frame
.equ	PP_RER_CRC			= 0x10 	; Frame had CRC error
.equ	PP_RER_RUNT			= 0x20 	; Runt frame
.equ	PP_RER_EXTRA		= 0x40 	; Frame was too long


; Register 5, Receiver Control - PP_RxCTL	0x0104
.equ	PP_RxCTL_IAHash		= 0x40 	; Accept frames that match hash
.equ	PP_RxCTL_Promiscuous	= 0x80 	; Accept any frame
.equ	PP_RxCTL_RxOK		= 0x01 	; Accept well formed frames
.equ	PP_RxCTL_Multicast	= 0x02 	; Accept multicast frames
.equ	PP_RxCTL_IA			= 0x04 	; Accept frame that matches IA
.equ	PP_RxCTL_Broadcast	= 0x08 	; Accept broadcast frames
.equ	PP_RxCTL_CRC		= 0x10 	; Accept frames with bad CRC
.equ	PP_RxCTL_RUNT		= 0x20 	; Accept runt frames
.equ	PP_RxCTL_EXTRA		= 0x40 	; Accept frames that are too long


; Register 7, Transmit Configuration - PP_TxCFG	0x0106
.equ	PP_TxCFG_CRS		= 0x40 	; Enable interrupt on loss of carrier
.equ	PP_TxCFG_SQE		= 0x80 	; Enable interrupt on Signal Quality Error
.equ	PP_TxCFG_TxOK		= 0x01 	; Enable interrupt on successful xmits
.equ	PP_TxCFG_Late		= 0x02 	; Enable interrupt on "out of window" 
.equ	PP_TxCFG_Jabber		= 0x04 	; Enable interrupt on jabber detect
.equ	PP_TxCFG_Collision	= 0x08 	; Enable interrupt if collision
.equ	PP_TxCFG_16Collisions	= 0x80 	; Enable interrupt if > 16 collisions
.equ	PP_TxCFG_ALL_IE		= 0x87C0 	; Mask for all Tx events


; Register 8, Transmit Event - PP_TER 0x0128
.equ	PP_TER_CRS			= 0x40 	; Carrier lost
.equ	PP_TER_SQE			= 0x80 	; Signal Quality Error
.equ	PP_TER_TxOK			= 0x01 	; Packet sent without error
.equ	PP_TER_Late			= 0x02 	; Out of window
.equ	PP_TER_Jabber		= 0x04 	; Stuck transmit?
.equ	PP_TER_NumCollisions	= 0x78 	; Number of collisions
.equ	PP_TER_16Collisions	= 0x80 	; > 16 collisions


; Register 9, Transmitter Command Status - PP_TxCmd 0x0108
.equ	PP_TxCmd_TxStart_5	= 0x00 	; Start after 5 bytes in buffer
.equ	PP_TxCmd_TxStart_381	= 0x40 	; Start after 381 bytes in buffer
.equ	PP_TxCmd_TxStart_1021	= 0x80 	; Start after 1021 bytes in buffer
.equ	PP_TxCmd_TxStart_Full	= 0xC0 	; Start after all bytes loaded
.equ	PP_TxCmd_Force		= 0x01 	; Discard any pending packets
.equ	PP_TxCmd_OneCollision	= 0x02 	; Abort after a single collision
.equ	PP_TxCmd_NoCRC		= 0x10 	; Do not add CRC
.equ	PP_TxCmd_NoPad		= 0x20 	; Do not pad short packets


; Register B, Buffer Configuration - PP_BufCFG 0x010A
.equ	PP_BufCFG_SWI		= 0x40 	; Force interrupt via software
.equ	PP_BufCFG_RxDMA		= 0x80 	; Enable interrupt on Rx DMA
.equ	PP_BufCFG_TxRDY		= 0x01 	; Enable interrupt when ready for Tx
.equ	PP_BufCFG_TxUE		= 0x02 	; Enable interrupt in Tx underrun
.equ	PP_BufCFG_RxMiss		= 0x04 	; Enable interrupt on missed Rx packets
.equ	PP_BufCFG_Rx128		= 0x08 	; Enable Rx interrupt after 128 bytes
.equ	PP_BufCFG_TxCol		= 0x10 	; Enable int on Tx collision ctr overflow
.equ	PP_BufCFG_Miss		= 0x20 	; Enable int on Rx miss ctr overflow
.equ	PP_BufCFG_RxDest		= 0x80 	; Enable int on Rx dest addr match


; Register C, Buffer Event - PP_BufEvent 0x012C
.equ	PP_BufEvent_SWInt		= 0x40
.equ	PP_BufEvent_Rdy4Tx	= 0x01
.equ	PP_BufEvent_TxUnder	= 0x02
.equ	PP_BufEvent_RxMiss	= 0x04


; Register 13, Line Control - PP_LineCTL 0x0112
.equ	PP_LineCTL_Rx		= 0x40 	; Enable receiver
.equ	PP_LineCTL_Tx		= 0x80 	; Enable transmitter
.equ	PP_LineCTL_AUI		= 0x01	; bit 8 AUIonly When set, AUI selected (takes precedence over 
							; AutoAUI/10BT).
.equ	PP_LineCTL_Auto		= 0x02	; bit 9 AutoAUI/10BT When set, automatic interface selection 
							; enabled. When both bits 8 and 9 are clear, 10BASE-T selected.
.equ	PP_LineCTL_BackOff	= 0x08	; bit B When set, the modified backoff algorithm is used. When clear,
							; the standard backoff algorithm is used.
.equ	PP_LineCTL_Polarity	= 0x10	; bit C
.equ	PP_LineCTL_2PartDef	= 0x20	; bit D When set, two-part deferral is disabled.
.equ	PP_LineCTL_LoRx		= 0x40	; bit E LoRx Squelch When set, receiver squelch level reduced 
							; by approximately 6 dB.

; Register 14, Line Status - PP_LineStat 0x0134
.equ	PP_LineStat_LinkOK	= 0x80 	; Line is connected and working
.equ	PP_LineStat_AUI		= 0x01 	; Connected via AUI
.equ	PP_LineStat_10BT		= 0x02 	; Connected via twisted pair
.equ	PP_LineStat_Polarity	= 0x10 	; Line polarity OK (10BT only)
.equ	PP_LineStat_CRS		= 0x40 	; Frame being received


; Register 15, Self Control - PP_SelfCtl 0x0114
.equ	PP_SelfCtl_Reset		= 0x40 	; Self-clearing reset


; Register 16, Self Status - PP_SelfStat 0x0136
.equ	PP_SelfStat_InitD		= 0x80 	; Chip initialization complete
.equ	PP_SelfStat_SIBSY		= 0x01 	; EEPROM is busy
.equ	PP_SelfStat_EEPROM	= 0x02 	; EEPROM present
.equ	PP_SelfStat_EEPROM_OK	= 0x04 	; EEPROM checks out
.equ	PP_SelfStat_ELPresent	= 0x08 	; External address latch logic available
.equ	PP_SelfStat_EEsize	= 0x10 	; Size of EEPROM


; Register 17, Bus Control - PP_BusCtl 0x0116
.equ	PP_BusCtl_ResetRxDMA	= 0x40 	; Reset receiver DMA engine
.equ	PP_BusCtl_DMAextend	= 0x01
.equ	PP_BusCtl_UseSA		= 0x02
.equ	PP_BusCtl_MemoryE		= 0x04 	; Enable "memory mode"
.equ	PP_BusCtl_DMAburst	= 0x08
.equ	PP_BusCtl_IOCH_RDYE	= 0x10
.equ	PP_BusCtl_RxDMAsize	= 0x20
.equ	PP_BusCtl_EnableIRQ	= 0x80 	; Enable interrupts


; Register 18, Bus Status - PP_BusStat 0x0138
.equ	PP_BusStat_TxBid		= 0x80 	; Tx error
.equ	PP_BusStat_TxRDY		= 0x01 	; Ready for Tx data


; Register 19, Test Control - PP_TestCTL 0x0118
.equ	PP_TestCTL_DisableLT	= 0x80 	; Disable LT
.equ	PP_TestCTL_FDX		= 0x40	; Full Dulpex Mode
.equ	PP_TestCTL_HDX		= 0x00	; Half Duplex Mode


; =======================================================================
; == CS8900==============================================================
; =======================================================================
;
; CS8900 has 16bit registers. Both bytes should be used when reading or writing


; -----------------------------------------------------------------------------------------
; CS8900 cs8900_poll_init
; -----------------------------------------------------------------------------------------
;
; initialise CS8900 Ethernet interface
;
; on exit
;	r16	= 0	: Success
;		= 1	: Not initialised
;		= 2	: No Response

cs8900_poll_init:
	ldi	r16, EGPIO_ETHER_RESET		; unreset chip
	rcall	egpio_write

	rcall delay_65536

	; step 1: software reset the chip
	rcall	cs_software_reset
	cpi	r16, 0 				; if r16 was successeful r16=0?
	breq	step2
	ret						; return

	
	; step 2 : Set up Ethernet hardware address
step2:

	; Add the MAC address to the NIC
	ldi	r16, PP_IA & 0xFF			; LSB register
	ldi	r17, PP_IA >> 8			; MSB register

	;load X with Mac_addr
	ldi	XL, MAC_addr_SA & 0xff		; set XL with the lowbyte of the MAC address
	ldi	XH, MAC_addr_SA >> 8		; set XH with the highbyte of the MAC address

	clr	r19
	ldi	tmp, 6				; loop count 6 for the 6 octets 5-0

LoadMAC:							
	ld	r18, X+				; Octet 5 of IA
	rcall	WritePPRegister

	dec	tmp
	cpi	tmp, 0				; will have performed the loop 6 times
	BRNE	LoadMAC

	; step 3 : Configure RxCTL to receive good frames for Indivual Addr, Broadcast, and Multicast.

	ldi	r16, PP_RxCTL & 0xFF		; LSB register
	ldi	r17, PP_RxCTL >> 8		; MSB register
	ldi	r18, PP_RxCTL_IAHash		; LSB data
	ldi	r19, PP_RxCTL_RxOK | PP_RxCTL_Multicast | PP_RxCTL_IA | PP_RxCTL_Broadcast 	
							; MSB data

;	ldi	r19, PP_RxCTL_Promiscuous|PP_RxCTL_RxOK	; or set to Promiscuous mode to receive all network traffic
	rcall	WritePPRegister

	; step 4: Configure TestCTL (DuplexMode)

 	ldi	r16, PP_LineCTL & 0xFF		; LSB register
	ldi	r17, PP_LineCTL >> 8		; MSB register
	clr	r18					; LSB data
	ldi	r19, duplexMode			; MSB data
	rcall WritePPRegister

	; step 5: Set SerRxOn, SerTxOn in LineCTL

 	; Enable Tx
	ldi	r16, PP_LineCTL & 0xFF		; LSB register
	ldi	r17, PP_LineCTL >> 8		; MSB register
	ldi	r18, PP_LineCTL_Rx | PP_LineCTL_Tx		
							; LSB data
	clr	r19					; MSB data
	rcall WritePPRegister


; return codes
	clr r16
	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_poll
; -----------------------------------------------------------------------------------------
;
cs_poll:

poll_until_no_events:

	rcall	cs_Poll_Registers			; polls the Rx, Tx, Buff registers, returning with r18 & r19 the data else 0
	
	cpi	r18, 0x0				; check if any events were returned
	brne	process_event
	cpi	r19, 0x0
	breq	poll_registers_ret

process_event:
	mov	r17, r18
	andi	r17, 0x3F				; find the register mask
							; 0x04 = RxEvent
							; 0x08 = TxEvent
							; 0x0C = BuffEvent

	cpi	r17, 0x04
	breq	CS_Process_RxEvent
	cpi	r17, 0x08
	breq	CS_Process_TxEvent
	cpi	r17, 0x0C	
	breq	CS_Process_BuffEvent
	
	; RxMiss Counter	see page 79
	; counter-overflow report: RxMISS (register 10) - Do statistics here.

	; TxCOL counter
	; counter-overflow report: TxCOL (register 12) - Do your statistics here.
	rjmp	poll_until_no_events

poll_registers_ret:
	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_Process_RxEvent
; -----------------------------------------------------------------------------------------
;
; entry : 	r18 = LSB
;		r19 = MSB
;
CS_Process_RxEvent:

	rcall Eth
	rcall	space					; output a space	
	rcall rx
	rcall	space					; output a space	

	; test for RxOK bit. 
	bst	r19, 0				; test for RxOK bit
	brbs	6, CS_RxOK
	
	; otherwise check the error bits
	bst	r19, 4				; PP_RER_CRC
	brbs	6, Rx_Err_CRC
	bst	r19, 5				; PP_RER_RUNT - frame shorter than 64 bates
	brbs	6, Rx_Err_RUNT
	bst	r19, 6				; PP_RER_EXTRA - frame longer than 1518 bytes
	brbs	6, Rx_Err_EXTRA

	rcall output_Error

	rjmp	poll_until_no_events

; -----------------------------------------------------------------------------------------
; CS8900 cs_Process_TxEvent
; -----------------------------------------------------------------------------------------
;
; entry : 	r18 = LSB
;		r19 = MSB
;
CS_Process_TxEvent:

	rcall Eth
	rcall	space					; output a space	
	rcall tx
	rcall	space					; output a space	

	rcall	output_r19_r18
	rcall	crlf

	rjmp	poll_until_no_events
	
; -----------------------------------------------------------------------------------------
; CS8900 cs_Process_BuffEvent
; -----------------------------------------------------------------------------------------
;
; entry : 	r18 = LSB
;		r19 = MSB
;
CS_Process_BuffEvent:

	rcall Eth
	rcall	space					; output a space	
	rcall bu
	rcall	space					; output a space	

	rcall	output_r19_r18
	rcall	crlf

	rjmp	poll_until_no_events


; -----------------------------------------------------------------------------------------
; CS8900 Rx_Error_CRC
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r18, r19 = Receive Frame Status
Rx_Err_CRC:

	rcall output_BadCRC

	rcall	rx_skip	
	rjmp	poll_until_no_events

; -----------------------------------------------------------------------------------------
; CS8900 Rx_Err_RUNT
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r18, r19 = Receive Frame Status
Rx_Err_RUNT:
	rcall output_BadRunt

	rcall	rx_skip	
	rjmp	poll_until_no_events

; -----------------------------------------------------------------------------------------
; CS8900 rx_extra
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r18, r19 = Receive Frame Status
Rx_Err_EXTRA:
	rcall output_BadExtra

	rcall	rx_skip	
	rjmp	poll_until_no_events


rx_skip:

	; Note: Must skip this received error frame. Otherwise, CS8900 hangs here.
	; Read the length of Rx frame
	
	; read the packet length from the buffer into r18 and r19
	ldi	r16,PP_Rx_LENGTH & 0xFF		; LSB register
	ldi	r17,PP_Rx_LENGTH >> 8		; MSB register
	rcall	ReadPPRegisterHiLo

	; rite Skip1 to RxCfg Register and also keep the current configuration
	ldi	r16, PP_RxCFG & 0xFF		; LSB register
	ldi	r17, PP_RxCFG >> 8		; MSB register
	ldi	r18, PP_RxCFG_Skip1		; LSB data RX_Skip_1
	ldi	r19, 0x00				; MSB data
	rcall	WritePPRegister

	ldi	r16, 0x00			
	sts	cs8900_state, r16			; set state

	ret

; -----------------------------------------------------------------------------------------
; CS8900 CS_RxOK
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r18, r19 = Receive Frame Status

CS_RxOK:
	bst	r19, 1				; received frame had a Destination Address that was accepted by the hash filter
	brbs	6, rx_hashed
	bst	r19, 2				; received frame had a Destination Address which matched the Individual Address
	brbs	6, rx_IndivAdr
	bst	r19, 3				; Broadcast Address
	brbs	6, rx_broadcast	

	rjmp	poll_until_no_events

rx_hashed:
	rcall rx_read_frame
	rjmp	poll_until_no_events

rx_IndivAdr:
	rcall rx_read_frame
	rjmp	poll_until_no_events

rx_broadcast:
	rcall	rx_read_frame	
	rjmp	poll_until_no_events


rx_read_frame:
	; read the Rx Status from the buffer into r18 and r19
;	ldi	r16,PP_Rx_STATUS & 0xFF		; LSB register
;	ldi	r17,PP_Rx_STATUS >> 8		; MSB register
;	rcall	ReadPPRegisterHiLo

;	ldi	r16, up_S
;	rcall serial_tx
;	rcall	output_r19_r18
;	rcall	space					; output a space	


	; Read the Rx Status, and Rx Length Registers.
	ldi	r16,PP_Rx_LENGTH & 0xFF		; LSB register
	ldi	r17,PP_Rx_LENGTH >> 8		; MSB register
	rcall	ReadPPRegisterHiLo

;	ldi	r16, up_L 
;	rcall serial_tx
;	rcall	output_r19_r18
;	rcall	space					; output a space	


	sts 	cs8900_rx_ptr, r18		; save length of packet in the Rx buffer pointer
	sts 	cs8900_rx_ptr+1, r19

	;load Z with Rx Buffer
	ldi	ZL, CS8900_RX_BUF & 0xff	; set ZL with the lowbyte of the Rx buffer
	ldi	ZH, CS8900_RX_BUF >> 8		; set ZH with the highbyte of the Rx buffer

	mov 	YL, ZL				; copy into Y
	mov	YH, ZH

	add	YL, r18				; add the length
	add	YH, r19

	lds	r16, CS8900_RTDATA			; get the next byte and discard. Seems to be lsb of length

copy_rx_data:
	lds	r16, CS8900_RTDATA			; get the next byte
	st Z+, r16					; store it in memory

	cp	ZH, YH				; pointer = end of data High byte?
	brne	skip_rx_LSB				; no, don't bother checking the low byte

	cp	ZL, YL				; ZH=LH at this stage. does pointer low byte = end of data low byte
	breq	exit_copy_rx_data_loop		; yes. Data end reached. Finish

skip_rx_LSB:
	lds	r17, CS8900_RTDATA+1			; get the next byte
	st Z+, r17

	cp	ZH, YH				; pointer = end of data High byte?
	brne	copy_rx_data

	cp	ZL, YL				; pointer = end of data low byte?
	brne	copy_rx_data

exit_copy_rx_data_loop:

	; reading the RxEvent register signals that the host is finished with this frame.
	; this is done, when returning from this procedure on the next poll

	ldi	r16, PACKET_Rx			
	sts	cs8900_state, r16			; set state

	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_Poll_Registers
; -----------------------------------------------------------------------------------------
;
; exit : 	r18 = LSB
;		r19 = MSB
cs_Poll_Registers:
	; Poll the Rx and Tx and Buffer event registers with Rx, Tx, Buffer taking proiority.


	; EventMask = 0xFFC0			; mask out the register type
    
	; Read Rx Event register into r18 and r19
	ldi	r16,PP_RER & 0xFF			; LSB register
	ldi	r17,PP_RER >> 8			; MSB register
	rcall	ReadPPRegister
	mov	r17, r18				; Keep original value in r18 and operate on r17
	andi	r17, 0xC0				; Event Register Mask
	andi	r19, 0xFF

	cpi	r17,0
	brne	poll_return
	cpi	r19,0
	brne	poll_return

	; nothing was in rx so check tx

	; Read Tx Event register into r18 and r19
	ldi	r16,PP_TER & 0xFF			; LSB register
	ldi	r17,PP_TER >> 8			; MSB register
	rcall	ReadPPRegister
	mov	r17, r18				; Keep original value in r18 and operate on r17
	andi	r17, 0xC0				; Event Register Mask
	andi	r19, 0xFF

	cpi	r17,0
	brne	poll_return
	cpi	r19,0
	brne	poll_return

	; Read Buffer Event register into r18 and r19
	ldi	r16,PP_BufEvent & 0xFF		; LSB register
	ldi	r17,PP_BufEvent >> 8		; MSB register
	rcall	ReadPPRegister
	mov	r17, r18				; Keep original value in r18 and operate on r17
	andi	r17, 0xC0				; Event Register Mask
	andi	r19, 0xFF

	cpi	r17,0
	brne	poll_return
	cpi	r19,0
	brne	poll_return

	clr r18
	clr r19
poll_return:
	ret




; -----------------------------------------------------------------------------------------
; CS8900 cs_software_wakeup
; -----------------------------------------------------------------------------------------
;
; Not currently used
;
cs_software_wakeup:

	; wake up the CS8900
	ldi	r16,PP_SelfCTL & 0xFF		; LSB   CS8900_PPTR = PP_SelfCTL
	ldi	r17,PP_SelfCTL >> 8		; MSB
	sts	CS8900_PPTR, r16
	sts	CS8900_PPTR+1, r17

	ret


; -----------------------------------------------------------------------------------------
; CS8900 cs_software_reset
; -----------------------------------------------------------------------------------------
;
; software reset the CS8900 chip
;
; exit : r16 =	0 - Successfull
;			1 - reset bit not set

cs_software_reset:

	; send a software reset 
	ldi 	r16, PP_SelfCTL & 0xFF		; LSB   CS8900_PPTR = PP_SelfCTL
	ldi 	r17, PP_SelfCTL >> 8		; MSB
	ldi 	r18, PP_SelfCtl_Reset		; LSB	with CS8900_RTDATA = 0x0040
	clr 	r19					; MSB
	rcall WritePPRegister

	; add a short delay while chip resetting in progress
	rcall delay_65536
	
	; try 3072 times to read the INITD flag
	ldi	YL, 0x00
	ldi	YH, 0x0C

INITD_loop:
	; check the PP_SelfStat_InitD bit to find out if the chip successflly reset
	ldi	r16,PP_SelfStat & 0xFF		; LSB register
	ldi	r17,PP_SelfStat >> 8		; MSB register
	rcall	ReadPPRegister

	bst	r18, 7				; stores bit7 INITD of r18 in T flag 
	brbs	6, cs_reset_complete 		; if set, then the initialisation is complete

	dec	YL					; if not, try again 
	brne	INITD_loop				; Branch if YL<>0
	dec	YH
	brne	INITD_loop				; Branch if YH<>0

	ldi r16, 1					; failure
	ret

cs_reset_complete:
	clr r16					; success
	ret



; -----------------------------------------------------------------------------------------
; CS8900 cs8900_poll_send
; -----------------------------------------------------------------------------------------
;
; Transmit a frame in Poll mode
;
; on entry
; 		r16/r17 = databuffer pointer
; 		r18/r19 = length
; exit	
;		r16	= 0 success
;			= 1 Tx bid error
;			= 2 Tx bid not ready for Tx
;			= 3 Tx complete with error

cs8900_poll_send:

	mov	YL, r18				; get low byte of length
	mov	YH, r19				; get high byte of length

	mov	ZL, r16
	mov	ZH, r17 

	; Step 1: Write the TX command

	; check for a Previous Bid For Tx
	clr	r17					; MSB
	lds tmp, cs8900_PrevTxBidFail
	cpi tmp, 0x0
	breq No_prevTxBid
	
	; Previous BidForTX has reserved the Tx FIFO on CS8900, The FIFO must be released before 
	; proceeding. Setting the PP_TxCmd_Force bit will cause CS8900 to release the previously reserved Tx buffer.
	
	ldi	r16, PP_TxCmd_TxStart_Full | PP_TxCmd_Force	
							; LSB	write when all the packet is in the buffer and force

	sts	cs8900_PrevTxBidFail, r21	; clear the previous bid flag
	rjmp	writeTxCMD

No_prevTxBid:
	ldi	r16, PP_TxCmd_TxStart_Full 	; LSB	write when all the packet is in the buffer


writeTxCMD:
	sts	CS8900_TxCMD, r16
	sts	CS8900_TxCMD+1, r17

	; Step 2: Write the frame length (number of bytes to TX).
	; Note: After the frame length has been written, the CS8900 reserves the Tx buffer for this bid 
	; whether PP_BusStat_TxRDY is set or not.

	sts	CS8900_TxLEN, YL
	sts	CS8900_TxLEN+1,YH

	; Read BusST to verify it is set as Rdy4Tx.
	ldi	r16,PP_BusStat & 0xFF		; LSB 
	ldi	r17,PP_BusStat >> 8		; MSB
	rcall	ReadPPRegister

	; Step 3: Check for a Tx Bid Error (TxBidErr bit)

	; TxBidErr happens only if Tx_length is too small or too large.
	bst	r18, 7				; PP_BusStat_TxBid
	brbc	6, NoTxBidErr 			; if clear
	
	;debug
	rcall Eth
	rcall space
	rcall	output_BidError
	rcall crlf
	
	ldi r16, 0x1
	ret

     	; Step 4: check if chip is ready for Tx now
	; If Bid4Tx not ready, skip the frame

NoTxBidErr:
	bst	r19,0					; PP_BusStat_TxRDY
	brbs	6, TxRDY	 			; if set, Tx ok

	; If not ready for Tx now, abort this frame.
	; Note: Another alternative is to poll PP_BusStat_TxRDY until it is set, if you don't want to abort Tx frames.
	
	; Set to 1, and next time cs8900_poll_send() is called, it can set PP_TxCmd_Force to release the reserved Tx 
	; buffer in the CS8900 chip.
	ldi	r16, 0x01
	sts	cs8900_PrevTxBidFail, r16

	inc	r16					; return with 2, Tx Bid not ready 4 Tx
	ret

	; Step 5: copy Tx data into CS8900's buffer. This actually starts the Txmit

TxRDY:

	; Y contains the length
	mov	YL, r18				; get low byte of length
	mov	YH, r19				; get high byte of length

	; add the length to the start address for the loop
	add	YL, ZL
	adc	YH, ZH

tx_data_loop:
	ld	r16, Z+
	sts	CS8900_RTDATA, r16

	cp	ZH, YH				; pointer = end of data High byte?
	brne	skip_LSB				; no, don't bother checking the low byte

	cp	ZL, YL				; ZH=LH at this stage. does pointer low byte = end of data low byte
	breq	tx_data_loop			; yes. Data end reached. Finish

skip_LSB:
	ld	r16, Z+				; output the next byte to the CS_DATA_PORT high byte
	sts	CS8900_RTDATA+1, r16

	cp	ZH, YH				; pointer = end of data High byte?
	brne	tx_data_loop

	cp	ZL, YL				; pointer = end of data low byte?
	brne	tx_data_loop

exit_send_data_loop:
	
	; Step 6: Poll the TxEvent Reg for the TX completed status
	
	; This step is optional. If you don't wait until Tx compelete, the next time cs8900_poll_send() bids for Tx, 
	; it may encounter Not Ready For Tx because CS8900 is still Tx'ing.

	; try 0x7F00 times to read the Tx Event Register
	ldi	YL, 0x00
	ldi	YH, 0x7F

read_TER:
	; check to see if TxComplete
	ldi	r16,PP_TER & 0xFF			; LSB 
	ldi	r17,PP_TER >> 8			; MSB
	rcall	ReadPPRegister
	
	mov	tmp, r18

	andi	tmp, 0x08
	cpi	tmp, 0x08
	breq	TX_complete

	dec	YL					; if not, try again 
	brne	read_TER				; Branch if YL<>0
	dec	YH
	brne	read_TER				; Branch if YH<>0

TX_complete:
	cpi	r18, 0x08				; no errors flagged in LSB
	brne	Tx_with_error
	cpi	r19, PP_TER_TxOK			; just the TxOK set?
	brne	Tx_with_error			
	
	; Tx complete without error, return 0
	clr	r16
	ret

Tx_with_error:
	ldi	r16, 0x03				; Tx complete with error, 
	ret



; -----------------------------------------------------------------------------------------
; CS8900 WritePPRegister
; -----------------------------------------------------------------------------------------
;
; write value to the Packet Pointer register at address offset.
; LSB first MSB second
;
; on entry
; 		r16/r17 = address
; 		r18/r19 = data
; on exit
; 		r16/r17 = address
; 		r18/r19 = data

WritePPRegister:
	sts	CS8900_PPTR, 		r16		; write a 16 bit register offset to IO port CS8900_PPTR
	sts	CS8900_PPTR+1,	r17
	sts	CS8900_PDATA, 	r18		; write 16 bits to IO port number CS8900_PDATA
	sts	CS8900_PDATA+1,	r19
	ret

; -----------------------------------------------------------------------------------------
; CS8900 ReadPPRegister
; -----------------------------------------------------------------------------------------
;
; Read value from the Packet Pointer register at address offset. 
; LSB first MSB second
;
; on entry
; 		r16/r17 = address
; on exit
; 		r16/r17 = address
; 		r18/r19 = data

ReadPPRegister:
	sts	CS8900_PPTR, r16			; write a 16 bit register offset to IO port CS8900_PPTR
	sts	CS8900_PPTR+1, r17
	lds	r18, CS8900_PDATA			; read 16 bits from IO port number CS8900_PDATA
	lds	r19, CS8900_PDATA+1
	ret

; -----------------------------------------------------------------------------------------
; CS8900 ReadPPRegisterHiLo
; -----------------------------------------------------------------------------------------
; 
; Read value from the Packet Pointer register at address offset. 
; This is a special case where the high order byte is read first then the low order byte.  
; This special case is only used to read the RxStatus and RxLength registers 
;
; on entry
; 		r16/r17 = address
; on exit
; 		r16/r17 = address
; 		r18/r19 = data

ReadPPRegisterHiLo:
	sts	CS8900_PPTR, r16			; write a 16 bit register offset to IO port CS8900_PPTR
	sts	CS8900_PPTR+1, r17
	lds	r19, CS8900_PDATA+1		; read 16 bits from IO port number CS8900_PDATA
	lds	r18, CS8900_PDATA
	ret


; -----------------------------------------------------------------------------------------
; CS8900 WriteIORegister
; -----------------------------------------------------------------------------------------
;
; Write the 16 bit data in r18, r19 to the register, r16, r17.
;
; on entry
; 		X = register
; 		r16/r17 = data
; on exit
; 		X = register+1
; 		r16/r17 = data

WriteIORegister:
	st	X, r18				; Write the 16 bits of data to the IO Register
	st	X+, r19
	ret


; -----------------------------------------------------------------------------------------
; CS8900 cs_debug_pp
; -----------------------------------------------------------------------------------------
;
; Will read the two bytes from the PP data register as pointed to by r16 / r17
;
; on entry
; 		r16/r17 = address
; on exit
; 		r18/r19 = data
;
cs_debug_pp:

	rcall	ReadPPRegister	
	rcall	output_r18_r19
	rcall	crlf
	ret

; -----------------------------------------------------------------------------------------
; CS8900 delay
; -----------------------------------------------------------------------------------------

delay_65536:
	clr	r16
	clr	r17
delay_loop:
	inc	r17
	brne	delay_loop
	inc	r16
	brne	delay_loop

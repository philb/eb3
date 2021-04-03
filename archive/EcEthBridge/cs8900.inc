
; Cirrus Logic CS8900a I/O Registers


.equ	CS_DATA_P0		= CS8900_BASE + 0x00			; R/W	Rx / Tx Data Port 0
.equ	CS_DATA_P1		= CS8900_BASE + 0x02			; R/W	Rx / Tx Data Port 1
.equ	CS_TX_CMD_I		= CS8900_BASE + 0x04			; Write	TxCMD (Transmit Command)
.equ	CS_TX_LEN_I		= CS8900_BASE + 0x06			; Write	TxLength (Transmit Length)
.equ	CS_INT_STAT		= CS8900_BASE + 0x08			; Read	Interrupt Status Queue
.equ	CS_PP_PTR		= CS8900_BASE + 0x0A			; R/W	Packet Page Pointer
.equ	CS_PP_DATA0		= CS8900_BASE + 0x0C			; R/W	Packet Page Data (Port 0)
.equ	CS_PP_DATA1		= CS8900_BASE + 0x0E			; R/W	Packet Page Data (Port 1)


; Cirrus Logic CS8900a Packet Page registers

; 0x0000 - 0x0045 Bus Interface Registers

.equ	CS_PROD_ID		= 0x0000	; Read Only				Product Identification code
.equ	CS_IO_BASE		= 0x0020	; Default = 0300h		I/O Base Address	
.equ	CS_INT_NUM		= 0x0022	; XXXX XXXX XXXX X100	Interrupt Number
.equ	CS_DMA_CHAN		= 0x0024	; XXXX XXXX XXXX XX11	DMA Channel
.equ	CS_DMA_SOF		= 0x0026	; 0000h					DMA Start of Frame Offset
.equ	CS_DMA_FCNT		= 0x0028	; X000h					DMA Frame Count
.equ	CS_DMA_RXCNT	= 0x002A	; 0000h					DMA Byte Count
.equ	CS_MEM_BASE		= 0x002C	; XXX0 0000h			Memory Base Address
.equ	CS_BOOT_BASE	= 0x0030	; XXX0 0000h			Boot PROM Base Address
.equ	CS_BOOT_MASK	= 0x0034	; XXX0 0000h			Boot PROM Address Mask
.equ	CS_EE_CMD		= 0x0040	
.equ	CS_EE_DATA		= 0x0042
.equ	CS_RX_FRM_CNT	= 0x0050

; 0x0100 - 0x013F Status and Control Registers

; configuration and control registers
.equ	CS_RX_CFG		= 0x0102	; 0003h					Register 03 - Receiver Configuration
.equ	CS_RX_CTL		= 0x0104	; 0005h					Register 05 - Receiver Control
.equ	CS_TX_CFG		= 0x0106	; 0007h					Register 07 - Transmit Configuration
.equ	CS_TX_CMD_P		= 0x0108	; 0009h					Register 09 - Transmit Command Status
.equ	CS_BUF_CFG		= 0x010A	; 000Bh					Register 0B - Buffer Configuration
;						  0x010C	; Undefined				Reserved
;						  0x010E	; Undefined				Reserved
;						  0x0110	; Undefined				Reserved
.equ	CS_LINE_CTRL	= 0x0112	; 00013h [sic]			Register 13 - Line Control
.equ	CS_SELF_CTRL	= 0x0114	; 0015h					Register 15 - Self Control
.equ	CS_BUS_CTRL		= 0x0116	; 0017h					Register 17 - Bus Control
.equ	CS_TEST_CTRL	= 0x0118	; 0019h					Register 19 - Test Control

;status and event registers
.equ	CS_ISQ			= 0x0120	;						Register 00 - Interrupt Status Queue				
.equ	CS_RX_EVENT		= 0x0124	;						Register 04 - Receiver Event
.equ	CS_TX_EVENT		= 0x0128	;						Register 08 - Transmitter Event
.equ	CS_BUF_EVENT	= 0x012C	;						Register 0C - Buffer Event
.equ	CS_RX_MISS		= 0x0130	;						Register 10 - Receiver Miss Counter
.equ	CS_TX_COLL		= 0x0132	;						Register 12 - Transmit Collision Counter
.equ	CS_LINE_STAT	= 0x0134	;						Register 14 - Line Status
.equ	CS_SELF_STAT	= 0x0136	;						Register 16 - Self Status
.equ	CS_BUS_STAT		= 0x0138	;						Register 18 - Bus Status
.equ	CS_AUI_TDR		= 0x013C	;						Register 1C - AUI Time Domain Reflectometer

; 0x0140 - 0x014F Initiate Transmit Registers

.equ	CS_PP_TX_CMD	= 0x0144	;						Transmit Command Request
.equ	CS_PP_TX_LEN	= 0x0146	;						Transmit Length

; 0x0150 - 0x015D Address Filter Registers

.equ	CS_LOG_ADDR		= 0x0150	;						Logical Address Filter (hash table)
.equ	CS_IEEE_ADDR	= 0x0158	;						Individual Address

; Frame location
.equ	CS_RX_STATUS	= 0x0400	;						Receive Status
.equ	CS_RX_LENGTH	= 0x0402	;						Receive Length in bytes
.equ	CS_RX_FR_LOC	= 0x0404	;						Receive Frame Location
.equ	CS_TX_FR_LOC	= 0x0A00	;						Transmit Frame Location


; Register 4, Receiver Event
.equ	CS_RX_IAHash	= 0x040		; If the received frame's Destination Address is accepted by the hash filter, then
									; this bit is set if, and only if IAHashA (Register 5, RxCTL, Bit 6) is set, and 
									; Hashed (Bit 9) is set.
.equ	CS_RX_Dribble	= 0x080		; If set, the received frame had from one to seven bits after the last received full 
									; byte. An "Alignment Error" occurs when Dribblebits and CRCerror (Bit C) are both set.
.equ	CS_RX_RxOK		= 0x01		; If set, the received frame had a good CRC and valid length (i.e., there is not a CRC 
									; error, Runt error, or Extradata error). When RxOK is set, then the length of the 
									; received frame is contained at PacketPage base + 0402h. If RxOKiE (Register 3, RxCFG, 
									; Bit 8) is set, there is an interrupt.
.equ	CS_RX_Hashed	= 0x02		; If set, the received frame had a Destination Address that was accepted by the hash 
									; filter. If Hashed and RxOK (Bit 8) are set, Bits F through A of RxEvent become the Hash
									; Table Index for this frame [See Section 5.2.10 on page 87 for an exception regarding 
									; broadcast frames!].If Hashed and RxOK are not both set, then Bits F through A are 
									; individual event bits as defined below.
.equ	CS_RX_IndivAdr	= 0x04		; If the received frame had a Destination Address which matched the Individual Address found
									; at PacketPage base + 0158h, then this bit is set if, and only if, RxOK (Bit 8) is set and 
									; IndividualA (Register 5, RxCTL, Bit A) is set.
.equ	CS_RX_Broadcast	= 0x08		; If the received frame had a Broadcast Address (FFFF FFFF FFFFh) as the Destination Address,
									; then this bit is set if, and only if, RxOK is set and BroadcastA (Register 5, RxCTL, Bit B)
									; is set.
.equ	CS_RX_CRCError	= 0x10		; If set, the received frame had a bad CRC. If CRCerroriE (Register 3, RxCFG, Bit C) is set, there
									; is an interrupt
.equ	CS_RX_Run		= 0x20		; If set, the received frame was shorter than 64 bytes. If RuntiE (Register 3, RxCFG, Bit D) is set,
									; there is an interrupt.
.equ	CS_RX_Extradata	= 0x40		; If set, the received frame was longer than 1518 bytes. All bytes beyond 1518 are discarded. If
									; ExtradataiE (Register 3, RxCFG, Bit E) is set, there is an interrupt.


; Register 5, RxCTL
.equ CS_RX_IAHashA		= 0x40		;>bit 6 IAHashA When set, Individual Address frames that 
									; pass the hash filter are accepted
.equ CS_RX_PromiscuousA = 0x80		; bit 7 PromiscuousA When set, all frames are accepted *
.equ CS_RX_RxOKA		= 0x01		;>bit 8 RxOKA When set, frames with valid length and CRC 
									; and that pass the DA filter are accepted.
.equ CS_RX_MulticastA	= 0x02		;>bit 9 MulticastA When set, Multicast frames that pass 
									; the hash filter are accepted *
.equ CS_RX_IndividualA	= 0x04		;>bit A IndividualA When set, frames with DA that matches 
									; the IA at PacketPage base+ 0158h are accepted *
.equ CS_RX_BroadcastA	= 0x08		;>bit B BroadcastA When set, all broadcast frames are accepted. *
.equ CS_RX_CRCerrorA	= 0x10		; bit C CRCerrorA When set, frames with bad CRC that pass 
									; the DA filter are accepted.
.equ CS_RX_RuntA		= 0x20		; bit D RuntA When set, frames shorter than 64 bytes that 
									; pass the DA filter are accepted.	
.equ CS_RX_ExtradataA	= 0x40		; bit E ExtradataA When set, frames longer than 1518 bytes that 
									; pass the DA filter are accepted (only the first 1518 bytes are buffered).	
									; * 7,9,A,B must also meet the criteria programmed into bits 8,C,D,E


; Register 13, LineCTL
.equ CS_LCTL_SerRxON	= 0x40		;>bit 6 SerRxON When set, reception enabled.
.equ CS_LCTL_SerTxON	= 0x80		;>bit 7
.equ CS_LCTL_AUIonly	= 0x01		; bit 8 AUIonly When set, AUI selected (takes precedence over 
									; AutoAUI/10BT).
.equ CS_LCTL_AutoAUI	= 0x02		; bit 9 AutoAUI/10BT When set, automatic interface selection 
									; enabled. When both bits 8 and 9 are clear, 10BASE-T selected.
.equ CS_LCTL_ModBackoffE= 0x08		; bit B When set, the modified backoff algorithm is used. When clear,
									; the standard backoff algorithm is used.
.equ CS_LCTL_PolarityDis= 0x10		; bit C
.equ CS_LCTL_2PartDefDis= 0x20		; bit D When set, two-part deferral is disabled.
.equ CS_LCTL_LoRx		= 0x40		; bit E LoRx Squelch When set, receiver squelch level reduced 
									; by approximately 6 dB.


; =======================================================================
; == CS8900==============================================================
; =======================================================================
;
; CS8900 has 16bit registers. Both bytes should be used when reading or writing


; -----------------------------------------------------------------------------------------
; CS8900 cs_init
; -----------------------------------------------------------------------------------------
;
; initialise CS8900 Ethernet interface
;
; on exit
;			r16 = 0	: Success
;				= 1	: Not Found
;				= 2 : No Response

cs_init:	

	; check for chip
	lds r18, CS_PP_DATA0			; read the Packet Page Data Port
	lds r19, CS_PP_DATA0+1

	ldi	r16, 0x0E					; expected value
	cp	r18, r16
	brne cs8900_not_found			; didn't find it						;

	ldi r16, 0x63 					; expected value
	cp r19, r16
	brne cs8900_not_found			; didn't find it						;


	rcall cs_software_wakeup
	rcall cs_software_reset
	;wait for reset to complete

	; poll register 16 (Self Status) insted of inserting a time delay. 
	; INITD=1 initialisation is complete
	; SIBUSY=0 indicates the EEPROM is no longer being accessed

Initialising:

    ; Read Self Status register into r18 and r19
    ldi r16,CS_SELF_STAT & 0xFF		; LSB register
	ldi r17,CS_SELF_STAT >> 8		; MSB register
	rcall cs_read_pp


	; read Self Status Register
	; INITD = bit 7 0x80 of LSB
	; SiBUSY = bit 8 0x1 of MSB
	bst	r18, 7					; stores bit7 INITD of r18 in T flag 
	brbs 6, INIT_Complete	 	; if set, then the initialisation is complete
	rjmp Initialising

INIT_Complete:

    ldi r16,CS_SELF_STAT & 0xFF		; LSB register
	ldi r17,CS_SELF_STAT >> 8		; MSB register
	rcall cs_read_pp

	bst	r19, 1					; stores bit1 SiBusy of r19 in T flag 
	brbs 6, Init_Complete		; if set, still accessing the EEPROM

	; check it responds by writing 0x0102 to Packet Page Pointer Port
	ldi r18, 0x01
	ldi r19, 0x02
	sts	CS_PP_PTR, r18
	sts	CS_PP_PTR+1, r19

	; and read the Paket Page Data Port which should contain 0x03
	lds r18, CS_PP_DATA0			; read the Packet Page Data Port
	lds r19, CS_PP_DATA0+1

	ldi	r16, 0x03					; expected value
	cp	r19, r16
	brne cs8900_no_response			; didn't find it						;

    ; Add the MAC address to the NIC
	ldi r16, CS_IEEE_ADDR & 0xFF	; LSB register
    ldi r17, CS_IEEE_ADDR >> 8		; MSB register

	;load X with Mac_addr
	ldi	XL, MAC_addr_SA & 0xff		; set XL with the lowbyte of the MAC address
	ldi	XH, MAC_addr_SA >> 8		; set XH with the highbyte of the MAC address

	clr r19
	ldi tmp, 6						; loop count 6 for the 6 octets 5-0

LoadMAC:							
	ld r18, X+						; Octet 5 of IA
	rcall cs_write_pp

	dec tmp
	cpi tmp, 0						; will have performed the loop 6 times
	BRNE LoadMAC


    ; Enable Tx
    ldi r16,	CS_LINE_CTRL & 0xFF	; LSB register
	ldi r17,	CS_LINE_CTRL >> 8	; MSB register
	ldi r18, 	CS_LCTL_SerRxON | CS_LCTL_SerTxON		; LSB data
	clr r19							; MSB data
	rcall cs_write_pp


    ; Enable Rx
	ldi r16,	CS_RX_CTL & 0xFF	; LSB register
    ldi r17,	CS_RX_CTL >> 8		; MSB register
	ldi r18,	CS_RX_IAHashA		; LSB data
	ldi r19, 	CS_RX_RxOKA | CS_RX_MulticastA | CS_RX_IndividualA | CS_RX_BroadcastA 	; MSB data
	rcall cs_write_pp

; return codes

	ldi r16,0x00
	ret
cs8900_not_found:
	ldi r16, 0x01
	ret
cs8900_no_response:
	ldi r16, 0x02
	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_poll
; -----------------------------------------------------------------------------------------
;
cs_poll:

poll_until_no_events:

	rcall cs_Poll_Registers		; polls the Rx, Tx, Buff registers, returning with r18 & r19 the data else 0
	
	cpi r18, 0x0				; check if any events were returned
	brne process_event
	cpi r19, 0x0
	breq poll_registers_ret
	
process_event:
	mov r17, r18
	andi r17, 0x3F				; find the register mask
								; 0x04 = RxEvent
								; 0x08 = TxEvent
								; 0x0C = BuffEvent

	cpi r17, 0x04
	breq CS_Process_RxEvent
	cpi r17, 0x08
	breq CS_Process_TxEvent
	cpi r17, 0x0C	
	breq CS_Process_BuffEvent
				
	rjmp poll_until_no_events

poll_registers_ret:
	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_Process_RxEvent
; -----------------------------------------------------------------------------------------
;
; entry : 	r18 = LSB
;			r19 = MSB
;
CS_Process_RxEvent:
	
	ldi r16, 0x72 ; "r"
	rcall serial_tx
	ldi r16, 0x20 ; " "
	rcall serial_tx

	rcall output_r18_r19
	rcall crlf

	rjmp poll_until_no_events

; -----------------------------------------------------------------------------------------
; CS8900 cs_Process_TxEvent
; -----------------------------------------------------------------------------------------
;
; entry : 	r18 = LSB
;			r19 = MSB
;
CS_Process_TxEvent:
	ldi r16, 0x74 ; "t"
	rcall serial_tx
	ldi r16, 0x20 ; " "
	rcall serial_tx

	rcall output_r18_r19
	rcall crlf

	rjmp poll_until_no_events
	
; -----------------------------------------------------------------------------------------
; CS8900 cs_Process_BuffEvent
; -----------------------------------------------------------------------------------------
;
; entry : 	r18 = LSB
;			r19 = MSB
;
CS_Process_BuffEvent:
	ldi r16, 0x62 ; "b"
	rcall serial_tx
	ldi r16, 0x20 ; " "
	rcall serial_tx

	rcall output_r18_r19
	rcall crlf

	rjmp poll_until_no_events

; -----------------------------------------------------------------------------------------
; CS8900 cs_Poll_Registers
; -----------------------------------------------------------------------------------------
;
; exit : 	r18 = LSB
;			r19 = MSB
cs_Poll_Registers:
	; Poll the Rx and Tx and Buffer event registers with Rx, Tx, Buffer taking proiority.


	; EventMask = 0xFFC0		; mask out the register type
    
	; Read Rx Event register into r18 and r19
    ldi r16,CS_RX_EVENT & 0xFF		; LSB register
	ldi r17,CS_RX_EVENT >> 8		; MSB register
	rcall cs_read_pp
	mov r17, r18					; Keep original value in r18 and operate on r17
	andi r17, 0xC0					; Event Register Mask
	andi r19, 0xFF

	cpi r17,0
	brne poll_return
	cpi r19,0
	brne poll_return

	; nothing was in rx so check tx

    ; Read Tx Event register into r18 and r19
    ldi r16,CS_TX_EVENT & 0xFF		; LSB register
	ldi r17,CS_TX_EVENT >> 8		; MSB register
	rcall cs_read_pp
	mov r17, r18					; Keep original value in r18 and operate on r17
	andi r17, 0xC0					; Event Register Mask
	andi r19, 0xFF

	cpi r17,0
	brne poll_return
	cpi r19,0
	brne poll_return

    ; Read Buffer Event register into r18 and r19
    ldi r16,CS_BUF_EVENT & 0xFF		; LSB register
	ldi r17,CS_BUF_EVENT >> 8		; MSB register
	rcall cs_read_pp
	mov r17, r18					; Keep original value in r18 and operate on r17
	andi r17, 0xC0					; Event Register Mask
	andi r19, 0xFF

	cpi r17,0
	brne poll_return
	cpi r19,0
	brne poll_return

	clr r18
	clr r19
poll_return:
	ret





; -----------------------------------------------------------------------------------------
; CS8900 cs_software_wakeup
; -----------------------------------------------------------------------------------------
;
cs_software_wakeup:

	; wake up the CS8900
	ldi r16,CS_SELF_CTRL & 0xFF		; LSB   CS_PP_PTR = CS_SELF_CTRL
    ldi r17,CS_SELF_CTRL >> 8		; MSB
	sts	CS_PP_PTR, r16
	sts	CS_PP_PTR+1, r17

	ret


; -----------------------------------------------------------------------------------------
; CS8900 cs_software_reset
; -----------------------------------------------------------------------------------------
;
cs_software_reset:

    ; send a software reset 
	ldi r16,CS_SELF_CTRL & 0xFF		; LSB   CS_PP_PTR = CS_SELF_CTRL
    ldi r17,CS_SELF_CTRL >> 8		; MSB
	ldi r18, 0x40					; LSB	with CS_DATA_P0 = 0x0040
	clr r19							; MSB
	rcall cs_write_pp

	ret

; -----------------------------------------------------------------------------------------
; CS8900 test_tx
; -----------------------------------------------------------------------------------------
;
cs_test_tx:


	; The host bids for frame storage by writing the Transmit Command to the TxCMD register
	; (memory base+ 0144h in memory mode and I/O base + 0004h in I/O mode).

	ldi r16,CS_PP_TX_CMD & 0xFF		; LSB   CS_PP_TX_CMD
    ldi r17,CS_PP_TX_CMD >> 8		; MSB
	ldi r18, 0xC0					; LSB	write when all the packet is in the buffer
	clr r19							; MSB
;	rcall cs_write_pp

	; The host writes the transmit frame length to the TxLength register (memory base +
	; 0146h in memory mode and I/O base + 0006h in I/O mode). If the transmit length is
	; erroneous, the command is discarded and the TxBidErr bit (Register 18, BusST, Bit 7) is set.

	rcall cs_find_tx_length

	ldi r16,CS_PP_TX_LEN & 0xFF		; LSB   CS_PP_TX_LEN
    ldi r17,CS_PP_TX_LEN >> 8		; MSB
	mov r18, YH						; LSB	write packet length
	mov r19, YL						; MSB
;	rcall cs_write_pp

	; The host reads the BusST register. This read is performed in memory mode by
	; reading Register 18, at memory base + 0138h. In I/O mode, the host must first set
	; the PacketPage Pointer at the correct location by writing 0138h to the PacketPage
	; Pointer Port (I/O base + 000Ah). The host can then read the BusST register from the
	; PacketPage Data Port (I/O base + 000Ch).

wait_until_tx_ready:
	ldi r16,CS_BUS_STAT & 0xFF		; LSB   CS_BUS_STAT
    ldi r17,CS_BUS_STAT >> 8		; MSB
;	rcall cs_read_pp

	; After reading the register, the Rdy4TxNOW bit (Bit 8) is checked. If the bit is set, the
	; frame can be written. If the bit is clear, the host must continue reading the BusST register
	; (Register 18) and checking the Rdy4TxNOW bit (Bit 8) until the bit is set.

;	bst r19, 0	
;	brbc 6, wait_until_tx_ready

	; When the CS8900A is ready to accept the frame, the host transfers the entire frame from
	; host memory to CS8900A memory using "REP" instruction (REP MOVS starting at memory base 
	; + 0A00h in memory mode, and REP OUT to Receive/Transmit Data Port (I/O base + 0000h) in I/O mode).


	rcall create_tx_packet

	ret


; -----------------------------------------------------------------------------------------
; CS8900 create_tx_packet
; -----------------------------------------------------------------------------------------
;
; 
create_tx_packet:
	;load X with Mac_addr for DA
	ldi	ZL, MAC_addr_DA & 0xff		; set ZL with the lowbyte of the MAC address
	ldi	ZH, MAC_addr_DA >> 8		; set ZH with the highbyte of the MAC address

	ldi r17, 12						; loop count 12 for the 2 x 6 octets 5-0

LoadMAC_DA_SA:							
	ld r16, Z+						; Octet 5 of IA
;	sts CS_DATA_P0, r16
;debug
	rcall output_r16

	dec r17
	cpi r17, 0						; will have performed the loop 12 times
	BRNE LoadMAC_DA_SA

	; Y contains the length
;	sts CS_DATA_P0, YH
;debug
	mov r16, YH
	rcall output_r16
	
;	sts CS_DATA_P0, YL
;debug
	mov r16, YL
	rcall output_r16

	; now output the data
	ldi	ZH, ECONET_RX_BUF >> 8		; set ZH with the highbyte of the Econet receive buffer
	ldi	ZL, ECONET_RX_BUF & 0xff	; set XL with the lowbyte of the Econet receive buffer
	lds	YH, adlc_rx_ptr + 1			; put the Rx pointer address in Y
	lds	YL, adlc_rx_ptr

	clr tmp
	
tx_data_loop:
	ld	r16, Z+						; get byte from RxBuffer, and increment buffer address counter
;	sts CS_DATA_P0, r16
;debug
	rcall output_r16
	
	cp	ZH, YH						; pointer = RxBuffer position high byte 
	brne	tx_data_loop	 		; no, then continue and loop
	
	cp	ZL, YL						; pointer = RxBuffer position low byte
	brne	tx_data_loop			; no, then continue and loop

	rcall crlf

	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_find_tx_length
; -----------------------------------------------------------------------------------------
;
; exit :	YL = LSB of length
;			YH = MSB of length

cs_find_tx_length:

	; Length =	6 bytes DA Address
	;			6 bytes SA Address
	;			2 bytes Length
	;			x bytes LLC data
	; 14 + length
	 
	; find the length of the packet we received

	ldi	ZH, ECONET_RX_BUF >> 8		; set ZH with the highbyte of the Econet receive buffer
	ldi	ZL, ECONET_RX_BUF & 0xff	; set XL with the lowbyte of the Econet receive buffer
	lds	YH, adlc_rx_ptr + 1			; put the Rx pointer address in Y
	lds	YL, adlc_rx_ptr
	
	clc
	sub YL, ZL
	sbc YH, ZH

	; Y now contains the length of the data packet
	; add the header info
	ldi ZL, 14
	clr ZH
	clc

	add YL, ZL
	adc YH, ZH


ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_write_pp
; -----------------------------------------------------------------------------------------
;
; on entry
; 			r16/r17 = address
; 			r18/r19 = data
; on exit
; 			r16/r17 = address
; 			r18/r19 = data

cs_write_pp:
	sts	CS_PP_PTR, 		r16
	sts	CS_PP_PTR+1,	r17
	sts	CS_PP_DATA0, 	r18
	sts	CS_PP_DATA0+1,	r19
	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_read_pp
; -----------------------------------------------------------------------------------------
;
; on entry
; 			r16/r17 = address
; on exit
; 			r16/r17 = address
; 			r18/r19 = data

cs_read_pp:
	sts	CS_PP_PTR, r16
	sts	CS_PP_PTR+1, r17
	lds	r18, CS_PP_DATA0
	lds	r19, CS_PP_DATA0+1
	ret

; -----------------------------------------------------------------------------------------
; CS8900 cs_debug_pp
; -----------------------------------------------------------------------------------------
;
; Will read the two bytes from the PP data register as pointed to by r16 / r17
;
; on entry
; 			r16/r17 = address
; on exit
; 			r18/r19 = data
;
cs_debug_pp:

	rcall cs_read_pp	
	rcall output_r18_r19
	rcall crlf
	ret


; -----------------------------------------------------------------------------------------
; Some general debugging routines to output some registers to the serial port
; -----------------------------------------------------------------------------------------


output_r16:
	rcall serial_tx_hex
	ldi r16, 0x20
	rcall serial_tx
	ret

output_r17:
	mov r16,r17
	rcall serial_tx_hex
	ldi r16, 0x20
	rcall serial_tx
	ret

output_r18:
	mov r16, r18
	rcall serial_tx_hex
	ldi r16, 0x20
	rcall serial_tx
	ret

output_r19:
	mov r16, r19
	rcall serial_tx_hex
	ldi r16, 0x20
	rcall serial_tx
	ret

output_r16_r17:
	rcall output_r16			; output r16
	rcall output_r17			; output r17
	ret

output_r18_r19:
	rcall output_r18
	rcall output_r19
	ret


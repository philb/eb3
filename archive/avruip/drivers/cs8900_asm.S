#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0     /* Required for port labels to work as expected */
#include <avr/io.h>        /* Contains I/O port declarations */

#include "cs8900.h"
#include "egpio.h"


#define serial_tx_arg		r24

#define tmp1			r18
#define zero_reg              r1

#define PACKET_Rx 		0x00
#define CS8900_RX_BUF		0x2000
#define cs8900_state		0x00
#define cs8900_rx_ptr		0x2100



.section .text             ; Start program section

; =======================================================================
; == CS8900==============================================================
; =======================================================================
;
; CS8900 has 16bit registers. Both bytes should be used when reading or writing


output_ChipID:
	ldi	serial_tx_arg, 0x0D		; CR
	rcall	serial_tx
	ldi	serial_tx_arg, 0x0A		; LF
	rcall	serial_tx
	ldi	serial_tx_arg, 0x54		; T
	rcall	serial_tx

	; Information: read Chip Id and Revision
;	chip_type = ReadPPRegister(PP_ChipID);
	ldi	r24, lo8(PP_ChipID)		; LSB register
	ldi	r25, hi8(PP_ChipID)		; MSB register
	rcall	ReadPPRegister

	push	r24
	mov	r24, r25
	rcall	serial_tx_hex
	pop	r24
	rcall	serial_tx_hex

	ldi	serial_tx_arg, 0x20		; space
	rcall	serial_tx
	ldi	serial_tx_arg, 0x52		; R
	rcall	serial_tx

;	chip_rev = ReadPPRegister(PP_ChipRev);
	ldi	r24, lo8(PP_ChipRev)		; LSB register
	ldi	r25, hi8(PP_ChipRev)		; MSB register
	rcall	ReadPPRegister

	push	r24
	mov	r24, r25
	rcall	serial_tx_hex
	pop	r24
	rcall	serial_tx_hex

	ldi	serial_tx_arg, 0x0D		; CR
	rcall 	serial_tx
	ldi	serial_tx_arg, 0x0A		; LF
	rcall 	serial_tx
	ret
	
; -----------------------------------------------------------------------------------------
; CS8900 cs8900_poll_init
; -----------------------------------------------------------------------------------------
;
; initialise CS8900 Ethernet interface
;
; on entry
; on exit
;	r24	= 0	: Success
;		= 1	: Not initialised
;		= 2	: No Response

.global cs8900_poll_init
cs8900_poll_init:
	ldi	r24, EGPIO_ETHER_RESET		; unreset chip
	rcall	egpio_write

	rcall	delay_65536
	rcall	output_ChipID			; send Chip ID to serial

	; step 1: software reset the chip
	rcall	cs_software_reset
	cpi	r24, 0 				; if r24 was successeful r24=0?
	breq	step2
	ret					; return
	
	; step 2 : Set up Ethernet hardware address
	; Add the MAC address to the NIC
step2:

	ldi	r24, lo8(PP_IA)			; LSB register
	ldi	r25, hi8(PP_IA)			; MSB register
	lds	r22, eeprom + 20		; location of MAC address in global variable
	lds	r23, eeprom + 21
	rcall	WritePPRegister
	lds	r22, eeprom + 22
	lds	r23, eeprom + 23
	adiw	r24, 2
	rcall	WritePPRegister
	lds	r22, eeprom + 24
	lds	r23, eeprom + 25
	adiw	r24, 2
	rcall	WritePPRegister

	; step 3 : Configure RxCTL to receive good frames for Indivual Addr, Broadcast, and Multicast.

	ldi	r24, lo8(PP_RxCTL)		; LSB register
	ldi	r25, hi8(PP_RxCTL)		; MSB register
	ldi	r22, lo8(PP_RxCTL_IAHash)	; LSB data
;	clr	r22				; LSB data
	ldi	r23, hi8(PP_RxCTL_RxOK | PP_RxCTL_Multicast | PP_RxCTL_IA | PP_RxCTL_Broadcast)
						; MSB data

;	ldi	r23, PP_RxCTL_Promiscuous|PP_RxCTL_RxOK	; or set to Promiscuous mode to receive all network traffic
	rcall	WritePPRegister

	; step 4: Configure TestCTL 

	ldi	r24, lo8(PP_LineCTL)		; LSB register
	ldi	r25, hi8(PP_LineCTL)		; MSB register
	clr	r22				; LSB data
	ldi	r23, hi8(PP_TestCTL_FDX)	; MSB data	-this should really be taken from the EEPROM config
	rcall	WritePPRegister

	; step 5: Set SerRxOn, SerTxOn in LineCTL

 	; Enable Tx
	ldi	r24, lo8(PP_LineCTL)		; LSB register
	ldi	r25, hi8(PP_LineCTL)		; MSB register
	ldi	r22, lo8(PP_LineCTL_Rx | PP_LineCTL_Tx)
						; LSB data
	clr	r23				; MSB data
	rcall	WritePPRegister

; return codes
	clr	r24
	ret


; -----------------------------------------------------------------------------------------
; CS8900 cs_software_reset
; -----------------------------------------------------------------------------------------
;
; software reset the CS8900 chip
;
; exit : r24 =	0 - Successful
;		1 - reset bit not set

.global cs_software_reset
cs_software_reset:

	; send a software reset 
	ldi 	r24, lo8(PP_SelfCTL)		; LSB   CS8900_PPTR = PP_SelfCTL
	ldi 	r25, hi8(PP_SelfCTL)		; MSB
	ldi 	r22, PP_SelfCtl_Reset		; LSB	with CS8900_RTDATA = 0x0040
	clr 	r23				; MSB
	rcall	WritePPRegister

	; add a short delay while chip resetting in progress
	rcall	delay_65536
	
	; try 3072 times to read the INITD flag
	ldi	YL, 0x00
	ldi	YH, 0x0C

INITD_loop:
	; check the PP_SelfStat_InitD bit to find out if the chip successflly reset
	ldi	r24, lo8(PP_SelfStat)		; LSB register
	ldi	r25, hi8(PP_SelfStat)		; MSB register
	rcall	ReadPPRegister

	bst	r24, 7				; stores bit7 INITD of r22 in T flag 
	brbs	6, cs_reset_complete 		; if set, then the initialisation is complete

	dec	YL				; if not, try again 
	brne	INITD_loop			; Branch if YL<>0
	dec	YH
	brne	INITD_loop			; Branch if YH<>0

	ldi	r24, 1				; failure
	ret

cs_reset_complete:
	clr	r24				; success
	ret





; -----------------------------------------------------------------------------------------
; CS8900 cs8900_poll_send
; -----------------------------------------------------------------------------------------
;
; Transmit a frame in Poll mode
;
; on entry
; 		r24/r25 = length
; exit	
;		r24	= 0 success
;			= 1 Tx bid error
;			= 2 Tx bid not ready for Tx
;			= 3 Tx complete with error


; on entry
;	r24	= packet size LSB
;	r25	= packet size MSB

.global cs8900_poll_send
cs8900_poll_send:

	mov	YL, r24				; get low byte of length
	mov	YH, r25				; get high byte of length

	; Step 1: Write the TX command

	clr	r25				; MSB

	; check for a Previous Bid For Tx
	lds	tmp1, gPrevTxBidFail
	cpi	tmp1, 0x0
	breq	No_prevTxBid
	
	; Previous BidForTX has reserved the Tx FIFO on CS8900, The FIFO must be released before 
	; proceeding. Setting the PP_TxCmd_Force bit will cause CS8900 to release the previously reserved Tx buffer.

	ldi	r24, lo8(PP_TxCmd_TxStart_Full | PP_TxCmd_Force)
						; LSB	write when all the packet is in the buffer and force

	sts	gPrevTxBidFail, zero_reg	; clear the previous bid flag
	rjmp	writeTxCMD

No_prevTxBid:
	ldi	r24, lo8(PP_TxCmd_TxStart_Full)	; LSB	write when all the packet is in the buffer


writeTxCMD:
	sts	CS8900_TxCMD, 	r24
	sts	CS8900_TxCMD+1, r25


	; Step 2: Write the frame length (number of bytes to TX).
	; Note: After the frame length has been written, the CS8900 reserves the Tx buffer for this bid 
	; whether PP_BusStat_TxRDY is set or not.

;	mov	YL, r24
;	mov	YH, r25

	sts	CS8900_TxLEN, YL
	sts	CS8900_TxLEN+1,YH

	; Read BusST to verify it is set as Rdy4Tx.
	ldi	r24, lo8(PP_BusStat)		; LSB 
	ldi	r25, hi8(PP_BusStat)		; MSB
	rcall	ReadPPRegister

	; Step 3: Check for a Tx Bid Error (TxBidErr bit)

	; TxBidErr happens only if Tx_length is too small or too large.
	bst	r24, 7				; PP_BusStat_TxBid
	brbc	6, NoTxBidErr 			; if clear
	
	ldi	r24, 0x1
	ret

	; Step 4: check if chip is ready for Tx now
	; If Bid4Tx not ready, skip the frame

NoTxBidErr:
	bst	r25,0				; PP_BusStat_TxRDY
	brbs	6, TxRDY	 		; if set, Tx ok

	; If not ready for Tx now, abort this frame.
	; Note: Another alternative is to poll PP_BusStat_TxRDY until it is set, if you don't want to abort Tx frames.
	
	; Set to 1, and next time cs8900_poll_send() is called, it can set PP_TxCmd_Force to release the reserved Tx 
	; buffer in the CS8900 chip.
	ldi	r24, 0x01
	sts	gPrevTxBidFail, r24

	inc	r24				; return with 2, Tx Bid not ready 4 Tx
	ret

	; Step 5: copy Tx data into CS8900's buffer. This actually starts the Txmit

TxRDY:

;temporary exit here with r24=0 - this send is now being handled in nic.c
clr	r24
ret

	; Y contains the length
	mov	YL, r22				; get low byte of length
	mov	YH, r23				; get high byte of length

	; add the length to the start address for the loop
	add	YL, ZL
	adc	YH, ZH

tx_data_loop:
	ld	r24, Z+
	sts	CS8900_RTDATA, r24

	cp	ZH, YH				; pointer = end of data High byte?
	brne	skip_LSB			; no, don't bother checking the low byte

	cp	ZL, YL				; ZH=LH at this stage. does pointer low byte = end of data low byte
	breq	tx_data_loop			; yes. Data end reached. Finish

skip_LSB:
	ld	r24, Z+				; output the next byte to the CS_DATA_PORT high byte
	sts	CS8900_RTDATA+1, r24

	cp	ZH, YH				; pointer = end of data High byte?
	brne	tx_data_loop

	cp	ZL, YL				; pointer = end of data low byte?
	brne	tx_data_loop



.global cs8900_poll_send_end
cs8900_poll_send_end:
	
	; Step 6: Poll the TxEvent Reg for the TX completed status
	
	; This step is optional. If you don't wait until Tx compelete, the next time cs8900_poll_send() bids for Tx, 
	; it may encounter Not Ready For Tx because CS8900 is still Tx'ing.

	; try 0x7F00 times to read the Tx Event Register
	ldi	YL, 0x00
	ldi	YH, 0x7F

read_TER:
	; check to see if TxComplete
	ldi	r24, lo8(PP_TER)		; LSB 
	ldi	r25, hi8(PP_TER)		; MSB
	rcall	ReadPPRegister
	
	mov	tmp1, r24

	andi	tmp1, 0x08
	cpi	tmp1, 0x08
	breq	TX_complete

	dec	YL				; if not, try again 
	brne	read_TER			; Branch if YL<>0
	dec	YH
	brne	read_TER			; Branch if YH<>0

TX_complete:
	cpi	r24, 0x08			; no errors flagged in LSB
	brne	Tx_with_error
	cpi	r25, hi8(PP_TER_TxOK)		; just the TxOK set?  *** changed to hi8 ***
	brne	Tx_with_error			
	
	; Tx complete without error, return 0
	clr	r24
	ret

Tx_with_error:
	ldi	r24, 0x03			; Tx complete with error, 
	ret



; -----------------------------------------------------------------------------------------
; CS8900 WritePPRegister
; -----------------------------------------------------------------------------------------
;
; write value to the Packet Pointer register at address offset.
; LSB first MSB second
;
; on entry
; 		r24/r25 = address
; 		r22/r23 = data
; on exit
; 		r24/r25 = address
; 		r22/r23 = data
.globl	WritePPRegister
WritePPRegister:
	sts	CS8900_PPTR, 	r24		; write a 16 bit register offset to IO port CS8900_PPTR
	sts	CS8900_PPTR+1,	r25
	sts	CS8900_PDATA, 	r22		; write 16 bits to IO port number CS8900_PDATA
	sts	CS8900_PDATA+1,	r23
	ret

; -----------------------------------------------------------------------------------------
; CS8900 ReadPPRegister
; -----------------------------------------------------------------------------------------
;
; Read value from the Packet Pointer register at address offset. 
; LSB first MSB second
;
; on entry
; 		r24/r25 = address
; on exit
; 		r24/r25 = data
.globl	ReadPPRegister
ReadPPRegister:
	sts	CS8900_PPTR, 	r24		; write a 16 bit register offset to IO port CS8900_PPTR
	sts	CS8900_PPTR+1, 	r25
	lds	r24, CS8900_PDATA		; read 16 bits from IO port number CS8900_PDATA
	lds	r25, CS8900_PDATA+1
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
; 		r24/r25 = address
; on exit
; 		r24/r25 = data

ReadPPRegisterHiLo:
	sts	CS8900_PPTR, 	r24		; write a 16 bit register offset to IO port CS8900_PPTR
	sts	CS8900_PPTR+1, 	r25
	lds	r25, CS8900_PDATA+1		; read 16 bits from IO port number CS8900_PDATA
	lds	r24, CS8900_PDATA
	ret


; -----------------------------------------------------------------------------------------
; CS8900 delay
; -----------------------------------------------------------------------------------------

delay_65536:
	clr	r24
	clr	r25
delay_loop:
	inc	r25
	brne	delay_loop
	inc	r24
	brne	delay_loop


; -----------------------------------------------------------------------------------------
; CS8900 cs8900SendPacketData
; -----------------------------------------------------------------------------------------
; 
; on entry
; 		r24/r25 = buffer
;		r22/r23 = length
; on exit

.global	cs8900SendPacketData
cs8900SendPacketData:
	mov	r31, r25
	mov	r30, r24
	mov	tmp1, r22
	andi	r22, 0xfe
	clr	r24
	clr	r25
	sbc	r24, r22
	sbc	r25, r23
	breq	done
loop:		
	ld	r20, Z+
	ld	r21, Z+
	sts	CS8900_RTDATA, r20
	sts	CS8900_RTDATA + 1, r21
	adiw	r24, 2
	brcc	loop
done:	
	sbrs	tmp1, 0
	ret
	ld	r20, Z
	sts	CS8900_RTDATA, r20
	ret


; -----------------------------------------------------------------------------------------
; CS8900 cs8900RetreivePacketData
; -----------------------------------------------------------------------------------------
; 
; on entry
; 		r24/r25 = address of the buffer to place the data
;		r22/r23 = address to hold the length of the data


.global	cs8900RetreivePacketData
cs8900RetreivePacketData:
;	mov	r31, r25
;	mov	r30, r24
	movw	r30, r24			; put the buffer address in Z
	mov	tmp1, r22			; store a copy of length destination in tmp1
	andi	r22, 0xfe			; make sure it is even
	clr	r24
	clr	r25
	sbc	r24, r22			
	sbc	r25, r23
	breq	2f
1:	
	lds	r20, CS8900_RTDATA
	lds	r21, CS8900_RTDATA + 1
	st	Z+, r20
	st	Z+, r21
	adiw	r24, 2
	brcc	1b	
2:	
	sbrs	tmp1, 0
	ret
	lds	r20, CS8900_RTDATA
	st	Z, r20
	ret



; -----------------------------------------------------------------------------------------
; CS8900 poll_retrieve
; -----------------------------------------------------------------------------------------
;
.global cs8900_poll_retrieve
cs8900_poll_retrieve:

	; polls the Rx, Tx, Buff registers, returning with r24 & r25 the data else 0
    
	; Read Rx Event register into r24 and r25
	ldi	r24, lo8(PP_RER)			; LSB register
	ldi	r25, hi8(PP_RER)			; MSB register
	rcall	ReadPPRegister			; returned in r24 & r25

 	cpi	r24,ISQ_RxEvent			; result > 0x0004 = RxEvent?
 	cpc	r25,zero_reg			; carry set if it is greater

 	breq	return0

; -----------------------------------------------------------------------------------------
; CS8900 cs_Process_RxEvent
; -----------------------------------------------------------------------------------------
;
; entry : 	r24 = LSB
;		r25 = MSB
;
CS_Process_RxEvent:

	; test for RxOK bit. 
	bst	r25, 0				; test for RxOK bit
	brbs	6, CS_RxOK
	
	; otherwise check the error bits
	bst	r25, 4				; PP_RER_CRC
	brbs	6, Rx_Err_CRC
	bst	r25, 5				; PP_RER_RUNT - frame shorter than 64 bates
	brbs	6, Rx_Err_RUNT
	bst	r25, 6				; PP_RER_EXTRA - frame longer than 1518 bytes
	brbs	6, Rx_Err_EXTRA

skipframe:
	; otherwise unknown error
	rcall	rx_skip	

	ret
	
; -----------------------------------------------------------------------------------------
; CS8900 Rx_Error_CRC
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r24, r25 = Receive Frame Status
Rx_Err_CRC:

	; increment CRC error counter
	rjmp	skipframe	


; -----------------------------------------------------------------------------------------
; CS8900 Rx_Err_RUNT
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r24, r25 = Receive Frame Status
Rx_Err_RUNT:

	; increment RUNT byte underrun error counter
	rjmp	skipframe	


; -----------------------------------------------------------------------------------------
; CS8900 rx_extra
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r24, r25 = Receive Frame Status
Rx_Err_EXTRA:

	; increment Extra byte error counter
	rjmp	skipframe	


rx_skip:

	; Note: Must skip this received error frame. Otherwise, CS8900 hangs here.
	; Read the length of Rx frame
	rcall ReadRxStatusLengthRegister

	; read and then write Skip1 to RxCfg Register and also keeping the current configuration
	ldi	r24, lo8(PP_RxCFG)		; LSB register
	ldi	r25, hi8(PP_RxCFG)		; MSB register
	rcall	ReadPPRegister
	movw	r22,r24

	ori	r22,lo8(PP_RxCFG_Skip1)		; LSB data OR RX_Skip_1
	ldi	r24, lo8(PP_RxCFG)		; LSB register
	ldi	r25, hi8(PP_RxCFG)		; MSB register
	rcall	WritePPRegister

return0:
	clr	r24
	clr	r25
;	sts	cs8900_state, zero_reg		; set state
poll_ret:
poll_until_no_events:
	ret

; -----------------------------------------------------------------------------------------
; CS8900 CS_RxOK
; -----------------------------------------------------------------------------------------
; 
; on entry:
;		r24, r25 = Receive Frame Status

CS_RxOK:
	bst	r25, 1				; received frame had a Destination Address that was accepted by the hash filter
	brbs	6, rx_hashed
	bst	r25, 2				; received frame had a Destination Address which matched the Individual Address
	brbs	6, rx_IndivAdr
	bst	r25, 3				; Broadcast Address
	brbs	6, rx_broadcast	

	rjmp	poll_ret

rx_hashed:
	rcall 	rx_read_frame
	rjmp	poll_ret

rx_IndivAdr:
	rcall 	rx_read_frame
	rjmp	poll_ret

rx_broadcast:
	rcall	rx_read_frame	
	rjmp	poll_ret


rx_read_frame:
	rcall ReadRxStatusLengthRegister

	ret	; rest is currently handled by the C code




	sts 	cs8900_rx_ptr, r24		; save length of packet in the Rx buffer pointer
	sts 	cs8900_rx_ptr+1, r25

	;load Z with Rx Buffer
	ldi	ZL, lo8(CS8900_RX_BUF)		; set ZL with the lowbyte of the Rx buffer
	ldi	ZH, hi8(CS8900_RX_BUF)		; set ZH with the highbyte of the Rx buffer

	mov 	YL, ZL				; copy into Y
	mov	YH, ZH

	add	YL, r24				; add the length
	add	YH, r25

	lds	r24, CS8900_RTDATA		; get the next byte and discard. Seems to be lsb of length

copy_rx_data:
	lds	r24, CS8900_RTDATA		; get the next byte
	st	Z+, r24				; store it in memory

	cp	ZH, YH				; pointer = end of data High byte?
	brne	skip_rx_LSB				; no, don't bother checking the low byte

	cp	ZL, YL				; ZH=LH at this stage. does pointer low byte = end of data low byte
	breq	exit_copy_rx_data_loop		; yes. Data end reached. Finish

skip_rx_LSB:
	lds	r25, CS8900_RTDATA + 1		; get the next byte
	st	Z+, r25

	cp	ZH, YH				; pointer = end of data High byte?
	brne	copy_rx_data

	cp	ZL, YL				; pointer = end of data low byte?
	brne	copy_rx_data

exit_copy_rx_data_loop:

	; reading the RxEvent register signals that the host is finished with this frame.
	; this is done, when returning from this procedure on the next poll

	ldi	r24, PACKET_Rx
	sts	cs8900_state, r24		; set state

	ret





ReadRxStatusLengthRegister:
;	ldi	r24, lo8(PP_Rx_LENGTH)		; LSB register
;	ldi	r25, hi8(PP_Rx_LENGTH)		; MSB register

;	sts	CS8900_PPTR, 	r24		; write a 16 bit register offset to IO port CS8900_PPTR
;	sts	CS8900_PPTR+1, 	r25
;	lds	r25, CS8900_PDATA+1		; read 16 bits from IO port number CS8900_PDATA
;	lds	r24, CS8900_PDATA
;	ret

;	push	r24
;	push	r25
;	mov	tmp1, r24
;	ldi	serial_tx_arg, 0x4F		; O
;	rcall serial_tx
;	mov	serial_tx_arg, tmp1
;	rcall serial_tx_hex
;	ldi	serial_tx_arg, 0x20		; space
;	rcall serial_tx
;	pop	r25
;	pop	r24


	call	ReadRxStatusLengthRegister_alt
	call	ReadRxStatusLengthRegister_alt	
	ret


ReadRxStatusLengthRegister_alt:
	push	r17

	ldi	r24,lo8(CS8900_RTDATA + 1)
	ldi	r25,hi8(CS8900_RTDATA + 1)

	movw	r30,r24
	ld	r24,Z
	ldi	r25,lo8(0)

	mov	r17,r24

	ldi	r24,lo8(CS8900_RTDATA)
	ldi	r25,hi8(CS8900_RTDATA)

	movw	r30,r24
	ld 	r24,Z
	ldi	r25,lo8(0)

	mov	r19,r17
	ldi	r18,lo8(0)
	or	r18,r24
	or	r19,r25

	movw	r24,r18

	pop	r17
	ret
	

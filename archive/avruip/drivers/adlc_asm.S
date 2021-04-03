; =======================================================================
; == ADLC ===============================================================
; =======================================================================

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0	 /* Required for port labels to work as expected */
#include <avr/io.h>	/* Contains I/O port declarations */

#include "adlc.h"
#include "egpio.h"


// The ADLC has four control registers and two status registers.
#define	ADLC_CR1		0
#define	ADLC_CR2		1			// control bit 0
#define	ADLC_CR3		1 			// control bit 1
#define	ADLC_CR4		3
#define	ADLC_SR1		0
#define	ADLC_SR2		1

#define	ADLC_TXCONTINUE	2
#define	ADLC_TXTERMINATE	3
#define	ADLC_RX		2

// control register 1
#define	CR1_AC		0x01		// Address Control. Switch controlling access to CR2 or CR3
#define	CR1_RIE		0x02		// Set to enable Rx interrupts
#define	CR1_TIE		0x04		// Set to enable Tx interrupts
#define	CR1_RDSR_MODE	0x08		// -not used (DMA)
#define	CR1_TDSR_MODE	0x10		// -not used (DMA)
#define	CR1_DISCONTINUE	0x20		// -not used Rx Frame Discontinue
#define	CR1_RXRESET		0x40		// Resets all Rx status and FIFO registers
#define	CR1_TXRESET		0x80		// Resets all Tx status and FIFO registers

// control register 2
#define	CR2_PSE		0x01		// Status bit prioritised
#define	CR2_TWOBYTE		0x02		// Should be set. Causes ADLC to work in two byte mode so that
							// Receive Data Available and Transmit Data Register Available are
							// asserted when two data bytes are ready and two transmit registers
							// available
#define	CR2_FLAGIDLE	0x04		// Determines whether flag fill or mark idle is to be used between
							// packet transfers
#define	CR2_FC		0x08		// Select frame complete or Tx data register (TDRA) ready
#define	CR2_TXLAST		0x10		// Transmit last byte. (Initially set to 0// used to indicate when
							// CRC and closing flag should be transmitted.)
#define	CR2_CLRRX		0x20		// When set Rx status bits are cleared
#define	CR2_CLRTX		0x40		// When set Tx status bits are cleared
#define	CR2_RTS		0x80		// Request to send. Controls the RTS output used to enable the
							// output transmit driver.
// control register 3
#define	CR3_LCF		0x01		// Indicates that no logical control field select byte is included in the packet
#define	CR3_CEX		0x02		// Indicates control field is 8 bits long
#define	CR3_AEX		0x04		// No auto address extend// ADLC assumes 8 bit address. (Although
							// Econet uses 15 bit addresses, the ADLC's auto address extend
							// facility is not used since the high order bit has special 
							// significance.)
#define	CR3_ZERO_ONE	0x08		// Idle. (01/11). set to be all ones
#define	CR3_FDSE		0x10		// - not used Flag Detected Status Enable
#define	CR3_LOOP		0x20		// - not used Loop/Non loop mode
#define	CR3_TST		0x40		// - not used Go Active on Poll / test
#define	CR3_DTR		0x80		// - not used Loop On-Line Control DTR

// control register 4
#define	CR4_DFSF		0x01		// Indicates closing flag of one packet is also opening flag of
							// next. (Irrelevant in Econet systems since reception always 
							// follows transmission.)
#define	CR4_TXWLS		0x06  		// Set Tx word length to 8 bits
#define	CR4_RXWLS		0x18		// Set Rx word length to 8 bits
#define	CR4_ABT		0x20		// - not used Transmit Abort
#define	CR4_ABTEX		0x40		// - not used Abort Extend
#define	CR4_NRZI		0x80		// - not used NRZI/NRZ

// status register 1
#define	SR1_RDA		0x01		// Receive data available - set to confirm that two bytes are
							// available in the receive FIFO data buffer
#define	SR1_S2RQ		0x02		// - not used Status #2 Read request
#define	SR1_LOOP		0x04		// - not used Loop
#define	SR1_FD		0x08		// - not used Flag Detected (when enabled)
#define	SR1_CTS		0x10		// Clear to send. This bit reflects the condition of the CTS input.
							// If enable, a switch of CTS from low to high causes an interrupt.
#define	SR1_TXU		0x20		// - not used
#define	SR1_TDRA		0x40		// Tx data register available. This should be read before sending
							// a byte. When CTS is high, TDRA is deasserted to indicate that
							// transmission should end.
#define	SR1_IRQ		0x80		// IRQ. Interrupts are enabled as appropriage to indicate transmit,
							// receive and error conditions.

// status register 2
#define	SR2_AP		0x01		// Address present. Set to indicate that the first byte of a packet
							// is available for reading.
#define	SR2_FV		0x02		// Frame valid. Indicates packet received and CRC correct.
#define	SR2_RXIDLE		0x04		// Receive idle. Set to cause an IRQ after 15 one bits have been 
							// received.
#define	SR2_RXABT		0x08		// - not used Abort Received
#define	SR2_ERR		0x10		// - not used FCS Error
#define	SR2_DCD		0x20		// Data carrier detect. Reflects the DCD input, showing the state
							// of the clock signal. If enabled, an interrupt is generated if 
							// the clock fails.
#define	SR2_OVRN		0x40		// - not used Rx Overun
#define	SR2_RDA		0x80		// Receive data available. Used after a receive interrupt to confirm
							// that the data is acutally ready. Other causes of an interrupt
							// indicate an error condition.

// initial values for Control Registers
//#define	CR2val		CR2_PSE //| CR2_TWOBYTE

#define	CR2val		CR2_CLRTX | CR2_CLRRX | CR2_FLAGIDLE | CR2_TWOBYTE | CR2_PSE

// =======================================================================
// =Memory Map SRAM ====================================================
// =======================================================================


#define adlc_rx_ptr_lo		r12
#define adlc_rx_ptr_hi		r13

#define adlc_state		r15
#define tmp1			r18
#define tmp2			r19
#define saved_gicr		r20

#define egpio_arg			r24
#define serial_tx_arg		r24

#define zero_reg			r1

.section .text					; Start program section

addr_setup:
	cbi	PORTD, ADLC_RS0			; clear RS0 on PORTD
	cbi	PORTD, ADLC_RS1			; clear RS1 on PORTD
	sbrc	tmp1, 0				; if bit 0 in the address field is not clear
	sbi	PORTD, ADLC_RS0			;	set bit 0
	sbrc	tmp1, 1				; if bit 1 in the address field is not clear
	sbi	PORTD, ADLC_RS1			;	set bit 1
	ret

; -----------------------------------------------------------------------------------------
; ADLC Read
; -----------------------------------------------------------------------------------------
;
; tmp1=address
; tmp2=data

adlc_read_rx:
	ldi	tmp1, ADLC_RX
adlc_read:
	rcall   addr_setup
	sbi	PORTD, ADLC_RnW			; Read from ADLC

	ldi	tmp1, 0x1				; make sure the ADLC_CLK at PB0 stays set
	out	DDRB, tmp1				; clear all other lines on PORTB
	cbi	DDRE, ADLC_D0			; clear ADLC_D0 line on PORTE
	ldi	tmp1, 0xff
	out	PORTB, tmp1
	sbi	PORTE, ADLC_D0

	rcall	go_cycle

	bst	tmp1, ADLC_D0			; Store the bit from ADLC_D0 in the T flag
	bld	tmp2, 0				; load T into bit 0 of tmp2 to leave the byte read in tmp2

	ldi	tmp1, 0xff				; set pins back to output
	out	DDRB, tmp1				; Port B
	sbi	DDRE, ADLC_D0			; ADLC_DO pin on PORT E
	ret

; -----------------------------------------------------------------------------------------
; ADLC Write
; -----------------------------------------------------------------------------------------
;
; tmp1=address
; tmp2=data

adlc_write_cr1:
	ldi	tmp1, ADLC_CR1
	rjmp	adlc_write
adlc_write_cr2:	
	ldi	tmp1, ADLC_CR2
adlc_write:
	rcall	addr_setup

	cbi	PORTD, ADLC_RnW			; write to ADLC

	; set the data
	out	PORTB, tmp2				; put the data on PORT B
	cbi	PORTE, ADLC_D0			; clear bit 2 on PORT E
	sbrc	tmp2, 0				; if bit 0 in the data field is not clear
	sbi	PORTE, ADLC_D0			;	set bit to 1

go_cycle:
	clc
	
	in	saved_gicr, GICR			; save the contents of the General Interrupt Control Register
	ldi	r24, (1 << INT1)			; External INT1 enable
adlc_write_prewait:
	sbic	PIND, 3
	rjmp	adlc_write_prewait
	out	GIFR, r24				; write to the General Interrupt Flag Register
	out	GICR, r24				; write to the General Interrupt Control Register

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done
	brcs	adlc_write_done

adlc_write_done:
	ret

; -----------------------------------------------------------------------------------------
; ADLC access
; -----------------------------------------------------------------------------------------
;
; perform one read or write access cycle
.global __vector_2
__vector_2:
	rjmp	1f
1:	cbi	PORTE, ADLC_nCE			; Clear the ADLC_nCE
	rjmp	1f
1:	rjmp	1f
1:	nop	
	in	tmp2, PINB				; read PortB to tmp2
	in	tmp1, PINE				; read PortE to tmp1
	nop
	sbi	PORTE, ADLC_nCE			; set ADLC_nCE to flag read completed
	out	GICR, saved_gicr			; restore the contents of the Global Interrupt Control Register
	sec						; set carry to flag interrupt is handled
	reti						; return from interrupt

	.globl	adlc_await_idle
adlc_await_idle:
	; switch to 1-byte mode, no PSE
	ldi	tmp2, CR2_CLRRX
	rcall	adlc_write_cr2				; write to Control Register

idle_wait_loop:	
	ldi	tmp1, ADLC_SR2
	rcall	adlc_read
	bst	tmp2, 2				; 4 ;Idle?
	brbc	6, idle_wait_loop

	clr	r24
	ret
	

; -----------------------------------------------------------------------------------------
; ADLC Tx frame
; -----------------------------------------------------------------------------------------
;
	;;  r24, r25  pointer to first mbuf
	;;  r22=0 -> this is a data frame, skip the cb and scout bytes

.globl	adlc_tx_frame
adlc_tx_frame:
	push	YH
	push	YL
	movw	YL, r24

	ldi	r24, EGPIO_ECONET_GREEN
	rcall	egpio_write

	ldi	r23, 5
	; go on the wire and start transmitting
	ldi	tmp2, CR1_RXRESET
	rcall	adlc_write_cr1
	
	ldi	tmp2, CR2_RTS | CR2_FLAGIDLE
	rcall	adlc_write_cr2

tx_one_mbuf:
	ld	XL, Y
	ldd	XH, Y+1
	ldd	r21, Y+4
	adiw	YL, 6

await_tdra:	
	ldi	tmp1, ADLC_SR1
	rcall	adlc_read
	bst	tmp2, 6				; TDRA?
	brbs	6, got_tdra
	cpi	tmp2, 0
	breq	await_tdra

	mov	r22, tmp2
	ldi	serial_tx_arg, 0x45
	rcall	serial_tx
	mov	serial_tx_arg, r22
	rcall	serial_tx_hex
	ldi	r22, 1
	rjmp	abandon_tx

got_tdra:
	cpi	r22, 0
	brne	not_data
	dec	r23
	brne	not_data
	adiw	YL, 2
	subi	r21, 2
	
not_data:
	ld	tmp2, Y+
	dec	r21
	brne	not_end
	cpi	XL, 0
	cpc	XH, zero_reg
	brne	not_end
	
tx_end:	
	ldi	tmp1, ADLC_TXTERMINATE
	rcall	adlc_write

	ldi	tmp2, CR2_FC | CR2val
	rcall	adlc_write_cr2

	; enable receiver
	ldi	tmp2, 0
	rcall	adlc_write_cr1

await_end:	
	ldi	tmp1, ADLC_SR1
	rcall	adlc_read
	bst	tmp2, 6				; FC?
	brbc	6, await_end

abandon_tx:	
	ldi	tmp1, (RX_SCOUT_ACK | RX_IDLE)
	sbrs	r22, 0
	ldi	tmp1, (RX_DATA_ACK | RX_IDLE)	
	mov	adlc_state, tmp1

	rcall	init_rx_buf

	; reset tx
	ldi	tmp2, CR1_RIE | CR1_TXRESET	; Enable Receive interrupts | Reset the TX status
	rcall	adlc_write_cr1			; write to the ADLC

	ldi	r24, EGPIO_ECONET_GREEN | EGPIO_SET
	rcall	egpio_write

	pop	YL
	pop	YH
	ret

not_end:	
	ldi	tmp1, ADLC_TXCONTINUE
	rcall	adlc_write
	cpi	r21, 0
	brne	await_tdra
	movw	YL, XL
	ori	r22, 2
	rjmp	tx_one_mbuf

; -----------------------------------------------------------------------------------------
; ADLC rx_overrun
; -----------------------------------------------------------------------------------------
;
rx_overrun:
	ldi	serial_tx_arg, 0x6F		; "o" - overrun
	rcall serial_tx
	rcall	adlc_ready_to_receive_scout
	rjmp	adlc_irq_ret
	
; -----------------------------------------------------------------------------------------
; ADLC process_dcd - No Clock
; -----------------------------------------------------------------------------------------
;
process_dcd:
	ldi	serial_tx_arg, 0x6E		; "n" - no clock
	rcall	serial_tx				; 
;	rcall	noclock
	rjmp	adlc_irq_ret			; return from interrupt


; -----------------------------------------------------------------------------------------
; ADLC rx_error	Receive Error
; -----------------------------------------------------------------------------------------
;
rx_error:
	ldi	serial_tx_arg, 0x65		; "e" - error
	rcall	serial_tx			; 
	rcall	adlc_rx_flush			; clear any remaing bytes

; -----------------------------------------------------------------------------------------
; ADLC abandon_rx Abandon Rx
; -----------------------------------------------------------------------------------------
;
; exit: adlc_state = 0
; 
abandon_rx:
	; abandon in-progress reception
	rcall	adlc_clear_rx			; clear rx status
	rjmp	process_s2rq			; might be AP as well

; -----------------------------------------------------------------------------------------
; ADLC process_idle
; -----------------------------------------------------------------------------------------
;
; exit: adlc_state = 0
; 
process_idle:
	rcall	adlc_clear_rx			; clear Rx Status Register
	ldi	r24, EGPIO_ECONET_GREEN | EGPIO_SET
	rcall	egpio_write
	rjmp	adlc_irq_ret			; clean up routine for interrupt handling

; -----------------------------------------------------------------------------------------
; ADLC IRQ main entry point
; -----------------------------------------------------------------------------------------
;
.global __vector_1
__vector_1:	
	push	r24					; save register values to the stack
	push	tmp1
	push	tmp2
	in	tmp1, SREG				; Save the status register
	push	tmp1
	push	XH
	push	XL

	; disable adlc irq and re-enable global interrupts
	out	GICR, zero_reg
	sei

process_s2rq:	
	ldi	tmp1, ADLC_SR2			; set address to Status Register2
	rcall	adlc_read				; read Status Register2

	bst	tmp2, 0				; 1 ;AP: Address Present?
	brbs	6, process_ap			;   ;yes
	bst	tmp2, 7				; 80;RDA? Receive Data Available
	brbs	6, process_rda			;   ;yes
	bst	tmp2, 1				; 2 ;FV: Frame Valid?
	brbs	6, process_fv			;   ;yes
	bst	tmp2, 6				; 40;overrun? Rx Overrun
	brbs	6, rx_overrun			;   ;yes
	bst	tmp2, 5				; 20;DCD? signifies presence of clock
	brbs	6, process_dcd			;   ;yes
	bst	tmp2, 4				; 10;CRC error?
	brbs	6, rx_error				;   ;yes
	bst	tmp2, 3				; 8 ;Abort? Abort Received?
	brbs	6, abandon_rx			;   ;yes
	bst	tmp2, 2				; 4 ;Idle?: Receive Idle?
	brbs	6, process_idle			;   ;yes
	rjmp	adlc_irq_ret

; -----------------------------------------------------------------------------------------
; ADLC process_fv Frame Valid
; -----------------------------------------------------------------------------------------
;
;
; exit : adlc_state = FRAME COMPLETE
; 
process_fv:
	rcall	adlc_rx_flush			; clear remaining bytes in the input buffer

	mov	tmp1, adlc_state
	ori	tmp1, FRAME_COMPLETE
	mov	adlc_state, tmp1			; set ADLC state to FRAME COMPLETE

	rjmp	adlc_irq_ret			; return from interrupt


; -----------------------------------------------------------------------------------------
; ADLC process_ap Address Present
; -----------------------------------------------------------------------------------------
;
; The address field is the first 8 bits following the opening flag, so will always be the
; first byte of the packet. This byte being present causes the Rx Interrupt if enabled.

discontinue_again:
	ldi	tmp2, CR1_TXRESET | CR1_RIE | CR1_DISCONTINUE
	rcall	adlc_write_cr1
	rjmp	adlc_irq_ret	
	
process_ap:
	ldi	tmp1, DISCONTINUED
	cp	tmp1, adlc_state
	breq	discontinue_again
	
	ldi	r24, EGPIO_ECONET_GREEN
	rcall	egpio_write

	movw	XL, adlc_rx_ptr_lo

	; Read 1 byte to the buffer
	rcall	adlc_read_rx			; read ADLC, tmp2 = data read

	st	X+, tmp2				; store the data read in the Rx buffer

	movw	adlc_rx_ptr_lo, XL

	inc	adlc_state

	rjmp	adlc_irq_ret

; -----------------------------------------------------------------------------------------
; ADLC process_rda Receive Data Available
; -----------------------------------------------------------------------------------------
;
is_full:
	ldi	serial_tx_arg, 0x4f
	rcall	serial_tx
	mov	serial_tx_arg, XH
	rcall	serial_tx_hex
	mov	serial_tx_arg, XL
	rcall	serial_tx_hex
	rjmp	discontinue

process_rda:
	movw	XL, adlc_rx_ptr_lo

	; read 2 bytes to the buffer

	; 1st byte
	rcall	adlc_read_rx			; read ADLC, tmp2 = data read
	st	X+, tmp2				; store the data read in the Rx buffer

	; 2nd byte
	rcall	adlc_read_rx			; read ADLC, tmp2 = data read
	st	X+, tmp2				; store the data read in the Rx buffer

	movw	adlc_rx_ptr_lo, XL

	ldi	tmp1, lo8(ECONET_RX_BUF + ECONET_RX_BUF_SIZE)
	ldi	tmp2, hi8(ECONET_RX_BUF + ECONET_RX_BUF_SIZE)
	cp	XL, tmp1
	cpc	XH, tmp2
	brcc	is_full

	;;  if we're receiving payload, we already decided this packet was good.  Nothing more to do.
	mov	tmp1, adlc_state
	andi	tmp1, 15
	cpi	tmp1, RX_PAYLOAD
	breq	adlc_irq_ret

	inc	adlc_state

	;;  if this was the first RDA event for a frame, we need more data.
	cpi	tmp1, RX_CHECK_NET1
	breq	adlc_irq_ret

	;;  now we have the full destination address, check if this is a packet we wanted
	cpi	tmp1, RX_CHECK_NET2
	breq	check_dst_net

	;; if we get here, something is wrong.  Reset and start over.
	rjmp	discontinue

check_dst_net:	
	lds	tmp1, ECONET_RX_BUF + 1
	rcall	interesting_network
	brcs	adlc_irq_ret
	breq	discontinue

	lds	tmp2, eeprom + 6
	lds	tmp1, ECONET_RX_BUF
	cp	tmp2, tmp1
	breq	adlc_irq_ret

discontinue:
	ldi	tmp2, CR1_TXRESET | CR1_RIE | CR1_DISCONTINUE
	rcall	adlc_write_cr1

	ldi	tmp1, DISCONTINUED
	mov	adlc_state, tmp1

	; reset rx buffer pointer
	ldi	XL, lo8(ECONET_RX_BUF)		; set XL with the lowbyte of the Econet receive buffer
	ldi	XH, hi8(ECONET_RX_BUF)		; set XH with the highbyte of the Econet receive buffer

	movw	adlc_rx_ptr_lo, XL

	rjmp	adlc_irq_ret


; -----------------------------------------------------------------------------------------
; ADLC Rx Flush
; -----------------------------------------------------------------------------------------
	
adlc_rx_flush:
	; switch to 1-byte mode, no PSE
	ldi	tmp2, CR2_FLAGIDLE | CR2_RTS
	rcall	adlc_write_cr2			; write to Control Register

	ldi	tmp2, 0
	rcall	adlc_write_cr1

	; read SR2
	ldi	tmp1, ADLC_SR2			; read Status Register 2
	rcall	adlc_read				; read Satus Register into tmp2

	;if there is a byte to read, read it first before clearing
	
	bst	tmp2, 7				; RDA? is receive data available
	brbc	6, no_rda				; no

	movw	XL, adlc_rx_ptr_lo

	; Read 1 byte to the buffer
	rcall	adlc_read_rx			; read ADLC, tmp2 = data read

	st	X+, tmp2				; store the data read in the Rx buffer

	movw	adlc_rx_ptr_lo, XL

no_rda:
	ldi	tmp2, CR2_FLAGIDLE | CR2_RTS | CR2_CLRRX | CR2_PSE | CR2_TWOBYTE
	rjmp	adlc_write_cr2				; write to Control Register


; -----------------------------------------------------------------------------------------
; ADLC clear Rx
; -----------------------------------------------------------------------------------------
;
; clear rx status and switch back to 1-byte mode
;

adlc_clear_rx:
	clr	adlc_state
	ldi	tmp2, CR2val			; reset Receieve status bits
	rjmp	adlc_write_cr2			; write to ADLC

; -----------------------------------------------------------------------------------------
; ADLC IRQ ret
; -----------------------------------------------------------------------------------------

adlc_irq_ret:
	pop	XL
	pop	XH
	
	ldi	r24, (1 << INT0)			; restore the interrupt flag
	out	GICR, r24
 	pop	tmp1					; restore Status register from the Stack
	out	SREG, tmp1		
	pop	tmp2					; restore other registers from the Stack
	pop	tmp1
	pop	r24
	reti						; return from interrupt

init_rx_buf:	
	; initialise the Rx Buffer pointer
	ldi	XL, lo8(ECONET_RX_BUF)		; set XL with the lowbyte of the Econet receive buffer
	ldi	XH, hi8(ECONET_RX_BUF)		; set XH with the highbyte of the Econet receive buffer

	movw	adlc_rx_ptr_lo, XL

	ret
	
; -----------------------------------------------------------------------------------------
; ADLC init
; -----------------------------------------------------------------------------------------

.global adlc_init
adlc_init:
	; During a power-on sequence, the ADLC is reset via the RESET input and internally 
	; latched in a reset condition to prevent erroneous output transitions. The four control
	; registers must be programmed prior to the release of the reset condition. The release
	; of the reset condition is peformed by software by writing a "0" into the Rx RS control
	; bit (receiver) and/or Tx RS control bit (transmitter). The release of the reset condition
	; must be done after the RESET input has gone high.
	; At any time during operation, writing a "1" into the Rx RS control bit or Tx RS control
	; bit causes the reset condition of the receiver or the transmitter.

	; raise the reset line on the ADLC
	ldi	egpio_arg, EGPIO_ADLC_RESET | EGPIO_SET	; set the ADLC reset to 1
	rcall	egpio_write

	; set the initial values of Control Register 1
	ldi	tmp2, CR1_AC | CR1_TXRESET | CR1_RXRESET	; Switch controlling access to CR3
							; Resets all Tx, Rx status and FIFO registers
							; these will automatically go back to 0
	rcall	adlc_write_cr1				; write to ADLC

	; weird voodoo write copied from acorn code
	ldi	tmp2, 0x1E								
	ldi	tmp1, ADLC_TXTERMINATE		; write to TXTerminate address
	rcall	adlc_write

	; set the initial values of Control Register 3 (CR1_AC previously set)
	clr	tmp2					; Set initial value to 0
	ldi	tmp1, ADLC_CR3			; set to ADLC Control Register 3
	rcall	adlc_write				; write to ADLC

	; set the initial values of Control Register 4
	ldi	tmp2, CR4_TXWLS | CR4_RXWLS	; Set Tx & Rx Word lengths to 8 bits
	ldi	tmp1, ADLC_CR4			; set to ADLC Control Register 4
	rcall	adlc_write				; write to ADLC

	ldi	tmp2, CR1_TXRESET | CR1_RXRESET ;  keep transmitter and receiver reset, irqs off, back to CR2
	rcall	adlc_write_cr1			; write to the ADLC

	;;  fall through
	
; -----------------------------------------------------------------------------------------
; ADLC ready to receive
; -----------------------------------------------------------------------------------------
;
; exit	: adlc_state = 0
; 		: adlc_rx_ptr	= ECONET RX Buffer LSB
; 		: adlc_rx_ptr + 1	= ECONET RX Buffer MSB

.global	adlc_ready_to_receive_scout
adlc_ready_to_receive_scout:
	ldi	r24, RX_SCOUT
	
.global	adlc_ready_to_receive
adlc_ready_to_receive:
	;initialise the adlc_state
	mov	adlc_state, r24

	rcall	init_rx_buf

	ldi	tmp2, CR1_TXRESET			; reset tx, unreset rx
	rcall	adlc_write_cr1			; write to the ADLC

	ldi	tmp2, CR2val			; Status bit prioritised
	rcall	adlc_write_cr2			; write to the ADLC

	ldi	tmp2, CR1_RIE | CR1_TXRESET	; Enable Receive interrupts | Reset the TX status
	rcall	adlc_write_cr1			; write to the ADLC

	ldi	r24, EGPIO_ECONET_GREEN | EGPIO_SET
	rjmp	egpio_write

; -----------------------------------------------------------------------------------------
; Examine the network number and figure out whether it's something we are interested in.
; -----------------------------------------------------------------------------------------
;
;  entry: tmp1=network number
;  exit:  C set -> do want this frame
;	 Z set -> don't want this frame
;	  C,Z both clear -> need to check station number too 

interesting_network:
	cpi	tmp1, 0xff
	breq	yes_network
	cpi	tmp1, 0x0
	breq	maybe_network
	push	YL
	push	YH
	ldi	YL, lo8(rTableEthType)
	ldi	YH, hi8(rTableEthType)
	add	YL, tmp1
	adc	YH, zero_reg
	ld	tmp1, Y
	cpi	tmp1, 0
	pop	YH
	pop	YL
	brne	yes_network
	clc
	ret

maybe_network:	
	clc
	clz
	ret

yes_network:
	sec
	clz
	ret

	.globl	get_adlc_state
get_adlc_state:
	mov	r24, adlc_state
	ret

	.globl	get_adlc_rx_ptr
get_adlc_rx_ptr:
	movw	r24, adlc_rx_ptr_lo
	ret
	

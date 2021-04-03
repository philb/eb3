; =======================================================================
; == ADLC ===============================================================
; =======================================================================

; -----------------------------------------------------------------------------------------
; ADLC Read
; -----------------------------------------------------------------------------------------
;
; r16=address
; r17=data

adlc_read:
	cbi	PORTD, ADLC_RS0			; clear RS0 on PORTD
	cbi	PORTD, ADLC_RS1			; clear RS1 on PORTD
	sbrc	r16, 0				; if bit 0 in the address field is not clear
	sbi	PORTD, ADLC_RS0			;    set bit 0
	sbrc	r16, 1				; if bit 1 in the address field is not clear
	sbi	PORTD, ADLC_RS1			;    set bit 1
	sbi	PORTD, ADLC_RnW			; Read from ADLC

	ldi	r16, 0x1				; make sure the ADLC_CLK at PB0 stays set
	out	DDRB, r16				; clear all other lines on PORTB
	cbi	DDRE, ADLC_D0			; clear ADLC_D0 line on PORTE

	clc
	
	in	r18, GICR				; save the contents of the General Interrupt Control Register
	ldi	tmp, (1 << INT1)			; External INT1 enable
	out	GIFR, tmp				; write to the General Interrupt Flag Register
	out	GICR, tmp				; write to the General Interrupt Control Register


adlc_read_wait:					; wait for adlc_access to run
	brcc	adlc_read_wait			; r16 and r17 will return with PortB and PortE
	
	bst	r16, ADLC_D0			; Store the bit from ADLC_D0 in the T flag
	bld	r17, 0				; load T into bit 0 of r17 to leave the byte read in r17

	ldi	r16, 0xff				; set pins back to output
	out	DDRB, r16				; Port B
	sbi	DDRE, ADLC_D0			; ADLC_DO pin on PORT E
	ret


; -----------------------------------------------------------------------------------------
; ADLC Write
; -----------------------------------------------------------------------------------------
;
; r16=address
; r17=data

adlc_write:
	; set the addressing
	cbi	PORTD, ADLC_RS0			; clear RS0 on PORTD -
	cbi	PORTD, ADLC_RS1			; clear RS1 on PORTD - ADLC Rx
	sbrc	r16, 0				; if bit 0 in the address field is not clear
	sbi	PORTD, ADLC_RS0			;    set bit 0
	sbrc	r16, 1				; if bit 1 in the address field is not clear
	sbi	PORTD, ADLC_RS1			;    set bit 1
	cbi	PORTD, ADLC_RnW			; write to ADLC

	; set the data
	out	PORTB, r17				; put the data on PORT B
	cbi	PORTE, ADLC_D0			; clear bit 2 on PORT E
	sbrc	r17, 0				; if bit 0 in the data field is not clear
	sbi	PORTE, ADLC_D0			;    set bit to 1

	clc
	
	in	r18, GICR				; save the contents of the General Interrupt Control Register
	ldi	tmp, (1 << INT1)			; External INT1 enable
	out	GIFR, tmp				; write to the General Interrupt Flag Register
	out	GICR, tmp				; write to the General Interrupt Control Register

adlc_write_wait:					; wait for adlc_access to run
	brcc adlc_write_wait			; by checking if carry is cleared
	ret


; -----------------------------------------------------------------------------------------
; ADLC access
; -----------------------------------------------------------------------------------------
;
; perform one read or write access cycle

adlc_access:
	nop
	nop
	cbi	PORTE, ADLC_nCE			; Clear the ADLC_nCE
	nop	
	nop
	nop
	nop
	nop
	in	r17, PINB				; read PortB to r17
	in	r16, PINE				; read PortE to r16
	nop
	sbi	PORTE, ADLC_nCE			; set ADLC_nCE to flag read completed
	out	GICR, r18				; restore the contents of the Global Interrupt Control Register
	sec							; set carry to flag interrupt is handled
	reti						; return from interrupt

; -----------------------------------------------------------------------------------------
; ADLC Tx frame
; -----------------------------------------------------------------------------------------
;

	; XL/XH pointer to start of frame to transmit
	; YL/YH pointer to end of frame
adlc_tx_frame:
	ldi	r21, 0
	; go on the wire and start transmitting
	ldi	r16, ADLC_CR1
	ldi	r17, CR1_RXRESET
	rcall	adlc_write
	
	ldi	r16, ADLC_CR2
	ldi	r17, CR2_RTS | CR2_FLAGIDLE
	rcall	adlc_write

await_tdra:	
	ldi	r16, ADLC_SR1
	rcall	adlc_read
	;;  XXX check for lost CTS or DCD here
	bst	r17, 6				; TDRA?
	brbc	6, await_tdra

skip:	
	ld	r17, X+
	cpi	r20, 1
	brne	not_data
	inc	r21
	cpi	r21, 5
	breq	skip
	cpi	r21, 6
	breq	skip
	
not_data:	
	cp	XL, YL
	brne	not_end
	cp	XH, YH
	breq	tx_end
	
not_end:	
	ldi	r16, ADLC_TXCONTINUE
	rcall	adlc_write
	rjmp	await_tdra

tx_end:	
	ldi	r16, ADLC_TXTERMINATE
	rcall	adlc_write

	ldi	r16, ADLC_CR2
	ldi	r17, CR2_FC | CR2val
	rcall	adlc_write

	; enable receiver
	ldi	r16, ADLC_CR1
	ldi	r17, 0
	rcall	adlc_write

await_end:	
	ldi	r16, ADLC_SR1
	rcall	adlc_read
	bst	r17, 6				; FC?
	brbc	6, await_end

	; reset tx
	ldi	r16, ADLC_CR1
	ldi	r17, CR1_TXRESET
	rjmp	adlc_write

; -----------------------------------------------------------------------------------------
; ADLC rx_overrun
; -----------------------------------------------------------------------------------------
;
rx_overrun:
	ldi	r16, 0x6F				; "o" - overrun
	rcall serial_tx				; 
	rjmp 	abandon_rx

; -----------------------------------------------------------------------------------------
; ADLC process_dcd - No Clock
; -----------------------------------------------------------------------------------------
;
process_dcd:
	ldi	r16, 0x6E				; "n" - no clock
	rcall serial_tx				; 
;	rcall noclock
	rjmp	adlc_irq_ret			; return from interrupt


; -----------------------------------------------------------------------------------------
; ADLC rx_error	Receive Error
; -----------------------------------------------------------------------------------------
;
rx_error:
	ldi		r16, 0x65			; "e" - error
	rcall 	serial_tx			; 
	rcall	adlc_rx_flush			; clear any remaing bytes

; -----------------------------------------------------------------------------------------
; ADLC abandon_rx Abandon Rx
; -----------------------------------------------------------------------------------------
;
; exit: adlc_state = 0
; 
abandon_rx:
	; abandon in-progress reception
	clr	adlc_state
	rcall	adlc_clear_rx			; clear rx status
	rjmp	process_s2rq			; might be AP as well

; -----------------------------------------------------------------------------------------
; ADLC process_idle
; -----------------------------------------------------------------------------------------
;
; exit: adlc_state = 0
; 
process_idle:					
	ldi	r16, 0x69				; append "i" - network is idle
	call	serial_tx				; write to serial
	rcall	crlf

	clr	adlc_state
	
	rcall	adlc_clear_rx			; clear Rx Status Register
	rjmp	adlc_irq_ret			; clean up routine for interrupt handling

; -----------------------------------------------------------------------------------------
; ADLC IRQ main entry point
; -----------------------------------------------------------------------------------------
;
adlc_irq:	
	push	tmp					; save register values to the stack
	push	r16
	push	r17
	in	r16, SREG				; Save the status register
	push	r16

	; disable adlc irq and re-enable global interrupts
	clr	tmp
	out	GICR, tmp
	sei

	ldi	r16, ADLC_SR1			; set read address to ADLC Status Register1
	rcall	adlc_read				; read the ADLC Status Register into r17

	bst	r17, 0				; RDA? Receive data available
	brbs	6, process_rda			; yes
	bst	r17, 1				; S2RQ - Status #2 Read Request ?
	brbs	6, process_s2rq			; yes

;this hasn't occurred so far
	ldi	r16, 0x56				; "V" - valid CRC is Valid
	rcall	serial_tx				; write to serial

	rjmp	adlc_irq_ret			; clean up routine for interrupt handling


process_s2rq:
	ldi	r16, ADLC_SR2			; set address to Status Register2
	rcall	adlc_read				; read Status Register2

	bst	r17, 0				; 1 ;AP: Address Present?
	brbs	6, process_ap			;   ;yes
	bst	r17, 7				; 80;RDA? Receive Data Available
	brbs	6, process_rda			;   ;yes
	bst	r17, 6				; 40;overrun? Rx Overrun
	brbs	6, rx_overrun			;   ;yes
	bst	r17, 5				; 20;DCD? signifies presence of clock
	brbs	6, process_dcd			;   ;yes
	bst	r17, 4				; 10;CRC error?
	brbs	6, rx_error				;   ;yes
	bst	r17, 3				; 8 ;Abort? Abort Received?
	brbs	6, abandon_rx			;   ;yes
	bst	r17, 2				; 4 ;Idle?: Receive Idle?
	brbs	6, process_idle			;   ;yes
	bst	r17, 1				; 2 ;FV: Frame Valid?
	brbs	6, process_fv			;   ;yes

; -----------------------------------------------------------------------------------------
; ADLC process_fv Frame Valid
; -----------------------------------------------------------------------------------------
;
;
; exit : adlc_state = FRAME COMPLETE
; 
process_fv:
	rcall	adlc_rx_flush			; clear remaining bytes in the input buffer

	ldi	r16, FRAME_COMPLETE		;
	mov	adlc_state, r16			; set ADLC state to FRAME COMPLETE

	rjmp	adlc_irq_ret			; return from interrupt


; -----------------------------------------------------------------------------------------
; ADLC process_ap Address Present
; -----------------------------------------------------------------------------------------
;
; The address field is the first 8 bits following the opening flag, so will always be the
; first byte of the packet. This byte being present causes the Rx Interrupt if enabled.

process_ap:
;	ldi	r16, 0x61				; "a"
;	call	serial_tx				; send to serial

	lds	XL, adlc_rx_ptr			; put the Rx buffer pointer address in X
	lds	XH, adlc_rx_ptr + 1

	; Read 1 byte to the buffer
	ldi	r16, ADLC_RX			; set read address to ADLC_RX (2) 
	rcall	adlc_read				; read ADLC, r17 = data read

	st	X+, r17				; store the data read in the Rx buffer

	sts	adlc_rx_ptr, XL			; store the incremented Rx buffer pointer
	sts	adlc_rx_ptr + 1, XH		; 

	inc	adlc_state

;delay	
	ldi	r16, 160				; set up delay loop
delay:
	dec	r16					; minus 1
	brne	delay					; loop until finished

	rjmp	process_s2rq			; continue processing Status Register 2 until finished


; -----------------------------------------------------------------------------------------
; ADLC process_rda Receive Data Available
; -----------------------------------------------------------------------------------------
;

process_rda:
	lds	XL, adlc_rx_ptr			; put the Rx buffer pointer address in X
	lds	XH, adlc_rx_ptr + 1

	; read 2 bytes to the buffer

	; 1st byte
	ldi	r16, ADLC_RX			; set read address to ADLC_RX (2) 
	rcall	adlc_read				; read ADLC, r17 = data read
	st	X+, r17				; store the data read in the Rx buffer

	; 2nd byte
	ldi	r16, ADLC_RX			; set read address to ADLC_RX (2) 
	rcall	adlc_read				; read ADLC, r17 = data read
	st	X+, r17				; store the data read in the Rx buffer

	sts	adlc_rx_ptr, XL			; store the incremented Rx buffer pointer
	sts	adlc_rx_ptr + 1, XH		; 

	;;  if we're receiving payload, we already decided this packet was good.  Nothing more to do.
	mov	r16, adlc_state
	cpi	r16, RX_DATA
	breq	adlc_irq_ret

	inc	adlc_state

	;;  if this was the first RDA event for a frame, we need more data.
	cpi	r16, RX_CHECK_NET1
	breq	adlc_irq_ret
	cpi	r16, RX_SCOUT_ACK1
	breq	adlc_irq_ret

	;;  now we have the full destination address, check if this is a packet we wanted
	cpi	r16, RX_CHECK_NET2
	breq	check_dst_net

	;; if we get here, something is wrong.  Reset and start over.
	rjmp	discontinue

check_dst_net:	
	lds	r16, ECONET_RX_BUF + 1
	rcall	interesting_network
	brcs	adlc_irq_ret
	brne	discontinue

	rjmp	adlc_irq_ret			; return from interrupt

discontinue:
	ldi	r16, ADLC_CR1
	ldi	r17, CR1_TXRESET | CR1_RIE | CR1_DISCONTINUE
	rcall	adlc_write

adlc_rx_reset:	
	clr	adlc_state

	; reset rx buffer pointer
	ldi	XL, ECONET_RX_BUF & 0xff	; set XL with the lowbyte of the Econet receive buffer
	ldi	XH, ECONET_RX_BUF >> 8		; set XH with the highbyte of the Econet receive buffer
	
	sts	adlc_rx_ptr, XL			; set ALDC receive ptr to the start of the Rx buffer
	sts	adlc_rx_ptr + 1, XH

	rjmp	adlc_irq_ret


; -----------------------------------------------------------------------------------------
; ADLC Rx Flush
; -----------------------------------------------------------------------------------------
	
adlc_rx_flush:
	; switch to 1-byte mode, no PSE
	ldi	r16, ADLC_CR2			; set to write to Control Register 2
	ldi	r17, CR2_FLAGIDLE | CR2_RTS
	rcall	adlc_write				; write to Control Register

	ldi	r16, ADLC_CR1
	ldi	r17, 0
	rcall	adlc_write

	; read SR2
	ldi	r16, ADLC_SR2			; read Status Register 2
	rcall	adlc_read				; read Satus Register into r17

	;if there is a byte to read, read it first before clearing
	
	bst	r17, 7				; RDA? is receive data available
	brbc	6, no_rda				; no

	lds	XL, adlc_rx_ptr			; put the Rx buffer pointer address in X
	lds	XH, adlc_rx_ptr + 1

	; Read 1 byte to the buffer
	ldi	r16, ADLC_RX			; set read address to ADLC_RX (2) 
	rcall	adlc_read				; read ADLC, r17 = data read

	st	X+, r17				; store the data read in the Rx buffer

	sts	adlc_rx_ptr, XL			; store the incremented Rx buffer pointer
	sts	adlc_rx_ptr + 1, XH		; 

no_rda:
	ldi	r16, ADLC_CR2			; set to write to Control Register 2
	ldi	r17, CR2_FLAGIDLE | CR2_RTS | CR2_CLRRX | CR2_PSE | CR2_TWOBYTE
	rcall	adlc_write				; write to Control Register

	ret


; -----------------------------------------------------------------------------------------
; ADLC clear Rx
; -----------------------------------------------------------------------------------------
;
; clear rx status and switch back to 1-byte mode
;

adlc_clear_rx:
	ldi	r16, ADLC_CR2			; set to write to Control Register 2
	ldi	r17, CR2val				; reset Receieve status bits
	rjmp	adlc_write				; write to ADLC

; -----------------------------------------------------------------------------------------
; ADLC IRQ ret
; -----------------------------------------------------------------------------------------

adlc_irq_ret:
	ldi	tmp, (1 << INT0)			; restore the interrupt flag
	out	GICR, tmp
	pop	r16					; restore Status register from the Stack
	out	SREG, r16		
	pop	r17					; restore other registers from the Stack
	pop	r16
	pop	tmp
	reti						; return from interrupt

; -----------------------------------------------------------------------------------------
; ADLC init
; -----------------------------------------------------------------------------------------


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
	ldi	r16, EGPIO_ADLC_RESET | EGPIO_SET	; set the ADLC reset to 1
	rcall	egpio_write				; write to ADLC

	; set the initial values of Control Register 1
	ldi	r17, CR1_AC | CR1_TXRESET | CR1_RXRESET	; Switch controlling access to CR3
							; Resets all Tx, Rx status and FIFO registers
							; these will automatically go back to 0
	ldi	r16, ADLC_CR1			; set to ADLC Control Register 1
	rcall	adlc_write				; write to ADLC

	;new
	ldi r17, 0x1E								
	ldi r16, ADLC_TXTERMINATE		; write to TXTerminate address
	rcall adlc_write
	;end new

	; set the initial values of Control Register 3 (CR1_AC previously set)
	clr	r17					; Set initial value to 0
	ldi	r16, ADLC_CR3			; set to ADLC Control Register 3
	rcall	adlc_write				; write to ADLC


	; set the initial values of Control Register 4
	ldi	r17, CR4_TXWLS | CR4_RXWLS	; Set Tx & Rx Word lengths to 8 bits
	ldi	r16, ADLC_CR4			; set to ADLC Control Register 4
	rcall	adlc_write				; write to ADLC

	ldi	r17, CR1_TXRESET | CR1_RXRESET ;  keep transmitter and receiver reset, irqs off, back to CR2
	ldi	r16, ADLC_CR1			; set to write to Control Register 1
	rcall	adlc_write				; write to the ADLC
	
; -----------------------------------------------------------------------------------------
; ADLC ready to receive
; -----------------------------------------------------------------------------------------
;
; exit	: adlc_state = 0
; 		: adlc_rx_ptr    = ECONET RX Buffer LSB
; 		: adlc_rx_ptr + 1    = ECONET RX Buffer MSB

adlc_ready_to_receive:
 	
	ldi	r17, CR1_RXRESET | CR1_TXRESET; reset tx and rx
	ldi	r16, ADLC_CR1			; set to write to Control Register 1
	rcall	adlc_write				; write to the ADLC

	ldi	r17, CR2val				; Status bit prioritised
	ldi	r16, ADLC_CR2			; set to write to Control Register 2
	rcall	adlc_write				; write to the ADLC

	ldi	r17, CR1_TXRESET			; reset tx, unreset rx
	ldi	r16, ADLC_CR1			; set to write to Control Register 1
	rcall	adlc_write				; write to the ADLC

	;initialise the adlc_state

	clr	adlc_state

	; initialise the Rx Buffer pointer

	ldi	XL, ECONET_RX_BUF & 0xff	; set XL with the lowbyte of the Econet receive buffer
	ldi	XH, ECONET_RX_BUF >> 8		; set XH with the highbyte of the Econet receive buffer
	
	sts	adlc_rx_ptr, XL			; set ALDC receive ptr to the start of the Rx buffer
	sts	adlc_rx_ptr + 1, XH

	ldi	r17, CR1_RIE | CR1_TXRESET	; Enable Receive interrupts | Reset the TX status
	ldi	r16, ADLC_CR1			; set to write to Control Register 1
	rjmp	adlc_write				; write to the ADLC

; -----------------------------------------------------------------------------------------
; Examine the network number and figure out whether it's something we are interested in.
; -----------------------------------------------------------------------------------------
;
;  entry: r16=network number
;  exit:  C set -> do want this frame
;         Z set -> don't want this frame
;	  C,Z both clear -> need to check station number too 

interesting_network:
	cpi	r16, 0xff
	breq	yes_network
	
	clc
	clz
	ret

yes_network:
	sec
	clz
	ret

no_network:
	sez
	clc
	ret
	
	

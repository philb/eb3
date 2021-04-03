; =======================================================================
; == Serial =============================================================
; =======================================================================

	; 115200bps bit time = 8.68us =~ 70 cycles @ 8MHz



.equ	up_A	= 0x41
.equ	up_B	= 0x42
.equ	up_C	= 0x43
.equ	up_D	= 0x44
.equ	up_E	= 0x45
.equ	up_F	= 0x46
.equ	up_G	= 0x47
.equ	up_H	= 0x48
.equ	up_I	= 0x49
.equ	up_J	= 0x4A
.equ	up_K	= 0x4B
.equ	up_L	= 0x4C
.equ	up_M	= 0x4D
.equ	up_N	= 0x4E
.equ	up_O	= 0x4F
.equ	up_P	= 0x50
.equ	up_Q	= 0x51
.equ	up_R	= 0x52
.equ	up_S	= 0x53
.equ	up_T	= 0x54
.equ	up_U	= 0x55
.equ	up_V	= 0x56
.equ	up_W	= 0x57
.equ	up_X	= 0x58
.equ	up_Y	= 0x59
.equ	up_Z	= 0x5A
.equ	lo_a	= 0x61
.equ	lo_b	= 0x62
.equ	lo_c	= 0x63
.equ	lo_d	= 0x64
.equ	lo_e	= 0x65
.equ	lo_f	= 0x66
.equ	lo_g	= 0x67
.equ	lo_h	= 0x68
.equ	lo_i	= 0x69
.equ	lo_j	= 0x6A
.equ	lo_k	= 0x6B
.equ	lo_l	= 0x6C
.equ	lo_m	= 0x6D
.equ	lo_n	= 0x6E
.equ	lo_o	= 0x6F
.equ	lo_p	= 0x70
.equ	lo_q	= 0x71
.equ	lo_r	= 0x72
.equ	lo_s	= 0x73
.equ	lo_t	= 0x74
.equ	lo_u	= 0x75
.equ	lo_v	= 0x76
.equ	lo_w	= 0x77
.equ	lo_x	= 0x78
.equ	lo_y	= 0x79
.equ	lo_z	= 0x7A


; -----------------------------------------------------------------------------------------
; Serial Tx
; -----------------------------------------------------------------------------------------
;
; r16 = byte to send (ASCII code)

serial_tx:
	push	r18					; preserve r18 to the stack
	push 	XL					; X is used in the egpio_write routine
	push	XH
	in	r18, SREG				; read contents of status register
	push	r18					; preserve status register to the stack including interrupt status
	cli						; stop interrupts
	mov	r18, r16				; put the byte to send in r18

	; send start bit
	ldi	r16, EGPIO_TP1			; sends 0 to data area of EGPIO_TP1 
	rcall	egpio_write

	ldi	tmp, 8				; set to loop through the 8 bits in the byte to send

serial_tx_loop:
	ldi	r16, 16				; set a wait loop for 16

serial_tx_wait:
	dec	r16					; minus 1
	brne	serial_tx_wait			; loop if not finished
							; The value to send has to be sent one bit at a time from 
							; right to left
	ror	r18					; move the bits to the right. bit 0-> C flag
	brcc	send_0				; if lowest bit was clear branch to send_0
	ldi	r16, EGPIO_TP1 | EGPIO_SET 	; otherwise next bit to send is 1
	rjmp	send_x

send_0:
	ldi	r16, EGPIO_TP1			; bit to send is 0
	nop
	nop

send_x:
	rcall	egpio_write				; write the bit to the memory map

	dec	tmp					; minus 1 for the outer tx loop that runs through the 8 bits
	brne	serial_tx_loop			; loop if not finished

	ldi	r16, 18				; set up another loop

serial_tx_wait2:
	dec	r16					; minus 1
	brne	serial_tx_wait2			; loop until finished

	; send stop bit
	ldi	r16, EGPIO_TP1 | EGPIO_SET	; in the memory map EGPIO_TP1 = 1
	rcall	egpio_write

	ldi	r16, 100				; set up another wait loop

serial_tx_wait3:
	dec	r16					; minus 1
	brne	serial_tx_wait3			; loop until finished
		
		
	pop	r18					; retrieve SREG from the stack
	out	SREG, r18				; restore SREG
	pop	XH
	pop	XL
	pop	r18					; restore r18 from the stack
	ret


; -----------------------------------------------------------------------------------------
; Serial Tx Hex
; -----------------------------------------------------------------------------------------
;
; Ouput the byte to the serial in Hexadecimal
;
; r16 = byte

serial_tx_hex:
	push 	r16					; save register to the stack    
	lsr	r16					; get the upper nibble first   
	lsr	r16
	lsr	r16
	lsr	r16
	rcall	serial_tx_nibble			; send the first value	
	pop 	r16					; retrieve the saved value
	andi	r16, 0xf				; seperate the lower nibble
serial_tx_nibble:
	cpi	r16, 10				; check the value is 0-9		
	brcc	serial_tx_hex1			; > 10 goto serial_tx_hex1 to add the A-F hex value
	ldi	tmp, 48				; tmp=48, the ASCII value offset from 0 for 0-9
serial_tx_hex2:
	add	r16, tmp				; add to the lower nibble to get the ASCII code
	rjmp	serial_tx				; transmit
serial_tx_hex1:
	ldi	tmp, 55				; tmp=48, the ASCII value offset from 0 for A-F
	rjmp	serial_tx_hex2			; jmp to add and transmit

; -----------------------------------------------------------------------------------------
; CRLF
; -----------------------------------------------------------------------------------------
;
; output <cr><lf> to the serial debugger

crlf:
	ldi	r16, 13				; <CR> to r16
	rcall	serial_tx				; ouput to serial
	ldi	r16, 10				; <LF> to r16
	rjmp	serial_tx				; output to serial

; -----------------------------------------------------------------------------------------
; space
; -----------------------------------------------------------------------------------------
;
; output a space

space:
	ldi	r16, 0x20				; space
	rjmp	serial_tx				; ouput to serial

ok:
	ldi	r16, lo_o
	rcall	serial_tx
	ldi	r16, lo_k
	rcall	serial_tx
	rjmp crlf

Eth:
	ldi	r16, up_E
	rcall	serial_tx
	ldi	r16, lo_t
	rcall	serial_tx
	ldi	r16, lo_h
	rjmp 	serial_tx

Eco:
	ldi	r16, up_E
	rcall	serial_tx
	ldi	r16, lo_c
	rcall	serial_tx
	ldi	r16, lo_o
	rjmp 	serial_tx

rx:
	ldi	r16, lo_r
	rcall	serial_tx
	ldi	r16, lo_x
	rjmp 	serial_tx

tx:
	ldi	r16, lo_t
	rcall	serial_tx
	ldi	r16, lo_x
	rjmp 	serial_tx

bu:
	ldi	r16, lo_b
	rcall	serial_tx
	ldi	r16, lo_u
	rjmp	serial_tx


; -----------------------------------------------------------------------------------------
; debug
; -----------------------------------------------------------------------------------------
;
; output "debug" to the serial debugger

debug:
	ldi	r16, lo_d				; "d"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_e				; "e"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_b				; "b"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_u				; "u"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_g				; "g"
	rjmp	serial_tx				; ouput to serial

; -----------------------------------------------------------------------------------------
; No Clock
; -----------------------------------------------------------------------------------------
;
; output "No Clock" to the serial debugger
;
noclock:
	ldi	r16, up_N				; "N"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_o				; "o"
	rcall	serial_tx				; ouput to serial
	rcall	space					; output a space	
	ldi	r16, lo_c				; "c"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_l				; "l"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_o				; "o"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_c				; "c"
	rcall	serial_tx				; ouput to serial
	ldi	r16, lo_k				; "k"
	rcall	crlf					; ouput CRLF



; -----------------------------------------------------------------------------------------
; Some general debugging routines to output some registers to the serial port
; -----------------------------------------------------------------------------------------


output_r16:
	rcall	serial_tx_hex
	rcall	space					; output a space	
	ret

output_r17:
	mov	r16,r17
	rcall	serial_tx_hex
	rcall	space					; output a space	
	ret

output_r18:
	mov	r16, r18
	rcall	serial_tx_hex
	rcall	space					; output a space	
	ret

output_r19:
	mov	r16, r19
	rcall	serial_tx_hex
	rcall	space					; output a space	
	ret

output_r16_r17:
	rcall	output_r16				; output r16
	rcall	output_r17				; output r17
	ret

output_r18_r19:
	rcall	output_r18
	rcall	output_r19
	ret

output_r17_r16:
	rcall	output_r17				; output r17
	rcall	output_r16				; output r16
	ret

output_r19_r18:
	rcall	output_r19
	rcall	output_r18
	ret

output_BidError:
	; debug	"Bid Error"
	rcall	tx
	ldi	r16, up_B
	rcall	serial_tx
	ldi	r16, lo_i
	rcall	serial_tx
	ldi	r16, lo_d
	rcall	serial_tx
	rcall space
	rcall output_Error
	ret


output_Error:
	; debug	"Error"
	ldi	r16, up_E
	rcall serial_tx
	ldi	r16, lo_r
	rcall	serial_tx
	rcall	serial_tx
	ldi	r16, lo_o
	rcall serial_tx
	ldi	r16, lo_r
	rcall	serial_tx
	rcall crlf
	ret

output_BadCRC:
	; debug	"Bad CRC"
	rcall output_Bad
	rcall space
	ldi	r16, up_C
	rcall	serial_tx
	ldi	r16, up_R
	rcall	serial_tx
	ldi	r16, up_C
	rcall	serial_tx
	rcall crlf
	ret

output_BadRunt:
	; debug	"Bad RUNT"
	rcall output_Bad
	rcall space
	ldi	r16, up_R
	rcall	serial_tx
	ldi	r16, up_U
	rcall	serial_tx
	ldi	r16, up_N
	rcall	serial_tx
	ldi	r16, up_T
	rcall	serial_tx
	rcall crlf

output_BadExtra:
	; debug	"Bad Extra"
	rcall output_Bad
	rcall space
	ldi	r16, up_E
	rcall	serial_tx
	ldi	r16, lo_x
	rcall	serial_tx
	ldi	r16, lo_t
	rcall	serial_tx
	ldi	r16, lo_r
	rcall	serial_tx
	ldi	r16, lo_a
	rcall	serial_tx
	rcall crlf

output_Bad:
	ldi	r16, up_B
	rcall serial_tx
	ldi	r16, lo_a
	rcall	serial_tx
	ldi	r16, lo_d
	rcall serial_tx
	ret

output_UDP:
	ldi	r16, up_U
	rcall serial_tx
	ldi	r16, up_D
	rcall	serial_tx
	ldi	r16, up_P
	rcall serial_tx
	ret



; -----------------------------------------------------------------------------------------
; output_ip_buffer
; -----------------------------------------------------------------------------------------
; 
; send ip buffer to the serial output

output_ip_buffer:

	; now the ip header packet
	ldi	r16, ip_header & 0xff		; set r16 with the lowbyte of the ip header
	ldi	r17, ip_header >> 8		; set r17 with the highbyte of the ip header
	lds	r19, ip_TotalLength		; get high byte of length
	lds	r18, ip_TotalLength+1		; get low byte of length

	add	r18, r16				; work out the buffer end address (buffer+length)
	adc	r19, r17

	rcall	output_buffer

	ret


; -----------------------------------------------------------------------------------------
; output_udp_buffer
; -----------------------------------------------------------------------------------------
; 
; send udp buffer to the serial output

output_udp_buffer:

	;copy the data to the UDP data buffer
	ldi	r16, udp_packet & 0xff		; set r16 with the lowbyte of the udp data buffer
	ldi	r17, udp_packet >> 8		; set r17 with the highbyte of the udp data buffer
	lds	r19, udp_length			; store high byte of length in the UDP header
	lds	r18, udp_length+1			; store low byte of length in the UDP header

	add	r18, r16				; work out the buffer end address (buffer+length)
	adc	r19, r17

	rcall	output_buffer

	ret

; -----------------------------------------------------------------------------------------
; output_econet_rx_buffer
; -----------------------------------------------------------------------------------------
; 
; send econet rx buffer to the serial output

output_econet_rx_buffer:

	ldi	r16, ECONET_RX_BUF & 0xff	; set r16 with the lowbyte of the Econet receive buffer
	ldi	r17, ECONET_RX_BUF >> 8		; set r17 with the highbyte of the Econet receive buffer
	lds	r18, adlc_rx_ptr
	lds	r19, adlc_rx_ptr + 1		; put the end buffer address in r18, r19

	rcall	output_buffer

	ret

; -----------------------------------------------------------------------------------------
; output_ethernet_rx_buffer
; -----------------------------------------------------------------------------------------
; 
; send ethernet rx buffer to the serial output

output_ethernet_rx_buffer:			; send it out on serial

	ldi	r16, CS8900_RX_BUF & 0xff	; set r16 with the lowbyte of the Ethernet receive buffer
	ldi	r17, CS8900_RX_BUF >> 8		; set r17 with the highbyte of the Ethernet receive buffer
	lds	r18, cs8900_rx_ptr		; put the buffer length in r18, r19
	lds	r19, cs8900_rx_ptr + 1		
	
	add	r18, r16				; work out the buffer end address (buffer+length)
	adc	r19, r17

	rcall	output_buffer			; send the ethernet packet to the serial output
	ret

; -----------------------------------------------------------------------------------------
; output_buffer
; -----------------------------------------------------------------------------------------
; 
; send buffer to the serial output
;
; on entry	:	r16 = buffer address LSB
;		 	r17 = buffer address MSB
;			r18 = end buffer address LSB
;			r19 = end buffer address MSB

output_buffer:

	mov	ZL, r16				; set ZH with the highbyte of the buffer
	mov	ZH, r17				; set ZL with the lowbyte of the buffer

print_packet_loop:
	ld	r16, Z+				; get byte from Buffer, and increment buffer address counter
	rcall	serial_tx_hex			; output in hex
	
	rcall	space					; output a space	
	
	cp	ZH, r19				; does pointer = Buffer end high byte 
	brne	print_packet_loop			; no, then continue and loop
	
	cp	ZL, r18				; does pointer = Buffer end low byte
	brne	print_packet_loop			; no, then continue and loop

	rcall	crlf					; finished outputting the buffer contents, printer crlf
	
	ret

; =======================================================================
; == UDP ================================================================
; =======================================================================
;
; functions for UDP protocol layer



; -----------------------------------------------------------------------------------------
; CS8900 create_udp_packet
; -----------------------------------------------------------------------------------------
;
; create a udp packet with data from buffer passed on entry
;
; on entry	:	r16 = buffer address LSB
;		 	r17 = buffer address MSB
;			r18 = end buffer address LSB
;			r19 = end buffer address MSB

create_udp_packet:

	;copy the data to the UDP data buffer
	ldi	XH, udp_data >> 8			; set XH with the highbyte of the udp data buffer
	ldi	XL, udp_data & 0xff		; set XL with the lowbyte of the udp data buffer

	mov	ZL, r16				; set ZL with the lowbyte of the buffer
	mov	ZH, r17				; set ZH with the highbyte of the buffer

	mov	YL, r18				; put the buffer end address in Y
	mov	YH, r19

copy_data_loop:
	ld	tmp, Z+				; get byte from RxBuffer, and increment buffer address counter
	st	X+, tmp				; store it in the udp data buffer
	
	cp	ZH, YH				; pointer = RxBuffer position high byte 
	brne	copy_data_loop	 		; no, then continue and loop
	
	cp	ZL, YL				; pointer = RxBuffer position low byte
	brne	copy_data_loop			; no, then continue and loop


	;copy the length to the UDP length

	; find the length of the packet received
	mov	ZL, r16				; set ZL with the lowbyte of the buffer
	mov	ZH, r17				; set ZH with the highbyte of the buffer

	sub	YL, ZL
	sbc	YH, ZH

	; Y now contains the length of the data packet
	; add the udp header length of 8 bytes

	adiw	YH:YL, udp_header_length
	
	sts	udp_length, YH			; store high byte of length in the UDP header
	sts	udp_length+1, YL			; store low byte of length in the UDP header

	; work out the checksums
	; first the UDP header

	ldi	ZH, udp_packet >> 8		; set ZH with the highbyte of the udp data buffer
	ldi	ZL, udp_packet & 0xff		; set ZL with the lowbyte of the udp data buffer

	lds	lengthH, udp_length
	lds	lengthL, udp_length +1
	
	clr	r16
	sts 	udp_chksum, r16			; make sure the checksum bytes are 0 before calculation
	sts 	udp_chksum+1, r16

	; leave UDP checksum as 0 for debugging
;	rcall	cksum					; value returned in valueH/valueL

;	sts	udp_chksum, valueH
;	sts	udp_chksum+1, valueL

	ret



; =======================================================================
; == IP =================================================================
; =======================================================================



; -----------------------------------------------------------------------------------------
; CS8900 create_ip_packet_udp
; -----------------------------------------------------------------------------------------
;
; create an ip packet with data from buffer passed on entry
; this will start 14 bytes into the CS8900 Tx buffer to allow space for the IA and DA addresses
; and the type IP to be added.
; 
; IP packet = 20bytes + UDP packet
;
; on entry	:	r16 = buffer address LSB
;		 	r17 = buffer address MSB
;			r18 = end buffer address LSB
;			r19 = end buffer address MSB
; 
create_ip_packet_udp:

	lds	YH, udp_length			; get high byte of udp length
	lds	YL, udp_length+1			; get low byte of udp length

	; now add this total to the total IP header length
	adiw	YH:YL, ip_length

	; store ip header length
	sts	ip_TotalLength, YH		; store high byte of length
	sts	ip_TotalLength+1, YL		; store low byte of length

	lds	ZH, ip_ID 				; set ZH with the highbyte of the ip_ID
	lds	ZL, ip_ID+1				; set ZL with the lowbyte of the ip_ID
	
	adiw	ZH:ZL, 0x01				; add 1 to the ID

	sts	ip_ID , ZH				; and store the new ID
	sts	ip_ID+1 , ZL

	; now the ip header packet
	ldi	ZH, ip_header >> 8		; set ZH with the highbyte of the ip header
	ldi	ZL, ip_header & 0xff		; set ZL with the lowbyte of the ip header

	clr	lengthH
	ldi	lengthL,0x14 			; just the 20 header bytes

	sts	ip_chksum, r16			; make sure the checksum bytes are 0 before calculation
	sts	ip_chksum+1, r16
	
	rcall cksum					; value returned in valueH/valueL

	sts	ip_chksum, valueH
	sts	ip_chksum+1, valueL

	; ip packet is now complete

ret



;
; Calc checksum. Reads packet at offset zL/zH for
; lengthH/lengthL bytes and calculates the checksum
; in valueH/valueL.
;
cksum: 
	clr	chksumL				; we do the arithmetic
	clr	chksumM				; using a 24
	clr	chksumH				; bit area
	sbrs	lengthL,0				; odd length?
	rjmp	cksumC
	mov	YL,ZL
	mov	YH,ZH
	add	YL,lengthL
	adc	YH,lengthH
	clr	WL
	st	y,WL		      	 	; clear byte after last
	adiw	lengthL,1
cksumC: 
	lsr	lengthH
	ror	lengthL
cksuml: 		
	ld	WH,z+		 			; get high byte of 16-bit word
	ld	WL,z+
	add	chksumL,WL	      		; add to accum
	brcc	noLcarry
	ldi	WL,1
	add	chksumM,WL
	brcc	noLcarry
	add	chksumH,WL
noLcarry:
	add	chksumM,WH	      		; add in the high byte
	brcc	noHcarry
	inc	chksumH
noHcarry:
	subi	lengthL,1
	sbci	lengthH,0
	clr	WL
	cpi	lengthL,0
	cpc	lengthH,WL
	breq	CkDone
	brpl	cksuml    
CkDone: 
	add	chksumL,chksumH	 		; add in the third byte of 24 bit area
	brcc	CkDone1
	inc	chksumM
CkDone1:
	mov	valueL,chksumL
	com	valueL
	mov	valueH,chksumM
	com	valueH
	ret

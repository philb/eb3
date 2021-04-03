;**** A P P L I C A T I O N   N O T E   A V R 1 0 0  ************************
;*
;* Title:		Accessing the EEPROM
;* Version:		2.0
;* Last updated:	98.10.14
;* Target:		AT90S8515
;* Suitable for:	Any AVR with internal EEPROM
;*
;* Support E-mail:	avr@atmel.com
;*
;* DESCRIPTION
;* This Application note shows how to read data from and write data to the
;* EEPROM. Both random access and sequential access routines are listed.
;* The code is written for 8515. To modify for 90S4414,90S2313,90S2323...
;* apply the following changes:
;*	- Remove all entries to EEPROM Address Register High Byte EEARH
;*
;* To modify for 90S1200, apply the changes above. In addition:
;*	- Remove all writes to EEMWE
;*
;*
;* Change log
;*	V2.0	98.10.14 (jboe)		Bugfix, changed to support AT90S8515 
;*	V1.1	97.07.04 (gk)		Created
;***************************************************************************
	
;***************************************************************************
;* 
;* EEWrite
;*
;* This subroutine waits until the EEPROM is ready to be programmed, then
;* programs the EEPROM with register variable "EEdwr" at address "EEawr:EEawr"
;*
;* Number of words	: 9 + return
;* Number of cycles	: 11 + return (if EEPROM is ready)
;* Low Registers used	: None
;* High Registers used:	; 3 (EEdwr,EEawr,EEawrh)
;*
;***************************************************************************

;***** Subroutine register variables

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0     /* Required for port labels to work as expected */

#include <avr/io.h>

#define EEdi    r22
#define EEdo    r24
#define EEal    r24
#define EEah    r25

;***** Code

.section .text             ; Start program section

.global eeprom_write_byte
eeprom_write_byte:
	sbic	EECR,EEWE	;if EEWE not clear
	rjmp	eeprom_write_byte

	out 	EEARH,EEah	;output address high byte, remove if no high byte exists
	out	EEARL,EEal	;output address low byte
		

	out	EEDR,EEdi	;output data
	cli			;disable global interrupts	
	sbi 	EECR,EEMWE	;set master write enable, remove if 90S1200 is used	
	sbi	EECR,EEWE	;set EEPROM Write strobe
				;This instruction takes 4 clock cycles since
				;it halts the CPU for two clock cycles
	sei			;enable global interrupts
	ret

;***************************************************************************
;* 
;* EERead
;*
;* This subroutine waits until the EEPROM is ready to be programmed, then
;* reads the register variable "EEdrd" from address "EEardh:EEard"
;*
;* Number of words	: 6 + return
;* Number of cycles	: 7 + return (if EEPROM is ready)
;* Low Registers used	: 1 (EEdrd)
;* High Registers used:	: 2 (EEard,EEardh)
;*
;***************************************************************************

;***** Code
.global eeprom_read_byte
eeprom_read_byte:	
	sbic	EECR,EEWE	;if EEWE not clear
	rjmp	eeprom_read_byte

	out 	EEARH,EEah	;output address high byte, remove if no high byte exists
	out	EEARL,EEal	;output address low byte


	sbi	EECR,EERE	;set EEPROM Read strobe
				;This instruction takes 4 clock cycles since
				;it halts the CPU for two clock cycles
	in	EEdo,EEDR	;get data
	ret


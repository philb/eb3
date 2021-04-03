
//  PUBLIC MODULE VARIABLE DEFINITIONS

// Variables filled from (and some written to) EEPROM 

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#ifndef __ASSEMBLER__

#define MACHINE_VER_LOW         0x01
#define MACHINE_VER_HIGH        0x00

typedef unsigned char	uint8_t;
typedef unsigned char	u_char;
typedef unsigned int	u_int;
typedef unsigned short	u_short;
typedef unsigned long	u_long;


typedef struct { 
    uint8_t Safety; 
    uint8_t Initialised; 
    uint8_t MachineLow; 
    uint8_t MachineHigh; 
    uint8_t ClockMultiplier; 
    uint8_t Econet_Network; 
    uint8_t Station; 
    uint8_t Ethernet_Network; 
    uint8_t IPAddr[4]; 
    uint8_t Subnet[4]; 
    uint8_t Gateway[4]; 
    uint8_t MAC[6];
    uint8_t WANRouter[4];
    uint8_t EconetIP[4];
    uint8_t EconetMask[4];
} sDefaults_t; 


extern sDefaults_t eeprom; 

extern uint8_t gPrevTxBidFail;

#endif /* __ASSEMBLER__ */


#endif /* __GLOBALS_H__ */


#ifndef EEPROM_H
#define EEPROM_H


// PUBLIC MODULE CONSTANT DEFINITIONS

enum {  EEPROM_SAFETY,			// A power-failure *CAN* corrupt this byte
        EEPROM_INITTED,			// The EEPROM content has been Initted flag
        EEPROM_MACHINE_VER_HIGH,	// Version numbering
        EEPROM_MACHINE_VER_LOW,
        EEPROM_ECONET_CLOCK, 		// Econet clock multiplier
        EEPROM_ECONET_NETWORK,		// Econet Network number of the econet port
        EEPROM_ECONET_STATION,    	// Econet station
        EEPROM_ETHERNET_NETWORK,	// Econet Network number of the ethernet port
        EEPROM_IPADDR1,			// IP Address
        EEPROM_IPADDR2,			//
        EEPROM_IPADDR3,			//
        EEPROM_IPADDR4,			//
        EEPROM_SUBNET1,			// Subnet
        EEPROM_SUBNET2,			//
        EEPROM_SUBNET3,			//
        EEPROM_SUBNET4,			//
        EEPROM_GATEWAY1,		// Gateway
        EEPROM_GATEWAY2,		//
        EEPROM_GATEWAY3,		//
        EEPROM_GATEWAY4,		//
        EEPROM_MAC1,			// MAC Address
        EEPROM_MAC2,			//
        EEPROM_MAC3,			//
        EEPROM_MAC4,			//
        EEPROM_MAC5,			//
        EEPROM_MAC6,			//
	EEPROM_WAN_ROUTER1,		// WAN Router address
	EEPROM_WAN_ROUTER2,		//
	EEPROM_WAN_ROUTER3,		//
	EEPROM_WAN_ROUTER4,		//
	EEPROM_WAN_ECONET_IP1,		// Econet-side IP
	EEPROM_WAN_ECONET_IP2,		// 
	EEPROM_WAN_ECONET_IP3,		// 
	EEPROM_WAN_ECONET_IP4,		// 
	EEPROM_WAN_ECONET_SUBNET1,	// Econet-side Subnet mask
	EEPROM_WAN_ECONET_SUBNET2,	// 
	EEPROM_WAN_ECONET_SUBNET3,	// 
	EEPROM_WAN_ECONET_SUBNET4	// 
     };



#define EEPROM_VALID    (0x55)



//  PUBLIC FUNCTION PROTOTYPES

extern  void    EEPROM_ReadAll(void);
extern  void    EEPROM_WriteAll(void);
extern  void    EEPROM_main(void);

#endif




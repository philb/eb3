#include <string.h>
#include <avr/eeprom.h>
#include "globals.h"
#include "eeprom.h"


//sDefaults_t INIT EEMEM =
sDefaults_t INIT =
		{0xAA, EEPROM_VALID, MACHINE_VER_HIGH, MACHINE_VER_LOW,	/* Safety, Initted, VER HIGH, VER LOW */
		0, 1, 0x51, 130,				/* Clock, Econet Net, Station, Ethernet Net */
		{1, 2, 128, 10},					/* IP Address */
		{255, 255, 0, 0},					/* Subnet Mask */
		{1, 2, 128, 9},					/* Default Router */
		{ 0x00, 0x00, 0xA4,				/* MAC Address 1-3 OUI Acorn */
		  0x33, 0x44, 0x55 },				/* MAC Address 4-6 NUI */
		{ 82, 71, 203, 201 },				/* WAN router */
		{ 192, 168, 200, 1 },				/* Econet-side IP */
		{ 255, 255, 255, 0 }};				/* Econet-side netmask */




void EEPROM_ReadAll(void)
{
  int i;
  uint8_t *p = (uint8_t *)&eeprom;
  for (i = 0; i < sizeof (eeprom); i++) {
    *(p++) = eeprom_read_byte ((uint8_t *)EEPROM_SAFETY + i);
  }
}

void EEPROM_WriteAll(void) __attribute__ ((noinline));

void EEPROM_WriteAll(void)
{
  int i;
  uint8_t *p = (uint8_t *)&eeprom;
  for (i = 0; i < sizeof (eeprom); i++) {
    eeprom_write_byte((uint8_t *)EEPROM_SAFETY + i, *(p++));
  }
}

static void EEPROM_InitCheck(void)
{
    eeprom.Initialised = eeprom_read_byte((uint8_t *)EEPROM_INITTED);

    if (eeprom.Initialised != EEPROM_VALID) {
      memcpy (&eeprom, &INIT, sizeof (INIT));
      EEPROM_WriteAll();
    }

}

void EEPROM_main (void)
{
   EEPROM_InitCheck();                 // Check EEPROM is valid. If not valid, Init it
   EEPROM_ReadAll ();                  // Get retained Info from EEPROM
}

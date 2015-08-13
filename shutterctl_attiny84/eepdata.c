#include "eepdata.h"
#include <avr/eeprom.h>

static eepdata eepro EEMEM = { 0x20 | ADDRESS };
eepdata eep;

void eepdata_init(void)
{
    eep.id = eeprom_read_byte(&eepro.id);
}


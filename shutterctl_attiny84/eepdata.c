#include "eepdata.h"
#include <avr/eeprom.h>

static eepdata eepro EEMEM = { 0x20 | ADDRESS , 4000, 4000 };
eepdata eep;

void eepdata_init(void)
{
    eep.id = eeprom_read_byte(&eepro.id);
    eep.ticks_down = eeprom_read_word(&eepro.ticks_down);
    eep.ticks_up = eeprom_read_word(&eepro.ticks_up);
}

void eepdata_save(void)
{
    eeprom_write_word(&eepro.ticks_down, eep.ticks_down);
    eeprom_write_word(&eepro.ticks_up, eep.ticks_up);
}


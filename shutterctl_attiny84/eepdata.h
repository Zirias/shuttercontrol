#ifndef EEPDATA_H
#define EEPDATA_H

#include <stdint.h>

typedef struct
{
    uint8_t id;
    uint16_t ticks_down;
    uint16_t ticks_up;
} eepdata;

extern eepdata eep;

#define eep_version (eep.id >> 5)
#define eep_address (eep.id & 0x1f)

void eepdata_init(void);
void eepdata_save(void);

#endif

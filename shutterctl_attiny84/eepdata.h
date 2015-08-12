#ifndef EEPDATA_H
#define EEPDATA_H

#include <stdint.h>

typedef struct
{
    uint8_t id;
} eepdata;

extern eepdata eep;

#define eep_version (eep.id >> 4)
#define eep_address (eep.id & 0xf)

void eepdata_init(void);

#endif

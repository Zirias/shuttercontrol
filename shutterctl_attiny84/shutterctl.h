#ifndef SHUTTERCTL_H
#define SHUTTERCTL_H

#include "common.h"

#define INVALID_POS 0xff

typedef enum shutterctl_prio
{
    PRIO_NONE = 0,
    PRIO_BUS,
    PRIO_MANUAL
} shutterctl_prio;

void shutterctl_init(void);

void shutterctl_stop(shutterctl_prio prio);
void shutterctl_up(shutterctl_prio prio, BOOL autostop);
void shutterctl_down(shutterctl_prio prio, BOOL autostop);

BOOL shutterctl_isactive(void);
uint8_t shutterctl_pos(void);
uint8_t shutterctl_maxpos(void);

#endif

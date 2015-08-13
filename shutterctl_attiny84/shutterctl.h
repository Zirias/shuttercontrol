#ifndef SHUTTERCTL_H
#define SHUTTERCTL_H

#include "common.h"

#include <avr/io.h>

#define shutter_stop() do { PORTA &= ~(_BV(PA6) | _BV(PA7)); } while (0)
#define shutter_up() do { shutter_stop(); PORTA |= _BV(PA6); } while (0)
#define shutter_down() do { shutter_stop(); PORTA |= _BV(PA7); } while (0)

#endif

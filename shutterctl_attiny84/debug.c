#include "debug.h"

#include <avr/io.h>

static int gpio_putchar(char c, FILE *stream)
{
    GPIOR0 = c;
    return 0;
}

static FILE dbg = FDEV_SETUP_STREAM(&gpio_putchar, 0, _FDEV_SETUP_WRITE);
FILE *dbgstr = &dbg;


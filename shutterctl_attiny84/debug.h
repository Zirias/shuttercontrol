#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <avr/pgmspace.h>

#ifdef DEBUG
#define debug(x) fputs_P(PSTR(x), dbgstr)
#define debugf(x, ...) fprintf_P(dbgstr, PSTR(x), __VA_ARGS__)
extern FILE *dbgstr;
#else
#define debug(x)
#define debugf(x, ...)
#endif

#endif

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG

#include <stdio.h>
#include <avr/pgmspace.h>

#define debug(x) fputs_P(PSTR(x), dbgstr)
#define debugf(x, ...) fprintf_P(dbgstr, PSTR(x), __VA_ARGS__)
extern FILE *dbgstr;

#else
#define debug(x)
#define debugf(x, ...)
#endif

#endif

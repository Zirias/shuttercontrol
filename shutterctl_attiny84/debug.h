#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define debug(x) fputs((x), dbgstr)
#define debugf(x, ...) fprintf(dbgstr, (x), __VA_ARGS__)
extern FILE *dbgstr;
#else
#define debug(x)
#define debugf(x, ...)
#endif

#endif

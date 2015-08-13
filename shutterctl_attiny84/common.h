#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#ifndef __GNUC__
#define __attribute__(x)
#define UNREACHABLE()
#else
#define UNREACHABLE() __builtin_unreachable()
#endif

typedef uint8_t BOOL;
#define TRUE 1
#define FALSE 0

#endif

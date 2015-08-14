#include <avr/io.h>

/* "bootcode" to execute as early as possible */

.section    .init1,"ax",@progbits

    /* initial pin configuration */

    ldi	    r16, 0x3f
    out	    _SFR_IO_ADDR(PORTA), r16
    out	    _SFR_IO_ADDR(PORTB), r16
    ldi	    r16, 0xc0
    out	    _SFR_IO_ADDR(DDRA), r16


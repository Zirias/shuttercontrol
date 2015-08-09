#include <avr/io.h>

#include <avr/avr_mcu_section.h>

AVR_MCU(8000000, "attiny84");
AVR_MCU_VOLTAGES(3300, 3300, 3300);

int main (void)
{
    while(1);
    return 0;
}


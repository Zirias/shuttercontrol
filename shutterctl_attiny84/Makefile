AVRDEV:= attiny84

CC:= avr-gcc
AVRSIZE:= avr-size

TARGET:= ../shutterctl.afx
MAP:= shutterctl.map

DEBUG:= 1

ADDRESS?= 1

CFLAGS:= -std=gnu99 -Wall -Os -g0 -mmcu=$(AVRDEV) -DF_CPU=8000000 \
    -DADDRESS=$(ADDRESS) -ffreestanding \
    -fno-inline-small-functions -ffunction-sections -fdata-sections \
    -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
LDFLAGS:= -Wl,--relax,--gc-sections,-s \
    -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000,-Map=$(MAP)
INCLUDES:= -I/usr/lib/avr/include -I/usr/local/include/simavr

OBJS:= init.o main.o eepdata.o event.o timer.o shutterctl.o buttons.o bus.o

ifeq ($(DEBUG),1)
CFLAGS+= -DDEBUG
OBJS+= debug.o
endif

include ../mk/silent.mk

all: $(TARGET)

$(MAP): $(TARGET)

$(TARGET): $(OBJS)
	$(VCCLD)
	$(VR)$(CC) -o$@ $(CFLAGS) $(LDFLAGS) $^
ifneq ($(AVRSIZE),)
	-$(VR)$(AVRSIZE) -C --mcu=$(AVRDEV) $(TARGET)
endif

%.d: %.c Makefile
	$(VDEP)
	$(VR)$(CC) -MM -MT"$@ $(@:.d=.o)" -MF$@ $(CFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(OBJS:.o=.d)
endif
endif

%.o: %.S Makefile
	$(VCC)
	$(VR)$(CC) -c -o$@ $(CFLAGS) $(INCLUDES) $<

%.o: %.c Makefile
	$(VCC)
	$(VR)$(CC) -c -o$@ $(CFLAGS) $(INCLUDES) $<

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET) $(MAP)

.PHONY: all clean distclean


CC:= avr-gcc

TARGET:= shutterctl.afx
MAP:= shutterctl.map

DEBUG:= 1

ADDRESS?= 1

CFLAGS:= -Wall -Os -gdwarf-2 -mmcu=attiny84 -DF_CPU=8000000 \
    -DADDRESS=$(ADDRESS) \
    -fno-inline-small-functions -ffunction-sections -fdata-sections
LDFLAGS:= -Wl,--relax,--gc-sections \
    -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000,-Map=$(MAP)
INCLUDES:= -I/usr/lib/avr/include -I/usr/local/include/simavr

OBJS:= shutterctl.o eepdata.o

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

%.d: %.c Makefile
	$(VDEP)
	$(VR)$(CC) -MM -MT"$@ $(@:.d=.o)" -MF$@ $(CFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(OBJS:.o=.d)
endif
endif

%.o: %.c Makefile
	$(VCC)
	$(VR)$(CC) -c -o$@ $(CFLAGS) $(INCLUDES) $<

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET) $(MAP)

.PHONY: all clean distclean


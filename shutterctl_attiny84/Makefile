AVRDEV:= attiny84

CC:= avr-gcc
SIZE:= avr-size
OBJCOPY:= avr-objcopy

TGTNAME:= shutterctl

TARGET:= ../$(TGTNAME).afx
MAP:= $(TGTNAME).map

ADDRESS?= 1

VERSION= 3

HEXDIR:= ../hex

HEX:= $(HEXDIR)/$(TGTNAME).hex
EEPROM:= $(HEXDIR)/eeprom-$(ADDRESS).hex

CFLAGS:= -std=gnu99 -Wall -Os -g0 -mmcu=$(AVRDEV) -DF_CPU=8000000 \
    -DVERSION=$(VERSION) -DADDRESS=$(ADDRESS) -ffreestanding \
    -fno-inline-small-functions -ffunction-sections -fdata-sections \
    -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
LDFLAGS:= -Wl,--relax,--gc-sections,-s \
    -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000,-Map=$(MAP)
INCLUDES:= -I/usr/lib/avr/include -I/usr/local/include/simavr

OBJS:= init.o main.o eepdata.o event.o timer.o shutterctl.o buttons.o bus.o

ifdef DEBUG
CFLAGS+= -DDEBUG
OBJS+= debug.o
OBJDIR:= obj/debug
else
OBJDIR:= obj/release
endif

include ../mk/silent.mk

all: $(TARGET)

$(MAP): $(TARGET)

$(TARGET): $(addprefix $(OBJDIR)/,$(OBJS))
	$(VCCLD)
	$(VR)$(CC) -o$@ $(CFLAGS) $(LDFLAGS) $^
ifneq ($(SIZE),)
	-$(VR)$(SIZE) -C --mcu=$(AVRDEV) $(TARGET)
endif

hex: $(HEX) $(EEPROM)

$(HEXDIR):
	$(VMD)
	$(VR)mkdir -p $(HEXDIR)

$(HEX): $(TARGET) | $(HEXDIR)
	$(VGEN)
	$(VR)$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(EEPROM): $(TARGET) | $(HEXDIR)
	$(VGEN)
	$(VR)$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

$(OBJDIR):
	$(VMD)
	$(VR)mkdir -p $(OBJDIR)

$(OBJDIR)/%.d: %.S Makefile | $(OBJDIR)
	$(VDEP)
	$(VR)$(CC) -MM -MT"$@ $(@:.d=.o)" -MF$@ $(CFLAGS) $(INCLUDES) $<

$(OBJDIR)/%.d: %.c Makefile | $(OBJDIR)
	$(VDEP)
	$(VR)$(CC) -MM -MT"$@ $(@:.d=.o)" -MF$@ $(CFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(addprefix $(OBJDIR)/,$(OBJS:.o=.d))
endif
endif

$(OBJDIR)/%.o: %.S Makefile | $(OBJDIR)
	$(VCC)
	$(VR)$(CC) -c -o$@ $(CFLAGS) $(INCLUDES) $<

$(OBJDIR)/%.o: %.c Makefile | $(OBJDIR)
	$(VCC)
	$(VR)$(CC) -c -o$@ $(CFLAGS) $(INCLUDES) $<

clean:
	rm -fr $(OBJDIR)

distclean: clean
	rm -fr $(HEXDIR)
	rm -fr obj
	rm -f $(TARGET) $(MAP)

.PHONY: all clean distclean


AVRDEV:= attiny84

AVRDUDE:= avrdude
PART:= t84
PROGRAMMER:= avrisp2

CC:= avr-gcc
SIZE:= avr-size
OBJCOPY:= avr-objcopy

TGTNAME:= shutterctl

TARGET:= ../$(TGTNAME).afx
MAP:= $(TGTNAME).map

ADDRESS?= 1

VERSION= 4

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

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
eepcfg.mk:
	$(VGENT)
	$(VR)echo $(EQT)C_VERSION := $(VERSION)$(EQT) >eepcfg.mk
	$(VR)echo $(EQT)C_ADDRESS := $(ADDRESS)$(EQT) >>eepcfg.mk

-include eepcfg.mk

ifneq ($(strip $(C_VERSION))_$(strip $(C_ADDRESS)),$(strip $(VERSION))_$(strip $(ADDRESS)))
.PHONY: eepcfg.mk
endif
endif
endif

$(MAP): $(TARGET)

$(TARGET): $(addprefix $(OBJDIR)/,$(OBJS))
	$(VCCLD)
	$(VR)$(CC) -o$@ $(CFLAGS) $(LDFLAGS) $^
ifneq ($(SIZE),)
	-$(VR)$(SIZE) -C --mcu=$(AVRDEV) $(TARGET)
endif

program: prgflash prgeep

prgflash: $(HEX)
	$(AVRDUDE) -c $(PROGRAMMER) -p $(PART) -U flash:w:$(HEX):i

prgeep: $(EEPROM)
	$(AVRDUDE) -c $(PROGRAMMER) -p $(PART) -U eeprom:w:$(EEPROM):i

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

$(OBJDIR)/eepdata.o: eepcfg.mk

clean:
	rm -fr $(OBJDIR)

distclean: clean
	rm -fr $(HEXDIR)
	rm -fr obj
	rm -f $(TARGET) $(MAP)

.PHONY: all hex program prgflash prgeep clean distclean


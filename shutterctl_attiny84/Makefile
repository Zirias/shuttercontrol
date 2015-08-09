CC:= avr-gcc

TARGET:= shutterctl.afx
MAP:= shutterctl.map

CFLAGS:= -O3 -g0 -mmcu=attiny84
LDFLAGS:= -Wl,-s,-Map=$(MAP)
INCLUDES:= -I/usr/local/include/simavr
OBJS:= shutterctl.o

all: $(TARGET)

$(MAP): $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o$@ $(CFLAGS) $(LDFLAGS) $^

%.o: %.c
	$(CC) -c -o$@ $(CFLAGS) $(INCLUDES) $<

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET) $(MAP)

.PHONY: all clean distclean


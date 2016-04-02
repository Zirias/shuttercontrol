TARGETS:= shuttercontrol shutterctl_attiny84/shutterctl.afx

all: $(TARGETS)

shuttercontrol:
	$(MAKE) -Csrc

shutterctl_attiny84/shutterctl.afx:
	$(MAKE) -Cshutterctl_attiny84

clean:
	$(MAKE) -Csrc clean
	$(MAKE) -Cshutterctl_attiny84 clean

distclean:
	$(MAKE) -Csrc distclean
	$(MAKE) -Cshutterctl_attiny84 distclean

.PHONY: all clean distclean $(TARGETS)


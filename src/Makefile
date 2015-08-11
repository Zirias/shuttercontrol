CXX:= g++

TARGET:= ../shuttercontrol

QTLIBS:= /usr/lib/x86_64-linux-gnu
QTINCLUDES:= /usr/include/qt4

QTMODULES:= QtCore QtGui

INCLUDES+= -I$(QTINCLUDES) $(addprefix -I$(QTINCLUDES)/,$(QTMODULES))
ifdef DEBUG
CXXFLAGS+= -O0 -g3
LDFLAGS+= -L$(QTLIBS)
else
CXXFLAGS+= -O3 -g0
LDFLAGS+= -L$(QTLIBS) -Wl,-s
endif
LIBS+= -pthread $(addprefix -l,$(QTMODULES))

OBJS:= main.o busclock.o controlleraction.o
MOCOBJS:= mainwindow.o controllerwidget.o shutterwidget.o buswidget.o \
    buslinewidget.o buslineid.o bus.o busconnector.o busclient.o controller.o \
    busconnectorwidget.o shutter.o

OBJS+= $(MOCOBJS) $(MOCOBJS:.o=_moc.o)

include ../mk/silent.mk

all: $(TARGET)

$(TARGET): $(OBJS)
	$(VCCLD)
	$(VR)$(CXX) -o$@ $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS)

%.d: %.cpp Makefile
	$(VDEP)
	$(VR)$(CXX) -MM -MT"$@ $(@:.d=.o)" -MF$@ $(CXXFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(OBJS:.o=.d)
endif
endif

%.o: %.cpp Makefile
	$(VCXX)
	$(VR)$(CXX) -c -o$@ $(CXXFLAGS) $(INCLUDES) $<

%_moc.cpp: %.h Makefile
	$(VMOC)
	$(VR)moc -o$@ $<

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

distclean: clean
	rm -f $(TARGET)

.PHONY: all clean distclean
.PRECIOUS: %_moc.cpp


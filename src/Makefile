CXX:= g++

TARGET:= shuttercontrol

QTLIBS:= /usr/lib/x86_64-linux-gnu
QTINCLUDES:= /usr/include/qt4

QTMODULES:= QtCore QtGui

INCLUDES+= -I$(QTINCLUDES) $(addprefix -I$(QTINCLUDES)/,$(QTMODULES))
LDFLAGS+= -L$(QTLIBS)
LIBS+= $(addprefix -l,$(QTMODULES))

MOCHDRS:= mainwindow.h controllerwidget.h shutterwidget.h buswidget.h \
    buslinewidget.h
OBJS:= main.o mainwindow.o controllerwidget.o shutterwidget.o buswidget.o \
    buslinewidget.o

OBJS+= $(MOCHDRS:.h=_moc.o)

include ../mk/silent.mk

all: $(TARGET)

$(TARGET): $(OBJS)
	$(VCCLD)
	$(VR)$(CXX) -o$@ $(CFLAGS) $(LDFLAGS) $^ $(LIBS)

%.d: %.cpp Makefile
	$(VDEP)
	$(VR)$(CXX) -MM -MT"$@ $(@:.d=.o)" -MF$@ $(CFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(OBJS:.o=.d)
endif
endif

%.o: %.cpp Makefile
	$(VCXX)
	$(VR)$(CXX) -c -o$@ $(CFLAGS) $(INCLUDES) $<

%_moc.cpp: %.h Makefile
	$(VMOC)
	$(VR)moc -o$@ $<

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(MOCHDRS:.h=_moc.cpp)

distclean: clean
	rm -f $(TARGET)

.PHONY: all clean distclean
.PRECIOUS: %_moc.cpp


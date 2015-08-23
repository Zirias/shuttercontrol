CXX:= g++
MOC:= moc

TARGET:= ../shuttercontrol

QTLIBS:= /usr/lib/x86_64-linux-gnu
QTINCLUDES:= /usr/include/qt4

QTMODULES:= QtGui QtCore

SIMAVRLIBS:= /usr/local/lib
SIMAVRINCLUDES:= /usr/local/include/simavr

INCLUDES+= -I$(QTINCLUDES) $(addprefix -I$(QTINCLUDES)/,$(QTMODULES))
INCLUDES+= -I$(SIMAVRINCLUDES)

LIBS+= -pthread $(addprefix -l,$(QTMODULES)) \
       -Wl,--Bstatic -lsimavr -lelf -Wl,--Bdynamic

LDFLAGS+= -L$(QTLIBS) -L$(SIMAVRLIBS)

ifdef DEBUG
CXXFLAGS+= -O0 -g3
OBJDIR:= obj/debug
else
CXXFLAGS+= -O3 -g0
LDFLAGS+= -Wl,-s
OBJDIR:= obj/release
endif

ifdef WIN32CROSS
MXEPATH:= /opt/mxe
MXETARGET:= i686-w64-mingw32.static
QTLIBS:= $(MXEPATH)/usr/$(MXETARGET)/qt/lib
SIMAVRLIBS:= $(MXEPATH)/usr/$(MXETARGET)/lib
QTINCLUDES:= $(MXEPATH)/usr/$(MXETARGET)/qt/include/
CROSS:= $(MXEPATH)/usr/bin/$(MXETARGET)-
XMOC:= $(MXEPATH)/usr/$(MXETARGET)/qt/bin/
LIBS+= -ltiff -lpng -ljpeg -lmng -llzma -lz -llcms2 -lelf -lgdi32 -lcomdlg32 -loleaut32 -limm32 -lwinmm -lwinspool -lws2_32 -lole32 -luuid -luser32 -ladvapi32
OBJDIR:= obj/win32cross
TARGET:=$(TARGET).exe
CXXFLAGS+= -DWIN32
LDFLAGS+= -Wl,-subsystem,windows
endif

OBJS:= main.o busclock.o controlleraction.o
MOCOBJS:= mainwindow.o controllerwidget.o shutterwidget.o buswidget.o \
    buslinewidget.o buslineid.o bus.o busconnector.o busclient.o controller.o \
    busconnectorwidget.o shutter.o avrcontainer.o avrpinconnector.o \
    avrwidget.o

OBJS+= $(MOCOBJS)
ALLOBJS:= $(OBJS) $(MOCOBJS:.o=_moc.o)

include ../mk/silent.mk

all: $(TARGET)

$(TARGET): $(addprefix $(OBJDIR)/,$(ALLOBJS))
	$(VCCLD)
	$(VR)$(CROSS)$(CXX) -o$@ $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.d: %.cpp Makefile | $(OBJDIR)
	$(VDEP)
	$(VR)$(CROSS)$(CXX) -MM -MT"$@ $(@:.d=.o)" \
	    -MF$@ $(CXXFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(addprefix $(OBJDIR)/,$(ALLOBJS:.o=.d))
endif
endif

$(OBJDIR)/%.o: %.cpp Makefile | $(OBJDIR)
	$(VCXX)
	$(VR)$(CROSS)$(CXX) -c -o$@ $(CXXFLAGS) $(INCLUDES) $<

%_moc.cpp: %.h Makefile
	$(VMOC)
	$(VR)$(XMOC)$(MOC) -o$@ $<

clean:
	rm -fr $(OBJDIR)
	rm -f $(MOCOBJS:.o=_moc.cpp)

distclean: clean
	rm -f $(TARGET)

.PHONY: all clean distclean
.PRECIOUS: %_moc.cpp


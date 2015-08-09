CXX:= g++

TARGET:= shuttercontrol

QTLIBS:= /usr/lib/x86_64-linux-gnu
QTINCLUDES:= /usr/include/qt4

QTMODULES:= QtCore QtGui

INCLUDES+= -I$(QTINCLUDES) $(addprefix -I$(QTINCLUDES)/,$(QTMODULES))
LDFLAGS+= -L$(QTLIBS)
LIBS+= $(addprefix -l,$(QTMODULES))

MOCHDRS:= mainwindow.h controllerwidget.h shutterwidget.h
OBJS:= main.o mainwindow.o controllerwidget.o shutterwidget.o

OBJS+= $(MOCHDRS:.h=_moc.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o$@ $(CFLAGS) $(LDFLAGS) $^ $(LIBS)

%.o: %.cpp
	$(CXX) -c -o$@ $(CFLAGS) $(INCLUDES) $<

%_moc.cpp: %.h
	moc -o$@ $<

clean:
	rm -f $(OBJS) $(MOCHDRS:.h=_moc.cpp)

distclean: clean
	rm -f $(TARGET)

.PHONY: all clean distclean


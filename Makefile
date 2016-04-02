MOC?= moc-qt4
QTLIBS?= /usr/local/lib/qt4
QTINCLUDES?= /usr/local/include/qt4
GNUMAKE?= gmake

MAKEFLAGS+= MOC=${MOC} QTLIBS=${QTLIBS} QTINCLUDES=${QTINCLUDES}

all:
	${GNUMAKE} ${MAKEFLAGS} $@

.DEFAULT:
	${GNUMAKE} ${MAKEFLAGS} $@

.PHONY: all


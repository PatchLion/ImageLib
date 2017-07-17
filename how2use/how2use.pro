TEMPLATE = app
CONFIG+=console
TARGET = HowToUse
QT += core gui widgets

include(../public.pri)

INCLUDEPATH += ../3rdpart/libraw/include ../include 
    #../3rdpart/log4qt/0.3/include

macx{
    DESTDIR = ../bin

    LIBS += -L../lib -lHTImageLib
    #LIBS += -L../3rdpart/log4qt/0.3/lib -llog4qtlib
    #LIBS += -L../3rdpart/libraw/lib -llibraw_static

    QMAKE_LFLAGS += -framework CoreFoundation \
                                        -framework ImageIO \
                                        -framework CoreGraphics \
                                        -framework AppKit \
                                        -framework CoreServices
}

win32{
    DESTDIR = ../bin/$(Platform)/$(Configuration)

    #DEFINES -= UNICODE

    INCLUDEPATH += ../3rdpart/FreeImage/include \
          ../3rdpart/lcms/include \
          ../3rdpart/exiv2/include

    LIBS += -L../lib/$(Platform)/$(Configuration) -lHTImageLib\
            -L../3rdpart/FreeImage/lib/$(Platform) -lFreeImage\
            -L../3rdpart/zlib/lib/$(Platform) -lzlib\
            -L../3rdpart/exiv2/lib/$(Platform)/$(Configuration) -lexiv2 -llibexpat -lxmp\
            -L../3rdpart/libraw/lib/$(Platform)/$(Configuration) -llibraw_static \
            -L../3rdpart/log4qt/0.3/lib/$(Platform)/$(Configuration) -llog4qtlib_msvc2013_static
}

#DEFINES +=BUILD_WITH_LIBRAW LIBRAW_NODLL

# Input
HEADERS += HTImageLib_Test.h
SOURCES += HTImageLib_Test.cpp main.cpp

FORMS += HTImageLib_Test.ui

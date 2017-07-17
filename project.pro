TEMPLATE = lib
CONFIG += staticlib
TARGET = ImageLib
QT += core gui network

include(public.pri)

INCLUDEPATH += sources includes
HEADERS += includes/*.h
SOURCES +=sources/*.cpp

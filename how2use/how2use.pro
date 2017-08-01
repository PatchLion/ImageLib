TEMPLATE = app
CONFIG+=console c++11
TARGET = how2use
QT += core gui widgets network

include(../public.pri)

INCLUDEPATH += ../includes

# Input
HEADERS += *.h \
../includes/*.h
SOURCES += *.cpp \
../sources/*.cpp

FORMS += *.ui

#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T16:53:43
#
#-------------------------------------------------

QT       += core gui network

TARGET = Market-api
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    market.pb.cc \
    marketsession.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    market.pb.h \
    marketsession.h \
    settings.h

FORMS    += mainwindow.ui \
    settings.ui
LIBS += -lprotobuf
INCLUDEPATH += ../../protobuff/include/













#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T21:15:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qsum
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sum.cpp \
    detailstable.cpp

HEADERS  += mainwindow.h \
    sum.h \
    detailstable.h

LIBS += -lpolarssl
TRANSLATIONS = qsum_ar.ts
CODECFORSRC     = UTF-8
CODECFORTR      = UTF-8
RC_FILE = qsum.rc

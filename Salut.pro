#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T15:34:42
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Salut
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sqltest.cpp \
    settingsform.cpp \
    settings.cpp \
    contractform.cpp \
    contract.cpp \
    person.cpp \
    personform.cpp \
    energyform.cpp \
    energyrecord.cpp \
    transakce.cpp

HEADERS  += mainwindow.h \
    sqlI.h \
    sqltest.h \
    settingsform.h \
    settings.h \
    contractform.h \
    contract.h \
    person.h \
    personform.h \
    energyform.h \
    energyrecord.h \
    transakce.h

FORMS    += mainwindow.ui \
    energyform.ui \
    transakce.ui

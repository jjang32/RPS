#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T13:46:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RPS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        adanormal.cpp \
        basic_rps.cpp \
        cheat.cpp \
        fixed_hedge.cpp \
        fixedtree.cpp \
        lose.cpp \
        main.cpp \
        mainwindow.cpp \
        sleeping_hedge.cpp \
        win.cpp

HEADERS += \
        adanormal.h \
        basic_rps.h \
        cheat.h \
        fixed_hedge.h \
        fixedtree.h \
        lose.h \
        mainwindow.h \
        sleeping_hedge.h \
        win.h

FORMS += \
        adanormal.ui \
        basic_rps.ui \
        cheat.ui \
        fixed_hedge.ui \
        fixedtree.ui \
        lose.ui \
        mainwindow.ui \
        sleeping_hedge.ui \
        win.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

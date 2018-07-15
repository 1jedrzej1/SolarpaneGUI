#-------------------------------------------------
#
# Project created by QtCreator 2014-01-07T10:17:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SolarpaneGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    uart_thread.cpp \
    setdateandtime.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    uart_thread.h \
    setdateandtime.h \
    qcustomplot.h \
    UARTdef.h

FORMS    += mainwindow.ui \
    setdateandtime.ui

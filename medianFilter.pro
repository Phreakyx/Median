#-------------------------------------------------
#
# Project created by QtCreator 2019-04-025T19:14:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = medianFilter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    medianfilter.cpp

HEADERS  += mainwindow.h \
    medianfilter.h \
    common.h


FORMS    += mainwindow.ui

QT += concurrent widgets
CONFIG += console
CONFIG -= app_bundle


target.path = $$[QT_INSTALL_EXAMPLES]/qtconcurrent/wordcount
INSTALLS += target
INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/ucrt/x86"
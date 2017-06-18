#-------------------------------------------------
#
# Project created by QtCreator 2017-06-17T19:10:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoveLetter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loveletterwidget.cpp \
    gamebase.cpp \
    lovelettergame.cpp \
    qlearning.cpp \
    agent.cpp \
    maptreewidget.cpp \
    showdatawidget.cpp

HEADERS  += mainwindow.h \
    loveletterwidget.h \
    gamebase.h \
    lovelettergame.h \
    qlearning.h \
    agent.h \
    maptreewidget.h \
    showdatawidget.h

include("D:\All\Code\QT\module\easywebcontrol\easywebcontrol.pri")

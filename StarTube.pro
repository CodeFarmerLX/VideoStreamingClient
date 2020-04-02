#-------------------------------------------------
#
# Project created by QtCreator 2019-06-19T17:47:16
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StarTube
TEMPLATE = app


SOURCES += main.cpp\
        StartWindow.cpp \
    RegView.cpp \
    LobbyView.cpp \
    MyRoom.cpp \
    UsrRoom.cpp \
    VideoSuface.cpp \
    FindPw.cpp \
    mybarrage.cpp

HEADERS  += StartWindow.h \
    RegView.h \
    LobbyView.h \
    package.h \
    MyRoom.h \
    UsrRoom.h \
    VideoSuface.h \
    mysleep.h \
    FindPw.h \
    usrsleep.h \
    mybarrage.h \
    ss.h

FORMS    += StartWindow.ui \
    RegView.ui \
    LobbyView.ui \
    MyRoom.ui \
    UsrRoom.ui \
    FindPw.ui

RESOURCES += \
    picture.qrc

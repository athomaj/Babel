#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T10:52:32
#
#-------------------------------------------------

QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI_Babel
TEMPLATE = app

INCLUDEPATH += /usr/local/include /usr/local/include/opus/ /usr/local/ssl/include ../openssl/include/openssl/

LIBS += -lportaudio -lopus -lssl -lcrypto -L/usr/local/lib -L/usr/lib

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        ../Client/portAudio.cpp           \
          ../Client/EncodeManager.cpp       \
          ../Client/Connection_Lin.cpp      \
          ../Client/Packet.cpp              \
          ../Client/Rsa.cpp                 \
          ../Client/BBException.cpp         \
          ../Client/ManagePacket.cpp        \
          ../Client/Action.cpp                      \
          ../Client/HandleConnection.cpp    \
    ../Client/mythread.cpp \
    ../Client/Connection_Win.cpp

HEADERS  += mainwindow.h \
    IGui.h \
    ../Client/AClient.h \
    ../Client/Action.hh \
    ../Client/BBException.hpp \
    ../Client/Connection_Lin.h \
    ../Client/Connection_Win.h \
    ../Client/EncodeManager.hh \
    ../Client/HandleConnection.h \
    ../Client/Identity.hpp \
    ../Client/IEncodeManager.hh \
    ../Client/IPortAudio.hh \
    ../Client/ManagePacket.hh \
    ../Client/Packet.h \
    ../Client/portAudio.hh \
    ../Client/Rsa2.h \
    ../Client/IClient.h \
    ../Client/mythread.h

FORMS    += mainwindow.ui

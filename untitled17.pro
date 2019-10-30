#-------------------------------------------------
#
# Project created by QtCreator 2019-10-18T14:32:54
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
CONFIG  += qaxcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled17
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

#INCLUDEPATH += $$PWD/3rdparty/CH-HCNetSDK_Win64/include

#LIBS += $$PWD/3rdparty/CH-HCNetSDK_Win64/lib/GdiPlus.lib \
#        $$PWD/3rdparty/CH-HCNetSDK_Win64/lib/HCCore.lib \
#        $$PWD/3rdparty/CH-HCNetSDK_Win64/lib/HCNetSDK.lib \
#        $$PWD/3rdparty/CH-HCNetSDK_Win64/lib/PlayCtrl.lib \

INCLUDEPATH += $$PWD/3rdparty/CH-HCNetSDK_Win32/include

LIBS += $$PWD/3rdparty/CH-HCNetSDK_Win32/lib/GdiPlus.lib \
        $$PWD/3rdparty/CH-HCNetSDK_Win32/lib/HCCore.lib \
        $$PWD/3rdparty/CH-HCNetSDK_Win32/lib/HCNetSDK.lib \
        $$PWD/3rdparty/CH-HCNetSDK_Win32/lib/PlayCtrl.lib \


SOURCES += \
        Logger.cpp \
        hcnet.cpp \
        main.cpp \
        mainwindow.cpp \
        worker.cpp

HEADERS += \
        Logger.h \
        LoggerTemplate.h \
        hcnet.h \
        mainwindow.h \
        webaxwidget.h \
        worker.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = video.ico

# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


TEMPLATE = app
TARGET = FCClient_Qt
DESTDIR = ../../bin/debug/client
QT += core gui multimedia network xml webkit qtmain
CONFIG += debug
DEFINES += _USE_STDAFX_H_ QT_LARGEFILE_SUPPORT QT_MULTIMEDIA_LIB QT_XML_LIB QT_NETWORK_LIB QT_WEBKIT_LIB
INCLUDEPATH += ./GeneratedFiles \
    ./GeneratedFiles/Debug \
    . \
    ./ui
win32:LIBS += -L"./../../common/pthreads-win32/lib" \
    -lpthreadVSE2
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(FCClient_Qt.pri)
win32:RC_FILE = FCClient_Qt.rc
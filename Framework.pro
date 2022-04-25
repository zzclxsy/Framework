CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

DEFINES += FRAMEWORK_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES+=StaticLib
include(./XEvent/XEvent.pri)
include(./XTime/XTime.pri)
include(./XLogPrint/XLogPrint.pri)
include(./XNetwork/XNetwork.pri)
include(./JsonCpp/JsonCpp.pri)
include(./XApi/XApi.pri)
# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

CONFIG(debug, debug|release) {
    TARGET = libFrameworkd
    DESTDIR = $$PWD/../lib/debug
} else {
    TARGET = libFramework
    DESTDIR = $$PWD/../lib/release
}


HEADERS += \
    XCoreApplication.h \
    XObjectManager.h \
    XRingBuffer.h

SOURCES += \
    XCoreApplication.cpp \
    XObjectManager.cpp \
    XRingBuffer.cpp


#boost库
INCLUDEPATH += $$PWD/../ThirdParty/inc
INCLUDEPATH += $$PWD/JsonCpp/include

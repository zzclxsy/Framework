CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(./XEvent/XEvent.pri)
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
    XCoreApplication.h

SOURCES += \
    XCoreApplication.cpp


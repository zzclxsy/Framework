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
include(./XConfig/XConfig.pri)
include(./XTask/XTask.pri)
include(./XTimer/XTimer.pri)
include(./XMQTT/XMQTT.pri)
# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target


CONFIG(debug, debug|release) {
    TARGET = libFrameworkd
    DESTDIR = E:\\lib\\x64\\debug\\XFramework
} else {
    TARGET = libFramework
    DESTDIR = E:\\lib\\x64\\release\\XFramework
}



HEADERS += \
    XCoreApplication.h \
    XHttpBaseDef.h \
    XHttpHelper.h \
    XHttpKeys.h \
    XObjectManager.h \
    XRingBuffer.h \
    XServer.h \
    XUtils.h

SOURCES += \
    XCoreApplication.cpp \
    XHttpHelper.cpp \
    XObjectManager.cpp \
    XRingBuffer.cpp \
    XServer.cpp \
    XUtils.cpp

DISTFILES += \
    AppConfig.json

#boost库头文件，需要另行添加
INCLUDEPATH += E:\\include
INCLUDEPATH += E:\\include\\mqtt
#JsonCpp头文件
INCLUDEPATH += $$PWD/JsonCpp/include

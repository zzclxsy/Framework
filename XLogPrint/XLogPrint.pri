HEADERS += \
    $$PWD/XLogPrint.h \
    $$PWD/common.h \
    $$PWD/device/XLogConsoleDevice.h \
    $$PWD/XLogContent.h \
    $$PWD/XLogDevice.h \
    $$PWD/device/XLogDeviceBase.h \
    $$PWD/XLogRule.h \
    $$PWD/device/XLogTcpDevice.h

SOURCES += \
    $$PWD/device/XLogConsoleDevice.cpp \
    $$PWD/XLogContent.cpp \
    $$PWD/XLogDevice.cpp \
    $$PWD/XLogRule.cpp \
    $$PWD/device/XLogTcpDevice.cpp

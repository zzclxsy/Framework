#ifndef XLOG_H
#define XLOG_H
#include "./XLogContent.h"
#include "./XLogDevice.h"
#include "./XLogRule.h"

#include "common.h"

#define XDEBUG                      XLogger(logLevel::E_DEBUG)
#define XDEBUG_TO(devName)          XLogger(logLevel::E_DEBUG, devName)
#define XDEBUG_PRINT                XLogger(logLevel::E_DEBUG).write

#define XINFO                       XLogger(logLevel::E_INFO)
#define XINFO_TO(devName)           XLogger(logLevel::E_INFO, devName)
#define XINFO_PRINT                 XLogger(logLevel::E_INFO).write

#define XWARN                       XLogger(logLevel::E_WARN)
#define XWARN_TO(devName)           XLogger(logLevel::E_WARN, devName)
#define XWARN_PRINT                 XLogger(logLevel::E_WARN).write


#define XERROR                      XLogger(logLevel::E_ERROR)
#define XERROR_TO(devName)          XLogger(logLevel::E_ERROR, devName)
#define XERROR_PRINT                XLogger(logLevel::E_ERROR).write

#define XLOG_Add_Device(XLogDevice, alias)  XLogContent::instant()->addDevive(XLogDevice, alias)
#define XLOG_Del_Device(alias)              XLogContent::instant()->removeDevice(alias)
#define XLOG_Get_Device(alias)              XLogContent::instant()->getDevice(alias)

#endif // XLOG_H

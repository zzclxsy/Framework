#ifndef XLOG_H
#define XLOG_H
#include "./XLogContent.h"
#include "./XLogDevice.h"
#include "./XLogRule.h"

#include "common.h"

#define XDEBUG                      XLogger(PriorityLevel::DEBUG)
#define XDEBUG_TO(devName)          XLogger(PriorityLevel::DEBUG, devName)
#define XDEBUG_PRINT                XLogger(PriorityLevel::DEBUG).write

#define XINFO                       XLogger(PriorityLevel::INFO)
#define XINFO_TO(devName)           XLogger(PriorityLevel::INFO, devName)
#define XINFO_PRINT                 XLogger(PriorityLevel::INFO).write

#define XWARN                       XLogger(PriorityLevel::WARN)
#define XWARN_TO(devName)           XLogger(PriorityLevel::WARN, devName)
#define XWARN_PRINT                 XLogger(PriorityLevel::WARN).write


#define XERROR                      XLogger(PriorityLevel::ERROR)
#define XERROR_TO(devName)          XLogger(PriorityLevel::ERROR, devName)
#define XERROR_PRINT                XLogger(PriorityLevel::ERROR).write

#define XLOG_Add_Device(XLogDevice, alias)  XLogContent::instant()->addDevive(XLogDevice, alias)
#define XLOG_Del_Device(alias)              XLogContent::instant()->removeDevice(alias)
#define XLOG_Get_Device(alias)              XLogContent::instant()->getDevice(alias)
#endif // XLOG_H

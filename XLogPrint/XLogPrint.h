#ifndef XLOG_H
#define XLOG_H
#include "./XLogContent.h"
#include "./XLogDevice.h"
#include "./XLogRule.h"

#include "common.h"

#define XDEBUG          XLogger(PriorityLevel::DEBUG)
#define XDEBUG_PRINT    XLogger(PriorityLevel::DEBUG).write

#define XINFO           XLogger(PriorityLevel::INFO)
#define XINFO_PRINT     XLogger(PriorityLevel::INFO).write

#define XWARN           XLogger(PriorityLevel::WARN)
#define XWARN_PRINT     XLogger(PriorityLevel::WARN).write


#define XERROR          XLogger(PriorityLevel::ERROR)
#define XERROR_PRINT    XLogger(PriorityLevel::ERROR).write

#define XLOG_ADD_DEVICE XLogContent::instant()->addDevive
#define XLOG_DEL_DEVICE XLogContent::instant()->removeDevice
#define XLOG_SET_RULE   XLogContent::instant()->setRule

#endif // XLOG_H

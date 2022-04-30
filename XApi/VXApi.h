#ifndef VXAPI_H
#define VXAPI_H
#include "VXTime.h"
#include "VXModule.h"
#include "VXModuleConfig.h"
#include "VXModuleTest.h"
#include "VXTcpServer.h"
#include "VXTaskManager.h"
#include "VXModuleTypeDef.h"
#include"VXFactory.h"

VXTime * XGetTimeModule();
VXModuleConfig *XGetConfigModule();
VXModuleTest *XGetPGMTest();
VXTaskManager *XGetTcpTaskManager();
#endif // VXAPI_H

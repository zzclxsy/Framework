#ifndef VXAPI_H
#define VXAPI_H
#include "VXTime.h"
#include "VXModule.h"
#include "VXModuleConfig.h"
#include "VXModuleTest.h"
#include "VXTcpServer.h"
#include "VXTcpClient.h"
#include "VXTaskManager.h"
#include "VXModuleTypeDef.h"
#include "VXFactory.h"
#include "VXSocketSession.h"

VXTime * XGetTimeModule();
VXModuleConfig *XGetConfigModule();
VXModuleTest *XGetPGMTest();
VXTaskManager *XGetTcpTaskManager();

#endif // VXAPI_H

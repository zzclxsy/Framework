#ifndef VXAPI_H
#define VXAPI_H
#include "VXTime.h"
#include "VXModuleConfig.h"
#include "VXModuleTest.h"
VXTime * XGetTimeModule();
VXModuleConfig *XGetConfigModule();
VXModuleTest *XGetPGMTest();
#endif // VXAPI_H

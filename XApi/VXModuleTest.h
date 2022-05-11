#ifndef VXMODULETEST_H
#define VXMODULETEST_H
#include "VXTestTask.h"
class VXModuleTest
{
public:
    virtual void ExecuteTask(VXTestTask * task) = 0;
    virtual void ClosedTask(VXTestTask * task) = 0;
    virtual void CancelTask(VXTestTask * task) = 0;
    virtual Json::Value GetTestItemList() = 0;
};

#endif // VXMODULETEST_H

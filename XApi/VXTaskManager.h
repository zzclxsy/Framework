#ifndef VXTASKMANAGER_H
#define VXTASKMANAGER_H
#include "VXTestTask.h"
class VXTaskManager
{
public:
    virtual VXTestTask * FindTask(const std::string& taskId) = 0;
    virtual void ClearTasks() = 0;
};


#endif // VXTASKMANAGER_H

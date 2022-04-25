#ifndef VXOBJECTMANAGER_H
#define VXOBJECTMANAGER_H
#include "VXModule.h"
#include "VXFactory.h"
class VXObjectManager{
public:
    virtual VXModule * GetModule(int id) = 0;
    virtual int Register(VXFactory * factory) = 0;
    virtual VXFactory * GetFactory(int id) = 0;
};

#endif // VXOBJECTMANAGER_H

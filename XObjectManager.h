#ifndef XOBJECTMANAGER_H
#define XOBJECTMANAGER_H
#include "XApi/VXObjectManager.h"

class XObjectManagerPrivate;
class XObjectManager:
        public VXObjectManager,
        public VXModule
{
public:
    XObjectManager();
    virtual ~XObjectManager();

    virtual VXModule *GetModule(int id);
    virtual int Register(VXFactory * factory);
    virtual VXFactory *GetFactory(int id);

private:
    XObjectManagerPrivate * d_ptr;
};

#endif // XOBJECTMANAGER_H

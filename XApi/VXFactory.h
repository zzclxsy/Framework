#ifndef VXFACTORY_H
#define VXFACTORY_H

#include "VXModule.h"
#include "VXModuleTypeDef.h"

class VXFactory
{
public:
    VXFactory(){}
    virtual ~VXFactory(){}

    virtual VXModule *create() = 0;
    virtual int getModuleID() = 0;
    virtual std::string getDescrip() = 0;
};


#define X_FACTORY_DECL(ClsName)                         \
    class ClsName##Factory: public VXFactory {          \
    public:                                             \
        ClsName##Factory(){}                            \
        virtual ~ClsName##Factory(){}                   \
        virtual VXModule *create();                     \
        virtual int getModuleID();                      \
        virtual std::string getDescrip();               \
    };

#define X_FACTORY_NAME(ClsName) ClsName##Factory

#define X_FACTORY_IMPL(ClsName, ModuleId, Description)  \
    VXModule * ClsName##Factory::create(){              \
        VXModule *p = new ClsName();                    \
        return p;                                       \
    }                                                   \
    int ClsName##Factory::getModuleID(){                \
        return ModuleId;                                \
    }                                                   \
    std::string ClsName##Factory::getDescrip(){         \
        return Description;                             \
    }


#define X_MODULE_INIT(ClsName)              \
    auto t = ClsName##Factory();            \
    m_xModule = t.getModuleID();            \
    m_xDescrip = t.getDescrip();

#endif // VXFACTORY_H

#ifndef VXMODULE_H
#define VXMODULE_H
#include <string>
#include "json/json.h"
class VXModule
{
public:
    VXModule(){}
    virtual ~VXModule(){}

    virtual bool Initialize(const Json::Value& /*cfgData*/){ return false; }
    virtual void Release(){}

    int GetModuleID(){return m_xModule;}
    std::string GetDescrip(){return m_xDescrip;}

protected:
    int m_xModule;
    std::string m_xDescrip;
};

#endif // VXMODULE_H

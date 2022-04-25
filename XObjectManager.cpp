#include "XObjectManager.h"
#include "XLogPrint/XLogPrint.h"
class XObjectManagerPrivate
{
public:
    std::map<int, VXFactory *> m_factories;
    std::map<int, VXModule *>  m_services;
    std::map<std::string, int> m_configs;
};

XObjectManager::XObjectManager()
{
    d_ptr = new XObjectManagerPrivate;
}

XObjectManager::~XObjectManager()
{
    auto it = d_ptr->m_services.begin();
    for (; it != d_ptr->m_services.end(); ++it)
    {
        it->second->Release();
        delete it->second;
    }
    d_ptr->m_services.clear();
}

VXModule *XObjectManager::GetModule(int id)
{
    std::map<int, VXModule *>::iterator it;
    it = d_ptr->m_services.find(id);
    if (it != d_ptr->m_services.end()){
        return it->second;
    }
    VXFactory * factory = this->GetFactory(id);
    if (!factory){
        XERROR << "JHObjectManager::GetModule, get factory failed, id:" << id;
        return nullptr;
    }

    VXModule * p = factory->create();
    d_ptr->m_services.insert(std::make_pair(id, p));
    return p;
}

int XObjectManager::Register(VXFactory *factory)
{
    int moduleId = factory->getModuleID();
    int key = moduleId;
    if (d_ptr->m_factories.find(key) != d_ptr->m_factories.end()){
        XERROR << "XObjectManager::Register, already registered" << moduleId;
        return -1;
    }
    d_ptr->m_factories.insert(std::make_pair(key, factory));
    XDEBUG << "XObjectManager::Register:\t key:" << key << ", description:" << factory->getDescrip();
    return (int)d_ptr->m_factories.size();
}

VXFactory *XObjectManager::GetFactory(int id)
{
    std::map<int, VXFactory *>::iterator it = d_ptr->m_factories.end();

    it = d_ptr->m_factories.find(id);
    if (it != d_ptr->m_factories.end())
    {
        return it->second;
    }

    return nullptr;
}

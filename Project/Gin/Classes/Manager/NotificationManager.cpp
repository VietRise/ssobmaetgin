#include "NotificationManager.h"

using namespace std;
USING_NS_CC;
using namespace rapidjson;

static NotificationManager* _instance = nullptr;

NotificationManager::NotificationManager()
{
}

NotificationManager::~NotificationManager()
{
    this->_listObserver.clear();
}

NotificationManager* NotificationManager::getInstance()
{
    if (_instance == nullptr)
        _instance = new NotificationManager();
    return _instance;
}

void NotificationManager::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_DELETE(_instance);
}

void NotificationManager::registerNotification(void* target, int id, const NotificationCallback& callback)
{
    NotifyObserver* observer = new NotifyObserver();
    observer->autorelease();
    observer->setTarget(target);
    observer->setId(id);
    observer->setCallbackFunction(callback);
    this->_listObserver.pushBack(observer);
}

void NotificationManager::registerNotificationWithJsonData(void* target, int id, const NotificationCallbackWithJsonData& callback)
{
    NotifyObserver* observer = new NotifyObserver();
    observer->autorelease();
    observer->setTarget(target);
    observer->setId(id);
    observer->setCallbackFunction(callback);
    this->_listObserver.pushBack(observer);
}

void NotificationManager::registerNotificationWithData(void* target, int id, const NotificationCallbackWithData& callback)
{
    NotifyObserver* observer = new NotifyObserver();
    observer->autorelease();
    observer->setTarget(target);
    observer->setId(id);
    observer->setCallbackFunction(callback);
    this->_listObserver.pushBack(observer);
}

void NotificationManager::unregisterNotification(void* target, int id)
{
    for (auto iter = this->_listObserver.begin(); iter != this->_listObserver.end();)
    {
        NotifyObserver* observer = (*iter);
        if (observer->getTarget() == target && observer->getId() == id)
            iter = this->_listObserver.erase(iter);
        else
            ++iter;
    }
}

void NotificationManager::unregisterNotification(void* target)
{
    for (auto iter = this->_listObserver.begin(); iter != this->_listObserver.end();)
    {
        NotifyObserver* observer = (*iter);
        if (observer->getTarget() == target)
            iter = this->_listObserver.erase(iter);
        else
            ++iter;
    }
}

void NotificationManager::postNotification(int id)
{
    auto      end      = this->_listObserver.end();
    for (auto observer = this->_listObserver.begin(); observer != end; ++observer)
    {
        if ((*observer)->getId() == id)
        {
            (*observer)->executeCallback(nullptr, nullptr);
            (*observer)->executeCallbackWithJsonData(nullptr, rapidjson::Document());
        }
    }
}

void NotificationManager::postNotification(int id, Ref* data)
{
    postNotification(id, nullptr, data);
}

void NotificationManager::postNotification(int id, void* sender, Ref* data)
{
    auto      end      = this->_listObserver.end();
    for (auto observer = this->_listObserver.begin(); observer != end; ++observer)
        if ((*observer)->getId() == id)
            (*observer)->executeCallback(sender, data);
}

void NotificationManager::postNotification(int id, void* sender, const cocos2d::Value& data)
{
    auto      end      = this->_listObserver.end();
    for (auto observer = this->_listObserver.begin(); observer != end; ++observer)
        if ((*observer)->getId() == id)
            (*observer)->executeCallbackWithData(sender, data);
}

void NotificationManager::postNotification(int id, void* sender, const rapidjson::Document& data)
{
    auto      end      = this->_listObserver.end();
    for (auto observer = this->_listObserver.begin(); observer != end; ++observer)
        if ((*observer)->getId() == id)
            (*observer)->executeCallbackWithJsonData(sender, data);
}

void NotificationManager::postNotification(int id, const rapidjson::Document& data)
{
    postNotification(id, nullptr, data);
}

void NotificationManager::postNotification(int id, const cocos2d::Value& data)
{
    postNotification(id, nullptr, data);
}

/// NotifyObserver
NotifyObserver::NotifyObserver()
{
    this->_name                         = "";
    this->_callbackFunction             = nullptr;
    this->_callbackFunctionWithJsonData = nullptr;
    this->_callbackFunctionWithData     = nullptr;
}

NotifyObserver::~NotifyObserver()
{
}

string NotifyObserver::getName()
{
    return this->_name;
}

void NotifyObserver::setName(const std::string& name)
{
    this->_name = name;
}

void NotifyObserver::setId(int id)
{
    this->_id = id;
}

int NotifyObserver::getId()
{
    return this->_id;
}

void* NotifyObserver::getTarget()
{
    return this->_target;
}

void NotifyObserver::setTarget(void* target)
{
    this->_target = target;
}

void NotifyObserver::setCallbackFunction(const NotificationCallback& callback)
{
    this->_callbackFunction = callback;
}

void NotifyObserver::setCallbackFunction(const NotificationCallbackWithJsonData& callback)
{
    this->_callbackFunctionWithJsonData = callback;
}

void NotifyObserver::setCallbackFunction(const NotificationCallbackWithData& callback)
{
    this->_callbackFunctionWithData = callback;
}

void NotifyObserver::executeCallback(void* sender, Ref* data)
{
    if (this->_callbackFunction != nullptr)
        this->_callbackFunction(sender, this->_id, data);
}

void NotifyObserver::executeCallbackWithJsonData(void* sender, const rapidjson::Document& data)
{
    if (this->_callbackFunctionWithJsonData != nullptr)
        this->_callbackFunctionWithJsonData(sender, this->_id, data);
}

void NotifyObserver::executeCallbackWithData(void* sender, const cocos2d::Value& data)
{
    if (this->_callbackFunctionWithData != nullptr)
        this->_callbackFunctionWithData(sender, this->_id, data);
}

void NotifyObserver::unregisterNotification()
{
}

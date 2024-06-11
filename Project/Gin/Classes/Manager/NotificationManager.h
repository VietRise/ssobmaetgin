#ifndef __NOTIFICATION_MANAGER_H__
#define __NOTIFICATION_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include <functional>

using namespace std::placeholders;

#define NOTIFYCALLBACK_SELECTOR(_TARGET, _SELECTOR)  std::bind(&_SELECTOR, _TARGET, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

typedef std::function<void(void* sender, int id, cocos2d::Ref* data)> NotificationCallback;
typedef std::function<void(void* sender, int id, const rapidjson::Document& data)> NotificationCallbackWithJsonData;
typedef std::function<void(void* sender, int id, const cocos2d::Value& data)> NotificationCallbackWithData;

class NotifyObserver : public cocos2d::Ref
{
private:
    void* _target;
    std::string _name;
    int         _id;

    NotificationCallback             _callbackFunction;
    NotificationCallbackWithJsonData _callbackFunctionWithJsonData;
    NotificationCallbackWithData     _callbackFunctionWithData;
protected:
public:

    NotifyObserver();
    ~NotifyObserver();

    std::string getName();
    void setName(const std::string& name);

    void setId(int id);
    int getId();

    void* getTarget();
    void setTarget(void* target);

    void setCallbackFunction(const NotificationCallback& callback);
    void setCallbackFunction(const NotificationCallbackWithJsonData& callback);
    void setCallbackFunction(const NotificationCallbackWithData& callback);

    void executeCallback(void* sender, cocos2d::Ref* data);
    void executeCallbackWithJsonData(void* sender, const rapidjson::Document& data);
    void executeCallbackWithData(void* sender, const cocos2d::Value& data);

    void unregisterNotification();
};

class NotificationManager
{
private:
    cocos2d::Vector<NotifyObserver*> _listObserver;
protected:
public:
    NotificationManager();
    ~NotificationManager();

    static NotificationManager* getInstance();
    static void destroyInstance();

    void registerNotification(void* target, int id, const NotificationCallback& callback);
    void registerNotificationWithJsonData(void* target, int id, const NotificationCallbackWithJsonData& callback);
    void registerNotificationWithData(void* target, int id, const NotificationCallbackWithData& callback);

    void unregisterNotification(void* target, int id);
    void unregisterNotification(void* target);

    void postNotification(int id);

    void postNotification(int id, cocos2d::Ref* data);
    void postNotification(int id, const rapidjson::Document& data);
    void postNotification(int id, const cocos2d::Value& data);

    void postNotification(int id, void* sender, cocos2d::Ref* data);
    void postNotification(int id, void* sender, const cocos2d::Value& data);
    void postNotification(int id, void* sender, const rapidjson::Document& data);
};


#endif //__NOTIFICATION_MANAGER_H__  
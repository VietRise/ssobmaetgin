#ifndef __MESSAGE_HANDLER_H__
#define __MESSAGE_HANDLER_H__

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <functional>
#include <map>
#include "Define/ServerClientMessageDefine.h"

#define MESSAGEHANDLE_SELECTOR(_SELECTOR)  std::bind(&_SELECTOR, this, std::placeholders::_1, std::placeholders::_2)

typedef std::function<void(MessageID messageId, const rapidjson::Document& data)> HandlerCallback;

class HandlerObject : public cocos2d::Ref
{
private:
    MessageID _messageId;
    Ref* _target;
    HandlerCallback _callback;
protected:
public:
    HandlerObject();
    ~HandlerObject();

    void setMessageId(MessageID messageId);
    MessageID getMessageId();

    void setTarget(Ref* target);
    Ref* getTarget();

    void setCallback(const HandlerCallback& callback);
    void postMessage(const rapidjson::Document& data);
};

class MessageHandler
{
private:
    std::map<MessageID, cocos2d::Vector<HandlerObject*>> _mapMessageHandler;
protected:
public:
    MessageHandler();
    ~MessageHandler();

    static MessageHandler* getInstance();
    static void destroyInstance();

    void registerHandleMessage(cocos2d::Ref* target, MessageID messageId, const HandlerCallback& callback);
    void unregisterHandleMessage(cocos2d::Ref* target, MessageID messageId);
    void unregisterHandleMessage(cocos2d::Ref* target);

    void postMessage(MessageID messageId, const rapidjson::Document& data);
};

#endif //__MESSAGE_HANDLER_H__

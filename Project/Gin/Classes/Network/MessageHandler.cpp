#include "MessageHandler.h"

USING_NS_CC;

static MessageHandler* _instance = nullptr;

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

MessageHandler* MessageHandler::getInstance()
{
    if (_instance == nullptr)
        _instance = new MessageHandler();
    return _instance;
}

void MessageHandler::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_DELETE(_instance);
}

void MessageHandler::registerHandleMessage(cocos2d::Ref* target, MessageID messageId, const HandlerCallback& callback)
{
    HandlerObject* handleObject = new HandlerObject();
    handleObject->autorelease();
    handleObject->setMessageId(messageId);
    handleObject->setCallback(callback);
    handleObject->setTarget(target);

    Vector<HandlerObject*>* object = &_mapMessageHandler[messageId];
    object->pushBack(handleObject);
}

void MessageHandler::unregisterHandleMessage(cocos2d::Ref* target, MessageID messageId)
{
    Vector<HandlerObject*>* listHandler = &_mapMessageHandler[messageId];
    for (auto             iter          = listHandler->begin(); iter != listHandler->end();)
    {
        if ((*iter)->getTarget() == target)
            iter = listHandler->erase(iter);
        else
            ++iter;
    }
}

void MessageHandler::unregisterHandleMessage(cocos2d::Ref* target)
{
    for (auto iter = this->_mapMessageHandler.begin(); iter != this->_mapMessageHandler.end(); iter++)
    {
        auto      listHandler = &(*iter).second;
        for (auto iter2       = listHandler->begin(); iter2 != listHandler->end();)
        {
            if ((*iter2)->getTarget() == target)
                iter2 = listHandler->erase(iter2);
            else
                ++iter2;
        }
    }
}

void MessageHandler::postMessage(MessageID messageId, const rapidjson::Document& data)
{
    if (_mapMessageHandler.find(messageId) != _mapMessageHandler.end())
    {
        const Vector<HandlerObject*>& listHandler = _mapMessageHandler.at(messageId);
        for (int i = 0; i < listHandler.size(); i++)
            listHandler.at(i)->postMessage(data);
    }
}

////////////////////////////////////////////////////////
////				HandlerObject					////
////////////////////////////////////////////////////////

HandlerObject::HandlerObject()
{
}

HandlerObject::~HandlerObject()
{
}

void HandlerObject::setMessageId(MessageID messageId)
{
    this->_messageId = messageId;
}

MessageID HandlerObject::getMessageId()
{
    return this->_messageId;
}

void HandlerObject::setTarget(Ref* target)
{
    this->_target = target;
}

Ref* HandlerObject::getTarget()
{
    return this->_target;
}

void HandlerObject::setCallback(const HandlerCallback& callback)
{
    this->_callback = callback;
}

void HandlerObject::postMessage(const rapidjson::Document& data)
{
    if (this->_callback != nullptr)
        this->_callback(this->_messageId, data);
}

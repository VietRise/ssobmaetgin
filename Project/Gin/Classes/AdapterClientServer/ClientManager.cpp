#include "ClientManager.h"
#include "AdapterClientServer.h"
#include "Network/MessageHandler.h"
#include "Helper/JsonHelper.h"
#include "Helper/GameUtils.h"
#include "Define/GameDefine.h"
#include "cocos2d.h"
#include "Define/ServerClientMessageDefine.h"

USING_NS_CC;
using namespace rapidjson;
using namespace std;

static ClientManager* _instance = nullptr;

ClientManager::~ClientManager()
{
}

ClientManager::ClientManager()
{
}

ClientManager* ClientManager::getInstance()
{
    if (!_instance)
        _instance = new ClientManager();
    return _instance;
}

void ClientManager::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_DELETE(_instance);
}

void ClientManager::sendMessage(const rapidjson::Document& data)
{
#if DEBUG_LOG_SEND_MESSAGE == 1
    LogHelper::message(data);
#endif
    AdapterClientServer::getInstance()->addMessageClientSender(data);
}

void ClientManager::onMessage(const rapidjson::Document& data)
{
#if DEBUG_LOG_RECEIVE_MESSAGE == 1
    LogHelper::message(data);
#endif
    MessageID msgId = (MessageID) JsonHelper::getInt(data, KEY_MESSAGE_ID, (int) MessageID::MESSAGE_UNKNOWN);
    MessageHandler::getInstance()->postMessage(msgId, data);
}

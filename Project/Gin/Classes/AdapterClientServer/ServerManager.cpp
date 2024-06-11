#include "ServerManager.h"
#include "AdapterClientServer.h"
#include "Network/MessageHandler.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "cocos2d.h"
#include "Define/ServerClientMessageDefine.h"

USING_NS_CC;
using namespace rapidjson;

static ServerManager* _instance = nullptr;

ServerManager::~ServerManager()
{
}

ServerManager::ServerManager()
{
}

ServerManager* ServerManager::getInstance()
{
    if (!_instance)
        _instance = new ServerManager();
    return _instance;
}

void ServerManager::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_DELETE(_instance);
}

void ServerManager::sendMessage(const rapidjson::Document& data)
{
    AdapterClientServer::getInstance()->addMessageServerSender(data);
}

void ServerManager::onMessage(const rapidjson::Document& data)
{
    MessageHandler::getInstance()->postMessage((MessageID) JsonHelper::getInt(data, KEY_MESSAGE_ID, -1), data);
}
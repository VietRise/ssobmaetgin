#include "AdapterClientServer.h"
#include "ClientManager.h"
#include "ServerManager.h"
#include "Helper/JsonHelper.h"
#include "Define/GameDefine.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

static AdapterClientServer* _instance = nullptr;

AdapterClientServer::~AdapterClientServer()
{
}

AdapterClientServer::AdapterClientServer()
{
}

AdapterClientServer* AdapterClientServer::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new AdapterClientServer();
        _instance->init();
    }
    return _instance;
}

void AdapterClientServer::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_RELEASE(_instance);
}

bool AdapterClientServer::init()
{
    if (!Node::init())
        return false;
    _scheduler->scheduleUpdate(this, Scheduler::PRIORITY_NON_SYSTEM_MIN, false);
    return true;
}

void AdapterClientServer::update(float dt)
{
    this->onProcessMessageFromClient();
    this->onProcessMessageFromServer();
}

void AdapterClientServer::onProcessMessageFromClient()
{
    if (_queueMessageClientSender.size() != 0)
    {
        vector<std::string> tempMessageClientSender = _queueMessageClientSender;
        _queueMessageClientSender.clear();
        for (const std::string& message : tempMessageClientSender)
            ServerManager::getInstance()->onMessage(JsonHelper::parseStringToJson(message));
    }
}

void AdapterClientServer::onProcessMessageFromServer()
{
    if (_queueMessageServerSender.size() != 0)
    {
        vector<std::string> tempMessageServerSender = _queueMessageServerSender;
        _queueMessageServerSender.clear();
        for (const std::string& message : tempMessageServerSender)
            ClientManager::getInstance()->onMessage(JsonHelper::parseStringToJson(message));
    }
}

void AdapterClientServer::addMessageClientSender(const rapidjson::Document& message)
{
    _queueMessageClientSender.push_back(JsonHelper::convertDocumentToString(message));
}

void AdapterClientServer::addMessageServerSender(const rapidjson::Document& message)
{
    _queueMessageServerSender.push_back(JsonHelper::convertDocumentToString(message));
}

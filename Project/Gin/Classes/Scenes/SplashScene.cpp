#include "SplashScene.h"
#include "Manager/SceneManager.h"
#include "Manager/SoundManager.h"
#include "AdapterClientServer/ClientManager.h"
#include "Helper/JsonHelper.h"
#include "Network/MessageSender.h"
#include "Network/MessageHandler.h"
#include "UserData/UserData.h"
#include "Manager/TextManager.h"
#include "AdapterClientServer/AdapterClientServer.h"
#include "Native/CallToOtherLanguage.h"
#include "ServerOffline/ServerOffline.h"
#include "Helper/GameUtils.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Manager/AdsManager.h"
#include "Component/GinCardStore.h"
#include "Define/ServerClientMessageDefine.h"

#define DEFAULT_TIME 1.0f

USING_NS_CC;
using namespace std;
using namespace ui;

SplashScene::~SplashScene()
{
}

SplashScene::SplashScene()
{
    this->_loadResourceDone = false;
    this->_timeLoading      = 0;
}

Scene* SplashScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SplashScene::create();
    scene->addChild(layer);
    return scene;
}

SplashScene* SplashScene::create()
{
    SplashScene* layer = new SplashScene();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool SplashScene::init()
{
    if (!InterfaceScene::init())
        return false;
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_LOAD_SAVED_MATCH_RESPONSE, MESSAGEHANDLE_SELECTOR(SplashScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_USER_DATA_RESPONSE, MESSAGEHANDLE_SELECTOR(SplashScene::onServerResponse));

    this->setContentSize(_screenSize);

    Node* node  = CSLoader::createNode("ui/SplashScene/SplashScene.csb");
    Node* child = node->getChildByName("root");
    child->removeFromParent();
    auto root = static_cast<cocos2d::ui::Layout*>(child);
    root->setAnchorPoint(Vec2(0.5f, 0.5f));
    root->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(root, kMiddleground);
    ui::Helper::doLayout(this);
    this->schedule(CC_SCHEDULE_SELECTOR(SplashScene::updateCheckStartup), 1.0f);
    return true;
}

void SplashScene::onEnter()
{
    InterfaceScene::onEnter();
}

void SplashScene::onExit()
{
    MessageHandler::getInstance()->unregisterHandleMessage(this);
    InterfaceScene::onExit();
}

void SplashScene::onLoadResourcesSyncDone()
{
}

void SplashScene::onLoadResourcesAsyncDone()
{
    this->_loadResourceDone = true;
}

SceneID SplashScene::getSceneID()
{
    return SceneID::COMMON_SCENE;
}

void SplashScene::onServerResponse(MessageID messageId, const rapidjson::Document& data)
{
    switch (messageId)
    {
        case MessageID::MESSAGE_LOAD_SAVED_MATCH_RESPONSE:
            onLoadSave(data);
            break;
        case MessageID::MESSAGE_USER_DATA_RESPONSE:
            onUserData(data);
            break;
        default:
            break;
    }
}

void SplashScene::onLoadSave(const rapidjson::Document& data)
{
    SceneManager::getInstance()->openScene(SceneID::LOBBY_SCENE);
}

void SplashScene::onUserData(const rapidjson::Document& data)
{
    double      money  = data[KEY_MONEY].GetDouble();
    std::string name   = data[KEY_NAME].GetString();
    std::string avatar = data[KEY_AVATAR].GetString();

    UserData::getInstance()->setLevel(JsonHelper::getInt(data, KEY_LEVEL, 0));
    UserData::getInstance()->setLevelExp(JsonHelper::getInt(data, KEY_LEVELEXP, 0));
    UserData::getInstance()->setNextLevelExp(JsonHelper::getInt(data, KEY_NEXTLEVELEXP, 0));

    UserData::getInstance()->setMoney(money);
    UserData::getInstance()->setUserName(name);
    UserData::getInstance()->setAvatarLink(avatar);
}

void SplashScene::updateCheckStartup(float dt)
{
    this->_timeLoading += dt;
    if ((this->_loadResourceDone && this->_timeLoading >= 7) || (this->_loadResourceDone && CallToOtherLanguage::getInstance()->checkStartup()))
    {
        s_versionCode       = CallToOtherLanguage::getInstance()->getVersionCode();
        s_versionName       = CallToOtherLanguage::getInstance()->getVersionName();
        s_enableShowAds     = RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_SHOW_ADS, true);
        s_enableGameService = RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_GAME_SERVICE_ENABLE, true);
        s_enableSafeArea    = CallToOtherLanguage::getInstance()->hasSafeArea();

        AdapterClientServer::getInstance();
        AdsManager::getInstance();
        ServerOffline::getInstance();
        ServerOffline::getInstance()->resetUserData();
        TextManager::getInstance()->loadData("localize.json");
        GinCardStore::getInstance()->loadCardDefine("card_define.json");

        DelayTime* delay    = DelayTime::create(DEFAULT_TIME * 2.0f);
        CallFunc * callback = CallFunc::create([=] {
            if (UserData::getInstance()->getSoundBackground())
            {
                if (SoundManager::getInstance()->isMusicActive())
                {
                    UserData::getInstance()->setSoundBackground(false);
                    UserData::getInstance()->setRestoreSoundBG(true);
                    SoundManager::getInstance()->stopBackgroundMusicAuto();
                }
                else
                {
                    UserData::getInstance()->setRestoreSoundBG(false);
                    SoundManager::getInstance()->stopBackgroundMusicAuto();
                    SoundManager::getInstance()->playBackgroundMusicAuto();
                }
            }
            else
            {
                GameUtils::checkToRestoreSoundBackground();
            }
            if (CallToOtherLanguage::getInstance()->havePlayGameService() && RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_AUTO_SIGN_IN_GAME_SERVICE, false))
                CallToOtherLanguage::getInstance()->signinPlayGameService();

            MessageSender::requestLoadSavedMatch();
        });
        this->runAction(Sequence::create(delay, callback, nullptr));
        this->unschedule(schedule_selector(SplashScene::updateCheckStartup));
    }
}

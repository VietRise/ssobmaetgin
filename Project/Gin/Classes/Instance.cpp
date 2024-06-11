#include "Instance.h"
#include "Manager/NotificationManager.h"
#include "Network/MessageHandler.h"
#include "Manager/MyActionsManager.h"
#include "Manager/DownloadManager.h"
#include "Manager/TextManager.h"
#include "Manager/SoundManager.h"
#include "Manager/AdsManager.h"
#include "UserData/UserData.h"
#include "Manager/ResourcesManager.h"
#include "Component/GinCardStore.h"
#include "Scenes/DebugLayer.h"
#include "AdapterClientServer/AdapterClientServer.h"
#include "AdapterClientServer/ClientManager.h"
#include "AdapterClientServer/ServerManager.h"
#include "Manager/SceneManager.h"
#include "Manager/BoardManager.h"
#include "ServerOffline/ServerOffline.h"
#include "Native/CallToOtherLanguage.h"
#include "Native/NativeCallBack.h"
#include "Scenes/Gin/GPGinSlotManager.h"
#include "Scenes/CheatCardLayer.h"
#include "Scenes/LoadingLayer.h"

void Instance::destroyInstance()
{
    LoadingLayer::destroyInstance();
	GPGinSlotManager::destroyInstance();
	GinCardStore::destroyInstance();
    DebugLayer::destroyInstance();
    CheatCardLayer::destroyInstance();
    UserData::destroyInstance();
    SceneManager::destroyInstance();
    BoardManager::destroyInstance();
    ServerOffline::destroyInstance();
    MessageHandler::destroyInstance();
    TextManager::destroyInstance();
    DownloadManager::destroyInstance();
    ResourcesManager::destroyInstance();
    MyActionsManager::destroyInstance();
    SoundManager::destroyInstance();
    SceneManager::destroyInstance();
    NotificationManager::destroyInstance();
    ClientManager::destroyInstance();
    ServerManager::destroyInstance();
    AdapterClientServer::destroyInstance();
    CallToOtherLanguage::destroyInstance();
    NativeCallBack::destroyInstance();
    AdsManager::destroyInstance();
}

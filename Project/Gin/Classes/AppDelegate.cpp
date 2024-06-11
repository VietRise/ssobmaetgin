#include "AppDelegate.h"
#include "Instance.h"
#include "UserData/UserData.h"
#include "Manager/SceneManager.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"
#include "Define/RemoteConfigDefine.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Native/CallToOtherLanguage.h"
#include "Helper/fmt/format.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#define DEBUG_SCREEN_WIDTH 1300
#define DEBUG_SCREEN_HEIGHT 640

//#define DEBUG_SCREEN_WIDTH 1300
//#define DEBUG_SCREEN_HEIGHT 975
#endif

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    destroyInstance();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    FileUtils::getInstance()->addSearchPath("cstd");
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    FileUtils::getInstance()->addSearchPath("data_ios");
#else
    FileUtils::getInstance()->addSearchPath("data");
#endif
    UserData::getInstance()->obtainDeviceInfo();

    auto director = Director::getInstance();
    auto glview   = director->getOpenGLView();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    Size size = Size(DEBUG_SCREEN_WIDTH, DEBUG_SCREEN_HEIGHT);
#endif
    if (!glview)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect(GameUtils::getGameName() +  fmt::format(" ({0} x {1})", (int) size.width, (int) size.height), Rect(0, 0, DEBUG_SCREEN_WIDTH, DEBUG_SCREEN_HEIGHT), 1.0f);
#else
        glview = GLViewImpl::create("Boss");
#endif
        director->setOpenGLView(glview);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    glview->setFrameSize(size.width, size.height);
#endif
    s_resolution_ratio = glview->getFrameSize().width / glview->getFrameSize().height;
    //  turn on display FPS
    //  director->setDisplayStats(true);

    director->setAnimationInterval(1.0 / 60);
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    SceneManager::getInstance()->openScene(SceneID::COMMON_SCENE);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CallToOtherLanguage::getInstance()->scheduleLocalNotification(s_localNotficationData);
    GameUtils::pauseGame();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    // Remove all local notifications
    if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_LOCAL_NOTIFICATION_CLEAN_CURRENT, true))
        CallToOtherLanguage::getInstance()->cleanAllLocalNotification();
    CallToOtherLanguage::getInstance()->cancelAllLocalNotification();
    //
    GameUtils::resumeGame();
}

void AppDelegate::destroyInstance()
{
    Instance::destroyInstance();
}

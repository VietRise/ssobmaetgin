#include "CallToOtherLanguage.h"
#include "Define/GameDefine.h"
#include "Native/NativeCallBack.h"
#include "Helper/GameUtils.h"
#include "Define/InGameDefine.h"
#include "Manager/AdsManager.h"

USING_NS_CC;

static CallToOtherLanguage* _instance = nullptr;

CallToOtherLanguage::~CallToOtherLanguage()
{
}

CallToOtherLanguage::CallToOtherLanguage()
{
}

CallToOtherLanguage* CallToOtherLanguage::getInstance()
{
    if (_instance == nullptr)
        _instance = new CallToOtherLanguage();
    return _instance;
}

void CallToOtherLanguage::destroyInstance()
{
    if (_instance)
        CC_SAFE_RELEASE(_instance);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
bool CallToOtherLanguage::checkInternet()
{
    return true;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
void CallToOtherLanguage::showAlert(const char* message)
{
}
#endif

void CallToOtherLanguage::openAppInStore()
{
    CCLOG("openAppInStore");
}

void CallToOtherLanguage::loadAllAds()
{
}

void CallToOtherLanguage::initAds()
{
}

void CallToOtherLanguage::fetchRewardedAds()
{
}

bool CallToOtherLanguage::showRewardedAds(VideoAdsRequestSourceEnum source)
{
    NativeCallBack::getInstance()->watchVideoCompleted(source);
    return false;
}

bool CallToOtherLanguage::isRewardedAdsAvailable()
{
    return true;
}

void CallToOtherLanguage::showBannerAds(const std::string& name, int pos, float percentX, float percentY)
{
}

void CallToOtherLanguage::hideBannerAds(const std::string& name)
{
}

bool CallToOtherLanguage::canShowInterstitialAds()
{
    return true;
}

void CallToOtherLanguage::loadInterstitialAds()
{
}

void CallToOtherLanguage::showInterstitialAds(bool callback, const std::string& nameFunction)
{
    CCLOG("showInterstitialAds");
    if (callback)
        AdsManager::getInstance()->callbackInterstitial(nameFunction, true);
}

void CallToOtherLanguage::openAnotherAppInStore(const std::string& packageName)
{
    CCLOG("openAnotherAppInStore: %s", packageName.c_str());
}

void CallToOtherLanguage::openAnotherAppInStoreByLink(const std::string& url)
{
    CCLOG("openAnotherAppInStoreByLink: %s", url.c_str());
}

void CallToOtherLanguage::submitLeaderboardMoney(double money)
{
}

void CallToOtherLanguage::submitLeaderboardLevel(double level)
{
}

void CallToOtherLanguage::showLeaderBoardBillionaireClub()
{
}

void CallToOtherLanguage::showLeaderBoardMillionaireClub()
{
}

void CallToOtherLanguage::showLeaderBoardLevel()
{
}

void CallToOtherLanguage::signinPlayGameService()
{
}

void CallToOtherLanguage::signoutPlayGameService()
{
}

bool CallToOtherLanguage::isPlayGameServiceSignedIn()
{
    if (s_enableGameService)
        return false;
    return false;
}

bool CallToOtherLanguage::havePlayGameService()
{
    return s_enableGameService;
}

std::string CallToOtherLanguage::getMyPackageName()
{
    return DEFAULT_PACKAGE_NAME;
}

bool CallToOtherLanguage::isPackageInstalled(const std::string& packageName)
{
    return true;
}

int CallToOtherLanguage::getVersionCode()
{
    return 1;
}

std::string CallToOtherLanguage::getVersionName()
{
    return "0.0.0";
}

void CallToOtherLanguage::scheduleLocalNotification(const std::string& data)
{
}

void CallToOtherLanguage::cancelAllLocalNotification()
{
}

void CallToOtherLanguage::cleanAllLocalNotification()
{
}

std::string CallToOtherLanguage::getURLAppStore()
{
    return "";
}

std::string CallToOtherLanguage::getURLAppStoreTracking(const std::string& source, const std::string& campaign)
{
    return "";
}

void CallToOtherLanguage::shareDataToMessenger(const std::string& title, const std::string& message)
{
    CCLOG("shareDataToMessenger: %s %s", title.c_str(), message.c_str());
}

void CallToOtherLanguage::forceToRegisterAudioFocusListenner()
{
}

bool CallToOtherLanguage::checkStartup()
{
    return true;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
bool CallToOtherLanguage::hasSafeArea()
{
    return false;
}
#endif

void CallToOtherLanguage::showAppTrackingTransparency()
{
}

void CallToOtherLanguage::showInAppReview(int state, const std::string& condition)
{
    CCLOG("showInAppReview: %d %s", state, condition.c_str());
}

bool CallToOtherLanguage::willPlayBackgroundMusic()
{
    return false;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
void CallToOtherLanguage::copyToClipboard(const std::string& str)
{
    FileUtils::getInstance()->writeStringToFile(getStringLog(), "../log.txt");
}
#endif

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "platform/android/jni/JniHelper.h"
#include <jni.h>

bool CallToOtherLanguage::checkInternet()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "checkInternet", "()Z"))
    {
        jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return false;
}

void CallToOtherLanguage::showAlert(const char* message)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "showAlert", "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(message);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::openAppInStore()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "openAppInStore", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::openAnotherAppInStore(const std::string& packageName)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "openAnotherAppInStore", "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(packageName.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::openAnotherAppInStoreByLink(const std::string& url)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "openAnotherAppInStoreByLink", "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(url.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::loadAllAds()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "callLoadAllAds", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::initAds()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "initAds", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::fetchRewardedAds()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_FetchRewardedAds", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

bool CallToOtherLanguage::showRewardedAds(VideoAdsRequestSourceEnum source)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_ShowRewardedAds", "(I)Z"))
    {
        jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID, (jint) source);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return false;
}

bool CallToOtherLanguage::isRewardedAdsAvailable()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_IsRewardedAdsAvailable", "()Z"))
    {
        jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return false;
}

void CallToOtherLanguage::showBannerAds(const std::string& name, int pos, float percentX, float percentY)
{
    /*if (name == "")
    {
        name = "ALL";
    }
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_ShowBannerAds", "(Ljava/lang/String;Z)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(name.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, top);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }*/
}

void CallToOtherLanguage::hideBannerAds(const std::string& name)
{
    /*if (name == "")
    {
        name = "ALL";
    }
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_HideBannerAds", "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(name.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }*/
}

bool CallToOtherLanguage::canShowInterstitialAds()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_CanShowInterstitialAds", "()Z"))
    {
        jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return false;
}

void CallToOtherLanguage::loadInterstitialAds()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_LoadInterstitialAds", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}


void CallToOtherLanguage::showInterstitialAds(bool callback, const std::string& nameFunction)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AD_MANAGER_CLASS_NAME, "call_ShowInterstitialAds", "(ZLjava/lang/String;)V"))
    {
        jstring jni_nameFunction = t.env->NewStringUTF(nameFunction.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, callback, jni_nameFunction);
        t.env->DeleteLocalRef(jni_nameFunction);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::submitLeaderboardMoney(double money)
{
    if (s_enableGameService)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           PLAY_GAME_SERVICE_CLASS_NAME,    // fully qualified class name
                                           "submitLeaderBoardMoney",                              // method name
                                           "(D)V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID, money);
            // Delete unused data
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void CallToOtherLanguage::submitLeaderboardLevel(double level)
{
    if (s_enableGameService)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           PLAY_GAME_SERVICE_CLASS_NAME,    // fully qualified class name
                                           "submitLeaderBoardLevel",                              // method name
                                           "(D)V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID, level);
            // Delete unused data
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void CallToOtherLanguage::showLeaderBoardBillionaireClub()
{
    if (s_enableGameService)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           PLAY_GAME_SERVICE_CLASS_NAME,    // fully qualified class name
                                           "showLeaderBoardBillionaireClub",                              // method name
                                           "()V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            // Delete unused data
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void CallToOtherLanguage::showLeaderBoardMillionaireClub()
{
    if (s_enableGameService)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           PLAY_GAME_SERVICE_CLASS_NAME,    // fully qualified class name
                                           "showLeaderBoardMillionaireClub",                              // method name
                                           "()V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            // Delete unused data
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void CallToOtherLanguage::showLeaderBoardLevel()
{
    if (s_enableGameService)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           PLAY_GAME_SERVICE_CLASS_NAME,    // fully qualified class name
                                           "showLeaderBoardLevel",                              // method name
                                           "()V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            // Delete unused data
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void CallToOtherLanguage::signinPlayGameService()
{
    if (s_enableGameService)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           PLAY_GAME_SERVICE_CLASS_NAME,    // fully qualified class name
                                           "signinPlayGameService",                              // method name
                                           "()V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            // Delete unused data
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void CallToOtherLanguage::signoutPlayGameService()
{
    if (s_enableGameService)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           PLAY_GAME_SERVICE_CLASS_NAME,    // fully qualified class name
                                           "signoutPlayGameService",                              // method name
                                           "()V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            // Delete unused data
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

bool CallToOtherLanguage::isPlayGameServiceSignedIn()
{
    if (s_enableGameService)
    {
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, PLAY_GAME_SERVICE_CLASS_NAME, "isSignedIn", "()Z"))
        {
            jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
            return value;
        }
    }
    return false;
}

bool CallToOtherLanguage::havePlayGameService()
{
    if (s_enableGameService)
    {
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, PLAY_GAME_SERVICE_CLASS_NAME, "havePlayGameService", "()Z"))
        {
            jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
            return value;
        }
    }
    return false;
}

void CallToOtherLanguage::copyToClipboard(const std::string& str)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,    // fully qualified class name
                                       "copyToClipboard",                              // method name
                                       "(Ljava/lang/String;)V"))
    {
        jstring jstr = t.env->NewStringUTF(str.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, jstr);
        // Delete unused data
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
}

std::string CallToOtherLanguage::getMyPackageName()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t,
                                                APP_ACTIVITY_CLASS_NAME,
                                                "getMyPackageName",
                                                "()Ljava/lang/String;"))
    {
        jstring     value            = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string valuePackageName = JniHelper::jstring2string(value);
        t.env->DeleteLocalRef(value);
        t.env->DeleteLocalRef(t.classID);
        return valuePackageName;
    }
    return "";
}

bool CallToOtherLanguage::isPackageInstalled(const std::string& packageName)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t,
                                                APP_ACTIVITY_CLASS_NAME,
                                                "isPackageInstalled",
                                                "(Ljava/lang/String;)Z"))
    {
        jstring  jstr  = t.env->NewStringUTF(packageName.c_str());
        jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID, jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return false;
}

int CallToOtherLanguage::getVersionCode()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "getVersionCode", "()I"))
    {
        jint value = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return 0;
}

std::string CallToOtherLanguage::getVersionName()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t,
                                                APP_ACTIVITY_CLASS_NAME,
                                                "getVersionName",
                                                "()Ljava/lang/String;"))
    {
        jstring     value            = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string valuePackageName = JniHelper::jstring2string(value);
        t.env->DeleteLocalRef(value);
        t.env->DeleteLocalRef(t.classID);
        return valuePackageName;
    }
    return "0.0.0";
}

// Local notification
void CallToOtherLanguage::scheduleLocalNotification(const std::string& data)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,
                                       "scheduleLocalNotification",
                                       "(Ljava/lang/String;)V"))
    {
        jstring jstr = t.env->NewStringUTF(data.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, jstr);
        // Delete unused data
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::cancelAllLocalNotification()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,
                                       "cancelAllLocalNotification",
                                       "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::cleanAllLocalNotification()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,
                                       "cleanAllLocalNotification",
                                       "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

std::string CallToOtherLanguage::getURLAppStore()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,
                                       "getURLAppStore",
                                       "()Ljava/lang/String;"))
    {
        jstring     url = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string res = JniHelper::jstring2string(url);
        // Delete unused data
        t.env->DeleteLocalRef(url);
        t.env->DeleteLocalRef(t.classID);
        return res;
    }
    return "";
}

std::string CallToOtherLanguage::getURLAppStoreTracking(const std::string& source, const std::string& campaign)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,
                                       "getURLAppStoreTracking",
                                       "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
    {
        jstring str1 = t.env->NewStringUTF(source.c_str());
        jstring str2 = t.env->NewStringUTF(campaign.c_str());

        jstring     url = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID, str1, str2);
        std::string res = JniHelper::jstring2string(url);
        // Delete unused data
        t.env->DeleteLocalRef(str1);
        t.env->DeleteLocalRef(str2);
        t.env->DeleteLocalRef(url);
        t.env->DeleteLocalRef(t.classID);
        return res;
    }
    return "";
}

void CallToOtherLanguage::shareDataToMessenger(const std::string& title, const std::string& message)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,
                                       "shareDataToMessenger",
                                       "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jtitle   = t.env->NewStringUTF(title.c_str());
        jstring jmessage = t.env->NewStringUTF(message.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jtitle, jmessage);
        // Delete unused data
        t.env->DeleteLocalRef(jtitle);
        t.env->DeleteLocalRef(jmessage);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CallToOtherLanguage::forceToRegisterAudioFocusListenner()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       APP_ACTIVITY_CLASS_NAME,
                                       "forceToRegisterAudioFocusListenner",
                                       "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

bool CallToOtherLanguage::checkStartup()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "checkStartup", "()Z"))
    {
        jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return false;
}

bool CallToOtherLanguage::hasSafeArea()
{
    return false;
}

void CallToOtherLanguage::showAppTrackingTransparency()
{
}

void CallToOtherLanguage::showInAppReview(int state, const std::string& condition)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, STORE_SERVICE_CLASS_NAME, "showInAppReview", "(ILjava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(condition.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, (jint) state, stringArg1);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

bool CallToOtherLanguage::willPlayBackgroundMusic()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, APP_ACTIVITY_CLASS_NAME, "willPlayBackgroundMusic", "()Z"))
    {
        jboolean value = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return value;
    }
    return false;
}

#endif

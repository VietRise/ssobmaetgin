#include "AdsManager.h"
#include "Native/CallToOtherLanguage.h"
#include "Helper/GameUtils.h"
#include "Manager/NotificationManager.h"

USING_NS_CC;
using namespace std;

#define CALLBACK_BACKUP_TIME 4
#define INTERSTITIAL_CHECK_TIME 4

static AdsManager* _instance = nullptr;

AdsManager::~AdsManager()
{
}

AdsManager::AdsManager()
{
    _timeCheckInterstitial     = 0;
    _showInterstitialTimestamp = -1;
    _nameCallBackInterstitial  = "";
    _callbackInterstitial      = false;
}

AdsManager* AdsManager::getInstance()
{
    if (!_instance)
    {
        _instance = AdsManager::create();
        _instance->retain();
    }
    return _instance;
}

void AdsManager::destroyInstance()
{
    if (_instance)
        CC_SAFE_RELEASE_NULL(_instance);
}

bool AdsManager::init()
{
    if (!Node::init())
        return false;
    _scheduler->schedule(schedule_selector(AdsManager::update), this, 1.0f, false);
    return true;
}

void AdsManager::update(float dt)
{
    if (s_enableShowAds)
    {
        this->scheduleFetchInterstitial(dt);
        if (_callbackInterstitial && _showInterstitialTimestamp != -1 && (GameUtils::getCurrentTime() - _showInterstitialTimestamp) > CALLBACK_BACKUP_TIME)
            callbackInterstitial(_nameCallBackInterstitial, false);
    }
}

void AdsManager::scheduleFetchInterstitial(const float& dt)
{
    _timeCheckInterstitial += dt;
    if (_timeCheckInterstitial > INTERSTITIAL_ADMOB_SCHEDULE)
    {
        _timeCheckInterstitial = 0;
        if (CallToOtherLanguage::getInstance()->checkInternet())
            loadInterstitialAds();
    }
}

void AdsManager::scheduleFetchVideo()
{
    _scheduler->unschedule(REWARD_VIDEO_ADMOB_SCHEDULE_NAME, this);
    _scheduler->schedule([=](float dt) {
        if (!isRewardedAdsAvailable() && CallToOtherLanguage::getInstance()->checkInternet())
            fetchRewardedAds();
    }, this, REWARD_VIDEO_ADMOB_SCHEDULE_TIME, 2, 2.0f, false, REWARD_VIDEO_ADMOB_SCHEDULE_NAME);
}

void AdsManager::initAds()
{
    if (s_enableShowAds)
        CallToOtherLanguage::getInstance()->initAds();
}

void AdsManager::loadAllAds()
{
    if (s_enableShowAds)
        CallToOtherLanguage::getInstance()->loadAllAds();
}

void AdsManager::fetchRewardedAds()
{
    if (s_enableShowAds)
        CallToOtherLanguage::getInstance()->fetchRewardedAds();
}

bool AdsManager::isRewardedAdsAvailable()
{
    return s_enableShowAds && CallToOtherLanguage::getInstance()->isRewardedAdsAvailable();
}

void AdsManager::showBannerAds(const std::string& name, int pos, float percentX, float percentY)
{
    if (s_enableShowAds)
        CallToOtherLanguage::getInstance()->showBannerAds(name, pos, percentX, percentY);
}

void AdsManager::hideBannerAds(const std::string& name)
{
    if (s_enableShowAds)
        CallToOtherLanguage::getInstance()->hideBannerAds(name);
}

bool AdsManager::canShowInterstitialAds()
{
    return s_enableShowAds && CallToOtherLanguage::getInstance()->canShowInterstitialAds();
}

void AdsManager::showInterferingAds(bool show, bool callback, const std::string& nameFunction)
{
    if (_callbackInterstitial && _nameCallBackInterstitial != "")
        callbackInterstitial(_nameCallBackInterstitial, false);
    setCallbackInterstitial(callback, GameUtils::getCurrentTime(), nameFunction);
    if (show && canShowInterstitialAds())
    {
#if USE_AUTO_PLAY_CARD == 0
        showInterstitialAds(callback, nameFunction);
#else
        callbackInterstitial(nameFunction, true);
#endif
    }
    else if (callback)
    {
        callbackInterstitial(nameFunction, true);
    }
}

void AdsManager::loadInterstitialAds()
{
    if (s_enableShowAds)
        CallToOtherLanguage::getInstance()->loadInterstitialAds();
}

void AdsManager::showInterstitialAds(bool callback, const std::string& function)
{
    if (s_enableShowAds)
    {
        GameUtils::showAdsLoading(Director::getInstance()->getRunningScene(), LOADING_ADS_DELAYTIME, [=]() {
            CallToOtherLanguage::getInstance()->showInterstitialAds(callback, function);
        });
    }
}

void AdsManager::showRewardedAds(VideoAdsRequestSourceEnum source)
{
    if (s_enableShowAds)
        GameUtils::showAdsLoading(Director::getInstance()->getRunningScene(), LOADING_ADS_DELAYTIME, [=]() {
            CallToOtherLanguage::getInstance()->showRewardedAds(source);
        });
}

void AdsManager::callbackInterstitial(const std::string& function, bool closedKey)
{
    if (_callbackInterstitial && _nameCallBackInterstitial == function && _showInterstitialTimestamp > -1)
    {
        bool adShowed = closedKey;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (!closedKey && GameUtils::getCurrentTime() - _showInterstitialTimestamp > INTERSTITIAL_CHECK_TIME)
            adShowed = true;
#endif
        cocos2d::ValueMap data;
        data[AD_NAME_FUNCTION_KEY] = cocos2d::Value(function);
        data[AD_CLOSED_KEY]        = cocos2d::Value(adShowed);
        NotificationManager::getInstance()->postNotification(EVENT_INTERSTITIAL_HIDE, cocos2d::Value(data));
        setCallbackInterstitial(false, -1, "");
    }
}

void AdsManager::setCallbackInterstitial(bool callback, double time, const std::string& function)
{
    _callbackInterstitial      = callback;
    _showInterstitialTimestamp = time;
    _nameCallBackInterstitial  = function;
}
#include "NativeCallBack.h"
#include "Network/MessageSender.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/NotificationManager.h"
#include "Manager/AdsManager.h"

static NativeCallBack* _instance = nullptr;

NativeCallBack::~NativeCallBack()
{
}

NativeCallBack::NativeCallBack()
{
}

NativeCallBack* NativeCallBack::getInstance()
{
    if (!_instance)
        _instance = new NativeCallBack();
    return _instance;
}

void NativeCallBack::destroyInstance()
{
    if (_instance)
        CC_SAFE_DELETE(_instance);
}

void NativeCallBack::interstitialHide(const std::string& nameFunction, bool adClosed)
{
    AdsManager::getInstance()->callbackInterstitial(nameFunction, adClosed);
}

void NativeCallBack::onPlayGameServiceSignedIn()
{
    NotificationManager::getInstance()->postNotification(EVENT_PLAYGAME_SIGNIN, cocos2d::Value("signin"));
}

void NativeCallBack::onPlayGameServiceSignedOut()
{
    NotificationManager::getInstance()->postNotification(EVENT_PLAYGAME_SIGNOUT, cocos2d::Value("signout"));
}

void NativeCallBack::watchVideoCompleted(VideoAdsRequestSourceEnum source)
{
    switch (source)
    {
        case VideoAdsRequestSourceEnum::OLD_VIDEO_REWARD:
            MessageSender::requestBonusMoneyWatchVideo();
            break;
        case VideoAdsRequestSourceEnum::VIDEO_REWARD_SKIP_WAITING:
            MessageSender::requestVideoRewardSkip();
            break;
        default:
            break;
    }
}

void NativeCallBack::completedAppReview()
{
    MessageSender::requestRatingSubmit(5);
    NotificationManager::getInstance()->postNotification(EVENT_RATING_REVIEW_COMPLETED, cocos2d::Value());
}

#include "CallToOtherLanguage.h"
#include "Network/MessageSender.h"
#include "Manager/NotificationManager.h"
#include "Manager/AdsManager.h"
#include "Helper/GameUtils.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "AppController.h"
#import "PlayGameService.h"
#import "StoreService.h"
#import "AdManager.h"

bool CallToOtherLanguage::checkInternet()
{
    return [AppController checkInternet];
}

void CallToOtherLanguage::showAlert(const char* message)
{
    NSString* messageIOS = [NSString stringWithUTF8String:message];
    [AppController showAlert:messageIOS];
}

void CallToOtherLanguage::openAppInStore()
{
    [AppController openAppInStore];
}

void CallToOtherLanguage::initAds()
{
    AppController * app = (AppController * ) [[UIApplication sharedApplication] delegate];
    [app initAds];
}

void CallToOtherLanguage::loadAllAds()
{
    [AdManager call_LoadAllAds];
}

bool CallToOtherLanguage::isRewardedAdsAvailable()
{
    return [AdManager call_IsRewardedAdsAvailable];
}

void CallToOtherLanguage::fetchRewardedAds()
{
    [AdManager call_FetchRewardedAds];
}

bool CallToOtherLanguage::showRewardedAds(VideoAdsRequestSourceEnum source)
{
    return [AdManager call_ShowRewardedAds:(int) source];
}

void CallToOtherLanguage::showBannerAds(const std::string& name, int pos, float percentX, float percentY)
{
    //        [AdManager call_showBannerAds:[NSString stringWithUTF8String:name.c_str()] :pos :percentX :percentY];
}

void CallToOtherLanguage::hideBannerAds(const std::string& name)
{
    //        [AdManager hideBannerAds:[NSString stringWithUTF8String:name.c_str()]];
}

bool CallToOtherLanguage::canShowInterstitialAds()
{
    return [AdManager call_CanShowInterstitialAds];
}

void CallToOtherLanguage::loadInterstitialAds()
{
    [AdManager call_LoadInterstitialAds];
}

void CallToOtherLanguage::showInterstitialAds(bool callback, const std::string& nameFunction)
{
    [AdManager call_ShowInterstitialAds:callback :[NSString stringWithUTF8String:nameFunction.c_str()]];
}

void CallToOtherLanguage::submitLeaderboardMoney(double money)
{
    if (s_enableGameService)
        [PlayGameService submitLeaderBoardMoney:money];
}

void CallToOtherLanguage::submitLeaderboardLevel(double level)
{
    if (s_enableGameService)
        [PlayGameService submitLeaderBoardLevel:level];
}

void CallToOtherLanguage::showLeaderBoardBillionaireClub()
{
    if (s_enableGameService)
        [PlayGameService showLeaderBoardBillionaireClub];
}

void CallToOtherLanguage::showLeaderBoardMillionaireClub()
{
    if (s_enableGameService)
        [PlayGameService showLeaderBoardMillionaireClub];
}

void CallToOtherLanguage::showLeaderBoardLevel()
{
    if (s_enableGameService)
        [PlayGameService showLeaderBoardLevel];
}

void CallToOtherLanguage::signinPlayGameService()
{
    if (s_enableGameService)
        [PlayGameService signInPlayGameService];
}

void CallToOtherLanguage::signoutPlayGameService()
{
    if (s_enableGameService)
        [PlayGameService signoutPlayGameService];
}

bool CallToOtherLanguage::isPlayGameServiceSignedIn()
{
    if (s_enableGameService)
        return [PlayGameService isSignedIn];
    return false;
}

bool CallToOtherLanguage::havePlayGameService()
{
    if (s_enableGameService)
        return [PlayGameService havePlayGameService];
    return false;
}

void CallToOtherLanguage::openAnotherAppInStore(const std::string& packageName)
{
    [AppController openAnotherAppInStore:[NSString stringWithUTF8String:packageName.c_str()]];
}

void CallToOtherLanguage::openAnotherAppInStoreByLink(const std::string& url)
{
    [AppController openAnotherAppInStoreByLink:[NSString stringWithUTF8String:url.c_str()]];
}

std::string CallToOtherLanguage::getMyPackageName()
{
    return [[AppController getMyPackageName] UTF8String];
}

bool CallToOtherLanguage::isPackageInstalled(const std::string& packageName)
{
    return [AppController isPackageInstalled:[NSString stringWithUTF8String:packageName.c_str()]];
}

int CallToOtherLanguage::getVersionCode()
{
    return [AppController getVersionCode];
}

std::string CallToOtherLanguage::getVersionName()
{
    return [[AppController getVersionName] UTF8String];
}

void CallToOtherLanguage::scheduleLocalNotification(const std::string& data)
{
    [AppController scheduleLocalNotification:[NSString stringWithUTF8String:data.c_str()]];
}

void CallToOtherLanguage::cancelAllLocalNotification()
{
    [AppController cancelAllLocalNotification];
}

void CallToOtherLanguage::cleanAllLocalNotification()
{
    [AppController cleanAllLocalNotification];
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
    AppController* app = (AppController* )[[UIApplication sharedApplication] delegate];
    [app shareDataToMessenger:[NSString stringWithUTF8String:title.c_str()] :[NSString stringWithUTF8String:message.c_str()]];
}

void CallToOtherLanguage::forceToRegisterAudioFocusListenner()
{
}

bool CallToOtherLanguage::checkStartup()
{
    AppController* app = (AppController* )[[UIApplication sharedApplication] delegate];
    return [app checkStartup];
}

bool CallToOtherLanguage::hasSafeArea()
{
    return [AppController hasSafeArea];
}

void CallToOtherLanguage::showAppTrackingTransparency()
{
    [AppController showAppTrackingTransparency];
}

void CallToOtherLanguage::showInAppReview(int state, const std::string& condition)
{
    [StoreService showInAppReview:state :[NSString stringWithUTF8String:condition.c_str()]];
}

bool CallToOtherLanguage::willPlayBackgroundMusic()
{
    return [AppController willPlayBackgroundMusic];
}

void CallToOtherLanguage::copyToClipboard(const std::string& str)
{
    try
    {
        NSData  * data   = [NSData dataWithBytes:str.c_str() length:strlen(str.c_str())];
        NSString* string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        if (string != nil)
            [AppController copyToClipboard:string];
        else
            showAlert("Copy failed");
    } catch (NSException* e)
    {
    }
}

#endif

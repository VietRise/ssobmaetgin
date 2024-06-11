#ifndef __CALL_TO_OTHER_LANGUAGE_H__
#define __CALL_TO_OTHER_LANGUAGE_H__

#include "Define/GameDefine.h"

class CallToOtherLanguage : public cocos2d::Node
{
private:
    CallToOtherLanguage();
public:
    ~CallToOtherLanguage();
    static CallToOtherLanguage* getInstance();
    static void destroyInstance();

    bool checkInternet();
    void showAlert(const char* message);

    void openAppInStore();

    // Ads
    void loadAllAds();
    void initAds();
    bool isRewardedAdsAvailable();
    void fetchRewardedAds();
    bool showRewardedAds(VideoAdsRequestSourceEnum source);
    void showBannerAds(const std::string& name, int pos, float percentX, float percentY);
    void hideBannerAds(const std::string& name);
    void loadInterstitialAds();
    void showInterstitialAds(bool callback, const std::string& nameFunction);
    bool canShowInterstitialAds();

    void submitLeaderboardMoney(double money);
    void submitLeaderboardLevel(double level);
    void showLeaderBoardBillionaireClub();
    void showLeaderBoardMillionaireClub();
    void showLeaderBoardLevel();
    void signinPlayGameService();
    void signoutPlayGameService();
    bool isPlayGameServiceSignedIn();
    bool havePlayGameService();

    void copyToClipboard(const std::string& str);

    void scheduleLocalNotification(const std::string& data);
    void cancelAllLocalNotification();
    void cleanAllLocalNotification();

    std::string getMyPackageName();
    int getVersionCode();
    std::string getVersionName();
    std::string getURLAppStore();
    std::string getURLAppStoreTracking(const std::string& source, const std::string& campaign);
    bool isPackageInstalled(const std::string& packageName);
    void openAnotherAppInStore(const std::string& packageName);
    void openAnotherAppInStoreByLink(const std::string& url);

    void shareDataToMessenger(const std::string& title, const std::string& message);
    void forceToRegisterAudioFocusListenner();
    bool checkStartup();
    bool hasSafeArea();

    void showAppTrackingTransparency();
    void showInAppReview(int state, const std::string& condition);
    bool willPlayBackgroundMusic();
};

#endif // __CALL_TO_OTHER_LANGUAGE_H__

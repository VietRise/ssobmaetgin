#ifndef __ADS_MANAGER_H__
#define __ADS_MANAGER_H__

#include "cocos2d.h"
#include "Define/GameDefine.h"

class AdsManager : public cocos2d::Node
{
private:
    float       _timeCheckInterstitial;
    double      _showInterstitialTimestamp;
    std::string _nameCallBackInterstitial;
    bool        _callbackInterstitial;

    void scheduleFetchInterstitial(const float& dt);

    bool canShowInterstitialAds();
    void showInterstitialAds(bool callback, const std::string& function);
    void setCallbackInterstitial(bool callback, double time, const std::string& function);
public:
    ~AdsManager();
    AdsManager();

    virtual bool init() override;
    static AdsManager* getInstance();
    static void destroyInstance();
    virtual void update(float dt) override;

    void scheduleFetchVideo();

    void initAds();
    void loadAllAds();
    bool isRewardedAdsAvailable();
    void fetchRewardedAds();
    void showBannerAds(const std::string& name, int pos, float percentX, float percentY);
    void hideBannerAds(const std::string& name);
    void loadInterstitialAds();
    void showInterferingAds(bool show, bool callback, const std::string& nameFunction);
    void showRewardedAds(VideoAdsRequestSourceEnum source);
    void callbackInterstitial(const std::string& function, bool closedKey);

    CREATE_FUNC(AdsManager);
};

#endif // ! __ADS_MANAGER_H__

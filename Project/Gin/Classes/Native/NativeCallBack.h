#ifndef __NATIVE_CALLBACK_H__
#define __NATIVE_CALLBACK_H__

#include "Define/GameDefine.h"

class NativeCallBack
{
private:
    NativeCallBack();
public:
    ~NativeCallBack();
    static NativeCallBack* getInstance();
    static void destroyInstance();

    void watchVideoCompleted(VideoAdsRequestSourceEnum source);
    void interstitialHide(const std::string& nameFunction, bool adClosed);
    void onPlayGameServiceSignedIn();
    void onPlayGameServiceSignedOut();
    void completedAppReview();
};

#endif // __NATIVECALLBACK_H__
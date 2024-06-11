#ifndef NativeCallBackIOS_h
#define NativeCallBackIOS_h

#include "Define/GameDefine.h"

class NativeCallBackIOS
{
public:
    static void watchVideoCompleted(VideoAdsRequestSourceEnum source);
    static void interstitialHide(std::string nameFunction, bool adClosed);
    static void onPlayGameServiceSignedIn();
    static void onPlayGameServiceSignedOut();
    static void pauseGame();
    static void resumeGame();
    static void completedAppReview();
};

#endif /* NativeCallBackIOS_h */

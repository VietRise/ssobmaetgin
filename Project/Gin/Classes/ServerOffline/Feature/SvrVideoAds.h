#ifndef __SVR_VIDEO_ADS_H__
#define __SVR_VIDEO_ADS_H__

class SvrVideoAds
{
private:
    static void revewWatchVideoCount();
    static int getWatchedVideoCount();
protected:
public:
    static void initWatchVideos();
    static bool isReachVideoWatchPerDay();
    static void updateWatchVideoCount();
    static double getWatchVideoBonus();
    static double getNextWatchVideoBonus();
    static void resetWatchVideoBonus();
};

#endif //__SVR_VIDEO_ADS_H__

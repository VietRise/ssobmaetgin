#ifndef AdManager_h
#define AdManager_h

#import <Foundation/Foundation.h>
#import "AdmobMediation.h"

@interface AdManager : NSObject {
    int _priorityAdmobMediation;
    long _interstitialLastTime;
    long _interstitialGrapTime;
}

@property ( nonatomic , strong ) AdmobMediation * _admobMediation;

+ (id) shareInstance;
- (void) create: (RootViewController*) rootView;
- (void) loadAllAds;

- (void) loadBanner: (NSString*)name;
- (void) showBanner: (NSString*) name :(int)pos :(float)percentX :(float)percentY;
- (void) hideBanner: (NSString*) name;
- (int) getBannerBottomHeight;

- (void) loadInterstitial;
- (void) displayInterstitial:(BOOL)callback :(NSString*) name;

-(void) loadRewardedVideoAd;
-(void) displayRewaredVideoAd: (int)source;
-(BOOL) onRewardedViewAvailable;
-(BOOL) canShowInterstitialAds;

+(void) call_LoadAllAds;
+(void) call_ShowBannerAds: (NSString*) name :(int)pos :(float)percentX :(float)percentY;
+(void) call_HideBannerAds: (NSString*) name;
+(void) call_LoadInterstitialAds;
+(void) call_ShowInterstitialAds: (BOOL)callback :(NSString*) name;
+(void) call_FetchRewardedAds;
+(BOOL) call_ShowRewardedAds: (int) source;
+(BOOL) call_IsRewardedAdsAvailable;
+(BOOL) call_CanShowInterstitialAds;

// Callback to C++
- (void) callInterstitialHide:(BOOL)callback :(NSString*)name :(BOOL)adClosed;
- (void) callWatchVideoCompleted: (int)source;

@end
#endif /* AdManager_h */

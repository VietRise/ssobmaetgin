#import <Foundation/Foundation.h>
#import "AdManager.h"
#import "FirebaseRC.h"
#import "FirebaseAnalyticsController.h"
#import "NativeCallBackIOS.h"
#import "AppController.h"
#define TAG @"AdManager"
#define KEY_ADMANAGER_ENABLE_ADMOB_MEDIATION_WITH_PRIORITY      @"function_admanager_enable_admob_mediation_with_priority"
#define KEY_ADMANAGER_ENABLE_IRONSOURCE_MEDIATION_WITH_PRIORITY @"function_admanager_enable_ironsource_mediation_with_priority"
#define KEY_ADMANAGER_SHOW_INTERSTITIAL_TIME                    @"function_admanager_show_interstitial_time"
#define MAX_NUMBER_MEDIATION 3

@implementation AdManager
@synthesize _admobMediation;

static AdManager *instance = nil;

+ (id) shareInstance
{
    if (!instance) {
        instance = [[AdManager alloc] init];
        [instance retain];
    }
    return instance;
}

- (void) create: (RootViewController*) rootView;
{
    instance = self;
    _admobMediation = nil;
    _priorityAdmobMediation = 0;
    _interstitialLastTime = 0;
    _interstitialGrapTime = 0;
    
    _priorityAdmobMediation = [FirebaseRC getInteger:KEY_ADMANAGER_ENABLE_ADMOB_MEDIATION_WITH_PRIORITY :1];
    _interstitialGrapTime = [FirebaseRC getLong:KEY_ADMANAGER_SHOW_INTERSTITIAL_TIME :0];
    _interstitialLastTime = [[NSDate date] timeIntervalSince1970];
    if (_priorityAdmobMediation > 0)
    {
        _admobMediation = [[AdmobMediation alloc] init];
        [_admobMediation retain];
        [_admobMediation create: rootView];
    }
}

- (void) loadAllAds
{
    if (_admobMediation != nil)
    {
        [_admobMediation loadAllAds];
    }
}

// ------------------------- Banner -----------------------
- (void) loadBanner: (NSString*)name
{
    if (_admobMediation != nil)
    {
        [_admobMediation loadBanner:name];
    }
}

- (void) showBanner: (NSString*) name :(int)pos :(float)percentX :(float)percentY
{
    if (_admobMediation != nil)
    {
        [_admobMediation showBanner:name :pos :percentX :percentY];
    }
}

- (void) hideBanner: (NSString*) name
{
    if (_admobMediation != nil)
    {
        [_admobMediation hideBanner:name];
    }
}

- (int) getBannerBottomHeight
{
    if (_admobMediation != nil)
    {
        return [_admobMediation getBannerBottomHeight];
    }
    return 0;
}

// ------------------------ Interstitial -------------------------
- (void) loadInterstitial
{
    if (_admobMediation != nil)
    {
        [_admobMediation loadInterstitial];
    }
}

- (void) displayInterstitial:(BOOL)callback :(NSString*) name
{
    bool show = false;
    double currentTime = [[NSDate date] timeIntervalSince1970];
    if([self canShowInterstitialAds])
    {
        for (int i = 1; i <= MAX_NUMBER_MEDIATION; i++)
        {
            if (i == _priorityAdmobMediation) {
                if (_admobMediation != nil && [_admobMediation onInterstitialAvailable]) {
                    NSLog(@"%@ displayInterstitial Admob", TAG);
                    [FirebaseAnalyticsController logInterstitialAdsImpression:@"AdmobMediation"];
                    [_admobMediation displayInterstitial:callback: name];
                    show = true;
                    break;
                }
            }
        }
    }

    if (!show) {
        [self loadInterstitial];
        [[AdManager shareInstance] callInterstitialHide: callback : name : false];
    }
}

// ------------------------- Reward Video ---------------------------
-(void) loadRewardedVideoAd
{
    if (_admobMediation != nil) {
       [_admobMediation loadRewardedVideoAd];
    }
}

-(void) displayRewaredVideoAd: (int)source
{
    bool show = false;
    for (int i = 1; i <= MAX_NUMBER_MEDIATION; i++)
    {
        if (i == _priorityAdmobMediation) {
            if (_admobMediation != nil && [_admobMediation onRewardedViewAvailable]) {
                NSLog(@"%@ displayRewaredVideoAd Admob", TAG);
                [FirebaseAnalyticsController logVideoAdsImpression:@"AdmobMediation"];
                [_admobMediation displayRewaredVideoAd:source];
                show = true;
                break;
            }
        }
    }
    if (!show) {
        NSLog(@"%@ displayRewaredVideoAd NoVideoAds", TAG);
    }
}

- (BOOL) onRewardedViewAvailable
{
    if ((_admobMediation != nil && [_admobMediation onRewardedViewAvailable])) {
        return true;
    }
    return false;
}

- (BOOL) canShowInterstitialAds
{
    double currentTime = [[NSDate date] timeIntervalSince1970];
    if(currentTime - _interstitialLastTime >= _interstitialGrapTime || currentTime < _interstitialLastTime)
        return true;
    return false;
}

+(void) call_LoadAllAds
{
    if(instance != nil)
    {
        try {
            [instance loadAllAds];
        } catch (NSException* e) {
        }
    }
}

+(void) call_ShowBannerAds: (NSString*) name :(int)pos :(float)percentX :(float)percentY
{
    if(instance != nil)
    {
        try {
            [instance showBanner:name :pos :percentX :percentY];
        } catch (NSException* e) {
        }
    }
}

+(void) call_HideBannerAds: (NSString*) name
{
    if(instance != nil)
    {
        try {
            [instance hideBanner:name];
        } catch (NSException* e) {
        }
    }
}

+(void) call_LoadInterstitialAds
{
    if(instance != nil)
    {
        try {
            [instance loadInterstitial];
        } catch (NSException* e) {
        }
    }
}

+(void) call_ShowInterstitialAds: (BOOL)callback :(NSString*) name
{
    if(instance != nil)
    {
        try {
            [instance displayInterstitial:callback :name];
        } catch (NSException* e) {
        }
    }
}

+(void) call_FetchRewardedAds
{
    if(instance != nil)
    {
        try {
            [instance loadRewardedVideoAd];
        } catch (NSException* e) {
        }
    }
}

+(BOOL) call_ShowRewardedAds: (int) source
{
    if(instance != nil)
    {
        try {
            if ([instance onRewardedViewAvailable]) {
                [instance displayRewaredVideoAd:source];
                return true;
            }
        } catch (NSException* e) {
        }
        return false;
    }
    return false;
}

+(BOOL) call_IsRewardedAdsAvailable
{
    if(instance != nil)
    {
        try {
            return [instance onRewardedViewAvailable];
        } catch (NSException* e) {
        }
        return false;
    }
    return false;
}

+(BOOL) call_CanShowInterstitialAds
{
    if(instance != nil)
    {
        try {
            return [instance canShowInterstitialAds];
        } catch (NSException* e) {
        }
        return false;
    }
    return false;
}

// Callback to C++
- (void) callInterstitialHide:(BOOL)callback :(NSString*)name :(BOOL)adClosed
{
    if(adClosed)
        _interstitialLastTime = [[NSDate date] timeIntervalSince1970];
    if(callback)
        NativeCallBackIOS::interstitialHide(std::string([name UTF8String]), adClosed);
}

- (void) callWatchVideoCompleted: (int)source
{
    VideoAdsRequestSourceEnum sourceEnum = (VideoAdsRequestSourceEnum)source;
    NativeCallBackIOS::watchVideoCompleted(sourceEnum);
}

@end

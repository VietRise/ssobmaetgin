#import "AdmobMediation.h"
#import <Foundation/Foundation.h>
#import "FirebaseRC.h"
#import "AdManager.h"
#import "FirebaseAnalyticsController.h"
#import "AppController.h"
#import "AppDefine.h"
@import GoogleMobileAds;
@import FBAudienceNetwork;
@import GoogleMobileAdsMediationTestSuite;

@implementation AdmobMediation

@synthesize _bannerView;
@synthesize _interstitial;
@synthesize _rewardedAd;

#define NUMBER_MAX_FAIL_TO_LOAD_INTERSTITIAL 1
#define NUMBER_MAX_FAIL_TO_LOAD_REWARD_VIDEO 1
#define BANNER_DEFAULT_NAME @"banner_default"
#define BANNER_LARGE_NAME @"banner_large"

#define KEY_ADMOB_APP_ID @"admob_app_id"
#define KEY_ADMOB_BANNER_ID_DEFAULT @"admob_banner_id_default"
#define KEY_ADMOB_BANNER_ID_LARGE @"admob_banner_id_large"
#define KEY_ADMOB_INTERSTITIAL_ID @"admob_interstitial_id"
#define KEY_ADMOB_REWARD_VIDEO_ID @"admob_reward_video_id"
#define KEY_ADMOB_TEST @"admob_test"
#define KEY_ADMOB_ALLOW_LOG_EVENT @"admob_allow_log_event"
#define KEY_ADMOB_BANNER_BOTTOM_PADDING_BONUS @"admob_banner_bottom_padding_bonus"

-(void) create: (RootViewController*) rootView
{
    _enableShowBanner = false;
    _showingInterstitialName = @"";
    _callbackInterstitial = false;
    _countFailToLoadInterstitial = 0;
    _isLoadingInterstitial = false;
    _isLoadingRewardVideo = false;
    _countFailToLoadRewardVideo = 0;
    _requestSource = 0;
    _userDidEarnReward = false;
    _initSuccess = false;
    _admobTest = ADMOD_TEST;
    _admobId = ADMOB_UNIT_ID_IOS;
    _bannerIdDefault = @"";
    _bannerIdLarge = @"";
    _interstitalId = @"";
    _rewardedVideoId = @"";
    _shouldLoadAllAds = false;
    _enableLog = false;
    _bannerPaddingBonus = 0;
    
    
    _rootViewController = rootView;
    // Banner
    self._bannerView = nil;
    self._bannerViewLarge = nil;
    
    // Interstital
    self._interstitial = nil;
    
    // Rewarded video
    self._rewardedAd = nil;
    
    [[GADMobileAds sharedInstance] startWithCompletionHandler: ^(GADInitializationStatus *_Nonnull status)
     {
        NSLog(@"Admob onInitializationComplete");
        NSDictionary<NSString *, GADAdapterStatus *>* statusMap = [status adapterStatusesByClassName];
        for (NSString* adapterClass in statusMap.allKeys)
        {
            GADAdapterStatus* adapterStatus = [statusMap valueForKey: adapterClass];
            NSLog(@"Admob Adapter name: %@, Description: %@, Latency: %f", adapterClass, adapterStatus.description, adapterStatus.latency);
        }
        
       
        [self initialize];
        if (_shouldLoadAllAds) {
            _shouldLoadAllAds = false;
            [self loadAllAds];
        }
        
        if (_admobTest)
        {
//            GADMobileAds.sharedInstance.requestConfiguration.testDeviceIdentifiers = @[@"3c9ddd0ed6df0c29dd7f48f3e376b243"];
//             [GoogleMobileAdsMediationTestSuite presentOnViewController:_rootViewController delegate:nil];
        }
    }];
    // Set the flag as true.
    if (@available(iOS 14.0, *))
    {
        [FBAdSettings setAdvertiserTrackingEnabled:YES];
    }
}

-(void) initialize
{
    _admobTest = [FirebaseRC getBoolean: KEY_ADMOB_TEST: ADMOD_TEST];
    if (_admobTest)
    {
        _admobId = [[NSString alloc] initWithString:ADMOB_UNIT_ID_IOS];
        _bannerIdDefault = [[NSString alloc] initWithString:BANNER_DEFAULT_AD_UNIT_ID_IOS_TEST];
        _bannerIdLarge = [[NSString alloc] initWithString:BANNER_LARGE_AD_UNIT_ID_IOS_TEST];
        _interstitalId = [[NSString alloc] initWithString:INTERSTITIAL_AD_UNIT_ID_IOS_TEST];
        _rewardedVideoId = [[NSString alloc] initWithString:REWARDED_AD_UNIT_ID_IOS_TEST];
    }
    else
    {
        _admobId = [[NSString alloc] initWithString:[FirebaseRC getString:KEY_ADMOB_APP_ID :ADMOB_UNIT_ID_IOS]];
        _bannerIdDefault = [[NSString alloc] initWithString:[FirebaseRC getString:KEY_ADMOB_BANNER_ID_DEFAULT :BANNER_DEFAULT_AD_UNIT_ID_IOS]];
        _bannerIdLarge = [[NSString alloc] initWithString:[FirebaseRC getString:KEY_ADMOB_BANNER_ID_LARGE :BANNER_LARGE_AD_UNIT_ID_IOS]];
        _interstitalId = [[NSString alloc] initWithString:[FirebaseRC getString:KEY_ADMOB_INTERSTITIAL_ID :INTERSTITIAL_AD_UNIT_ID_IOS]];
        _rewardedVideoId = [[NSString alloc] initWithString:[FirebaseRC getString:KEY_ADMOB_REWARD_VIDEO_ID :REWARDED_AD_UNIT_ID_IOS]];
    }
    _enableLog = [FirebaseRC getBoolean:KEY_ADMOB_ALLOW_LOG_EVENT: false];
    _bannerPaddingBonus = [FirebaseRC getInteger:KEY_ADMOB_BANNER_BOTTOM_PADDING_BONUS :0];
    
    if (_enableShowBanner) {
        self._bannerView = [[GADBannerView alloc] initWithAdSize: GADAdSizeBanner];
        self._bannerView.translatesAutoresizingMaskIntoConstraints = NO;
        self._bannerView.adUnitID = _bannerIdDefault;
        self._bannerView.rootViewController = _rootViewController;
        self._bannerView.delegate = self;
        [_rootViewController.view addSubview: _bannerView];
//        [_rootViewController.view addConstraints:@[
//            [NSLayoutConstraint constraintWithItem:self._bannerView
//                                       attribute:NSLayoutAttributeBottom
//                                       relatedBy:NSLayoutRelationEqual
//                                          toItem:_rootViewController
//                                       attribute:NSLayoutAttributeTop
//                                      multiplier:1
//                                        constant:0],
//            [NSLayoutConstraint constraintWithItem:self._bannerView
//                                       attribute:NSLayoutAttributeCenterX
//                                       relatedBy:NSLayoutRelationEqual
//                                          toItem:_rootViewController
//                                       attribute:NSLayoutAttributeCenterX
//                                      multiplier:1
//                                        constant:0]
//                                        ]];
        
        self._bannerViewLarge = [[GADBannerView alloc] initWithAdSize: GADAdSizeLargeBanner];
        self._bannerViewLarge.translatesAutoresizingMaskIntoConstraints = NO;
        self._bannerViewLarge.adUnitID = _bannerIdLarge;
        self._bannerViewLarge.rootViewController = _rootViewController;
        self._bannerViewLarge.delegate = self;
        [_rootViewController.view addSubview: self._bannerViewLarge];
//        [_rootViewController.view addConstraints:@[
//            [NSLayoutConstraint constraintWithItem:self._bannerViewLarge
//                                       attribute:NSLayoutAttributeCenterY
//                                       relatedBy:NSLayoutRelationEqual
//                                          toItem:_rootViewController
//                                       attribute:NSLayoutAttributeCenterY
//                                      multiplier:1
//                                        constant:0],
//            [NSLayoutConstraint constraintWithItem:self._bannerViewLarge
//                                       attribute:NSLayoutAttributeCenterX
//                                       relatedBy:NSLayoutRelationEqual
//                                          toItem:_rootViewController
//                                       attribute:NSLayoutAttributeCenterX
//                                      multiplier:1
//                                        constant:0]
//                                        ]];
        
       
    }
    
    _initSuccess = true;
    
    [self hideBanner:BANNER_DEFAULT_NAME];
    [self hideBanner:BANNER_LARGE_NAME];
    
    if (_admobTest)
    {
        
    }
}

-(void) loadAllAds
{
    if (_initSuccess) {
        [self loadInterstitial];
        [self createAndLoadNewVideoAds];
       if (_enableShowBanner) {
           [self loadBanner: BANNER_DEFAULT_NAME];
           [self loadBanner: BANNER_LARGE_NAME];
       }
   } else {
       _shouldLoadAllAds = true;
   }
}

// Banner
-(void) loadBanner: (NSString*)name
{
    if (_initSuccess) {
        if (_enableShowBanner) {
            if ([name isEqualToString: BANNER_DEFAULT_NAME]) {
                if (self._bannerView != nil) {
                    [self._bannerView loadRequest: [GADRequest request]];
                }
            } else if ([name isEqualToString: BANNER_LARGE_NAME]) {
                if (self._bannerViewLarge != nil) {
                    [self._bannerViewLarge loadRequest: [GADRequest request]];
                }
            }
        }
    }
}

-(void) showBanner: (NSString*) name :(int)pos :(float)percentX :(float)percentY
{
    if (_initSuccess && _enableShowBanner) {
        if ([name isEqualToString: BANNER_DEFAULT_NAME])
        {
            if (self._bannerView != nil)
                self._bannerView.hidden = false;
        }
        else if ([name isEqualToString: BANNER_LARGE_NAME])
        {
            if (self._bannerViewLarge != nil)
                self._bannerViewLarge.hidden = false;
        }
    }
}

-(void) hideBanner: (NSString*) name
{
    if (_initSuccess && _enableShowBanner) {
        if ([name isEqualToString: BANNER_DEFAULT_NAME])
        {
            if (self._bannerView != nil)
                self._bannerView.hidden = true;
        }
        else if ([name isEqualToString: BANNER_LARGE_NAME])
        {
            if (self._bannerViewLarge != nil)
                self._bannerViewLarge.hidden = true;
        }
    }
}

-(int) getBannerBottomHeight
{
    return _bannerPaddingBonus;
}

// Interstitial
-(void) requestNewInterstitial
{
    _isLoadingInterstitial = true;
    // Request to download ads
    GADRequest* request = [GADRequest request];
    [GADInterstitialAd loadWithAdUnitID: _interstitalId
                                  request:request
                        completionHandler:^(GADInterstitialAd *ad, NSError *error) {
        _isLoadingInterstitial = false;
        if (error) {
            NSLog(@"Failed to load interstitial ad with error: %@", [error localizedDescription]);
            [self logLoadAdFail:@"interstitial": error];
            _countFailToLoadInterstitial++;
            if (_countFailToLoadInterstitial <= NUMBER_MAX_FAIL_TO_LOAD_INTERSTITIAL)
            {
                [self requestNewInterstitial];
            }
            return;
        }
        NSLog(@"[Admob] RewardedAd onAdLoaded");
        self._interstitial = ad;
        self._interstitial.fullScreenContentDelegate = self;
      }];
}

-(void) loadInterstitial
{
    if (_initSuccess) {
        if (!_isLoadingInterstitial && _interstitial == nil) {
            _countFailToLoadInterstitial = 0;
            [self requestNewInterstitial];
        }
    }
}

-(void) displayInterstitial: (BOOL)callback :(NSString*) name
{
    if (self._interstitial != nil) {
        
        _callbackInterstitial = callback;
        _showingInterstitialName = [[NSString alloc] initWithString:name];
        [self._interstitial presentFromRootViewController:_rootViewController];
    } else {
        [self loadInterstitial];
        [[AdManager shareInstance] callInterstitialHide: callback : name : false];
        [AppController callResumeGame];
    }
}

-(BOOL) onInterstitialAvailable
{
    if (self._interstitial != nil)
        return true;
    return false;
}

// Video
-(void) createAndLoadNewVideoAds
{
    if (_initSuccess)
    {
        _isLoadingRewardVideo = true;
        // Request to download ads
        GADRequest * request = [ GADRequest request ];
        [GADRewardedAd loadWithAdUnitID: _rewardedVideoId
                        request:request
              completionHandler:^(GADRewardedAd *ad, NSError *error) {
                _isLoadingRewardVideo = false;
                if (error) {
                    NSLog(@"[Admob] Rewarded ad failed to load with error: %@", [error localizedDescription]);
                    _countFailToLoadRewardVideo++;
                    [self logLoadAdFail:@"reward_ad": error];
                    if (_countFailToLoadRewardVideo <= NUMBER_MAX_FAIL_TO_LOAD_REWARD_VIDEO)
                    {
                        [self createAndLoadNewVideoAds];
                    }
                    return;
                }
                _countFailToLoadRewardVideo = 0;
                self._rewardedAd = ad;
                self._rewardedAd.fullScreenContentDelegate = self;
                NSLog(@"[Admob] Rewarded ad loaded.");
              }];
    }
}

-(void) loadRewardedVideoAd
{
    if (!_isLoadingRewardVideo && self._rewardedAd == nil)
    {
        _countFailToLoadRewardVideo = 0;
        [self createAndLoadNewVideoAds];
    }
}

-(void) displayRewaredVideoAd: (int) source
{
    if (self._rewardedAd != nil)
    {
        _requestSource = source;
        _userDidEarnReward = false;
        [self._rewardedAd presentFromRootViewController:_rootViewController
                               userDidEarnRewardHandler:^{
                                    NSLog(@"[Admob] userDidEarnReward");
                                    GADAdReward *reward = self._rewardedAd.adReward;
                                    _userDidEarnReward = true;
                               }];
    }
    else
    {
        [AppController callResumeGame];
    }
}

-(BOOL) onRewardedViewAvailable
{
    return (self._rewardedAd != nil);
}

// Log
-(void) logLoadAdFail:(NSString*)name :(NSError*)adError
{
    if (_enableLog) {
        int errorCode = adError.code;
        NSString* errorMsg = adError.localizedDescription;
        NSString* domain = adError.domain;
        NSString* cause = @"";
        NSError *underlyingError = adError.userInfo[NSUnderlyingErrorKey];
           if (underlyingError != nil) {
               cause = underlyingError.localizedDescription;
           }
        [FirebaseAnalyticsController logAdsLoadFail:name :errorCode :errorMsg :cause :domain];
   }
}

// --------------------- Delegate ----------------
/// --------------------------------- BANNER -----------------------------------
/// Tells the delegate that an ad request successfully received an ad. The delegate may want to add
/// the banner view to the view hierarchy if it hasn't been added yet.
- (void)bannerViewDidReceiveAd:(GADBannerView *)bannerView
{
    NSLog (@ "[Admob] adViewDidReceiveAd" );
}

/// Tells the delegate that an ad request failed. The failure is normally due to network
/// connectivity or ad availablility (i.e., no fill).
- (void)bannerView:(GADBannerView *)bannerView didFailToReceiveAdWithError:(NSError *)error {
NSLog(@"bannerView:didFailToReceiveAdWithError: %@", [error localizedDescription]);
}

- (void)bannerViewDidRecordImpression:(GADBannerView *)bannerView {
  NSLog(@"bannerViewDidRecordImpression");
}

/// Tells the delegate that a full screen view will be presented in response to the user clicking on
/// an ad. The delegate may want to pause animations and time sensitive interactions.
- (void)bannerViewWillPresentScreen:(GADBannerView *)bannerView {
  NSLog(@"bannerViewWillPresentScreen");
}

/// Tells the delegate that the full screen view will be dismissed.
- (void)bannerViewWillDismissScreen:(GADBannerView *)bannerView {
  NSLog(@"bannerViewWillDismissScreen");
}

/// Tells the delegate that the full screen view has been dismissed. The delegate should restart
/// anything paused while handling adViewWillPresentScreen:.
- (void)bannerViewDidDismissScreen:(GADBannerView *)bannerView {
  NSLog(@"bannerViewDidDismissScreen");
}

///-------------------------------- INTERSTITIAL & REWARDED VIDEO --------------------------------
///
/// /// Tells the delegate that the ad failed to present full screen content.
- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad
didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
    NSLog(@"[Admob] Ad did fail to present full screen content.");
    if (ad == self._interstitial)
    {
        [[AdManager shareInstance] callInterstitialHide: _callbackInterstitial : _showingInterstitialName : false];
        // Delay 1s to request load new interstital
        self._interstitial = nil;
        SEL aSelector = @selector(loadInterstitial);
        [self performSelector: aSelector withObject:nil afterDelay:1.0];
    }
    else if (ad == self._rewardedAd)
    {
        // Delay 1s to request load new video
        self._rewardedAd = nil;
        SEL aSelector = @selector(loadRewardedVideoAd);
        [self performSelector: aSelector withObject:nil afterDelay:1.0];
    }
    [AppController callResumeGame];
}

/// Tells the delegate that the ad presented full screen content.
/// /// Tells the delegate that the ad will present full screen content.
- (void)adWillPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    NSLog(@"[Admob] Ad did present full screen content.");
    [AppController callPauseGame];
}

/// Tells the delegate that the ad dismissed full screen content.
- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    NSLog(@"[Admob] Ad did dismiss full screen content.");
    if (ad == self._interstitial)
    {
        [[AdManager shareInstance] callInterstitialHide: _callbackInterstitial : _showingInterstitialName : true];
        self._interstitial = nil;
        // Delay 1s to request load new interstital
        SEL aSelector = @selector(loadInterstitial);
        [self performSelector: aSelector withObject:nil afterDelay:1.0];
    }
    else if (ad == self._rewardedAd)
    {
        if (_userDidEarnReward) {
            [[AdManager shareInstance] callWatchVideoCompleted:_requestSource];
            _requestSource = 0;
            _userDidEarnReward = false;
        }
        // Delay 1s to request load new video
        self._rewardedAd = nil;
        SEL aSelector = @selector(loadRewardedVideoAd);
        [self performSelector: aSelector withObject:nil afterDelay:1.0];
    }
    
    [AppController callResumeGame];
}
@end

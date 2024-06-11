#ifndef Admob_h
#define Admob_h

#import <Foundation/Foundation.h>
#import "RootViewController.h"
@import GoogleMobileAds;
@import UIKit;


@interface AdmobMediation: NSObject<GADBannerViewDelegate, GADFullScreenContentDelegate>{

    BOOL _enableShowBanner;
    
    NSString* _showingInterstitialName;
    BOOL _callbackInterstitial;
    BOOL _isLoadingInterstitial;
    int _countFailToLoadInterstitial;
    
    BOOL _isLoadingRewardVideo;
    int _countFailToLoadRewardVideo;
    int _requestSource;
    BOOL _userDidEarnReward;
    
    RootViewController* _rootViewController;
    
    BOOL _initSuccess;
    NSString* _admobId;
    NSString* _bannerIdDefault;
    NSString* _bannerIdLarge;
    NSString* _interstitalId;
    NSString* _rewardedVideoId;
    BOOL _shouldLoadAllAds;
    
    BOOL _enableLog;
    int _bannerPaddingBonus;
    BOOL _admobTest;
}

@property ( nonatomic , strong ) GADBannerView * _bannerView;
@property ( nonatomic , strong ) GADBannerView * _bannerViewLarge;
@property(nonatomic, strong) GADInterstitialAd* _interstitial;
@property ( nonatomic , strong ) GADRewardedAd * _rewardedAd; 

-(void) create: (RootViewController*) rootView;
-(void) initialize;
-(void) loadAllAds;
// Banner
-(void) loadBanner: (NSString*)name;
-(void) showBanner: (NSString*)name :(int)pos :(float)percentX :(float)percentY;
-(void) hideBanner: (NSString*)name;
-(int) getBannerBottomHeight;

// Interstitial
-(void) requestNewInterstitial;
-(void) loadInterstitial;
-(void) displayInterstitial: (BOOL)callback :(NSString*) name;
-(BOOL) onInterstitialAvailable;

// Video
-(void) createAndLoadNewVideoAds;
-(void) loadRewardedVideoAd;
-(void) displayRewaredVideoAd: (int) source;
-(BOOL) onRewardedViewAvailable;

// Log
-(void) logLoadAdFail:(NSString*)name :(NSError*)adError;

@end

#endif

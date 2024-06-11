#import "StoreService.h"
#include "NativeCallBackIOS.h"
#import "FirebaseAnalyticsController.h"
#import <StoreKit/StoreKit.h>

#define TAG @"StoreService"
static StoreService *instance = nil;
static int _ratingState = 0;
static NSString* _ratingCondition = @"";

@implementation StoreService

+ (id) shareInstance
{
    if (!instance) {
        instance = [[StoreService alloc] init];
    }
    return instance;
}

- (void) create: (RootViewController*) rootView
{
    instance = self;
    _rootViewController = rootView;
    _ratingState = 0;
    _ratingCondition = @"";
}

- (void) displayInAppReview
{
    try
    {
        if (@available(iOS 14.0, *))
        {
            [FirebaseAnalyticsController logShowInAppReview :_ratingState : _ratingCondition];
            [SKStoreReviewController requestReviewInScene:_rootViewController.view.window.windowScene];
        }
        else
        {
        }
    }
    catch(NSException* ex)
    {
    }
}

+ (void) showInAppReview:(int)state:(NSString*)condition
{
    _ratingState = state;
    _ratingCondition = condition;
    if (instance != nil)
    {
        [instance displayInAppReview];
    }
    else
    {
    }
}

@end


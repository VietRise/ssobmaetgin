#import "PlayGameService.h"
#import "FirebaseRC.h"
#import "NativeCallBackIOS.h"
#import "cocos2d.h"
#import "AppDefine.h"
@import GameKit;

@implementation PlayGameService

#define TYPE_LEADERBOARD_BILLIONAIRE 1
#define TYPE_LEADERBOARD_MILLIONAIRE 2
#define TYPE_LEADERBOARD_LEVEL 3

#define USERDEFAULT_GCENTER_SIGNED_IN @"gcenter_signedin"

static PlayGameService *sharedInstance = nil;

+ (id) shareInstance
{
    if (!sharedInstance) {
        sharedInstance = [[PlayGameService alloc] init];
    }
    return sharedInstance;
}

- (void) create: (RootViewController*) rootView
{
    _rootViewController = rootView;
    // Auto sign in
    // [self signInSilently];
}

- (void) signIn
{
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    localPlayer.authenticateHandler = ^(UIViewController *viewController, NSError *error){
        if (error != nil) {
            NSLog(@"Authenticate Fail: %@", error);
        } else {
            if (viewController != nil)
            {
                //showAuthenticationDialogWhenReasonable: is an example method name. Create your own method that displays an authentication view when appropriate for your app.
                [_rootViewController presentViewController:viewController animated:YES completion: nil];
            }
            else if (localPlayer.isAuthenticated)
            {
                //authenticatedPlayer: is an example method name. Create your own method that is called after the local player is authenticated.
                // [self authenticatedPlayer: localPlayer];
                [self onPlayGameServiceSignedIn: localPlayer];
                
            }
            else
            {
                // [self disableGameCenter];
                [self onPlayGameServiceSignedOut];
            }
        }
    };
}

- (void) signInSilently
{
    if (cocos2d::UserDefault::getInstance()->getBoolForKey([USERDEFAULT_GCENTER_SIGNED_IN UTF8String], false))
    {
        [self signIn];
    }
}

- (void) signOut
{
    
}

- (void) submitMoney: (double) money
{
    @try {
        if ([self isSigned])
        {
            if (@available(iOS 14.0, *))
            {
                [GKLeaderboard loadLeaderboardsWithIDs: [NSArray arrayWithObjects:[PlayGameService getLeaderboardId:TYPE_LEADERBOARD_BILLIONAIRE], [PlayGameService getLeaderboardId:TYPE_LEADERBOARD_MILLIONAIRE], nil] completionHandler:^(NSArray<GKLeaderboard *> * _Nullable leaderboards, NSError * _Nullable error) {
                    if (error == nil) {
                        NSLog(@"PlayGameService loadLeaderboardsWithIDs:Money successfully");
                        GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
                        for (int i = 0; i < leaderboards.count; i++) {
                            GKLeaderboard* leaderboard = leaderboards[i];
                            [leaderboard submitScore:money context:0 player:localPlayer completionHandler:^(NSError * _Nullable error1) {
                                if (error1 == nil) {
                                    NSLog(@"PlayGameService submitScore %f successfully", money);
                                }
                            }];
                        }
                    }
                }];
            }
            else
            {
                GKScore* scoreReporter1 = [[GKScore alloc] initWithLeaderboardIdentifier: [PlayGameService getLeaderboardId:TYPE_LEADERBOARD_BILLIONAIRE]];
                scoreReporter1.value = money;
                scoreReporter1.context = 0;
                
                GKScore* scoreReporter2 = [[GKScore alloc] initWithLeaderboardIdentifier: [PlayGameService getLeaderboardId:TYPE_LEADERBOARD_MILLIONAIRE]];
                scoreReporter2.value = money;
                scoreReporter2.context = 0;
                
                NSArray *scores = @[scoreReporter1, scoreReporter2];
                [GKScore reportScores:scores withCompletionHandler:^(NSError * _Nullable error) {
                    if (error == nil) {
                        NSLog(@"PlayGameService submitScore %f successfully", money);
                    }
                }];
            }
        }
        
    } @catch (NSException *exception) {
        
    } @finally {
        
    }
}

- (void) submitLevel: (double) level
{
    @try {
        if ([self isSigned]) {
            if (@available(iOS 14.0, *))
            {
                [GKLeaderboard loadLeaderboardsWithIDs: [NSArray arrayWithObjects:[PlayGameService getLeaderboardId:TYPE_LEADERBOARD_LEVEL], nil] completionHandler:^(NSArray<GKLeaderboard *> * _Nullable leaderboards, NSError * _Nullable error) {
                    if (error == nil) {
                        NSLog(@"PlayGameService loadLeaderboardsWithIDs:Level successfully");
                        GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
                        for (int i = 0; i < leaderboards.count; i++) {
                            GKLeaderboard* leaderboard = leaderboards[i];
                            [leaderboard submitScore:level context:0 player:localPlayer completionHandler:^(NSError * _Nullable error1) {
                                if (error1 == nil) {
                                    NSLog(@"PlayGameService submitLevel %f successfully", level);
                                }
                            }];
                        }
                    }
                }];
            }
            else
            {
                GKScore* scoreReporter = [[GKScore alloc] initWithLeaderboardIdentifier: [PlayGameService getLeaderboardId:TYPE_LEADERBOARD_LEVEL]];
                scoreReporter.value = level;
                scoreReporter.context = 0;
                
                NSArray *scores = @[scoreReporter];
                [GKScore reportScores:scores withCompletionHandler:^(NSError * _Nullable error) {
                    if (error == nil) {
                        NSLog(@"PlayGameService submitLevel %f successfully", level);
                    }
                }];
            }
        }
        
    } @catch (NSException *exception) {
        
    } @finally {
        
    }
}

- (bool) isSigned
{
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    return [localPlayer isAuthenticated];
}

- (bool) isAvailable
{
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    return ![localPlayer isUnderage];
}

- (void) displayLeaderBoard: (int) type
{
    @try {
        if ([self isSigned]){
            NSString* leaderboardID = [PlayGameService getLeaderboardId:type];
            GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
            if (gameCenterController != nil)
            {
               gameCenterController.gameCenterDelegate = self;
               gameCenterController.viewState = GKGameCenterViewControllerStateLeaderboards;
               gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
               gameCenterController.leaderboardIdentifier = leaderboardID;
               [_rootViewController presentViewController: gameCenterController animated: YES completion:nil];
            }
        } else {
            [self signIn];
        }
    } @catch (NSException *exception) {
        NSLog(@"displayLeaderBoard exception: %@", exception);
    } @finally {
        
    }
    
}

- (void) onPlayGameServiceSignedIn: (GKLocalPlayer*) player
{
    cocos2d::UserDefault::getInstance()->setBoolForKey(USERDEFAULT_GCENTER_SIGNED_IN.UTF8String, true);
    NativeCallBackIOS::onPlayGameServiceSignedIn();
}

- (void) onPlayGameServiceSignedOut
{
    cocos2d::UserDefault::getInstance()->setBoolForKey(USERDEFAULT_GCENTER_SIGNED_IN.UTF8String, false);
    NativeCallBackIOS::onPlayGameServiceSignedOut();
}

+ (NSString*) getLeaderboardId: (int) type
{
    NSString* leaderBoardId = @"";
    @try {
        if (type == TYPE_LEADERBOARD_BILLIONAIRE) {
            leaderBoardId = [FirebaseRC getString: @"leaderboard_billionaire_id" : KEY_LEADERBOARD_BILLIONAIRE_ID];
        } else if (type == TYPE_LEADERBOARD_MILLIONAIRE) {
            leaderBoardId = [FirebaseRC getString: @"leaderboard_millionaire_id" : KEY_LEADERBOARD_MILLIONAIRE_ID];
        } else if (type == TYPE_LEADERBOARD_LEVEL) {
            leaderBoardId = [FirebaseRC getString: @"leaderboard_level_id" : KEY_LEADERBOARD_LEVEL_ID];
        }
    } @catch (NSException *exception) {
        NSLog(@"getLeaderboardId exception: %@", exception);
    } @finally {
        
    }
    return leaderBoardId;
}

+ (void) submitLeaderBoardMoney: (double) money
{
    [[PlayGameService shareInstance] submitMoney: money];
}

+ (void) submitLeaderBoardLevel: (double) level
{
    [[PlayGameService shareInstance] submitLevel: level];
}

+ (void) signInPlayGameService
{
    [[PlayGameService shareInstance] signIn];
}

+ (void) signoutPlayGameService
{
    [[PlayGameService shareInstance] signOut];
}

+ (void) showLeaderBoardBillionaireClub
{
    [[PlayGameService shareInstance] displayLeaderBoard: TYPE_LEADERBOARD_BILLIONAIRE];
}

+ (void) showLeaderBoardMillionaireClub
{
    [[PlayGameService shareInstance] displayLeaderBoard: TYPE_LEADERBOARD_MILLIONAIRE];
}

+ (void) showLeaderBoardLevel
{
    [[PlayGameService shareInstance] displayLeaderBoard: TYPE_LEADERBOARD_LEVEL];
}

+ (bool) isSignedIn
{
    return [[PlayGameService shareInstance] isSigned];
}

+ (bool) havePlayGameService
{
    return [[PlayGameService shareInstance] isAvailable];
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController NS_AVAILABLE(10_9, 6_0)
{
    [_rootViewController dismissViewControllerAnimated:YES completion:nil];
}

@end

#ifndef PlayGameService_h
#define PlayGameService_h

#import <Foundation/Foundation.h>
#import "RootViewController.h"
@import GameKit;

@interface PlayGameService : NSObject<GKGameCenterControllerDelegate> {
    
}

@property(nonatomic, readonly) RootViewController* rootViewController;

+ (id) shareInstance;
- (void) create: (RootViewController*) rootView;
- (void) signIn;
- (void) signInSilently;
- (void) signOut;
- (void) submitMoney: (double) money;
- (void) submitLevel: (double) level;
- (bool) isSigned;
- (bool) isAvailable;
- (void) displayLeaderBoard: (int) type;
- (void) onPlayGameServiceSignedIn: (GKLocalPlayer*) player;
- (void) onPlayGameServiceSignedOut;
+ (NSString*) getLeaderboardId: (int) type;
+ (void) submitLeaderBoardMoney: (double) money;
+ (void) submitLeaderBoardLevel: (double) level;
+ (void) signInPlayGameService;
+ (void) signoutPlayGameService;
+ (void) showLeaderBoardBillionaireClub;
+ (void) showLeaderBoardMillionaireClub;
+ (void) showLeaderBoardLevel;
+ (bool) isSignedIn;
+ (bool) havePlayGameService;

@end

#endif /* Header_h */

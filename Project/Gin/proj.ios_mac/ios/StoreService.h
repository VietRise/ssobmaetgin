#ifndef StoreService_h
#define StoreService_h

#import <Foundation/Foundation.h>
#import "RootViewController.h"

@interface StoreService : NSObject {
}

@property(nonatomic, readonly) RootViewController* rootViewController;

+ (id) shareInstance;
- (void) create: (RootViewController*) rootView;
- (void) displayInAppReview;
+ (void) showInAppReview:(int)state:(NSString*)condition;

@end

#endif /* StoreService_h */

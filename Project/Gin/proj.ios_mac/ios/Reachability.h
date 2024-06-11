#ifndef __Reachability__
#define __Reachability__

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
typedef enum {
    NotReachable = 0,
    ReachableViaWiFi,
    ReachableViaWWAN
} NetworkStatus;
#define kReachabilityChangedNotification @"kNetworkReachabilityChangedNotification"

@interface Reachability: NSObject
{
    SCNetworkReachabilityRef reachabilityRef;
}

//reachabilityWithAddress- Use to check the reachability of a particular IP address.
+ (Reachability*) reachabilityWithAddress: (const struct sockaddr_in*) hostAddress;

//reachabilityForInternetConnection- checks whether the default route is available.
//  Should be used by applications that do not connect to a particular host
+ (Reachability*) reachabilityForInternetConnection;

//Start listening for reachability notifications on the current run loop
- (BOOL) startNotifier;
- (void) stopNotifier;

- (NetworkStatus) currentReachabilityStatus;
@end

#endif /* defined(__BanCaOnline__Reachability__) */

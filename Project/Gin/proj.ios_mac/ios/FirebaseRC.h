#ifndef FirebaseRC_h
#define FirebaseRC_h

#import <Foundation/Foundation.h>
@import Firebase;

@interface FirebaseRC : NSObject {
    BOOL _fetchCompleted;
}
@property ( nonatomic , strong ) FIRRemoteConfig * _remoteConfig;

- (void) create;
- (void) fetchConfig;
- (BOOL) isFetchCompleted;
+ (id) sharedObject;
+ (void) setDefault: (NSString*) key: (NSObject*) value;
+ (NSString*) getString: (NSString*) key: (NSString*) defaultValue;
+ (long) getLong: (NSString*) key: (long) defaultValue;
+ (int) getInteger: (NSString*) key: (int) defaultValue;
+ (double) getDouble: (NSString*) key: (double) defaultValue;
+ (float) getFloat: (NSString*) key: (float) defaultValue;
+ (bool) getBoolean: (NSString*) key: (bool) defaultValue;

@end

#endif /* FirebaseRC_h */

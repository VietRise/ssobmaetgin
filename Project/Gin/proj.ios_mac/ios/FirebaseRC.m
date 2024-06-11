#import "FirebaseRC.h"

static FirebaseRC *instance = nil;

@implementation FirebaseRC
@synthesize _remoteConfig;

+ (id) sharedObject
{
    if (instance == nil) {
        instance = [[FirebaseRC alloc] init];
        [instance create];
    }
    return instance;
}

- (void) create
{
    instance = self;
    _fetchCompleted = false;
    
    self._remoteConfig = [FIRRemoteConfig remoteConfig];
    FIRRemoteConfigSettings *remoteConfigSettings = [[FIRRemoteConfigSettings alloc] init];
    remoteConfigSettings.minimumFetchInterval = 10;
    self._remoteConfig.configSettings = remoteConfigSettings;
    // [self._remoteConfig setDefaultsFromPlistFileName:@"RemoteConfigDefaults"];
    [self fetchConfig];
}

- (void) fetchConfig
{
    _fetchCompleted = false;
    [self._remoteConfig fetchWithCompletionHandler:^(FIRRemoteConfigFetchStatus status, NSError *error) {
        if (status == FIRRemoteConfigFetchStatusSuccess) {
            NSLog(@"[FirebaseRC] Config fetched!");
            [self._remoteConfig activateWithCompletion:^(BOOL changed, NSError * _Nullable error) {
                if (changed) {
                    NSLog(@"[FirebaseRC] Config changed!");
                } else {
                    NSLog(@"[FirebaseRC] Config not changed!");
                }
                _fetchCompleted = true;
            }];
        } else {
            _fetchCompleted = true;
            NSLog(@"[FirebaseRC] Config not fetched");
            NSLog(@"[FirebaseRC] Error %@", error.localizedDescription);
        }
    }];
}

- (BOOL) isFetchCompleted
{
    return _fetchCompleted;
}

+ (void) setDefault: (NSString*) key: (NSObject*) value
{
    if (instance != nil && instance._remoteConfig != nil) {
        NSDictionary* mapDefaultValue = [NSDictionary dictionaryWithObject:value forKey:key];
        [instance._remoteConfig setDefaults:mapDefaultValue];
    }
}

+ (NSString*) getString: (NSString*) key: (NSString*) defaultValue
{
    if (instance != nil && instance._remoteConfig != nil) {
        [FirebaseRC setDefault:key :defaultValue];
        FIRRemoteConfigValue* value = [instance._remoteConfig configValueForKey:key];
        if (value) {
            return [value stringValue];
        }
    }
    return  defaultValue;
}

+ (long) getLong: (NSString*) key: (long) defaultValue
{
    if (instance != nil && instance._remoteConfig != nil) {
        [FirebaseRC setDefault:key : [NSNumber numberWithLong:defaultValue]];
        FIRRemoteConfigValue* value = [instance._remoteConfig configValueForKey:key];
        if (value) {
            return [[value numberValue] longValue];
        }
    }
    return  defaultValue;
}

+ (int) getInteger: (NSString*) key: (int) defaultValue
{
    if (instance != nil && instance._remoteConfig != nil) {
        [FirebaseRC setDefault:key : [NSNumber numberWithInt:defaultValue]];
        FIRRemoteConfigValue* value = [instance._remoteConfig configValueForKey:key];
        if (value) {
            return [[value numberValue] intValue];
        }
    }
    return  defaultValue;
}

+ (double) getDouble: (NSString*) key: (double) defaultValue
{
    if (instance != nil && instance._remoteConfig != nil) {
        [FirebaseRC setDefault:key :[NSNumber numberWithDouble:defaultValue]];
        FIRRemoteConfigValue* value = [instance._remoteConfig configValueForKey:key];
        if (value) {
            return [[value numberValue] doubleValue];
        }
    }
    return  defaultValue;
}

+ (float) getFloat: (NSString*) key: (float) defaultValue
{
    if (instance != nil && instance._remoteConfig != nil) {
        [FirebaseRC setDefault:key :[NSNumber numberWithFloat:defaultValue]];
        FIRRemoteConfigValue* value = [instance._remoteConfig configValueForKey:key];
        if (value) {
            return [[value numberValue] floatValue];
        }
    }
    return  defaultValue;
}

+ (bool) getBoolean: (NSString*) key: (bool) defaultValue
{
    if (instance != nil && instance._remoteConfig != nil) {
        [FirebaseRC setDefault:key : defaultValue ? [NSString stringWithUTF8String: "True"] : [NSString stringWithUTF8String: "False"]];
        FIRRemoteConfigValue* value = [instance._remoteConfig configValueForKey:key];
        if (value) {
            return [value boolValue];
        }
    }
    return  defaultValue;
}

@end

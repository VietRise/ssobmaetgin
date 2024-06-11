package org.cocos2dx.cpp;

import com.google.firebase.crashlytics.FirebaseCrashlytics;

public class FirebaseCrashlyticsController {
    // Record non-fatal exception
    public static void recordException(Exception e) {
        FirebaseCrashlytics.getInstance().recordException(e);
    }

    // Record custom key
    public static void recordCustomKey(String key, String value) {
        FirebaseCrashlytics.getInstance().setCustomKey(key, value);
    }

    // Log
    public static void log(String message) {
        FirebaseCrashlytics.getInstance().log(message);
    }
}

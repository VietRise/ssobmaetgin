package org.cocos2dx.cpp;

import android.util.Log;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.remoteconfig.FirebaseRemoteConfig;
import com.google.firebase.remoteconfig.FirebaseRemoteConfigSettings;
import com.google.firebase.remoteconfig.FirebaseRemoteConfigValue;

import androidx.annotation.NonNull;

public class FirebaseRC {

    public static final long CACHE_EXPIRATION = 1;
    public static FirebaseRC instance = null;
    private AppActivity _activity;
    private FirebaseRemoteConfig _remoteConfig;
    private boolean _fetchCompleted = false;

    public FirebaseRC() {
        instance = this;
        _activity = null;
        _remoteConfig = null;
    }

    public void create(AppActivity activity) {
        this._activity = activity;
        this._remoteConfig = FirebaseRemoteConfig.getInstance();
        this._remoteConfig.setConfigSettingsAsync(new FirebaseRemoteConfigSettings.Builder()
                .setMinimumFetchIntervalInSeconds(CACHE_EXPIRATION)
                .build());
        this.fetchConfig();
    }

    public void fetchConfig() {
        _fetchCompleted = false;
        try {
            this._remoteConfig.fetchAndActivate()
                    .addOnCompleteListener(_activity, new OnCompleteListener<Boolean>() {
                        @Override
                        public void onComplete(@NonNull Task<Boolean> task) {
                            if (task.isSuccessful()) {
                                boolean updated = task.getResult();
                                Log.d("RC", "Config params updated: " + updated);
                            } else {
                                Log.d("RC", "Fetch failed");
                            }
                            _fetchCompleted = true;
                        }
                    });
        } catch (Exception ex) {
            _fetchCompleted = true;
        }
    }

    public static void fetch() {
        FirebaseRC.instance.fetchConfig();
    }

    public static void setDefault(String key, Object defaultValue) {
//        try {
//            if (FirebaseRC.instance != null && FirebaseRC.instance._remoteConfig != null) {
//                HashMap mapDefaultValue = new HashMap();
//                mapDefaultValue.put(key, defaultValue);
//                FirebaseRC.instance._remoteConfig.setDefaultsAsync(mapDefaultValue);
//            }
//        } catch (Exception ex) {
//        }
    }

    private static boolean isValidValue(FirebaseRemoteConfigValue rawValue) {
        if (rawValue != null) {
            if (rawValue.getSource() == FirebaseRemoteConfig.VALUE_SOURCE_REMOTE) //2: value from remote server
                return true;
            else if (rawValue.getSource() == FirebaseRemoteConfig.VALUE_SOURCE_DEFAULT) //1: value from default set by user
                return false;
            else if (rawValue.getSource() == FirebaseRemoteConfig.VALUE_SOURCE_STATIC) //0: value returned is default (FRC don't have the key)
                return false;
        }
        return false;
    }

    public static String getString(String key, String defaultValue) {
        try {

            FirebaseRC.setDefault(key, defaultValue);
            if (FirebaseRC.instance != null && FirebaseRC.instance._remoteConfig != null) {
                FirebaseRemoteConfigValue rawValue = FirebaseRC.instance._remoteConfig.getValue(key);
                if (FirebaseRC.isValidValue(rawValue)) {
                    return rawValue.asString();
                }
            }
        } catch (Exception ex) {
        }
        return defaultValue;
    }

    public static long getLong(String key, long defaultValue) {
        try {
            FirebaseRC.setDefault(key, defaultValue);
            if (FirebaseRC.instance != null && FirebaseRC.instance._remoteConfig != null) {
                FirebaseRemoteConfigValue rawValue = FirebaseRC.instance._remoteConfig.getValue(key);
                if (FirebaseRC.isValidValue(rawValue)) {
                    return rawValue.asLong();
                }
            }
        } catch (Exception ex) {
        }
        return defaultValue;
    }

    public static int getInteger(String key, int defaultValue) {
        try {
            FirebaseRC.setDefault(key, defaultValue);
            if (FirebaseRC.instance != null && FirebaseRC.instance._remoteConfig != null) {
                FirebaseRemoteConfigValue rawValue = FirebaseRC.instance._remoteConfig.getValue(key);
                if (FirebaseRC.isValidValue(rawValue)) {
                    return (int) rawValue.asLong();
                }
            }
        } catch (Exception ex) {
        }
        return defaultValue;
    }

    public static double getDouble(String key, double defaultValue) {
        try {
            FirebaseRC.setDefault(key, defaultValue);
            if (FirebaseRC.instance != null && FirebaseRC.instance._remoteConfig != null) {
                FirebaseRemoteConfigValue rawValue = FirebaseRC.instance._remoteConfig.getValue(key);
                if (FirebaseRC.isValidValue(rawValue)) {
                    return rawValue.asDouble();
                }
            }
        } catch (Exception ex) {
        }
        return defaultValue;
    }

    public static float getFloat(String key, float defaultValue) {
        try {
            FirebaseRC.setDefault(key, defaultValue);
            if (FirebaseRC.instance != null && FirebaseRC.instance._remoteConfig != null) {
                FirebaseRemoteConfigValue rawValue = FirebaseRC.instance._remoteConfig.getValue(key);
                if (FirebaseRC.isValidValue(rawValue)) {
                    return (float) rawValue.asDouble();
                }
            }
        } catch (Exception ex) {
        }
        return defaultValue;
    }

    public static boolean getBoolean(String key, boolean defaultValue) {
        try {
            FirebaseRC.setDefault(key, defaultValue);
            if (FirebaseRC.instance != null && FirebaseRC.instance._remoteConfig != null) {
                FirebaseRemoteConfigValue rawValue = FirebaseRC.instance._remoteConfig.getValue(key);
                if (FirebaseRC.isValidValue(rawValue)) {
                    return rawValue.asBoolean();
                }
            }
        } catch (Exception ex) {
        }
        return defaultValue;
    }

    public boolean isFetchCompleted() {
        return _fetchCompleted;
    }
}

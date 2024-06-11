package org.cocos2dx.cpp;

import java.util.Locale;

public class DeviceInfo {

    public static String getDeviceLanguage() {
        try {
            return Locale.getDefault().getLanguage();
        } catch (Exception er) {
        }
        return "";
    }

    public static String getDeviceLanguageCountry() {
        try {
            return Locale.getDefault().getCountry();
        } catch (Exception er) {
        }
        return "";
    }
}

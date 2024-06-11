package org.cocos2dx.cpp.Ads;

import android.app.Activity;
import android.util.Log;
import org.cocos2dx.cpp.FirebaseAnalyticsController;
import org.cocos2dx.cpp.FirebaseRC;
import org.cocos2dx.lib.Cocos2dxHelper;

public class AdManager {

    public static final String TAG = "AdManager";
    public static final String KEY_ADMANAGER_ENABLE_ADMOB_MEDIATION_WITH_PRIORITY = "function_admanager_enable_admob_mediation_with_priority";
    public static final String KEY_ADMANAGER_ENABLE_IRONSOURCE_MEDIATION_WITH_PRIORITY = "function_admanager_enable_ironsource_mediation_with_priority";
    public static final String KEY_ADMANAGER_SHOW_INTERSTITIAL_TIME = "function_admanager_show_interstitial_time";
    public static final int MAX_NUMBER_MEDIATION = 3;

    private Activity _activity;
    private AdmobMediation _admobMediation;

    private int _priorityAdmobMediation;
    private int _priorityIronSourceMediation;
    private long _interstitialLastTime = 0;
    private long _interstitialGrapTime = 0;

    public static AdManager _instance = null;

    public static native void interstitialHide(String nameFunction, boolean adClosed);

    public static native void watchVideoCompleted(int source);

    public AdManager() {
        _instance = this;
        _admobMediation = null;
        _priorityAdmobMediation = 0;
        _priorityIronSourceMediation = 0;
    }

    public void create(Activity activity) {
        _activity = activity;
        _priorityAdmobMediation = FirebaseRC.getInteger(KEY_ADMANAGER_ENABLE_ADMOB_MEDIATION_WITH_PRIORITY, 1);
        _priorityIronSourceMediation = FirebaseRC.getInteger(KEY_ADMANAGER_ENABLE_IRONSOURCE_MEDIATION_WITH_PRIORITY, 2);
        _interstitialGrapTime = FirebaseRC.getLong(KEY_ADMANAGER_SHOW_INTERSTITIAL_TIME, 0);
        if (_priorityAdmobMediation > 0) {
            _admobMediation = new AdmobMediation();
            _admobMediation.create(activity);
        }
        _interstitialLastTime = System.currentTimeMillis() / 1000;
    }

    public void loadAllAds() {
        if (_admobMediation != null) {
            _admobMediation.loadAllAds();
        }
    }

    // ------------------------- Banner -----------------------
    public void loadBanner(String name) {
        if (_admobMediation != null) {
            _admobMediation.loadBanner(name);
        }
    }

    public void showBanner(String name, int pos, float percentX, float percentY) {
        if (_admobMediation != null) {
            _admobMediation.showBanner(name, pos, percentX, percentY);
        }
    }

    public void hideBanner(String name) {
        if (_admobMediation != null) {
            _admobMediation.hideBanner(name);
        }
    }

    public int getBannerBottomHeight() {
        if (_admobMediation != null) {
            return _admobMediation.getBannerBottomHeight();
        }
        return 0;
    }

    // ------------------------ Interstitial -------------------------
    public void loadInterstitial() {
        if (_admobMediation != null) {
            _admobMediation.loadInterstitial();
        }
    }

    public void displayInterstitial(boolean callBack, final String nameFunction) {
        boolean show = false;
        if (this.canShowInterstitialAds()) {
            for (int i = 1; i <= MAX_NUMBER_MEDIATION; i++) {
                if (i == _priorityAdmobMediation) {
                    if (_admobMediation != null && _admobMediation.onInterstitialAvailable()) {
                        Log.d(TAG, "AdManager displayInterstitial Admob");
                        FirebaseAnalyticsController.logInterstitialAdsImpression("AdmobMediation");
                        _admobMediation.displayInterstitial(callBack, nameFunction);
                        show = true;
                        break;
                    }
                } else if (i == _priorityIronSourceMediation) {
                }
            }
        }
        if (!show) {
            loadInterstitial();
            this.callInterstitialHide(callBack, nameFunction, false);
        }
    }

    // ------------------------- Reward Video ---------------------------
    public void loadRewardedVideoAd() {
        if (_admobMediation != null) {
            _admobMediation.loadRewardedVideoAd();
        }
    }

    public void displayRewaredVideoAd(int source) {
        boolean show = false;
        for (int i = 1; i <= MAX_NUMBER_MEDIATION; i++) {
            if (i == _priorityAdmobMediation) {
                if (_admobMediation != null && _admobMediation.onRewardedViewAvailable()) {
                    Log.d(TAG, "AdManager displayRewaredVideoAd Admob");
                    FirebaseAnalyticsController.logVideoAdsImpression("AdmobMediation");
                    _admobMediation.displayRewaredVideoAd(source);
                    show = true;
                    break;
                }
            } else if (i == _priorityIronSourceMediation) {
            }
        }
        if (!show) {
            Log.d(TAG, "AdManager displayRewaredVideoAd NoVideoAds");
        }
    }

    public boolean onRewardedViewAvailable() {
        if ((_admobMediation != null && _admobMediation.onRewardedViewAvailable())) {
            return true;
        }
        return false;
    }

    // Callback to C++
    public void callInterstitialHide(final boolean callback, final String nameFunction, final boolean adClosed) {
        if (adClosed)
            _interstitialLastTime = System.currentTimeMillis() / 1000;
        if (callback) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    AdManager.interstitialHide(nameFunction, adClosed);
                }
            });
        }
    }

    public void callWatchVideoCompleted(final int source) {
        Cocos2dxHelper.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                AdManager.watchVideoCompleted(source);
            }
        });
    }

    public boolean canShowInterstitialAds() {
        long currentTime = System.currentTimeMillis() / 1000;
        if (currentTime - _interstitialLastTime >= _interstitialGrapTime || currentTime < _interstitialLastTime)
            return true;
        return false;
    }

    /////////////////////////////////////////
    //Call from C
    public static void call_LoadAllAds() {
        if (_instance != null && _instance._activity != null) {
            try {
                _instance._activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            _instance.loadAllAds();
                        } catch (Exception e) {
                        }
                    }
                });
            } catch (Exception e) {
            }
        }
    }

    public static void call_FetchRewardedAds() {
        if (_instance != null && _instance._activity != null) {
            try {
                _instance._activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            _instance.loadRewardedVideoAd();
                        } catch (Exception e) {
                        }
                    }
                });
            } catch (Exception e) {
            }
        }
    }

    public static boolean call_ShowRewardedAds(int requestSource) {
        if (_instance != null && _instance._activity != null) {
            if (!_instance.onRewardedViewAvailable())
                return false;
            try {
                final int source = requestSource;
                _instance._activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            _instance.displayRewaredVideoAd(source);
                        } catch (Exception e) {
                        }
                    }
                });
            } catch (Exception e) {
            }
            return true;
        }
        return false;
    }

    public static boolean call_CanShowInterstitialAds() {
        if (_instance != null && _instance._activity != null) {
            try {
                return _instance.canShowInterstitialAds();
            } catch (Exception e) {
            }
        }
        return false;
    }

    public static boolean call_IsRewardedAdsAvailable() {
        if (_instance != null && _instance._activity != null) {
            try {
                return _instance.onRewardedViewAvailable();
            } catch (Exception e) {
            }
        }
        return false;
    }

    public static void call_LoadInterstitialAds() {
        if (_instance != null && _instance._activity != null) {
            try {
                _instance._activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            _instance.loadInterstitial();
                        } catch (Exception e) {
                        }
                    }
                });
            } catch (Exception e) {
            }
        }
    }

    public static void call_ShowInterstitialAds(boolean callback, String nameFunction) {
        if (_instance != null && _instance._activity != null) {
            try {
                final boolean finalCallback = callback;
                final String finalNameFunction = nameFunction;
                _instance._activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            _instance.displayInterstitial(finalCallback, finalNameFunction);
                        } catch (Exception e) {
                        }
                    }
                });
            } catch (Exception e) {
            }
        }
    }

    public static void call_ShowBannerAds(final String name, final int pos, final float percentX, final float percentY) {
        if (_instance != null && _instance._activity != null) {
            try {
                _instance._activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            _instance.showBanner(name, pos, percentX, percentY);
                        } catch (Exception e) {
                        }
                    }
                });
            } catch (Exception e) {
            }
        }
    }

    public static void call_HideBannerAds(final String name) {
        if (_instance != null && _instance._activity != null) {
            try {
                _instance._activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            _instance.hideBanner(name);
                        } catch (Exception e) {
                        }
                    }
                });
            } catch (Exception e) {
            }
        }
    }

    public static int call_GetBottomBannerHeight() {
        if (_instance != null && _instance._activity != null) {
            try {
                return _instance.getBannerBottomHeight();
            } catch (Exception e) {
            }
        }
        return 0;
    }
}

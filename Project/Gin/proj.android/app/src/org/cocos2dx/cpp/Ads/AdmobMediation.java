package org.cocos2dx.cpp.Ads;

import android.app.Activity;
import android.content.res.Resources;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.widget.FrameLayout;

import com.emagssob.gin.R;
import com.google.android.ads.mediationtestsuite.MediationTestSuite;
import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.OnUserEarnedRewardListener;
import com.google.android.gms.ads.RequestConfiguration;
import com.google.android.gms.ads.initialization.AdapterStatus;
import com.google.android.gms.ads.initialization.InitializationStatus;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;
import com.google.android.gms.ads.rewarded.RewardItem;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;

import android.view.LayoutInflater;
import android.widget.RelativeLayout;

import androidx.annotation.NonNull;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.Connectivity;
import org.cocos2dx.cpp.FirebaseAnalyticsController;
import org.cocos2dx.cpp.FirebaseRC;
import org.cocos2dx.cpp.Utils;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class AdmobMediation {

    public enum BannerPos {
        TOP(1),
        MIDDLE(2),
        BOTTOM(3);

        private final int value;

        BannerPos(final int newValue) {
            value = newValue;
        }

        public int getValue() {
            return value;
        }
    }

    public static final String TAG = "AdmobMediation";
    public static final boolean TEST = false;

    public static final int NUMBER_MAX_FAIL_TO_LOAD_INTERSTITIAL = 0;
    public static final int NUMBER_MAX_FAIL_TO_LOAD_REWARD_VIDEO = 0;
    public static final String BANNER_DEFAULT_NAME = "banner_default";
    public static final String BANNER_LARGE_NAME = "banner_large";

    public static final String KEY_ADMOB_BANNER_ID_DEFAULT = "admob_banner_id_default";
    public static final String KEY_ADMOB_BANNER_ID_LARGE = "admob_banner_id_large";
    public static final String KEY_ADMOB_INTERSTITIAL_ID = "admob_interstitial_id";
    public static final String KEY_ADMOB_REWARD_VIDEO_ID = "admob_reward_video_id";
    public static final String KEY_ADMOB_ALLOW_LOG_EVENT = "admob_allow_log_event";
    public static final String KEY_ADMOB_BANNER_BOTTOM_PADDING_BONUS = "admob_banner_bottom_padding_bonus";
    public static final String KEY_DATA_PERSONALIZE_AD = "personalize_ad";

    private Activity _activity;
    // Layout banner
    private RelativeLayout _layoutBannerDefault;
    private FrameLayout _frameLayoutBannerDefault;
    private RelativeLayout _layoutBannerLarge;
    // Banner
    private AdView _defaultAdview;
    private AdView _largeAdview;
    // Interstitial
    private InterstitialAd _interstitial;
    private boolean _isCallBackInterstitial;
    private String _nameCallBackInterstitial;
    // Reward Video
    private RewardedAd _rewardVideo;

    private boolean _enableShowBanner = false;
    private boolean _isLoadingInterstitial = false;
    private int _countFailToLoadInterstitial = 0;
    private boolean _isLoadingRewardVideo = false;
    private int _countFailToLoadRewardVideo = 0;

    private int _requestSource = 0;
    private boolean _rewardVideoLoaded = false;

    private boolean _initSuccess;
    private String _bannerIdDefault;
    private String _bannerIdLarge;
    private String _interstitialId;
    private String _rewardedVideoId;
    private boolean _enableLog;
    private int _bannerPaddingBonus;
    private boolean _showPersonalizeAds;
    private boolean _shouldLoadAllAds;

    public AdmobMediation() {
        _activity = null;
        _layoutBannerDefault = null;
        _frameLayoutBannerDefault = null;
        _layoutBannerLarge = null;
        _defaultAdview = null;
        _largeAdview = null;
        _interstitial = null;
        _isCallBackInterstitial = false;
        _nameCallBackInterstitial = "";
        _rewardVideo = null;
        _initSuccess = false;
        _bannerIdDefault = "";
        _bannerIdLarge = "";
        _interstitialId = "";
        _rewardedVideoId = "";
        _enableLog = false;
        _bannerPaddingBonus = 10;
        _showPersonalizeAds = false;
        _shouldLoadAllAds = false;
    }

    public void create(Activity activity) {
        Log.d(TAG, "Init Admob");
        _activity = activity;
        // Init admob
        MobileAds.initialize(_activity, new OnInitializationCompleteListener() {
            @Override
            public void onInitializationComplete(@NonNull InitializationStatus initializationStatus) {
                Log.d(TAG, "onInitializationComplete");
                if (TEST) {
                    Map<String, AdapterStatus> statusMap = initializationStatus.getAdapterStatusMap();
                    for (String adapterClass : statusMap.keySet()) {
                        AdapterStatus status = statusMap.get(adapterClass);
                        if (status != null) {
                            Log.d(TAG, String.format("Adapter name: %s, Description: %s, Latency: %d", adapterClass, status.getDescription(), status.getLatency()));
                        }
                    }
                }
                initialize();
                if (_shouldLoadAllAds) {
                    _shouldLoadAllAds = false;
                    loadAllAds();
                }
            }
        });
    }

    public void initialize() {
        // Create Banner, Interstitial, RewardedVideo
        Resources resources = _activity.getResources();
        _enableLog = FirebaseRC.getBoolean(KEY_ADMOB_ALLOW_LOG_EVENT, false);
        _bannerPaddingBonus = FirebaseRC.getInteger(KEY_ADMOB_BANNER_BOTTOM_PADDING_BONUS, 10);
        _showPersonalizeAds = Utils.getBoolKey(_activity, KEY_DATA_PERSONALIZE_AD, false);

        if (TEST) {
            _bannerIdDefault = resources.getString(R.string.admob_banner_test);
            _bannerIdLarge = resources.getString(R.string.admob_banner_test);
            _interstitialId = resources.getString(R.string.admob_interstitial_test);
            _rewardedVideoId = resources.getString(R.string.admob_reward_video_test);
        } else {
            _bannerIdDefault = resources.getString(R.string.admob_banner_default);
            _bannerIdLarge = resources.getString(R.string.admob_banner_large);
            _interstitialId = resources.getString(R.string.admob_interstitial);
            _rewardedVideoId = resources.getString(R.string.admob_reward_video);

            _bannerIdDefault = FirebaseRC.getString(KEY_ADMOB_BANNER_ID_DEFAULT, _bannerIdDefault);
            _bannerIdLarge = FirebaseRC.getString(KEY_ADMOB_BANNER_ID_LARGE, _bannerIdLarge);
            _interstitialId = FirebaseRC.getString(KEY_ADMOB_INTERSTITIAL_ID, _interstitialId);
            _rewardedVideoId = FirebaseRC.getString(KEY_ADMOB_REWARD_VIDEO_ID, _rewardedVideoId);
        }

        // Banner
        if (_enableShowBanner) {
            LayoutInflater li = _activity.getLayoutInflater();

            // Init default banner
            _layoutBannerDefault = (RelativeLayout) li.inflate(R.layout.layout_banner_default, null);
            RelativeLayout.LayoutParams layoutBannerDefaultParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
            _activity.getWindow().addContentView(_layoutBannerDefault, layoutBannerDefaultParams);
            _frameLayoutBannerDefault = _layoutBannerDefault.findViewById(R.id.frameLayoutBannerBottom);
            _defaultAdview = new AdView(_activity);
            _defaultAdview.setAdUnitId(_bannerIdDefault);
            _defaultAdview.setAdSize(getAdaptiveBannerSize(1.0f));
            FrameLayout.LayoutParams layoutBannerDefault = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
            _frameLayoutBannerDefault.addView(_defaultAdview, layoutBannerDefault);

            // Init large banner
            _layoutBannerLarge = (RelativeLayout) li.inflate(R.layout.layout_banner_large, null);
            RelativeLayout.LayoutParams layoutBannerLargeParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
            _activity.getWindow().addContentView(_layoutBannerLarge, layoutBannerLargeParams);
            _largeAdview = new AdView(_activity);
            _largeAdview.setAdUnitId(_bannerIdLarge);
            _largeAdview.setAdSize(AdSize.MEDIUM_RECTANGLE);
            RelativeLayout.LayoutParams layoutBannerLarge = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
            _layoutBannerLarge.addView(_largeAdview, layoutBannerLarge);
        }

        //Test ads
//        List<String> testDeviceIds = Arrays.asList("392E9DA689A4E778DE1468D682BEC07D");
//        RequestConfiguration configuration =
//                new RequestConfiguration.Builder().setTestDeviceIds(testDeviceIds).build();
//        MobileAds.setRequestConfiguration(configuration);

        _initSuccess = true;

        hideBanner(BANNER_DEFAULT_NAME);
        hideBanner(BANNER_LARGE_NAME);

        if (TEST) {
            MediationTestSuite.launch(_activity);
        }
    }

    public void loadAllAds() {
        if (_initSuccess) {
            loadDelayInterstitial();
            createAndLoadNewVideoAds();
            if (_enableShowBanner) {
                loadBanner(BANNER_DEFAULT_NAME);
                loadBanner(BANNER_LARGE_NAME);
            }
        } else {
            _shouldLoadAllAds = true;
        }
    }

    private AdRequest createAdRequest() {
        /*
        AdRequest adRequest = null;
        if (_showPersonalizeAds) {
            adRequest = new AdRequest.Builder().build();
        } else {
            Bundle extras = new Bundle();
            extras.putString("npa", "1");

            adRequest = new AdRequest.Builder()
                    .addNetworkExtrasBundle(AdMobAdapter.class, extras)
                    .build();
        }
        */
        AdRequest adRequest = new AdRequest.Builder().build();
        return adRequest;
    }

    public void setShowPersonalizeAds(boolean showPersonalizeAds, boolean inUUE, boolean userChanged) {
        if (_showPersonalizeAds != showPersonalizeAds) {
            _showPersonalizeAds = showPersonalizeAds;
            Utils.setBoolKey(_activity, KEY_DATA_PERSONALIZE_AD, showPersonalizeAds);
        }
        // Update consent for all ad networks
        if (inUUE && userChanged) {
            // Unity Ads
//            MetaData gdprMetaData = new MetaData(_activity);
//            gdprMetaData.set("gdpr.consent", true);
//            gdprMetaData.commit();
        }
    }

    // ------------------------- Banner -----------------------
    public void loadBanner(String name) {
        if (_initSuccess) {
            if (_enableShowBanner) {
                if (name.equals(BANNER_DEFAULT_NAME)) {
                    if (_defaultAdview != null) {
                        AdRequest adRequest = createAdRequest();
                        _defaultAdview.loadAd(adRequest);
                    }
                } else if (name.equals(BANNER_LARGE_NAME)) {
                    if (_largeAdview != null) {
                        AdRequest adRequest = createAdRequest();
                        _largeAdview.loadAd(adRequest);
                    }
                }
            }
        }
    }

    public void showBanner(String name, int pos, float percentX, float percentY) {
        if (_initSuccess && _enableShowBanner) {
            if (name.equals(BANNER_DEFAULT_NAME)) {
                if (pos == BannerPos.TOP.getValue()) {
                    RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) _frameLayoutBannerDefault.getLayoutParams();
                    params.addRule(RelativeLayout.CENTER_HORIZONTAL);
                    params.addRule(RelativeLayout.ALIGN_PARENT_TOP);
                    // params.topMargin = 5;
                    _frameLayoutBannerDefault.setLayoutParams(params);
                } else if (pos == BannerPos.MIDDLE.getValue()) {
                    RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) _frameLayoutBannerDefault.getLayoutParams();
                    params.addRule(RelativeLayout.CENTER_HORIZONTAL);
                    params.addRule(RelativeLayout.CENTER_VERTICAL);
                    _frameLayoutBannerDefault.setLayoutParams(params);
                } else if (pos == BannerPos.BOTTOM.getValue()) {
                    RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) _frameLayoutBannerDefault.getLayoutParams();
                    params.addRule(RelativeLayout.CENTER_HORIZONTAL);
                    params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
                    // params.bottomMargin = 5;
                    _frameLayoutBannerDefault.setLayoutParams(params);
                }

                if (!_layoutBannerDefault.isEnabled()) {
                    _layoutBannerDefault.setEnabled(true);
                }
                if (_layoutBannerDefault.getVisibility() != View.VISIBLE) {
                    _layoutBannerDefault.setVisibility(View.VISIBLE);
                }
                _defaultAdview.setVisibility(View.VISIBLE);
            } else if (name.equals(BANNER_LARGE_NAME)) {
                Display display = _activity.getWindowManager().getDefaultDisplay();
                DisplayMetrics outMetrics = new DisplayMetrics();
                display.getMetrics(outMetrics);
                int notchHeight = AppActivity.getNavigationBarHeight();
                float left = (percentX * outMetrics.widthPixels) - AdSize.MEDIUM_RECTANGLE.getWidthInPixels(_activity.getApplicationContext()) * 0.5f;
                float top = ((1 - percentY) * (outMetrics.heightPixels + notchHeight)) - AdSize.MEDIUM_RECTANGLE.getHeightInPixels(_activity.getApplicationContext()) * 0.5f;

                RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) _largeAdview.getLayoutParams();
                if (pos == BannerPos.TOP.getValue()) {
                    params.leftMargin = (int) left;
                    // params.topMargin = 5;
                } else if (pos == BannerPos.MIDDLE.getValue()) {
                    params.leftMargin = (int) left;
                    params.topMargin = (int) top;

                } else if (pos == BannerPos.BOTTOM.getValue()) {
                    params.leftMargin = (int) left;
                    // params.bottomMargin = 5;
                }
                if (!_layoutBannerLarge.isEnabled()) {
                    _layoutBannerLarge.setEnabled(true);
                }
                if (_layoutBannerLarge.getVisibility() != View.VISIBLE) {
                    _layoutBannerLarge.setVisibility(View.VISIBLE);
                }
                _largeAdview.setLayoutParams(params);
                _largeAdview.setVisibility(View.VISIBLE);
            }
        }
    }

    public void hideBanner(String name) {
        if (_initSuccess && _enableShowBanner) {
            if (name.equals(BANNER_DEFAULT_NAME)) {
                if (_layoutBannerDefault.isEnabled()) {
                    _layoutBannerDefault.setEnabled(false);
                }
                if (_layoutBannerDefault.getVisibility() == View.VISIBLE) {
                    _layoutBannerDefault.setVisibility(View.INVISIBLE);
                }
                _defaultAdview.setVisibility(View.GONE);
            } else if (name.equals(BANNER_LARGE_NAME)) {
                if (_layoutBannerLarge.isEnabled()) {
                    _layoutBannerLarge.setEnabled(false);
                }
                if (_layoutBannerLarge.getVisibility() == View.VISIBLE) {
                    _layoutBannerLarge.setVisibility(View.INVISIBLE);
                }
                _largeAdview.setVisibility(View.GONE);
            }
        }
    }

    private AdSize getAdaptiveBannerSize(float widthPercent) {
        Display display = _activity.getWindowManager().getDefaultDisplay();
        DisplayMetrics outMetrics = new DisplayMetrics();
        display.getMetrics(outMetrics);

        float widthPixels = outMetrics.widthPixels * widthPercent;
        float density = outMetrics.density;
        int adWidth = (int) (widthPixels / density);
        return AdSize.getCurrentOrientationAnchoredAdaptiveBannerAdSize(_activity, adWidth);
    }

    public int getBannerBottomHeight() {
        Display display = _activity.getWindowManager().getDefaultDisplay();
        DisplayMetrics outMetrics = new DisplayMetrics();
        display.getMetrics(outMetrics);
        int result = Math.round(getAdaptiveBannerSize(1.0f).getHeight() / (outMetrics.heightPixels / outMetrics.density) * AppActivity.DESIGN_RESOLUTION_HEIGHT);
        return result + _bannerPaddingBonus;
    }

    // ------------------------ Interstitial -------------------------
    public void requestNewInterstitial() {
        if (Connectivity.isConnected(_activity)) {
            _isLoadingInterstitial = true;
            AdRequest adRequest = this.createAdRequest();
            InterstitialAd.load(_activity, _interstitialId, adRequest, new InterstitialAdLoadCallback() {
                @Override
                public void onAdLoaded(@NonNull InterstitialAd interstitialAd) {
                    super.onAdLoaded(interstitialAd);
                    _interstitial = interstitialAd;
                    _isLoadingInterstitial = false;
                    Log.d(TAG, "Interstitial onAdLoaded");
                }

                @Override
                public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                    super.onAdFailedToLoad(loadAdError);
                    Log.d(TAG, "Interstitial onAdFailedToLoad: " + loadAdError.getMessage());
                    _interstitial = null;
                    _isLoadingInterstitial = false;
                    logLoadAdFail("interstitial", loadAdError);
                    _countFailToLoadInterstitial++;
                    if (_countFailToLoadInterstitial <= NUMBER_MAX_FAIL_TO_LOAD_INTERSTITIAL)
                        requestNewInterstitial();
                }
            });
        }
    }

    public void loadInterstitial() {
        if (_initSuccess) {
            if (_interstitial == null && !_isLoadingInterstitial) {
                _countFailToLoadInterstitial = 0;
                this.requestNewInterstitial();
            }
        }
    }

    public void loadDelayInterstitial() {
        if (_initSuccess && _activity != null && _interstitial == null && !_isLoadingInterstitial) {
            try {
                _activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        new Handler().postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                loadInterstitial();
                            }
                        }, 1000);
                    }
                });
            } catch (Exception ex) {
                loadInterstitial();
            }
        }
    }

    public void displayInterstitial(boolean callBack, final String nameFunction) {
        if (_interstitial != null) {
            _interstitial.setFullScreenContentCallback(new FullScreenContentCallback() {
                @Override
                public void onAdDismissedFullScreenContent() {
                    super.onAdDismissedFullScreenContent();
                    Log.d(TAG, "Interstitial The ad was dismissed.");
                    _interstitial = null;
                    loadDelayInterstitial();
                    AdManager._instance.callInterstitialHide(_isCallBackInterstitial, _nameCallBackInterstitial, true);
                }

                @Override
                public void onAdFailedToShowFullScreenContent(@NonNull AdError adError) {
                    super.onAdFailedToShowFullScreenContent(adError);
                    Log.d(TAG, "The ad failed to show.");
                    _interstitial = null;
                    loadDelayInterstitial();
                    AdManager._instance.callInterstitialHide(_isCallBackInterstitial, _nameCallBackInterstitial, false);
                }

                @Override
                public void onAdShowedFullScreenContent() {
                    super.onAdShowedFullScreenContent();
                    _interstitial = null;
                    Log.d(TAG, "The ad was shown.");
                }

                @Override
                public void onAdClicked() {
                    super.onAdClicked();
                }

                @Override
                public void onAdImpression() {
                    super.onAdImpression();
                }
            });
        }
        // Show interstitial
        if (_interstitial != null) {
            _isCallBackInterstitial = callBack;
            _nameCallBackInterstitial = nameFunction;
            _interstitial.show(_activity);
        } else {
            this.loadDelayInterstitial();
            AdManager._instance.callInterstitialHide(callBack, nameFunction, false);
        }
    }

    public boolean onInterstitialAvailable() {
        return (_interstitial != null);
    }

    // ------------------------- Reward Video ---------------------------
    private void createAndLoadNewVideoAds() {
        if (_initSuccess) {
            _isLoadingRewardVideo = true;
            AdRequest adRequest = this.createAdRequest();
            RewardedAd.load(_activity, _rewardedVideoId, adRequest, new RewardedAdLoadCallback() {
                @Override
                public void onAdLoaded(@NonNull RewardedAd rewardedAd) {
                    super.onAdLoaded(rewardedAd);
                    Log.d(TAG, "RewardedAd onAdLoaded");
                    _rewardVideo = rewardedAd;
                    _isLoadingRewardVideo = false;
                    _rewardVideoLoaded = true;
                    _countFailToLoadRewardVideo = 0;
                }

                @Override
                public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                    super.onAdFailedToLoad(loadAdError);
                    Log.d(TAG, "RewardedAd onAdFailedToLoad: " + loadAdError.getMessage());
                    _rewardVideo = null;
                    logLoadAdFail("reward_ad", loadAdError);
                    _isLoadingRewardVideo = false;
                    if (Connectivity.isConnected(_activity)) {
                        _countFailToLoadRewardVideo++;
                        if (_countFailToLoadRewardVideo <= NUMBER_MAX_FAIL_TO_LOAD_REWARD_VIDEO) {
                            createAndLoadNewVideoAds();
                        }
                    }
                }
            });
        }
    }

    public void loadRewardedVideoAd() {
        if (_rewardVideo == null && !_isLoadingRewardVideo) {
            _rewardVideoLoaded = false;
            _countFailToLoadRewardVideo = 0;
            this.createAndLoadNewVideoAds();
        }
    }

    public void displayRewaredVideoAd(int source) {
        if (_rewardVideo != null) {
            _requestSource = source;
            _rewardVideo.setFullScreenContentCallback(new FullScreenContentCallback() {
                @Override
                public void onAdShowedFullScreenContent() {
                    super.onAdShowedFullScreenContent();
                    Log.d(TAG, "RewardedAd onAdShowedFullScreenContent");
                    _rewardVideo = null;
                }

                @Override
                public void onAdFailedToShowFullScreenContent(@NonNull AdError adError) {
                    super.onAdFailedToShowFullScreenContent(adError);
                    Log.d(TAG, "RewardedAd onAdFailedToShowFullScreenContent:" + adError.toString());
                    _rewardVideo = null;
                    loadRewardedVideoAd();
                }

                @Override
                public void onAdDismissedFullScreenContent() {
                    super.onAdDismissedFullScreenContent();
                    Log.d(TAG, "RewardedAd onAdDismissedFullScreenContent");
                    loadRewardedVideoAd();
                }
            });
            _rewardVideo.show(_activity, new OnUserEarnedRewardListener() {
                @Override
                public void onUserEarnedReward(@NonNull RewardItem rewardItem) {
                    Log.d(TAG, "RewardedAd onUserEarnedReward");
                    AdManager._instance.callWatchVideoCompleted(_requestSource);
                    _requestSource = 0;
                }
            });
        } else {
            _rewardVideoLoaded = false;
            Log.d(TAG, "RewardedAd The rewarded ad wasn't ready yet.");
        }
    }

    public boolean onRewardedViewAvailable() {
        return _rewardVideoLoaded;
    }

    public void logLoadAdFail(String name, LoadAdError adError) {
        if (_enableLog) {
            int errorCode = adError.getCode();
            String errorMsg = adError.getMessage();
            String domain = adError.getDomain();
            String cause = "";
            AdError causeError = adError.getCause();
            if (causeError != null) {
                cause = causeError.toString();
            }
            FirebaseAnalyticsController.logAdsLoadFail(name, errorCode, errorMsg, cause, domain);
        }
    }
}

package org.cocos2dx.cpp;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;
import android.view.View;
import android.widget.Toast;

import com.google.android.ump.ConsentDebugSettings;
import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

import org.cocos2dx.cpp.Ads.AdManager;
import org.cocos2dx.lib.Cocos2dxActivity;

import java.lang.reflect.Method;
import java.util.Calendar;

import org.cocos2dx.lib.Cocos2dxHelper;

import com.google.android.ump.ConsentInformation;
import com.google.android.ump.ConsentRequestParameters;
import com.google.android.ump.FormError;
import com.google.android.ump.ConsentForm;
import com.google.android.ump.UserMessagingPlatform;

public class AppActivity extends Cocos2dxActivity {

    public static final String TAG = "AppActivity";

    public static AppActivity mInstance;
    private AdManager mAdManager = null;
    private PlayGameService mPlayGameService = null;
    private FirebaseRC firebaseRemoteConfig = null;
    private StoreService storeService = null;
    public PackageManager mPackageManager = null;
    private static boolean checkStartupDone = false;
    private static boolean willPlayMusic = true;

    public static int DESIGN_RESOLUTION_WIDTH = 1136;
    public static int DESIGN_RESOLUTION_HEIGHT = 640;

    public ConsentInformation consentInformation;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        if (Build.VERSION.SDK_INT >= 24) {
            try {
                Method m = StrictMode.class.getMethod("disableDeathOnFileUriExposure");
                m.invoke(null);
            } catch (Exception e) {
            }
        }
        mInstance = this;
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        hideSystemUI();

        try {
            willPlayMusic = Cocos2dxHelper.willPlayBackgroundMusic();
        } catch (Exception ex) {
        }

        try {
            FirebaseAnalyticsController.onCreate(this);
            FirebaseNotification.create();
        } catch (Exception ex) {
        }

        try {
            mPlayGameService = new PlayGameService();
            mPlayGameService.create(this);
        } catch (Exception ex) {
        }

        try {
            firebaseRemoteConfig = new FirebaseRC();
            firebaseRemoteConfig.create(this);
        } catch (Exception ex) {
        }

        try {
            storeService = new StoreService();
            storeService.create(this);
        } catch (Exception ex) {
        }

        try {
            mPackageManager = getPackageManager();
        } catch (Exception ex) {
        }
    }

    public void initAdsManager() {
        try {
            if (mAdManager == null) {
                mAdManager = new AdManager();
                mAdManager.create(this);
                mAdManager.loadAllAds();
            }
        } catch (Exception ex) {
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        try {
            if (mPlayGameService != null)
                mPlayGameService.onResume();
        } catch (Exception ex) {
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus)
            hideSystemUI();
    }

    private void hideSystemUI() {
        try {
            // Enables regular immersive mode.
            // For "lean back" mode, remove SYSTEM_UI_FLAG_IMMERSIVE.
            // Or for "sticky immersive," replace it with SYSTEM_UI_FLAG_IMMERSIVE_STICKY
            View decorView = getWindow().getDecorView();
            decorView.setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        } catch (Exception ex) {
        }
    }

    public void showArlertAndroid(final String message) {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (!AppActivity.this.isFinishing())
                    Toast.makeText(AppActivity.this, message, Toast.LENGTH_LONG).show();
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        try {
            Bundle bundle = intent.getExtras();
            if (bundle != null && bundle.containsKey(LocalNotificationReceiver.FIELD_DATA_NAME))
                FirebaseAnalyticsController.logOpenLocalNotification(bundle.getString(LocalNotificationReceiver.FIELD_DATA_NAME), bundle.getInt(LocalNotificationReceiver.FIELD_DATA_ID));
        } catch (Exception ex) {
        }
    }

    public void checkStartupState() {
        try {
            if (firebaseRemoteConfig != null && firebaseRemoteConfig.isFetchCompleted())
                checkStartupDone = true;
        } catch (Exception ex) {
        }
    }

    public static int getNavigationBarHeight() {
        try {
            DisplayMetrics metrics = new DisplayMetrics();
            mInstance.getWindowManager().getDefaultDisplay().getMetrics(metrics);
            int usableHeight = metrics.heightPixels;
            mInstance.getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
            int realHeight = metrics.heightPixels;
            if (realHeight > usableHeight)
                return realHeight - usableHeight;
        } catch (Exception ex) {
        }
        return 0;
    }

    public static boolean checkInternet() {
        if (Connectivity.isConnected(mInstance))
            return true;
        return false;
    }

    public static void showAlert(final String message) {
        try {
            mInstance.showArlertAndroid(message);
        } catch (Exception e) {
        }
    }

    public static void openAppInStore() {
        try {
            Intent i = new Intent(Intent.ACTION_VIEW);
            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            i.setData(Uri.parse("market://details?id=" + AppActivity.mInstance.getPackageName().toString()));
            Cocos2dxActivity activity = (Cocos2dxActivity) getContext();
            activity.startActivity(i);
        } catch (Exception e) {
        }
    }

    public static void openAnotherAppInStore(String url) {
        try {
            Intent intent = mInstance.getPackageManager().getLaunchIntentForPackage(url);
            if (intent != null) {
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                mInstance.startActivity(intent);
            } else {
                Intent i = new Intent(Intent.ACTION_VIEW);
                i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                i.setData(Uri.parse("market://details?id=" + url));
                ((Cocos2dxActivity) getContext()).startActivity(i);
            }
        } catch (Exception e) {
        }
    }

    public static void openAnotherAppInStoreByLink(String url) {
        try {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setData(Uri.parse(url));
            ((Cocos2dxActivity) getContext()).startActivity(intent);
        } catch (Exception e) {
        }
    }

    public static void copyToClipboard(final String data) {
        AppActivity.mInstance.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    android.content.ClipboardManager clipboard = (android.content.ClipboardManager) AppActivity.mInstance.getSystemService(Context.CLIPBOARD_SERVICE);
                    android.content.ClipData clip = android.content.ClipData.newPlainText("text", data);
                    clipboard.setPrimaryClip(clip);
                    Toast.makeText(AppActivity.mInstance.getApplicationContext(), "Copied to Clipboard!", Toast.LENGTH_SHORT).show();
                } catch (Exception e) {
                    Toast.makeText(AppActivity.mInstance.getApplicationContext(), e.toString(), Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    public static void initAds() {
        try {
            mInstance.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    try {
//                        mInstance.initAdsManager();
                        mInstance.setupAdsConsentAndInitAdsManager();
                    } catch (Exception e) {
                    }
                }
            });
        } catch (Exception e) {
        }
    }

    public static String getMyPackageName() {
        return mInstance.getPackageName();
    }

    public static boolean isPackageInstalled(String packageName) {
        return false;
    }

    public static int getVersionCode() {
        try {
            int versionCode = Cocos2dxActivity.getContext().getPackageManager().getPackageInfo(Cocos2dxActivity.getContext().getPackageName(), 0).versionCode;
            return versionCode;
        } catch (Exception e) {
            return 0;
        }
    }

    public static String getVersionName() {
        try {
            String versionName = Cocos2dxActivity.getContext().getPackageManager().getPackageInfo(Cocos2dxActivity.getContext().getPackageName(), 0).versionName;
            return versionName;
        } catch (Exception e) {
            return "0.0.0";
        }
    }

    public static void scheduleLocalNotification(String dataJson) {
        try {
            AppActivity.cancelAllLocalNotification();
            Log.d(TAG, "scheduleLocalNotification");
            Log.d(TAG, "dataJson: " + dataJson);
            JsonArray listNotification = new Gson().fromJson(dataJson, JsonArray.class);
            if (listNotification.size() > 0) {
                long currentTime = System.currentTimeMillis() / 1000;
                for (int i = 0; i < listNotification.size(); i++) {
                    JsonObject notification = listNotification.get(i).getAsJsonObject();
                    int id = notification.get(LocalNotificationReceiver.FIELD_DATA_ID).getAsInt();
                    String name = notification.get(LocalNotificationReceiver.FIELD_DATA_NAME).getAsString();
                    String type = notification.get(LocalNotificationReceiver.FIELD_DATA_TYPE).getAsString();
                    String title = notification.get(LocalNotificationReceiver.FIELD_DATA_TITLE).getAsString();
                    String message = notification.get(LocalNotificationReceiver.FIELD_DATA_MESSAGE).getAsString();
                    String image = notification.get(LocalNotificationReceiver.FIELD_DATA_IMAGE).getAsString();
                    long periodTime = (long) notification.get(LocalNotificationReceiver.FIELD_DATA_PERIOD_TIME).getAsDouble();
                    if (periodTime - currentTime > 0) {
                        addAlarmLocalNotification(id, name, type, title, message, image, periodTime);
                    }
                }
            }
        } catch (Exception ex) {
            Log.e(TAG, ex.getLocalizedMessage());
        }
    }

    public static void cancelAllLocalNotification() {
        try {
            Log.d(TAG, "cancelAllLocalNotification");
            NotificationUtils.cancelAllAlarms(mInstance);
        } catch (Exception ex) {
            Log.e(TAG, ex.getLocalizedMessage());
        }
    }

    public static void cleanAllLocalNotification() {
        try {
            Log.d(TAG, "cleanAllLocalNotification");
            NotificationUtils.cleanAllNotifications(mInstance);
        } catch (Exception ex) {
            Log.e(TAG, ex.getLocalizedMessage());
        }
    }

    public static void addAlarmLocalNotification(int id, String name, String type, String title, String message, String image, long periodTime) {
        try {
            Calendar setcalendar = Calendar.getInstance();
            setcalendar.setTimeInMillis((long) periodTime * 1000);
            Log.d(TAG, "addAlarmLocalNotification " + id + " " + name + " " + type + " " + title + " " + message + " " + image + " " + setcalendar.getTime());

            ComponentName receiver = new ComponentName(mInstance, LocalNotificationReceiver.class);
            PackageManager pm = mInstance.getPackageManager();
            pm.setComponentEnabledSetting(receiver,
                    PackageManager.COMPONENT_ENABLED_STATE_ENABLED,
                    PackageManager.DONT_KILL_APP);

            Intent intent = new Intent(mInstance, LocalNotificationReceiver.class);
            Bundle data = new Bundle();
            data.putInt(LocalNotificationReceiver.FIELD_DATA_ID, id);
            data.putString(LocalNotificationReceiver.FIELD_DATA_NAME, name);
            data.putString(LocalNotificationReceiver.FIELD_DATA_TYPE, type);
            data.putString(LocalNotificationReceiver.FIELD_DATA_TITLE, title);
            data.putString(LocalNotificationReceiver.FIELD_DATA_MESSAGE, message);
            data.putString(LocalNotificationReceiver.FIELD_DATA_IMAGE, image);
            data.putString(LocalNotificationReceiver.FIELD_DATA_PERIOD_TIME, setcalendar.getTime().toString());
            intent.putExtras(data);

            PendingIntent pendingIntent = PendingIntent.getBroadcast(mInstance, id, intent, PendingIntent.FLAG_UPDATE_CURRENT);
            AlarmManager am = (AlarmManager) mInstance.getSystemService(ALARM_SERVICE);
            am.set(AlarmManager.RTC_WAKEUP, setcalendar.getTimeInMillis(), pendingIntent);

            // Save this notification to database
            NotificationUtils.saveAlarmId(mInstance, id);
        } catch (Exception ex) {
            Log.e(TAG, ex.getLocalizedMessage());
        }
    }

    public static String getURLAppStore() {
        return "https://play.google.com/store/apps/details?id=" + AppActivity.mInstance.getPackageName();
    }

    public static void shareDataToMessenger(final String title, final String message) {
        mInstance.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Intent sendIntent = new Intent();
                sendIntent.setAction(Intent.ACTION_SEND);
                sendIntent.putExtra(Intent.EXTRA_TITLE, title);
                sendIntent.putExtra(Intent.EXTRA_TEXT, message);
                sendIntent.setType("text/plain");

                Intent shareIntent = Intent.createChooser(sendIntent, null);
                mInstance.startActivity(shareIntent);
            }
        });
    }

    //  This will pause music which is playing from another App Music
    public static void forceToRegisterAudioFocusListenner() {
        try {
            mInstance.registerAudioFocusListenner();
        } catch (Exception e) {
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        try {
            if (mPlayGameService != null) {
                mPlayGameService.onActivityResult(requestCode, resultCode, data);
            }
        } catch (Exception ex) {
        }
    }

    public static void setDesignResolutionContentSize(int width, int height) {
        AppActivity.DESIGN_RESOLUTION_WIDTH = width;
        AppActivity.DESIGN_RESOLUTION_HEIGHT = height;
    }

    public static boolean checkStartup() {
        try {
            mInstance.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    try {
                        mInstance.checkStartupState();
                    } catch (Exception ex) {
                    }
                }
            });
            if (AppActivity.checkStartupDone)
                return true;
        } catch (Exception ex) {
        }
        return false;
    }

    public static boolean willPlayBackgroundMusic() {
        return AppActivity.willPlayMusic;
    }

    public void setupAdsConsentAndInitAdsManager()
    {
        // Debug
//        ConsentDebugSettings debugSettings = new ConsentDebugSettings.Builder(this)
//                .addTestDeviceHashedId("392E9DA689A4E778DE1468D682BEC07D")
//                .setDebugGeography(ConsentDebugSettings.DebugGeography.DEBUG_GEOGRAPHY_EEA)
//                .build();

        // Set tag for under age of consent. false means users are not under age
        // of consent.
        ConsentRequestParameters params = new ConsentRequestParameters
                .Builder()
                .setTagForUnderAgeOfConsent(false)
//                .setConsentDebugSettings(debugSettings) // Debug
                .build();

        consentInformation = UserMessagingPlatform.getConsentInformation(this);
        consentInformation.reset();
        consentInformation.requestConsentInfoUpdate(
                this,
                params,
                (ConsentInformation.OnConsentInfoUpdateSuccessListener) () -> {
                    // TODO: Load and show the consent form.
                    UserMessagingPlatform.loadAndShowConsentFormIfRequired(
                            this,
                            (ConsentForm.OnConsentFormDismissedListener) loadAndShowError -> {
                                if (loadAndShowError != null) {
                                    // Consent gathering failed.
                                    Log.w(TAG, String.format("%s: %s",
                                            loadAndShowError.getErrorCode(),
                                            loadAndShowError.getMessage()));
                                }

                                // Consent has been gathered.
                                if(consentInformation.canRequestAds() == true)
                                {
                                    mInstance.initAdsManager();
                                }
                            }
                    );
                },
                (ConsentInformation.OnConsentInfoUpdateFailureListener) requestConsentError -> {
                    // Consent gathering failed.
                    Log.w(TAG, String.format("%s: %s",
                            requestConsentError.getErrorCode(),
                            requestConsentError.getMessage()));
                });

        // Check if you can initialize the Google Mobile Ads SDK in parallel
        // while checking for new consent information. Consent obtained in
        // the previous session can be used to request ads.
        if(consentInformation.canRequestAds() == true)
        {
            mInstance.initAdsManager();
        }
    }
}

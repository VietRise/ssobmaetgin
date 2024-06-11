package org.cocos2dx.cpp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class LocalNotificationReceiver extends BroadcastReceiver {
    public static final boolean DEBUG = false;
    public static final String TAG = "NotificationReceiver";
    public static final String FIELD_DATA_ID = "id";
    public static final String FIELD_DATA_NAME = "name";
    public static final String FIELD_DATA_TYPE = "type";
    public static final String FIELD_DATA_TITLE = "title";
    public static final String FIELD_DATA_MESSAGE = "message";
    public static final String FIELD_DATA_IMAGE = "image";
    public static final String FIELD_DATA_PERIOD_TIME = "period_time";

    public static final String TYPE_NORMAL = "normal";
    public static final String TYPE_BANNER = "banner";

    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "onReceive");
        Bundle data = intent.getExtras();
        if (data != null) {
            try {
                int id = data.getInt(LocalNotificationReceiver.FIELD_DATA_ID);
                String name = data.getString(LocalNotificationReceiver.FIELD_DATA_NAME);
                String type = data.getString(LocalNotificationReceiver.FIELD_DATA_TYPE);
                String title = data.getString(LocalNotificationReceiver.FIELD_DATA_TITLE);
                String message = data.getString(LocalNotificationReceiver.FIELD_DATA_MESSAGE);
                String image = data.getString(LocalNotificationReceiver.FIELD_DATA_IMAGE);
                String time = data.getString(LocalNotificationReceiver.FIELD_DATA_PERIOD_TIME);
                if (DEBUG)
                    title += " (" + time + ")";

                if (type.equals(TYPE_NORMAL)) {
                    NotificationUtils.sendNotificationNormal(context, id, name, title, message, image);
                    NotificationUtils.removeAlarmId(context, id);
                    FirebaseAnalyticsController.logReceiveLocalNotification(context, name, id);
                } else if (type.equals(TYPE_BANNER)) {
                    NotificationUtils.sendNotificationCustomBackground(context, id, name, image);
                    NotificationUtils.removeAlarmId(context, id);
                    FirebaseAnalyticsController.logReceiveLocalNotification(context, name, id);
                }
            } catch (Exception e) {
                Log.e("LocalNotificationR", e.toString());
            }
        }
    }
}

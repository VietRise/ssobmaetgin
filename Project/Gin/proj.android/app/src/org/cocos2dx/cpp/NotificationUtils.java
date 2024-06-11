package org.cocos2dx.cpp;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.preference.PreferenceManager;
import android.util.Log;

import androidx.core.app.NotificationCompat;
import androidx.core.content.ContextCompat;

import com.emagssob.gin.R;

import org.json.JSONArray;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class NotificationUtils {
    private static final String sTagAlarms = ":alarms";
    public static final String TAG = "NotificationUtils";
    public static final boolean DEBUG = false;

    public static void sendNotificationNormal(Context context, int id, String name, String title, String message, String urlIcon) {
        try {
            Intent intent = new Intent(context, AppActivity.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
            intent.putExtra(LocalNotificationReceiver.FIELD_DATA_NAME, name);
            intent.putExtra(LocalNotificationReceiver.FIELD_DATA_ID, id);
            PendingIntent pendingIntent = PendingIntent.getActivity(context, 0 /* Request code */, intent,
                    PendingIntent.FLAG_ONE_SHOT);
            Bitmap largeImage = null;
            if (!urlIcon.isEmpty() && Utils.urlValidator(urlIcon)) {
                URL url = new URL(urlIcon);
                largeImage = BitmapFactory.decodeStream(url.openConnection().getInputStream());
            } else {
                largeImage = BitmapFactory.decodeResource(context.getResources(), R.drawable.ic_notification);
            }

            String channelId = context.getString(R.string.default_notification_channel_id);
            Uri defaultSoundUri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
            NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(context, channelId)
                    .setVisibility(NotificationCompat.VISIBILITY_PUBLIC)
                    .setSmallIcon(R.drawable.icon)
                    //.setStyle(new androidx.media.app.NotificationCompat.MediaStyle())
                    //.setCategory(NotificationCompat.CATEGORY_PROMO)
                    .setColor(ContextCompat.getColor(context, R.color.colorPrimary))
                    .setContentTitle(title)
                    .setContentText(message)
                    .setLargeIcon(largeImage)
                    .setAutoCancel(true)
                    .setSound(defaultSoundUri)
                    .setContentIntent(pendingIntent);

            NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
            if (notificationManager != null) {
                // Since android Oreo notification channel is needed.
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    NotificationChannel channel = new NotificationChannel(channelId, Utils.getAppName(context), NotificationManager.IMPORTANCE_DEFAULT);
                    notificationManager.createNotificationChannel(channel);
                }
                notificationManager.notify(id /* ID of notification */, notificationBuilder.build());
            }
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    public static void sendNotificationCustomBackground(Context context, int id, String name, String urlImageBG) {
    }

    private static void cancelAlarm(Context context, int notificationId) {
        try {
            AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
            Intent intent = new Intent(context, LocalNotificationReceiver.class);
            PendingIntent pendingIntent = PendingIntent.getBroadcast(context, notificationId, intent, PendingIntent.FLAG_CANCEL_CURRENT);
            if (alarmManager != null && pendingIntent != null && NotificationUtils.hasAlarm(context, intent, notificationId)) {
                alarmManager.cancel(pendingIntent);
                pendingIntent.cancel();
            }
            removeAlarmId(context, notificationId);
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    public static void cancelAllAlarms(Context context) {
        try {
            List<Integer> listNoficationID = getAlarmIds(context);
            if (DEBUG) {
                Log.d(TAG, "cancelAllAlarms IDs: " + listNoficationID.toString());
            }
            for (int idAlarm : listNoficationID) {
                cancelAlarm(context, idAlarm);
            }
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    public static boolean hasAlarm(Context context, Intent intent, int notificationId) {
        return PendingIntent.getBroadcast(context, notificationId, intent, PendingIntent.FLAG_NO_CREATE) != null;
    }

    public static void saveAlarmId(Context context, int id) {
        try {
            List<Integer> idsAlarms = getAlarmIds(context);
            if (idsAlarms.contains(id)) {
                return;
            }
            idsAlarms.add(id);
            if (DEBUG) {
                Log.d(TAG, "saveAlarmId IDs: " + idsAlarms.toString());
            }
            saveIdsInPreferences(context, idsAlarms);
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    public static void removeAlarmId(Context context, int id) {
        try {
            List<Integer> idsAlarms = getAlarmIds(context);

            for (int i = 0; i < idsAlarms.size(); i++) {
                if (idsAlarms.get(i) == id)
                    idsAlarms.remove(i);
            }
            if (DEBUG) {
                Log.d(TAG, "removeAlarmId IDs: " + idsAlarms.toString());
            }
            saveIdsInPreferences(context, idsAlarms);
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    private static List<Integer> getAlarmIds(Context context) {
        List<Integer> ids = new ArrayList<>();
        try {
            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
            JSONArray jsonArray2 = new JSONArray(prefs.getString(context.getPackageName() + sTagAlarms, "[]"));

            for (int i = 0; i < jsonArray2.length(); i++) {
                ids.add(jsonArray2.getInt(i));
            }

        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }

        return ids;
    }

    private static void saveIdsInPreferences(Context context, List<Integer> lstIds) {
        try {
            JSONArray jsonArray = new JSONArray();
            for (Integer idAlarm : lstIds) {
                jsonArray.put(idAlarm);
            }
            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
            SharedPreferences.Editor editor = prefs.edit();
            editor.putString(context.getPackageName() + sTagAlarms, jsonArray.toString());
            editor.apply();
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    public static void cleanAllNotifications(Context context) {
        try {
            NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
            notificationManager.cancelAll();
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }
}

package org.cocos2dx.cpp;

import android.net.Uri;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.messaging.FirebaseMessaging;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import java.util.Map;

public class FirebaseNotification extends FirebaseMessagingService {

    private static final String[] TOPICS = {"global"};
    // private static final String[] TOPICS = {"test"};
    private static final String NAME = "Firebase";
    private static final String TAG = "FirebaseNotification";
    private static boolean DEBUG_GET_TOKEN = false;

    private static final String CUSTOM_MESSAGE = "CustomMessage";
    private static final String CUSTOM_MESSAGE_VALUE_1 = "banner";

    public static void create() {
        // Get FCM token
        if (DEBUG_GET_TOKEN) {
            FirebaseMessaging.getInstance().getToken().addOnCompleteListener(new OnCompleteListener<String>() {
                @Override
                public void onComplete(@NonNull Task<String> task) {
                    if (!task.isSuccessful()) {
                        Log.w(TAG, "getInstanceId failed", task.getException());
                        return;
                    }
                    // Get new Instance ID token
                    String token = task.getResult();
                    Log.d(TAG, "FCM token:" + token);
                }
            });
        }
    }

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        super.onMessageReceived(remoteMessage);
        // TODO(developer): Handle FCM messages here.
        // Not getting messages here? See why this may be: https://goo.gl/39bRNJ
        Log.d(TAG, "From: " + remoteMessage.getFrom());
        String customMessage = "";
        // Check if message contains a data payload.
        Map<String, String> data = remoteMessage.getData();
        if (data.size() > 0) {
            Log.d(TAG, "Message data payload: " + data);
            customMessage = data.get(CUSTOM_MESSAGE);
        }
        // Check if message contains a notification payload.
        if (remoteMessage.getNotification() != null) {
            Log.d(TAG, "Message Notification Body: " + remoteMessage.getNotification().getBody());
            if (customMessage != null && customMessage.equals(CUSTOM_MESSAGE_VALUE_1)) {
            } else {
                this.sendNotification(remoteMessage.getNotification());
            }
        }
        // Also if you intend on generating your own notifications as a result of a received FCM
        // message, here is where that should be initiated. See sendNotification method below.
    }

    @Override
    public void onDeletedMessages() {
        super.onDeletedMessages();
    }

    @Override
    public void onNewToken(String s) {
        super.onNewToken(s);
    }

    public static void subscribeTopic() {
        for (String topic : TOPICS) {
            FirebaseMessaging.getInstance().subscribeToTopic(topic);
        }
    }

    private void sendNotification(RemoteMessage.Notification notificationBody) {

        Uri uriNotification = notificationBody.getImageUrl();
        String urlImage = "";
        if (uriNotification != null) {
            urlImage = uriNotification.toString();
        }
        NotificationUtils.sendNotificationNormal(this, 0, NAME, notificationBody.getTitle(), notificationBody.getBody(), urlImage);
    }
}




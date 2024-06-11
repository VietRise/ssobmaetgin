package org.cocos2dx.cpp;

import android.os.Build;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.android.play.core.review.ReviewInfo;
import com.google.android.play.core.review.ReviewManager;
import com.google.android.play.core.review.ReviewManagerFactory;
import com.google.android.play.core.review.testing.FakeReviewManager;
import com.google.android.play.core.tasks.OnCompleteListener;
import com.google.android.play.core.tasks.OnFailureListener;
import com.google.android.play.core.tasks.Task;

public class StoreService {

    public static StoreService instance = null;
    private static final String TAG = "StoreService";
    private AppActivity _activity;

    public StoreService() {
        instance = this;
    }

    public static int _ratingState = 0;
    public static String _ratingCondition = "";

    public void create(AppActivity activity) {
        _activity = activity;
    }

    public void displayInAppReview() {
        try {
            if (Build.VERSION.SDK_INT >= 21) {
                if (_activity != null) {
                    ReviewManager reviewManager = ReviewManagerFactory.create(_activity);
//                  ReviewManager reviewManager = new FakeReviewManager(_activity);
                    Task<ReviewInfo> request = reviewManager.requestReviewFlow();
                    request.addOnCompleteListener(task -> {
                        if (task.isSuccessful()) {
                            try {
                                // We can get the ReviewInfo object
                                ReviewInfo reviewInfo = task.getResult();
                                if (reviewInfo != null) {
                                    FirebaseAnalyticsController.logShowInAppReview(StoreService._ratingState, StoreService._ratingCondition);
                                    reviewManager.launchReviewFlow(_activity, reviewInfo).addOnFailureListener(new OnFailureListener() {
                                        @Override
                                        public void onFailure(Exception e) {
                                            Log.d(TAG, e.getMessage());
                                            FirebaseCrashlyticsController.recordException(e);
                                        }
                                    }).addOnCompleteListener(new OnCompleteListener<Void>() {
                                        @Override
                                        public void onComplete(@NonNull Task<Void> task) {
//                                        StoreService.completedAppReview();
                                        }
                                    });
                                }
                            } catch (Exception ex) {
                                Log.d(TAG, ex.getMessage());
                                FirebaseCrashlyticsController.recordException(ex);
                            }
                        } else {
                            Log.d(TAG, task.getException().getMessage());
                            FirebaseCrashlyticsController.recordException(task.getException());
                        }
                    });
                }
            } else {
            }
        } catch (Exception ex) {
            Log.d(TAG, ex.getMessage());
            FirebaseCrashlyticsController.recordException(ex);
        }
    }

    public static void showInAppReview(int state, String condition) {
        StoreService._ratingState = state;
        StoreService._ratingCondition = condition;
        if (StoreService.instance != null)
            StoreService.instance.displayInAppReview();
    }

    public static native void completedAppReview();
}

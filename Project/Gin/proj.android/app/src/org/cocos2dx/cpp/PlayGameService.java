package org.cocos2dx.cpp;

import android.app.AlertDialog;
import android.content.Intent;
import android.util.Log;

import com.emagssob.gin.R;
import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesCallbackStatusCodes;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.games.GamesClientStatusCodes;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;

import org.cocos2dx.lib.Cocos2dxHelper;

import androidx.annotation.NonNull;

public class PlayGameService {

    public static final String USERDEFAULT_GGPGS_SIGNED_IN = "ggpgs_signedin";
    public static PlayGameService instance = null;

    private static final String TAG = "playgame";
    // Request code used to invoke sign in user interactions.
    private static final int RC_SIGN_IN = 9001;

    // Client used to sign in with Google APIs
    private GoogleSignInClient _googleSignInClient = null;

    // The currently signed in account, used to check the account has changed outside of this activity when resuming.
    GoogleSignInAccount mSignedInAccount = null;

    AppActivity _activity;

    int _showLeaderboardId = 0;

    public PlayGameService() {
        instance = this;
    }

    public void create(AppActivity activity) {
        _activity = activity;
        try {
            GoogleSignInOptions signInOptions = GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN;
            // Create the client used to sign in.
            _googleSignInClient = GoogleSignIn.getClient(_activity,
                    signInOptions);
        } catch (Exception ex) {
        }
    }

    /**
     * Start a sign in activity.  To properly handle the result, call tryHandleSignInResult from
     * your Activity's onActivityResult function
     */
    public void startSignInIntent() {
        if (isAvaiable()) {
            try {
                if (checkPlayServices()) {
                    _activity.startActivityForResult(_googleSignInClient.getSignInIntent(), RC_SIGN_IN);
                }
            } catch (Exception ex) {
            }
        }
    }

    private boolean checkPlayServices() {
        GoogleApiAvailability apiAvailability = GoogleApiAvailability.getInstance();
        int resultCode = apiAvailability.isGooglePlayServicesAvailable(_activity);
        if (resultCode != ConnectionResult.SUCCESS) {
            if (apiAvailability.isUserResolvableError(resultCode)) {
                signInSilently();
            }
            return false;
        }
        return true;
    }

    /**
     * Try to sign in without displaying dialogs to the user.
     * <p>
     * If the user has already signed in previously, it will not show dialog.
     */
    public void signInSilently() {
        try {
            if (isAvaiable()) {
                GoogleSignInOptions signInOptions = GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN;
                GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
                if (GoogleSignIn.hasPermissions(account, signInOptions.getScopeArray())) {
                    // Already signed in.
                    // The signed in account is stored in the 'account' variable.
                    onConnected(account);
                } else {
                    // Haven't been signed-in before. Try the silent sign-in first.
                    _googleSignInClient.silentSignIn().addOnCompleteListener(_activity,
                            new OnCompleteListener<GoogleSignInAccount>() {
                                @Override
                                public void onComplete(@NonNull Task<GoogleSignInAccount> task) {
                                    if (task.isSuccessful()) {
                                        Log.d(TAG, "signInSilently(): success");
                                        onConnected(task.getResult());
                                    } else {
                                        Log.d(TAG, "signInSilently(): failure", task.getException());
                                        onDisconnected();
                                    }
                                }
                            });
                }
            }
        } catch (Exception ex) {
        }
    }

    public void signOut() {
        if (isAvaiable()) {
            Log.d(TAG, "signOut()");
            _googleSignInClient.signOut().addOnCompleteListener(_activity,
                    new OnCompleteListener<Void>() {
                        @Override
                        public void onComplete(@NonNull Task<Void> task) {
                            if (task.isSuccessful()) {
                                Log.d(TAG, "signOut(): success");
                            }
                            onDisconnected();
                        }
                    });
        }
    }

    private void onConnected(GoogleSignInAccount googleSignInAccount) {
        if (isAvaiable()) {
            Cocos2dxHelper.setBoolForKey(USERDEFAULT_GGPGS_SIGNED_IN, true);
            Log.d(TAG, "onConnected(): connected to Google APIs");
            if (mSignedInAccount != googleSignInAccount) {
                mSignedInAccount = googleSignInAccount;
                Cocos2dxHelper.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        PlayGameService.onPlayGameServiceSignedIn();
                    }
                });
            }
        }
    }

    private void onDisconnected() {
        if (isAvaiable()) {
            Cocos2dxHelper.setBoolForKey(USERDEFAULT_GGPGS_SIGNED_IN, false);
            Log.d(TAG, "onDisconnected()");
            mSignedInAccount = null;
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    PlayGameService.onPlayGameServiceSignedOut();
                }
            });
        }
    }

    /**
     * Since a lot of the operations use tasks, we can use a common handler for whenever one fails.
     *
     * @param exception The exception to evaluate.  Will try to display a more descriptive reason for the exception.
     * @param details   Will display alongside the exception if you wish to provide more details for why the exception
     *                  happened
     */
    private void handleException(Exception exception, String details) {
        if (isAvaiable()) {
            int status = 0;

            if (exception instanceof ApiException) {
                ApiException apiException = (ApiException) exception;
                status = apiException.getStatusCode();
            }

            String errorString = null;
            switch (status) {
                case GamesCallbackStatusCodes.OK:
                    break;
                default:
                    errorString = _activity.getString(R.string.unexpected_status, GamesClientStatusCodes.getStatusCodeString(status));
                    break;
            }

            if (errorString == null) {
                return;
            }

            String message = _activity.getString(R.string.status_exception_error, details, status, exception);
            new AlertDialog.Builder(_activity)
                    .setTitle("Error")
                    .setMessage(message + "\n" + errorString)
                    .setNeutralButton(android.R.string.ok, null)
                    .show();
        }
    }

    public void onResume() {
        boolean signedIn = Cocos2dxHelper.getBoolForKey(USERDEFAULT_GGPGS_SIGNED_IN, false);
        if (signedIn) {
            signInSilently();
        }
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (isAvaiable()) {
            if (requestCode == RC_SIGN_IN && data != null) {
                GoogleSignInResult result = Auth.GoogleSignInApi.getSignInResultFromIntent(data);
                if (result.isSuccess()) {
                    // The signed in account is stored in the result.
                    GoogleSignInAccount signedInAccount = result.getSignInAccount();
                    onConnected(signedInAccount);
                    if (_showLeaderboardId != 0) {
                        displayLeaderBoard(_showLeaderboardId);
                    }
                } else {
                    onDisconnected();
                }
            }
        }
    }

    private static final int RC_LEADERBOARD_UI = 9004;

    private void displayLeaderBoard(int type) {
        try {
            if (isAvaiable()) {
                _showLeaderboardId = 0;
                if (mSignedInAccount != null) {
                    String id = getLeaderboardId(type);
                    if (!id.isEmpty()) {
                        Games.getLeaderboardsClient(_activity, mSignedInAccount)
                                .getLeaderboardIntent(id)
                                .addOnSuccessListener(new OnSuccessListener<Intent>() {
                                    @Override
                                    public void onSuccess(Intent intent) {
                                        _activity.startActivityForResult(intent, RC_LEADERBOARD_UI);
                                    }
                                });
                    }
                } else {
                    _showLeaderboardId = type;
                    startSignInIntent();
                }
            }
        } catch (Exception ex) {
        }
    }

    public void submitMoney(double money) {
        try {
            if (isAvaiable()) {
                if (mSignedInAccount != null) {
                    Games.getLeaderboardsClient(_activity, mSignedInAccount)
                            .submitScoreImmediate(getLeaderboardId(1), (long) money);
                    Games.getLeaderboardsClient(_activity, mSignedInAccount)
                            .submitScoreImmediate(getLeaderboardId(2), (long) money);
                }
            }
        } catch (Exception ex) {
        }
    }

    public void submitLevel(double level) {
        try {
            if (isAvaiable()) {
                if (mSignedInAccount != null) {
                    Games.getLeaderboardsClient(_activity, mSignedInAccount)
                            .submitScoreImmediate(getLeaderboardId(3), (long) level);
                }
            }
        } catch (Exception ex) {
        }
    }

    public boolean isSigned() {
        if (isAvaiable()) {
            if (mSignedInAccount != null) {
                return true;
            }
        }
        return false;
    }

    public boolean isAvaiable() {
        if (_googleSignInClient != null) {
            return true;
        }
        return false;
    }

    public static void submitLeaderBoardMoney(double score) {
        try {
            if (PlayGameService.instance != null) {
                PlayGameService.instance.submitMoney(score);
            }
        } catch (Exception ex) {
        }
    }

    public static void submitLeaderBoardLevel(double level) {
        try {
            if (PlayGameService.instance != null) {
                PlayGameService.instance.submitLevel(level);
            }
        } catch (Exception ex) {
        }
    }

    public static void signinPlayGameService() {
        try {
            if (PlayGameService.instance != null) {
                PlayGameService.instance.startSignInIntent();
            }
        } catch (Exception ex) {
        }
    }

    public static void signoutPlayGameService() {
        try {
            if (PlayGameService.instance != null) {
                PlayGameService.instance.signOut();
            }
        } catch (Exception ex) {
        }
    }

    public static void showLeaderBoardBillionaireClub() {
        try {
            if (PlayGameService.instance != null) {
                PlayGameService.instance.displayLeaderBoard(1);
            }
        } catch (Exception ex) {
        }
    }

    public static void showLeaderBoardMillionaireClub() {
        try {
            if (PlayGameService.instance != null) {
                PlayGameService.instance.displayLeaderBoard(2);
            }
        } catch (Exception ex) {
        }
    }

    public static void showLeaderBoardLevel() {
        try {
            if (PlayGameService.instance != null) {
                PlayGameService.instance.displayLeaderBoard(3);
            }
        } catch (Exception ex) {
        }
    }

    public static boolean isSignedIn() {
        try {
            if (PlayGameService.instance != null) {
                return PlayGameService.instance.isSigned();
            }
        } catch (Exception ex) {
        }
        return false;
    }

    public static boolean havePlayGameService() {
        if (PlayGameService.instance != null) {
            return PlayGameService.instance.isAvaiable();
        }
        return false;
    }

    public String getLeaderboardId(int type) {
        try {
            if (type == 1) {
                return FirebaseRC.getString("leaderboard_billionaire_id", _activity.getString(R.string.leaderboard__1_id));
            } else if (type == 2) {
                return FirebaseRC.getString("leaderboard_millionaire_id", _activity.getString(R.string.leaderboard__2_id));
            } else if (type == 3) {
                return FirebaseRC.getString("leaderboard_level_id", _activity.getString(R.string.leaderboard__3_id));
            }
        } catch (Exception ex) {
        }
        if (type == 1) {
            return _activity.getString(R.string.leaderboard__1_id);
        } else if (type == 2) {
            return _activity.getString(R.string.leaderboard__2_id);
        } else if (type == 3) {
            return _activity.getString(R.string.leaderboard__3_id);
        }
        return "";
    }

    public static native void onPlayGameServiceSignedIn();

    public static native void onPlayGameServiceSignedOut();
}

package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;

import com.google.firebase.analytics.FirebaseAnalytics;

public class FirebaseAnalyticsController {

    public static String EVENT_GET_MONEY_SCENE_ID = "scene_id";
    public static String EVENT_GET_MONEY_DEFAULT = "get_money_default";
    public static String EVENT_GET_MONEY_DEFAULT_MONEY = "get_money";
    public static String EVENT_GET_MONEY_WATCH_VIDEO = "get_money_watch_video";
    public static String EVENT_GET_MONEY_WATCH_VIDEO_MONEY_GETED = "get_money";
    public static String EVENT_GET_MONEY_WATCH_VIDEO_ORDER = "video_order";
    public static String EVENT_GET_MONEY_WELCOMEBACK = "get_money_welcomeback";
    public static String EVENT_GET_MONEY_WELCOMEBACK_DAY = "day";
    public static String EVENT_GET_MONEY_WELCOMEBACK_MONEY = "get_money";
    public static String EVENT_OPEN_APP_RATING = "open_app_rating";
    public static String EVENT_OPEN_WATCH_VIDEO = "open_watch_video";
    public static String EVENT_OPEN_WATCH_VIDEO_STATE = "open_watch_video_state";
    public static String EVENT_OPEN_TUTORIAL = "open_tutorial";

    public static String EVENT_CONTINUE_TO_PLAY = "continue_play";
    public static String EVENT_CONTINUE_TO_PLAY_BET = "continue_play_bet";
    public static String EVENT_CONTINUE_TO_PLAY_RATIO = "continue_play_ratio";

    public static String EVENT_UNLOCK_CITY = "unlock_city";
    public static String EVENT_UNLOCK_CITY_TYPE = "city_type";
    public static String EVENT_UNLOCK_CITY_MONEY = "curr_money";

    public static String EVENT_UNLOCK_TABLE = "unlock_table";
    public static String EVENT_UNLOCK_TABLE_TYPE = "table_type";
    public static String EVENT_UNLOCK_TABLE_MONEY = "curr_money";

    public static String EVENT_START_MATCH = "start_match";
    public static String EVENT_START_MATCH_BET = "start_match_bet";
    public static String EVENT_START_MATCH_CITY_TYPE = "start_match_city_type";
    public static String EVENT_START_MATCH_TABLE_TYPE = "start_match_table_type";
    public static String EVENT_START_MATCH_RATIO = "start_match_ratio";
    public static String EVENT_START_MATCH_SPEED = "start_match_speed";
    public static String EVENT_START_MATCH_ORDER = "start_match_order";
    public static String EVENT_START_MATCH_MONEY = "start_match_money";
    public static String EVENT_START_MATCH_RULE = "start_match_rule";
    public static String EVENT_OPEN_APP = "open_app";

    public static String EVENT_WIN_MATCH = "win_match";
    public static String EVENT_LOSE_MATCH = "lose_match";
    public static String EVENT_RESULT_TIER = "result_tier";
    public static String EVENT_RESULT_TYPE = "result_type";
    public static String EVENT_RESULT_CLAIMED_MONEY = "result_money_claimed";
    public static String EVENT_RESULT_CURRENT_MONEY = "result_money_curr";
    public static String EVENT_RESULT_BET = "result_bet";
    public static String EVENT_RESULT_CITY_TYPE = "city_type";
    public static String EVENT_RESULT_MATCH_COUNT = "match_count";
    public static String EVENT_RESULT_SCORE = "score";
    public static String EVENT_RESULT_BONUS_CARDS = "num_bonus_cards";
    public static String EVENT_RESULT_SECRETS = "num_secrets";
	public static String EVENT_SHOW_FULL_ADS = "show_full_ads";

	public static String EVENT_WIN_HIT_POT = "win_hit_pot";
	public static String EVENT_WIN_HIT_POT_USER = "user";
    public static String EVENT_WIN_HIT_POT_ROUND = "round";
    public static String EVENT_WIN_HIT_POT_ROUND_TOTAL = "round_total";
    public static String EVENT_WIN_HIT_POT_MONEY_USER = "money_user";
    public static String EVENT_WIN_HIT_POT_MONEY_HITPOT = "money_hitpot";
    public static String EVENT_WIN_HIT_POT_MONEY_BET = "money_bet";

    public static String EVENT_USER_DO_ACTION = "user_do_action";
    public static String EVENT_USER_DO_ACTION_TYPE = "type";
    public static String EVENT_USER_DO_ACTION_SCORE = "score";

    public static String EVENT_CLICK_SORT_CARD = "click_sort_card";
    public static String EVENT_CLICK_SORT_CARD_TYPE = "type";

    public static String EVENT_CLICK_VIEW_RESULT = "click_view_result";

    public static String EVENT_USER_PROPERTIES_MONEY = "Money";
    public static String EVENT_USER_PROPERTIES_MONEY_RANGE = "MoneyRange";
    public static String EVENT_USER_PROPERTIES_LEVEL = "Level";
    public static String EVENT_USER_PROPERTIES_GAME_VERSION = "GameVersion";
    public static String EVENT_USER_PROPERTIES_VIDEO_COUNT = "VideoCount";
    public static String EVENT_USER_PROPERTIES_WIN_NUMBER = "WinNumber";
    public static String EVENT_USER_PROPERTIES_LOSE_NUMBER = "LoseNumber";
    public static String EVENT_USER_PROPERTIES_WIN_RATE = "WinRate";
    public static String EVENT_USER_PROPERTIES_TOTAL_MATCH = "MatchNumber";
    public static String EVENT_USER_PROPERTIES_WIN_HITPOT_NUMBER = "WinHitpotNumber";

    public static String EVENT_SELECT_CONTENT_TYPE_GET_MONEY = "GetMoney";
    public static String EVENT_SELECT_CONTENT_TYPE_ITEM_ID_DEFAULT = "Default";
    public static String EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WATCH_VIDEO = "WatchVideo";
    public static String EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WELCOME_BACK = "WelcomeBack";
    public static String EVENT_SELECT_CONTENT_TYPE_ITEM_ID_TIME_BONUS = "TimeBonus";

    public static String EVENT_SELECT_POST_SCORE_MATCH_RESULT = "MatchResult";
    public static String EVENT_SELECT_POST_SCORE_WIN = "Win";
    public static String EVENT_SELECT_POST_SCORE_LOSE = "Lose";

    public static String EVENT_OPEN_RATING_POPUP = "show_rating";
    public static String EVENT_CLICK_BUTTON_RATING_POPUP = "click_rating";
    public static String EVENT_STAR_RATING_POPUP = "rate_star";
    public static String EVENT_RATING_POPUP_PARAM_STATE = "StateRatingPopup";
    public static String EVENT_RATING_POPUP_PARAM_CONDITION = "Condition";
    public static String EVENT_RATING_POPUP_PARAM_PLACE = "Place";
    public static String EVENT_RATING_POPUP_PARAM_BUTTON = "Button";
    public static String EVENT_RATING_POPUP_PARAM_STAR = "Star";
    public static String EVENT_RATING_POPUP_PARAM_STAR_STRING = "StarString";

    public static String EVENT_GAME_CENTER_ACTION = "game_center_action";
    public static String EVENT_GAME_CENTER_PARAM_GAME = "game";
    public static String EVENT_GAME_CENTER_PARAM_ACTION = "action";
    public static String EVENT_GAME_CENTER_PARAM_PLAY_GAME = "play_game";
    public static String EVENT_GAME_CENTER_PARAM_INSTALL_GAME = "install_game";

    public static String EVENT_TIME_BONUS = "claim_time_bonus";
    public static String EVENT_TIME_BONUS_STEP = "claim_time_bonus_step";
    public static String EVENT_TIME_BONUS_VALUE = "claim_time_bonus_value";
    public static String EVENT_TIME_BONUS_WATCHED_VIDEO_REWARD = "claim_time_bonus_watched_video_reward";
    public static String EVENT_TIME_BONUS_STEP_PREFIX = "step_";
    public static String EVENT_TIME_BONUS_PARAM_LV = "reward_level";
    public static String EVENT_TIME_BONUS_LV_PREFIX = "lv_";

    public static String EVENT_CLAIM_MAILBOX_REWARD = "claim_mail_box_reward";
    public static String EVENT_MAILBOX_PARAM_ID = "mailbox_id";
    public static String EVENT_MAILBOX_PARAM_REWARD_MONEY = "reward_money";

    public static String EVENT_CHANGE_AVATAR = "change_avatar";
    public static String EVENT_CHANGE_AVATAR_PARAM_NAME = "avatar_name";

    public static String EVENT_RECEIVE_LOCAL_NOTIFICATION = "receive_local_notification";
    public static String EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_NAME = "name";
    public static String EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_ID = "id";
    public static String EVENT_OPEN_LOCAL_NOTIFICATION = "open_local_notification";
    public static String EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_NAME = "name";
    public static String EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_ID = "id";

    public static String EVENT_AD_MANAGER_INTERSTITIAL_IMPRESSION = "ad_manager_interstitial_impression";
    public static String EVENT_AD_MANAGER_INTERSTITIAL_MEDIATION_NAME = "mediation";

    public static String EVENT_AD_MANAGER_VIDEO_IMPRESSION = "ad_manager_video_impression";
    public static String EVENT_AD_MANAGER_VIDEO_MEDIATION_NAME = "mediation";

    public static String EVENT_AD_MANAGER_LOAD_FAIL = "ad_load_fail";
    public static String EVENT_AD_MANAGER_LOAD_FAIL_NAME = "name";
    public static String EVENT_AD_MANAGER_LOAD_FAIL_ERROR_CODE = "errorCode";
    public static String EVENT_AD_MANAGER_LOAD_FAIL_ERROR_MSG = "errorMsg";
    public static String EVENT_AD_MANAGER_LOAD_FAIL_DOMAIN = "domain";
    public static String EVENT_AD_MANAGER_LOAD_FAIL_CAUSE = "cause";

    public static String EVENT_NETWORK = "network";
    public static String EVENT_NETWORK_STATUS = "status";
    public static String EVENT_NETWORK_ONLINE = "online";
    public static String EVENT_NETWORK_OFFLINE = "offline";

    public static String EVENT_SHOW_INAPP_REVIEW = "show_inapp_review";
    public static String EVENT_SHOW_INAPP_REVIEW_COMPLETED = "show_inapp_review_completed";

    public static Activity activity;
    public static FirebaseAnalytics firebaseAnalytics = null;

    public static void onCreate(Activity act) {
        activity = act;
        firebaseAnalytics = FirebaseAnalytics.getInstance(activity);
    }

    public static void logGetMoneyDefault(String sceneId, float money) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putFloat(EVENT_GET_MONEY_DEFAULT_MONEY, money);
                params.putString(EVENT_GET_MONEY_SCENE_ID, sceneId);
                params.putInt(FirebaseAnalytics.Param.VALUE, 1);
                firebaseAnalytics.logEvent(EVENT_GET_MONEY_DEFAULT, params);


                Bundle params2 = new Bundle();
                params2.putString(FirebaseAnalytics.Param.CONTENT_TYPE, EVENT_SELECT_CONTENT_TYPE_GET_MONEY);
                params2.putString(FirebaseAnalytics.Param.ITEM_ID, EVENT_SELECT_CONTENT_TYPE_ITEM_ID_DEFAULT);
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.SELECT_CONTENT, params2);
            }
        } catch (Exception e) {
        }
    }

    public static void logGetMoneyWatchVideo(String sceneId, int orderId, float money) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putFloat(EVENT_GET_MONEY_WATCH_VIDEO_MONEY_GETED, money);
                params.putInt(EVENT_GET_MONEY_WATCH_VIDEO_ORDER, orderId);
                params.putString(EVENT_GET_MONEY_SCENE_ID, sceneId);
                params.putInt(FirebaseAnalytics.Param.VALUE, 1);
                firebaseAnalytics.logEvent(EVENT_GET_MONEY_WATCH_VIDEO, params);

                Bundle params2 = new Bundle();
                params2.putString(FirebaseAnalytics.Param.CONTENT_TYPE, EVENT_SELECT_CONTENT_TYPE_GET_MONEY);
                params2.putString(FirebaseAnalytics.Param.ITEM_ID, EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WATCH_VIDEO);
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.SELECT_CONTENT, params2);
            }
        } catch (Exception e) {
        }
    }

    public static void logGetMoneyWelcomeBack(String sceneId, int day, float money) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putFloat(EVENT_GET_MONEY_WELCOMEBACK_MONEY, money);
                params.putString(EVENT_GET_MONEY_WELCOMEBACK_DAY, "Day_" + day);
                params.putString(EVENT_GET_MONEY_SCENE_ID, sceneId);
                params.putInt(FirebaseAnalytics.Param.VALUE, 1);
                firebaseAnalytics.logEvent(EVENT_GET_MONEY_WELCOMEBACK, params);

                Bundle params2 = new Bundle();
                params2.putString(FirebaseAnalytics.Param.CONTENT_TYPE, EVENT_SELECT_CONTENT_TYPE_GET_MONEY);
                params2.putString(FirebaseAnalytics.Param.ITEM_ID, EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WELCOME_BACK);
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.SELECT_CONTENT, params2);
            }
        } catch (Exception e) {
        }
    }

    public static void logOpenAppRating() {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                firebaseAnalytics.logEvent(EVENT_OPEN_APP_RATING, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logOpenWatchVideo(String state) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();

                params.putString(EVENT_OPEN_WATCH_VIDEO_STATE, state);
                firebaseAnalytics.logEvent(EVENT_OPEN_WATCH_VIDEO, params);

            }
        } catch (Exception e) {
        }
    }

    public static void logOpenTutorial() {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                firebaseAnalytics.logEvent(EVENT_OPEN_TUTORIAL, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logContinueToPlay(double bet, float ratio) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putDouble(EVENT_CONTINUE_TO_PLAY_BET, bet);
                params.putFloat(EVENT_CONTINUE_TO_PLAY_RATIO, ratio);
                firebaseAnalytics.logEvent(EVENT_CONTINUE_TO_PLAY, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logUnlockCity(String type, double money) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_UNLOCK_CITY_TYPE, type);
                params.putDouble(EVENT_UNLOCK_CITY_MONEY, money);
                firebaseAnalytics.logEvent(EVENT_UNLOCK_CITY, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logUnlockTable(String type, double money) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_UNLOCK_TABLE_TYPE, type);
                params.putDouble(EVENT_UNLOCK_TABLE_MONEY, money);
                firebaseAnalytics.logEvent(EVENT_UNLOCK_TABLE, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logStartMatch(String cityType, String tableType, double money, double bet, float ratio, float speed, int orderId) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_START_MATCH_CITY_TYPE, cityType);
                params.putString(EVENT_START_MATCH_TABLE_TYPE, tableType);
                params.putDouble(EVENT_START_MATCH_MONEY, money);
                params.putDouble(EVENT_START_MATCH_BET, bet);
                params.putFloat(EVENT_START_MATCH_RATIO, ratio);
                params.putFloat(EVENT_START_MATCH_SPEED, speed);
                params.putInt(EVENT_START_MATCH_ORDER, orderId);
                firebaseAnalytics.logEvent(EVENT_START_MATCH, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logUserProperties(String gameVersion, String moneyName, int winNumber, int loseNumber, int winHitpotNumber, int level) {
        try {
            if (firebaseAnalytics != null) {
                firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_MONEY_RANGE, moneyName);
                firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_GAME_VERSION, gameVersion);
                firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_WIN_NUMBER, Integer.toString(winNumber));
                firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_LOSE_NUMBER, Integer.toString(loseNumber));
                firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_TOTAL_MATCH, Integer.toString(winNumber + loseNumber));
                firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_WIN_HITPOT_NUMBER, Integer.toString(winHitpotNumber));
				firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_LEVEL, Integer.toString(level));

                if (winNumber + loseNumber > 0) {
                    firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_WIN_RATE, Float.toString((winNumber * 1.0f / (winNumber + loseNumber)) * 100.0f));
                }
            }
        } catch (Exception e) {
        }
    }

    public static void logWinResult(int tier, String type, double money, double currMoney, double bet, String cityType, int matchCount, int score, int numBonusCard, int numSecret) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putInt(EVENT_RESULT_TIER, tier);
                params.putString(EVENT_RESULT_TYPE, type);
                params.putDouble(EVENT_RESULT_CLAIMED_MONEY, money);
                params.putDouble(EVENT_RESULT_CURRENT_MONEY, currMoney);
                params.putDouble(EVENT_RESULT_BET, bet);
                params.putString(EVENT_RESULT_CITY_TYPE, cityType);
                params.putInt(EVENT_RESULT_MATCH_COUNT, matchCount);
                params.putInt(EVENT_RESULT_SCORE, score);
                params.putInt(EVENT_RESULT_BONUS_CARDS, numBonusCard);
                params.putInt(EVENT_RESULT_SECRETS, numSecret);
                firebaseAnalytics.logEvent(EVENT_WIN_MATCH, params);

                Bundle params2 = new Bundle();
                params2.putString(FirebaseAnalytics.Param.LEVEL, EVENT_SELECT_POST_SCORE_MATCH_RESULT);
                params2.putString(FirebaseAnalytics.Param.CHARACTER, EVENT_SELECT_POST_SCORE_WIN);
                params2.putString(FirebaseAnalytics.Param.SCORE, EVENT_SELECT_POST_SCORE_WIN);
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.POST_SCORE, params2);
            }
        } catch (Exception e) {
        }
    }

    public static void logLoseResult(int tier, String type, double money, double currMoney, double bet, String cityType, int matchCount, int score, int numBonusCard, int numSecret) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putInt(EVENT_RESULT_TIER, tier);
                params.putString(EVENT_RESULT_TYPE, type);
                params.putDouble(EVENT_RESULT_CLAIMED_MONEY, money);
                params.putDouble(EVENT_RESULT_CURRENT_MONEY, currMoney);
                params.putDouble(EVENT_RESULT_BET, bet);
                params.putString(EVENT_RESULT_CITY_TYPE, cityType);
                params.putInt(EVENT_RESULT_MATCH_COUNT, matchCount);
                params.putInt(EVENT_RESULT_SCORE, score);
                params.putInt(EVENT_RESULT_BONUS_CARDS, numBonusCard);
                params.putInt(EVENT_RESULT_SECRETS, numSecret);
                firebaseAnalytics.logEvent(EVENT_LOSE_MATCH, params);

                Bundle params2 = new Bundle();
                params2.putString(FirebaseAnalytics.Param.LEVEL, EVENT_SELECT_POST_SCORE_MATCH_RESULT);
                params2.putString(FirebaseAnalytics.Param.CHARACTER, EVENT_SELECT_POST_SCORE_LOSE);
                params2.putString(FirebaseAnalytics.Param.SCORE, EVENT_SELECT_POST_SCORE_LOSE);
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.POST_SCORE, params2);
            }
        } catch (Exception e) {
        }
    }

    public static void logWinHitPot(int slotOrder, int round, double moneyUser, double moneyHitpot, double moneyBet)
    {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_WIN_HIT_POT_USER, "user_" + slotOrder);
                params.putInt(EVENT_WIN_HIT_POT_ROUND, round);
                params.putString(EVENT_WIN_HIT_POT_ROUND_TOTAL, "round_" + round);
                params.putDouble(EVENT_WIN_HIT_POT_MONEY_USER, moneyUser);
                params.putDouble(EVENT_WIN_HIT_POT_MONEY_HITPOT, moneyHitpot);
                params.putDouble(EVENT_WIN_HIT_POT_MONEY_BET, moneyBet);
                firebaseAnalytics.logEvent(EVENT_WIN_HIT_POT, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logDoAction(String type, int score)
    {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_USER_DO_ACTION_TYPE, type);
                params.putInt(EVENT_USER_DO_ACTION_SCORE, score);
                firebaseAnalytics.logEvent(EVENT_USER_DO_ACTION, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logClickSortCard(String type)
    {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_CLICK_SORT_CARD_TYPE, type);
                firebaseAnalytics.logEvent(EVENT_CLICK_SORT_CARD, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logClickViewResult()
    {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                firebaseAnalytics.logEvent(EVENT_CLICK_VIEW_RESULT, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logTotalWatchVideoNumber(int number) {
        try {
            if (firebaseAnalytics != null) {
                firebaseAnalytics.setUserProperty(EVENT_USER_PROPERTIES_VIDEO_COUNT, Integer.toString(number));
            }
        } catch (Exception e) {
        }
    }

    public static void logShowFullAds() {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                firebaseAnalytics.logEvent(EVENT_SHOW_FULL_ADS, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logOpenApp(String appName) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_OPEN_APP, appName);
                firebaseAnalytics.logEvent(EVENT_OPEN_APP, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logOpenRatingPopup(int stateRatingPopup, String conditionName) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                String state = "Show_" + stateRatingPopup;
                params.putString(EVENT_RATING_POPUP_PARAM_STATE, state);
                params.putString(EVENT_RATING_POPUP_PARAM_CONDITION, conditionName);
                firebaseAnalytics.logEvent(EVENT_OPEN_RATING_POPUP, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logClickButtonRatingPopup(int stateRatingPopup, String conditionName, int star, String btnClickName, String where) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                String state = "Show_" + stateRatingPopup;
                params.putString(EVENT_RATING_POPUP_PARAM_STATE, state);
                params.putString(EVENT_RATING_POPUP_PARAM_CONDITION, conditionName);
                params.putInt(EVENT_RATING_POPUP_PARAM_STAR, star);
                params.putString(EVENT_RATING_POPUP_PARAM_STAR_STRING, "Star_" + star);
                params.putString(EVENT_RATING_POPUP_PARAM_BUTTON, btnClickName);
                params.putString(EVENT_RATING_POPUP_PARAM_PLACE, where);
                firebaseAnalytics.logEvent(EVENT_CLICK_BUTTON_RATING_POPUP, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logClickStarPopup(int stateRatingPopup, String conditionName, int star) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                String state = "Show_" + stateRatingPopup;
                params.putString(EVENT_RATING_POPUP_PARAM_STATE, state);
                params.putString(EVENT_RATING_POPUP_PARAM_CONDITION, conditionName);
                params.putInt(EVENT_RATING_POPUP_PARAM_STAR, star);
                params.putString(EVENT_RATING_POPUP_PARAM_STAR_STRING, "Star_" + star);
                firebaseAnalytics.logEvent(EVENT_STAR_RATING_POPUP, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logGameCenterAction(String game, int state) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_GAME_CENTER_PARAM_GAME, game);
                String action = "";
                if (state == 0) {
                    action = "Install";
                    params.putString(EVENT_GAME_CENTER_PARAM_INSTALL_GAME, action + "_" + game);
                } else if (state == 2) {
                    action = "Play";
                    params.putString(EVENT_GAME_CENTER_PARAM_PLAY_GAME, action + "_" + game);
                }
                params.putString(EVENT_GAME_CENTER_PARAM_ACTION, action);
                firebaseAnalytics.logEvent(EVENT_GAME_CENTER_ACTION, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logClaimTimeBonus(int step, double value, boolean watchedVideoReward, int lv) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_TIME_BONUS_STEP, EVENT_TIME_BONUS_STEP_PREFIX + step);
                params.putString(EVENT_TIME_BONUS_PARAM_LV, EVENT_TIME_BONUS_LV_PREFIX + lv);
                params.putDouble(EVENT_TIME_BONUS_VALUE, value);
                if (watchedVideoReward) {
                    params.putString(EVENT_TIME_BONUS_WATCHED_VIDEO_REWARD, EVENT_TIME_BONUS_STEP_PREFIX + step);
                }
                firebaseAnalytics.logEvent(EVENT_TIME_BONUS, params);

                Bundle params2 = new Bundle();
                params2.putString(FirebaseAnalytics.Param.CONTENT_TYPE, EVENT_SELECT_CONTENT_TYPE_GET_MONEY);
                params2.putString(FirebaseAnalytics.Param.ITEM_ID, EVENT_SELECT_CONTENT_TYPE_ITEM_ID_TIME_BONUS);
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.SELECT_CONTENT, params2);
            }
        } catch (Exception e) {
        }
    }

    public static void logClaimMailboxReward(String id, double value) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_MAILBOX_PARAM_ID, id);
                params.putDouble(EVENT_MAILBOX_PARAM_REWARD_MONEY, value);
                firebaseAnalytics.logEvent(EVENT_CLAIM_MAILBOX_REWARD, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logChangeAvatar(String name) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_CHANGE_AVATAR_PARAM_NAME, name);
                firebaseAnalytics.logEvent(EVENT_CHANGE_AVATAR, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logSpendMoney(String itemName, double amount) {
        try {
            if (firebaseAnalytics != null) {
                Bundle bundle = new Bundle();
                bundle.putString(FirebaseAnalytics.Param.ITEM_NAME, itemName);
                bundle.putDouble(FirebaseAnalytics.Param.VALUE, amount);
                bundle.putString(FirebaseAnalytics.Param.VIRTUAL_CURRENCY_NAME, "Money");
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.SPEND_VIRTUAL_CURRENCY, bundle);
            }
        } catch (Exception e) {
        }
    }

    public static void logEarnMoney(String itemName, double amount) {
        try {
            if (firebaseAnalytics != null) {
                Bundle bundle = new Bundle();
                bundle.putString(FirebaseAnalytics.Param.ITEM_NAME, itemName);
                bundle.putDouble(FirebaseAnalytics.Param.VALUE, amount);
                bundle.putString(FirebaseAnalytics.Param.VIRTUAL_CURRENCY_NAME, "Money");
                firebaseAnalytics.logEvent(FirebaseAnalytics.Event.EARN_VIRTUAL_CURRENCY, bundle);
            }
        } catch (Exception e) {
        }
    }

    public static void logOpenLocalNotification(String name, int id) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_NAME, name);
                params.putString(EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_ID, String.valueOf(id));
                firebaseAnalytics.logEvent(EVENT_OPEN_LOCAL_NOTIFICATION, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logReceiveLocalNotification(Context context, String name, int id) {
        try {
            Bundle params = new Bundle();
            params.putString(EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_NAME, name);
            params.putString(EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_ID, String.valueOf(id));
            FirebaseAnalytics.getInstance(context).logEvent(EVENT_RECEIVE_LOCAL_NOTIFICATION, params);
        } catch (Exception e) {
        }
    }

    public static void logInterstitialAdsImpression(String mediationName) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_AD_MANAGER_INTERSTITIAL_MEDIATION_NAME, mediationName);
                firebaseAnalytics.logEvent(EVENT_AD_MANAGER_INTERSTITIAL_IMPRESSION, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logVideoAdsImpression(String mediationName) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_AD_MANAGER_VIDEO_MEDIATION_NAME, mediationName);
                firebaseAnalytics.logEvent(EVENT_AD_MANAGER_VIDEO_IMPRESSION, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logAdsLoadFail(String name, int errorCode, String errorMsg, String cause, String domain) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                params.putString(EVENT_AD_MANAGER_LOAD_FAIL_NAME, name);
                params.putString(EVENT_AD_MANAGER_LOAD_FAIL_ERROR_CODE, String.valueOf(errorCode));
                params.putString(EVENT_AD_MANAGER_LOAD_FAIL_ERROR_MSG, errorMsg);
                params.putString(EVENT_AD_MANAGER_LOAD_FAIL_CAUSE, cause);
                params.putString(EVENT_AD_MANAGER_LOAD_FAIL_DOMAIN, domain);
                firebaseAnalytics.logEvent(EVENT_AD_MANAGER_LOAD_FAIL, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logNetwork(boolean online) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                String status = EVENT_NETWORK_OFFLINE;
                if (online)
                    status = EVENT_NETWORK_ONLINE;
                params.putString(EVENT_NETWORK_STATUS, status);
                firebaseAnalytics.logEvent(EVENT_NETWORK, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logShowInAppReview(int ratingState, String conditionName) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                String state = "Show_" + ratingState;
                params.putString(EVENT_RATING_POPUP_PARAM_STATE, state);
                params.putString(EVENT_RATING_POPUP_PARAM_CONDITION, conditionName);
                firebaseAnalytics.logEvent(EVENT_SHOW_INAPP_REVIEW, params);
            }
        } catch (Exception e) {
        }
    }

    public static void logShowInAppReviewCompleted(int ratingState, String conditionName) {
        try {
            if (firebaseAnalytics != null) {
                Bundle params = new Bundle();
                String state = "Show_" + ratingState;
                params.putString(EVENT_RATING_POPUP_PARAM_STATE, state);
                params.putString(EVENT_RATING_POPUP_PARAM_CONDITION, conditionName);
                firebaseAnalytics.logEvent(EVENT_SHOW_INAPP_REVIEW_COMPLETED, params);
            }
        } catch (Exception e) {
        }
    }
}

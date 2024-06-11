#import <Foundation/Foundation.h>
@import Firebase;
@import FirebaseAnalytics;
@import FirebaseCore;

#define EVENT_GET_MONEY_SCENE_ID @"scene_id"
#define EVENT_GET_MONEY_DEFAULT @"get_money_default"
#define EVENT_GET_MONEY_DEFAULT_MONEY @"get_money"
#define EVENT_GET_MONEY_WATCH_VIDEO @"get_money_watch_video"
#define EVENT_GET_MONEY_WATCH_VIDEO_MONEY_GETED @"get_money"
#define EVENT_GET_MONEY_WATCH_VIDEO_ORDER @"video_order"
#define EVENT_GET_MONEY_WELCOMEBACK @"get_money_welcomeback"
#define EVENT_GET_MONEY_WELCOMEBACK_DAY @"day"
#define EVENT_GET_MONEY_WELCOMEBACK_MONEY @"get_money"
#define EVENT_OPEN_APP_RATING @"open_app_rating"
#define EVENT_OPEN_WATCH_VIDEO @"open_watch_video"
#define EVENT_OPEN_WATCH_VIDEO_STATE @"open_watch_video_state"
#define EVENT_OPEN_TUTORIAL @"open_tutorial"

#define EVENT_CONTINUE_TO_PLAY @"continue_play"
#define EVENT_CONTINUE_TO_PLAY_BET @"continue_play_bet"
#define EVENT_CONTINUE_TO_PLAY_RATIO @"continue_play_ratio"

#define EVENT_UNLOCK_CITY @"unlock_city"
#define EVENT_UNLOCK_CITY_TYPE @"city_type"
#define EVENT_UNLOCK_CITY_MONEY @"curr_money"

#define EVENT_UNLOCK_TABLE @"unlock_table"
#define EVENT_UNLOCK_TABLE_TYPE @"table_type"
#define EVENT_UNLOCK_TABLE_MONEY @"curr_money"

#define EVENT_START_MATCH @"start_match"
#define EVENT_START_MATCH_BET @"start_match_bet"
#define EVENT_START_MATCH_CITY_TYPE @"start_match_city_type"
#define EVENT_START_MATCH_TABLE_TYPE @"start_match_table_type"
#define EVENT_START_MATCH_RATIO @"start_match_ratio"
#define EVENT_START_MATCH_SPEED @"start_match_speed"
#define EVENT_START_MATCH_ORDER @"start_match_order"
#define EVENT_START_MATCH_RULE @"start_match_rule"
#define EVENT_START_MATCH_MONEY @"start_match_money"
#define EVENT_OPEN_APP @"open_app"

#define EVENT_WIN_MATCH @"win_match"
#define EVENT_LOSE_MATCH  @"lose_match"
#define EVENT_RESULT_TIER  @"result_tier"
#define EVENT_RESULT_TYPE  @"result_type"
#define EVENT_RESULT_CLAIMED_MONEY  @"result_money_claimed"
#define EVENT_RESULT_CURRENT_MONEY  @"result_money_curr"
#define EVENT_RESULT_BET  @"result_bet"
#define EVENT_RESULT_CITY_TYPE @"city_type"
#define EVENT_RESULT_MATCH_COUNT @"match_count"
#define EVENT_RESULT_SCORE @"score"
#define EVENT_RESULT_BONUS_CARDS @"num_bonus_cards"
#define EVENT_RESULT_SECRETS @"num_secrets"
#define EVENT_SHOW_FULL_ADS @"show_full_ads"

#define EVENT_WIN_HIT_POT @"win_hit_pot"
#define EVENT_WIN_HIT_POT_USER @"user"
#define EVENT_WIN_HIT_POT_ROUND @"round"
#define EVENT_WIN_HIT_POT_ROUND_TOTAL @"round_total"
#define EVENT_WIN_HIT_POT_MONEY_USER @"money_user"
#define EVENT_WIN_HIT_POT_MONEY_HITPOT @"money_hitpot"
#define EVENT_WIN_HIT_POT_MONEY_BET @"money_bet"

#define EVENT_USER_DO_ACTION @"user_do_action"
#define EVENT_USER_DO_ACTION_TYPE @"type"
#define EVENT_USER_DO_ACTION_SCORE @"score"

#define EVENT_CLICK_SORT_CARD @"click_sort_card"
#define EVENT_CLICK_SORT_CARD_TYPE @"type"

#define EVENT_CLICK_VIEW_RESULT @"click_view_result"

#define EVENT_USER_PROPERTIES_MONEY @"Money"
#define EVENT_USER_PROPERTIES_MONEY_RANGE @"MoneyRange"
#define EVENT_USER_PROPERTIES_GAME_VERSION @"GameVersion"
#define EVENT_USER_PROPERTIES_VIDEO_COUNT @"VideoCount"
#define EVENT_USER_PROPERTIES_WIN_NUMBER @"WinNumber"
#define EVENT_USER_PROPERTIES_LOSE_NUMBER @"LoseNumber"
#define EVENT_USER_PROPERTIES_WIN_RATE @"WinRate"
#define EVENT_USER_PROPERTIES_TOTAL_MATCH @"MatchNumber"
#define EVENT_USER_PROPERTIES_WIN_HITPOT_NUMBER @"WinHitpotNumber"
#define EVENT_USER_PROPERTIES_LEVEL @"Level"

#define EVENT_SELECT_CONTENT_TYPE_GET_MONEY @"GetMoney"
#define EVENT_SELECT_CONTENT_TYPE_ITEM_ID_DEFAULT @"Default"
#define EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WATCH_VIDEO @"WatchVideo"
#define EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WELCOME_BACK @"WelcomeBack"
#define EVENT_SELECT_CONTENT_TYPE_ITEM_ID_TIME_BONUS @"TimeBonus"

#define EVENT_SELECT_POST_SCORE_MATCH_RESULT @"MatchResult"
#define EVENT_SELECT_POST_SCORE_WIN @"Win"
#define EVENT_SELECT_POST_SCORE_LOSE @"Lose"

#define EVENT_OPEN_RATING_POPUP @"show_rating"
#define EVENT_CLICK_BUTTON_RATING_POPUP @"click_rating"
#define EVENT_STAR_RATING_POPUP @"rate_star"
#define EVENT_RATING_POPUP_PARAM_STATE @"StateRatingPopup"
#define EVENT_RATING_POPUP_PARAM_CONDITION @"Condition"
#define EVENT_RATING_POPUP_PARAM_PLACE @"Place"
#define EVENT_RATING_POPUP_PARAM_BUTTON @"Button"
#define EVENT_RATING_POPUP_PARAM_STAR @"Star"
#define EVENT_RATING_POPUP_PARAM_STAR_STRING @"StarString"

#define EVENT_GAME_CENTER_ACTION @"game_center_action"
#define EVENT_GAME_CENTER_PARAM_GAME  @"game"
#define EVENT_GAME_CENTER_PARAM_ACTION @"action"
#define EVENT_GAME_CENTER_PARAM_PLAY_GAME @"play_game"
#define EVENT_GAME_CENTER_PARAM_INSTALL_GAME @"install_game"

#define EVENT_TIME_BONUS @"claim_time_bonus"
#define EVENT_TIME_BONUS_STEP @"claim_time_bonus_step"
#define EVENT_TIME_BONUS_VALUE @"claim_time_bonus_value"
#define EVENT_TIME_BONUS_WATCHED_VIDEO_REWARD @"claim_time_bonus_watched_video_reward"
#define EVENT_TIME_BONUS_PARAM_STEP @"step"
#define EVENT_TIME_BONUS_STEP_PREFIX @"step_"
#define EVENT_TIME_BONUS_PARAM_LV @"reward_level"
#define EVENT_TIME_BONUS_LV_PREFIX @"lv_"

#define EVENT_CLAIM_MAILBOX_REWARD @"claim_mail_box_reward"
#define EVENT_MAILBOX_PARAM_ID @"mailbox_id"
#define EVENT_MAILBOX_PARAM_REWARD_MONEY @"reward_money"

#define EVENT_CHANGE_AVATAR @"change_avatar"
#define EVENT_CHANGE_AVATAR_PARAM_NAME @"avatar_name"

#define EVENT_RECEIVE_LOCAL_NOTIFICATION @"receive_local_notification"
#define EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_NAME @"name"
#define EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_ID @"id"
#define EVENT_OPEN_LOCAL_NOTIFICATION @"open_local_notification"
#define EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_NAME @"name"
#define EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_ID @"id"

#define EVENT_AD_MANAGER_INTERSTITIAL_IMPRESSION  @"ad_manager_interstitial_impression"
#define EVENT_AD_MANAGER_INTERSTITIAL_MEDIATION_NAME  @"mediation"

#define EVENT_AD_MANAGER_VIDEO_IMPRESSION  @"ad_manager_video_impression"
#define EVENT_AD_MANAGER_VIDEO_MEDIATION_NAME  @"mediation"

#define EVENT_AD_MANAGER_LOAD_FAIL  @"ad_load_fail"
#define EVENT_AD_MANAGER_LOAD_FAIL_NAME  @"name"
#define EVENT_AD_MANAGER_LOAD_FAIL_ERROR_CODE  @"errorCode"
#define EVENT_AD_MANAGER_LOAD_FAIL_ERROR_MSG  @"errorMsg"
#define EVENT_AD_MANAGER_LOAD_FAIL_DOMAIN  @"domain"
#define EVENT_AD_MANAGER_LOAD_FAIL_CAUSE  @"cause"

#define EVENT_NETWORK @"network"
#define EVENT_NETWORK_STATUS @"status"
#define EVENT_NETWORK_ONLINE @"online"
#define EVENT_NETWORK_OFFLINE @"offline"

#define EVENT_SHOW_INAPP_REVIEW @"show_inapp_review"
#define EVENT_SHOW_INAPP_REVIEW_COMPLETED @"show_inapp_review_completed"

#define EVENT_BOT_WIN            @"bot_win"
#define EVENT_BOT_WIN_VERSION    @"bot_win_version"
#define EVENT_BOT_WIN_LEVEL      @"bot_win_level"
#define EVENT_BOT_WIN_MONEY      @"bot_win_money"
#define EVENT_BOT_WIN_CITY       @"bot_win_city"

#define EVENT_BOT_LOSE           @"bot_lose"
#define EVENT_BOT_LOSE_VERSION   @"bot_lose_version"
#define EVENT_BOT_LOSE_LEVEL     @"bot_lose_level"
#define EVENT_BOT_LOSE_MONEY     @"bot_lose_money"
#define EVENT_BOT_LOSE_CITY      @"bot_lose_city"

#define EVENT_BOT_UPGRADE_VERSION                   @"bot_upgrade_version"
#define EVENT_BOT_UPGRADE_VERSION_NEW               @"bot_upgrade_version_new"
#define EVENT_BOT_UPGRADE_VERSION_OLD               @"bot_upgrade_version_old"
#define EVENT_BOT_UPGRADE_VERSION_USER_WIN_RATE     @"bot_upgrade_version_user_win_rate"
#define EVENT_BOT_UPGRADE_VERSION_USER_MATCH_COUNT  @"bot_upgrade_version_user_match_count"
#define EVENT_BOT_UPGRADE_VERSION_USER_MONEY        @"bot_upgrade_version_user_money"

#define EVENT_BOT_DOWNGRADE_VERSION                     @"bot_downgrade_version"
#define EVENT_BOT_DOWNGRADE_VERSION_NEW                 @"bot_downgrade_version_new"
#define EVENT_BOT_DOWNGRADE_VERSION_OLD                 @"bot_downgrade_version_old"
#define EVENT_BOT_DOWNGRADE_VERSION_USER_WIN_RATE       @"bot_downgrade_version_user_win_rate"
#define EVENT_BOT_DOWNGRADE_VERSION_USER_MATCH_COUNT    @"bot_downgrade_version_user_match_count"
#define EVENT_BOT_DOWNGRADE_VERSION_USER_MONEY          @"bot_downgrade_version_user_money"

@implementation FirebaseAnalyticsController

+ (void)initFirebaseAnalytics
{
    [FIRApp configure];
}

+ (void) logGetMoneyDefault: (NSString*) sceneId: (float) money
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_GET_MONEY_DEFAULT parameters:@{
            EVENT_GET_MONEY_DEFAULT_MONEY: [NSNumber numberWithFloat:money],
            EVENT_GET_MONEY_SCENE_ID: sceneId,
            kFIRParameterValue: [NSNumber numberWithInt:1]
        }];
        
        [FIRAnalytics logEventWithName:kFIREventSelectContent parameters:@{
            kFIRParameterContentType: EVENT_SELECT_CONTENT_TYPE_GET_MONEY,
            kFIRParameterItemID: EVENT_SELECT_CONTENT_TYPE_ITEM_ID_DEFAULT
        }];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logGetMoneyWatchVideo: (NSString*) sceneId: (int)orderId: (float) money
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_GET_MONEY_WATCH_VIDEO parameters:@{
            EVENT_GET_MONEY_WATCH_VIDEO_MONEY_GETED: [NSNumber numberWithFloat:money],
            EVENT_GET_MONEY_WATCH_VIDEO_ORDER: [NSNumber numberWithInt:orderId],
            EVENT_GET_MONEY_SCENE_ID: sceneId,
            kFIRParameterValue: [NSNumber numberWithInt:1]
        }];
        
        [FIRAnalytics logEventWithName:kFIREventSelectContent parameters:@{
            kFIRParameterContentType: EVENT_SELECT_CONTENT_TYPE_GET_MONEY,
            kFIRParameterItemID: EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WATCH_VIDEO
        }];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logGetMoneyWelcomeBack: (NSString*)sceneId: (int)day: (float)money
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_GET_MONEY_WELCOMEBACK parameters:@{
            EVENT_GET_MONEY_WELCOMEBACK_MONEY: [NSNumber numberWithDouble:money],
            EVENT_GET_MONEY_WELCOMEBACK_DAY: [NSString stringWithFormat:@"Day_%i", day],
            EVENT_GET_MONEY_SCENE_ID: sceneId,
            kFIRParameterValue: [NSNumber numberWithInt:1]
        }];
        
        [FIRAnalytics logEventWithName:kFIREventSelectContent parameters:@{
            kFIRParameterContentType: EVENT_SELECT_CONTENT_TYPE_GET_MONEY,
            kFIRParameterItemID: EVENT_SELECT_CONTENT_TYPE_ITEM_ID_WELCOME_BACK
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logOpenAppRating
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_OPEN_APP_RATING parameters:@{}];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logOpenWatchVideo: (NSString*) state
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_OPEN_WATCH_VIDEO parameters:@{EVENT_OPEN_WATCH_VIDEO_STATE : state}];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logOpenTutorial
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_OPEN_TUTORIAL parameters:@{}];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logContinueToPlay: (double) bet: (float)ratio
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_CONTINUE_TO_PLAY parameters:@{
            EVENT_CONTINUE_TO_PLAY_BET: [NSNumber numberWithDouble:bet],
            EVENT_CONTINUE_TO_PLAY_RATIO: [NSNumber numberWithFloat:ratio]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logUnlockCity: (NSString*) type: (double)money
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_UNLOCK_CITY parameters:@{
            EVENT_UNLOCK_CITY_TYPE: type,
            EVENT_UNLOCK_CITY_MONEY: [NSNumber numberWithDouble:money]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logUnlockTable: (NSString*) type: (double)money
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_UNLOCK_TABLE parameters:@{
            EVENT_UNLOCK_TABLE_TYPE: type,
            EVENT_UNLOCK_TABLE_MONEY: [NSNumber numberWithDouble:money]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logStartMatch: (NSString*) cityType: (NSString*) tableType: (double)money: (double)bet: (float)ratio: (float)speed: (int)orderId
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_START_MATCH parameters:@{
                                                                    EVENT_START_MATCH_CITY_TYPE: cityType,
                                                                    EVENT_START_MATCH_TABLE_TYPE: tableType,
                                                                    EVENT_START_MATCH_MONEY: [NSNumber numberWithDouble:money],
                                                                    EVENT_START_MATCH_BET: [NSNumber numberWithDouble:bet],
                                                                    EVENT_START_MATCH_RATIO: [NSNumber numberWithFloat:ratio],
                                                                    EVENT_START_MATCH_SPEED: [NSNumber numberWithFloat:speed],
                                                                    EVENT_START_MATCH_ORDER:[NSNumber numberWithInt:orderId]
                                                                    }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logUserProperties: (NSString*) gameVersion: (NSString*)moneyName: (int)winNumber: (int)loseNumber: (int)winHitpotNumber: (int)level
{
    @try
    {
        [FIRAnalytics setUserPropertyString: moneyName forName:EVENT_USER_PROPERTIES_MONEY_RANGE];
        [FIRAnalytics setUserPropertyString: gameVersion forName:EVENT_USER_PROPERTIES_GAME_VERSION];
        [FIRAnalytics setUserPropertyString: [NSString stringWithFormat:@"%d", winNumber] forName:EVENT_USER_PROPERTIES_WIN_NUMBER];
        [FIRAnalytics setUserPropertyString: [NSString stringWithFormat:@"%d", loseNumber] forName:EVENT_USER_PROPERTIES_LOSE_NUMBER];
        [FIRAnalytics setUserPropertyString: [NSString stringWithFormat:@"%d", (winNumber + loseNumber)] forName:EVENT_USER_PROPERTIES_TOTAL_MATCH];
        [FIRAnalytics setUserPropertyString: [NSString stringWithFormat:@"%d", winHitpotNumber] forName:EVENT_USER_PROPERTIES_WIN_HITPOT_NUMBER];
        [FIRAnalytics setUserPropertyString: [NSString stringWithFormat:@"%d", level] forName:EVENT_USER_PROPERTIES_LEVEL];
        if(winNumber + loseNumber > 0)
        {
            [FIRAnalytics setUserPropertyString: [NSString stringWithFormat:@"%f", (winNumber*1.0f)/(winNumber+loseNumber)*100.0f] forName:EVENT_USER_PROPERTIES_WIN_RATE];
        }
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logWinResult: (int)tier: (NSString*) type: (double)money: (double)currMoney: (double)bet: (NSString*)cityType: (int)matchCount: (int)score :(int)numBonusCard : (int)numSecret
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_WIN_MATCH parameters:@{
                                                                       EVENT_RESULT_TIER: [NSNumber numberWithInt:tier],
                                                                       EVENT_RESULT_TYPE:type,
                                                                       EVENT_RESULT_CLAIMED_MONEY:[NSNumber numberWithDouble:money],
                                                                       EVENT_RESULT_CURRENT_MONEY:[NSNumber numberWithDouble:currMoney],
                                                                       EVENT_RESULT_BET:[NSNumber numberWithDouble:bet],
                                                                       EVENT_RESULT_CITY_TYPE:cityType,
                                                                       EVENT_RESULT_MATCH_COUNT: [NSNumber numberWithInt:matchCount],
                                                                       EVENT_RESULT_SCORE: [NSNumber numberWithInt:score],
                                                                       EVENT_RESULT_BONUS_CARDS: [NSNumber numberWithInt:numBonusCard],
                                                                       EVENT_RESULT_SECRETS: [NSNumber numberWithInt:numSecret]
                                                                       }];
        [FIRAnalytics logEventWithName:kFIREventPostScore parameters:@{
                                                                       kFIRParameterLevel: EVENT_SELECT_POST_SCORE_MATCH_RESULT,
                                                                       kFIRParameterCharacter:EVENT_SELECT_POST_SCORE_WIN,
                                                                       kFIRParameterScore:EVENT_SELECT_POST_SCORE_WIN                                                                  }];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logLoseResult: (int)tier: (NSString*) type: (double)money: (double)currMoney: (double)bet: (NSString*)cityType: (int)matchCount: (int)score: (int)numBonusCard: (int)numSecret
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_LOSE_MATCH parameters:@{
                                                                    EVENT_RESULT_TIER: [NSNumber numberWithInt:tier],
                                                                    EVENT_RESULT_TYPE:type,
                                                                    EVENT_RESULT_CLAIMED_MONEY:[NSNumber numberWithDouble:money],
                                                                    EVENT_RESULT_CURRENT_MONEY:[NSNumber numberWithDouble:currMoney],
                                                                    EVENT_RESULT_BET:[NSNumber numberWithDouble:bet],
                                                                    EVENT_RESULT_CITY_TYPE:cityType,
                                                                    EVENT_RESULT_MATCH_COUNT: [NSNumber numberWithInt:matchCount],
                                                                    EVENT_RESULT_SCORE: [NSNumber numberWithInt:score],
                                                                    EVENT_RESULT_BONUS_CARDS: [NSNumber numberWithInt:numBonusCard],
                                                                    EVENT_RESULT_SECRETS: [NSNumber numberWithInt:numSecret]
                                                                    }];
        [FIRAnalytics logEventWithName:kFIREventPostScore parameters:@{
                                                                       kFIRParameterLevel: EVENT_SELECT_POST_SCORE_MATCH_RESULT,
                                                                       kFIRParameterCharacter:EVENT_SELECT_POST_SCORE_LOSE,
                                                                       kFIRParameterScore:EVENT_SELECT_POST_SCORE_LOSE                                                                  }];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logWinHitPot: (int)slotOrder :(int)round : (double) moneyUser: (double) moneyHitpot : (double) moneyBet
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_WIN_HIT_POT parameters:@{
            EVENT_WIN_HIT_POT_USER: [NSString stringWithFormat:@"user_%d", slotOrder],
            EVENT_WIN_HIT_POT_ROUND: [NSNumber numberWithInt:round],
            EVENT_WIN_HIT_POT_ROUND_TOTAL: [NSString stringWithFormat:@"round_%d", round],
            EVENT_WIN_HIT_POT_MONEY_USER: [NSNumber numberWithDouble:moneyUser],
            EVENT_WIN_HIT_POT_MONEY_HITPOT: [NSNumber numberWithDouble:moneyHitpot],
            EVENT_WIN_HIT_POT_MONEY_BET: [NSNumber numberWithDouble:moneyBet]
        }];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logDoAction: (NSString*) type : (int) score
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_USER_DO_ACTION parameters:@{
            EVENT_USER_DO_ACTION_TYPE: type,
            EVENT_USER_DO_ACTION_SCORE: [NSNumber numberWithInt:score]
        }];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logClickSortCard: (NSString*) type
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_CLICK_SORT_CARD parameters:@{
            EVENT_CLICK_SORT_CARD_TYPE: type
        }];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logClickViewResult
{
    @try
    {
        [FIRAnalytics logEventWithName:EVENT_CLICK_VIEW_RESULT parameters:@{}];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}


+ (void) logTotalWatchVideoNumber: (int) number
{
    @try
    {
        [FIRAnalytics setUserPropertyString: [NSString stringWithFormat:@"%d", number] forName:EVENT_USER_PROPERTIES_VIDEO_COUNT];
    }
    @catch (NSException* e)
    {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logShowFullAds;
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_SHOW_FULL_ADS parameters:@{}];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logOpenApp: (NSString*) appName
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_OPEN_APP parameters:@{
            EVENT_OPEN_APP: appName
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logOpenRatingPopup: (int) stateRatingPopup : (NSString*) conditionName
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_OPEN_RATING_POPUP parameters:@{
            EVENT_RATING_POPUP_PARAM_STATE: [NSString stringWithFormat:@"Show_%d", stateRatingPopup],
            EVENT_RATING_POPUP_PARAM_CONDITION: conditionName
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logClickButtonRatingPopup: (int) stateRatingPopup : (NSString*) conditionName : (int) star : (NSString*) btnClickName : (NSString*) where
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_CLICK_BUTTON_RATING_POPUP parameters:@{
            EVENT_RATING_POPUP_PARAM_STATE: [NSString stringWithFormat:@"Show_%d", stateRatingPopup],
            EVENT_RATING_POPUP_PARAM_CONDITION: conditionName,
            EVENT_RATING_POPUP_PARAM_STAR: [NSNumber numberWithInt:star],
            EVENT_RATING_POPUP_PARAM_STAR_STRING: [NSString stringWithFormat:@"Star_%d", star],
            EVENT_RATING_POPUP_PARAM_BUTTON: btnClickName,
            EVENT_RATING_POPUP_PARAM_PLACE: where
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logClickStarPopup: (int) stateRatingPopup : (NSString*) conditionName : (int) star
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_STAR_RATING_POPUP parameters:@{
            EVENT_RATING_POPUP_PARAM_STATE: [NSString stringWithFormat:@"Show_%d", stateRatingPopup],
            EVENT_RATING_POPUP_PARAM_CONDITION: conditionName,
            EVENT_RATING_POPUP_PARAM_STAR: [NSNumber numberWithInt:star],
            EVENT_RATING_POPUP_PARAM_STAR_STRING: [NSString stringWithFormat:@"Star_%d", star]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logGameCenterAction: (NSString*) game : (int) state
{
    @try {
        NSString* action = @"";
        NSDictionary* parameters = @{
            EVENT_GAME_CENTER_PARAM_GAME: game,
            EVENT_GAME_CENTER_PARAM_ACTION: action
        };
        if (state == 0) {
            action = @"Install";
            [parameters setValue: [NSString stringWithFormat: @"%@%@", action, game] forKey:EVENT_GAME_CENTER_PARAM_INSTALL_GAME];
        } else if (state == 2) {
            action = @"Play";
            [parameters setValue: [NSString stringWithFormat: @"%@%@", action, game] forKey:EVENT_GAME_CENTER_PARAM_PLAY_GAME];
        }
        [FIRAnalytics logEventWithName:EVENT_GAME_CENTER_ACTION parameters: parameters];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logClaimTimeBonus: (int) step : (double) value : (bool) watchedVideoReward : (int) lv
{
    @try {
        NSDictionary* parameters = @{
            EVENT_TIME_BONUS_STEP: [NSString stringWithFormat: @"%@%d", EVENT_TIME_BONUS_STEP_PREFIX, step],
            EVENT_TIME_BONUS_PARAM_LV: [NSString stringWithFormat: @"%@%d", EVENT_TIME_BONUS_LV_PREFIX, lv],
            EVENT_TIME_BONUS_VALUE: [NSNumber numberWithDouble:value]
        };
        if(watchedVideoReward) {
            [parameters setValue: [NSString stringWithFormat: @"%@%d", EVENT_TIME_BONUS_STEP_PREFIX, step] forKey:EVENT_TIME_BONUS_WATCHED_VIDEO_REWARD];
        }
        [FIRAnalytics logEventWithName:EVENT_TIME_BONUS parameters: parameters];
        
        [FIRAnalytics logEventWithName:kFIREventSelectContent parameters:@{
            kFIRParameterContentType: EVENT_SELECT_CONTENT_TYPE_GET_MONEY,
            kFIRParameterItemID:EVENT_SELECT_CONTENT_TYPE_ITEM_ID_TIME_BONUS
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logClaimMailboxReward: (NSString*) id : (double) value
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_CLAIM_MAILBOX_REWARD parameters:@{
            EVENT_MAILBOX_PARAM_ID: id,
            EVENT_MAILBOX_PARAM_REWARD_MONEY: [NSNumber numberWithDouble:value]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logChangeAvatar: (NSString*) name
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_CHANGE_AVATAR parameters:@{
            EVENT_CHANGE_AVATAR_PARAM_NAME: name
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logSpendMoney: (NSString*) name: (double) amount
{
    @try {
        [FIRAnalytics logEventWithName: kFIREventSpendVirtualCurrency parameters:@{
            kFIRParameterItemName: name,
            kFIRParameterValue: [NSNumber numberWithDouble: amount],
            kFIRParameterVirtualCurrencyName: @"Money",
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logEarnMoney: (NSString*) name: (double) amount
{
    @try {
        [FIRAnalytics logEventWithName: kFIREventEarnVirtualCurrency parameters:@{
            kFIRParameterItemName: name,
            kFIRParameterValue: [NSNumber numberWithDouble: amount],
            kFIRParameterVirtualCurrencyName: @"Money",
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logOpenLocalNotification: (NSString*) name: (int) id
{
    @try {
        [FIRAnalytics logEventWithName: EVENT_OPEN_LOCAL_NOTIFICATION parameters:@{
            EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_NAME: name,
            EVENT_OPEN_LOCAL_NOTIFICATION_PARAM_ID: [NSNumber numberWithInt: id]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logReceiveLocalNotification: (NSString*) name: (int) id
{
    @try {
        [FIRAnalytics logEventWithName: EVENT_RECEIVE_LOCAL_NOTIFICATION parameters:@{
            EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_NAME: name,
            EVENT_RECEIVE_LOCAL_NOTIFICATION_PARAM_ID: [NSNumber numberWithInt: id]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logInterstitialAdsImpression:(NSString*)mediationName
{
    @try {
        [FIRAnalytics logEventWithName: EVENT_AD_MANAGER_INTERSTITIAL_IMPRESSION parameters:@{
            EVENT_AD_MANAGER_INTERSTITIAL_MEDIATION_NAME: mediationName
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logVideoAdsImpression:(NSString*)mediationName
{
    @try {
        [FIRAnalytics logEventWithName: EVENT_AD_MANAGER_VIDEO_IMPRESSION parameters:@{
            EVENT_AD_MANAGER_VIDEO_MEDIATION_NAME: mediationName
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logAdsLoadFail:(NSString*)name :(int)errorCode :(NSString*)errorMsg :(NSString*)cause :(NSString*)domain
{
    @try {
        [FIRAnalytics logEventWithName: EVENT_AD_MANAGER_LOAD_FAIL parameters:@{
            EVENT_AD_MANAGER_LOAD_FAIL_NAME: name,
            EVENT_AD_MANAGER_LOAD_FAIL_ERROR_CODE: [NSNumber numberWithInt: errorCode],
            EVENT_AD_MANAGER_LOAD_FAIL_ERROR_MSG: errorMsg,
            EVENT_AD_MANAGER_LOAD_FAIL_CAUSE: cause,
            EVENT_AD_MANAGER_LOAD_FAIL_DOMAIN: domain
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logNetwork: (bool)online
{
    @try {
        NSString* status = online ? EVENT_NETWORK_ONLINE : EVENT_NETWORK_OFFLINE;
        [FIRAnalytics logEventWithName: EVENT_NETWORK parameters:@{
            EVENT_NETWORK_STATUS: status
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logShowInAppReview:(int)ratingState: (NSString*)conditionName
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_SHOW_INAPP_REVIEW parameters:@{
            EVENT_RATING_POPUP_PARAM_STATE: [NSString stringWithFormat:@"Show_%d", ratingState],
            EVENT_RATING_POPUP_PARAM_CONDITION: conditionName
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logBotWin: (int)version: (int)level: (double)rewardMoney: (int)cityId
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_BOT_WIN parameters:@{
                EVENT_BOT_WIN_VERSION: [NSNumber numberWithInt:version],
                EVENT_BOT_WIN_LEVEL: [NSNumber numberWithInt:level],
                EVENT_BOT_WIN_MONEY: [NSNumber numberWithDouble:rewardMoney],
                EVENT_BOT_WIN_CITY: [NSNumber numberWithInt:cityId]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logBotLose: (int)version: (int)level: (double)rewardMoney: (int)cityId
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_BOT_LOSE parameters:@{
                EVENT_BOT_LOSE_VERSION: [NSNumber numberWithInt:version],
                EVENT_BOT_LOSE_LEVEL: [NSNumber numberWithInt:level],
                EVENT_BOT_LOSE_MONEY: [NSNumber numberWithDouble:rewardMoney],
                EVENT_BOT_LOSE_CITY: [NSNumber numberWithInt:cityId]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logBotUpgradeVersion: (int)newVersion: (int)oldVersion: (double)userWinRate: (int)userMatchCount: (double)userMoney
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_BOT_UPGRADE_VERSION parameters:@{
                EVENT_BOT_UPGRADE_VERSION_NEW: [NSNumber numberWithInt:newVersion],
                EVENT_BOT_UPGRADE_VERSION_OLD: [NSNumber numberWithInt:oldVersion],
                EVENT_BOT_UPGRADE_VERSION_USER_WIN_RATE: [NSNumber numberWithDouble:userWinRate],
                EVENT_BOT_UPGRADE_VERSION_USER_MATCH_COUNT: [NSNumber numberWithInt:userMatchCount],
                EVENT_BOT_UPGRADE_VERSION_USER_MONEY: [NSNumber numberWithDouble:userMoney]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}

+ (void) logBotDowngradeVersion: (int)newVersion: (int)oldVersion: (double)userWinRate: (int)userMatchCount: (double)userMoney
{
    @try {
        [FIRAnalytics logEventWithName:EVENT_BOT_DOWNGRADE_VERSION parameters:@{
                EVENT_BOT_DOWNGRADE_VERSION_NEW: [NSNumber numberWithInt:newVersion],
                EVENT_BOT_DOWNGRADE_VERSION_OLD: [NSNumber numberWithInt:oldVersion],
                EVENT_BOT_DOWNGRADE_VERSION_USER_WIN_RATE: [NSNumber numberWithDouble:userWinRate],
                EVENT_BOT_DOWNGRADE_VERSION_USER_MATCH_COUNT: [NSNumber numberWithInt:userMatchCount],
                EVENT_BOT_DOWNGRADE_VERSION_USER_MONEY: [NSNumber numberWithDouble:userMoney]
        }];
    } @catch (NSException *e) {
        NSLog(@"Exception: %@", e);
    }
}
@end

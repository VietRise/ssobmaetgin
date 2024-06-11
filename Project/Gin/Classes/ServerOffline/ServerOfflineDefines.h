#ifndef __SERVER_OFFLINE_DEFINES_H__
#define __SERVER_OFFLINE_DEFINES_H__

#define FIRST_TIME_IN_GAME "first_time_in_game"

#define USER_DEFAULT_VIDEO_BONUS 50000
#define USER_DEFAULT_VIDEO_BONUS_STEP 0.1f
#define USER_DEFAULT_VIDEO_BONUS_MAX 150000

#define USER_DEFAULT_SAVIOR_BONUS 10000
#define USER_DEFAULT_MONEY 0
#define USER_DEFAULT_MINIMUM_MONEY 1000
#define USER_DEFAULT_MAXIMUM_MONEY 999999999999999
#define USER_DEFAULT_DAILY_LOGIN_BONUS 100000
#define USER_DEFAULT_WELLCOME_STARTUP 100000
#define WELLCOMEBACK_DAY_DIFF 3
#define DAILY_LOGIN_DAY_DIFF 1

#define LOGIN_BONUS_RESULT_FAIL 0
#define LOGIN_BONUS_RESULT_DAILY 1
#define LOGIN_BONUS_RESULT_WELCOMEBACK 2
#define LOGIN_BONUS_RESULT_WELCOME_STARTUP 3

#define USER_DEFAULT_AVATAR "BotAvatar/54.png"

#define USER_MAXIMUM_VIDEO_WATCHED_PER_DAY 9999

#define STATUS_LOCK -1
#define STATUS_AVAILABLE 0
#define STATUS_ALREADY_BUY 1

#define MAX_LOG_IN_BONUS_COUNTER 7

#define RATING_STATE_WILL_SHOW_FIRST_TIME 0
#define RATING_STATE_NEVER_SHOW 100

#define SHOW_RATING_AT_LOBBY 1
#define SHOW_RATING_AT_GAMEPLAY 2

#define JSON_CITY "list_cities"
#define JSON_CITY_ID "id"
#define JSON_CITY_NAME "name"
#define JSON_CITY_STATUS "status"
#define JSON_CITY_BOT_NUM "bot_num"
#define JSON_CITY_BONUS "bonus"
#define JSON_CITY_BONUS_RATIO "ratio"
#define JSON_CITY_BONUS_CHANCE "chance"
#define JSON_CITY_UNLOCK_CONDITION "unlock_condition"
#define JSON_CITY_PRICE "purchase_price"

#define JSON_TABLE "list_tables"
#define JSON_TABLE_ID "id"
#define JSON_TABLE_NAME "name"
#define JSON_TABLE_STATUS "status"
#define JSON_TABLE_UNLOCK_CONDITION "unlock_condition"
#define JSON_TABLE_PRICE "purchase_price"
#define JSON_TABLE_BONUS "bonus"

#define JSON_LAST_UPDATE_DATE "last_update"
#define JSON_WATCHED_VIDEO_COUNT "watch_count"

#define JSON_CONDITION_FIELD_CONDITION "condition"
#define JSON_CONDITION_FIELD_MONEY "m"
#define JSON_CONDITION_FIELD_LEVEL "lv"
#define JSON_CONDITION_FIELD_WIN_RATE "wr"
#define JSON_CONDITION_FIELD_WIN_STREAK "ws"
#define JSON_CONDITION_FIELD_TOTAL_MATCH_LIFE_TIME "tmlt"
#define JSON_CONDITION_FIELD_MATCH_CITY_1 "mc1"
#define JSON_CONDITION_FIELD_MATCH_CITY_2 "mc2"
#define JSON_CONDITION_FIELD_MATCH_CITY_3 "mc3"
#define JSON_CONDITION_FIELD_CITY_UNLOCK "cu"
#define JSON_CONDITION_FIELD_WIN_BET_MULTIPLE "wbm"
#define JSON_CONDITION_FIELD_LASTEST_MINUTE "lm"
#define JSON_CONDITION_FIELD_SHOW_COUNT "sc"
#define JSON_CONDITION_FIELD_SHOW_IN_LOBBY "sil"
#define JSON_CONDITION_FIELD_SHOW_IN_GAME "sig"
#define JSON_CONDITION_FIELD_MATCH_ORDER "mo"
#define JSON_CONDITION_FIELD_WIN_SCORE_GAP_MULTIPLY "wmp"
#define JSON_CONDITION_FIELD_JACKPOT "jp"
#define JSON_CONDITION_FIELD_GAME_ORDER "go"
#define JSON_CONDITION_FIELD_ROUND_ORDER "ro"
#define JSON_CONDITION_FIELD_WIN_GAME "wg"

#define JSON_CONDITION_FIELD_MEMBER_ENABLED "enabled"
#define JSON_CONDITION_FIELD_MEMBER_MIN "min"
#define JSON_CONDITION_FIELD_MEMBER_MAX "max"

#define JSON_VIDEO_REWARDS_DATA_MAX_ROUND "max_round"
#define JSON_VIDEO_REWARDS_DATA_MAX_STEP "max_step"
#define JSON_VIDEO_REWARDS_DATA_COUNT_DOWN_TIME "count_down_time"
#define JSON_VIDEO_REWARDS_DATA_SKIPPABLE_STEP "skippable_step"
#define JSON_VIDEO_REWARDS_DATA_REWARD_MONEYS "reward_moneys"
#define JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS "bonus_reward_moneys"
#define JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS_MONEY "money"
#define JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS_RATIO "ratio"
#define JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_LEVEL "lv"

#define JSON_VIDEO_REWARDS_USER_DATA_CURRENT_ROUND "current_round"
#define JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP "current_step"
#define JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT "available_at"
#define JSON_VIDEO_REWARDS_USER_DATA_ENABLE "enable"
#define JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP_SKIPPED "current_step_skipped"
#define JSON_VIDEO_REWARDS_USER_DATA_LAST_REWARD_CLAIMED_AT "last_reward_claimed_at"

#define KEY_USER_LEVELEXP "user_levelexp"
#define KEY_USER_LEVEL "user_level"
#define KEY_DATA_MONEY "user_money"
#define KEY_DATA_NAME "user_name"
#define KEY_DATA_AVATAR "user_avatar"
#define KEY_LIST_ALREADY_BUY_CITIES "list_purchased_cities"
#define KEY_LIST_ALREADY_BUY_TABLES "list_purchased_tables"
#define KEY_LIST_WATCHED_VIDEO "list_watched_videos"
#define KEY_USER_LAST_LOGIN "last_login"
#define KEY_HAS_SAVED_MATCH "has_saved_match"
#define KEY_SAVED_MATCH_DATA "saved_match_data"
#define KEY_LIST_MAILBOX_CHECKED "mailbox_checked"
#define KEY_CURRENT_HOURLY_BONUS_INDEX "hourly_index"
#define KEY_CURRENT_HOURLY_BONUS_NEXT_TIME "hourly_next_time"
#define KEY_CURRENT_HOURLY_BONUS_START_TIME "hourly_start_time"
#define KEY_USER_RATING_STATE "user_rating_state_new"
#define KEY_USER_RATING_SHOW_LATST_TIME "user_rating_show_last_time"
#define KEY_USER_RATING_SHOW_COUNT "user_rating_show_count_new"
#define KEY_USER_WIN_MATCH_COUNT "user_win_match_count"
#define KEY_USER_LOSE_MATCH_COUNT "user_lose_match_count"
#define KEY_USER_WIN_GAME_COUNT "user_win_game_count"
#define KEY_USER_LOSE_GAME_COUNT "user_lose_game_count"
#define KEY_VIDEO_REWARD_DATA "video_reward_data"
#define KEY_VIDEO_REWARD_USER_DATA "video_reward_user_data"
#define KEY_CURRENT_LOG_IN_BONUS_COUNTER "log_in_bonus_counter"
#define KEY_BOT_VERSION "bot_version"

#define KEY_MATCH_COUNT_CLASSIC_GIN "match_count_classic_gin"
#define KEY_MATCH_COUNT_STRAIGHT_GIN "match_count_straight_gin"
#define KEY_MATCH_COUNT_OKLAHOMA_GIN "match_count_oklahoma_gin"

#define KEY_GAME_COUNT_CLASSIC_GIN "game_count_classic_gin"
#define KEY_GAME_COUNT_STRAIGHT_GIN "game_count_straight_gin"
#define KEY_GAME_COUNT_OKLAHOMA_GIN "game_count_oklahoma_gin"

#define MAILBOX_CHECKED_FILENAME "mbcdata"
#define MAILBOX_LOCAL_FILENAME "mblocal"

#define KEY_MAIL_ID "id"
#define KEY_MAIL_TYPE "type"
#define KEY_MAIL_VERSION "version"
#define KEY_MAIL_TITLE "title"
#define KEY_MAIL_CONTENT "content"
#define KEY_MAIL_FULL_CONTENT "full_content"
#define KEY_MAIL_REWARD "reward"
#define KEY_MAIL_CHECK "check"
#define KEY_MAIL_DATA "data"
#define KEY_MAIL_START_AT "start_at"
#define KEY_MAIL_END_AT "end_at"
#define KEY_MAIL_LOCAL "local"

#define KEY_MAIL_REWARD_TYPE "type"
#define KEY_MAIL_REWARD_VALUE "value"

#define JSON_LOCAL_NOTIFICATION_DELAY "delay"
#define JSON_LOCAL_NOTIFICATION_NOTIFICATIONS "notifications"
#define JSON_LOCAL_NOTIFICATION_ID "id"
#define JSON_LOCAL_NOTIFICATION_NAME "name"
#define JSON_LOCAL_NOTIFICATION_TYPE "type"
#define JSON_LOCAL_NOTIFICATION_CONTENT_TYPE "content_type"
#define JSON_LOCAL_NOTIFICATION_TITLE "title"
#define JSON_LOCAL_NOTIFICATION_MESSAGE "message"
#define JSON_LOCAL_NOTIFICATION_IMAGE "image"
#define JSON_LOCAL_NOTIFICATION_VERSION "verison"
#define JSON_LOCAL_NOTIFICATION_DELAY_TIME "delay_time"
#define JSON_LOCAL_NOTIFICATION_NUMBER "number"
#define JSON_LOCAL_NOTIFICATION_VALID_TIME "valid_time"

#endif

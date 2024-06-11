#ifndef __SERVER_CLIENT_GAME_DEFINE_H__
#define __SERVER_CLIENT_GAME_DEFINE_H__

#include "Define/GameDefine.h"

enum class MessageID
{
    MESSAGE_UNKNOWN = 0,

    MESSAGE_BOT_JOIN_RESPONSE = 2,

    MESSAGE_PROFILE_REQUEST  = 3,
    MESSAGE_PROFILE_RESPONSE = 4,

    MESSAGE_TICKET_REQUEST  = 5,
    MESSAGE_TICKET_RESPONSE = 6,

    MESSAGE_JOIN_GAME_REQUEST  = 7,
    MESSAGE_JOIN_GAME_RESPONSE = 8,

    MESSAGE_UNLOCK_CITY_REQUEST  = 9,
    MESSAGE_UNLOCK_CITY_RESPONSE = 10,

    MESSAGE_UNLOCK_TABLE_REQUEST  = 11,
    MESSAGE_UNLOCK_TABLE_RESPONSE = 12,

    MESSAGE_USER_DATA_REQUEST  = 13,
    MESSAGE_USER_DATA_RESPONSE = 14,

    MESSAGE_CHANGE_USER_NAME_REQUEST  = 15,
    MESSAGE_CHANGE_USER_NAME_RESPONSE = 16,

    MESSAGE_CHANGE_USER_AVATAR_REQUEST  = 17,
    MESSAGE_CHANGE_USER_AVATAR_RESPONSE = 18,

    MESSAGE_BONUS_REQUEST  = 19,
    MESSAGE_BONUS_RESPONSE = 20,

    MESSAGE_SAVIOR_BONUS_REQUEST  = 21,
    MESSAGE_SAVIOR_BONUS_RESPONSE = 22,

    MESSAGE_SAVIOR_POPUP_REQUEST  = 23,
    MESSAGE_SAVIOR_POPUP_RESPONSE = 24,

    MESSAGE_VIDEO_ADS_MONEY_REQUEST  = 26,
    MESSAGE_VIDEO_ADS_MONEY_RESPONSE = 27,

    MESSAGE_LOG_IN_OFFLINE_REQUEST  = 28,
    MESSAGE_LOG_IN_OFFLINE_RESPONSE = 29,

    MESSAGE_LOAD_SAVED_GAME_REQUEST   = 30,
    MESSAGE_LOAD_SAVED_MATCH_RESPONSE = 31,

    MESSAGE_GET_HOURLY_BONUS_REQUEST  = 32,
    MESSAGE_GET_HOURLY_BONUS_RESPONSE = 33,

    MESSAGE_CHEAT_CARD_REQUEST  = 34,
    MESSAGE_CHEAT_CARD_RESPONSE = 35,

    MESSAGE_CHECK_SHOW_RATING_POPUP_REQUEST  = 36,
    MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE = 37,

    MESSAGE_RATING_POPUP_SUBMIT_REQUEST  = 38,
    MESSAGE_RATING_POPUP_SUBMIT_RESPONSE = 39,

    MESSAGE_SUGGEST_UPDATE_REQUEST  = 40,
    MESSAGE_SUGGEST_UPDATE_RESPONSE = 41,

    MESSAGE_MAILBOX_REQUEST  = 42,
    MESSAGE_MAILBOX_RESPONSE = 43,

    MESSAGE_MAILBOX_CLAIM_REWARD_REQUEST  = 44,
    MESSAGE_MAILBOX_CLAIM_REWARD_RESPONSE = 45,

    MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_REQUEST  = 46,
    MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE = 47,

    MESSAGE_VIDEO_REWARD_INFO_REQUEST  = 48,
    MESSAGE_VIDEO_REWARD_INFO_RESPONSE = 49,

    MESSAGE_VIDEO_REWARD_CLAIM_REQUEST  = 50,
    MESSAGE_VIDEO_REWARD_CLAIM_RESPONSE = 51,

    MESSAGE_VIDEO_REWARD_SKIP_REQUEST  = 52,
    MESSAGE_VIDEO_REWARD_SKIP_RESPONSE = 53,

    MESSAGE_LOCAL_NOTIFICATION_REQUEST  = 54,
    MESSAGE_LOCAL_NOTIFICATION_RESPONSE = 55,

    MESSAGE_QUIT_IN_GAME_REQUEST  = 56,
    MESSAGE_QUIT_IN_GAME_RESPONSE = 57,

    MESSAGE_START_MATCH_REQUEST  = 58,
    MESSAGE_START_MATCH_RESPONSE = 59,
    MESSAGE_END_MATCH_RESPONSE   = 60,

    MESSAGE_PLAYER_HAND_CARD_RESPONSE = 61,
    MESSAGE_BOT_HAND_CARD_RESPONSE    = 62,

    MESSAGE_FINISH_DEAL_CARD_REQUEST = 63,

    MESSAGE_PLAYER_BEGIN_TURN_REQUEST  = 64,
    MESSAGE_PLAYER_BEGIN_TURN_RESPONSE = 65,

    MESSAGE_PLAYER_END_TURN_RESPONSE = 66,

    MESSAGE_PLAYER_TURN_STEP_GET_CARD_BEGIN_RESPONSE  = 100,
    MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_REQUEST  = 101,
    MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_RESPONSE = 102,
    MESSAGE_PLAYER_TURN_STEP_GET_CARD_DONE_REQUEST    = 103,

    MESSAGE_PLAYER_TURN_STEP_THROW_CARD_BEGIN_RESPONSE  = 104,
    MESSAGE_PLAYER_TURN_STEP_THROW_CARD_DONE_REQUEST    = 105,
    MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_REQUEST  = 106,
    MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_RESPONSE = 107,

    MESSAGE_PLAYER_TURN_STEP_PASS_BEGIN_RESPONSE  = 108,
    MESSAGE_PLAYER_TURN_STEP_PASS_DONE_REQUEST    = 109,
    MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_REQUEST  = 110,
    MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_RESPONSE = 111,

    MESSAGE_PLAYER_CUSTOM_MELDS_REQUEST  = 112,
    MESSAGE_PLAYER_CUSTOM_MELDS_RESPONSE = 113,
};

const std::map<MessageID, std::string> MessageNames = {{ MessageID::MESSAGE_UNKNOWN,                                     STRINGIFY(MESSAGE_UNKNOWN) },

                                                       { MessageID::MESSAGE_BOT_JOIN_RESPONSE,                           STRINGIFY(MESSAGE_BOT_JOIN_RESPONSE) },
                                                       { MessageID::MESSAGE_PROFILE_REQUEST,                             STRINGIFY(MESSAGE_PROFILE_REQUEST) },
                                                       { MessageID::MESSAGE_PROFILE_RESPONSE,                            STRINGIFY(MESSAGE_PROFILE_RESPONSE) },

                                                       { MessageID::MESSAGE_TICKET_REQUEST,                              STRINGIFY(MESSAGE_TICKET_REQUEST) },
                                                       { MessageID::MESSAGE_TICKET_RESPONSE,                             STRINGIFY(MESSAGE_TICKET_RESPONSE) },

                                                       { MessageID::MESSAGE_JOIN_GAME_REQUEST,                           STRINGIFY(MESSAGE_JOIN_GAME_REQUEST) },
                                                       { MessageID::MESSAGE_JOIN_GAME_RESPONSE,                          STRINGIFY(MESSAGE_JOIN_GAME_RESPONSE) },

                                                       { MessageID::MESSAGE_UNLOCK_CITY_REQUEST,                         STRINGIFY(MESSAGE_UNLOCK_CITY_REQUEST) },
                                                       { MessageID::MESSAGE_UNLOCK_CITY_RESPONSE,                        STRINGIFY(MESSAGE_UNLOCK_CITY_RESPONSE) },

                                                       { MessageID::MESSAGE_UNLOCK_TABLE_REQUEST,                        STRINGIFY(MESSAGE_UNLOCK_TABLE_REQUEST) },
                                                       { MessageID::MESSAGE_UNLOCK_TABLE_RESPONSE,                       STRINGIFY(MESSAGE_UNLOCK_TABLE_RESPONSE) },

                                                       { MessageID::MESSAGE_USER_DATA_REQUEST,                           STRINGIFY(MESSAGE_USER_DATA_REQUEST) },
                                                       { MessageID::MESSAGE_USER_DATA_RESPONSE,                          STRINGIFY(MESSAGE_USER_DATA_RESPONSE) },

                                                       { MessageID::MESSAGE_CHANGE_USER_NAME_REQUEST,                    STRINGIFY(MESSAGE_CHANGE_USER_NAME_REQUEST) },
                                                       { MessageID::MESSAGE_CHANGE_USER_NAME_RESPONSE,                   STRINGIFY(MESSAGE_CHANGE_USER_NAME_RESPONSE) },

                                                       { MessageID::MESSAGE_CHANGE_USER_AVATAR_REQUEST,                  STRINGIFY(MESSAGE_CHANGE_USER_AVATAR_REQUEST) },
                                                       { MessageID::MESSAGE_CHANGE_USER_AVATAR_RESPONSE,                 STRINGIFY(MESSAGE_CHANGE_USER_AVATAR_RESPONSE) },

                                                       { MessageID::MESSAGE_BONUS_REQUEST,                               STRINGIFY(MESSAGE_BONUS_REQUEST) },
                                                       { MessageID::MESSAGE_BONUS_RESPONSE,                              STRINGIFY(MESSAGE_BONUS_RESPONSE) },

                                                       { MessageID::MESSAGE_SAVIOR_BONUS_REQUEST,                        STRINGIFY(MESSAGE_SAVIOR_BONUS_REQUEST) },
                                                       { MessageID::MESSAGE_SAVIOR_BONUS_RESPONSE,                       STRINGIFY(MESSAGE_SAVIOR_BONUS_RESPONSE) },

                                                       { MessageID::MESSAGE_SAVIOR_POPUP_REQUEST,                        STRINGIFY(MESSAGE_SAVIOR_POPUP_REQUEST) },
                                                       { MessageID::MESSAGE_SAVIOR_POPUP_RESPONSE,                       STRINGIFY(MESSAGE_SAVIOR_POPUP_RESPONSE) },

                                                       { MessageID::MESSAGE_VIDEO_ADS_MONEY_REQUEST,                     STRINGIFY(MESSAGE_VIDEO_ADS_MONEY_REQUEST) },
                                                       { MessageID::MESSAGE_VIDEO_ADS_MONEY_RESPONSE,                    STRINGIFY(MESSAGE_VIDEO_ADS_MONEY_RESPONSE) },

                                                       { MessageID::MESSAGE_LOG_IN_OFFLINE_REQUEST,                      STRINGIFY(MESSAGE_LOG_IN_OFFLINE_REQUEST) },
                                                       { MessageID::MESSAGE_LOG_IN_OFFLINE_RESPONSE,                     STRINGIFY(MESSAGE_LOG_IN_OFFLINE_RESPONSE) },

                                                       { MessageID::MESSAGE_LOAD_SAVED_GAME_REQUEST,                     STRINGIFY(MESSAGE_LOAD_SAVED_GAME_REQUEST) },
                                                       { MessageID::MESSAGE_LOAD_SAVED_MATCH_RESPONSE,                   STRINGIFY(MESSAGE_LOAD_SAVED_MATCH_RESPONSE) },

                                                       { MessageID::MESSAGE_GET_HOURLY_BONUS_REQUEST,                    STRINGIFY(MESSAGE_GET_HOURLY_BONUS_REQUEST) },
                                                       { MessageID::MESSAGE_GET_HOURLY_BONUS_RESPONSE,                   STRINGIFY(MESSAGE_GET_HOURLY_BONUS_RESPONSE) },

                                                       { MessageID::MESSAGE_CHEAT_CARD_REQUEST,                          STRINGIFY(MESSAGE_CHEAT_CARD_REQUEST) },
                                                       { MessageID::MESSAGE_CHEAT_CARD_RESPONSE,                         STRINGIFY(MESSAGE_CHEAT_CARD_RESPONSE) },

                                                       { MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_REQUEST,             STRINGIFY(MESSAGE_CHECK_SHOW_RATING_POPUP_REQUEST) },
                                                       { MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE,            STRINGIFY(MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE) },

                                                       { MessageID::MESSAGE_RATING_POPUP_SUBMIT_REQUEST,                 STRINGIFY(MESSAGE_RATING_POPUP_SUBMIT_REQUEST) },
                                                       { MessageID::MESSAGE_RATING_POPUP_SUBMIT_RESPONSE,                STRINGIFY(MESSAGE_RATING_POPUP_SUBMIT_RESPONSE) },

                                                       { MessageID::MESSAGE_SUGGEST_UPDATE_REQUEST,                      STRINGIFY(MESSAGE_SUGGEST_UPDATE_REQUEST) },
                                                       { MessageID::MESSAGE_SUGGEST_UPDATE_RESPONSE,                     STRINGIFY(MESSAGE_SUGGEST_UPDATE_RESPONSE) },

                                                       { MessageID::MESSAGE_MAILBOX_REQUEST,                             STRINGIFY(MESSAGE_MAILBOX_REQUEST) },
                                                       { MessageID::MESSAGE_MAILBOX_RESPONSE,                            STRINGIFY(MESSAGE_MAILBOX_RESPONSE) },

                                                       { MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_REQUEST,                STRINGIFY(MESSAGE_MAILBOX_CLAIM_REWARD_REQUEST) },
                                                       { MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_RESPONSE,               STRINGIFY(MESSAGE_MAILBOX_CLAIM_REWARD_RESPONSE) },

                                                       { MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_REQUEST,        STRINGIFY(MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_REQUEST) },
                                                       { MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE,       STRINGIFY(MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE) },

                                                       { MessageID::MESSAGE_VIDEO_REWARD_INFO_REQUEST,                   STRINGIFY(MESSAGE_VIDEO_REWARD_INFO_REQUEST) },
                                                       { MessageID::MESSAGE_VIDEO_REWARD_INFO_RESPONSE,                  STRINGIFY(MESSAGE_VIDEO_REWARD_INFO_RESPONSE) },

                                                       { MessageID::MESSAGE_VIDEO_REWARD_CLAIM_REQUEST,                  STRINGIFY(MESSAGE_VIDEO_REWARD_CLAIM_REQUEST) },
                                                       { MessageID::MESSAGE_VIDEO_REWARD_CLAIM_RESPONSE,                 STRINGIFY(MESSAGE_VIDEO_REWARD_CLAIM_RESPONSE) },

                                                       { MessageID::MESSAGE_VIDEO_REWARD_SKIP_REQUEST,                   STRINGIFY(MESSAGE_VIDEO_REWARD_SKIP_REQUEST) },
                                                       { MessageID::MESSAGE_VIDEO_REWARD_SKIP_RESPONSE,                  STRINGIFY(MESSAGE_VIDEO_REWARD_SKIP_RESPONSE) },

                                                       { MessageID::MESSAGE_LOCAL_NOTIFICATION_REQUEST,                  STRINGIFY(MESSAGE_LOCAL_NOTIFICATION_REQUEST) },
                                                       { MessageID::MESSAGE_LOCAL_NOTIFICATION_RESPONSE,                 STRINGIFY(MESSAGE_LOCAL_NOTIFICATION_RESPONSE) },

                                                       { MessageID::MESSAGE_QUIT_IN_GAME_REQUEST,                        STRINGIFY(MESSAGE_QUIT_IN_GAME_REQUEST) },
                                                       { MessageID::MESSAGE_QUIT_IN_GAME_RESPONSE,                       STRINGIFY(MESSAGE_QUIT_IN_GAME_RESPONSE) },

                                                       { MessageID::MESSAGE_START_MATCH_REQUEST,                         STRINGIFY(MESSAGE_START_MATCH_REQUEST) },
                                                       { MessageID::MESSAGE_START_MATCH_RESPONSE,                        STRINGIFY(MESSAGE_START_MATCH_RESPONSE) },

                                                       { MessageID::MESSAGE_END_MATCH_RESPONSE,                          STRINGIFY(MESSAGE_END_MATCH_RESPONSE) },

                                                       { MessageID::MESSAGE_PLAYER_HAND_CARD_RESPONSE,                   STRINGIFY(MESSAGE_PLAYER_HAND_CARD_RESPONSE) },
                                                       { MessageID::MESSAGE_BOT_HAND_CARD_RESPONSE,                      STRINGIFY(MESSAGE_BOT_HAND_CARD_RESPONSE) },

                                                       { MessageID::MESSAGE_FINISH_DEAL_CARD_REQUEST,                    STRINGIFY(MESSAGE_FINISH_DEAL_CARD_REQUEST) },

                                                       { MessageID::MESSAGE_PLAYER_BEGIN_TURN_REQUEST,                   STRINGIFY(MESSAGE_PLAYER_BEGIN_TURN_REQUEST) },
                                                       { MessageID::MESSAGE_PLAYER_BEGIN_TURN_RESPONSE,                  STRINGIFY(MESSAGE_PLAYER_BEGIN_TURN_RESPONSE) },

                                                       { MessageID::MESSAGE_PLAYER_END_TURN_RESPONSE,                    STRINGIFY(MESSAGE_PLAYER_END_TURN_RESPONSE) },

                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_BEGIN_RESPONSE,    STRINGIFY(MESSAGE_PLAYER_TURN_STEP_GET_CARD_BEGIN_RESPONSE) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_REQUEST,    STRINGIFY(MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_REQUEST) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_RESPONSE,   STRINGIFY(MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_RESPONSE) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_DONE_REQUEST,      STRINGIFY(MESSAGE_PLAYER_TURN_STEP_GET_CARD_DONE_REQUEST) },

                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_BEGIN_RESPONSE,  STRINGIFY(MESSAGE_PLAYER_TURN_STEP_THROW_CARD_BEGIN_RESPONSE) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_DONE_REQUEST,    STRINGIFY(MESSAGE_PLAYER_TURN_STEP_THROW_CARD_DONE_REQUEST) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_REQUEST,  STRINGIFY(MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_REQUEST) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_RESPONSE, STRINGIFY(MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_RESPONSE) },

                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_BEGIN_RESPONSE,        STRINGIFY(MESSAGE_PLAYER_TURN_STEP_PASS_BEGIN_RESPONSE) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_DONE_REQUEST,          STRINGIFY(MESSAGE_PLAYER_TURN_STEP_PASS_DONE_REQUEST) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_REQUEST,        STRINGIFY(MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_REQUEST) },
                                                       { MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_RESPONSE,       STRINGIFY(MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_RESPONSE) },

                                                       { MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_REQUEST,                 STRINGIFY(MESSAGE_PLAYER_CUSTOM_MELDS_REQUEST) },
                                                       { MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_RESPONSE,                STRINGIFY(MESSAGE_PLAYER_CUSTOM_MELDS_RESPONSE) },
};

#define RESULT_BUY_FAIL_NOT_ENOUGH_MONEY -1
#define RESULT_BUY_FAIL_REQUIREMENT_NOT_MET -2
#define RESULT_BUY_FAIL_UNABLE_TO_BUY -3
#define RESULT_BUY_FAIL 0
#define RESULT_BUY_SUCCESS 1

#define RESULT_FAIL 0
#define RESULT_SUCCESS 1

#define SAVIOR_OPTION_FREE_GOLD 0
#define SAVIOR_OPTION_VIDEO_ADS 1

#define KEY_MESSAGE_ID "message_id"
#define KEY_RESULT "result"
#define KEY_VERSION_CODE "version_code"
#define KEY_WHERE "where"

#define KEY_BOT_INDEX "index"
#define KEY_BOT_NAME "name"
#define KEY_BOT_AVATAR "avata"
#define KEY_BOT_MONEY "money"
#define KEY_BOT_LIST "list_bot"

#define KEY_ADS_VIEW_MONEY "ads_money"

#define KEY_MONEY "money"
#define KEY_NAME "name"
#define KEY_AVATAR "img_path"
#define KEY_LEVEL "level"
#define KEY_LEVELEXP "level_exp"
#define KEY_NEXTLEVELEXP "next_exp"
#define KEY_LIST_PLACES "list_cities"
#define KEY_LIST_TABLES "list_tables"
#define KEY_INTERFERING_ADS "interfering_ads"

#define KEY_PLACE_ID "city_id"
#define KEY_PLACE_STATUS "city_status"
#define KEY_PLACE_NAME "city_name"
#define KEY_PLACE_PRICE "city_price"
#define KEY_PLACE_CONDITION "condition_city"

#define KEY_TABLE_ID "table_id"
#define KEY_TABLE_NAME "table_name"
#define KEY_TABLE_STATUS "table_status"
#define KEY_TABLE_PRICE "table_price"
#define KEY_TABLE_BONUS "bonus"

#define KEY_BET "bet"
#define KEY_TARGET_SCORE "target_score"
#define KEY_MAX_MULTIPLY_VALUE "max_multiply_value"
#define KEY_COUNT_DOWN_TIME "count_down_time"
#define KEY_MULTIPLY_BONUS "multiply_bonus"

#define KEY_MIN_BET "min_bet"
#define KEY_MAX_BET "max_bet"
#define KEY_DEFAULT_TARGET_SCORE "default_target_score"

#define KEY_BONUS_MONEY "bonus_money"
#define KEY_NEXT_BONUS_MONEY "next_bonus_money"
#define KEY_WELCOME_BACK_DAY "welcome_back_day"

#define KEY_FREE_MONEY "free_money"
#define KEY_SAVIOR_STATUS "savior_status"
#define KEY_SAVIOR_TYPE "savior_type"
#define KEY_SAVIOR_LIST "savior_list"

#define KEY_MAILBOX_LIST "mailbox_list"
#define KEY_MAILBOX_ID "mailbox_id"
#define KEY_MAILBOX_TYPE "mailbox_type"
#define KEY_MAILBOX_TITLE "mailbox_title"
#define KEY_MAILBOX_FULL_CONTENT "mailbox_full_content"
#define KEY_MAILBOX_CONTENT "mailbox_content"
#define KEY_MAILBOX_DATA "mailbox_data"
#define KEY_MAILBOX_START_AT "mailbox_start_at"
#define KEY_MAILBOX_END_AT "mailbox_end_at"
#define KEY_MAILBOX_REWARDS "mailbox_rewards"

#define KEY_CHEAT_CARD_USER_ID "user_id"
#define KEY_CHEAT_CARD_CARD_ID "card_id"
#define KEY_CHEAT_CARD_TYPE "type"
#define KEY_CHEAT_CARD_CHEATED_CARDS "cheat_cards"
#define KEY_CHEAT_CARD_REMAIN_CARDS "remain_cards"
#define KEY_CHEAT_CARD_FIRST_MOVE "first_move"

#define KEY_RATING_OPEN_AT "rating_open_at"
#define KEY_RATING_STATE "rating_state"
#define KEY_RATING_STAR "rating_star"
#define KEY_RATING_CONDITION "rating_condition"

#define KEY_VIDEO_REWARDS_ENABLE "video_reward_enable"
#define KEY_VIDEO_REWARD_ENABLE "enable"
#define KEY_VIDEO_REWARD_CURRENT_STEP "current_step"
#define KEY_VIDEO_REWARD_COUNT_DOWN "count_down"
#define KEY_VIDEO_REWARD_SKIPPABLE "skippable"
#define KEY_VIDEO_REWARD_IS_LOTTERY "is_lottery"
#define KEY_VIDEO_REWARD_REWARD_DATA "data"
#define KEY_VIDEO_REWARD_IS_SKIP_WAITING_PHASE "is_skip_waiting_phase"
#define KEY_VIDEO_REWARD_REWARD_AVAILABLE_AT "video_reward_available_at"
#define KEY_VIDEO_REWARD_REWARD_RESET_AT "video_reward_reset_at"
#define KEY_VIDEO_REWARD_REWARD_LV "reward_level"
#define KEY_VIDEO_REWARD_CURRENT_ROUND "current_round"
#define KEY_VIDEO_REWARD_MAX_ROUND "max_round"

#define KEY_SUGGEST_UPDATE_VERSION "suggest_version"
#define KEY_SUGGEST_UPDATE_CONTENT "suggest_content"

#define KEY_LOCAL_NOTIFICATION_NOTIFICATIONS "notifications"
#define KEY_LOCAL_NOTIFICATION_ID "id"
#define KEY_LOCAL_NOTIFICATION_NAME "name"
#define KEY_LOCAL_NOTIFICATION_TYPE "type"
#define KEY_LOCAL_NOTIFICATION_TITLE "title"
#define KEY_LOCAL_NOTIFICATION_MESSAGE "message"
#define KEY_LOCAL_NOTIFICATION_IMAGE "image"
#define KEY_LOCAL_NOTIFICATION_DELAY_TIME "delay_time"
#define KEY_LOCAL_NOTIFICATION_PERIOD_TIME "period_time"

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    INGAME    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#define KEY_JACKPOT "jackpot"
#define KEY_JACKPOT_ROUND "jackpot_round"
#define KEY_JACKPOT_MONEY "jackpot_money"
#define KEY_JACKPOT_LOCK "jackpot_lock"

#define KEY_GIN_ACTION_TYPE "action_type"
#define KEY_GIN_ACTION_RESULT "action_result"
#define KEY_GIN_PLAYER_INDEX "player_index"
#define KEY_GIN_PLAYER_DATA "player_data"
#define KEY_GIN_IS_USER "is_user"
#define KEY_GIN_SCORE "score"
#define KEY_GIN_ROUND "round"
#define KEY_GIN_START_ROUND_INDEX "start_round_index"
#define KEY_GIN_CARD_VALUE "card_value"
#define KEY_GIN_CUSTOM_MELD_VALUE "custom_meld"
#define KEY_GIN_HAND_CARDS "hand_cards"
#define KEY_GIN_THROW_CARDS "throw_cards"
#define KEY_GIN_TAKE_PLAYER_INDEX "taken_player_index"
#define KEY_GIN_LIST_PLAYER_DATA "list_player_data"
#define KEY_GIN_REWARD_MONEY "reward_money"
#define KEY_GIN_WIN_ORDER "win_order"
#define KEY_GIN_PREVIOUS_PLAYER_INDEX "previous_player_index"
#define KEY_GIN_PREVIOUS_PLAYER_CARD "previous_player_card"
#define KEY_GIN_LIST_COMBINATION "list_combination"
#define KEY_GIN_GIVE_CARD_DATA "give_card_data"
#define KEY_GIN_GIVE_CARD_VALUES "give_card_values"
#define KEY_GIN_GIVE_CARD_PLAYER_INDEX "give_card_player_index"
#define KEY_GIN_GIVE_CARD_PLAYER_COMBINATION_INDEX "give_card_player_combination_index"
#define KEY_GIN_RESULT_TYPE "result_type"
#define KEY_GIN_HINT_TAKE "hint_take"
#define KEY_GIN_CAN_TAKE "can_take"

#define KEY_GIN_RESULT_DETAIL "result_detail"
#define KEY_GIN_RESULT_DETAIL_HAND_POINTS "hand_points"
#define KEY_GIN_RESULT_DETAIL_WIN_POINTS "win_points"
#define KEY_GIN_RESULT_DETAIL_TARGET_POINTS "target_points"
#define KEY_GIN_RESULT_DETAIL_NORMAL_WIN "normal_win"
#define KEY_GIN_RESULT_DETAIL_JACKPOT_WIN "jackpot_win"
#define KEY_GIN_RESULT_DETAIL_BONUS_WIN "bonus_win"

#define KEY_GIN_RESULT_DETAIL_BONUS_WIN "bonus_win"
#define KEY_GIN_RESULT_DETAIL_JACKPOT_WIN "jackpot_win"
#define KEY_GIN_RESULT_DETAIL_BONUS_POINTS_GIN "bonus_points_gin"
#define KEY_GIN_RESULT_DETAIL_BONUS_POINTS_BIG_GIN "bonus_points_big_gin"
#define KEY_GIN_RESULT_DETAIL_BONUS_POINTS_UNDERCUT "bonus_points_undercut"
#define KEY_GIN_RESULT_DETAIL_BONUS_POINTS_SPADES_BONUS "bonus_points_spades_bonus"

#define KEY_PLACE_JACKPOT_RATIO_BET "jackpot_ratio_bet"
#define KEY_PLACE_REQUIRED_MONEY "required_money"

#define KEY_FIRST_TURN_INDEX "first_turn_index"
#define KEY_UP_CARD_VALUE "up_card_value"
#define KEY_KNOCK_POINTS "knock_point"
#define KEY_BOT_ID "bot_id"
#define KEY_IS_SPADES_DOUBLE "is_spades_double"

#endif //__SERVER_CLIENT_GAME_DEFINE_H__

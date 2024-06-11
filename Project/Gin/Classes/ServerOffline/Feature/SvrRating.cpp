#include "SvrRating.h"
#include "Define/GameDefine.h"
#include "Helper/JsonHelper.h"
#include "Helper/GameUtils.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Define/ResourcesDefine.h"
#include "ServerOffline/SvrUserProfile.h"
#include "ServerOffline/ServerOffline.h"
#include "ServerOffline/Feature/SvrLevel.h"
#include "ServerOffline/Feature/SvrShop.h"

int   SvrRating::_ratingStateNeverShow = RATING_STATE_NEVER_SHOW;
std::string                  SvrRating::_dataRatingJson = "";
std::vector<RatingCondition> SvrRating::_listRatingCondition;

void SvrRating::initStateRating()
{
    GameUtils::setIntegerForKeyUserDefault(KEY_USER_RATING_STATE, RATING_STATE_WILL_SHOW_FIRST_TIME);
}

void SvrRating::setStateRating(int state)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_USER_RATING_STATE, state);
}

int SvrRating::getStateRating()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_USER_RATING_STATE, RATING_STATE_WILL_SHOW_FIRST_TIME);
}

void SvrRating::setLastTimeShowRatingPopup(long long lastTimeShowRating)
{
    GameUtils::setDoubleForKeyUserDefault(KEY_USER_RATING_SHOW_LATST_TIME, lastTimeShowRating);
}

long long SvrRating::getLastTimeShowRatingPopup()
{
    return GameUtils::getDoubleForKeyUserDefault(KEY_USER_RATING_SHOW_LATST_TIME, 0);
}

void SvrRating::setCountShowRatingPopup(int count)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_USER_RATING_SHOW_COUNT, count);
}

int SvrRating::getCountShowRatingPopup()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_USER_RATING_SHOW_COUNT, 0);
}

void SvrRating::getDataConditionRatingPopup()
{
    _dataRatingJson                  = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_RATING_CONDITION, Resources::JSON::JSON_FILE_RATING);
    CCLOG(StringHelper::format("%s", _dataRatingJson.c_str()).c_str());
    SvrRating::_ratingStateNeverShow = RemoteConfig::getInteger(REMOTECONFIG_FUNCTION_RATING_STATE_NEVER_SHOW, RATING_STATE_NEVER_SHOW);
    const rapidjson::Document& document      = JsonHelper::parseStringToJson(_dataRatingJson);
    const rapidjson::Value   & conditionJson = JsonHelper::getValue(document, JSON_CONDITION_FIELD_CONDITION);
    _listRatingCondition.clear();
    for (auto iterCondition = conditionJson.MemberBegin(); iterCondition != conditionJson.MemberEnd(); iterCondition++)
    {
        const rapidjson::Value& value = iterCondition->value;
        RatingCondition ratingCondition;
        ratingCondition.name = iterCondition->name.GetString();
        for (auto iterMember = value.MemberBegin(); iterMember != value.MemberEnd(); iterMember++)
        {
            RatingObjectCondition ratingObjectCondition;
            ratingObjectCondition.name = iterMember->name.GetString();
            const rapidjson::Value& valueMember = iterMember->value;
            ratingObjectCondition.enable = JsonHelper::getInt(valueMember, JSON_CONDITION_FIELD_MEMBER_ENABLED, 0);
            ratingObjectCondition.min    = JsonHelper::getDouble(valueMember, JSON_CONDITION_FIELD_MEMBER_MIN, -1);
            ratingObjectCondition.max    = JsonHelper::getDouble(valueMember, JSON_CONDITION_FIELD_MEMBER_MAX, -1);
            ratingCondition.listValue.push_back(ratingObjectCondition);
        }
        _listRatingCondition.push_back(ratingCondition);
    }
}

std::string SvrRating::checkConditionToShowRatingPopup(int where)
{
    for (const auto& condition : _listRatingCondition)
    {
        bool passCondition = true;
        for (const auto& memberCondition : condition.listValue)
        {
            if (memberCondition.enable > 0)
            {
                if (!checkConditionSuccess(condition.name, memberCondition, SvrRating::getConditionValue(memberCondition.name, where)))
                {
                    passCondition = false;
                    break;
                }
            }
        }

        if (passCondition)
        {
            //#if USE_DEBUG_VIEW == 1
            //#define STRING_CAUSE_CONDITION_SUCCESS "ConditionName: %s, ShowWhere: %d, Money: %.0f, Level: %.0f, WinRate: %.2f, WinStreak: %.0f, TotalMatch: %.0f, TotalMatchCity1: %.0f, TotalMatchCity2: %.0f, TotalMatchCity3: %.0f, UnlockCity: %.0f, WinBetMultiple: %.2f, TimePassFromLastTimeShow: %.0f min, CountShowRatingPopup: %.0f, MatchOder: %.0f"
            //            std::string successCondition = cocos2d::StringUtils::format(STRING_CAUSE_CONDITION_SUCCESS,
            //                                                                        condition.name.c_str(),
            //                                                                        where,
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_MONEY),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_LEVEL),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_WIN_RATE),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_WIN_STREAK),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_TOTAL_MATCH_LIFE_TIME),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_MATCH_CITY_1),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_MATCH_CITY_2),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_MATCH_CITY_3),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_CITY_UNLOCK),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_WIN_BET_MULTIPLE),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_LASTEST_MINUTE),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_SHOW_COUNT),
            //                                                                        getConditionValue(JSON_CONDITION_FIELD_MATCH_ORDER));
            //            GameUtils::log(successCondition, cocos2d::Color3B::GREEN);
            //#endif
            return condition.name;
        }
    }
#if USE_CHEAT_SHOW_RATING
    return "1";
#endif
    return "";
}

double SvrRating::getConditionValue(const std::string& conditionMemberName, int where/* = 0*/)
{
    if (conditionMemberName.compare(JSON_CONDITION_FIELD_MONEY) == 0)
        return SvrUserProfile::getMoney();
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_LEVEL) == 0)
        return SvrLevel::getLevel();
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_WIN_RATE) == 0)
    {
        int   winMatch  = SvrUserProfile::getCountWinMatchUser();
        int   loseMatch = SvrUserProfile::getCountLoseMatchUser();
        float winrate   = (winMatch + loseMatch) != 0 ? winMatch * 100.0f / (winMatch + loseMatch) : 0.0f;
        return winrate;
    }
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_WIN_STREAK) == 0)
        return ServerOffline::_consecutiveWin;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_TOTAL_MATCH_LIFE_TIME) == 0)
        return SvrUserProfile::getTotalMatchCount();
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_MATCH_CITY_1) == 0)
        return SvrUserProfile::getMatchCount((int) CityType::CLASSIC_GIN);
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_MATCH_CITY_2) == 0)
        return SvrUserProfile::getMatchCount((int) CityType::STRAIGHT_GIN);
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_MATCH_CITY_3) == 0)
        return SvrUserProfile::getMatchCount((int) CityType::OKLAHOMA_GIN);
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_CITY_UNLOCK) == 0)
        return SvrShop::getLastedAlreadyBuyCity();
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_WIN_BET_MULTIPLE) == 0)
        return ServerOffline::_winBetMultiple;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_LASTEST_MINUTE) == 0)
        return (GameUtils::getCurrentTime() - SvrRating::getLastTimeShowRatingPopup()) / MINUTE_IN_SECOND;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_SHOW_COUNT) == 0)
        return SvrRating::getCountShowRatingPopup();
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_MATCH_ORDER) == 0)
        return ServerOffline::_matchCountPerOneTimeEnterRoom;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_SHOW_IN_GAME) == 0)
        return where == SHOW_RATING_AT_GAMEPLAY ? 1 : 0;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_SHOW_IN_LOBBY) == 0)
        return where == SHOW_RATING_AT_LOBBY ? 1 : 0;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_WIN_SCORE_GAP_MULTIPLY) == 0)
        return ServerOffline::_winScoreGapMultiply;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_JACKPOT) == 0)
        return ServerOffline::_winJackpot;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_GAME_ORDER) == 0)
        return ServerOffline::_gameCountPerOneTimeEnterRoom;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_ROUND_ORDER) == 0)
        return ServerOffline::_winGameRoundCount;
    else if (conditionMemberName.compare(JSON_CONDITION_FIELD_WIN_GAME) == 0)
        return ServerOffline::_winGame;

    return 0;
}

bool SvrRating::checkConditionSuccess(const std::string& conditionName, const RatingObjectCondition& memberCondition, double value)
{
    if (((memberCondition.min >= 0 && value >= memberCondition.min) || memberCondition.min < 0) && ((memberCondition.max >= 0 && value <= memberCondition.max) || memberCondition.max < 0))
        return true;
//#if USE_DEBUG_VIEW == 1
//#define STRING_CAUSE_CONDITION_FAILURE "ConditionName: %s, MemberConditionName: %s, Value: %.2f, Min: %.0f, Max: %.0f"
//    std::string causeFailureCondition = cocos2d::StringUtils::format(STRING_CAUSE_CONDITION_FAILURE,
//                                                                     conditionName.c_str(),
//                                                                     memberCondition.name.c_str(),
//                                                                     value,
//                                                                     (double) memberCondition.min,
//                                                                     (double) memberCondition.max);
//    GameUtils::log(causeFailureCondition, cocos2d::Color3B::RED);
//#endif
#if USE_CHEAT_SHOW_RATING
    return true;
#endif
    return false;
}
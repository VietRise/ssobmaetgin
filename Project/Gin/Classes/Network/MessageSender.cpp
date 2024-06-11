#include "MessageSender.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "AdapterClientServer/ClientManager.h"

using namespace rapidjson;
using namespace std;

void MessageSender::requestStartGame()
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_START_MATCH_REQUEST, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestQuitInGame()
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_QUIT_IN_GAME_REQUEST, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestDealCardFinish()
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_FINISH_DEAL_CARD_REQUEST, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestBeginTurnFinish(int playerIndex)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_BEGIN_TURN_REQUEST, al);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestThrowCard(int cardValue, int actionType, const std::vector<std::vector<int>>& melds)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_REQUEST, al);
    doc.AddMember(KEY_GIN_CARD_VALUE, cardValue, al);
    doc.AddMember(KEY_GIN_ACTION_TYPE, actionType, al);
    rapidjson::Value listMeldValueJson(rapidjson::kArrayType);
    for (const auto& meld : melds)
    {
        rapidjson::Value meldValueJson(rapidjson::kArrayType);
        for (auto        card : meld)
            meldValueJson.PushBack(rapidjson::Value(card), al);
        listMeldValueJson.PushBack(meldValueJson, al);
    }
    doc.AddMember(KEY_GIN_CUSTOM_MELD_VALUE, listMeldValueJson, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestThrowCardDone(int playerIndex, const std::vector<std::vector<int>>& melds)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_DONE_REQUEST, al);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, al);
    rapidjson::Value listMeldValueJson(rapidjson::kArrayType);
    for (const auto& meld : melds)
    {
        rapidjson::Value meldValueJson(rapidjson::kArrayType);
        for (auto        card : meld)
            meldValueJson.PushBack(rapidjson::Value(card), al);
        listMeldValueJson.PushBack(meldValueJson, al);
    }
    doc.AddMember(KEY_GIN_CUSTOM_MELD_VALUE, listMeldValueJson, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestGetCard(int playerIndex/* = -1*/)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_REQUEST, al);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestGetCardDone(int playerIndex)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_DONE_REQUEST, al);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestPass(int playerIndex, int upCardValue)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, doc.GetAllocator());
    doc.AddMember(KEY_GIN_CARD_VALUE, upCardValue, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestPassDone(int playerIndex)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_DONE_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestCustomMelds(int playerIndex, const std::vector<std::vector<int>>& melds)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& al = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_REQUEST, al);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, doc.GetAllocator());
    rapidjson::Value listMeldValueJson(rapidjson::kArrayType);
    for (const auto& meld : melds)
    {
        rapidjson::Value meldValueJson(rapidjson::kArrayType);
        for (auto        card : meld)
            meldValueJson.PushBack(rapidjson::Value(card), al);
        listMeldValueJson.PushBack(meldValueJson, al);
    }
    doc.AddMember(KEY_GIN_CUSTOM_MELD_VALUE, listMeldValueJson, al);
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestUserProfile(const std::string& where, int versionCode)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PROFILE_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_VERSION_CODE, versionCode, doc.GetAllocator());
    doc.AddMember(KEY_WHERE, rapidjson::Value().SetString(where.c_str(), doc.GetAllocator()), doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestShowGoldBonusPopup()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_SAVIOR_POPUP_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestGoldBonus()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_SAVIOR_BONUS_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestUnlockTable(int id)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_UNLOCK_TABLE_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_TABLE_ID, id, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestUnlockCity(int id)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_UNLOCK_CITY_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_PLACE_ID, id, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestOpenTicket(int cityId, int tableId)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_TICKET_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_PLACE_ID, cityId, doc.GetAllocator());
    doc.AddMember(KEY_TABLE_ID, tableId, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestFlightNow(int cityId, int tableId, double betPrice, int targetScore)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_JOIN_GAME_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_PLACE_ID, cityId, doc.GetAllocator());
    doc.AddMember(KEY_TABLE_ID, tableId, doc.GetAllocator());
    doc.AddMember(KEY_BET, betPrice, doc.GetAllocator());
    doc.AddMember(KEY_TARGET_SCORE, targetScore, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestChangeName(const std::string& name)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHANGE_USER_NAME_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_NAME, rapidjson::Value().SetString(name.c_str(), doc.GetAllocator()), doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestChangeProfilePicture(const std::string& path)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHANGE_USER_AVATAR_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_AVATAR, rapidjson::Value().SetString(path.c_str(), doc.GetAllocator()), doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestUserData()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_USER_DATA_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestBonusMoneyWatchVideo()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_BONUS_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestVideoAdsMoney()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_ADS_MONEY_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestLogInBonus()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_LOG_IN_OFFLINE_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestLoadSavedMatch()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_LOAD_SAVED_GAME_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestHourlyBonus()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_GET_HOURLY_BONUS_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestCheckShowRatingPopup(int where)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_RATING_OPEN_AT, where, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestRatingSubmit(int star)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_RATING_POPUP_SUBMIT_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_RATING_STAR, star, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestCheatCards(int slotIdx, CheatCardType cheatCardType, int cardValue)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHEAT_CARD_REQUEST, doc.GetAllocator());
    if (slotIdx != -1)
        doc.AddMember(KEY_CHEAT_CARD_USER_ID, slotIdx, doc.GetAllocator());
    doc.AddMember(KEY_CHEAT_CARD_CARD_ID, cardValue, doc.GetAllocator());
    doc.AddMember(KEY_CHEAT_CARD_TYPE, (int) cheatCardType, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestVideoRewardEnable()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestVideoRewardInfo()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_INFO_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestVideoRewardSkip()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_SKIP_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestVideoRewardClaim(bool isSkippedWaitingPhase)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_CLAIM_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_VIDEO_REWARD_IS_SKIP_WAITING_PHASE, isSkippedWaitingPhase, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestCheckSuggestUpdate()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_SUGGEST_UPDATE_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestMailbox()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_MAILBOX_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestClaimMailboxReward(const std::string& type, const std::string& id)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_REQUEST, doc.GetAllocator());
    doc.AddMember(KEY_MAILBOX_TYPE, rapidjson::Value().SetString(type.c_str(), doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember(KEY_MAILBOX_ID, rapidjson::Value().SetString(id.c_str(), doc.GetAllocator()), doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

void MessageSender::requestLocalNotificationData()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_LOCAL_NOTIFICATION_REQUEST, doc.GetAllocator());
    ClientManager::getInstance()->sendMessage(doc);
}

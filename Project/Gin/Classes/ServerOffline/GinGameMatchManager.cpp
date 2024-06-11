#include "GinGameMatchManager.h"
#include "Network/MessageHandler.h"
#include "AdapterClientServer/ServerManager.h"
#include "ServerOfflineDefines.h"
#include "Helper/JsonHelper.h"
#include "Helper/GameUtils.h"
#include "Helper/LogHelper.h"
#include "Helper/StringHelper.h"
#include "SvrUserProfile.h"
#include "Define/InGameDefine.h"
#include "Define/GameDefine.h"
#include "FirebaseGG/Firebase.h"
#include "Helper/DebugHelper.h"
#include "SvrBotHelper.h"
#include "SvrHelper.h"
#include "ServerOffline.h"
#include "SvrCard.h"
#include "Rules/RuleCommon.h"
#include "Rules/RuleInstantWin.h"
#include "Rules/RuleKnocking.h"
#include "Rules/RuleTakeCard.h"
#include "Rules/RuleThrowCard.h"
#include "ServerOffline/Bot/BotConstant.h"
#include "ServerOffline/Bot/BotLogger.h"
#include "ServerOffline/Feature/SvrRating.h"
#include "ServerOffline/Feature/SvrLog.h"
#include "ServerOffline/Feature/SvrLevel.h"

USING_NS_CC;
using namespace std;

#if USE_AUTO_PLAY_CARD
    #define BOT_PASS_ACTION_DELAY_DURATION_MIN  0.1f
    #define BOT_PASS_ACTION_DELAY_DURATION_MAX  0.1f

    #define BOT_GET_CARD_ACTION_DELAY_DURATION_MIN  0.1f
    #define BOT_GET_CARD_ACTION_DELAY_DURATION_MAX  0.1f

    #define BOT_THROW_CARD_ACTION_DELAY_DURATION_MIN  0.1f
    #define BOT_THROW_CARD_ACTION_DELAY_DURATION_MAX  0.1f
#else
    #define BOT_PASS_ACTION_DELAY_DURATION_MIN 1.0f
    #define BOT_PASS_ACTION_DELAY_DURATION_MAX 2.0f

    #define BOT_GET_CARD_ACTION_DELAY_DURATION_MIN 0.5f
    #define BOT_GET_CARD_ACTION_DELAY_DURATION_MAX 1.5f

    #define BOT_THROW_CARD_ACTION_DELAY_DURATION_MIN 0.75f
    #define BOT_THROW_CARD_ACTION_DELAY_DURATION_MAX 1.75f
#endif // USE_AUTO_PLAY_CARD

ServerOffline* GinGameMatchManager::_server = nullptr;

GinGameMatchManager::~GinGameMatchManager()
{
    for (HandCard* hand : this->_listHandCards)
        if (hand != nullptr)
            CC_SAFE_DELETE(hand);
    this->_listHandCards.clear();
}

GinGameMatchManager::GinGameMatchManager()
{
    this->_matchCount            = 0;
    this->_state                 = GameMatchState::IDLE;
    this->_levelBot              = (int) BotConstant::Level::LEVEL_0;
    this->_cheatFirstMoveSlotIdx = -1;
    this->_jackpotRound          = 0;
    this->_jackpotMoney          = 0;
    this->_jackpotLock           = false;
    this->_jackpotIsWin          = false;
    this->_isFinishTargetPoints  = false;
    this->_round                 = 0;
    ServerOffline::_matchCountPerOneTimeEnterRoom = 0;
    ServerOffline::_winScoreGapMultiply           = 0;
    ServerOffline::_winJackpot                    = 0;
    ServerOffline::_gameCountPerOneTimeEnterRoom  = 0;
    ServerOffline::_winGameRoundCount             = 0;
    ServerOffline::_winGame                       = 0;
    GinGameMatchManager::_server                  = nullptr;
}

GinGameMatchManager* GinGameMatchManager::createInstance()
{
    auto instance = GinGameMatchManager::create();
    instance->retain();
    return instance;
}

void GinGameMatchManager::destroyInstance()
{
    if (GinGameMatchManager::_server)
        GinGameMatchManager::_server->_currentGameManager = nullptr;
    CC_SAFE_RELEASE(this);
}

bool GinGameMatchManager::init()
{
    this->_state = GameMatchState::IDLE;
    this->_scheduler->scheduleUpdate(this, Scheduler::PRIORITY_NON_SYSTEM_MIN, false);
    this->_lastWinIndex   = INDEX_NONE;
    this->_firstPlayIndex = INDEX_NONE;
    this->_upCard         = SvrCard::Index::CARD_NONE;
    this->_isSpadesDouble = false;
    return true;
}

void GinGameMatchManager::joinGame(const GameMatchBundle& bundle)
{
    this->_state  = GameMatchState::PLAYER_JOINED;
    this->_bundle = bundle;
    const int totalPlayer = getTotalPlayer();
    this->_listHandCards.clear();
    for (int i = 0; i < totalPlayer; i++)
        this->_listHandCards.push_back(new HandCard(i));

    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_START_MATCH_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_FINISH_DEAL_CARD_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_BEGIN_TURN_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_DONE_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_DONE_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_DONE_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));

#if USE_CHEAT_CARD
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHEAT_CARD_REQUEST, MESSAGEHANDLE_SELECTOR(GinGameMatchManager::onMessage));
#endif
}

void GinGameMatchManager::startNewMatch()
{
#if USE_DEBUG_VIEW
    GameUtils::clearLog();
#endif
    _matchCount++;
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    LogHelper::match("");
    LogHelper::match("===================================================================================");
    LogHelper::matchArgs("START NEW MATCH", D_GET_VAR(_matchCount));
#endif
#if STRESS_TEST
    if (SvrUserProfile::getMoney() <= 1000)
        SvrUserProfile::setMoney("", 100000000);
#endif
    ServerOffline::_matchCountPerOneTimeEnterRoom++;
    ServerOffline::_winBetMultiple = 0;
    onResetData();
    onPrepareMatch();
#if STRESS_TEST
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(CallFunc::create([=]() { onDealCard(); }), nullptr));
#else
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
        onDealCard();
    }), nullptr));
#endif
    saveMatchData();
}

void GinGameMatchManager::endGame()
{
    MessageHandler::getInstance()->unregisterHandleMessage(this);
    destroyInstance();
}

void GinGameMatchManager::onKick(int result)
{
    sendMessageQuitMatch(result);
    endGame();
}

void GinGameMatchManager::onMessage(MessageID messageId, const rapidjson::Document& data)
{
    switch (messageId)
    {
        case MessageID::MESSAGE_START_MATCH_REQUEST:
        {
            processStartMatchResponse(data);
            break;
        }
        case MessageID::MESSAGE_FINISH_DEAL_CARD_REQUEST:
        {
            if (this->_state == GameMatchState::DEAL_CARD)
                onMatchLoop();
            break;
        }
        case MessageID::MESSAGE_PLAYER_BEGIN_TURN_REQUEST:
        {
            onMessagePlayerBeginTurn(JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, INDEX_NONE));
            break;
        }
        case MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_REQUEST:
        {
            onMessagePlayerTurnStepPassAction(JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, INDEX_NONE), JsonHelper::getInt(data, KEY_GIN_CARD_VALUE, SvrCard::Index::CARD_NONE));
            break;
        }
        case MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_DONE_REQUEST:
        {
            onMessagePlayerTurnStepPassDone(JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, INDEX_NONE));
            break;
        }
        case MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_REQUEST:
        {
            onMessagePlayerTurnStepGetCardAction(INDEX_PLAYER, JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, INDEX_NONE));
            break;
        }
        case MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_DONE_REQUEST:
        {
            onMessagePlayerTurnStepGetCardDone(JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, INDEX_NONE));
            break;
        }
        case MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_REQUEST:
        {
            HandCard* hand = getHandCard(INDEX_PLAYER);
            if (hand != nullptr && hand->isUser)
                getPlayerCustomMelds(data);
            int actionType = JsonHelper::getInt(data, KEY_GIN_ACTION_TYPE, (int) RuleConstant::ActionType::NONE);
            int cardIndex  = JsonHelper::getInt(data, KEY_GIN_CARD_VALUE, SvrCard::Index::CARD_NONE);
            onMessagePlayerTurnStepThrowCardAction(INDEX_PLAYER, cardIndex, (RuleConstant::ActionType) actionType);
            break;
        }
        case MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_DONE_REQUEST:
        {
            const int playerIndex = JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, INDEX_NONE);
            HandCard* hand = getHandCard(playerIndex);
            if (hand != nullptr && hand->isUser)
                getPlayerCustomMelds(data);
            onMessagePlayerTurnStepThrowCardDone(playerIndex);
            break;
        }
        case MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_REQUEST:
        {
            const int playerIndex = JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, INDEX_NONE);
            HandCard* hand = getHandCard(playerIndex);
            if (hand != nullptr && hand->isUser)
            {
                getPlayerCustomMelds(data);
                if (hand->index == getCurrentHand()->index && this->_playerTurnState == PlayerTurnState::STATE_THROW_CARD)
                {
                    pair<RuleConstant::ActionType, int> suggest = getActionTypeAndSuggestKnockPointByVariation(hand);
                    sendMessagePlayerCustomMelds(playerIndex, suggest.first, suggest.second);
                }
                else
                    sendMessagePlayerCustomMelds(playerIndex, RuleConstant::ActionType::NONE, -1);
            }
            break;
        }
        case MessageID::MESSAGE_CHEAT_CARD_REQUEST:
        {
#if USE_CHEAT_CARD
            onMessageCheatCard(data);
#endif
            break;
        }
        default:break;
    }
}

void GinGameMatchManager::processStartMatchResponse(const rapidjson::Document& data)
{
    if (SvrUserProfile::getMoney() >= USER_DEFAULT_MINIMUM_MONEY && (_state == GameMatchState::PLAYER_JOINED || _state == GameMatchState::FINISH))
        startNewMatch();
    else
        onKick();
}

void GinGameMatchManager::onResetData()
{
    this->_winOrder.clear();
    this->_mapResultDetails.clear();
    if (this->_isFinishTargetPoints)
    {
        this->_jackpotRound = 0;
        this->_jackpotLock  = false;
        this->_mapTargetPoints.clear();
        this->_isFinishTargetPoints = false;
        this->_round                = 0;
        this->_mapBet.clear();

        ServerOffline::_winJackpot          = 0;
        ServerOffline::_winScoreGapMultiply = 0;
        ServerOffline::_winGameRoundCount   = 0;
        ServerOffline::_winGame             = false;
    }
    this->_knockMapAction.clear();
    this->_knockPoints = RuleCommon::getKnockPoints((RuleConstant::GameRule) this->_bundle.cityId);
    this->_playerCustomMelds.clear();
    this->_currentPlayOrderIndex  = -1;
    this->_previousPlayOrderIndex = -1;
    this->_upCard                 = SvrCard::Index::CARD_NONE;
    this->_listPass.clear();
    this->_pileDiscards.clear();
    this->_isSpadesDouble      = false;
    this->_playerTurnStateDone = PlayerTurnState::STATE_READY;
    this->stopAllActions();
    for (const auto& hand : this->_listHandCards)
        if (hand != nullptr)
            hand->reset();
    const int roomBet = this->_bundle.price * BONUS_POINTS_BIG_GIN;
    if (this->_listBotInfo.empty())
    {
        if (this->_cheatBotList.empty())
        {
            this->_listBotInfo = SvrBotHelper::getBotInfo(_bundle.numBot, _bundle, getCurrentAvatarChecked());
        }
        else
        {
            this->_listBotInfo = _cheatBotList;
            _cheatBotList.clear();
        }
        vector<int> indexes;
        for (int    i = 0; i < this->_listBotInfo.size(); i++)
            indexes.push_back(i);
        sendBotData(indexes);
    }
    else
    {
        vector<int> indexes;
        for (int    i = 0; i < this->_listBotInfo.size(); i++)
        {
            bool forceChange = false;
            if (this->_listBotInfo.at(i).money <= roomBet)
                forceChange = true;
            if (forceChange)
            {
                this->_listBotInfo[i] = SvrBotHelper::changeBot(this->_listBotInfo[i], this->_bundle, getCurrentAvatarChecked(i + 1));
                indexes.push_back(i);
            }
        }
        sendBotData(indexes);
    }
    BotHelper::resetData();
}

void GinGameMatchManager::sendBotData(const vector<int>& changedIndex)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_BOT_JOIN_RESPONSE, allocator);
    rapidjson::Value botArr(rapidjson::kArrayType);
    for (const int& index : changedIndex)
    {
        SvrBotData& botInfo = this->_listBotInfo[index];
        rapidjson::Value val;
        val.SetObject();
        val.AddMember(KEY_BOT_INDEX, index, allocator);
        val.AddMember(KEY_BOT_NAME, rapidjson::Value().SetString(botInfo.name.c_str(), allocator), allocator);
        val.AddMember(KEY_BOT_AVATAR, rapidjson::Value().SetString(botInfo.avatar.c_str(), allocator), allocator);
        val.AddMember(KEY_BOT_MONEY, botInfo.money, allocator);
        botArr.PushBack(val, allocator);
    }
    doc.AddMember(KEY_BOT_LIST, botArr, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::onPrepareMatch()
{
    this->_round++;
    if (hasJackpot())
    {
        if (this->_jackpotIsWin)
        {
            this->_jackpotRound = 0;
            this->_jackpotMoney = 0;
            this->_jackpotLock  = false;
            this->_jackpotIsWin = false;
        }
        this->_jackpotRound++;
        this->_jackpotMoney += this->_bundle.price * this->_bundle.jackpotRatioBet;
    }
    for (const auto& hand : this->_listHandCards)
    {
        // Bet
        const double betMoney = RATIO_BET * this->_bundle.price;
        if (betMoney > 0)
            this->_mapBet[hand->index] = betMoney;

        map<int, double> initMapResultDetails;
        for (int         i = 0; i < RESULT_DETAIL_MAX_SIZE; i++)
            initMapResultDetails[i] = 0;

        this->_mapResultDetails[hand->index] = initMapResultDetails;
        updateKnockAction(hand->index, RuleConstant::ActionType::NOT_ENOUGH_TO_KNOCK);
    }
#if STRESS_TEST == 0
    sendMatchInfo();
#endif
    SvrLog::logEarnSpendMoney();
    this->_state = GameMatchState::READY;
}

void GinGameMatchManager::onDealCard()
{
    int   userMatchCount     = SvrUserProfile::getMatchCount(_bundle.cityId);
    int   userWinMatchCount  = SvrUserProfile::getCountWinMatchUser();
    int   userLoseMatchCount = SvrUserProfile::getCountLoseMatchUser();
    float userWinRate        = SvrRating::getConditionValue(JSON_CONDITION_FIELD_WIN_RATE);

#if DEBUG_LOG_BOT && DEBUG_DEAL_CARD
    LogHelper::matchArgs("USER STATS", D_GET_VAR(userMatchCount, userWinMatchCount, userLoseMatchCount, userWinRate));
#endif
    this->_state = GameMatchState::DEAL_CARD;
    const int totalPlayer = getTotalPlayer();

    // Release level of bot.
    double userMoney = SvrUserProfile::getMoney();
    this->_levelBot = BotHelper::getLevel(userWinRate, userMatchCount, userMoney);

#if DEBUG_LOG_BOT || STRESS_TEST
    LogHelper::matchArgs("BOT RELEASE", D_GET_VAR(_levelBot, BotHelper::getVersion(), _bundle.cityId, userWinRate, userMatchCount, userMoney, SvrUserProfile::getTotalMatchCount()));
#endif

#if USE_CHEAT_CARD == 0
    if (this->_lastWinIndex == INDEX_NONE)
        this->_lastWinIndex = INDEX_PLAYER;
    this->registerPlayOrder(this->_lastWinIndex, totalPlayer);

    // Deal card real
    vector<vector<int>> cardSlot = SvrHelper::checkDealCard(totalPlayer, _remainCards);

    // Check for new user
    for (int i = 0; i < MAX_RE_DEAL_COUNT; i++)
    {
        if (shouldDealCardAgain(cardSlot, _playOrder))
            cardSlot = SvrHelper::checkDealCard(totalPlayer, _remainCards);
        else
            break;
    }
#else
    vector<int> cheatRemainCards;
    /*
    _cheatFirstMoveSlotIdx  = 1;
    _mapCheatCardPlayers[0] = {1,4,9,11,12,23,27,31,33,35}; // USER
    _mapCheatCardPlayers[1] = {0,6,10,20,26,36,41,42,43,46}; // BOT 1
    cheatRemainCards        = {5,16,49,39,30,34,19,51,3,21,14,24,47,17,38,2,48,32,50,18,37,25,44,15,45,28,29,7,13,22,40,8};
    _levelBot               = 0;
    BotHelper::cheatVersion(0);
    */

    bool noCheat = true;
    for (const auto& mapCheatCardPlayer : this->_mapCheatCardPlayers)
    {
        if (!mapCheatCardPlayer.second.empty())
        {
            noCheat = false;
            break;
        }
    }
    if (this->_cheatFirstMoveSlotIdx != -1)
        this->_lastWinIndex = this->_cheatFirstMoveSlotIdx;
    if (this->_lastWinIndex == INDEX_NONE)
        this->_lastWinIndex = INDEX_PLAYER;
    this->registerPlayOrder(this->_lastWinIndex, totalPlayer);

    // Deal card real
    map<int, vector<int>> cardSlot = SvrHelper::checkDealCardCheat(totalPlayer, this->_mapCheatCardPlayers, this->_remainCards);

    // Check for new user
    for (int i = 0; i < MAX_RE_DEAL_COUNT && noCheat; i++)
    {
        if (shouldDealCardAgain(cardSlot))
            cardSlot = SvrHelper::checkDealCardCheat(totalPlayer, this->_mapCheatCardPlayers, this->_remainCards);
        else
            break;
    }
    if (!cheatRemainCards.empty())
        this->_remainCards = cheatRemainCards;
#endif
    this->_firstPlayIndex           = this->_lastWinIndex;
    this->_startRoundPlayOrderIndex = 0;

    // up card
    this->_upCard = this->_remainCards.back();
    this->_remainCards.pop_back();

    // update knock points
    if (this->_bundle.cityId == (int) RuleConstant::GameRule::OKLAHOMA_GIN)
    {
        const int point = SvrCard::getPoints(this->_upCard);
        this->_knockPoints    = (point == 1 ? 0 : point); // Ace <=> Straight GIN
        this->_isSpadesDouble = SvrCard::isSuit(this->_upCard, SvrCard::Suit::SUIT_SPADES);
    }

    // set hand cards player and bots
    int count     = 0;
    int indexSlot = this->_lastWinIndex;
    while (count < totalPlayer)
    {
        if (indexSlot == INDEX_PLAYER)
            sendCardToPlayer(cardSlot[indexSlot]);
        else
            sendCardToBot(cardSlot[indexSlot], indexSlot);
        indexSlot = (indexSlot + 1) % totalPlayer;
        count++;
    }

    SvrUserProfile::addMatchCount(_bundle.cityId);

#if DEBUG_LOG_BOT || DEBUG_DEAL_CARD
    DebugHelper::printDealCards(this->_listHandCards, this->_lastWinIndex, this->_levelBot, BotHelper::getVersion(), this->_upCard, this->_pileDiscards, this->_remainCards);
#endif
#if DEBUG_LOG_BOT
    string mode = RuleCommon::rule2Str((RuleConstant::GameRule) _bundle.cityId);
    LogHelper::matchArgs("GAME MODE", D_GET_VAR(mode, _bundle.targetScore, _isSpadesDouble, _knockPoints));
#endif
#if STRESS_TEST
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_FINISH_DEAL_CARD_REQUEST, doc.GetAllocator());
    onMessage(MessageID::MESSAGE_FINISH_DEAL_CARD_REQUEST, doc);
#endif
}

void GinGameMatchManager::sendMatchInfo()
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_START_MATCH_RESPONSE, allocator);
    doc.AddMember(KEY_PLACE_ID, this->_bundle.cityId, allocator);
    doc.AddMember(KEY_TABLE_ID, this->_bundle.tableId, allocator);
    doc.AddMember(KEY_BET, this->_bundle.price, allocator);
    if (hasJackpot())
        doc.AddMember(KEY_JACKPOT, getJackpotDataJson(allocator), allocator);
    rapidjson::Value playerData(rapidjson::kArrayType);

    const pair<int, float> curMultiplyBonus = getCurrentMultiplyValueByScoreGap();
    for (const auto& hand : this->_listHandCards)
    {
        rapidjson::Value handObject(rapidjson::kObjectType);
        handObject.AddMember(KEY_GIN_PLAYER_INDEX, hand->index, allocator);
        handObject.AddMember(KEY_GIN_IS_USER, hand->isUser, allocator);
        handObject.AddMember(KEY_GIN_SCORE, this->_mapTargetPoints[hand->index], allocator);
        handObject.AddMember(KEY_BET, this->_mapBet.at(hand->index), allocator);
        if (curMultiplyBonus.first != INDEX_NONE)
            handObject.AddMember(KEY_MULTIPLY_BONUS, (curMultiplyBonus.first == hand->index ? curMultiplyBonus.second : -curMultiplyBonus.second), allocator);
        else
            handObject.AddMember(KEY_MULTIPLY_BONUS, 0, allocator);
        playerData.PushBack(handObject, allocator);
    }
    doc.AddMember(KEY_GIN_LIST_PLAYER_DATA, playerData, allocator);
    doc.AddMember(KEY_GIN_ROUND, this->_round, allocator);
    doc.AddMember(KEY_TARGET_SCORE, this->_bundle.targetScore, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::registerPlayOrder(int firstIndex, int slotNum)
{
    _playOrder.clear();
    for (int i = 0; i < slotNum; i++)
        _playOrder.push_back(INDEX_NONE);
    _playOrder[0] = firstIndex;
    int      nextIndex = firstIndex;
    for (int i         = 1; i < slotNum; i++)
    {
        nextIndex = (nextIndex - 1 < 0) ? slotNum - 1 : nextIndex - 1;
        _playOrder[i] = nextIndex;
    }
}

void GinGameMatchManager::sendCardToPlayer(const vector<int>& cards)
{
    vector<int> sortCards = SvrCard::sortAsc(cards);
    getHandCard(INDEX_PLAYER)->setHandCards(sortCards);
#if STRESS_TEST == 0
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_HAND_CARD_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_HAND_CARDS, JsonHelper::makeValueArrayInt(doc, sortCards), allocator);
    doc.AddMember(KEY_FIRST_TURN_INDEX, this->_firstPlayIndex, allocator);
    doc.AddMember(KEY_UP_CARD_VALUE, this->_upCard, allocator);
    doc.AddMember(KEY_KNOCK_POINTS, this->_knockPoints, allocator);
    doc.AddMember(KEY_IS_SPADES_DOUBLE, this->_isSpadesDouble, allocator);
    ServerManager::getInstance()->sendMessage(doc);
#endif
}

void GinGameMatchManager::sendCardToBot(const vector<int>& cards, int slotIndex)
{
    vector<int> sortCards = SvrCard::sortAsc(cards);
    getHandCard(slotIndex)->setHandCards(sortCards);
#if STRESS_TEST == 0
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_BOT_HAND_CARD_RESPONSE, allocator);
    doc.AddMember(KEY_BOT_ID, slotIndex, allocator);
    doc.AddMember(KEY_GIN_HAND_CARDS, JsonHelper::makeValueArrayInt(doc, sortCards), allocator);
    ServerManager::getInstance()->sendMessage(doc);
#endif
}

HandCard* GinGameMatchManager::getHandCard(int userSlot)
{
    for (const auto& hand : this->_listHandCards)
        if (hand != nullptr && hand->index == userSlot)
            return hand;
    return nullptr;
}

HandCard* GinGameMatchManager::getPreviousHandCard()
{
    if (this->_previousPlayOrderIndex >= 0 && this->_previousPlayOrderIndex < this->_playOrder.size())
        return getHandCard(this->_playOrder.at(this->_previousPlayOrderIndex));
    return nullptr;
}

HandCard* GinGameMatchManager::getNextHandCard()
{
    const int nextPlayOrderIndex = tryGetNextPlayOrderIndex();
    if (nextPlayOrderIndex >= 0 && nextPlayOrderIndex < this->_playOrder.size())
        return getHandCard(this->_playOrder.at(nextPlayOrderIndex));
    return nullptr;
}

void GinGameMatchManager::logicDealCard()
{
    this->_state = GameMatchState::INGAME;
    logicNextTurn();
}

void GinGameMatchManager::logicInGame()
{
    logicNextTurn();
}

void GinGameMatchManager::logicNextTurn()
{
    if (this->_state == GameMatchState::INGAME)
    {
        this->_previousPlayOrderIndex = this->_currentPlayOrderIndex;
        this->_currentPlayOrderIndex  = tryGetNextPlayOrderIndex();
        if (this->_currentPlayOrderIndex != -1)
        {
#if STRESS_TEST == 0
            float delayTime = 0.05f;
#if USE_AUTO_PLAY_CARD
            delayTime = 0;
#endif
            Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(delayTime), CallFunc::create([=]() {
                logicPlayerBeginTurn();
            }), nullptr));
#else
            logicPlayerBeginTurn();
#endif
        }
        else
        {
            logicInGame();
        }
    }
}

int GinGameMatchManager::tryGetNextPlayOrderIndex()
{
    int  nextTurn = this->_currentPlayOrderIndex;
    bool found    = false;
    int  size     = this->_playOrder.size() - 1;
    while (size >= 0)
    {
        size--;
        nextTurn = (nextTurn + 1) % this->_playOrder.size();
        found    = true;
        break;
    }
    return found ? nextTurn : -1;
}

void GinGameMatchManager::logicPlayerBeginTurn()
{
    this->_playerTurnState     = PlayerTurnState::STATE_READY;
    this->_playerTurnStateDone = PlayerTurnState::STATE_READY;
    HandCard* hand = getCurrentHand();
#if STRESS_TEST == 0
    sendMessagePlayerBeginTurn(hand->index, hand->isUser);
#else
    onMessagePlayerBeginTurn(hand->index);
#endif
}

void GinGameMatchManager::logicPlayerTurnStep(int playerIndex)
{
    if (this->_state == GameMatchState::INGAME && getCurrentHand()->index == playerIndex)
    {
        HandCard* hand = getCurrentHand();
        if (this->_playerTurnState == PlayerTurnState::STATE_READY && SvrCard::isValidIndex(this->_upCard) && this->_listPass.size() < this->_listHandCards.size())
        {
            const auto& it = std::find(this->_listPass.begin(), this->_listPass.end(), hand->index);
            if (it == this->_listPass.end()) // not found
            {
                this->_listPass.push_back(hand->index);
                this->_playerTurnState = PlayerTurnState::STATE_PASS;
#if STRESS_TEST == 0
                sendMessagePlayerTurnStepPass(hand->index, RuleTakeCard::canTake(this->_upCard, hand->handCards), true);
#endif
                if (isBot(hand->index)) // bot
                    logicBotTurnStepPassAction(hand, this->_upCard);
            }
            else
            {
                logicNextTurn();
            }
        }
        else if (this->_playerTurnState == PlayerTurnState::STATE_READY && SvrCard::isValidIndex(this->_upCard) && this->_listPass.size() == this->_listHandCards.size())
        {
            // pass: face up card
            this->_pileDiscards.push_back(this->_upCard);
            this->_upCard = SvrCard::Index::CARD_NONE;
            this->_listPass.clear();

            // State get card
            this->_playerTurnState = PlayerTurnState::STATE_GET_CARD;
            HandCard* prevHand = getPreviousHandCard();
            int lastThrowCard = SvrCard::Index::CARD_NONE;
            if (this->_previousPlayOrderIndex >= 0 && this->_previousPlayOrderIndex < this->_playOrder.size())
                lastThrowCard = prevHand->getLastThrowCard();
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepGetCard(hand->index, prevHand->index, lastThrowCard, RuleTakeCard::canTake(lastThrowCard, hand->handCards), true);
#endif
            if (isBot(hand->index)) // bot
                logicBotTurnStepGetCardAction(hand, prevHand, SvrCard::Index::CARD_NONE);
        }
        else if (this->_playerTurnState == PlayerTurnState::STATE_READY && !SvrCard::isValidIndex(this->_upCard) && this->_listPass.empty())
        {
            // State get card
            this->_playerTurnState = PlayerTurnState::STATE_GET_CARD;
            HandCard* prevHand = getPreviousHandCard();
            int lastThrowCard = SvrCard::Index::CARD_NONE;
            if (this->_previousPlayOrderIndex >= 0 && this->_previousPlayOrderIndex < this->_playOrder.size())
                lastThrowCard = prevHand->getLastThrowCard();
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepGetCard(hand->index, prevHand->index, lastThrowCard, RuleTakeCard::canTake(lastThrowCard, hand->handCards), true);
#endif
            if (isBot(hand->index)) // bot
                logicBotTurnStepGetCardAction(hand, prevHand, lastThrowCard);
        }
        else if (this->_playerTurnState == PlayerTurnState::STATE_PASS && !SvrCard::isValidIndex(this->_upCard))
        {
            // State: throw card
            this->_playerTurnState = PlayerTurnState::STATE_THROW_CARD;
            pair<RuleConstant::ActionType, int> suggest = getActionTypeAndSuggestKnockPointByVariation(hand);
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepThrowCard(hand->index, suggest.first, suggest.second);
#endif
            if (isBot(hand->index)) // bot
                logicBotTurnStepThrowCardAction(hand, suggest.first);
        }
        else if (this->_playerTurnState == PlayerTurnState::STATE_GET_CARD)
        {
            // State throw card
            this->_playerTurnState = PlayerTurnState::STATE_THROW_CARD;
            pair<RuleConstant::ActionType, int> suggest = getActionTypeAndSuggestKnockPointByVariation(hand);
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepThrowCard(hand->index, suggest.first, suggest.second);
#endif
            if (isBot(hand->index)) // bot
                logicBotTurnStepThrowCardAction(hand, suggest.first);
        }
        else if (this->_playerTurnState == PlayerTurnState::STATE_THROW_CARD && (this->canKnockGin() || this->_remainCards.empty()))
        {
            this->_state = GameMatchState::POST_CHECK;
            onMatchLoop();
        }
        else
        {
            logicNextTurn();
        }
    }
}

void GinGameMatchManager::calculateMoneyEndMatch(const vector<int>& winOrderIndex, const vector<RuleLayoffCard>& suggestLayoffCards)
{
    const double     logUserMoneyBefore = SvrUserProfile::getMoney();
    map<int, double> mapMoneyRewards;
    for (const auto& hand : this->_listHandCards)
        mapMoneyRewards[hand->index] = 0;

    if (!winOrderIndex.empty())
    {
        const int winIndex = winOrderIndex.at(0);
        HandCard* winHand = this->getHandCard(winIndex);
        this->_lastWinIndex = winHand->index;

        map<int, double> winMapDetails;
        const auto& itWin = this->_mapResultDetails.find(winHand->index);
        if (itWin != this->_mapResultDetails.end())
            winMapDetails = itWin->second;

        const int handPoints = RuleCommon::getPoints(winHand->handCards, false);

        winMapDetails[RESULT_DETAIL_HAND_POINTS] = handPoints;

        // [START all lose hands]
        int loseTotalPoints = 0;
        for (const auto& i : winOrderIndex)
        {
            HandCard* loseHand = this->getHandCard(i);
            if (loseHand != nullptr && loseHand->index != winHand->index)
            {
                map<int, double> loseMapDetails;
                const auto& it = this->_mapResultDetails.find(loseHand->index);
                if (it != this->_mapResultDetails.end())
                    loseMapDetails = it->second;

                // Points
                const int loseHandPoints = RuleCommon::getPoints(loseHand->handCards, false);
                loseMapDetails[RESULT_DETAIL_HAND_POINTS] = loseHandPoints;
                this->_mapResultDetails[loseHand->index]  = loseMapDetails;
                loseTotalPoints += loseHandPoints;
            }
        }
        // [END all lose hands]

        int spadesDoubleBonusPoints = 0;
        int winTotalPoints          = loseTotalPoints - handPoints;
        winMapDetails[RESULT_DETAIL_WIN_POINTS] = winTotalPoints;
        spadesDoubleBonusPoints += RuleCommon::getPointSpadesDoubleBonus(winTotalPoints, this->_isSpadesDouble);

        // [START gin_or_big_gin]
        if (this->_knockMapAction[winHand->index] == RuleConstant::ActionType::GIN)
        {
            winMapDetails[RESULT_DETAIL_BONUS_POINTS_GIN] = BONUS_POINTS_GIN;
            winTotalPoints += BONUS_POINTS_GIN;
            spadesDoubleBonusPoints += RuleCommon::getPointSpadesDoubleBonus(BONUS_POINTS_GIN, this->_isSpadesDouble);
        }
        else if (this->_knockMapAction[winHand->index] == RuleConstant::ActionType::BIG_GIN)
        {
            winMapDetails[RESULT_DETAIL_BONUS_POINTS_BIG_GIN] = BONUS_POINTS_BIG_GIN;
            winTotalPoints += BONUS_POINTS_BIG_GIN;
            spadesDoubleBonusPoints += RuleCommon::getPointSpadesDoubleBonus(BONUS_POINTS_BIG_GIN, this->_isSpadesDouble);
        }
        // [END gin_or_big_gin]

        // [START undercut_knocker-lose_or_defender-win]
        if (canKnock() && this->_knockMapAction[winHand->index] != RuleConstant::ActionType::KNOCK)
        {
            winMapDetails[RESULT_DETAIL_BONUS_POINTS_UNDERCUT] = BONUS_POINTS_UNDERCUT;
            winTotalPoints += BONUS_POINTS_UNDERCUT;
            spadesDoubleBonusPoints += RuleCommon::getPointSpadesDoubleBonus(BONUS_POINTS_UNDERCUT, this->_isSpadesDouble);
        }
        // [END undercut_knocker-lose_or_defender-win]

        winMapDetails[RESULT_DETAIL_BONUS_POINTS_SPADES_DOUBLE] = spadesDoubleBonusPoints;

        if (winTotalPoints > 0)
        {
            winTotalPoints += spadesDoubleBonusPoints;
            this->_mapTargetPoints[winHand->index] += winTotalPoints;
        }

        // [START finish_points]
        this->_mapResultDetails[winHand->index] = winMapDetails;
        for (const auto& i : winOrderIndex)
        {
            auto loseHand = this->getHandCard(i);
            if (loseHand != nullptr && loseHand->index != winHand->index)
            {
                map<int, double> loseMapDetails;
                const auto& it = this->_mapResultDetails.find(loseHand->index);
                if (it != this->_mapResultDetails.end())
                    loseMapDetails = it->second;
                this->_mapResultDetails[loseHand->index] = loseMapDetails;
            }
        }
        // [END finish_points]

        // [START jackpot_lock]
        if (!this->_jackpotLock)
        {
            for (const auto& it : this->_mapTargetPoints)
                if (it.first != winHand->index && it.second != 0)
                    this->_jackpotLock = true;
        }
        // [END jackpot_lock]

        // [START calculate_money]
        if (this->_mapTargetPoints[winHand->index] >= this->_bundle.targetScore)
        {
            this->_isFinishTargetPoints = true;

            double winTotalBet   = 0;
            double winTotalBonus = 0;
            for (const auto& i : winOrderIndex)
            {
                HandCard* loseHand = this->getHandCard(i);
                if (loseHand != nullptr && loseHand->index != winHand->index)
                {
                    const int   scoreGap = this->_mapTargetPoints[winHand->index] - this->_mapTargetPoints[loseHand->index];
                    const float multiply = getMultiplyValueByScoreGap(scoreGap);

                    map<int, double> loseMapDetails;
                    const auto& it = this->_mapResultDetails.find(loseHand->index);
                    if (it != this->_mapResultDetails.end())
                        loseMapDetails = it->second;

                    const double winBet   = this->_mapBet[i];
                    const double winBonus = this->_mapBet[i] * (multiply - 1);
                    const double winTotal = winBet + winBonus;

                    loseMapDetails[RESULT_DETAIL_NORMAL_WIN] = -winBet;
                    loseMapDetails[RESULT_DETAIL_BONUS_WIN]  = -winBonus;
                    this->_mapResultDetails[loseHand->index] = loseMapDetails;

                    addMoneyToSlot(i, -winBet, "LoseBet");
                    addMoneyToSlot(i, -winBonus, "LoseBetMultiply");

                    mapMoneyRewards[i] = -winTotal;

                    winTotalBet += winBet;
                    winTotalBonus += winBonus;
                }
            }

            addMoneyToSlot(winHand->index, winTotalBet, "WinBet");
            addMoneyToSlot(winHand->index, winTotalBonus, "WinBetMultiply");

            winMapDetails[RESULT_DETAIL_NORMAL_WIN] = winTotalBet;
            winMapDetails[RESULT_DETAIL_BONUS_WIN]  = winTotalBonus;

            mapMoneyRewards[winHand->index] = winTotalBet + winTotalBonus;

            // Jackpot
            if (updateJackpot(winHand->index))
            {
                this->_jackpotIsWin = true;
                addMoneyToSlot(winHand->index, this->_jackpotMoney, "WinHitPot");
                winMapDetails[RESULT_DETAIL_JACKPOT_WIN] = this->_jackpotMoney;
                mapMoneyRewards[winHand->index] += this->_jackpotMoney;
            }

            this->_mapResultDetails[winHand->index] = winMapDetails;
            addExpToSlot(winHand->index, EXP_WIN);

            ServerOffline::_gameCountPerOneTimeEnterRoom++;

            if (isUser(winIndex))
            {
                ServerOffline::_winGame             = true;
                ServerOffline::_winGameRoundCount   = this->_round;
                ServerOffline::_winJackpot          = (this->_jackpotIsWin == true) ? 1 : 0;
                ServerOffline::_winScoreGapMultiply = (int) winTotalBonus / winTotalBet + 1;
            }
        }
        // [END calculate_money]
    }
    else
    {
        this->_lastWinIndex = INDEX_NONE;
    }

#if STRESS_TEST == 0
    sendMessageEndMatch(mapMoneyRewards, winOrderIndex, suggestLayoffCards);
#endif
    logData(mapMoneyRewards, winOrderIndex);
#if DEBUG_LOG_BOT
    int userMatchCount = SvrUserProfile::getMatchCount(_bundle.cityId);
    int userWinMatch   = SvrUserProfile::getCountWinMatchUser();
    int userLoseMatch  = SvrUserProfile::getCountLoseMatchUser();
    int userWinRate    = SvrRating::getConditionValue(JSON_CONDITION_FIELD_WIN_RATE);
    LogHelper::matchArgs("USER STATS", D_GET_VAR(userMatchCount, userWinMatch, userLoseMatch, userWinRate));
#endif
}

void GinGameMatchManager::logicPlayerCustomMelds()
{
    // Custom melds
    if (!this->_playerCustomMelds.empty())
    {
        const auto& hand = this->_listHandCards.at(INDEX_PLAYER);
        if (hand != nullptr)
        {
            for (const auto& cards : this->_playerCustomMelds)
            {
                if (RuleCommon::isMeld(cards))
                {
                    vector<int> sortCards = SvrCard::sortAsc(cards);
                    hand->addMeldCards(sortCards);
                    hand->removeHandCards(sortCards);
                }
            }
        }
    }

    // Auto
    for (const auto& hand : this->_listHandCards)
    {
#if USE_AUTO_PLAY_CARD == 0
        if (hand != nullptr && !hand->isUser)
#endif
        {
            for (const auto& meld : RuleCommon::getResultInfo(hand->handCards).melds)
            {
                hand->addMeldCards(meld);
                hand->removeHandCards(meld);
            }
        }
    }
}

void GinGameMatchManager::logicEndMatch()
{
    logicPlayerCustomMelds();
    clearMatchData();

    // [START layoff_cards]
    vector<RuleLayoffCard> suggestLayoffCards;
    const int              knockPlayerIndex = getKnockPlayerIndex();
    if (knockPlayerIndex > INDEX_NONE)
    {
        int      index = this->_startRoundPlayOrderIndex;
        for (int i     = 0; i < this->_playOrder.size(); i++)
        {
            const int playerIndex = this->_playOrder.at(index);
            if (playerIndex != knockPlayerIndex)
            {
                HandCard* sender = this->getHandCard(playerIndex);
                suggestLayoffCards = RuleLayoffCard::suggest(sender->handCards, this->getMapCombinationCards(knockPlayerIndex), sender->index);
                for (const auto& rgc : suggestLayoffCards)
                {
                    sender->removeHandCards(rgc.getCards());
                    sender->addLayoffCard(rgc);
                }
            }
            index                 = (index + 1) % this->_playOrder.size();
        }
    }
    // [END layoff_cards]

#if DEBUG_LOG_BOT
    for (const auto& hand : this->_listHandCards)
    {
        vector<pair<string, string>> logs;
        DebugHelper::printHandCommon(StringHelper::EMPTY_STRING, SvrCard::Index::CARD_NONE, hand, this->_knockPoints, false, logs, true);
        LogHelper::match("END MATCH - " + DebugHelper::player2Str(hand->index), logs, cocos2d::Color3B::ORANGE);
    }
#endif

    vector<int> winOrderIndex;
    if (canKnockGin())
    {
        int      index = this->_startRoundPlayOrderIndex;
        for (int i     = 0; i < this->_playOrder.size(); i++)
        {
            winOrderIndex.push_back(this->_playOrder.at(index));
            index = (index + 1) % this->_playOrder.size();
        }

        for (int i = 0; i < winOrderIndex.size(); i++)
        {
            int iPlayerIndex = winOrderIndex.at(i);
            int iPlayerPoint = this->getHandCard(iPlayerIndex)->getHandCardTotalPoints();

            for (int j = i + 1; j < winOrderIndex.size(); j++)
            {
                const int jPlayerIndex = winOrderIndex.at(j);
                const int jPlayerPoint = this->getHandCard(jPlayerIndex)->getHandCardTotalPoints();
                bool      isSwap       = false;
                if (jPlayerPoint < iPlayerPoint)
                {
                    isSwap = true;
                }
                else if (jPlayerPoint == iPlayerPoint)
                {
                    if (canKnock())
                        isSwap = isKnocker(iPlayerIndex) && isDefender(jPlayerIndex);
                    else if (canGinOrBigGin())
                        isSwap = isPlayerGinOrBigGin(jPlayerIndex);
                }
                if (isSwap)
                {
                    winOrderIndex.at(i) = jPlayerIndex;
                    winOrderIndex.at(j) = iPlayerIndex;
                    iPlayerIndex = jPlayerIndex;
                    iPlayerPoint = jPlayerPoint;
                }
            }
        }
    }
    calculateMoneyEndMatch(winOrderIndex, suggestLayoffCards);
    this->_state = GameMatchState::FINISH;
#if STRESS_TEST
    startNewMatch();
#endif
}

void GinGameMatchManager::onMatchLoop()
{
    switch (this->_state)
    {
        case GameMatchState::DEAL_CARD:this->logicDealCard();
            break;
        case GameMatchState::INGAME:this->logicInGame();
            break;
        case GameMatchState::POST_CHECK:this->logicEndMatch();
            break;
        default:break;
    }
}

bool GinGameMatchManager::isUser(int playerIndex)
{
    return playerIndex == INDEX_PLAYER;
}

bool GinGameMatchManager::isBot(int playerIndex)
{
#if USE_AUTO_PLAY_CARD == 0
    return playerIndex != INDEX_NONE && playerIndex != INDEX_PLAYER;
#else
    return true;
#endif
}

bool GinGameMatchManager::shouldDealCardAgain(const map<int, vector<int>>& cardSlot)
{
    if (SvrUserProfile::isNewUser())
    {
        const auto& result = RuleCommon::getResultInfo(cardSlot.at(INDEX_PLAYER));
        if (result.melds.empty())
            return true;

        const int totalMatch       = SvrUserProfile::getTotalMatchCount();
        bool      shouldCheckScore = false;
        if (totalMatch < 20 && ServerOffline::_consecutiveLose >= 3)
            shouldCheckScore = true;
        if (!shouldCheckScore && ServerOffline::_consecutiveLose >= 5)
            shouldCheckScore = true;
        if (!shouldCheckScore && totalMatch <= MATCH_COUNT_BOT_CANT_INSTANTLY_WIN)
            shouldCheckScore = true;
        if (shouldCheckScore)
        {
            // User
            if (totalMatch <= 3)
            {
                if (result.points >= MATCH_MAXIMUM_SCORE_OF_NEW_USER)
                    return true;
            }
            else
            {
                if (result.points >= MATCH_MAXIMUM_SCORE_OF_NEW_USER && RandomHelper::random_real(0.0f, 1.0f) <= 0.5f)
                    return true;
            }

            // Bots
            for (const int playerIndex : this->_playOrder)
            {
                if (isBot(playerIndex))
                {
                    const auto& botResult = RuleCommon::getResultInfo(cardSlot.at(playerIndex));
                    if (RuleInstantWin::canInstantWin(botResult.deadwood) || botResult.points <= 10)
                        return true;
                }
            }

            auto remainCards = SvrCard::cloneCards(this->_remainCards);
            const int& upCard = remainCards.back();
            remainCards.pop_back();
            const int& drawCard = remainCards.back();
            remainCards.pop_back();
            const int& firstPlayerIndex = this->_playOrder.at(0);
            if (isBot(firstPlayerIndex))
            {
                const auto& botHandCards = cardSlot.at(firstPlayerIndex);

                // Take a card in pass turn.
                const auto& botCards  = SvrCard::mergeCard(botHandCards, upCard);
                const auto& botResult = RuleCommon::getResultInfo(botCards);
                if (RuleInstantWin::canInstantWin(botResult.deadwood) || botResult.knockPoint <= 10)
                    return true;

                // Draw a card after the pass turn.
                const auto& botDrawCards       = SvrCard::mergeCard(botHandCards, drawCard);
                const auto& botResultDrawCards = RuleCommon::getResultInfo(botDrawCards);
                if (RuleInstantWin::canInstantWin(botResultDrawCards.deadwood) || botResultDrawCards.knockPoint <= 10)
                    return true;
            }
        }
    }
    return false;
}

bool GinGameMatchManager::shouldDealCardAgain(const vector<vector<int>>& cardSlot, const vector<int>& playOrder)
{
    map<int, vector<int>> mapSlot;
    for (int              i = playOrder.size() - 1; i >= 0; i--)
        mapSlot[playOrder.at((playOrder.size() - 1) - i)] = cardSlot.at(i);
    return shouldDealCardAgain(mapSlot);
}

HandCard* GinGameMatchManager::getCurrentHand()
{
    return this->getHandCard(this->_playOrder.at(this->_currentPlayOrderIndex));
}

bool GinGameMatchManager::canKnock()
{
    return any_of(this->_knockMapAction.begin(), this->_knockMapAction.end(), [](const pair<int, RuleConstant::ActionType>& it) -> bool {
        return it.second == RuleConstant::ActionType::KNOCK;
    });
}

bool GinGameMatchManager::canKnockGin()
{
    return any_of(this->_knockMapAction.begin(), this->_knockMapAction.end(), [](const pair<int, RuleConstant::ActionType>& it) -> bool {
        return it.second == RuleConstant::ActionType::KNOCK || it.second == RuleConstant::ActionType::GIN || it.second == RuleConstant::ActionType::BIG_GIN;
    });
}

bool GinGameMatchManager::canGinOrBigGin()
{
    return any_of(this->_knockMapAction.begin(), this->_knockMapAction.end(), [](const pair<int, RuleConstant::ActionType>& it) -> bool {
        return it.second == RuleConstant::ActionType::GIN || it.second == RuleConstant::ActionType::BIG_GIN;
    });
}

int GinGameMatchManager::getKnockPlayerIndex()
{
    for (const auto& it : this->_knockMapAction)
        if (it.second == RuleConstant::ActionType::KNOCK)
            return it.first;
    return INDEX_NONE;
}

void GinGameMatchManager::updateKnockAction(int playerIndex, RuleConstant::ActionType actionType)
{
    this->_knockMapAction[playerIndex] = actionType;
}

bool GinGameMatchManager::isKnocker(int playerIndex)
{
    return this->_knockMapAction.at(playerIndex) == RuleConstant::ActionType::KNOCK;
}

bool GinGameMatchManager::isDefender(int playerIndex)
{
    return canKnock() && this->_knockMapAction.at(playerIndex) != RuleConstant::ActionType::KNOCK;
}

bool GinGameMatchManager::isPlayerGinOrBigGin(int playerIndex)
{
    return this->_knockMapAction.at(playerIndex) == RuleConstant::ActionType::GIN || this->_knockMapAction.at(playerIndex) == RuleConstant::ActionType::BIG_GIN;
}

bool GinGameMatchManager::canKnockByVariations() const
{
    const auto& gameRule = (RuleConstant::GameRule) this->_bundle.cityId;
    return gameRule == RuleConstant::GameRule::CLASSIC_GIN || gameRule == RuleConstant::GameRule::OKLAHOMA_GIN;
}

bool GinGameMatchManager::hasJackpot() const
{
    return this->_bundle.jackpotRatioBet > 0;
}

bool GinGameMatchManager::updateJackpot(int winIndex)
{
    if (hasJackpot() && this->_isFinishTargetPoints)
    {
        bool winJackpot = false;
        for (const auto& it : this->_mapTargetPoints)
        {
            if (it.first != winIndex) // lose
            {
                if (it.second == 0)
                    winJackpot = true;
                else
                    return false;
            }
        }
        if (winJackpot)
            return true;
    }
    return false;
}

RDPlayer GinGameMatchManager::convertRDPlayer(HandCard* hand)
{
    RDPlayer player;
    if (hand != nullptr)
    {
        RuleConstant::ActionType actionType = RuleConstant::ActionType::NOT_ENOUGH_TO_KNOCK;
        if (!this->_knockMapAction.empty() && this->_knockMapAction.find(hand->index) != this->_knockMapAction.end())
            actionType = this->_knockMapAction[hand->index];

        player.index      = hand->index;
        player.cards      = hand->handCards;
        player.takeCards  = hand->getTakeCards();
        player.throwCards = hand->throwCards;
        player.actionType = actionType;
    }
    return player;
}

void GinGameMatchManager::getPlayerCustomMelds(const rapidjson::Document& data)
{
    if (data.HasMember(KEY_GIN_CUSTOM_MELD_VALUE))
        this->_playerCustomMelds = JsonHelper::getVectorVectorInt(data, KEY_GIN_CUSTOM_MELD_VALUE);
}

map<int, vector<vector<int>>> GinGameMatchManager::getMapCombinationCards(int targetPlayerIndex/* = INDEX_NONE*/)
{
    map<int, vector<vector<int>>> map;
    if (targetPlayerIndex == INDEX_NONE)
    {
        for (const auto& hand : this->_listHandCards)
            if (hand != nullptr)
                map[hand->index] = RuleCommon::getResultInfo(hand->handCards).melds;
    }
    else if (targetPlayerIndex > INDEX_NONE)
    {
        for (const auto& hand : this->_listHandCards)
            if (hand != nullptr && hand->index == targetPlayerIndex)
                map[hand->index] = hand->meldCards;
    }
    return map;
}

pair<RuleConstant::ActionType, int> GinGameMatchManager::getActionTypeAndSuggestKnockPointByVariation(HandCard* hand)
{
    if (canKnockByVariations())
    {
        if (hand->isUser)
        {
            vector<int> deadwoodCards = SvrCard::cloneCards(hand->handCards);
            for (const vector<int>& customMeld : _playerCustomMelds)
                if (RuleCommon::isMeld(customMeld))
                    SvrCard::removeCard(customMeld, deadwoodCards);
            return RuleThrowCard::getActionTypeDeadwood(deadwoodCards, this->_knockPoints);
        }
        else
            return RuleThrowCard::getActionType(hand->handCards, this->_knockPoints);
    }
    else if (this->_bundle.cityId == (int) RuleConstant::GameRule::STRAIGHT_GIN)
    {
        pair<RuleConstant::ActionType, int> result;
        if (hand->isUser)
        {
            vector<int> deadwoodCards = SvrCard::cloneCards(hand->handCards);
            for (const vector<int>& customMeld : _playerCustomMelds)
                if (RuleCommon::isMeld(customMeld))
                    SvrCard::removeCard(customMeld, deadwoodCards);
            result = RuleThrowCard::getActionTypeDeadwood(deadwoodCards, this->_knockPoints);
        }
        else
            result = RuleThrowCard::getActionType(hand->handCards, this->_knockPoints);

        if (result.first == RuleConstant::ActionType::GIN || result.first == RuleConstant::ActionType::BIG_GIN)
            return result;
        else
            return std::make_pair(RuleConstant::ActionType::NONE, -1);
    }
    return std::make_pair(RuleConstant::ActionType::NONE, -1);
}

RuleConstant::ActionType GinGameMatchManager::getActionTypeByVariation(HandCard* hand)
{
    return getActionTypeAndSuggestKnockPointByVariation(hand).first;
}

double GinGameMatchManager::getTotalBet()
{
    double totalBet = 0;
    for (const auto& kv : this->_mapBet)
        totalBet += kv.second;
    return totalBet;
}

int GinGameMatchManager::getTotalPlayer() const
{
    return this->_bundle.numPlayer + this->_bundle.numBot;
}

void GinGameMatchManager::sendMessagePlayerBeginTurn(int index, bool isUser)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_BEGIN_TURN_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, index, allocator);
    doc.AddMember(KEY_GIN_IS_USER, isUser, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerTurnStepPass(int index, bool hintTake, bool canTake)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_BEGIN_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, index, allocator);
    doc.AddMember(KEY_GIN_HINT_TAKE, hintTake, allocator);
    doc.AddMember(KEY_GIN_CAN_TAKE, canTake, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerTurnStepPassAction(int playerIndex, int cardIndex, bool result)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, allocator);
    doc.AddMember(KEY_GIN_CARD_VALUE, cardIndex, allocator);
    doc.AddMember(KEY_GIN_ACTION_RESULT, result, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerTurnStepGetCard(int playerIndex, int previousPlayerIndex, int previousPlayerCard, bool hintTake, bool canTake)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_BEGIN_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, allocator);
    doc.AddMember(KEY_GIN_PREVIOUS_PLAYER_INDEX, previousPlayerIndex, allocator);
    doc.AddMember(KEY_GIN_PREVIOUS_PLAYER_CARD, previousPlayerCard, allocator);
    doc.AddMember(KEY_GIN_HINT_TAKE, hintTake, allocator);
    doc.AddMember(KEY_GIN_CAN_TAKE, canTake, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerTurnStepGetCardAction(int playerIndex, double money, int losePlayerIndex, int card, int startRoundPlayerIndex, bool result)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, allocator);
    doc.AddMember(KEY_GIN_CARD_VALUE, card, allocator);
    doc.AddMember(KEY_GIN_REWARD_MONEY, money, allocator);
    doc.AddMember(KEY_GIN_TAKE_PLAYER_INDEX, losePlayerIndex, allocator);
    doc.AddMember(KEY_GIN_LIST_PLAYER_DATA, getAllPlayerDataJson(doc), allocator);
    doc.AddMember(KEY_GIN_START_ROUND_INDEX, startRoundPlayerIndex, allocator);
    doc.AddMember(KEY_GIN_ACTION_RESULT, result, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerTurnStepThrowCard(int playerIndex, RuleConstant::ActionType action, int knockPoints)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_BEGIN_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, allocator);
    doc.AddMember(KEY_GIN_ACTION_TYPE, (int) action, allocator);
    doc.AddMember(KEY_KNOCK_POINTS, knockPoints, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerTurnStepThrowCardActionResult(int playerIndex, HandCard* hand, bool result, int cardIndex, RuleConstant::ActionType action)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, hand->index, allocator);
    doc.AddMember(KEY_GIN_ACTION_RESULT, result, allocator);
    if (result)
        doc.AddMember(KEY_GIN_PLAYER_DATA, getPlayerDataJson(doc, hand, {}), allocator);
    doc.AddMember(KEY_GIN_CARD_VALUE, cardIndex, allocator);
    doc.AddMember(KEY_GIN_ACTION_TYPE, (int) action, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerEndTurn()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_END_TURN_RESPONSE, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessagePlayerCustomMelds(int playerIndex, RuleConstant::ActionType action, int knockPoints)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_RESPONSE, allocator);
    doc.AddMember(KEY_GIN_PLAYER_INDEX, playerIndex, allocator);
    doc.AddMember(KEY_GIN_ACTION_TYPE, (int) action, allocator);
    doc.AddMember(KEY_KNOCK_POINTS, knockPoints, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessageEndMatch(const map<int, double>& mapMoneyRewards, const vector<int>& winOrder, const vector<RuleLayoffCard>& suggestLayoffCards)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_END_MATCH_RESPONSE, allocator);
    rapidjson::Value array(rapidjson::kArrayType);

    pair<int, float> curMultiplyBonus = getCurrentMultiplyValueByScoreGap();

    for (const int& playerIndex : this->_playOrder)
    {
        HandCard* hand = getHandCard(playerIndex);
        if (hand != nullptr)
        {
            RuleConstant::ResultType resultType;
            int                      winOrderValue;
            if (!winOrder.empty())
            {
                winOrderValue = find(winOrder.begin(), winOrder.end(), hand->index) - winOrder.begin();
                resultType    = RuleConstant::ResultType::POINT;
                if (winOrderValue != -1)
                {
                    const int                winIndex   = !winOrder.empty() ? winOrder.at(0) : INDEX_NONE;
                    RuleConstant::ActionType actionType = this->_knockMapAction.at(winIndex);
                    if (actionType == RuleConstant::ActionType::NOT_ENOUGH_TO_KNOCK)
                        actionType                      = RuleConstant::ActionType::UNDERCUT;

                    if (hand->index == winIndex) // win
                        resultType = RuleCommon::getResultTypeWin(actionType);
                    else // lose
                        resultType = RuleCommon::getResultTypeLose(actionType);
                }
            }
            else
            {
                winOrderValue = INDEX_NONE;
                resultType    = RuleConstant::ResultType::DRAW;
            }
            rapidjson::Value handObject = getPlayerDataJson(doc, hand, suggestLayoffCards);
            handObject.AddMember(KEY_GIN_REWARD_MONEY, mapMoneyRewards.at(hand->index), allocator);
            handObject.AddMember(KEY_GIN_WIN_ORDER, winOrderValue, allocator);
            handObject.AddMember(KEY_GIN_RESULT_TYPE, (int) resultType, allocator);
            handObject.AddMember(KEY_GIN_RESULT_DETAIL, getPlayerResultDetailJson(doc, hand->index), allocator);
            if (curMultiplyBonus.first != INDEX_NONE)
                handObject.AddMember(KEY_MULTIPLY_BONUS, (curMultiplyBonus.first == hand->index ? curMultiplyBonus.second : -curMultiplyBonus.second), allocator);
            else
                handObject.AddMember(KEY_MULTIPLY_BONUS, 0, allocator);
            array.PushBack(handObject, allocator);
        }
    }
    doc.AddMember(KEY_GIN_LIST_PLAYER_DATA, array, allocator);
    if (hasJackpot())
        doc.AddMember(KEY_JACKPOT, getJackpotDataJson(allocator), allocator);
    ServerManager::getInstance()->sendMessage(doc);

#if DEBUG_LOG_BOT
    DebugHelper::printMatchResult(this->_listHandCards, this->_lastWinIndex, winOrder, this->_mapResultDetails, this->_mapTargetPoints, this->_bundle.targetScore, this->_isSpadesDouble, this->hasJackpot());
#endif
}

void GinGameMatchManager::sendMessageCheatCard(int slotIdx)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHEAT_CARD_RESPONSE, allocator);
    doc.AddMember(KEY_CHEAT_CARD_USER_ID, slotIdx, allocator);
    doc.AddMember(KEY_CHEAT_CARD_FIRST_MOVE, this->_cheatFirstMoveSlotIdx, allocator);
    doc.AddMember(KEY_CHEAT_CARD_CHEATED_CARDS, JsonHelper::makeValueArrayInt(doc, this->_mapCheatCardPlayers[slotIdx]), allocator);
    vector<int> listCheat;
    for (int    i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        for (int j : this->_mapCheatCardPlayers[i])
            listCheat.push_back(j);
    const auto& listRemainCards = SvrCard::initAllCardsWithExcludes(listCheat);
    doc.AddMember(KEY_CHEAT_CARD_REMAIN_CARDS, JsonHelper::makeValueArrayInt(doc, listRemainCards), allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

void GinGameMatchManager::sendMessageQuitMatch(int result)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_QUIT_IN_GAME_RESPONSE, allocator);
    doc.AddMember(KEY_RESULT, result, allocator);
    ServerManager::getInstance()->sendMessage(doc);
}

rapidjson::Value GinGameMatchManager::getAllPlayerDataJson(rapidjson::Document& doc)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& listHandCard : this->_listHandCards)
        array.PushBack(getPlayerDataJson(doc, listHandCard, {}), allocator);
    return array;
}

rapidjson::Value GinGameMatchManager::getPlayerDataJson(rapidjson::Document& doc, HandCard* hand, const vector<RuleLayoffCard>& suggestLayoffCards)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value handObject(rapidjson::kObjectType);
    handObject.AddMember(KEY_GIN_PLAYER_INDEX, hand->index, allocator);
    handObject.AddMember(KEY_GIN_IS_USER, hand->isUser, allocator);
    handObject.AddMember(KEY_MONEY, hand->isUser ? SvrUserProfile::getMoney() : getBotInfo(hand->index).money, allocator);
    rapidjson::Value listHandCards(rapidjson::kArrayType);
    if (!suggestLayoffCards.empty())
    {
        vector<int> handCardsBeforeGive(hand->handCards.begin(), hand->handCards.end());
        for (const auto& suggestLayoffCard : suggestLayoffCards)
            if (suggestLayoffCard.getSenderIndex() == hand->index)
                for (const int& card : suggestLayoffCard.getCards())
                    if (!SvrCard::containsCard(card, handCardsBeforeGive))
                        handCardsBeforeGive.push_back(card);
        SvrCard::sortAsc(handCardsBeforeGive);
        for (const int& card : handCardsBeforeGive)
            listHandCards.PushBack(card, allocator);
    }
    else
    {
        for (const int& card : hand->handCards)
            listHandCards.PushBack(card, allocator);
    }
    handObject.AddMember(KEY_GIN_HAND_CARDS, listHandCards, allocator);
    handObject.AddMember(KEY_GIN_THROW_CARDS, JsonHelper::makeValueArrayInt(doc, hand->throwCards), allocator);
    handObject.AddMember(KEY_GIN_LIST_COMBINATION, JsonHelper::makeValueArrayArrayInt(doc, hand->meldCards), allocator);

    int level    = 0;
    int levelExp = 0;
    int nextExp  = 0;
    if (hand->isUser)
    {
        level    = SvrLevel::getLevel();
        levelExp = SvrLevel::getLevelExp();
        nextExp  = SvrLevel::getLevelExp(level + 1).second;
    }
    handObject.AddMember(KEY_LEVEL, level, allocator);
    handObject.AddMember(KEY_LEVELEXP, levelExp, allocator);
    handObject.AddMember(KEY_NEXTLEVELEXP, nextExp, allocator);

    rapidjson::Value layoffCardsValue(rapidjson::kArrayType);
    for (const auto& suggestLayoffCard : suggestLayoffCards)
    {
        if (suggestLayoffCard.getSenderIndex() == hand->index)
        {
            rapidjson::Value data(rapidjson::kObjectType);
            data.AddMember(KEY_GIN_GIVE_CARD_VALUES, JsonHelper::makeValueArrayInt(doc, suggestLayoffCard.getCards()), allocator);
            data.AddMember(KEY_GIN_GIVE_CARD_PLAYER_INDEX, suggestLayoffCard.getPosition(), allocator);
            data.AddMember(KEY_GIN_GIVE_CARD_PLAYER_COMBINATION_INDEX, suggestLayoffCard.getMeldIndex(), allocator);
            layoffCardsValue.PushBack(data, allocator);
        }
    }
    handObject.AddMember(KEY_GIN_GIVE_CARD_DATA, layoffCardsValue, allocator);
    return handObject;
}

rapidjson::Value GinGameMatchManager::getPlayerResultDetailJson(rapidjson::Document& doc, int index)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value handObject(rapidjson::kObjectType);
    const auto& it = this->_mapResultDetails.find(index);
    if (it != this->_mapResultDetails.end())
    {
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_TARGET_POINTS, this->_mapTargetPoints.at(index), allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_HAND_POINTS, (int) it->second[RESULT_DETAIL_HAND_POINTS], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_WIN_POINTS, (int) it->second[RESULT_DETAIL_WIN_POINTS], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_BONUS_POINTS_GIN, it->second[RESULT_DETAIL_BONUS_POINTS_GIN], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_BONUS_POINTS_BIG_GIN, it->second[RESULT_DETAIL_BONUS_POINTS_BIG_GIN], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_BONUS_POINTS_UNDERCUT, it->second[RESULT_DETAIL_BONUS_POINTS_UNDERCUT], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_BONUS_POINTS_SPADES_BONUS, it->second[RESULT_DETAIL_BONUS_POINTS_SPADES_DOUBLE], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_NORMAL_WIN, it->second[RESULT_DETAIL_NORMAL_WIN], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_BONUS_WIN, it->second[RESULT_DETAIL_BONUS_WIN], allocator);
        handObject.AddMember(KEY_GIN_RESULT_DETAIL_JACKPOT_WIN, it->second[RESULT_DETAIL_JACKPOT_WIN], allocator);
    }
    return handObject;
}

rapidjson::Value GinGameMatchManager::getJackpotDataJson(rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::Value jackpotObject(rapidjson::kObjectType);
    jackpotObject.AddMember(KEY_JACKPOT_MONEY, this->_jackpotMoney, allocator);
    jackpotObject.AddMember(KEY_JACKPOT_ROUND, this->_jackpotRound, allocator);
    jackpotObject.AddMember(KEY_JACKPOT_LOCK, this->_jackpotLock, allocator);
    return jackpotObject;
}

void GinGameMatchManager::onMessagePlayerBeginTurn(int playerIndex)
{
    if (_playerTurnState == PlayerTurnState::STATE_READY)
        logicPlayerTurnStep(playerIndex);
}

void GinGameMatchManager::onMessagePlayerTurnStepPassAction(int playerIndex, int cardIndex)
{
    HandCard* hand = getCurrentHand();
    if (hand && this->_playerTurnStateDone != PlayerTurnState::STATE_PASS && hand->index == playerIndex && SvrCard::isValidIndex(this->_upCard))
    {
        if (cardIndex == this->_upCard) // take face up card
        {
            hand->addHandCard(cardIndex);
            hand->addTakeCard(cardIndex);
            SvrCard::removeCard(cardIndex, this->_pileDiscards);
            this->_upCard = SvrCard::Index::CARD_NONE;
            this->_listPass.clear();
            sendMessagePlayerTurnStepPassAction(hand->index, cardIndex, true);
#if DEBUG_LOG_BOT
            DebugHelper::printHandAction("Take", cardIndex, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints, true, false, true);
#endif
        }
        else
        { // pass
            sendMessagePlayerTurnStepPassAction(hand->index, SvrCard::Index::CARD_NONE, true);
#if DEBUG_LOG_BOT
            DebugHelper::printHandAction("Pass", cardIndex, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints);
#endif
        }
        this->_playerTurnStateDone = PlayerTurnState::STATE_PASS;
#if STRESS_TEST || USE_AUTO_PLAY_CARD
        onMessagePlayerTurnStepPassDone(playerIndex);
#endif
    }
}

void GinGameMatchManager::onMessagePlayerTurnStepPassDone(int playerIndex)
{
    if (_playerTurnState == PlayerTurnState::STATE_PASS)
        logicPlayerTurnStep(playerIndex);
}

void GinGameMatchManager::onMessagePlayerTurnStepGetCardAction(int playerIndex, int playerActionIndex)
{
    HandCard* hand = getCurrentHand();
    if (hand && this->_playerTurnStateDone != PlayerTurnState::STATE_GET_CARD && playerIndex == hand->index)
    {
        int  prevPlayerIndex = INDEX_NONE;
        int  cardIndex       = SvrCard::Index::CARD_NONE;
        bool result          = false;
        if (hand->handCards.size() == RuleConstant::Common::MAX_HAND_CARD)
        {
            if (this->_playOrder.at(this->_previousPlayOrderIndex) == playerActionIndex) // Take card
            {
                prevPlayerIndex = playerActionIndex;
                HandCard* prevHand = getHandCard(prevPlayerIndex);
                cardIndex = prevHand->throwCards.back();

                result = true;
                prevHand->throwCards.pop_back();

                hand->addHandCard(cardIndex);
                hand->addTakeCard(cardIndex);
                SvrCard::removeCard(cardIndex, this->_pileDiscards);

                // update KNOCK
                if (canKnockByVariations() && RuleKnocking::canKnock(this->_knockPoints, hand->handCards))
                    updateKnockAction(hand->index, RuleConstant::ActionType::KNOCK);

                this->_playerTurnStateDone = PlayerTurnState::STATE_GET_CARD;
#if DEBUG_LOG_BOT
                DebugHelper::printHandAction("Take", cardIndex, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints, true);
#endif
            }
            else // Pick card
            {
                cardIndex = !this->_remainCards.empty() ? this->_remainCards.back() : SvrCard::Index::CARD_NONE;
                if (SvrCard::isValidIndex(cardIndex))
                {
                    result = true;

                    this->_remainCards.pop_back();
                    hand->addHandCard(cardIndex);

                    // update KNOCK
                    if (canKnockByVariations() && RuleKnocking::canKnock(this->_knockPoints, hand->handCards))
                        updateKnockAction(hand->index, RuleConstant::ActionType::KNOCK);

                    this->_playerTurnStateDone = PlayerTurnState::STATE_GET_CARD;
#if DEBUG_LOG_BOT
                    DebugHelper::printHandAction("Pick", cardIndex, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints, true, true);
#endif
                }
            }
        }
#if STRESS_TEST == 0
        sendMessagePlayerTurnStepGetCardAction(hand->index, 0, prevPlayerIndex, cardIndex, this->_playOrder.at(this->_startRoundPlayOrderIndex), result);
#else
        if (result)
            onMessagePlayerTurnStepGetCardDone(hand->index);
        else
            assert("Something Wrong!!!");
#endif
#if DEBUG_DEAL_CARD
        if (result)
            assert("Something Wrong!!!");
#endif
    }
}

void GinGameMatchManager::onMessagePlayerTurnStepGetCardDone(int playerIndex)
{
    if (this->_playerTurnState == PlayerTurnState::STATE_GET_CARD)
        logicPlayerTurnStep(playerIndex);
}

void GinGameMatchManager::onMessagePlayerTurnStepThrowCardAction(int playerIndex, int cardIndex, RuleConstant::ActionType actionType)
{
    HandCard* hand = getCurrentHand();
    if (hand && this->_playerTurnStateDone != PlayerTurnState::STATE_THROW_CARD && hand->index == playerIndex)
    {
        if (actionType == RuleConstant::ActionType::BIG_GIN && actionType == getActionTypeByVariation(hand))
        {
            // update action
            updateKnockAction(hand->index, actionType);
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepThrowCardActionResult(playerIndex, hand, true, SvrCard::Index::CARD_NONE, actionType);
            sendMessagePlayerEndTurn();
#endif
            this->_playerTurnStateDone = PlayerTurnState::STATE_THROW_CARD;
#if DEBUG_LOG_BOT
            DebugHelper::printHandAction("BIG GIN!", SvrCard::Index::CARD_NONE, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints);
#endif
#if STRESS_TEST
            onMessagePlayerTurnStepThrowCardDone(playerIndex);
#endif
        }
        else if (actionType == RuleConstant::ActionType::GIN && actionType == getActionTypeByVariation(hand))
        {
            int throwCard;
            if (hand->isUser)
            {
                vector<int> deadwoodCards = SvrCard::cloneCards(hand->handCards);
                for (const vector<int>& customMeld : _playerCustomMelds)
                    if (RuleCommon::isMeld(customMeld))
                        SvrCard::removeCard(customMeld, deadwoodCards);
                throwCard = SvrCard::getMaxIndex(deadwoodCards);
            }
            else
            {
                throwCard = RuleInstantWin::getDeadwood(hand->handCards);
            }
            hand->removeHandCard(throwCard);
            hand->removeTakeCard(throwCard);
            hand->addThrowCard(throwCard);
            this->_pileDiscards.push_back(throwCard);

            // update action
            updateKnockAction(hand->index, actionType);
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepThrowCardActionResult(playerIndex, hand, true, throwCard, actionType);
            sendMessagePlayerEndTurn();
#endif
            this->_playerTurnStateDone = PlayerTurnState::STATE_THROW_CARD;
#if DEBUG_LOG_BOT
            DebugHelper::printHandAction("GIN!", throwCard, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints);
#endif
#if STRESS_TEST
            onMessagePlayerTurnStepThrowCardDone(playerIndex);
#endif
        }
        else if (actionType == RuleConstant::ActionType::KNOCK && actionType == getActionTypeByVariation(hand))
        {
            int throwCard;
            if (hand->isUser)
            {
                vector<int> deadwoodCards = SvrCard::cloneCards(hand->handCards);
                for (const vector<int>& customMeld : _playerCustomMelds)
                    if (RuleCommon::isMeld(customMeld))
                        SvrCard::removeCard(customMeld, deadwoodCards);
                throwCard = SvrCard::getMaxIndex(deadwoodCards);
            }
            else
                throwCard = RuleCommon::getResultInfo(hand->handCards).getHighestDeadwood();

            hand->removeHandCard(throwCard);
            hand->removeTakeCard(throwCard);
            hand->addThrowCard(throwCard);
            this->_pileDiscards.push_back(throwCard);
            // update action
            updateKnockAction(hand->index, actionType);
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepThrowCardActionResult(playerIndex, hand, true, throwCard, actionType);
            sendMessagePlayerEndTurn();
#endif
            this->_playerTurnStateDone = PlayerTurnState::STATE_THROW_CARD;
#if DEBUG_LOG_BOT
            DebugHelper::printHandAction("KNOCK!", throwCard, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints);
#endif
#if STRESS_TEST
            onMessagePlayerTurnStepThrowCardDone(playerIndex);
#endif
        }
        else if (SvrCard::isValidIndex(cardIndex) && RuleThrowCard::canThrow(cardIndex, hand->handCards))
        {
            hand->removeHandCard(cardIndex);
            hand->removeTakeCard(cardIndex);
            hand->addThrowCard(cardIndex);
            this->_pileDiscards.push_back(cardIndex);
            // update KNOCK
            if (canKnockByVariations())
            {
                if (actionType == RuleConstant::ActionType::KNOCK)
                    updateKnockAction(hand->index, actionType);
                else
                    updateKnockAction(hand->index, RuleConstant::ActionType::NOT_ENOUGH_TO_KNOCK);
            }
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepThrowCardActionResult(playerIndex, hand, true, cardIndex, actionType);
            sendMessagePlayerEndTurn();
#endif
            this->_playerTurnStateDone = PlayerTurnState::STATE_THROW_CARD;
#if DEBUG_LOG_BOT
            DebugHelper::printHandAction("Dump", cardIndex, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints);
#endif
#if STRESS_TEST
            onMessagePlayerTurnStepThrowCardDone(playerIndex);
#endif
        }
        else
        {
#if STRESS_TEST == 0
            sendMessagePlayerTurnStepThrowCardActionResult(playerIndex, hand, false, cardIndex, actionType);
#endif
#if DEBUG_LOG_BOT
            DebugHelper::printHandAction("ERROR!!! Dump", cardIndex, hand, this->_pileDiscards, this->_remainCards, this->_knockPoints);
#endif
        }
    }
}

void GinGameMatchManager::onMessagePlayerTurnStepThrowCardDone(int playerIndex)
{
    if (this->_playerTurnState == PlayerTurnState::STATE_THROW_CARD)
        logicPlayerTurnStep(playerIndex);
}

void GinGameMatchManager::onMessageCheatCard(const rapidjson::Document& data)
{
#if USE_CHEAT_CARD
    int           slotIdx = JsonHelper::getInt(data, KEY_CHEAT_CARD_USER_ID, INDEX_NONE);
    int           cardIdx = JsonHelper::getInt(data, KEY_CHEAT_CARD_CARD_ID, SvrCard::Index::CARD_NONE);
    CheatCardType type    = (CheatCardType) JsonHelper::getInt(data, KEY_CHEAT_CARD_TYPE, 0);
    if (type == CheatCardType::CHEAT_CARD_TYPE_FIRST_MOVE)
    {
        if (cardIdx == 1)
        {
            this->_cheatFirstMoveSlotIdx = slotIdx;
        }
        else
        {
            this->_cheatFirstMoveSlotIdx     = INDEX_NONE;
            if (slotIdx == 0)
                this->_cheatFirstMoveSlotIdx = 0;
        }
    }
    else if (type == CheatCardType::CHEAT_CARD_TYPE_CLEAR)
    {
        this->_mapCheatCardPlayers.clear();
        this->_cheatFirstMoveSlotIdx = INDEX_NONE;
    }

    vector<int> listCards = this->_mapCheatCardPlayers[slotIdx];
    const int   length    = listCards.size() - RuleConstant::Common::MAX_HAND_CARD;
    for (int    i         = 0; i < length; i++)
        listCards.pop_back();

    if (SvrCard::isValidIndex(cardIdx))
    {
        if (type == CheatCardType::CHEAT_CARD_TYPE_ADD)
        {
            if (this->_cheatFirstMoveSlotIdx == INDEX_NONE)
                this->_cheatFirstMoveSlotIdx = 0;
            if (listCards.size() >= RuleConstant::Common::MAX_HAND_CARD)
                return;
            if (find(listCards.begin(), listCards.end(), cardIdx) == listCards.end())
                listCards.push_back(cardIdx);
        }
        else if (type == CheatCardType::CHEAT_CARD_TYPE_REMOVE)
        {
            for (int i = 0; i < listCards.size(); i++)
            {
                if (listCards.at(i) == cardIdx)
                {
                    listCards.erase(listCards.begin() + i);
                    break;
                }
            }
        }
        else if (type == CheatCardType::CHEAT_CARD_TYPE_REMOVE)
        {
            listCards.clear();
        }
    }
    this->_mapCheatCardPlayers[slotIdx] = listCards;

    vector<int> listCheat;
    for (int    i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        const vector<int>& card = this->_mapCheatCardPlayers[i];
        for (int c : card)
            listCheat.push_back(c);
    }
    if (listCheat.empty())
        this->_cheatFirstMoveSlotIdx = INDEX_NONE;
    sendMessageCheatCard(slotIdx);
#endif
}

void GinGameMatchManager::addMoneyToSlot(int index, double money, const string& name)
{
    if (isUser(index))
    {
        ServerOffline::_winBetMultiple += money / _bundle.price;
        SvrUserProfile::setMoney(name, money);
    }
    else
    {
        SvrBotData& botInfo = getBotInfo(index);
        botInfo.money += money;
        botInfo.money       = (botInfo.money < 0) ? 0 : botInfo.money;
    }
}

void GinGameMatchManager::addExpToSlot(int index, int exp)
{
    if (isUser(index))
        SvrLevel::addLevelExp(exp);
}

void GinGameMatchManager::logicBotTurnStepPassAction(HandCard* botHand, int upCard)
{
    const auto& previousTurn = convertRDPlayer(getPreviousHandCard());
    const auto& currentTurn  = convertRDPlayer(botHand);
    const bool isBeginTurn = (this->_listPass.size() == 1);
    botPassRunAction(botHand->index, BotHelper::pass(this->_levelBot, upCard, previousTurn, currentTurn, this->_remainCards, isBeginTurn));
}

void GinGameMatchManager::logicBotTurnStepGetCardAction(HandCard* botHand, HandCard* previousHand, int previousCard)
{
    const auto& previousTurn = convertRDPlayer(previousHand);
    const auto& currentTurn  = convertRDPlayer(botHand);
    const int takeCard    = BotHelper::takeCard(this->_levelBot, previousCard, previousTurn, currentTurn, this->_remainCards);
    const int playerIndex = SvrCard::isValidIndex(takeCard) ? previousHand->index : INDEX_NONE;
    botGetCardRunAction(botHand->index, playerIndex);
}

void GinGameMatchManager::logicBotTurnStepThrowCardAction(HandCard* botHand, RuleConstant::ActionType actionType)
{
    const auto& previousTurn = convertRDPlayer(getPreviousHandCard());
    const auto& currentTurn  = convertRDPlayer(botHand);
    if (actionType == RuleConstant::ActionType::GIN)
    {
        if (BotHelper::gin(this->_levelBot, previousTurn, currentTurn, (RuleConstant::GameRule) this->_bundle.cityId))
        {
            botThrowCardRunAction(botHand->index, SvrCard::Index::CARD_NONE, actionType);
            return;
        }
        actionType = RuleConstant::ActionType::NONE;
    }
    else if (actionType == RuleConstant::ActionType::BIG_GIN)
    {
        if (BotHelper::bigGin(this->_levelBot, previousTurn, currentTurn))
        {
            botThrowCardRunAction(botHand->index, SvrCard::Index::CARD_NONE, actionType);
            return;
        }
        actionType = RuleConstant::ActionType::NONE;
    }
    else if (actionType == RuleConstant::ActionType::KNOCK)
    {
        if (canKnockByVariations())
        {
            int throwCard = BotHelper::knock(this->_levelBot, previousTurn, currentTurn, (RuleConstant::GameRule) this->_bundle.cityId, this->_knockPoints);
            if (SvrCard::isValidIndex(throwCard))
            {
                botThrowCardRunAction(botHand->index, throwCard, actionType);
                return;
            }
        }
        actionType = RuleConstant::ActionType::NONE;
    }
    botThrowCardRunAction(botHand->index, BotHelper::throwCard(this->_levelBot, previousTurn, currentTurn, (RuleConstant::GameRule) this->_bundle.cityId, this->_pileDiscards), actionType);
}

void GinGameMatchManager::botPassRunAction(int playerIndex, int cardIndex)
{
#if STRESS_TEST == 0
    Vector<FiniteTimeAction*> seq;
    seq.pushBack(DelayTime::create(GameUtils::randomDoubleInRange(BOT_PASS_ACTION_DELAY_DURATION_MIN, BOT_PASS_ACTION_DELAY_DURATION_MAX)));
    seq.pushBack(CallFunc::create([=]() { onMessagePlayerTurnStepPassAction(playerIndex, cardIndex); }));
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(seq));
#else
    onMessagePlayerTurnStepPassAction(playerIndex, cardIndex);
#endif
}

void GinGameMatchManager::botGetCardRunAction(int playerIndex, int playerActionIndex)
{
#if STRESS_TEST == 0
    Vector<FiniteTimeAction*> seq;
    seq.pushBack(DelayTime::create(GameUtils::randomDoubleInRange(BOT_GET_CARD_ACTION_DELAY_DURATION_MIN, BOT_GET_CARD_ACTION_DELAY_DURATION_MAX)));
    seq.pushBack(CallFunc::create([=]() { onMessagePlayerTurnStepGetCardAction(playerIndex, playerActionIndex); }));
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(seq));
#else
    onMessagePlayerTurnStepGetCardAction(playerIndex, playerActionIndex);
#endif
}

void GinGameMatchManager::botThrowCardRunAction(int playerIndex, int throwCard, RuleConstant::ActionType actionType)
{
#if STRESS_TEST == 0
    Vector<FiniteTimeAction*> seq;
    seq.pushBack(DelayTime::create(GameUtils::randomDoubleInRange(BOT_THROW_CARD_ACTION_DELAY_DURATION_MIN, BOT_THROW_CARD_ACTION_DELAY_DURATION_MAX)));
    seq.pushBack(CallFunc::create([=]() { onMessagePlayerTurnStepThrowCardAction(playerIndex, throwCard, actionType); }));
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(seq));
#else
    onMessagePlayerTurnStepThrowCardAction(playerIndex, throwCard, actionType);
#endif
}

void GinGameMatchManager::saveMatchData()
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    doc.AddMember(KEY_PLACE_ID, this->_bundle.cityId, allocator);
    doc.AddMember(KEY_TABLE_ID, this->_bundle.tableId, allocator);
    doc.AddMember(KEY_BET, this->_bundle.price, allocator);
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& hand : this->_listHandCards)
        if (hand != nullptr)
            array.PushBack(getPlayerDataJson(doc, hand, {}), allocator);
    doc.AddMember(KEY_GIN_LIST_PLAYER_DATA, array, allocator);
    SvrUserProfile::setPlayingMatchData(JsonHelper::convertDocumentToString(doc));
    cocos2d::UserDefault::getInstance()->setBoolForKey(KEY_HAS_SAVED_MATCH, true);
}

void GinGameMatchManager::clearMatchData()
{
    SvrUserProfile::setPlayingMatchData("");
    cocos2d::UserDefault::getInstance()->setBoolForKey(KEY_HAS_SAVED_MATCH, false);
}

void GinGameMatchManager::logUser(bool isWin)
{
    if (isWin)
    {
        ServerOffline::_consecutiveWin++;
        ServerOffline::_consecutiveLose = 0;
        SvrUserProfile::setCountWinMatchUser(SvrUserProfile::getCountWinMatchUser() + 1);

        if (_isFinishTargetPoints)
            SvrUserProfile::setCountWinGameUser(SvrUserProfile::getCountWinGameUser() + 1);
    }
    else
    {
        ServerOffline::_consecutiveWin = 0;
        ServerOffline::_consecutiveLose++;
        SvrUserProfile::setCountLoseMatchUser(SvrUserProfile::getCountLoseMatchUser() + 1);

        if (_isFinishTargetPoints)
            SvrUserProfile::setCountLoseGameUser(SvrUserProfile::getCountLoseGameUser() + 1);
    }
}

void GinGameMatchManager::logData(const map<int, double>& mapMoneyRewards, const vector<int>& winOrder)
{
    // User
    if (mapMoneyRewards.at(INDEX_PLAYER) > 0)
        logUser(true);
    else if (mapMoneyRewards.at(INDEX_PLAYER) < 0)
        logUser(false);
    else
        logUser(this->_lastWinIndex == INDEX_PLAYER);

    // Bot
    double botRewardMoney = 0;
    for (const auto& it : mapMoneyRewards)
        if (isBot(it.first))
            botRewardMoney += it.second;
    if (botRewardMoney > 0)
        Firebase::logBotWin(BotHelper::getVersion(), this->_levelBot, botRewardMoney, this->_bundle.cityId);
    else if (botRewardMoney < 0)
        Firebase::logBotLose(BotHelper::getVersion(), this->_levelBot, botRewardMoney, this->_bundle.cityId);
    else
    {
        if (isUser(this->_lastWinIndex))
            Firebase::logBotLose(BotHelper::getVersion(), this->_levelBot, 0, this->_bundle.cityId);
        else
            Firebase::logBotWin(BotHelper::getVersion(), this->_levelBot, 0, this->_bundle.cityId);
    }

    if (this->_jackpotRound == 1)
        SvrUserProfile::addGameCount(this->_bundle.cityId);

#if DEBUG_LOG_BOT
    if (this->_isFinishTargetPoints && this->hasJackpot())
        BotLogger::logCount(BotLogger::Event::JACKPOT_COUNT);

    if (botRewardMoney > 0)
    {
        BotLogger::logCount(BotLogger::Event::MATCH_WIN);
        BotLogger::logMoney(BotLogger::Event::MATCH_WIN, botRewardMoney);
    }
    else if (botRewardMoney < 0)
    {
        BotLogger::logCount(BotLogger::Event::MATCH_LOSE);
        BotLogger::logMoney(BotLogger::Event::MATCH_LOSE, botRewardMoney);
    }
    else // botRewardMoney == 0
    {
        if (this->_lastWinIndex == INDEX_NONE)
            BotLogger::logCount(BotLogger::Event::MATCH_DRAW);
        else if (this->_lastWinIndex == INDEX_PLAYER)
            BotLogger::logCount(BotLogger::Event::MATCH_LOSE);
        else
            BotLogger::logCount(BotLogger::Event::MATCH_WIN);
    }

    for (const auto& it : this->_mapResultDetails)
    {
        if (isBot(it.first))
        {
            if (!winOrder.empty())
            {
                int winOrderValue = find(winOrder.begin(), winOrder.end(), it.first) - winOrder.begin();
                if (winOrderValue != -1)
                {
                    const int winIndex   = !winOrder.empty() ? winOrder.at(0) : INDEX_NONE;
                    auto      actionType = this->_knockMapAction.at(winIndex);
                    if (actionType == RuleConstant::ActionType::NOT_ENOUGH_TO_KNOCK)
                        actionType = RuleConstant::ActionType::UNDERCUT;

                    if (it.first == winIndex) // bot win
                    {
                        BotLogger::logCount(BotLogger::convertEvent(RuleCommon::getResultTypeWin(actionType)));
                        switch (actionType)
                        {
                            case RuleConstant::ActionType::KNOCK:BotLogger::logCount(BotLogger::Event::ACTION_KNOCK_WIN);
                                break;
                            case RuleConstant::ActionType::GIN:BotLogger::logCount(BotLogger::Event::ACTION_GIN_WIN);
                                break;
                            case RuleConstant::ActionType::BIG_GIN:BotLogger::logCount(BotLogger::Event::ACTION_BIG_GIN_WIN);
                                break;
                            default:break;
                        }

                        if (this->hasJackpot() && it.second.at(RESULT_DETAIL_JACKPOT_WIN) > 0)
                        {
                            BotLogger::logCount(BotLogger::Event::JACKPOT_WIN);
                            BotLogger::logMoney(BotLogger::Event::JACKPOT_WIN, it.second.at(RESULT_DETAIL_JACKPOT_WIN));
                        }
                    }
                    else // bot lose
                    {
                        BotLogger::logCount(BotLogger::convertEvent(RuleCommon::getResultTypeLose(actionType)));
                        switch (this->_knockMapAction.at(it.first))
                        {
                            case RuleConstant::ActionType::KNOCK:BotLogger::logCount(BotLogger::Event::ACTION_KNOCK_LOSE);
                                break;
                            case RuleConstant::ActionType::GIN:BotLogger::logCount(BotLogger::Event::ACTION_GIN_LOSE);
                                break;
                            case RuleConstant::ActionType::BIG_GIN:BotLogger::logCount(BotLogger::Event::ACTION_BIG_GIN_LOSE);
                                break;
                            default:break;
                        }
                    }
                }
            }
            else
            {
                BotLogger::logCount(BotLogger::Event::MATCH_DRAW);
            }
        }
    }

    BotLogger::logPrint(this->_levelBot);
#endif
}

vector<string> GinGameMatchManager::getCurrentAvatarChecked(int excludeSlotIndex)
{
    vector<string> result;
    if (excludeSlotIndex != INDEX_PLAYER)
        result.push_back(SvrUserProfile::getAvatar());
    for (int i = 0; i < _listBotInfo.size(); i++)
        if (excludeSlotIndex != i + 1)
            result.push_back(_listBotInfo[i].avatar);
    return result;
}

SvrBotData& GinGameMatchManager::getBotInfo(int playerIndex)
{
    static SvrBotData defaultValue;
    if (playerIndex == INDEX_PLAYER || playerIndex == INDEX_NONE)
        return defaultValue;
    int index = playerIndex - 1;
    if (index >= 0 && index < _listBotInfo.size())
        return _listBotInfo[index];
    return defaultValue;
}

float GinGameMatchManager::getMaxMultiplyValue() const
{
    if (s_targetScoreDataRange.find((int) 0) != s_targetScoreDataRange.end())
    {
        const map<int, vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) 0);
        int cityType = (int) _bundle.cityId;
        if (map.find((int) _bundle.cityId) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        float maxValue = 1;
        for (const auto& scoreData : map.at(cityType))
            if (scoreData._targetScore == _bundle.targetScore)
                for (const auto& it : scoreData._multiplyDataMap)
                    if (maxValue < it.second)
                        maxValue = it.second;
        return maxValue;
    }
    return 1;
}

float GinGameMatchManager::getMultiplyValueByScoreGap(int scoreGap) const
{
    float multiply = 1;
    if (s_targetScoreDataRange.find((int) 0) != s_targetScoreDataRange.end())
    {
        const map<int, vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) 0);
        int cityType = (int) _bundle.cityId;
        if (map.find((int) _bundle.cityId) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        for (const auto& scoreData : map.at(cityType))
            if (scoreData._targetScore == _bundle.targetScore)
                for (const auto& it : scoreData._multiplyDataMap)
                {
                    if (scoreGap >= it.first)
                        multiply = it.second;
                    else
                        break;
                }
    }
    return multiply;
}

float GinGameMatchManager::getJackpotMultiplyValue(int cityId, int targetScore) const
{
    if (s_targetScoreDataRange.find((int) 0) != s_targetScoreDataRange.end())
    {
        const map<int, vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) 0);
        int cityType = cityId;
        if (map.find(cityId) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        for (const auto& scoreData : map.at(cityType))
            if (scoreData._targetScore == targetScore)
                return scoreData._jackpotBetMultiply;
    }
    return 0;
}

pair<int, float> GinGameMatchManager::getCurrentMultiplyValueByScoreGap()
{
    if (this->_listHandCards.size() == MAXIMUM_NUMBER_SLOT_GAME)
    {
        const auto& playerIndex1 = this->_listHandCards.at(0)->index;
        const auto& playerIndex2 = this->_listHandCards.at(1)->index;

        int winIndex;
        int loseIndex;
        if (this->_mapTargetPoints[playerIndex1] > this->_mapTargetPoints[playerIndex2])
        {
            winIndex  = playerIndex1;
            loseIndex = playerIndex2;
        }
        else if (this->_mapTargetPoints[playerIndex1] < this->_mapTargetPoints[playerIndex2])
        {
            winIndex  = playerIndex2;
            loseIndex = playerIndex1;
        }
        else
        {
            winIndex  = INDEX_NONE;
            loseIndex = INDEX_NONE;
        }

        if (winIndex != INDEX_NONE && loseIndex != INDEX_NONE)
        {
            const int scoreGap = this->_mapTargetPoints[winIndex] - this->_mapTargetPoints[loseIndex];
            const int multiply = getMultiplyValueByScoreGap(scoreGap);
            return std::make_pair(winIndex, multiply);
        }
    }
    return std::make_pair(INDEX_NONE, 0);
}

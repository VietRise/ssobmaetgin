#ifndef GinGameMatchManager_cpp
#define GinGameMatchManager_cpp

#include "cocos2d.h"
#include "InGameServerOfflineDefines.h"
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"
#include "HandCard.h"
#include "Rules/RuleLayoffCard.h"
#include "Bot/BotHelper.h"
#include "Define/InGameDefine.h"
#include "Define/ServerClientMessageDefine.h"
#include "SvrBotHelper.h"
#include "SvrHelper.h"

class ServerOffline;

enum class GameMatchState
{
    IDLE,
    PLAYER_JOINED,
    READY,
    DEAL_CARD,
    INGAME,
    POST_CHECK,
    FINISH,
};

class GinGameMatchManager : public cocos2d::Node
{
public:
    enum class PlayerTurnState
    {
        STATE_READY,
        STATE_PASS,
        STATE_GET_CARD, // PASS, DRAW CARD, TAKE CARD
        STATE_THROW_CARD, // KNOCK, THROW CARD
    };

private:
    std::vector<std::vector<int>> _cheatCardSlot;
    std::vector<SvrBotData>       _cheatBotList;
    int                           _matchCount;
    std::vector<SvrBotData>       _listBotInfo;
    std::vector<int>              _remainCards;
    std::vector<int>              _pileDiscards;

    GameMatchState  _state;
    GameMatchBundle _bundle;
    int _levelBot;

    int                    _lastWinIndex;
    int                    _firstPlayIndex;
    int                    _startRoundPlayOrderIndex;
    int                    _currentPlayOrderIndex;
    int                    _previousPlayOrderIndex;
    std::vector<int>       _playOrder;
    std::vector<int>       _winOrder;
    std::vector<HandCard*> _listHandCards;
    PlayerTurnState        _playerTurnState;
    PlayerTurnState        _playerTurnStateDone;

    std::map<int, std::vector<int>> _mapCheatCardPlayers;
    int                             _cheatFirstMoveSlotIdx;

    // [START: jackpot]
    int    _jackpotRound;
    double _jackpotMoney;
    bool   _jackpotLock;
    bool   _jackpotIsWin;
    // [END: jackpot]

    std::map<int, std::map<int, double>> _mapResultDetails;

    std::map<int, int>                      _mapTargetPoints;
    bool                                    _isFinishTargetPoints;
    int                                     _round;
    std::map<int, double>                   _mapBet;
    std::map<int, RuleConstant::ActionType> _knockMapAction;
    int                                     _knockPoints;
    std::vector<std::vector<int>>           _playerCustomMelds;
    int                                     _upCard;
    std::vector<int>                        _listPass;
    bool                                    _isSpadesDouble;
    void onResetData();
    void sendBotData(const std::vector<int>& changedIndex);
    void onPrepareMatch();
    void onDealCard();
    void sendMatchInfo();
    void registerPlayOrder(int firstIndex, int slotNum);
    void sendCardToPlayer(const std::vector<int>& cards);
    void sendCardToBot(const std::vector<int>& cards, int slotIndex);
    void logicDealCard();
    void logicInGame();
    void logicNextTurn();
    int tryGetNextPlayOrderIndex();
    void logicPlayerBeginTurn();
    void logicPlayerTurnStep(int playerIndex);
    void calculateMoneyEndMatch(const std::vector<int>& winOrderIndex, const std::vector<RuleLayoffCard>& suggestLayoffCards = {});
    void logicPlayerCustomMelds();
    void logicEndMatch();

    void onMatchLoop();
    bool isUser(int playerIndex);
    bool isBot(int playerIndex);
    bool shouldDealCardAgain(const std::map<int, std::vector<int>>& cardSlot);
    bool shouldDealCardAgain(const std::vector<std::vector<int>>& cardSlot, const std::vector<int>& playOrder);
    HandCard* getCurrentHand();

    // Knock
    bool canKnock();
    bool canKnockGin();
    bool canGinOrBigGin();
    int getKnockPlayerIndex();
    void updateKnockAction(int playerIndex, RuleConstant::ActionType actionType);
    bool isKnocker(int playerIndex);
    bool isDefender(int playerIndex);
    bool isPlayerGinOrBigGin(int playerIndex);
    bool canKnockByVariations() const;

    bool hasJackpot() const;
    bool updateJackpot(int winIndex);

    RDPlayer convertRDPlayer(HandCard* hand);

    void getPlayerCustomMelds(const rapidjson::Document& data);
    std::map<int, std::vector<std::vector<int>>> getMapCombinationCards(int targetPlayerIndex = INDEX_NONE);
    std::pair<RuleConstant::ActionType, int> getActionTypeAndSuggestKnockPointByVariation(HandCard* hand);
    RuleConstant::ActionType getActionTypeByVariation(HandCard* hand);

    double getTotalBet();
    int getTotalPlayer() const;

    std::vector<std::string> getCurrentAvatarChecked(int excludeSlotIndex = -1);
    std::pair<int, float> getCurrentMultiplyValueByScoreGap();

public:
    ~GinGameMatchManager();
    GinGameMatchManager();
    CREATE_FUNC(GinGameMatchManager);
    static GinGameMatchManager* createInstance();
    void destroyInstance();

    virtual bool init() override;
    void joinGame(const GameMatchBundle& bundle);
    void startNewMatch();
    void endGame();
    void onKick(int result = 1);

    void onMessage(MessageID messageId, const rapidjson::Document& data);
    void processStartMatchResponse(const rapidjson::Document& data);

    void addMoneyToSlot(int index, double money, const std::string& name);
    void addExpToSlot(int index, int exp);

    HandCard* getHandCard(int userSlot);
    HandCard* getPreviousHandCard();
    HandCard* getNextHandCard();

    void sendMessagePlayerBeginTurn(int index, bool isUser);
    void sendMessagePlayerTurnStepPass(int playerIndex, bool hintTake, bool canTake);
    void sendMessagePlayerTurnStepPassAction(int playerIndex, int cardIndex, bool result);
    void sendMessagePlayerTurnStepGetCard(int playerIndex, int previousPlayerIndex, int previousPlayerCard, bool hintTake, bool canTake);
    void sendMessagePlayerTurnStepGetCardAction(int playerIndex, double money, int losePlayerIndex, int card, int startRoundPlayerIndex, bool result);
    void sendMessagePlayerTurnStepThrowCard(int playerIndex, RuleConstant::ActionType action, int knockPoints);
    void sendMessagePlayerTurnStepThrowCardActionResult(int playerIndex, HandCard* hand, bool result, int cardIndex, RuleConstant::ActionType action);
    void sendMessagePlayerEndTurn();
    void sendMessagePlayerCustomMelds(int playerIndex, RuleConstant::ActionType action, int knockPoints);
    void sendMessageEndMatch(const std::map<int, double>& mapMoneyRewards, const std::vector<int>& winOrder, const std::vector<RuleLayoffCard>& suggestLayoffCards);
    void sendMessageCheatCard(int slotIdx);
    void sendMessageQuitMatch(int result);

    rapidjson::Value getAllPlayerDataJson(rapidjson::Document& doc);
    rapidjson::Value getPlayerDataJson(rapidjson::Document& doc, HandCard* hand, const std::vector<RuleLayoffCard>& suggestLayoffCards);
    rapidjson::Value getPlayerResultDetailJson(rapidjson::Document& doc, int index);
    rapidjson::Value getJackpotDataJson(rapidjson::Document::AllocatorType& allocator);

    void onMessagePlayerBeginTurn(int playerIndex);
    void onMessagePlayerTurnStepPassAction(int playerIndex, int cardIndex);
    void onMessagePlayerTurnStepPassDone(int playerIndex);
    void onMessagePlayerTurnStepGetCardAction(int playerIndex, int playerActionIndex);
    void onMessagePlayerTurnStepGetCardDone(int playerIndex);
    void onMessagePlayerTurnStepThrowCardAction(int playerIndex, int cardIndex, RuleConstant::ActionType actionType);
    void onMessagePlayerTurnStepThrowCardDone(int playerIndex);
    void onMessageCheatCard(const rapidjson::Document& data);

    void logicBotTurnStepPassAction(HandCard* botHand, int upCard);
    void logicBotTurnStepGetCardAction(HandCard* botHand, HandCard* previousHand, int previousCard);
    void logicBotTurnStepThrowCardAction(HandCard* botHand, RuleConstant::ActionType actionType);

    void botPassRunAction(int playerIndex, int cardIndex);
    void botGetCardRunAction(int playerIndex, int playerActionIndex);
    void botThrowCardRunAction(int playerIndex, int throwCard, RuleConstant::ActionType actionType);

    void logUser(bool isWin);
    void logData(const std::map<int, double>& mapMoneyRewards, const std::vector<int>& winOrder);
    void saveMatchData();
    void clearMatchData();
    SvrBotData& getBotInfo(int playerIndex);
    float getMaxMultiplyValue() const;
    float getMultiplyValueByScoreGap(int scoreGap) const;
    float getJackpotMultiplyValue(int cityId, int targetScore) const;

    static ServerOffline* _server;
};

#endif /* GinGameMatchManager_cpp */

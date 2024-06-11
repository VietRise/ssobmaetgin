#ifndef __GPGinScene__
#define __GPGinScene__

class GPGinScene;

#include "cocos2d.h"
#include "Scenes/InterfaceScene.h"
#include "Define/GameDefine.h"
#include "GPGinInterfaceLayer.h"
#include "GPGinSlotManager.h"
#include "GPGinDealCardLayer.h"
#include "GPGinControlCardLayer.h"
#include "GPGinEffect.h"
#include "Component/TutorialView.h"
#include "Define/ServerClientMessageDefine.h"

class GPGinScene : public InterfaceScene
{
private:
    GPGinSlotManager     * _slotManager;
    GPGinInterfaceLayer  * _interfaceLayer;
    GPGinControlCardLayer* _controlCardLayer;
    GPGinDealCardLayer   * _dealCardLayer;
    GPGinEffect          * _effectLayer;

    int                  _rule;
    CityType             _cityID;
    TableType            _tableID;
    double               _betMoney;
    float                _luckyPercent;
    bool                 _rejoin;
    int                  _stateRating;
    std::string          _conditionRatingName;
    std::string          _roomName;
    bool                 _adsExit;
    int                  _round;
    int                  _targetScore;
    std::vector<BetData> _listBetData;
    bool                 _canQuit;
    bool                 _inRound;
    bool                 _exited;
    int                  _countToPlaySoundDealCard;
    int                  _firstIndexDealCard;
    int                  _upcardValue;
    int                  _knockScoreValue;
    float                _multiplyValue;

    double               _jackpotMoney;
    int                  _jackpotRound;
    bool                 _isSpadesDouble;
    bool                 _jackpotLock;

    int                           _countListDealCard;
    std::vector<std::vector<int>> _listDealCards;
    bool                          _showAdsForce;

    std::map<int,bool>  _mapStatusPassShow;

    void onDealCardCallback();
    void posDealCardCallback();
    void registerHandleMessage(MessageID messageId);
    void onServerResponse(MessageID messageId, const rapidjson::Document& data);
    void onNotify(void* sender, int id, const cocos2d::Value& data);
    void prepareNewMatch();
    void prepareTableDeal();
    void setBetMoney(double money);
    void startGame();
    void handleBotJoin(const rapidjson::Document& data);
    void initRoom();
    void removeSlot(int slotIdx);
    void handleQuitInGame(const rapidjson::Document& data);
    void handleSaviorGoldBonusPopup(const rapidjson::Document& data);
    void handleGoldBonus(const rapidjson::Document& data);
    void handleSaviorGoldBonus(const rapidjson::Document& data);
    void handleRejoinGame(const rapidjson::Document& data);
    void handleUserData(const rapidjson::Document& data);
    void handleTicker(const rapidjson::Document& data);
    void handleUserProfile(const rapidjson::Document& data);
    void handleCheckRatingPopup(const rapidjson::Document& data);
    void resetDealCards();
    void handleDealCards();
    void handlePlayerHandCard(const rapidjson::Document& data);
    void handleBotHandCard(const rapidjson::Document& data);
    void handleBeginTurn(const rapidjson::Document& data);
    void handleEndTurn(const rapidjson::Document& data);
    void handleTurn(GinMatchStatus status, const rapidjson::Document& data);
    void handlePlayerTurnStepPassingDemandServer(const rapidjson::Document& data);
    void handlePlayerTurnStepPassing(const rapidjson::Document& data);
    void handlePlayerTurnStepGetCardDemandServer(const rapidjson::Document& data);
    void handlePlayerTurnStepGetCard(const rapidjson::Document& data);
    void handlePlayerThrowCardDemandServer(const rapidjson::Document& data);
    void handlePlayerThrowCard(const rapidjson::Document& data);
    void handleEndMatch(const rapidjson::Document& data);
    void handleCheatCardsResponse(const rapidjson::Document& data);
    void handleVideoRewardInfo(const rapidjson::Document& data);
    void handleVideoRewardEnable(const rapidjson::Document& data);
    void handleVideoAdsMoney(const rapidjson::Document& data);
    void handleHourlyBonusData(const rapidjson::Document& data);
    void handleManualMeldData(const rapidjson::Document& data);
    void onGetLocalNotificationData(const rapidjson::Document& data);
    void onMatchCompleted();
    void showCannotQuitNotify();
    void initGame();
    void reset();
    bool checkGetMoney();
    void updateThrowCardsPopup();
public:
CC_SYNTHESIZE_READONLY(float, _layoutRootScale, LayoutRootScale);
CC_SYNTHESIZE_READONLY(cocos2d::Size, _layoutRootCutomSize, LayoutRootCutomSize);
CC_SYNTHESIZE(int, _turnActionOrderIdx, TurnActionOrderIdx);

    GPGinScene(CityType cityType, TableType tableType, double betMoney, int targetScore, float maxMultiplyValue);
    ~GPGinScene();

    static cocos2d::Scene* createScene(CityType cityType, TableType tableType, double betMoney, int targetScore, float maxMultiplyValue);
    static GPGinScene* create(CityType cityType, TableType tableType, double betMoney, int targetScore, float maxMultiplyValue);

    void initScreen();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;

    virtual void onLoadResourcesSyncDone() override;
    virtual void onLoadResourcesAsyncDone() override;
    virtual SceneID getSceneID() override;
    void changeLanguage(Language language);
    GPGinInterfaceLayer* getInterfaceLayer();
    GPGinControlCardLayer* getControlCardLayer();
    GPGinEffect* getGinEffect();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    void onQuit();
    void onContinue(double bet, int targetScore);
    void onContinue2();
    void onChooseBet();
    void showTutorial(bool show);
    void waitToStart(float time);
    void hideLayerForQuit();
    void prepareQuit();
    bool checkShowInterstitialAdmob();
    void resetShowInterstitialAdmob();
    void handleGinCommandMatchInfoResponse(const rapidjson::Document& resp);

    CityType getCityType();
    const float& getLuckyPercent();
    const std::vector<cocos2d::Point>& getThrowPos();
    const std::vector<cocos2d::Point>& getDealPos();
    const std::vector<cocos2d::Point>& getShowCardPos();
    const std::vector<cocos2d::Point>& getShowStatusPos();
    const std::vector<cocos2d::Point>& getHandCardPos();
    cocos2d::Point getSlotPos(int order);
    cocos2d::Vec2 getRealPositionOfDrawCardAtBase();
    cocos2d::Size getLayoutCardSize(int orderIdx);
    int getUpCardValue();
    void setVisibleScoreBox(bool show, int value, int score, int slotIdx, const cocos2d::Vec2& position, GinResultType type);
    void updateScoreBox(int value, int score, int slotIdx, const cocos2d::Vec2& position, GinResultType type);
    int getScoreBoxValue(int slotIdx);
    void updateDeadwoodBox(int score, int knockScore, int slotIdx);
    bool inRound();
    void showRoundPointPopup(const std::vector<PlayerResultData>& listPlayerResultData = std::vector<PlayerResultData>());
    void showStatus(int orderIdx, FightStatus status, bool isChallenge = true);
    void showMatchEndStatus(GinMatchEndStatus status, cocos2d::Point positionOffset);
    void showGin(int orderIdx, cocos2d::Vec2 position);
    cocos2d::Sprite* getLastDrawCard();
    std::vector<std::vector<int>> getListMeldOnHandMainUser(bool isChallenged);
    std::vector<std::vector<int>> getListMeldOnHandMainUserBySoftGroup();

    const BetData& getBetData(int slotIdx);
    BetData& getBetDataRef(int slotIdx);

    double getTotalBet();
    static bool                   _forceContinue;

    int getRule();
    int getTargetScore();

    void hideAllPassEffect();

    void hideAllPassEffectExcept(int currentIdx);
};

#endif //__GPGinScene__

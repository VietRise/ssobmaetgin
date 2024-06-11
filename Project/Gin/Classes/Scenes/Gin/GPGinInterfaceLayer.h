#ifndef __GPGinInterfaceLayer__
#define __GPGinInterfaceLayer__

class GPGinInterfaceLayer;

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>
#include "GinSlotInfo.h"
#include "GPGinUserSlotNode.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"
#include "Component/JackpotIconNode.h"
#include "Component/GinUserHandNode.h"
#include "Component/GinScoreTable.h"
#include "Component/ResultScoreBoxNode.h"
#include "Component/EndGameResultNode.h"
#include "GinCard.h"

#define USER_INFO_LAYOUT_Z_ORDER 100
#define END_GAME_LAYOUT_Z_ORDER 8000

class GPGinScene;

class GPGinInterfaceLayer : public cocos2d::Layer
{
private:
    cocos2d::Size _screenSize;
    GPGinScene* _gPScene;

    cocos2d::ui::Layout    * _layoutInterfaceRoot;
    cocos2d::ui::Layout    * _layoutContent;
    cocos2d::ui::Button    * _buttonSetting;
    cocos2d::ui::Button    * _buttonTutorial;
    cocos2d::Node          * _nodeTable;
    cocos2d::ui::Text      * _textRoomTitle;
    cocos2d::ui::Text      * _textRoom;
    cocos2d::ui::Text      * _textBetTitle;
    cocos2d::ui::TextBMFont* _textBet;
    cocos2d::ui::Text      * _textBonusTitle;
    cocos2d::ui::TextBMFont* _textBonus;
    cocos2d::ui::Text      * _textRoundTitle;
    cocos2d::ui::TextBMFont* _textRound;
    cocos2d::ui::Text      * _textMaxMultiplyTitle;
    cocos2d::ui::TextBMFont* _textMaxMultiply;
    cocos2d::ui::Layout    * _layoutButtonQuit;
    cocos2d::ui::Layout    * _layoutRoomInfo;
    cocos2d::ui::Layout    * _layoutSlot;
    cocos2d::ui::Layout    * _layoutContinue;
    cocos2d::ui::Button    * _buttonQuitOpenState;
    cocos2d::ui::Button    * _buttonQuitCloseState;
    cocos2d::ui::Button    * _buttonChooseBet;
    cocos2d::ui::Button    * _buttonContinue;
    cocos2d::ui::Button    * _buttonShowResult;
    cocos2d::ui::Layout    * _layoutRewards;
    cocos2d::ui::Button    * _buttonVideoBonusReward;
    cocos2d::ui::Text      * _textBMVideoBonusReward;
    cocos2d::ui::ImageView * _imageVideoBonusReward;
    cocos2d::ui::ImageView * _imageVideoBonusRewardGlow0;
    cocos2d::ui::ImageView * _imageVideoBonusRewardGlow1;
    cocos2d::ui::Button    * _buttonFreeGold;
    cocos2d::ui::TextBMFont* _textBMFreeGold;
    cocos2d::ui::Text      * _textVideoBonusHint;
    cocos2d::ui::Button    * _buttonHourlyBonus;
    cocos2d::ui::Text      * _textHourlyBonusTitle;
    cocos2d::ui::TextBMFont* _textHourlyBonusMoney;
    cocos2d::ui::TextBMFont* _textHourlyBonusCountDown;
    cocos2d::ui::TextBMFont* _textHourlyBonusGetNow;
    cocos2d::ui::ImageView * _imgHourlyBonusGet;
    cocos2d::ui::Layout    * _layoutEventDecor;

    cocos2d::ui::Layout   * _layoutCardsOnTable;
    cocos2d::ui::Layout   * _layoutDrawAndTakeCards;
    cocos2d::ui::Layout   * _layoutNumCardInDraw;
    cocos2d::ui::ImageView* _imageViewDrawCard;
    cocos2d::ui::ImageView* _imageViewTakeCard;
    cocos2d::ui::Button   * _buttonShowThrowCard;

    cocos2d::ui::Layout     *_jackpotLayout;
    cocos2d::ui::TextBMFont *_textJackpotValue;
    cocos2d::ui::TextBMFont *_textJackpotTitle;
    cocos2d::ui::Button     *_buttonJackpotHelp;

    GinScoreTable* _ginScoreTable;
    ResultScoreBoxNode* _resultScoreBox;
    EndGameResultNode*  _endGameResultNode;

    cocos2d::ui::Layout     *_extraRoomInforLayout;
    cocos2d::ui::TextBMFont *_textBMExtraRoomInfor;
    cocos2d::ui::Layout     *_firstDumpCardLayout;
    GinCard                 *_firstDumpCard;

    double         _hourlyBonusCountDown;
    double         _hourlyBonusMoney;
    cocos2d::Point _posButtonHourlyBonus;

    float _imageVideoBonusRewardScaleX;
    float _imageVideoBonusRewardScaleY;

    std::map<HintDropPlaceType, cocos2d::Rect>  _listHintDropArea;
    std::map<std::string, cocos2d::ui::Layout*> _mapTemplate;
    std::map<int, std::string>                  _listTableMappingFilename;
    cocos2d::Vector<GPGinUserSlotNode*>         _listSlotNode;
    cocos2d::Vec2                               _layoutContinuePos;

    std::vector<cocos2d::Point> _listDealCardPos;
    std::vector<cocos2d::Point> _listShowCardPos;
    std::vector<cocos2d::Point> _listThrowCardPos;
    std::vector<cocos2d::Point> _listShowStatusPos;
    std::vector<cocos2d::Point> _listHandCardPos;

    std::vector<cocos2d::Point> _listShowCardGlobalPos;
    std::vector<cocos2d::Point> _listHandCardGlobalPos;

    int _numDrawCard;
    cocos2d::Node* _nodeFakeDrawCard;
    std::vector<cocos2d::Sprite*> _listFakeCardDrawCard;

    bool   _quitState;

    cocos2d::Vec2 _posRealDrawCardAtBase;
    cocos2d::Vec2 _posOriginalTextNumCardInDraw;
    cocos2d::Vec2 _posScoreNodeMainUser;

    void runUserJoinRoomAnimation(int slotIdx);
    void onButtonClick(cocos2d::Ref* sender);
    void onSettingsButtonClick(cocos2d::Ref* sender);
    void onTutorialButtonClick(cocos2d::Ref* sender);
    void onQuitButtonClick(cocos2d::Ref* sender);
    void onContinueButtonClick(cocos2d::Ref* sender);
    void onChooseBetButtonClick(cocos2d::Ref* sender);
    void onQuit(bool quit);
    void initFakeCards();
protected:
public:
    GPGinInterfaceLayer();
    ~GPGinInterfaceLayer();

    virtual bool init() override;
    void initLayout(cocos2d::Size screenSize, float scale);
    void initBackground(CityType cityType);

    void setNumDrawCard(int numberCard, bool drawCard, bool showFakeCard = true);
    int getNumDrawCard();
    cocos2d::Sprite* getLastDrawCard();

    cocos2d::ui::Layout* getLayoutEffectHint();
    cocos2d::ui::Layout* getLayoutStartRound();
    cocos2d::ui::Button* getButtonShowThrowCards();

    void changeLanguage(Language language);

    cocos2d::Point getDealHandPosition(int orderIdx);
    cocos2d::Vec2 getRealCardOnDrawCardPosition();
    cocos2d::Vec2 getDrawCardPos();
    cocos2d::Vec2 getTakeCardPos();
    cocos2d::Vec2 getScoreBoxMainUserPos();
    cocos2d::Vec2 getPositionTextBonus();
    cocos2d::Vec2 getPositionTextMoney(int orderIdx);

    std::vector<cocos2d::Point> getListDealCardPosition();
    std::vector<cocos2d::Point> getListDealCardPosition(const std::vector<int>& orderIdx);
    const std::map<HintDropPlaceType, cocos2d::Rect>& getHintDropPlaceArea();

    GPGinUserSlotNode* getMainUserSlot();
    GPGinUserSlotNode* getUserSlot(int index);

    void addSlot(GinSlotInfo* slotInfo);
    void setMainSlot(GinSlotInfo* slotInfo, cocos2d::Node* parent);
    int getEmptySlotIdx();
    cocos2d::Point getSlotPos(int order);

    GPGinUserSlotNode* getEmptySlotNode();
    GPGinUserSlotNode* getSlotNode(int slotIdx);

    void reset();
    void setRoomInfo(CityType cityType, TableType tableType, double betMoney, int round, float multiplyValue);
    void setRoomName(const std::string& name);
    void setBetMoney(long long money);
    void setRoundNumber(int round);
    void setMaxMultiplyNumber(float value);
    void updateMoney(int orderIdx);
    void updateMoney();
    void updateLevel();
    void updateLevel(int orderIdx);
    void initTable(TableType type);
    cocos2d::ui::Layout* getTableLayout(TableType type);
    cocos2d::ui::Layout* loadTableLayout(const std::string& fileName);
    void setQuitState(bool quit);
    bool getQuitState();
    void addUserHandNode(cocos2d::Vector<GinUserHandNode*> listUserHandNode);
    void setGPScene(GPGinScene* gPScene);
    void removeSlot(int orderIdx);

    std::vector<cocos2d::Point> getListSlotDealCardPosition(std::vector<int> orderIdx);
    const std::vector<cocos2d::Point>& getThrowPos();
    const std::vector<cocos2d::Point>& getDealPos();
    const std::vector<cocos2d::Point>& getShowCardPos();
    const std::vector<cocos2d::Point>& getShowCardGlobalPos();
    const std::vector<cocos2d::Point>& getShowStatusPos();
    const std::vector<cocos2d::Point>& getHandCardPos();
    const std::vector<cocos2d::Point>& getHandCardGlobalPos();

    void showNotifyBack(bool show);
    void setVisibleDrawCard(bool isVisible);
    void setVisibleLayoutDrawCard(bool isVisible);
    void setVisibleLayoutTakeCard(bool isVisible);

    void setAllUserTurnOff();
    void setUserTurnOff(int orderIdx);
    void setUserOnTurn(int orderIdx);
    void updateStatusUser(GinMatchStatus status, int idInTurn, bool isResetName = false);
    void onAvatarClick(GPGinUserSlotNode* userSlotNode);

    void finishDealCard(int knockScore);
    void showStatus(int orderIdx, FightStatus fightStatus);
    void hideStatus(int orderIdx);
    void showMatchEndStatus(GinMatchEndStatus status, cocos2d::Point positionOffset);
    void setVisibleScoreBox(bool show, int value, int score, int slotIdx, const cocos2d::Vec2& position, GinResultType type);
    void setVisibleScoreBox(bool show, int slotIdx);
    void updateScoreBox(int value, int score, int slotIdx, const cocos2d::Vec2& position, GinResultType type);
    int getScoreBoxValue(int slotIdx);
    void setVisibleDeadwoodBox(bool show, int score, int knockScore, int slotIdx, cocos2d::Vec2 position);
    void updateDeadwoodBox(int score, int knockScore, int slotIdx);
    int getDeadwoodScore(int slotIdx);
    cocos2d::Size getLayoutCardSize(int orderIdx);
    void showEffectScoreWinLose(bool win, long long money, bool finalRes, int orderIdx, bool fast = false);
    void showEffectRank(int orderIdx, GinResultType type, int rank);
    void showLayoutContinue(bool show);
    void showButtonResult(bool show);
    void showLayoutRewards(bool show);
    void updateLayoutRewards(const rapidjson::Document& data);
    void onVideoAdsMoney(const rapidjson::Document& data);
    void onHourlyBonusData(const rapidjson::Document& data);
    void setHourlyBonusButtonState(int state, double countdown, double money);
    cocos2d::Action* createMoveHourlyBonusButton(int state);
    void checkAndEnableEventDecorLayer();

    void showNotify(bool show, int orderIdx, std::string text);
    void showTextDropArea(bool show, int orderIdx);
    void onQuitKeyBackClick();
    void resetLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData);
    void updateLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData);
    cocos2d::Vec2 convertWorldPosToLayoutRootPos(const cocos2d::Vec2& posWorld);
    void hideLayerForQuit();
    CREATE_FUNC(GPGinInterfaceLayer);

    static float _scaleDrawCard;
    static float _scaleTakeCard;

    void runShowScoreTableAnimation();
    void runHideScoreTableAnimation();
    cocos2d::Point getLastFakeDrawCardWorldPosition();

    void setLayoutDrawAndTakeVisible(bool visible);
    void setScoreBoxesVisible(bool visible);
    void setScoreBoxColor(int value, int slotIdx);

    ResultScoreBoxNode* getResultScoreBox();

    void setJackpotValue(long value);

    EndGameResultNode* getEndGameResultNode();

    GinScoreTable* getScoreTable();

    void setVisibleUIOnEndGame();

    void playShowExtraRoomInformation(int cardValue, bool isSpadesDouble);
    void playHideExtraRoomInformation();

    void runJackpotBackgroundAnimation(bool animation);
    void speedUpJackpotBackgroundAnimation(bool speed);

    void playShowJackpot();
    void playHideJackpot();

    void setJackpotVisible(bool visible);
    cocos2d::Size getJackpotSizeAfterScale();
    cocos2d::Point getJackpotGlobalPosition();

    void playMoveAndHideScoreBoxAnimation(int slotIdx, cocos2d::Point globalPosition, float delay);

    void playTableFadeAnimation(float duration, float opacity);
    void playShowTableAnimation();
    void playHideTableAnimation();
};

#endif //__GPGinInterfaceLayer__

#ifndef __GPGinControlCardLayer__
#define __GPGinControlCardLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>
#include <vector>
#include <cmath>
#include "Manager/TextManager.h"
#include "GinCard.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"
#include "Component/GinUserHandNode.h"

#define STYLE_CARDS_ON_HAND_STRAIGHT 0
#define STYLE_CARDS_ON_HAND_CURVE 1
//#define STYLE_CARDS_ON_HAND STYLE_CARDS_ON_HAND_STRAIGHT
#define STYLE_CARDS_ON_HAND STYLE_CARDS_ON_HAND_CURVE

#define TIME_CARD_THROW_END_MATCH_USER  0.2f
#define TIME_CARD_THROW_END_MATCH_BOT  0.2f
#define TIME_CARD_THROW           0.35f
#define TIME_CARD_DRAW            0.4f
#define TIME_CARD_TAKE            0.4f
#define TIME_CARD_GIVE            0.8f
#define TIME_CARD_SORT            0.2f
#define TIME_FINAL_CARD_DELAY     0.5f
#if USE_AUTO_PLAY_CARD == 1
#define TIME_CARD_THROW_END_MATCH_USER  0.2f
#define TIME_CARD_THROW_END_MATCH_BOT  0.2f
#define TIME_CARD_THROW           0.35f
#define TIME_CARD_DRAW            0.4f
#define TIME_CARD_TAKE            0.4f
#define TIME_CARD_GIVE            0.8f
#define TIME_CARD_SORT            0.2f
#endif

#define CARD_ON_TABLE_SCALE_1        0.6f
//#define CARD_IN_HAND_SCALE                            1.0f
#define CARD_IN_HAND_SCALE                              1.3f
#define CARD_IN_HAND_MAX_SCALE                          1.56f
#define CARD_IN_HAND_OTHER_PLAYER_SCALE                0.44f
#define DRAG_CARD_POSITION_Y_OFFSET                    25.0f

#define DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1_DEFINE            25
#define DISTANCE_BETWEEN_TWO_CARD_OTHER_PLAYER                22
#define WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_ON_HAND_MAIN_USER 100
#define WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1 72
#define CURVE_CARD_LAYOUT_POSITION_OFFSET cocos2d::Point(0, 10)

enum class TypeArrangeCardOnHand
{
    TYPE_ARRANGE_IN_START_END,
    TYPE_ARRANGE_IN_START_END_MULTI,
    TYPE_ARRANGE_INCREASE,
    TYPE_ARRANGE_SUIT,
    TYPE_ARRANGE_COMBINATION,
    TYPE_ARRANGE_MELD_CARD,
    TYPE_ARRANGE_GROUP_CARD,
    TYPE_ARRANGE_GROUP_CARD_MULTI
};

enum class TypeArrangeCardOnHandOtherPlayer
{
    TYPE_ARRANGE_ON_HAND,
    TYPE_ARRANGE_ON_TABLE
};

enum class ZorderControlLayer
{
    EFFECT_HINT_BELOW_CARD = (1000),
    LAYOUT_ROOT            = (2000),
    CARD_THROW             = (3000),
    CARD_IN_HAND           = (4000),
    EFFECT_HINT            = (6000),
    CARD_ON_HAND_USER      = (7000),
};

enum class ControlStatus
{
    CONTROL_STATUS_NORMAL,
    CONTROL_STATUS_PASSING,
    CONTROL_STATUS_GET,
    CONTROL_STATUS_DUMP,
    CONTROL_STATUS_SHOW_CARD,
    CONTROL_STATUS_FIGHT_CHALLENGE_FOLD,
    CONTROL_STATUS_MATCH_END
};

enum class MeldType
{
    MELD_NONE,
    MELD_SANH,
    MELD_BALA
};

enum class FakeCardType
{
    NONE,
    DRAW,
    TAKE
};

struct GroupCard
{
    std::vector<GinCard*> _listCards;
    cocos2d::ui::Layout* _layoutMeld;
    MeldType _type;
public:
    GroupCard()
    {
        _layoutMeld = nullptr;
        _type       = MeldType::MELD_NONE;
    }
};

struct SoftGroupCard
{
    std::vector<GinCard*> _listCard;
    MeldType              _type;
    int                   _startIndexInHand;
    int                   _endIndexInHand;
public:
    SoftGroupCard()
    {
        _startIndexInHand = -1;
        _endIndexInHand   = -1;
        _type             = MeldType::MELD_NONE;
    }
};

class GPGinScene;

typedef std::function<void()> MatchEndEachUserFinishCallback;
typedef std::function<void()> MatchEndFinishCallback;

class GPGinControlCardLayer : public cocos2d::Layer
{
private:
    cocos2d::Size _screenSize;
    cocos2d::ui::Layout* _layoutRoot;
    cocos2d::ui::Layout* _layoutGroupCardTittle;
    cocos2d::ui::Layout* _layoutNumberOfRemainCardTemp;
    cocos2d::ui::Layout* _layoutHintTemp;
    cocos2d::ui::Layout* _layoutCards;
    cocos2d::Node      * _nodeAnchorCards;
    cocos2d::ui::Button* _buttonDraw;
    cocos2d::ui::Button* _buttonPass;
    cocos2d::ui::Button* _buttonKnock;
    cocos2d::ui::Button* _buttonGin;
    cocos2d::ui::Button* _buttonBigGin;
    cocos2d::ui::Button* _buttonDump;
    cocos2d::ui::Button* _buttonSort;
    cocos2d::ui::Button* _buttonTake;
    cocos2d::ui::Button* _buttonGroup;
    cocos2d::ui::Button* _buttonUngroup;
    cocos2d::ui::Button* _buttonShowThrowCards;
    cocos2d::ui::Button* _buttonFindMeld;
    cocos2d::ui::Layout* _layoutStepHint;
    cocos2d::ui::Layout* _hintTakeCard;
    cocos2d::ui::Layout* _hintDrawCard;
    cocos2d::ui::Layout* _layoutNotify;

    cocos2d::Point       _dumpPlacePosInLayoutCard;
    cocos2d::Point       _drawPlacePosInLayoutCard;

    cocos2d::ui::Layout* _layoutDark;

    cocos2d::ui::Layout* _layoutParticle;
    cocos2d::ParticleSystemQuad* _particleKnock;
    cocos2d::ParticleSystemQuad* _particleGin;
    cocos2d::ParticleSystemQuad* _particleBigGin;

    cocos2d::Vector<GinUserHandNode*>                                                 _listUserHandNode;
    std::vector<std::vector<GinCard*>>                                                _listHandCardsAllUser;
    std::vector<GinCard*>                                                             _listHandCardsMainUser;
    std::vector<GroupCard>                                                            _listGroupHandCardsMainUser;
    std::vector<SoftGroupCard>                                                        _listSoftGroupHandCardsMainUser;
    std::vector<cocos2d::Point>                                                       _listShowCardPos;
    std::vector<cocos2d::Point>                                                       _listDealCardPos;
    std::vector<cocos2d::Point>                                                       _listThrowCardPos;
    std::vector<cocos2d::Point>                                                       _listShowStatusPos;
    std::vector<GinCard*>                                                             _listThrowCards;
    std::vector<std::vector<GinCard*>>                                                _listTakeCard;
    std::vector<std::vector<std::vector<GinCard*>>>                                   _listMeldsAllUser;
    std::vector<std::vector<GinCard*>>                                                _listRemainCards;
    std::vector<std::vector<GinCard*>>                                                _listShowCards;
    std::vector<std::vector<int>>                                                     _listSuggestTakeCardsCached;
    std::vector<GinCard*>                                                             _listCardsWereAutoSelectedForHintTake;
    std::vector<std::pair<int, std::vector<GinCard*>>>                                _listCardsWereAutoUngroupForHintTake;
    std::vector<std::pair<std::pair<cocos2d::Node*, cocos2d::Node*>, cocos2d::Point>> _listGroupTitleNode;
    std::map<std::string, std::vector<cocos2d::Node*>>                                _mapEffectHighlight;
    std::map<HintDropPlaceType, std::pair<cocos2d::Rect, cocos2d::Value>>             _listHintDropArea;

    cocos2d::ui::Layout      *_layoutTouchDraw;
    cocos2d::ui::Layout      *_layoutTouchTake;

    GinCard       * _ginCardDrapCached;
    cocos2d::Touch* _ptouchDrapCached;
    cocos2d::Event* _pEventDrapCached;
    GinCardDragType _currentDragCardsType;

    bool            _ginDrawAndTakeAnimEnable = false;
    GinCard       * _ginDrawAndTakeFakeCard = nullptr;
    cocos2d::Touch* _ginDrawAndTakeFirstTouch = nullptr;
    cocos2d::Touch* _ginDrawAndTakeCurrentTouch = nullptr;
    cocos2d::Event* _ginDrawAndTakeEvent = nullptr;
    bool            _ginDrawAndTakeAnimationRunning = false;
    float           _ginDrawAndTakeTouchHoldCountDown = 0;
    FakeCardType    _ginDrawAndTakeType = FakeCardType::NONE;
    cocos2d::Point  _ginDrawAndTakeOriginGlobalPos;
    GinCardProperty _ginDrawAndTakeCardProperty;
    int            _ginDrawAndTakeFakeCardHandIndex = -1;

    ControlStatus _status;

    bool _cardsIsOnHand;
    bool _shouldSortCardAfterPickingCard;
    bool _canTouchCard;

    std::vector<cocos2d::Size>      _listLayoutCardSize;
    std::vector<std::vector<float>> _listDistanceBetweenTwoCardOnTable1Short;
    std::vector<MeldColorType>      _listMeldHighLightColor;

    cocos2d::Point _anchorCardInLayoutCardPosition;

    cocos2d::ui::Layout* _layoutTurn;
    cocos2d::ui::ImageView* _imageTurnBg;
    cocos2d::ui::Text* _textTurn;
    cocos2d::ui::ImageView* _imageTurnBot;
    cocos2d::ui::ImageView* _imageTurnUser;

    cocos2d::BezierTo* getBezierAnimationVertical(cocos2d::Point srcPos, cocos2d::Point dstPos, bool isLeft, float duration);
    cocos2d::BezierTo* getBezierAnimationHorizontal(cocos2d::Point srcPos, cocos2d::Point dstPos, bool isUp, float duration);
protected:
public:
    static float                                                      _distanceBetweenTwoCards;
    static float                                                      _distanceGapBetweenTwoMelds;
    static float                                                      _angleBetweenTwoCards;
    static float                                                      _r;
    static float                                                      DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1;
    static int                                                        _currentSortMethodIndex;
    static std::vector<std::pair<TypeArrangeCardOnHand, std::string>> _listSortMethod;

CC_SYNTHESIZE(GPGinScene*, _gameGin, GameGin);
CC_SYNTHESIZE(int, _currentScore, CurrentScore);
CC_SYNTHESIZE(int, _currentKnockScore, CurrentKnockScore);
CC_SYNTHESIZE(int, _knockDeadwood, KnockDeadwood);

    GPGinControlCardLayer();
    ~GPGinControlCardLayer();

    virtual bool init() override;
    cocos2d::ui::Layout* getLayoutRoot();
    cocos2d::ui::Layout* getLayoutCard();
    cocos2d::Node* getNodeAnchorCard();

    std::vector<GinCard*> getListHandCardsMainUser();
    std::vector<GinCard*> getListThrowCards(int orderIdxFristDeal = 0);
    std::vector<int> getListThrowCardsValue(int orderIdxFristDeal = 0);
    cocos2d::Vector<GinUserHandNode*> getListUserHandNode();
    const std::vector<GroupCard>& getListGroupCardsMainUser();
    std::vector<std::vector<int>> getListMeldOnHandMainUser();
    std::vector<std::vector<int>> getListMeldOnHandMainUserBySoftGroup();

    void initLayout(const cocos2d::Size& screenSize, float scale);
    void alignChildrenPostion(const cocos2d::Vec2& drawCardPos, const cocos2d::Vec2& takeCardPos, cocos2d::ui::Button* buttonThrowCards);
    void reset();
    void clearAllCard();
    void finishDealCard(int orderIdxFirstPlaying, int upcard, const cocos2d::Point& upCardStartPos);
    void setUserOnTurn(int orderIdx, bool onTurn);
    void setVisibleButtonXep(bool isVisible);
    void setVisibleButtonShowThrowCards(bool isVisible);
    void hintPassCard(bool value, bool hintTake, bool canTake);
    void hintGetCard(bool value, int fromOrderId, int previousCards, bool hintTake, bool canTake);
    void hintDumpCard(bool value, ActionType type);
    void highlightPhom(const std::vector<std::vector<int>>& listPhomHighLight, bool isDrawCard);

    cocos2d::RepeatForever* actionHint(const cocos2d::Vec2& move);
    void initListHandCardsAllUser(const std::vector<std::vector<int>>& listDealCards);
    void initListHandCardsMainUser(const std::vector<int>& listIndex);
    void initListHandCardsAnthorUser(const std::vector<int>& listIndex, int orderIdx);
    void showUpCard(int card, const cocos2d::Point& startPos);
    void throwCard(int orderId, int card, int actionType);
    void drawCard(int orderId, int index);
    void takeCard(int orderId, int index);
    void giveCard(int orderIdx, const PointGiveServerDefine& pointGiveCard, int step);
    void matchEndEachUser(PlayerResultData res, bool win, GinResultType typeWin, MatchEndEachUserFinishCallback finishCallback);
    void preMatchEnd();
    void matchEnd(const PlayerResultData& win, const std::vector<PlayerResultData>& listLose, MatchEndFinishCallback callback);
    void showCardsTake(int orderId, int takeCardValue);
    void removeHandCardsByIndex(int index);
    GinCard* getHandCardsByIndex(int orderId, int index);
    GinCard* getHandCardsOnUser(int orderId);

    void addFakeCardToUserHand(GinCard* card);
    void removeFakeCardFromUserHand(GinCard* card, bool sortCardOnHand = true);

    void onButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void onRequestDump(bool byClickBtn, ActionType actionType, GinCard* card = nullptr, cocos2d::Touch* ptouch = nullptr, cocos2d::Event* pEvent = nullptr);
    void onRequestSortCard(TypeArrangeCardOnHand type);

    virtual bool onTouchBegan(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchMoved(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchEnded(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchCancelled(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;

    bool onTouchBeganCard(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchMovedCard(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchEndedCard(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchCancelledCard(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);

    void initDrawAndTakeAnimationCachedData();
    void resetDrawAndTakeAnimationCachedData();
    void createDrawAndTakeFakeCard();
    void returnFakeCardToOriginPositionAndRemove(bool playAnim = true, bool sortCardOnHand = true);
    void resetDrawAndTakeAnimationCachedDataOnDrawCard();

    void setGinDrawAndTakeCardProperty(GinCard* card);

    void enableDrawAndTakeButton(bool enable);

    bool onTouchBeganLayoutTouchTake(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchMovedLayoutTouchTake(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchEndedLayoutTouchTake(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchCancelledLayoutTouchTake(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);

    bool onTouchBeganLayoutTouchDraw(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchMovedLayoutTouchDraw(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchEndedLayoutTouchDraw(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    void onTouchCancelledLayoutTouchDraw(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);

    void getAngleBetweenTwoCards(int numberCard);
    std::vector<GinCardProperty> listPositionCardsOnHandAccordingNumberCards_Straight(int numberCard, bool isConvertWorldSpace = false);
    std::vector<GinCardProperty> listPositionCardsOnHandAccordingNumberCards_Curve(int numberCard, bool isConvertWorldSpace = false);
    std::vector<GinCardProperty> listPositionCardsOnHandAccordingNumberCards(int numberCard);
    std::vector<std::vector<GinCardProperty>> listPositionCardsOnHandAccordingNumberCardsOfAllPlayers(const std::vector<std::vector<int>>& listDealCard);
    std::vector<GinCardProperty>
    listPositionCardsOnHandAccordingNumberCardsOtherPlayer_Straight(int orderIdx, int numberCard, bool directionRight, const cocos2d::Vec2& position, TypeArrangeCardOnHandOtherPlayer type);
    std::vector<GinCardProperty> listPositionCardsOnHandAccordingNumberCardsOtherPlayer(int numberCard, int orderIdx, TypeArrangeCardOnHandOtherPlayer type);
    void playEffectSortCardsOnHand();
    void playEffectSortCardsOnHandSpeedControl(float timeScale);
    void showCardsOnHand(const std::vector<GinCard*>& listHandCard, int orderIdx);
    void arrangeCardOnHand(TypeArrangeCardOnHand type, int posStart = -1, int posEnd = -1, int numberCardGap = 0);
    void arrangeCardOnHandBySortingGroupAndTrashCard();
    void arrangeCardOnHandOnTrashCard(std::vector<GinCard*>& listTrashCard);
    void arrangeCardOnHandPhom();
    void softArrangeCardOnHandPhom();
    void arrangeCardOnHandBySortingGroupAtTheBeginning();
    void arrangeCardOnHandAfterPickCard(GinCard* card);
    void softArrangeCardOnHandAfterPickCard(GinCard* card);
    int getIndexCardOnHand(GinBaseCard* card);
    bool enableTouchCard();
    void changeLanguage(Language language);
    void showNotifyWrong(const std::string& content);
    GinCard* createCard(int index, float scale, bool open, bool isSmall = false);
    bool checkCardInListTakedCard(int card);
    static bool Doi(int clientId1, int clientId2);
    static bool Ca(int clientId1, int clientId2);
    static bool LienTuc(int clientId1, int clientId2);
    void sortIncrease(std::vector<GinCard*>& list);
    void sortDecrease(std::vector<GinCard*>& list);
    void sortIncrease(std::vector<int>& list);
    void sortDecrease(std::vector<int>& list);
    void sortSuit(std::vector<GinCard*>& list);
    MeldType getPhomType(const std::vector<int>& phom);
    MeldType getPhomType(const std::vector<GinCard*>& phom);
    int CalScore(const CardInHand& cih);
    bool PhomIsValid(const CardInHand& cih);
    bool MeldIsValid(const std::vector<GinCard*>& listCard, bool isStrictMode = false);
    bool MeldIsValid(std::vector<int> listCard, bool isStrictMode = false);
    int Max(const std::vector<int>& list);
    bool canGive(const std::vector<int>& phom, int card);
    bool Give(std::vector<int>& phom, int card);
    cocos2d::Point checkIn(const std::vector<std::vector<int>>& listPhom, int card);
    int calCol(int a[13][4], int nr, int nc, int col);
    int calRow(int a[13][4], int nr, int nc, int row);
    std::vector<std::vector<int>> getCa(std::vector<int> list);
    CardInHand getPhom(const std::vector<GinCard*>& list);
    CardInHand getPhom(const std::vector<int>& list_int);
    ListOverlap getPhomOverlap(const std::vector<int>& list);
    std::vector<int> getAllTrash(const std::vector<int>& allCard, const std::vector<std::vector<int>>& list_phom);
    std::vector<GinCard*> getTrashCards();
    std::vector<std::vector<int>> getPhomContainCard(const std::vector<GinCard*>& listCard, int card);
    std::vector<GinCard*> getListSelectedCardOnHand();
    std::vector<int> getListSelectedCardValueOnHand();
    std::vector<GinCard*> getListSelectedCardOnHandInGroup();
    std::vector<GinCard*> getListTrashCardsOnHand();
    bool checkHaveSelectedCardOnListHandCards(const std::vector<GinCard*>& listHandCards);
    bool checkCardIsSlectedOnListHandCards(int valueCard);
    void checkStateOfGroupAndUngroupButton();
    bool checkStateDumpCardButton();
    void checkAutoSelectCardIngroup(int cardValue);
    void checkAutoUnselectCardInGroup(int cardValue);
    int checkCardInAGroup(int cardValue);
    std::vector<GinCard*> checkCardInAGroup(GinCard* card);
    int checkCardInASoftGroup(int cardValue);
    std::vector<GinCard*> checkCardInASoftGroup(GinCard* card);
    int checkListCardInAGroup(const std::vector<GinCard*>& listCards);
    int checkListCardInAGroup(const std::vector<int>& listCards);
    bool checkEachCardDontInAnyGroup(const std::vector<GinCard*>& listCards);
    bool checkEachCardDontInAnyGroup(const std::vector<int>& listCards);
    bool checkAllCadsOnHandInGroup();
    void setEnableButton(cocos2d::ui::Button* button, bool enable);
    cocos2d::Vec2 getButtonPos(cocos2d::ui::Button* button);
    GroupCard addGroup(std::vector<GinCard*> listCard, bool animShowGroup, bool sortGroup = false, int index = -1);
    void animationShowGroup(const GroupCard& groupCard);
    void updateGroup(GroupCard& groupCard);
    void removeGroup(int groupIndex);
    void clearGroup(bool needRemoveLayout);
    void clearSoftGroup();
    void checkUpdateGroup(GinCard* startCard, GinCard* endCard, int numberGap);
    void removeCardOutOfGroup(int groupIndex, int cardValue);
    std::vector<int> convertListCardInt(const std::vector<GinCard*>& listCards);
    int getScore(int cardValue);
    void calculateScoreCardOnHand();
    void calculateScoreCardOnHandBySoftGroup();
    void checkStatusOfShowThrowCardButton();
    void updateNumberOfRemainCards(int orderIdx, bool anim = false);
    void resetDataByDrapDropCards(bool success);
    bool isVisibleEffectHintAtDropPlace(HintDropPlaceType type);
    void setVisibleEffectHintAtDropPlace(bool visible, HintDropPlaceType type);
    void hideAllEffectHintAtDropPlace();
    cocos2d::Node* showEffectHighlight(const std::string& name, cocos2d::Node* parent, const cocos2d::Size& size, const cocos2d::Vec2& position, int zorder);
    void removeEffectHighlight(const std::string& name);
    void removeAllEffectHighlight();
    void showGroupButton(bool show, const std::vector<GinCard*>& listCard);
    void showUngroupButton(bool show, const std::vector<GinCard*>& listCard);
    void checkUserMovingCardsInSpecificArea(cocos2d::Touch* ptouch, GinCard* card);
    HintDropPlaceType checkUserDropCardsInSpecificArea();
    void clearAllEffectHintDropArea();
    void startDrap(GinCard* card);
    float unselectAllCardsOnHand(bool anim = true);
    void showStepHint(bool show);
    void checkStepHint();
    bool shouldGroupCardsByHintTakeCard(const std::vector<GinCard*>& listCard);
    cocos2d::Vec2 getPosShowCard(int orderId, float width);
    cocos2d::Vec2 convertWorldPosToLayoutRootPos(const cocos2d::Vec2& pos);
    void forceUndrapAllCards();
    void checkUndoActionHintTakeCard(bool drawCard = false);
    bool checkValidCardsOnHand(GinCard* card);
    bool checkValidCards(const std::vector<GinCard*>& listCard, GinCard* card);
    void showHintDraw(bool show);
    void showHintTake(bool show);
    void showHintKnock(bool show);
    void showHintDump(bool show);
    void showHintPass(bool show);
    void showHintGin(bool show);
    void showHintBigGin(bool show);
    bool checkValidMeldGroupLayout(cocos2d::Node* node);
    bool removeCardFromRemainCards(int orderIdx, GinCard* card);
    bool removeCardFromShowingCards(int orderIdx, GinCard* card);
    int insertCardToShowingCards(int orderIdx, GinCard* ginCard);
    std::vector<GinCardProperty> calculatePositionShowingCards(int orderIdx);
    cocos2d::Vec2 calculateGlobalPositionScoreBox(int orderIdx);
    void arrageShowingCards(int orderIdx, const std::vector<int>& excludeCards);
    int calculateScoreCards(const std::vector<GinCard*>& listCard);
    int calculateScoreCards(const std::vector<int>& listCard);
    virtual void update(float dt) override;
    CREATE_FUNC(GPGinControlCardLayer);

    void resetListSoftGroupHandCardMainUser();
    bool updateListSoftGroupHandCardMainUser();
    bool updateListSoftGroupHandCardMainUserOnThrowCard(GinCard* card);
    int isCardInSoftGroup(GinCard* card);
    void handleManualMeldResponse(int orderIdx, int actionType, int knockDeadwood);
    void updateThrowActionUI(ActionType actionType);
    void setThrowCardsVisible(bool visible);
    void setShowCardsVisible(bool visible);
    void hideAllEffectHint();

    float calculateMatchEndShowCardDuration(const PlayerResultData& win, const std::vector<PlayerResultData>& listLose);
    float calculateMatchEndEachUserShowCardDuration(const PlayerResultData& res);

    int getPossibleNewCardIndexBySoftGroup(int cardValue);

    void sortSoftGroups();
    void sortSoftGroupContainCards(std::vector<int> cards, bool sortSanhOnly = true);
    bool isCardInSoftGroup(int cardValue, SoftGroupCard group);

    float unselectAllCardsExclude(int excludeCardValue, bool anim);

    void shakeTable();
    void shakeTableWithDelay(float delay);

    cocos2d::Point getDumpPlacePosInLayoutCard();
    cocos2d::Size getDumpPlaceSize();

    cocos2d::Point getDrawPlacePosInLayoutCard();
    cocos2d::Size getDrawPlaceSize();

    bool canDumpCard();

    cocos2d::Point correctOutOfBoundDraggingCardPosition(cocos2d::Point cardPos);
    float calculateDraggingCardRotation(cocos2d::Point cardPos);

    void initParticles();
    void visibleParticleKnock();
    void visibleParticleGin();
    void visibleParticleBigGin();
    void hideAllParticle();

    void visibleTurnLayout(bool visible);
    void setTurn(int index);
};

#endif //__GPGinControlCardLayer__

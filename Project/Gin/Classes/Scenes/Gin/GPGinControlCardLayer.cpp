#include "GPGinControlCardLayer.h"
#include "Helper/GameUtils.h"
#include "Manager/SoundManager.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"
#include "Define/GameDefine.h"
#include "GPGinScene.h"
#include "Network/MessageSender.h"
#include "Helper/CardIndexConverter.h"
#include "GPGinEffect.h"
#include "Manager/BoardManager.h"
#include "ServerOffline/SvrHelper.h"
#include "ServerOffline/Rules/RuleShowCard.h"
#include "ServerOffline/SvrHelper.h"
#include "ServerOffline/SvrCard.h"
#include "ServerOffline/Rules/RuleCommon.h"
#include "Component/GinUserHandNode.h"
#include "Helper/fmt/format.h"
#include "FirebaseGG/Firebase.h"
#include "Native/CallToOtherLanguage.h"

float GPGinControlCardLayer::_distanceBetweenTwoCards = 0;
float GPGinControlCardLayer::_distanceGapBetweenTwoMelds = 0;
float GPGinControlCardLayer::_angleBetweenTwoCards = 0;
float GPGinControlCardLayer::_r = 0;

USING_NS_CC;
using namespace ui;
using namespace std;

#define TAG_ACTION_ZOOM_IN_OUT 3
#define CHEAT_OPEN_CARD_ON_HAND 0
#define D_POS_HINT_THROW                22
#define PICK_CARD_HEIGHT 20
#define DELAY_SHOW_CARD_ON_TABLE 0.2f

#define POSITION_OFFSET_Y_LAYOUT_CARD 10 //USer for curve style

#define DRAW_AND_TAKE_ANIM_ENABLE true
#define DRAW_AND_TAKE_ANIM_TOUCH_HOLD_COUNT_DOWN 0.2f
#define DRAW_AND_TAKE_ANIM_CARD_RETURN_DURATION 0.1f

float GPGinControlCardLayer::DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 = DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1_DEFINE;
int   GPGinControlCardLayer::_currentSortMethodIndex = 0;
std::vector<std::pair<TypeArrangeCardOnHand, std::string>> GPGinControlCardLayer::_listSortMethod;

GPGinControlCardLayer::~GPGinControlCardLayer()
{
    CC_SAFE_RELEASE(_layoutGroupCardTittle);
    CC_SAFE_RELEASE(_layoutNumberOfRemainCardTemp);
    CC_SAFE_RELEASE(_layoutHintTemp);

    CC_SAFE_RELEASE(_layoutDark);

    _listUserHandNode.clear();

    if (_ginDrawAndTakeFakeCard != nullptr)
    {
        _ginDrawAndTakeFakeCard->removeFromParent();
        _ginDrawAndTakeFakeCard->release();
    }
}

GPGinControlCardLayer::GPGinControlCardLayer()
{
    _gameGin = nullptr;
    _canTouchCard = true;
    _status = ControlStatus::CONTROL_STATUS_NORMAL;
    _listSortMethod.clear();
    _listSortMethod = {{TypeArrangeCardOnHand::TYPE_ARRANGE_INCREASE, GET_TEXT_BY_ID(TXT_BUTTON_ARRANGE_TYPE_RANK)},};
    _listMeldHighLightColor = {MeldColorType::COLOR_1, MeldColorType::COLOR_2, MeldColorType::COLOR_3};

    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);
}

bool GPGinControlCardLayer::init()
{
    if (!Layer::init())
        return false;
    return true;
}

cocos2d::ui::Layout *GPGinControlCardLayer::getLayoutCard()
{
    return _layoutCards;
}

cocos2d::Node *GPGinControlCardLayer::getNodeAnchorCard()
{
    return _nodeAnchorCards;
}

std::vector<GinCard *> GPGinControlCardLayer::getListHandCardsMainUser()
{
    return _listHandCardsMainUser;
}

const std::vector<GroupCard> &GPGinControlCardLayer::getListGroupCardsMainUser()
{
    return _listGroupHandCardsMainUser;
}

std::vector<std::vector<int>> GPGinControlCardLayer::getListMeldOnHandMainUser()
{
    std::vector<std::vector<int>> listMeld;
    for (auto const &groupCard: _listGroupHandCardsMainUser)
    {
        if (this->MeldIsValid(groupCard._listCards))
        {
            std::vector<int> meld;
            for (auto const &card: groupCard._listCards)
                meld.push_back(card->getCardIndex());
            listMeld.push_back(meld);
        }
    }
    return listMeld;
}

std::vector<std::vector<int>> GPGinControlCardLayer::getListMeldOnHandMainUserBySoftGroup()
{
    std::vector<std::vector<int>> listMeld;
    for (auto const &groupCard: _listSoftGroupHandCardsMainUser)
    {
        if (this->MeldIsValid(groupCard._listCard))
        {
            std::vector<int> meld;
            for (auto const &card: groupCard._listCard)
                meld.push_back(card->getCardIndex());
            listMeld.push_back(meld);
        }
    }
    return listMeld;
}

std::vector<GinCard *> GPGinControlCardLayer::getListThrowCards(int orderIdxFristDeal /*= 0*/)
{
    return _listThrowCards;
}

std::vector<int> GPGinControlCardLayer::getListThrowCardsValue(int orderIdxFristDeal/* = 0*/)
{
    std::vector<GinCard *> listThrowCard = this->getListThrowCards(orderIdxFristDeal);
    std::vector<int> listThrowCardValue;
    for (auto const &card: listThrowCard)
        listThrowCardValue.push_back(card->getCardIndex());
    return listThrowCardValue;
}

cocos2d::Vector<GinUserHandNode *> GPGinControlCardLayer::getListUserHandNode()
{
    return _listUserHandNode;
}

void GPGinControlCardLayer::initLayout(const cocos2d::Size &screenSize, float scale)
{
//    if (screenSize.width <= 1186)
//        GPGinControlCardLayer::DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 =
//                DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1_DEFINE - 1.6f;
    _screenSize = screenSize;

    Node *node = CSLoader::createNode("ui/GameplayScene/Gin/GPGinControlLayer.csb");
    Node *child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout *>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->addChild(_layoutRoot, (int) ZorderControlLayer::LAYOUT_ROOT);

//    this->_layoutRoot->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
//    this->_layoutRoot->setColor(Color3B::GREEN);

    if (s_enableSafeArea)
    {
        float padding = SAFE_AREA_PADDING;

        LayoutComponent *layoutComponent = (LayoutComponent *) this->_layoutRoot->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + padding);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + padding);
        layoutComponent->refreshLayout();

    }
    if (s_resolution_ratio <= RESOLUTION_1_6)
    {
        Helper::doLayout(this);

        float paddingWidth = -1 * (screenSize.width - _layoutRoot->getContentSize().width) / 2;
        float paddingHeight = -1 * (screenSize.height - _layoutRoot->getContentSize().height) / 2;

        LayoutComponent *layoutComponent = (LayoutComponent *) _layoutRoot->getComponent(__LAYOUT_COMPONENT_NAME);

//        layoutComponent->setStretchHeightEnabled(true);
//        layoutComponent->setStretchWidthEnabled(true);

        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + paddingWidth);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + paddingWidth);
        layoutComponent->setTopMargin(layoutComponent->getTopMargin() + paddingHeight);
        layoutComponent->setBottomMargin(layoutComponent->getBottomMargin() + paddingHeight);

        layoutComponent->refreshLayout();

        this->_layoutRoot->setScale(scale);

//        _screenSize = Director::getInstance()->getWinSize();
//        this->setContentSize(_screenSize);
    }

    Helper::doLayout(this);

    _layoutGroupCardTittle = static_cast<Layout *>(node->getChildByName("LayoutGroupCardTitle"));
    _layoutGroupCardTittle->retain();

    _layoutNumberOfRemainCardTemp = static_cast<Layout *>(node->getChildByName("LayoutRemainingNum"));
    _layoutNumberOfRemainCardTemp->retain();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        GinUserHandNode *tongitUserHandNode = GinUserHandNode::create();
        tongitUserHandNode->setLayoutRemainCard(_layoutNumberOfRemainCardTemp);
        _listUserHandNode.pushBack(tongitUserHandNode);
    }

    _layoutHintTemp = static_cast<Layout *>(node->getChildByName("LayoutHint"));
    _layoutHintTemp->retain();

    _layoutCards = static_cast<Layout *>(Helper::seekWidgetByName(_layoutRoot, "PanelCards"));
    _layoutCards->setLocalZOrder((int) ZorderControlLayer::CARD_ON_HAND_USER);
    if (STYLE_CARDS_ON_HAND == STYLE_CARDS_ON_HAND_CURVE)
    {
        Point posLayoutCards = _layoutCards->getPosition();
        posLayoutCards.y += POSITION_OFFSET_Y_LAYOUT_CARD;

        _layoutCards->setPosition(posLayoutCards);
    }

    _nodeAnchorCards = static_cast<Node *>(_layoutRoot->getChildByName("NodeMakerAnchorCards"));
    Vec2 posNodeAnchorCards = _nodeAnchorCards->getPosition();
    posNodeAnchorCards.y = posNodeAnchorCards.y / 10;
    _nodeAnchorCards->setPosition(posNodeAnchorCards);

    this->_anchorCardInLayoutCardPosition = _layoutCards->convertToNodeSpace(
                                                _nodeAnchorCards->getParent()->convertToWorldSpace(_nodeAnchorCards->getPosition()));

    _distanceBetweenTwoCards =
            (_layoutCards->getContentSize().width - GIN_WIDTH_CARD * CARD_IN_HAND_SCALE) / (TOTAL_NUMBER_CARD - 1) +
            (s_resolution_ratio <= RESOLUTION_1_6 ? 5 : 0);
    this->getAngleBetweenTwoCards(10);
    _distanceGapBetweenTwoMelds = (s_resolution_ratio <= RESOLUTION_1_6 ?
                                   WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_ON_HAND_MAIN_USER - 10
                                                                        : WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_ON_HAND_MAIN_USER);

    _buttonDraw = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonBocBai"));
    _buttonDraw->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));
    _buttonDraw->setOpacity(0);

    _hintDrawCard = (Layout *) _layoutHintTemp->clone();
    _layoutRoot->addChild(_hintDrawCard, (int) ZorderControlLayer::EFFECT_HINT);

    _hintTakeCard = (Layout *) _layoutHintTemp->clone();
    _layoutRoot->addChild(_hintTakeCard, (int) ZorderControlLayer::EFFECT_HINT);

    _buttonPass = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonPass"));
    _buttonPass->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));

    _buttonKnock = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonKnock"));
    _buttonKnock->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));

    _buttonGin = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonGin"));
    _buttonGin->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));

    _buttonBigGin = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonBigGin"));
    _buttonBigGin->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));

    _buttonSort = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonXep"));
    _buttonSort->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));

    _buttonFindMeld = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonFindMeld"));
    _buttonFindMeld->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));

    _buttonTake = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonTake"));
    _layoutRoot->removeChild(_buttonTake);
    _layoutRoot->addChild(_buttonTake, (int) ZorderControlLayer::EFFECT_HINT);
    _buttonTake->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));
    _buttonTake->setColor(Color3B::RED);
    _buttonTake->setOpacity(0.0f);

    _buttonDump = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonDump"));
    _layoutRoot->removeChild(_buttonDump);
    _layoutRoot->addChild(_buttonDump, (int) ZorderControlLayer::EFFECT_HINT);
    _buttonDump->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));
    _buttonDump->setColor(Color3B::RED);
    _buttonDump->setOpacity(0.0f);

    _buttonGroup = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonGroup"));
    _buttonGroup->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));
    _buttonGroup->setLocalZOrder((int) ZorderControlLayer::CARD_ON_HAND_USER + 1);

    _buttonUngroup = static_cast<Button *>(Helper::seekWidgetByName(_layoutRoot, "ButtonUnGroup"));
    _buttonUngroup->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));
    _buttonUngroup->setLocalZOrder((int) ZorderControlLayer::CARD_ON_HAND_USER + 1);

    _layoutNotify = static_cast<Layout *>(Helper::seekWidgetByName(_layoutRoot, "LayoutNotify"));
    _layoutNotify->setVisible(false);

    _layoutStepHint = static_cast<Layout *>(Helper::seekWidgetByName(_layoutRoot, "LayoutStepHint"));
    _layoutStepHint->setVisible(false);

    _layoutTouchDraw = static_cast<Layout *>(Helper::seekWidgetByName(_layoutRoot, "PanelTouchBocBai"));
    _layoutTouchTake = static_cast<Layout *>(Helper::seekWidgetByName(_layoutRoot, "PanelTouchTake"));

    _layoutDark = Layout::create();
    _layoutDark->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _layoutDark->setContentSize(screenSize);
    _layoutDark->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _layoutDark->setColor(Color3B::BLACK);
    _layoutDark->setVisible(false);
    _layoutDark->retain();

    hideAllEffectHint();

    _layoutParticle =  static_cast<Layout *>(Helper::seekWidgetByName(_layoutRoot, "PanelButtonParticle"));
    this->initParticles();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GPGinControlCardLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GPGinControlCardLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GPGinControlCardLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(GPGinControlCardLayer::onTouchCancelled, this);
    touchListener->setSwallowTouches(false);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _layoutCards);

    scheduleUpdate();

    this->initDrawAndTakeAnimationCachedData();


    _layoutTurn = static_cast<Layout *>(Helper::seekWidgetByName(_layoutRoot, "PanelTurn"));
    _imageTurnBg = static_cast<ImageView *>(Helper::seekWidgetByName(_layoutTurn, "ImageBG"));
    _textTurn = static_cast<Text *>(Helper::seekWidgetByName(_layoutTurn, "TextTurn"));
    _imageTurnBot = static_cast<ImageView *>(Helper::seekWidgetByName(_layoutTurn, "ImageArrowBot"));
    _imageTurnUser = static_cast<ImageView *>(Helper::seekWidgetByName(_layoutTurn, "ImageArrowUser"));

    Point imgTurnBotPos = _imageTurnBot->getPosition();
    Point imgTurnUserPos = _imageTurnUser->getPosition();

    _imageTurnBot->setPosition(imgTurnBotPos + Vec2(0, 20));
    _imageTurnUser->setPosition(imgTurnUserPos + Vec2(0, -20));

}

void GPGinControlCardLayer::alignChildrenPostion(const cocos2d::Vec2 &drawCardPos, const cocos2d::Vec2 &takeCardPos,
                                                 cocos2d::ui::Button *buttonThrowCards)
{
    _buttonDraw->setPosition(this->convertWorldPosToLayoutRootPos(drawCardPos));
    _buttonTake->setPosition(this->convertWorldPosToLayoutRootPos(takeCardPos));
    _buttonDump->setPosition(this->convertWorldPosToLayoutRootPos(takeCardPos));

    _layoutTouchDraw->setPosition(this->convertWorldPosToLayoutRootPos(drawCardPos));
    _layoutTouchTake->setPosition(this->convertWorldPosToLayoutRootPos(takeCardPos));

    _layoutTouchDraw->setContentSize(_buttonDraw->getContentSize());
    _layoutTouchDraw->setScale(_buttonDraw->getScale());

    _layoutTouchTake->setContentSize(_buttonTake->getContentSize());
    _layoutTouchTake->setScale(_buttonTake->getScale());

    _dumpPlacePosInLayoutCard = this->_layoutCards->convertToNodeSpace(takeCardPos);
    _drawPlacePosInLayoutCard = this->_layoutCards->convertToNodeSpace(drawCardPos);

    _buttonShowThrowCards = buttonThrowCards;
    auto posOriginalButtonShowThrowCards = this->convertWorldPosToLayoutRootPos(
            _buttonShowThrowCards->getParent()->convertToWorldSpace(_buttonShowThrowCards->getPosition()));
    _buttonShowThrowCards->removeFromParent();
    _buttonShowThrowCards->addTouchEventListener(CC_CALLBACK_2(GPGinControlCardLayer::onButtonTouch, this));
    _buttonShowThrowCards->setPosition(posOriginalButtonShowThrowCards);
    _layoutRoot->addChild(_buttonShowThrowCards, (int) ZorderControlLayer::EFFECT_HINT_BELOW_CARD);
    _listLayoutCardSize.clear();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listLayoutCardSize.push_back(_gameGin->getLayoutCardSize(i));
}

cocos2d::ui::Layout *GPGinControlCardLayer::getLayoutRoot()
{
    return _layoutRoot;
}

void GPGinControlCardLayer::reset()
{
    clearAllCard();
    _cardsIsOnHand = false;
    _canTouchCard = true;
    _buttonDraw->setVisible(false);
    _buttonPass->setVisible(false);
    _buttonKnock->setVisible(false);
    _buttonGin->setVisible(false);
    _buttonBigGin->setVisible(false);
    _buttonDump->setVisible(false);
    _buttonTake->setVisible(false);
    _buttonSort->setVisible(false);
    _buttonShowThrowCards->setVisible(false);
    _buttonFindMeld->setVisible(false);
    _layoutNotify->stopAllActions();
    _layoutNotify->setVisible(false);

    this->visibleTurnLayout(false);

    this->hideAllParticle();

    showUngroupButton(false, std::vector<GinCard *>());
    showGroupButton(false, std::vector<GinCard *>());

    _listDealCardPos = _gameGin->getDealPos();
//    _listShowCardPos = _gameGin->getShowCardPos();
    _listShowCardPos   = _gameGin->getInterfaceLayer()->getShowCardGlobalPos();
    _listThrowCardPos = _gameGin->getThrowPos();
    _listShowStatusPos = _gameGin->getShowStatusPos();
    _listHintDropArea.clear();
    for (const auto &hintDropArea: _gameGin->getInterfaceLayer()->getHintDropPlaceArea())
        _listHintDropArea.insert(std::pair<HintDropPlaceType, std::pair<Rect, cocos2d::Value>>(hintDropArea.first,
                                                                                               std::pair<Rect, cocos2d::Value>(
                                                                                                       hintDropArea.second,
                                                                                                       cocos2d::Value())));
    _listThrowCards.clear();
    _listTakeCard.clear();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listTakeCard.push_back(std::vector<GinCard *>());

    _listMeldsAllUser.clear();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listMeldsAllUser.push_back(std::vector<std::vector<GinCard *>>());

    _listRemainCards.clear();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listRemainCards.push_back(std::vector<GinCard *>());

    _listShowCards.clear();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listShowCards.push_back(std::vector<GinCard *>());
    _listSuggestTakeCardsCached.clear();
    _listCardsWereAutoSelectedForHintTake.clear();
    _listCardsWereAutoUngroupForHintTake.clear();
    resetDataByDrapDropCards(true);
    for (auto const &hand: _listUserHandNode)
        hand->reset();

    _status = ControlStatus::CONTROL_STATUS_NORMAL;
    _currentScore = 0;
    _shouldSortCardAfterPickingCard = false;
    _currentDragCardsType = GinCardDragType::GinDRAGCARD_UNKNOW;

    _listDistanceBetweenTwoCardOnTable1Short.clear();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listDistanceBetweenTwoCardOnTable1Short.push_back(
                {DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1, DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1});

    this->setThrowCardsVisible(true);
    this->setShowCardsVisible(true);

    this->_ginDrawAndTakeFakeCardHandIndex = -1;
    this->resetDrawAndTakeAnimationCachedData();
}

void GPGinControlCardLayer::clearAllCard()
{
    _listHandCardsMainUser.clear();
    _layoutCards->removeAllChildren();
    _layoutCards->setColor(Color3B::WHITE);

    for (int i = 1; i < _listHandCardsAllUser.size(); i++)
    {
        for (int j = 0; j < _listHandCardsAllUser[i].size(); j++)
            _listHandCardsAllUser[i][j]->removeFromParent();
        _listHandCardsAllUser[i].clear();
    }
    _listHandCardsAllUser.clear();

    for (auto &card: _listThrowCards)
        if (card)
            _layoutRoot->removeChild(card, true);
    _listThrowCards.clear();

    for (auto &cards: _listTakeCard)
        for (auto &card: cards)
            if (card)
                _layoutRoot->removeChild(card);
    _listTakeCard.clear();

    for (auto &cards: _listShowCards)
        for (auto &card: cards)
            if (card)
                _layoutRoot->removeChild(card);
    _listShowCards.clear();

    for (auto &meld: _listMeldsAllUser)
        for (auto &cards: meld)
            for (auto &card: cards)
                if (card)
                    _layoutRoot->removeChild(card);
    _listMeldsAllUser.clear();

    for (auto &cards: _listRemainCards)
        for (auto &card: cards)
            if (card)
                _layoutRoot->removeChild(card);
    _listRemainCards.clear();

    for (auto card: _listHandCardsMainUser)
        _layoutCards->removeChild(card);
    _listHandCardsMainUser.clear();
    this->clearGroup(false);
    this->clearSoftGroup();

    this->resetDrawAndTakeAnimationCachedData();
}

void
GPGinControlCardLayer::finishDealCard(int orderIdxFirstPlaying, int upcardValue, const cocos2d::Point &upCardStartPos)
{
    this->setVisibleButtonXep(true);
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        if (i != MAIN_USER_SLOT_INDEX)
            this->updateNumberOfRemainCards(i);
    this->showUpCard(upcardValue, this->_layoutRoot->convertToNodeSpace(upCardStartPos));
    if (orderIdxFirstPlaying == MAIN_USER_SLOT_INDEX)
        _buttonPass->setVisible(true);
}

void GPGinControlCardLayer::setUserOnTurn(int orderIdx, bool onTurn)
{
    _canTouchCard = true;
    if (orderIdx == MAIN_USER_SLOT_INDEX)
    {
        showStepHint(false);
        if (onTurn)
        {
//            showStepHint(true);
            showStepHint(false);
        } else
        {
            hideAllEffectHint();
        }
    } else
    {
        _listUserHandNode.at(orderIdx)->setOnTurn(onTurn);
    }

    //
    if(onTurn == true)
    {
        this->setTurn(orderIdx);
    }
}

RepeatForever *GPGinControlCardLayer::actionHint(const Vec2 &move)
{
    return RepeatForever::create(
            Sequence::create(MoveBy::create(0.35f, move), MoveBy::create(0.35f, Vec2(0, 0) - move), nullptr));
}

GinCard *GPGinControlCardLayer::createCard(int index, float scale, bool open, bool isSmall/* = false*/)
{
    GinCard *tlCard = GinCard::create(this);
    tlCard->setCardIndex(index, isSmall);
    tlCard->setCardValue(index);
    tlCard->setBack(1);
    tlCard->setScale(scale);
    if (open)
        tlCard->open();
    return tlCard;
}

void GPGinControlCardLayer::initListHandCardsAllUser(const std::vector<std::vector<int>> &listDealCards)
{
    _listHandCardsAllUser.clear();
    int index = 0;
    for (auto const &cards: listDealCards)
    {
        if (cards.size() != 0)
        {
            if (index == MAIN_USER_SLOT_INDEX) //0
                this->initListHandCardsMainUser(cards);
            else
                this->initListHandCardsAnthorUser(cards, index);
        } else
        {
            _listHandCardsAllUser.push_back(std::vector<GinCard *>());
        }
        index++;
    }
}

void GPGinControlCardLayer::initListHandCardsMainUser(const std::vector<int> &listIndex)
{
    for (const int &index: listIndex)
    {
        auto tlCard = createCard(index, CARD_IN_HAND_SCALE, false);
        _listHandCardsMainUser.push_back(tlCard);
        _layoutCards->addChild(tlCard);
    }
    _listHandCardsAllUser.push_back(_listHandCardsMainUser);
    this->showCardsOnHand(_listHandCardsMainUser, MAIN_USER_SLOT_INDEX);
}

void GPGinControlCardLayer::initListHandCardsAnthorUser(const std::vector<int> &listIndex, int orderIdx)
{
    std::vector<GinCard *> listHandCards;
    for (const int &index: listIndex)
    {
        auto tlCard = createCard(index, CARD_IN_HAND_OTHER_PLAYER_SCALE, false);
#if CHEAT_OPEN_CARD_ON_HAND
        tlCard->open();
#endif
        listHandCards.push_back(tlCard);
        _layoutRoot->addChild(tlCard);
    }
    _listHandCardsAllUser.push_back(listHandCards);
    this->showCardsOnHand(listHandCards, orderIdx);
}

void GPGinControlCardLayer::showUpCard(int card, const cocos2d::Point &startPos)
{
    int sizeOfListThrowCards = this->getListThrowCards().size();
    Vec2 extraDistance = Vec2(-0.2f * (sizeOfListThrowCards - 1), 0.3f * (sizeOfListThrowCards - 1));
//    Vec2 posTarget            = _listThrowCardPos.at(0) + extraDistance;
    Vec2 posTarget = this->convertWorldPosToLayoutRootPos(_listThrowCardPos.at(0) + extraDistance);
    // Create card
    GinCard *tlCard = createCard(card, GPGinInterfaceLayer::_scaleTakeCard, true);
    tlCard->setPosition(posTarget);
    tlCard->setRotation(0);
    tlCard->setOpacity(255);
    tlCard->close(false);
    //    tlCard->open(true);
    _layoutRoot->addChild(tlCard, (int) ZorderControlLayer::CARD_THROW + this->getListThrowCards().size());
    _listThrowCards.push_back(tlCard);

    Point dstPos = tlCard->getPosition();
    tlCard->setPosition(startPos);
    tlCard->runAction(Sequence::create(MoveTo::create(0.2f, dstPos), CallFunc::create([=]
                                                                                      {
                                                                                          tlCard->open(true);
                                                                                      }), nullptr));
}

void GPGinControlCardLayer::throwCard(int orderId, int card, int actionType)
{
    cocos2d::Point posCard;
    float rotation = 0;
    float scale = CARD_IN_HAND_SCALE;

    bool finalCard = false;
    if (actionType != ActionType::K_NONE)
    {
        finalCard = true;
    }

    this->hideAllParticle();

    if (orderId == MAIN_USER_SLOT_INDEX)
    {
        for (int i = 0; i < _listHandCardsMainUser.size(); i++)
        {
            GinCard *tgCard = _listHandCardsMainUser.at(i);
            if (tgCard->getCardIndex() == card)
            {
                int groupIndex = this->checkCardInAGroup(card);
                if (groupIndex >= 0)
                    this->removeCardOutOfGroup(groupIndex, card);

                posCard = this->convertWorldPosToLayoutRootPos(
                        _layoutCards->convertToWorldSpace(tgCard->getPosition()));
                rotation = tgCard->getRotation();
                scale = tgCard->getScale();
                _layoutCards->removeChild(_listHandCardsMainUser.at(i), true);
                _listHandCardsMainUser.erase(_listHandCardsMainUser.begin() + i);
                bool newMelds = this->updateListSoftGroupHandCardMainUserOnThrowCard(tgCard);
                if (newMelds)
                    MessageSender::requestCustomMelds(MAIN_USER_SLOT_INDEX, getListMeldOnHandMainUserBySoftGroup());
                break;
            }
        }
        playEffectSortCardsOnHand();
    } else
    {
        auto card2 = getHandCardsOnUser(orderId);
        if (card2)
        {
            posCard = this->convertWorldPosToLayoutRootPos(_gameGin->getHandCardPos()[orderId]);
            rotation = card2->getRotation();
            scale = card2->getScale();
            _layoutRoot->removeChild(card2);
        }
        this->updateNumberOfRemainCards(orderId, true);
    }
    GinCard *tlCard = createCard(card, scale, true);
    tlCard->setPosition(posCard);
    tlCard->setRotation(rotation);
    tlCard->setOpacity(255);
    _layoutRoot->addChild(tlCard, (int) ZorderControlLayer::CARD_THROW + this->getListThrowCards().size());
    _listThrowCards.push_back(tlCard);

    int sizeOfListThrowCards = this->getListThrowCards().size();
    Vec2 extraDistance = Vec2(-0.2f * (sizeOfListThrowCards - 1), 0.3f * (sizeOfListThrowCards - 1));
    Vec2 posTarget = this->convertWorldPosToLayoutRootPos(_listThrowCardPos.at(orderId) + extraDistance);

    float timeThrow = TIME_CARD_THROW;
    float distance = tlCard->getPosition().distance(posTarget);
    bool zoomCard = true;
    if (orderId == MAIN_USER_SLOT_INDEX && distance < 200)
    {
        timeThrow = TIME_CARD_THROW * 0.5f;
        zoomCard = false;
    }

//    finalCard = true;
    if (finalCard == false)
    {
//        tlCard->setLocalZOrder((int) ZorderControlLayer::CARD_ON_HAND_USER + this->getListThrowCards().size());
        if (orderId == MAIN_USER_SLOT_INDEX)
        {
            if (zoomCard)
            {
                tlCard->stopAllActions();
                tlCard->runAction(Spawn::create(EaseCubicActionOut::create(MoveTo::create(timeThrow, posTarget)),
                                                Sequence::create(ScaleTo::create(timeThrow * 0.2f,
                                                                                 GPGinInterfaceLayer::_scaleTakeCard +
                                                                                 0.1f),
                                                                 ScaleTo::create(timeThrow * 0.7f,
                                                                                 GPGinInterfaceLayer::_scaleTakeCard +
                                                                                 0.1f),
                                                                 ScaleTo::create(timeThrow * 0.1f,
                                                                                 GPGinInterfaceLayer::_scaleTakeCard),
                                                                 nullptr),
                                                RotateTo::create(timeThrow, 0),
                                                nullptr));
            } else
            {
                tlCard->stopAllActions();
                tlCard->runAction(Spawn::create(EaseCubicActionOut::create(MoveTo::create(timeThrow, posTarget)),
                                                Sequence::create(
                                                        ScaleTo::create(timeThrow, GPGinInterfaceLayer::_scaleTakeCard),
                                                        nullptr),
                                                RotateTo::create(timeThrow, 0),
                                                nullptr));
            }
        } else
        {
            tlCard->stopAllActions();
            tlCard->runAction(Spawn::create(EaseCubicActionOut::create(MoveTo::create(timeThrow, posTarget)),
                                            Sequence::create(ScaleTo::create(timeThrow * 0.2f,
                                                                             GPGinInterfaceLayer::_scaleTakeCard +
                                                                             0.1f),
                                                             ScaleTo::create(timeThrow * 0.7f,
                                                                             GPGinInterfaceLayer::_scaleTakeCard +
                                                                             0.1f),
                                                             ScaleTo::create(timeThrow * 0.1f,
                                                                             GPGinInterfaceLayer::_scaleTakeCard),
                                                             nullptr),
                                            RotateTo::create(timeThrow, 0),
                                            nullptr));
        }
    } else
    {
        this->visibleTurnLayout(false);

        bool isUser = (orderId == MAIN_USER_SLOT_INDEX);
        float spinDuration = tlCard->getSpinEffectDuration();
        float orgCardScale = CARD_IN_HAND_SCALE;
        float orgOpacity = tlCard->getOpacity();

        GinCard *fakeCard = this->createCard(tlCard->getCardValue(), tlCard->getScale(), true);
        fakeCard->setOpacity(tlCard->getOpacity());
        fakeCard->setVisible(false);
        tlCard->getParent()->addChild(fakeCard, tlCard->getLocalZOrder() - 1);

        tlCard->setOpacity(0);
        tlCard->stopAllActions();
        tlCard->setRotation(0);

        Point centerGlobalPoint = this->_layoutRoot->getContentSize() / 2;
//        Point centerPoint = tlCard->getParent()->convertToNodeSpace(centerGlobalPoint);
        Point centerPoint = centerGlobalPoint;

        fakeCard->setPosition(centerPoint);
//        fakeCard->setPosition(posTarget);

        tlCard->setLocalZOrder((int) ZorderControlLayer::CARD_ON_HAND_USER + this->getListThrowCards().size());


        BezierTo *bezierSpinTo = this->getBezierAnimationVertical(tlCard->getPosition(), centerPoint, isUser,
                                                                  spinDuration);
        BezierTo *bezierMoveTo = this->getBezierAnimationHorizontal(centerPoint, posTarget, isUser, timeThrow);

        // Card anim
        Vector<FiniteTimeAction *> listAction;
        listAction.pushBack(Spawn::create(CallFunc::create([=]
                                                           {
                                                               tlCard->playEffectSpinCard();
                                                           }),
                                          bezierSpinTo,
                                          FadeTo::create(spinDuration / 3, orgOpacity),
                                          Sequence::create(ScaleTo::create(spinDuration, orgCardScale * 1.4f),
                                                           EaseBackIn::create(ScaleTo::create(0.1f, orgCardScale * 1.1f)),
                                                           nullptr),
                                          nullptr));
//        listAction.pushBack(DelayTime::create(TIME_FINAL_CARD_DELAY));

//        listAction.pushBack(Spawn::create(
//                bezierMoveTo,
//                Sequence::create(ScaleTo::create(timeThrow * 0.2f,
//                                                 GPGinInterfaceLayer::_scaleTakeCard +
//                                                 0.1f),
//                                 ScaleTo::create(timeThrow * 0.7f,
//                                                 GPGinInterfaceLayer::_scaleTakeCard +
//                                                 0.1f),
//                                 ScaleTo::create(timeThrow * 0.1f,
//                                                 GPGinInterfaceLayer::_scaleTakeCard),
//                                 nullptr),
//                RotateTo::create(timeThrow, 0),
//                nullptr));

        listAction.pushBack(CallFunc::create([=]
                                             {
                                                 this->shakeTable();
                                                 fakeCard->setVisible(true);
                                                 fakeCard->setScale(tlCard->getScale());
                                                 fakeCard->runAction(
                                                         Sequence::create(Spawn::create(FadeTo::create(0.5f, 0),
                                                                                        ScaleTo::create(0.5f,
                                                                                                        /*GPGinInterfaceLayer::_scaleTakeCard*/tlCard->getScale() *
                                                                                                        1.5f),
                                                                                        nullptr),
                                                                          RemoveSelf::create(),
                                                                          nullptr));
                                             }));
        listAction.pushBack(CallFunc::create([=]
                                             {
                                                 GinMatchEndStatus status = GinMatchEndStatus::GIN_MATCH_END_STATUS_NONE;
                                                 if (actionType == ActionType::K_KNOCK)
                                                 {
                                                     status = GinMatchEndStatus::GIN_MATCH_END_STATUS_KNOCK;
                                                 } else if (actionType == ActionType::K_GIN)
                                                 {
                                                     status = GinMatchEndStatus::GIN_MATCH_END_STATUS_GIN;
                                                 } else if (actionType == ActionType::K_BIG_GIN)
                                                 {
                                                     status = GinMatchEndStatus::GIN_MATCH_END_STATUS_BIGGIN;
                                                 }
                                                 if (status != GinMatchEndStatus::GIN_MATCH_END_STATUS_NONE)
                                                 {
                                                     this->_gameGin->showMatchEndStatus(status, Vec2::ZERO);
                                                 }
                                             }));

        listAction.pushBack(DelayTime::create(TIME_FINAL_CARD_DELAY));
        listAction.pushBack(Spawn::create(
                bezierMoveTo,
                Sequence::create(ScaleTo::create(timeThrow * 0.2f,
                                                 GPGinInterfaceLayer::_scaleTakeCard +
                                                 0.1f),
                                 ScaleTo::create(timeThrow * 0.7f,
                                                 GPGinInterfaceLayer::_scaleTakeCard +
                                                 0.1f),
                                 ScaleTo::create(timeThrow * 0.1f,
                                                 GPGinInterfaceLayer::_scaleTakeCard),
                                 nullptr),
                RotateTo::create(timeThrow, 0),
                nullptr));

        Sequence *action = Sequence::create(listAction);

        tlCard->runAction(action);

        //Dark layout action
        this->_layoutDark->setOpacity(0);
        this->_layoutDark->setVisible(true);
        this->_layoutRoot->addChild(this->_layoutDark, tlCard->getLocalZOrder() - 1);
        this->_layoutDark->setPosition(centerGlobalPoint);

        this->_layoutDark->runAction(Sequence::create(FadeTo::create(spinDuration, 100),
                                                      DelayTime::create(TIME_FINAL_CARD_DELAY),
                                                      FadeTo::create(timeThrow * 0.5f, 0),
                                                      RemoveSelf::create(),
                                                      nullptr));
    }

    this->checkStatusOfShowThrowCardButton();
    this->resetDataByDrapDropCards(true);
}

void GPGinControlCardLayer::highlightPhom(const std::vector<std::vector<int>> &listPhomHighLight, bool isDrawCard)
{
    if (listPhomHighLight.size() > 0)
    {
        for (auto const &tgCard: _listHandCardsMainUser)
        {
            for (auto const &phom: listPhomHighLight)
            {
                bool track = false;
                for (const int &card: phom)
                {
                    if (tgCard->getCardIndex() == card)
                    {
                        tgCard->setEffectHightLightPhom(true, Color3B::WHITE);
                        track = true;
                        break;
                    }
                }
                if (track)
                {
                    if (isDrawCard && phom.size() > 3)
                        SoundManager::playSoundEffect(SoundEvent::TL_SECRET_MELD);
                    break;
                }
            }
        }
    }
}

void GPGinControlCardLayer::drawCard(int orderId, int index)
{
    if (orderId == MAIN_USER_SLOT_INDEX)
    {
        if(this->_ginDrawAndTakeFakeCard == nullptr)
        {
            std::vector<GinCardProperty> listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
                    _listHandCardsMainUser.size() + 1);
            auto tlCard = createCard(index, CARD_IN_HAND_SCALE, false);
            int possibleCardIndex = this->getPossibleNewCardIndexBySoftGroup(index);
            int propertyCardIndex = listPropertyCard.size() - 1;

            if (possibleCardIndex != -1)
            {
                propertyCardIndex = possibleCardIndex;
            } else if (this->_ginDrawAndTakeFakeCardHandIndex != -1)
            {
                propertyCardIndex = this->_ginDrawAndTakeFakeCardHandIndex;
            }
//        propertyCardIndex = (this->_ginDrawAndTakeFakeCardHandIndex != -1) ? this->_ginDrawAndTakeFakeCardHandIndex : (listPropertyCard.size()-1);
            const GinCardProperty &endPro = listPropertyCard[propertyCardIndex];

            tlCard->setPosition(endPro._positionOnHand);
            tlCard->setVisible(false);

//        _listHandCardsMainUser.push_back(tlCard);
            int cardZOrder = (int) ZorderControlLayer::CARD_ON_HAND_USER;

            if (possibleCardIndex != -1)
            {
                _listHandCardsMainUser.insert(_listHandCardsMainUser.begin() + propertyCardIndex, tlCard);
                cardZOrder += propertyCardIndex;
            } else if (this->_ginDrawAndTakeFakeCardHandIndex != -1)
            {
                _listHandCardsMainUser.insert(_listHandCardsMainUser.begin() + propertyCardIndex, tlCard);
                cardZOrder += this->_ginDrawAndTakeFakeCardHandIndex;
            } else
            {
                _listHandCardsMainUser.push_back(tlCard);
                cardZOrder += _listHandCardsMainUser.size() - 1;
            }
            _layoutCards->addChild(tlCard);

            GinCardProperty ginCardPropertyStart;
//        ginCardPropertyStart._positionOnHand = _gameGin->getRealPositionOfDrawCardAtBase();
//        ginCardPropertyStart._scale          = GPGinInterfaceLayer::_scaleDrawCard;
//        ginCardPropertyStart._angle          = 0;

            float drawFromStock = true;
            if (this->_ginDrawAndTakeFakeCardHandIndex != -1)
            {
                ginCardPropertyStart = this->_ginDrawAndTakeCardProperty;
                ginCardPropertyStart._scale = this->_listHandCardsMainUser.back()->getScale();
                ginCardPropertyStart._positionOnHand = this->convertWorldPosToLayoutRootPos(
                        _layoutCards->convertToWorldSpace(ginCardPropertyStart._positionOnHand));
                ginCardPropertyStart._zOder = cardZOrder;

                this->_ginDrawAndTakeFakeCardHandIndex = -1;
                drawFromStock = false;
            } else
            {
                ginCardPropertyStart._positionOnHand = _gameGin->getRealPositionOfDrawCardAtBase();
                ginCardPropertyStart._scale = GPGinInterfaceLayer::_scaleDrawCard;
                ginCardPropertyStart._angle = 0;
                ginCardPropertyStart._zOder = cardZOrder;
            }

            GinCardProperty ginCardPropertyEnd;
            ginCardPropertyEnd._positionOnHand = this->convertWorldPosToLayoutRootPos(
                    _layoutCards->convertToWorldSpace(endPro._positionOnHand));
            ginCardPropertyEnd._scale = endPro._scale;
            ginCardPropertyEnd._angle = endPro._angle;

            _shouldSortCardAfterPickingCard = false;
            showHintDump(false);
            showHintKnock(false);
            _canTouchCard = false;
            unselectAllCardsOnHand(false);
            forceUndrapAllCards();
            this->runAction(Sequence::create(CallFunc::create([=]()
                                                              {
                                                                  for (auto const &tgcard: _listCardsWereAutoSelectedForHintTake)
                                                                      tgcard->unSelected(true);
                                                                  GinCard *card = GinCard::create(nullptr);
                                                                  card->setCardIndex(tlCard->getCardIndex());
                                                                  card->setCardValue(-1);
                                                                  card->open(true);
                                                                  card->setBack(1);
                                                                  card->setScale(ginCardPropertyStart._scale);
                                                                  card->setRotation(ginCardPropertyStart._angle);
                                                                  card->setPosition(
                                                                          this->convertWorldPosToLayoutRootPos(
                                                                                  ginCardPropertyStart._positionOnHand));
                                                                  card->setLocalZOrder(ginCardPropertyStart._zOder);
                                                                  _layoutRoot->addChild(card);

                                                                  Vector<FiniteTimeAction *> listAction;

                                                                  float timeCardDraw = TIME_CARD_DRAW;
                                                                  if (drawFromStock == false)
                                                                  {
                                                                      listAction.pushBack(
                                                                              MoveBy::create(0.05f, Point(0, 10)));
                                                                      listAction.pushBack(DelayTime::create(0.05f));

                                                                      timeCardDraw = 0.1f;
                                                                  }
                                                                  listAction.pushBack(DelayTime::create(0.15f));
                                                                  listAction.pushBack(Spawn::create(
                                                                          EaseCubicActionOut::create(
                                                                                  MoveTo::create(timeCardDraw,
                                                                                                 ginCardPropertyEnd._positionOnHand)),
                                                                          EaseBackOut::create(
                                                                                  ScaleTo::create(timeCardDraw,
                                                                                                  ginCardPropertyEnd._scale)),
                                                                          RotateTo::create(timeCardDraw,
                                                                                           ginCardPropertyEnd._angle),
                                                                          nullptr));
                                                                  listAction.pushBack(DelayTime::create(0.1f));
                                                                  listAction.pushBack(CallFunc::create([=]()
                                                                                                       {
                                                                                                           if (checkValidCardsOnHand(
                                                                                                                   tlCard))
                                                                                                           {
                                                                                                               tlCard->setVisible(
                                                                                                                       true);
                                                                                                               tlCard->open(
                                                                                                                       false);
                                                                                                           }
                                                                                                       }));
                                                                  listAction.pushBack(RemoveSelf::create());

                                                                  card->runAction(Sequence::create(listAction));
                                                              }), DelayTime::create(TIME_CARD_DRAW),
                                             CallFunc::create([=]()
                                                              {
                                                                  playEffectSortCardsOnHand();
                                                              }),
                                             DelayTime::create(0.3f), CallFunc::create([=]()
                                                                                       {
                                                                                           this->checkStateOfGroupAndUngroupButton();
                                                                                       }), DelayTime::create(0.3f),
                                             CallFunc::create([=]()
                                                              {
                                                                  if (checkValidCardsOnHand(tlCard))
                                                                  {
                                                                      this->softArrangeCardOnHandAfterPickCard(tlCard);
                                                                      this->sortSoftGroupContainCards(
                                                                              {tlCard->getCardIndex()});
                                                                  }
                                                                  _canTouchCard = true;
                                                              }), nullptr));
        }
        else
        {
            this->_ginDrawAndTakeFakeCard->setCardValue(index);
            this->_ginDrawAndTakeFakeCard->setCardIndex(index);
            this->_ginDrawAndTakeFakeCard->open(true);
            if (checkValidCardsOnHand(this->_ginDrawAndTakeFakeCard))
            {
                this->softArrangeCardOnHandAfterPickCard(this->_ginDrawAndTakeFakeCard);
                this->sortSoftGroupContainCards(
                        {this->_ginDrawAndTakeFakeCard->getCardIndex()});
            }
            this->resetDrawAndTakeAnimationCachedDataOnDrawCard();
            this->_ginDrawAndTakeFakeCardHandIndex = -1;
            this->enableDrawAndTakeButton(true);
        }
    } else
    {
        auto tlCard = createCard(index, GPGinInterfaceLayer::_scaleDrawCard, false);
        tlCard->setRotation(0);
        tlCard->setPosition(this->convertWorldPosToLayoutRootPos(_gameGin->getRealPositionOfDrawCardAtBase()));
        tlCard->setLocalZOrder((int) ZorderControlLayer::CARD_IN_HAND);
#if CHEAT_OPEN_CARD_ON_HAND
        tlCard->open();
#endif
        _layoutRoot->addChild(tlCard);
        _listHandCardsAllUser[orderId].push_back(tlCard);
        tlCard->stopAllActions();
        tlCard->runAction(Sequence::create(MoveBy::create(0.05f, Point(0, 10)),
                                           DelayTime::create(0.05f),
                                           Spawn::create(EaseCubicActionOut::create(MoveTo::create(TIME_CARD_DRAW,
                                                                                                   this->convertWorldPosToLayoutRootPos(
                                                                                                           _gameGin->getHandCardPos()[orderId]))),
                                                         RotateTo::create(TIME_CARD_DRAW, 0),
                                                         ScaleTo::create(TIME_CARD_DRAW,
                                                                         CARD_IN_HAND_OTHER_PLAYER_SCALE*1.5f),
                                                         nullptr),
                                           FadeOut::create(0.05f),
                                           CallFunc::create([=]()
                                                            {
                                                                this->updateNumberOfRemainCards(orderId, true);
                                                                tlCard->setScale(CARD_IN_HAND_OTHER_PLAYER_SCALE);
                                                            }),
                                           nullptr));
    }
    SoundManager::playSoundEffect(SoundEvent::TL_DRAW_CARD);
}

void GPGinControlCardLayer::takeCard(int orderId, int index)
{
    GinCard *takeCard = _listThrowCards.back();
    _listThrowCards.pop_back();
    auto tlCard = createCard(index, takeCard->getScale(), false);
    tlCard->setRotation(takeCard->getRotation());
    tlCard->setLocalZOrder(takeCard->getLocalZOrder());
    tlCard->setVisible(false);
    showHintTake(false);
    showHintDraw(false);
    if (orderId == MAIN_USER_SLOT_INDEX)
    {
//        tlCard->setPosition(_layoutCards->convertToNodeSpace(_layoutRoot->convertToWorldSpace(takeCard->getPosition())));
//        _listHandCardsMainUser.push_back(tlCard);
//        _layoutCards->addChild(tlCard);

        std::vector<GinCardProperty> listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
                _listHandCardsMainUser.size() + 1);
        int possibleCardIndex = this->getPossibleNewCardIndexBySoftGroup(index);
        int propertyCardIndex = listPropertyCard.size() - 1;

        if (this->_ginDrawAndTakeFakeCardHandIndex != -1)
        {
            propertyCardIndex = this->_ginDrawAndTakeFakeCardHandIndex;
        } else if (possibleCardIndex != -1)
        {
            propertyCardIndex = possibleCardIndex;
        }
//        int propertyCardIndex = (this->_ginDrawAndTakeFakeCardHandIndex != -1) ? this->_ginDrawAndTakeFakeCardHandIndex : (listPropertyCard.size()-1);
        if (this->_ginDrawAndTakeFakeCardHandIndex != -1)
        {
            tlCard->setPosition(this->_ginDrawAndTakeCardProperty._positionOnHand);
            tlCard->setRotation(this->_ginDrawAndTakeCardProperty._angle);
            tlCard->setLocalZOrder(
                    this->_ginDrawAndTakeFakeCardHandIndex + (int) ZorderControlLayer::CARD_ON_HAND_USER);
            tlCard->setScale(this->_listHandCardsMainUser.back()->getScale());
            _listHandCardsMainUser.insert(_listHandCardsMainUser.begin() + propertyCardIndex, tlCard);
        } else if (possibleCardIndex != -1)
        {
            tlCard->setPosition(
                    _layoutCards->convertToNodeSpace(_layoutRoot->convertToWorldSpace(takeCard->getPosition())));
            tlCard->setRotation(listPropertyCard[propertyCardIndex]._angle);
            tlCard->setLocalZOrder(
                    listPropertyCard[propertyCardIndex]._zOder + (int) ZorderControlLayer::CARD_ON_HAND_USER);
            tlCard->setScale(listPropertyCard[propertyCardIndex]._scale);
            _listHandCardsMainUser.insert(_listHandCardsMainUser.begin() + propertyCardIndex, tlCard);
        } else
        {
            tlCard->setPosition(
                    _layoutCards->convertToNodeSpace(_layoutRoot->convertToWorldSpace(takeCard->getPosition())));
            _listHandCardsMainUser.push_back(tlCard);
        }
        _layoutCards->addChild(tlCard);
    } else
    {
        tlCard->setPosition(takeCard->getPosition());
        _layoutRoot->addChild(tlCard);
        _listHandCardsAllUser[orderId].push_back(tlCard);
    }

    float takeCardOrgOpacity = takeCard->getOpacity();
    if(this->_ginDrawAndTakeFakeCardHandIndex != -1)
    {
        takeCard->setOpacity(0);
    }
//    takeCard->setOpacity(0);
    takeCard->stopAllActions();
    takeCard->runAction(Sequence::create(CallFunc::create([=]()
                                                          {
                                                              this->showCardsTake(orderId, index);
                                                          }), RemoveSelf::create(),
                                         CallFunc::create([=] { takeCard->setOpacity(takeCardOrgOpacity); }), nullptr));
}

void GPGinControlCardLayer::showCardsTake(int orderId, int takeCardValue)
{
    if (orderId == MAIN_USER_SLOT_INDEX)
    {
        _canTouchCard = false;
        unselectAllCardsOnHand(false);
        forceUndrapAllCards();

        GinCard *tlCard = getHandCardsByIndex(orderId, takeCardValue);
        std::vector<GinCardProperty> listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
                _listHandCardsMainUser.size());
        int possibleCardIndex = this->getPossibleNewCardIndexBySoftGroup(takeCardValue);
        int propertyCardIndex = listPropertyCard.size() - 1;

        if (this->_ginDrawAndTakeFakeCardHandIndex != -1)
        {
            propertyCardIndex = this->_ginDrawAndTakeFakeCardHandIndex;
        } else if (possibleCardIndex != -1)
        {
            propertyCardIndex = possibleCardIndex;
        }
//        int propertyCardIndex = (this->_ginDrawAndTakeFakeCardHandIndex != -1) ? this->_ginDrawAndTakeFakeCardHandIndex : (listPropertyCard.size()-1);
        const GinCardProperty &property = listPropertyCard[propertyCardIndex];
        _shouldSortCardAfterPickingCard = false;

        bool pickFromStock = true;
        if (this->_ginDrawAndTakeFakeCardHandIndex != -1)
        {
            this->_ginDrawAndTakeFakeCardHandIndex = -1;
            pickFromStock = false;
        }

        GinCard *card = createCard(tlCard->getCardIndex(), tlCard->getScale(), false);
//        card->setRotation(tlCard->getRotation());
        card->setRotation(0);
        card->setScale(tlCard->getScale());
        card->setPosition(
                convertWorldPosToLayoutRootPos(tlCard->getParent()->convertToWorldSpace(tlCard->getPosition())));
        card->setLocalZOrder((int) ZorderControlLayer::CARD_ON_HAND_USER);
        card->setVisible(true);
        card->open(false);
        _layoutRoot->addChild(card);

        float timeCardTake = TIME_CARD_TAKE;

        Vector<FiniteTimeAction *> listAction;

        listAction.pushBack(DelayTime::create(0.2f));
        if (pickFromStock == true)
        {
            listAction.pushBack(MoveBy::create(0.1f, Point(0, PICK_CARD_HEIGHT)));
            listAction.pushBack(DelayTime::create(0.05f));
        }
        else
        {
            timeCardTake = 0.1f;
        }
        listAction.pushBack(CallFunc::create([=]()
                                             {
                                                 playEffectSortCardsOnHand();
                                             }));
        listAction.pushBack(Spawn::create(EaseCubicActionOut::create(MoveTo::create(timeCardTake,
                                                                                    convertWorldPosToLayoutRootPos(
                                                                                            _layoutCards->convertToWorldSpace(
                                                                                                    property._positionOnHand)))),
                                          EaseBackOut::create(ScaleTo::create(timeCardTake, property._scale)),
                                          RotateTo::create(timeCardTake, property._angle),
                                          nullptr));
        listAction.pushBack(Hide::create());
        listAction.pushBack(CallFunc::create([=]()
                                             {
                                                 if (checkValidCardsOnHand(tlCard))
                                                 {
                                                     tlCard->setVisible(true);
                                                     tlCard->open(false);
                                                 }
                                             }));
        listAction.pushBack(DelayTime::create(0.1f));
        listAction.pushBack(CallFunc::create([=]()
                                             {
                                                 this->checkStateOfGroupAndUngroupButton();
                                                 if (checkValidCardsOnHand(tlCard))
                                                 {
                                                     this->softArrangeCardOnHandAfterPickCard(tlCard);
                                                     this->sortSoftGroupContainCards({tlCard->getCardIndex()});
                                                 }
                                                 _canTouchCard = true;
                                             }));
        listAction.pushBack(RemoveSelf::create());
        card->runAction(Sequence::create(listAction));

//        card->runAction(Sequence::create(DelayTime::create(0.2f),
//                                         MoveBy::create(0.1f, Point(0, PICK_CARD_HEIGHT)),
//                                         DelayTime::create(0.05f),
//                                         CallFunc::create([=]() {
//                                             playEffectSortCardsOnHand();
//                                         }),
//                                         Spawn::create(EaseCubicActionOut::create(MoveTo::create(TIME_CARD_TAKE,
//                                                                                                 convertWorldPosToLayoutRootPos(_layoutCards->convertToWorldSpace(property._positionOnHand)))),
//                                                       EaseBackOut::create(ScaleTo::create(TIME_CARD_TAKE, property._scale)),
//                                                       RotateTo::create(TIME_CARD_TAKE, property._angle),
//                                                       nullptr),
//                                         Hide::create(),
//                                         CallFunc::create([=]() {
//                                             if (checkValidCardsOnHand(tlCard))
//                                             {
//                                                 tlCard->setVisible(true);
//                                                 tlCard->open(false);
//                                             }
//                                         }),
//                                         DelayTime::create(0.1f),
//                                         CallFunc::create([=]() {
//                                             this->checkStateOfGroupAndUngroupButton();
//                                             if (checkValidCardsOnHand(tlCard))
//                                                 this->softArrangeCardOnHandAfterPickCard(tlCard);
//                                             _canTouchCard = true;
//                                         }),
//                                         RemoveSelf::create(),
//                                         nullptr));
    } else
    {
        GinCard *tlCard = getHandCardsByIndex(orderId, takeCardValue);
        GinCard *card = createCard(tlCard->getCardIndex(), tlCard->getScale(), false);
        card->setRotation(tlCard->getRotation());
        card->setScale(tlCard->getScale());
        card->setPosition(tlCard->getPosition());
        card->setLocalZOrder((int) ZorderControlLayer::CARD_IN_HAND);
        card->setVisible(true);
        card->open(false);
        _layoutRoot->addChild(card);

        Vec2 dstPos = _layoutRoot->convertToNodeSpace(_gameGin->getInterfaceLayer()->getHandCardGlobalPos()[orderId]);

        card->runAction(Sequence::create(DelayTime::create(0.2f),
                                         MoveBy::create(0.1f, Point(0, PICK_CARD_HEIGHT)),
                                         DelayTime::create(0.05f),
//                                         Spawn::create(EaseCubicActionOut::create(MoveTo::create(TIME_CARD_TAKE, _gameGin->getHandCardPos()[orderId])),
//                                                       RotateTo::create(TIME_CARD_TAKE, 0),
//                                                       ScaleTo::create(TIME_CARD_TAKE, CARD_IN_HAND_OTHER_PLAYER_SCALE),
//                                                       nullptr),
                                         Spawn::create(
                                                 EaseCubicActionOut::create(MoveTo::create(TIME_CARD_TAKE, dstPos)),
                                                 RotateTo::create(TIME_CARD_TAKE, 0),
                                                 ScaleTo::create(TIME_CARD_TAKE, CARD_IN_HAND_OTHER_PLAYER_SCALE*1.5f),
                                                 nullptr),
                                         FadeOut::create(0.05f),
                                         CallFunc::create([=]()
                                                          {
                                                              this->updateNumberOfRemainCards(orderId, true);
                                                              card->setScale(CARD_IN_HAND_OTHER_PLAYER_SCALE);
                                                          }),
                                         RemoveSelf::create(),
                                         nullptr));
    }
    SoundManager::playSoundEffect(SoundEvent::TL_TAKE_CARD);
}

void GPGinControlCardLayer::giveCard(int orderIdx, const PointGiveServerDefine &pointGiveCard, int step)
{
    int toOrderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(pointGiveCard.slot);
    int meldIdx = pointGiveCard.phomOrder;
    this->runAction(Sequence::create(DelayTime::create(step * 1.8f), CallFunc::create([=]()
                                                                                      {
                                                                                          std::vector<GinCard *> listCards;
                                                                                          for (const auto &cardGive: pointGiveCard.card)
                                                                                          {
                                                                                              for (const auto &card: _listShowCards[orderIdx])
                                                                                              {
                                                                                                  if (card->getCardValue() ==
                                                                                                      cardGive)
                                                                                                  {
                                                                                                      listCards.push_back(
                                                                                                              card);
                                                                                                      break;
                                                                                                  }
                                                                                              }
                                                                                          }

                                                                                          for (const auto &card: listCards)
                                                                                          {
                                                                                              // Remove card from showing cards
                                                                                              this->removeCardFromShowingCards(
                                                                                                      orderIdx, card);
                                                                                              this->removeCardFromRemainCards(
                                                                                                      orderIdx, card);
                                                                                              // Add card to showing cards of another user
                                                                                              _listMeldsAllUser[toOrderIdx][meldIdx].push_back(
                                                                                                      card);
                                                                                              sortIncrease(
                                                                                                      _listMeldsAllUser[toOrderIdx][meldIdx]);
                                                                                              this->insertCardToShowingCards(
                                                                                                      toOrderIdx, card);
                                                                                          }
                                                                                      }), DelayTime::create(0.1f),
                                     CallFunc::create([=]()
                                                      {
                                                          std::vector<GinCardProperty> listShowingCardProperty = this->calculatePositionShowingCards(
                                                                  toOrderIdx);
                                                          for (const auto &cardGive: pointGiveCard.card)
                                                              for (int j = 0;
                                                                   j < _listShowCards[toOrderIdx].size(); j++)
                                                              {
                                                                  GinCard *card = _listShowCards[toOrderIdx][j];
                                                                  if (cardGive == card->getCardValue())
                                                                  {
                                                                      const GinCardProperty &cardProperty = listShowingCardProperty[j];
                                                                      card->setHighLight(true);
                                                                      card->setLocalZOrder(
                                                                              (int) ZorderControlLayer::CARD_IN_HAND +
                                                                              100 + cardProperty._zOder);
                                                                      card->runAction(Sequence::create(Spawn::create(
                                                                              EaseCubicActionOut::create(
                                                                                      MoveTo::create(TIME_CARD_GIVE,
                                                                                                     cardProperty._positionOnHand)),
                                                                              ScaleTo::create(TIME_CARD_GIVE,
                                                                                              cardProperty._scale),
                                                                              RotateTo::create(TIME_CARD_GIVE,
                                                                                               cardProperty._angle),
                                                                              nullptr), CallFunc::create([=]()
                                                                                                         {
                                                                                                             card->setLocalZOrder(
                                                                                                                     (int) ZorderControlLayer::CARD_IN_HAND +
                                                                                                                     cardProperty._zOder);

                                                                                                             SoundManager::playSoundEffect(
                                                                                                                     SoundEvent::TL_GIVE_CARD);
                                                                                                         }), nullptr));
                                                                  }
                                                              }
                                                      }), DelayTime::create(0.2f),
            // Update score
                                     CallFunc::create([=]()
                                                      {
                                                          // Update score of user who gived cards
                                                          int score = _gameGin->getScoreBoxValue(orderIdx) -
                                                                      this->calculateScoreCards(pointGiveCard.card);
                                                          int value =
                                                                  score <= _gameGin->getScoreBoxValue(toOrderIdx) ? 1
                                                                                                                  : -1;
//                                         _gameGin->updateScoreBox(value, score, orderIdx, calculateGlobalPositionScoreBox(orderIdx), GinResultType::GIN_RESULT_TYPE_SCORE);
//                                         // Update score of user show received cards
//                                         _gameGin->updateScoreBox(value * -1,
//                                                                  _gameGin->getScoreBoxValue(toOrderIdx),
//                                                                  toOrderIdx,
//                                                                  calculateGlobalPositionScoreBox(toOrderIdx),
//                                                                  GinResultType::GIN_RESULT_TYPE_SCORE);
                                                          _gameGin->updateScoreBox(0, score, orderIdx,
                                                                                   calculateGlobalPositionScoreBox(
                                                                                           orderIdx),
                                                                                   GinResultType::GIN_RESULT_TYPE_SCORE);
                                                          // Update score of user show received cards
                                                          _gameGin->updateScoreBox(0,
                                                                                   _gameGin->getScoreBoxValue(
                                                                                           toOrderIdx),
                                                                                   toOrderIdx,
                                                                                   calculateGlobalPositionScoreBox(
                                                                                           toOrderIdx),
                                                                                   GinResultType::GIN_RESULT_TYPE_SCORE);
                                                      }), DelayTime::create(0.6f),
            // Animation sort showing cards
                                     CallFunc::create([=]()
                                                      {
                                                          this->arrageShowingCards(orderIdx, {});
                                                          this->arrageShowingCards(toOrderIdx, pointGiveCard.card);
                                                      }), nullptr));
}

void GPGinControlCardLayer::matchEndEachUser(PlayerResultData res, bool win, GinResultType typeWin,
                                             MatchEndEachUserFinishCallback callback)
{
    if (res._playerIndex == -1)
    {
        return;
    }

    int orderId = GPGinSlotManager::getInstance()->getSlotOrderIdx(res._playerIndex);
    GinResultType type = (GinResultType) res._type;
    bool trackGin = false;
    if (win)
    {
        if (type == GinResultType::GIN_RESULT_TYPE_WIN_GIN || type == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
            trackGin = true;
    }
    // Show melds and remain card
    if (orderId == MAIN_USER_SLOT_INDEX)
    {
        int countCard = 0;
        // Get all card on hands include both meld and trash cards
        std::vector<std::vector<int>> listCardsShowOnTable;
        // -- Melds
        for (const auto &phom: res._listPhom)
        {
            listCardsShowOnTable.push_back(phom);
            _listMeldsAllUser[orderId].push_back(std::vector<GinCard *>());
            countCard += phom.size();
        }
        // -- Trash cards
        if (res._remainCards.size() > 0)
        {
            sortIncrease(res._remainCards);
            listCardsShowOnTable.push_back(res._remainCards);
            countCard += res._remainCards.size();
        }

        float width = countCard > 0 ? GIN_WIDTH_CARD * CARD_ON_TABLE_SCALE_1 +
                                      (countCard - 1 - (listCardsShowOnTable.size() - 1)) *
                                      DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 +
                                      (listCardsShowOnTable.size() - 1) * WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1 : 0;
        Vec2 posShowCard = this->getPosShowCard(orderId, width);
        // Show card on hand
        countCard = 0;
        int idxMeld = 0;
        int animCount = 0;
        for (int i = 0; i < listCardsShowOnTable.size(); i++)
        {
            const auto &cardsI = listCardsShowOnTable.at(i);
            bool isMeld = this->MeldIsValid(cardsI);
//            for (const auto& cardj : cardsI)
            for (int j = 0; j < cardsI.size(); ++j)
            {
                const auto &cardj = cardsI[j];
                Vec2 position = Vec2(0, 0);
                float rotation = 0;
                float scale = 1.0f;
                for (int k = 0; k < _listHandCardsMainUser.size(); k++)
                {
                    if (_listHandCardsMainUser.at(k)->getCardValue() == cardj)
                    {
                        position = this->convertWorldPosToLayoutRootPos(
                                _layoutCards->convertToWorldSpace(_listHandCardsMainUser.at(k)->getPosition()));
                        rotation = _listHandCardsMainUser.at(k)->getRotation();
                        scale = _listHandCardsMainUser.at(k)->getScale();
                        _layoutCards->removeChild(_listHandCardsMainUser.at(k), true);
                        _listHandCardsMainUser.erase(_listHandCardsMainUser.begin() + k);
                        break;
                    }
                }
                GinCard *tlCard = createCard(cardj, scale, true);
                tlCard->setPosition(position);
                tlCard->setRotation(rotation);
                tlCard->setHighLight(isMeld);
                if (isMeld)
                    _listMeldsAllUser[orderId][idxMeld].push_back(tlCard);
                else
                    _listRemainCards.at(orderId).push_back(tlCard);
                _listShowCards[orderId].push_back(tlCard);
                _layoutRoot->addChild(tlCard, (int) ZorderControlLayer::CARD_IN_HAND + countCard);

                CallFunc *finishCallback = nullptr;
                if ((i == listCardsShowOnTable.size() - 1) &&
                    (j == cardsI.size() - 1))
                {
                    finishCallback = CallFunc::create([=]
                                                      {
                                                          // Show score
//                                                          if (!trackGin)
                                                          {
                                                              Vec2 posScoreBox = this->calculateGlobalPositionScoreBox(
                                                                      orderId);
                                                              _currentScore = this->calculateScoreCards(
                                                                      _listRemainCards.at(orderId));
//                                _gameGin->setVisibleScoreBox(true, win ? 1 : -1, _currentScore, res._playerIndex, posScoreBox, type);
                                                              _gameGin->setVisibleScoreBox(true, 0, _currentScore,
                                                                                           res._playerIndex,
                                                                                           posScoreBox, type);
                                                          }

                                                          if (callback != nullptr)
                                                          {
                                                              callback();
                                                          }
                                                      });
                }

                // -- Action  move, rotate and scale
                Vec2 posAdd = Vec2(-width * 0.5f + 0.5 * GIN_WIDTH_CARD * CARD_ON_TABLE_SCALE_1 +
                                   (countCard - i) * DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 +
                                   i * WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1,
                                   0);
                Vec2 posTarget = posShowCard + posAdd;
                tlCard->runAction(Sequence::create(DelayTime::create(DELAY_SHOW_CARD_ON_TABLE * animCount),
                                                   Spawn::create(EaseCubicActionOut::create(
                                                                         MoveTo::create(TIME_CARD_THROW_END_MATCH_USER, posTarget)),
                                                                 ScaleTo::create(TIME_CARD_THROW_END_MATCH_USER,
                                                                                 CARD_ON_TABLE_SCALE_1),
                                                                 RotateTo::create(TIME_CARD_THROW_END_MATCH_USER, 0),
                                                                 nullptr),
                                                   finishCallback,
                                                   nullptr));

                countCard++;
                if (isMeld == false)
                {
                    animCount++;
                }
            }
            if (isMeld)
                idxMeld++;
            animCount++;
        }

        clearGroup(true);
        clearSoftGroup();

//        // Show score
//        if (!trackGin)
//        {
//            Vec2 posScoreBox = this->calculateGlobalPositionScoreBox(orderId);
//            _currentScore = this->calculateScoreCards(_listRemainCards.at(orderId));
//            _gameGin->setVisibleScoreBox(true, win ? 1 : -1, _currentScore, res._playerIndex, posScoreBox, type);
//        }
    } else
    {
        int countCard = 0;
        // Get all card on hands include both melds and trash cards
        std::vector<std::vector<int>> listCardsShowOnTable;
        // -- Melds
        for (auto &phom: res._listPhom)
        {
            sortIncrease(phom);
            listCardsShowOnTable.push_back(phom);
            _listMeldsAllUser[orderId].push_back(std::vector<GinCard *>());
            countCard += phom.size();
        }
        // -- Trash card
        if (res._remainCards.size() > 0)
        {
            sortIncrease(res._remainCards);
            listCardsShowOnTable.push_back(res._remainCards);
            countCard += res._remainCards.size();
        }

        Vec2 posStart = _layoutRoot->convertToNodeSpace(_gameGin->getInterfaceLayer()->getHandCardGlobalPos()[orderId]);

        float width = countCard > 0 ? GIN_WIDTH_CARD * CARD_ON_TABLE_SCALE_1 +
                                      (countCard - 1 - (listCardsShowOnTable.size() - 1)) *
                                      DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 +
                                      (listCardsShowOnTable.size() - 1) * WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1 : 0;
        Vec2 posShowCard = this->getPosShowCard(orderId, width);
        // Show card on hand
        countCard = 0;
        int animCount = 0;
        Vec2 posTarget = posShowCard;
        int zOrder = (int) ZorderControlLayer::CARD_IN_HAND;
        int idxMeld = 0;
        for (int i = 0; i < listCardsShowOnTable.size(); i++)
        {
            const auto &cardsI = listCardsShowOnTable.at(i);
            bool isMeld = this->MeldIsValid(cardsI);
//            for (const auto& cardj : cardsI)
            for (int j = 0; j < cardsI.size(); ++j)
            {
                const auto &cardj = cardsI[j];
                posTarget = posShowCard +
                            Vec2(-width * 0.5f + 0.5 * GIN_WIDTH_CARD * CARD_ON_TABLE_SCALE_1 +
                                 (countCard - i) * DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 +
                                 i * WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1, 0);
                zOrder++;

                // Open card on hand
                if (orderId < _listHandCardsAllUser.size() && countCard < _listHandCardsAllUser[orderId].size())
                {
                    CallFunc *finishCallback = nullptr;
                    if ((i == listCardsShowOnTable.size() - 1) &&
                        (j == cardsI.size() - 1))
                    {
                        finishCallback = CallFunc::create([=]
                                                          {
                                                              // Show score
//                                                              if (!trackGin)
//                                        _gameGin->setVisibleScoreBox(true, win ? 1 : -1, this->calculateScoreCards(_listRemainCards.at(orderId)), res._playerIndex, this->calculateGlobalPositionScoreBox(orderId), type);
                                                                  _gameGin->setVisibleScoreBox(true, 0,
                                                                                               this->calculateScoreCards(
                                                                                                       _listRemainCards.at(
                                                                                                               orderId)),
                                                                                               res._playerIndex,
                                                                                               this->calculateGlobalPositionScoreBox(
                                                                                                       orderId), type);
                                                              if (callback != nullptr)
                                                              {
                                                                  callback();
                                                              }
                                                          });
                    }

                    GinCard *card = _listHandCardsAllUser[orderId][countCard];
                    card->setScale(CARD_ON_TABLE_SCALE_1);
                    card->setCardIndex(cardj, false);
                    card->setCardValue(cardj);
//                    card->setPosition(posTarget);
                    card->setPosition(posStart);
                    card->setLocalZOrder(zOrder);
                    card->open();
                    card->setHighLight(isMeld);
                    if (isMeld)
                        _listMeldsAllUser[orderId][idxMeld].push_back(card);
                    else
                        _listRemainCards[orderId].push_back(card);
                    _listShowCards[orderId].push_back(card);
                    card->runAction(Sequence::create(DelayTime::create(DELAY_SHOW_CARD_ON_TABLE * animCount),
                                                     Spawn::create(FadeIn::create(TIME_CARD_THROW_END_MATCH_BOT),
                                                                   MoveTo::create(TIME_CARD_THROW_END_MATCH_BOT, posTarget),
                                                                   EaseBackOut::create(ScaleTo::create(TIME_CARD_THROW_END_MATCH_BOT,
                                                                                                       CARD_ON_TABLE_SCALE_1)),
                                                                   nullptr), finishCallback, nullptr));
                }
                countCard++;
                if (isMeld == false)
                {
                    animCount++;
                }
            }
            if (isMeld)
                idxMeld++;
            animCount++;
        }
        // Show score box
//        if (!trackGin)
//            _gameGin->setVisibleScoreBox(true, win ? 1 : -1, this->calculateScoreCards(_listRemainCards.at(orderId)), res._playerIndex, this->calculateGlobalPositionScoreBox(orderId), type);
    }
}

void GPGinControlCardLayer::preMatchEnd()
{
    this->removeAllEffectHighlight();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listUserHandNode.at(i)->clearHand();
    this->setVisibleButtonXep(false);
    showHintDraw(false);
    showHintTake(false);
    showUngroupButton(false, std::vector<GinCard *>());
    showGroupButton(false, std::vector<GinCard *>());
    this->setThrowCardsVisible(false);
    this->hideAllParticle();
    this->visibleTurnLayout(false);
}

void GPGinControlCardLayer::matchEnd(const PlayerResultData &win, const std::vector<PlayerResultData> &listLose,
                                     MatchEndFinishCallback callback)
{
    _status = ControlStatus::CONTROL_STATUS_MATCH_END;
    GinResultType typeWin = (GinResultType) win._type;

    int maxPlayerCount = listLose.size() + 1;
    int counter = 0;
    MatchEndEachUserFinishCallback finishCallback = [=, &counter]
    {
        counter++;
        if (counter == maxPlayerCount)
        {
            if (callback != nullptr)
            {
                callback();
            }
        }
    };

    this->matchEndEachUser(win, true, typeWin, finishCallback);
    for (const auto &data: listLose)
        this->matchEndEachUser(data, false, typeWin, finishCallback);
    showStepHint(false);
}

void GPGinControlCardLayer::onButtonTouch(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            if (pSender == _buttonPass && _status == ControlStatus::CONTROL_STATUS_PASSING)
            {
                this->hintPassCard(false, false, false);
                MessageSender::requestPass(MAIN_USER_SLOT_INDEX, -1);
            } else if (pSender == _buttonKnock)
            {
                this->onRequestDump(true, ActionType::K_KNOCK);
            } else if (pSender == _buttonGin)
            {
                this->onRequestDump(true, ActionType::K_GIN);
            } else if (pSender == _buttonBigGin)
            {
                this->onRequestDump(true, ActionType::K_BIG_GIN);
            } else if (pSender == _buttonDraw)
            {
                if (_status == ControlStatus::CONTROL_STATUS_GET)
                {
                    this->hintGetCard(false, 0, 0, false, false);
                    MessageSender::requestGetCard();
                    this->resetDrawAndTakeAnimationCachedData();
                }
            } else if (pSender == _buttonDump)
            {
                this->clearAllEffectHintDropArea();
                this->onRequestDump(true, ActionType::K_NONE);
            } else if (pSender == _buttonSort)
            {
                this->checkUndoActionHintTakeCard();
                _currentSortMethodIndex = (_currentSortMethodIndex + 1) % _listSortMethod.size();
                this->onRequestSortCard(_listSortMethod[_currentSortMethodIndex].first);
            } else if (pSender == _buttonTake)
            {
                if (_status == ControlStatus::CONTROL_STATUS_PASSING)
                {
                    this->hintPassCard(false, false, false);
                    MessageSender::requestPass(MAIN_USER_SLOT_INDEX, _gameGin->getUpCardValue());
                    this->resetDrawAndTakeAnimationCachedData();
                } else if (_status == ControlStatus::CONTROL_STATUS_GET)
                {
                    this->hintGetCard(false, MAIN_USER_SLOT_INDEX, -1, false, false);
                    int previousSlotIdx = GPGinSlotManager::getInstance()->getPreviousSlotIdx(
                            GPGinSlotManager::getInstance()->getSlotIdx(_gameGin->getTurnActionOrderIdx()));
                    MessageSender::requestGetCard(previousSlotIdx);
                    this->resetDrawAndTakeAnimationCachedData();
                }
            } else if (pSender == _buttonGroup)
            {
                std::vector<GinCard *> listSelectedCard = this->getListSelectedCardOnHand();
                for (auto const &card: listSelectedCard)
                {
                    int groupIndex = this->checkCardInAGroup(card->getCardIndex());
                    if (groupIndex >= 0)
                        return;
                }
                GroupCard groupCard = this->addGroup(listSelectedCard, true);
                for (auto const &card: listSelectedCard)
                    card->unSelected(true);
                this->runAction(Sequence::create(CallFunc::create([=]()
                                                                  {
                                                                      this->checkStateOfGroupAndUngroupButton();
                                                                  }), DelayTime::create(0.1f), CallFunc::create([=]()
                                                                                                                {
                                                                                                                    this->arrangeCardOnHand(
                                                                                                                            TypeArrangeCardOnHand::TYPE_ARRANGE_GROUP_CARD);
                                                                                                                    this->animationShowGroup(
                                                                                                                            groupCard);
                                                                                                                }),
                                                 nullptr));
            } else if (pSender == _buttonUngroup)
            {
                std::vector<GinCard *> listSelectedCard = this->getListSelectedCardOnHand();
                int groupIndex = this->checkListCardInAGroup(listSelectedCard);
                if (groupIndex >= 0 && groupIndex < _listGroupHandCardsMainUser.size())
                {
                    this->removeGroup(groupIndex);
                    for (auto const &card: listSelectedCard)
                        card->unSelected(true);
                    this->runAction(Sequence::create(CallFunc::create([=]()
                                                                      {
                                                                          this->checkStateOfGroupAndUngroupButton();
                                                                      }), DelayTime::create(0.1f),
                                                     CallFunc::create([=]()
                                                                      {
                                                                          this->arrangeCardOnHand(
                                                                                  TypeArrangeCardOnHand::TYPE_ARRANGE_GROUP_CARD);
                                                                      }), nullptr));
                }
            } else if (pSender == _buttonShowThrowCards)
            {
                ThrowCardDetailPopup *throwCardDetailPopup = BoardManager::createThrowCardDetailPopup();
                throwCardDetailPopup->loadData(getListThrowCardsValue());
                throwCardDetailPopup->attach(getParent(), kAboveForeground);
            } else if (pSender == _buttonFindMeld)
            {
                _shouldSortCardAfterPickingCard = false;
                this->onRequestSortCard(TypeArrangeCardOnHand::TYPE_ARRANGE_MELD_CARD);
                // Firebase::logClickSortCard("AutoMeld");
            }
        }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
        default:
            break;
    }
}

void GPGinControlCardLayer::onRequestDump(bool byClickBtn, ActionType actionType, GinCard *card /*= nullptr*/,
                                          cocos2d::Touch *ptouch /*= nullptr*/, cocos2d::Event *pEvent /*= nullptr*/)
{
    bool requestSuccess = false;
    auto callbackEnd = [=](bool success)
    {
        if (!byClickBtn)
        {
            if (success)
            {
                _ginCardDrapCached = card;
                _ptouchDrapCached = ptouch;
                _pEventDrapCached = pEvent;
            } else
            {
                card->onTouchEnded(ptouch, pEvent);
            }
        }
    };
    if (_status == ControlStatus::CONTROL_STATUS_DUMP)
    {
        if (actionType == ActionType::K_BIG_GIN || actionType == ActionType::K_GIN || actionType == ActionType::K_KNOCK)
        {
            requestSuccess = true;
            this->hintDumpCard(false, ActionType::K_NONE);
            MessageSender::requestThrowCard(-1, actionType, getListMeldOnHandMainUserBySoftGroup());
        } else
        {
            // Get list slected card
            std::vector<GinCard *> listSelectedCard;
            if (card == nullptr)
            {
                listSelectedCard = this->getListSelectedCardOnHand();
            } else
            {
                listSelectedCard = card->getListMovingCards();
            }
            auto callbackDumpCard = [=]()
            {
                this->hintDumpCard(false, ActionType::K_NONE);
                MessageSender::requestThrowCard(listSelectedCard[0]->getCardValue(), actionType,
                                                getListMeldOnHandMainUserBySoftGroup());
            };

            // Check condition for fighting card
            if (listSelectedCard.size() == 0)
            {
                showNotifyWrong(GET_TEXT_BY_ID(TXT_NO_CARD_SELECTED));
            } else if (listSelectedCard.size() == 1)
            {
                requestSuccess = true;
                callbackDumpCard();
            } else if (listSelectedCard.size() > 1)
            {
                showNotifyWrong(GET_TEXT_BY_ID(TXT_MORE_THAN_ONE_CARD_SELECTED));
            }
        }
    }

    callbackEnd(requestSuccess);
}

void GPGinControlCardLayer::onRequestSortCard(TypeArrangeCardOnHand type)
{
    bool checkHaveDragedCard = false;
    for (auto const &card: _listHandCardsMainUser)
    {
        if (card->getState() == GinCardState::GinCARDSTATE_DRAG)
        {
            checkHaveDragedCard = true;
            break;
        }
    }
    if (!checkHaveDragedCard)
    {
        float unselectedTime = this->unselectAllCardsOnHand(false);
        if (unselectedTime > 0)
        {
            this->showGroupButton(false, std::vector<GinCard *>());
            this->showUngroupButton(false, std::vector<GinCard *>());
            this->checkStateDumpCardButton();
        }
        this->runAction(Sequence::create(DelayTime::create(unselectedTime), CallFunc::create([=]()
                                                                                             {
                                                                                                 this->arrangeCardOnHand(
                                                                                                         type);
                                                                                             }), nullptr));
    }
}

void GPGinControlCardLayer::hintPassCard(bool value, bool hintTake, bool canTake)
{
    if (value)
    {
        _status = ControlStatus::CONTROL_STATUS_PASSING;
        _buttonDraw->setVisible(false);
        _buttonPass->setVisible(true);
        _buttonTake->setVisible(canTake);
        if (hintTake)
        {
            if (_listThrowCards.size() > 0)
            {
                const auto &cardEnd = _listThrowCards.at(_listThrowCards.size() - 1);
                cardEnd->setHide(false);
                showEffectHighlight((const std::string) ("TakeCard"),
                                    cardEnd->getParent(),
                                    (cardEnd->getContentSize() * cardEnd->getScale()) + Size(5, 5),
                                    cardEnd->getPosition() + Vec2(1, 0),
                                    cardEnd->getLocalZOrder() - 1);
            }
        }
        showHintTake(canTake);
        showHintPass(true);
    } else
    {
        _status = ControlStatus::CONTROL_STATUS_NORMAL;
        _buttonPass->setVisible(false);
        _buttonTake->setVisible(false);
        removeEffectHighlight("TakeCard");
        showHintPass(false);
        showHintTake(false);
    }
}

void GPGinControlCardLayer::hintGetCard(bool value, int fromOrderId, int previousCards, bool hintTake, bool canTake)
{
    if (value)
    {
        _status = ControlStatus::CONTROL_STATUS_GET;
        _listSuggestTakeCardsCached.clear();
        _listCardsWereAutoSelectedForHintTake.clear();
        _listCardsWereAutoUngroupForHintTake.clear();
        _buttonDraw->setVisible(true);

        if (previousCards >= 0)
        {
            _buttonTake->setVisible(true);
            showHintTake(canTake);
        } else
        {
            if (_listThrowCards.size() > 0)
                _listThrowCards.at(_listThrowCards.size() - 1)->setHide(true);
        }
        if (hintTake)
        {
            if (_listThrowCards.size() > 0)
            {
                const auto &cardEnd = _listThrowCards.at(_listThrowCards.size() - 1);
                cardEnd->setHide(false);
                showEffectHighlight("TakeCard",
                                    cardEnd->getParent(),
                                    (cardEnd->getContentSize() * cardEnd->getScale()) + Size(5, 5),
                                    cardEnd->getPosition() + Vec2(1, 0),
                                    cardEnd->getLocalZOrder() - 1);
            }
        }
        showHintDraw(true);
    } else
    {
        _status = ControlStatus::CONTROL_STATUS_NORMAL;
        _buttonDraw->setVisible(false);
        _buttonTake->setVisible(false);
        showHintDraw(false);
        showHintTake(false);
        removeEffectHighlight("DrawCard");
        removeEffectHighlight("TakeCard");
    }
}

void GPGinControlCardLayer::hintDumpCard(bool value, ActionType type)
{
    if (value)
    {
        _status = ControlStatus::CONTROL_STATUS_DUMP;
        this->checkStepHint();
        this->checkStateDumpCardButton();
        this->updateThrowActionUI(type);
    } else
    {
        _status = ControlStatus::CONTROL_STATUS_NORMAL;
        _buttonDump->setVisible(false);
        _buttonKnock->setVisible(false);
        _buttonGin->setVisible(false);
        _buttonBigGin->setVisible(false);
        removeEffectHighlight("DumpCard");
    }
}

void GPGinControlCardLayer::arrangeCardOnHandBySortingGroupAndTrashCard()
{
    for (int i = 0; i < (int) (_listGroupHandCardsMainUser.size() - 1); i++)
    {
        if (_listGroupHandCardsMainUser.at(i)._type == MeldType::MELD_NONE)
        {
            for (int j = i + 1; j < _listGroupHandCardsMainUser.size(); j++)
            {
                if (_listGroupHandCardsMainUser.at(j)._type != MeldType::MELD_NONE)
                {
                    GroupCard groupCard = _listGroupHandCardsMainUser.at(j);
                    _listGroupHandCardsMainUser.erase(_listGroupHandCardsMainUser.begin() + j);
                    _listGroupHandCardsMainUser.insert(_listGroupHandCardsMainUser.begin() + i, groupCard);
                    break;
                }
            }
        }
    }
    this->arrangeCardOnHandBySortingGroupAtTheBeginning();
    std::vector<GinCard *> listTrash = this->getTrashCards();
    for (GinCard *card: listTrash)
    {
        auto position = std::find(_listHandCardsMainUser.begin(), _listHandCardsMainUser.end(), card);
        if (position != _listHandCardsMainUser.end())
            _listHandCardsMainUser.erase(position);
    }
    this->arrangeCardOnHandOnTrashCard(listTrash);
}

void GPGinControlCardLayer::arrangeCardOnHandOnTrashCard(std::vector<GinCard *> &listTrashCard)
{
    if (_listSortMethod[_currentSortMethodIndex].first == TypeArrangeCardOnHand::TYPE_ARRANGE_INCREASE)
    {
        sortIncrease(listTrashCard);
        for (auto const &cards: listTrashCard)
            _listHandCardsMainUser.push_back(cards);
    } else if (_listSortMethod[_currentSortMethodIndex].first == TypeArrangeCardOnHand::TYPE_ARRANGE_SUIT)
    {
        sortSuit(listTrashCard);
        for (auto const &cards: listTrashCard)
            _listHandCardsMainUser.push_back(cards);
    } else
    {
        if (listTrashCard.size() >= 3)
        {
            CardInHand cih = getPhom(listTrashCard);
            for (auto const &phom: cih._phom)
                for (const int &card: phom)
                    for (auto const &tlCard: listTrashCard)
                        if (tlCard->getCardIndex() == card)
                        {
                            _listHandCardsMainUser.push_back(tlCard);
                            break;
                        }

            vector<vector<int>> ca = getCa(cih._trash);
            for (auto &cards: ca)
            {
                sortIncrease(cards);
                for (const int &card: cards)
                    for (auto const &tlCard: listTrashCard)
                        if (tlCard->getCardIndex() == card)
                        {
                            _listHandCardsMainUser.push_back(tlCard);
                            break;
                        }
            }
        } else
        {
            sortIncrease(listTrashCard);
            for (auto const &cards: listTrashCard)
                _listHandCardsMainUser.push_back(cards);
        }
    }

}

void GPGinControlCardLayer::arrangeCardOnHandPhom()
{
    if (_listHandCardsMainUser.size() <= 3)
    {
        this->clearGroup(true);
        sortIncrease(_listHandCardsMainUser);
        if (this->MeldIsValid(_listHandCardsMainUser))
            this->addGroup(_listHandCardsMainUser, false);
    } else
    {
        std::vector<GinCard *> temp = _listHandCardsMainUser;
        CardInHand cih = getPhom(_listHandCardsMainUser);
        _listHandCardsMainUser.clear();
        this->clearGroup(true);
        for (int i = 0; i < (int) (cih._phom.size() - 1); i++)
        {
            for (int j = i + 1; j < cih._phom.size(); j++)
            {
                if (cih._phom[i].size() < cih._phom[j].size())
                {
                    std::vector<int> phom = cih._phom[i];
                    cih._phom[i] = cih._phom[j];
                    cih._phom[j] = phom;
                }
            }
        }
        for (auto phom: cih._phom)
        {
            sortIncrease(phom);
            std::vector<GinCard *> groupCard;
            for (auto card: phom)
            {
                for (auto tlCard: temp)
                {
                    if (tlCard->getCardIndex() == card)
                    {
                        _listHandCardsMainUser.push_back(tlCard);
                        groupCard.push_back(tlCard);
                        break;
                    }
                }
            }
            this->addGroup(groupCard, false);
        }
        std::vector<GinCard *> listTrashCard;
        for (int trashCard: cih._trash)
        {
            for (auto card: temp)
            {
                if (card->getCardIndex() == trashCard)
                {
                    listTrashCard.push_back(card);
                    break;
                }
            }
        }
        this->arrangeCardOnHandOnTrashCard(listTrashCard);
    }
}

void GPGinControlCardLayer::softArrangeCardOnHandPhom()
{
    if (_listHandCardsMainUser.size() <= 3)
    {
        sortIncrease(_listHandCardsMainUser);
    } else
    {
        std::vector<GinCard *> temp = _listHandCardsMainUser;
        CardInHand cih = getPhom(_listHandCardsMainUser);
        _listHandCardsMainUser.clear();
        for (int i = 0; i < (int) (cih._phom.size() - 1); i++)
        {
            for (int j = i + 1; j < cih._phom.size(); j++)
            {
                if (cih._phom[i].size() < cih._phom[j].size())
                {
                    std::vector<int> phom = cih._phom[i];
                    cih._phom[i] = cih._phom[j];
                    cih._phom[j] = phom;
                }
            }
        }
        for (auto phom: cih._phom)
        {
            sortIncrease(phom);
            for (auto card: phom)
            {
                for (auto tlCard: temp)
                {
                    if (tlCard->getCardIndex() == card)
                    {
                        _listHandCardsMainUser.push_back(tlCard);
                        break;
                    }
                }
            }
        }
        std::vector<GinCard *> listTrashCard;
        for (int trashCard: cih._trash)
        {
            for (auto card: temp)
            {
                if (card->getCardIndex() == trashCard)
                {
                    listTrashCard.push_back(card);
                    break;
                }
            }
        }
        this->arrangeCardOnHandOnTrashCard(listTrashCard);
    }
    bool newMelds = this->updateListSoftGroupHandCardMainUser();
    this->calculateScoreCardOnHandBySoftGroup();

    MessageSender::requestCustomMelds(MAIN_USER_SLOT_INDEX, getListMeldOnHandMainUserBySoftGroup());
}

void GPGinControlCardLayer::arrangeCardOnHandBySortingGroupAtTheBeginning()
{
    // Remove card which's in group on hand
    for (int i = 0; i < _listGroupHandCardsMainUser.size(); i++)
    {
        for (int j = 0; j < _listGroupHandCardsMainUser.at(i)._listCards.size(); j++)
        {
            std::vector<GinCard *>::iterator pos = std::find(_listHandCardsMainUser.begin(),
                                                             _listHandCardsMainUser.end(),
                                                             _listGroupHandCardsMainUser[i]._listCards[j]);
            if (pos != _listHandCardsMainUser.end())
                _listHandCardsMainUser.erase(pos);
        }
    }
    // Insert all cards in group at beggining of list hand card
    int count = 0;
    for (int i = 0; i < _listGroupHandCardsMainUser.size(); i++)
    {
        for (int j = 0; j < _listGroupHandCardsMainUser.at(i)._listCards.size(); j++)
        {
            _listHandCardsMainUser.insert(_listHandCardsMainUser.begin() + count,
                                          _listGroupHandCardsMainUser[i]._listCards[j]);
            count++;
        }
    }
}

void GPGinControlCardLayer::arrangeCardOnHandAfterPickCard(GinCard *card)
{
    if (_shouldSortCardAfterPickingCard)
    {
        unselectAllCardsOnHand(false);
        forceUndrapAllCards();
        _shouldSortCardAfterPickingCard = false;
        GroupCard addedGroupCard;
        for (auto &groupCard: _listGroupHandCardsMainUser)
        {
            std::vector<GinCard *> listCards = groupCard._listCards;
            listCards.push_back(card);
            if (this->MeldIsValid(listCards))
            {
                groupCard._listCards.push_back(card);
                std::vector<GinCard *> listTrashCard = this->getListTrashCardsOnHand();
                for (auto trashCard: listTrashCard)
                {
                    if (trashCard != card)
                    {
                        listCards.push_back(trashCard);
                        if (this->MeldIsValid(listCards))
                            groupCard._listCards.push_back(trashCard);
                        else
                            listCards.pop_back();
                    }
                }
                this->sortIncrease(groupCard._listCards);
                this->updateGroup(groupCard);
                addedGroupCard = groupCard;
                break;
            }
        }
        if (addedGroupCard._listCards.size() == 0)
        {
            std::vector<GinCard *> listTrashCards = this->getListTrashCardsOnHand();
            CardInHand cardInHand = getPhom(listTrashCards);
            if (cardInHand._phom.size() > 0)
            {
                int indexMeld = -1;
                for (int i = 0; i < cardInHand._phom.size(); i++)
                {
                    const std::vector<int> &phom = cardInHand._phom[i];
                    if (MeldIsValid(phom))
                    {
                        for (const int &cardIdx: phom)
                        {
                            if (cardIdx == card->getCardIndex())
                            {
                                indexMeld = i;
                                break;
                            }
                        }
                        if (indexMeld >= 0)
                        {
                            break;
                        }
                    }
                }
                if (indexMeld != -1)
                {
                    std::vector<GinCard *> listCards;
                    for (const int &card2: cardInHand._phom[indexMeld])
                    {
                        for (auto const &tgCard: listTrashCards)
                        {
                            if (card2 == tgCard->getCardIndex())
                            {
                                listCards.push_back(tgCard);
                                break;
                            }
                        }
                    }
                    addedGroupCard = this->addGroup(listCards, true, true);
                }
            }
        }
        if (addedGroupCard._listCards.size() > 0)
        {
            float delayTimeToUnSelectCard = 0;
            for (int i = 0; i < addedGroupCard._listCards.size(); i++)
            {
                if (addedGroupCard._listCards[i]->getState() == GinCardState::GinCARDSTATE_SELECTED)
                {
                    delayTimeToUnSelectCard = 0.1f;
                    break;
                }
            }
            this->runAction(Sequence::create(CallFunc::create([=]()
                                                              {
                                                                  for (int i = 0;
                                                                       i < addedGroupCard._listCards.size(); i++)
                                                                      addedGroupCard._listCards.at(i)->unSelected(true);
                                                                  if (delayTimeToUnSelectCard > 0)
                                                                  {
                                                                      this->showGroupButton(false,
                                                                                            std::vector<GinCard *>());
                                                                      this->showUngroupButton(false,
                                                                                              std::vector<GinCard *>());
                                                                  }
                                                              }), DelayTime::create(delayTimeToUnSelectCard),
                                             CallFunc::create([=]()
                                                              {
                                                                  this->arrangeCardOnHand(
                                                                          TypeArrangeCardOnHand::TYPE_ARRANGE_GROUP_CARD);
                                                                  this->animationShowGroup(addedGroupCard);
                                                              }), nullptr));
        } else
        {
            this->arrangeCardOnHand(TypeArrangeCardOnHand::TYPE_ARRANGE_GROUP_CARD);
        }
    }
}

void GPGinControlCardLayer::softArrangeCardOnHandAfterPickCard(GinCard *card)
{
    bool newMelds = this->updateListSoftGroupHandCardMainUser();
    MessageSender::requestCustomMelds(MAIN_USER_SLOT_INDEX, getListMeldOnHandMainUserBySoftGroup());
}

void GPGinControlCardLayer::playEffectSortCardsOnHand()
{
    this->calculateScoreCardOnHandBySoftGroup();

    // Set property for each card
//    GinCard* tlCard = nullptr;
    int numberCards = _listHandCardsMainUser.size();
    const std::vector<GinCardProperty> &listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
            numberCards);
    for (int i = 0; i < numberCards; i++)
    {
        const GinCardProperty &properties = listPropertyCard[i];
        GinCard *tlCard = _listHandCardsMainUser[i];
        if (tlCard)
        {
            tlCard->setProperty(properties);
            tlCard->setLocalZOrder(properties._zOder);
            tlCard->setScale(properties._scale);
            Spawn *actSpawn = nullptr;
            if (tlCard->getState() == GinCardState::GinCARDSTATE_ONHAND)
            {
                float time = tlCard->getPosition().distance(tlCard->getProperty()._positionOnHand) *
                             TIME_ACTION_CARD_MOVE_ON_HAND_1PIXEL;
                actSpawn = Spawn::create(
                        EaseQuarticActionInOut::create(MoveTo::create(time, tlCard->getProperty()._positionOnHand)),
                        RotateTo::create(time, tlCard->getProperty()._angle), nullptr);
            } else if (tlCard->getState() == GinCardState::GinCARDSTATE_SELECTED)
            {
                float time = tlCard->getPosition().distance(tlCard->getProperty()._positionOnHandWasSelected) *
                             TIME_ACTION_CARD_MOVE_ON_HAND_1PIXEL;
                actSpawn = Spawn::create(EaseQuarticActionInOut::create(
                                                 MoveTo::create(time, tlCard->getProperty()._positionOnHandWasSelected)),
                                         RotateTo::create(time, tlCard->getProperty()._angle),
                                         nullptr);
            } else if (tlCard->getState() == GinCardState::GinCARDSTATE_DRAG)
            {
                if (_currentDragCardsType == GinCardDragType::GinDRAGCARD_GROUP_CARD)
                    tlCard->setLocalZOrder(tlCard->getProperty()._zOder + 13);
            }
            if (actSpawn)
            {
                actSpawn->setTag(TAG_ACTION_MOVEANDROTATE_CARD);
                tlCard->stopActionByTag(TAG_ACTION_SELECTED_CARD);
                tlCard->stopActionByTag(TAG_ACTION_UNSELECTED_CARD);
                tlCard->stopAllActionsByTag(TAG_ACTION_MOVEANDROTATE_CARD);
                tlCard->runAction(actSpawn);
            }
        }
    }
}

void GPGinControlCardLayer::playEffectSortCardsOnHandSpeedControl(float timeScale)
{
    this->calculateScoreCardOnHandBySoftGroup();

    // Set property for each card
//    GinCard* tlCard = nullptr;
    int numberCards = _listHandCardsMainUser.size();
    const std::vector<GinCardProperty> &listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
            numberCards);
    for (int i = 0; i < numberCards; i++)
    {
        const GinCardProperty &properties = listPropertyCard[i];
        GinCard *tlCard = _listHandCardsMainUser[i];
        if (tlCard)
        {
            tlCard->setProperty(properties);
            tlCard->setLocalZOrder(properties._zOder);
            tlCard->setScale(properties._scale);
            Spawn *actSpawn = nullptr;
            if (tlCard->getState() == GinCardState::GinCARDSTATE_ONHAND)
            {
                float time = tlCard->getPosition().distance(tlCard->getProperty()._positionOnHand) *
                             TIME_ACTION_CARD_MOVE_ON_HAND_1PIXEL * timeScale;
                actSpawn = Spawn::create(
                        EaseQuarticActionInOut::create(MoveTo::create(time, tlCard->getProperty()._positionOnHand)),
                        RotateTo::create(time, tlCard->getProperty()._angle), nullptr);
            } else if (tlCard->getState() == GinCardState::GinCARDSTATE_SELECTED)
            {
                float time = tlCard->getPosition().distance(tlCard->getProperty()._positionOnHandWasSelected) *
                             TIME_ACTION_CARD_MOVE_ON_HAND_1PIXEL * timeScale;
                actSpawn = Spawn::create(EaseQuarticActionInOut::create(
                                                 MoveTo::create(time, tlCard->getProperty()._positionOnHandWasSelected)),
                                         RotateTo::create(time, tlCard->getProperty()._angle),
                                         nullptr);
            } else if (tlCard->getState() == GinCardState::GinCARDSTATE_DRAG)
            {
                if (_currentDragCardsType == GinCardDragType::GinDRAGCARD_GROUP_CARD)
                    tlCard->setLocalZOrder(tlCard->getProperty()._zOder + 13);
            }
            if (actSpawn)
            {
                actSpawn->setTag(TAG_ACTION_MOVEANDROTATE_CARD);
                tlCard->stopActionByTag(TAG_ACTION_SELECTED_CARD);
                tlCard->stopActionByTag(TAG_ACTION_UNSELECTED_CARD);
                tlCard->stopAllActionsByTag(TAG_ACTION_MOVEANDROTATE_CARD);
                tlCard->runAction(actSpawn);
            }
        }
    }
}

void GPGinControlCardLayer::sortIncrease(std::vector<GinCard *> &list)
{
    std::sort(list.begin(), list.end(),
              [=](GinCard *n1, GinCard *n2) { return n1->getCardIndex() < n2->getCardIndex(); });
}

void GPGinControlCardLayer::sortIncrease(std::vector<int> &list)
{
    std::sort(list.begin(), list.end(), [=](const int &n1, const int &n2) { return n1 < n2; });
}

void GPGinControlCardLayer::sortDecrease(std::vector<GinCard *> &list)
{
    std::sort(list.begin(), list.end(),
              [=](GinCard *n1, GinCard *n2) { return n1->getCardIndex() > n2->getCardIndex(); });
}

void GPGinControlCardLayer::sortDecrease(std::vector<int> &list)
{
    std::sort(list.begin(), list.end(), [=](const int &n1, const int &n2) { return n1 > n2; });
}

void GPGinControlCardLayer::sortSuit(std::vector<GinCard *> &list)
{
    std::vector<std::vector<GinCard *>> listCardSuit;
    for (int i = 0; i < MAXIMUM_NUMBER_SUIT; i++)
        listCardSuit.push_back(std::vector<GinCard *>());
    for (GinCard *card: list)
        listCardSuit[card->getCardIndex() % MAXIMUM_NUMBER_SUIT].push_back(card);
    list.clear();

    for (auto const &cards: listCardSuit)
        for (auto const &card: cards)
            list.push_back(card);
}

MeldType GPGinControlCardLayer::getPhomType(const std::vector<int> &phom)
{
    int v1 = phom.at(0);
    int v2 = phom.at(1);
    if (MeldIsValid(phom))
    {
        if (v1 / MAXIMUM_NUMBER_SUIT == v2 / MAXIMUM_NUMBER_SUIT)
            return MeldType::MELD_BALA;
        return MeldType::MELD_SANH;
    }
    return MeldType::MELD_NONE;
}

MeldType GPGinControlCardLayer::getPhomType(const std::vector<GinCard *> &phom)
{
    int v1 = phom.at(0)->getCardIndex();
    int v2 = phom.at(1)->getCardIndex();
    if (MeldIsValid(phom))
    {
        if (v1 / MAXIMUM_NUMBER_SUIT == v2 / MAXIMUM_NUMBER_SUIT)
            return MeldType::MELD_BALA;
        return MeldType::MELD_SANH;
    }
    return MeldType::MELD_NONE;
}

bool GPGinControlCardLayer::Give(std::vector<int> &phom, int card)
{
    MeldType type = getPhomType(phom);
    if (type == MeldType::MELD_SANH)
    {
        int v0 = phom.at(0);
        int vEnd = phom.at(phom.size() - 1);
        if (LienTuc(v0, card))
        {
            phom.insert(phom.begin(), card);
            return true;
        } else if (LienTuc(vEnd, card))
        {
            phom.push_back(card);
            return true;
        }
    } else
    {
        int v0 = phom.at(0);
        if (Doi(v0, card))
        {
            phom.push_back(card);
            return true;
        }
    }
    return false;
}

bool GPGinControlCardLayer::canGive(const std::vector<int> &phom, int card)
{
    MeldType type = getPhomType(phom);
    if (type == MeldType::MELD_SANH)
    {
        int v1 = phom.at(0);
        int v2 = phom.at(phom.size() - 1);
        if (LienTuc(card, v1) || LienTuc(card, v2))
            return true;
    } else
    {
        int v1 = phom.at(0);
        if (v1 / 4 == card / 4)
            return true;
    }
    return false;
}

bool GPGinControlCardLayer::checkCardInListTakedCard(int card)
{
    for (auto const &tgCard: _listTakeCard.at(0))
        if (tgCard->getCardIndex() == card)
            return true;
    return false;
}

Point GPGinControlCardLayer::checkIn(const std::vector<std::vector<int>> &listPhom, int card)
{
    Point result = Point(-1, -1);
    for (int i = 0; i < listPhom.size(); i++)
        for (int j = 0; j < listPhom.at(i).size(); j++)
            if (listPhom.at(i).at(j) == card)
            {
                result.x = i;
                result.y = j;
                return result;
            }
    return result;
}

int GPGinControlCardLayer::calCol(int a[13][4], int nr, int nc, int col)
{
    int sum = 0;
    for (int i = 0; i < nr; i++)
        sum += a[i][col];
    return sum;
}

int GPGinControlCardLayer::calRow(int a[13][4], int nr, int nc, int row)
{
    int sum = 0;
    for (int i = 0; i < nc; i++)
        sum += a[row][i];
    return sum;
}

int GPGinControlCardLayer::Max(const std::vector<int> &list)
{
    int max = list.at(0);
    for (const int &i: list)
        if (i > max)
            max = i;
    return max;
}


std::vector<std::vector<int>> GPGinControlCardLayer::getCa(std::vector<int> list)
{
    sortIncrease(list);
    vector<int> listSpecialCard;
    for (auto itorCard = list.begin(); itorCard != list.end();)
        itorCard++;
    vector<int> trash;
    int a[13][4];
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
            a[i][j] = 0;

    for (int i = 0; i < list.size(); i++)
    {
        int value = list.at(i);
        int r = value / 4;
        int c = value % 4;
        a[r][c] = 1;
    }
    std::vector<std::vector<int>> list_ca_doi;
    for (int i = 12; i >= 0; i--)
    {
        std::vector<int> doi;
        for (int j = 3; j >= 0; j--)
        {
            if (a[i][j] == 1)
            {
                int value = i * 4 + j;
                doi.push_back(value);
            }
        }
        if (doi.size() == 2)
            list_ca_doi.push_back(doi);
        else if (doi.size() == 1)
            trash.push_back(doi.at(0));
    }
    std::vector<std::vector<int>> list_ca_sanh;
    for (const int &ii: trash)
    {
        if (list_ca_sanh.empty())
        {
            list_ca_sanh.push_back({ii});
        } else
        {
            bool add = true;
            for (auto &lis: list_ca_sanh)
            {
                if (Ca(lis.at(lis.size() - 1), ii))
                {
                    lis.push_back(ii);
                    add = false;
                    break;
                }
            }
            if (add)
                list_ca_sanh.push_back({ii});
        }
    }
    int size1 = list_ca_sanh.size();
    int size2 = size1;
    do
    {
        size1 = list_ca_sanh.size();
        for (int i = 0; i < list_ca_sanh.size(); i++)
        {
            if (list_ca_sanh.at(i).size() == 1)
            {
                int value = list_ca_sanh.at(i).at(0);
                bool need_remove = false;
                for (auto &jj: list_ca_doi)
                {
                    for (int k = 0; k < jj.size(); k++)
                    {
                        int v = jj.at(k);
                        if (Ca(value, v))
                        {
                            int recheck = 0;
                            if (k == 0)
                            {
                                jj.insert(jj.begin(), value);
                                if (value < v)
                                    recheck = 1;
                            } else if (k == jj.size() - 1)
                            {
                                jj.push_back(value);
                                if (value > v)
                                    recheck = 2;
                            } else
                            {
                                int vl = jj.at(k - 1);
                                if (Doi(vl, v))
                                    jj.insert(jj.begin() + k + 1, value);
                                else
                                    jj.insert(jj.begin() + k, value);
                            }
                            if (recheck == 1 && jj.size() == 3)
                            {
                                int v2 = jj.at(2);
                                jj.clear();
                                jj.push_back(v2);
                                jj.push_back(v);
                                jj.push_back(value);
                            } else if (recheck == 2 && jj.size() == 3)
                            {
                                int v2 = jj.at(0);
                                jj.clear();
                                jj.push_back(value);
                                jj.push_back(v);
                                jj.push_back(v2);
                            }
                            need_remove = true;
                            break;
                        }
                    }
                    if (need_remove)
                        break;
                }
                if (need_remove)
                {
                    list_ca_sanh.erase(list_ca_sanh.begin() + i);
                    i -= 1;
                }
            }
        }
        size2 = list_ca_sanh.size();
    } while (size1 != size2);
    for (auto const &doi: list_ca_doi)
        list_ca_sanh.push_back(doi);
    for (int i = 0; i < list_ca_sanh.size() - 1; i++)
    {
        for (int j = i; j < list_ca_sanh.size(); j++)
        {
            int max1 = Max(list_ca_sanh.at(i));
            if (list_ca_sanh.at(i).size() == 1)
                max1 = max1 + 500;
            int max2 = Max(list_ca_sanh.at(j));
            if (list_ca_sanh.at(j).size() == 1)
                max2 = max2 + 500;

            if (max2 < max1)
            {
                std::vector<int> temp = list_ca_sanh.at(i);
                list_ca_sanh.at(i) = list_ca_sanh.at(j);
                list_ca_sanh.at(j) = temp;
            }
        }
    }
    if (listSpecialCard.size() > 0)
        list_ca_sanh.insert(list_ca_sanh.begin(), listSpecialCard);
    return list_ca_sanh;
}

bool GPGinControlCardLayer::onTouchBegan(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    CCLOG("onTouchBegan");
    if (this->_ginDrawAndTakeAnimEnable == true)
    {
        Vec2 touchLocation = this->_layoutRoot->convertToNodeSpace(ptouch->getLocationInView());

        if (_layoutTouchDraw->getBoundingBox().containsPoint(touchLocation) == true &&
            _status == ControlStatus::CONTROL_STATUS_GET)
        {
            return this->onTouchBeganLayoutTouchDraw(ptouch, pEvent);
        }

        if (_layoutTouchTake->getBoundingBox().containsPoint(touchLocation) == true &&
            (_status == ControlStatus::CONTROL_STATUS_GET || _status == ControlStatus::CONTROL_STATUS_PASSING))
        {
            return this->onTouchBeganLayoutTouchTake(ptouch, pEvent);
        }
    }

    return this->onTouchBeganCard(ptouch, pEvent);
}

void GPGinControlCardLayer::onTouchMoved(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    CCLOG("onTouchMoved");
    if (this->_ginDrawAndTakeAnimationRunning == true)
    {
        this->_ginDrawAndTakeCurrentTouch = ptouch;
    }

    this->onTouchMovedCard(ptouch, pEvent);
}

void GPGinControlCardLayer::onTouchEnded(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    CCLOG("onTouchEnded");
    if (this->_ginDrawAndTakeAnimationRunning == true &&
        this->_ginDrawAndTakeType == FakeCardType::DRAW)
    {
        return this->onTouchEndedLayoutTouchDraw(ptouch, pEvent);
    }

    if (this->_ginDrawAndTakeAnimationRunning == true &&
        this->_ginDrawAndTakeType == FakeCardType::TAKE)
    {
        return this->onTouchEndedLayoutTouchTake(ptouch, pEvent);
    }

    this->onTouchEndedCard(ptouch, pEvent);
}

void GPGinControlCardLayer::onTouchCancelled(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    CCLOG("onTouchCancelled");
    if (this->_ginDrawAndTakeAnimationRunning == true &&
        this->_ginDrawAndTakeType == FakeCardType::DRAW)
    {
        return this->onTouchCancelledLayoutTouchDraw(ptouch, pEvent);
    }

    if (this->_ginDrawAndTakeAnimationRunning == true &&
        this->_ginDrawAndTakeType == FakeCardType::TAKE)
    {
        return this->onTouchCancelledLayoutTouchTake(ptouch, pEvent);
    }


    this->onTouchCancelledCard(ptouch, pEvent);
}

void GPGinControlCardLayer::initDrawAndTakeAnimationCachedData()
{
    this->_ginDrawAndTakeAnimEnable = DRAW_AND_TAKE_ANIM_ENABLE;
    if (this->_ginDrawAndTakeFakeCard != nullptr)
    {
        this->_ginDrawAndTakeFakeCard->setVisible(false);
        this->_ginDrawAndTakeFakeCard->removeFromParent();
        this->_ginDrawAndTakeFakeCard = nullptr;
    }
    this->_ginDrawAndTakeEvent = nullptr;
    this->_ginDrawAndTakeFirstTouch = nullptr;
    this->_ginDrawAndTakeCurrentTouch = nullptr;
    this->_ginDrawAndTakeAnimationRunning = false;
    this->_ginDrawAndTakeTouchHoldCountDown = DRAW_AND_TAKE_ANIM_TOUCH_HOLD_COUNT_DOWN;
    this->_ginDrawAndTakeType = FakeCardType::NONE;
    this->_ginDrawAndTakeOriginGlobalPos = Point::ZERO;
}

void GPGinControlCardLayer::resetDrawAndTakeAnimationCachedData()
{
    this->_ginDrawAndTakeAnimEnable = DRAW_AND_TAKE_ANIM_ENABLE;
    if (this->_ginDrawAndTakeFakeCard != nullptr)
    {
        this->_ginDrawAndTakeFakeCard->setVisible(false);
        this->_ginDrawAndTakeFakeCard->removeFromParent();
        this->_ginDrawAndTakeFakeCard = nullptr;
    }
    this->_ginDrawAndTakeEvent = nullptr;
    this->_ginDrawAndTakeFirstTouch = nullptr;
    this->_ginDrawAndTakeCurrentTouch = nullptr;
    this->_ginDrawAndTakeAnimationRunning = false;
    this->_ginDrawAndTakeTouchHoldCountDown = DRAW_AND_TAKE_ANIM_TOUCH_HOLD_COUNT_DOWN;
    this->_ginDrawAndTakeType = FakeCardType::NONE;
    this->_ginDrawAndTakeOriginGlobalPos = Point::ZERO;
}

void GPGinControlCardLayer::addFakeCardToUserHand(GinCard *card)
{
    if (card == nullptr)
    {
        return;
    }

    _listHandCardsMainUser.push_back(card);
    _layoutCards->addChild(card);

    std::vector<GinCardProperty> listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
                    _listHandCardsMainUser.size() + 1);
    card->setProperty(listPropertyCard.back());
}

void GPGinControlCardLayer::removeFakeCardFromUserHand(GinCard *card, bool sortCardOnHand /*= true*/)
{
    if (card == nullptr)
    {
        return;
    }

    card->stopAllActions();
    this->_listHandCardsMainUser.erase(std::remove(_listHandCardsMainUser.begin(), _listHandCardsMainUser.end(), card));
    this->_layoutCards->removeChild(card);

    if (sortCardOnHand == true)
    {
        playEffectSortCardsOnHand();
    }
}

void GPGinControlCardLayer::createDrawAndTakeFakeCard()
{
    if (this->_ginDrawAndTakeAnimationRunning == true &&
        this->_ginDrawAndTakeTouchHoldCountDown <= 0 &&
        this->_ginDrawAndTakeFakeCard == nullptr &&
        this->_ginDrawAndTakeType != FakeCardType::NONE)
    {
        if (this->_ginDrawAndTakeType == FakeCardType::DRAW)
        {
            this->_ginDrawAndTakeFakeCard = this->createCard(-1, /*CARD_IN_HAND_SCALE*/GPGinInterfaceLayer::_scaleTakeCard, false);
        } else
        {
            GinCard *lastDumpCard = this->_listThrowCards.back();

            this->_ginDrawAndTakeFakeCard = this->createCard(lastDumpCard->getCardIndex(), /*CARD_IN_HAND_SCALE*/GPGinInterfaceLayer::_scaleTakeCard, true);

            lastDumpCard->setVisible(false);
        }
        this->addFakeCardToUserHand(this->_ginDrawAndTakeFakeCard);

        Point pos = this->_ginDrawAndTakeFakeCard->getParent()->convertToNodeSpace(
                        this->_ginDrawAndTakeCurrentTouch->getLocation());
        pos.y += DRAG_CARD_POSITION_Y_OFFSET;
        this->_ginDrawAndTakeFakeCard->setPosition(pos);
        this->onTouchBeganCard(this->_ginDrawAndTakeFirstTouch, this->_ginDrawAndTakeEvent);
    }
}

void
GPGinControlCardLayer::returnFakeCardToOriginPositionAndRemove(bool playAnim /*=false*/, bool sortCardOnHand /*= true*/)
{
    if (this->_ginDrawAndTakeFakeCard == nullptr)
    {
        this->resetDrawAndTakeAnimationCachedData();
        return;
    }

    this->_ginDrawAndTakeAnimEnable = false;
    this->_ginDrawAndTakeFakeCard->setTouchEnable(false);
    this->enableDrawAndTakeButton(false);

    if (playAnim == true)
    {
        this->_ginDrawAndTakeFakeCard->runAction(
                Sequence::create(MoveTo::create(DRAW_AND_TAKE_ANIM_CARD_RETURN_DURATION,
                                                this->_ginDrawAndTakeFakeCard->getParent()->convertToNodeSpace(
                                                        this->_ginDrawAndTakeOriginGlobalPos)),
                                 CallFunc::create([=]
                                                  {

                                                      if (this->_ginDrawAndTakeType == FakeCardType::TAKE)
                                                      {
                                                          this->_listThrowCards.back()->setVisible(true);
                                                      }

                                                      removeFakeCardFromUserHand(this->_ginDrawAndTakeFakeCard,
                                                                                 sortCardOnHand);
                                                      this->_ginDrawAndTakeFakeCard = nullptr;
                                                      this->resetDrawAndTakeAnimationCachedData();
                                                      this->_ginDrawAndTakeAnimEnable = DRAW_AND_TAKE_ANIM_ENABLE;
                                                      this->enableDrawAndTakeButton(true);
                                                  }),
                                 nullptr));
    } else
    {
        if (this->_ginDrawAndTakeType == FakeCardType::TAKE)
        {
            this->_listThrowCards.back()->setVisible(true);
        }

        removeFakeCardFromUserHand(this->_ginDrawAndTakeFakeCard, sortCardOnHand);
        this->_ginDrawAndTakeFakeCard = nullptr;
        this->resetDrawAndTakeAnimationCachedData();
        this->_ginDrawAndTakeAnimEnable = DRAW_AND_TAKE_ANIM_ENABLE;
        this->enableDrawAndTakeButton(true);
    }
}

void GPGinControlCardLayer::resetDrawAndTakeAnimationCachedDataOnDrawCard()
{
    if (this->_ginDrawAndTakeFakeCard == nullptr)
    {
        this->resetDrawAndTakeAnimationCachedData();
        return;
    }

    this->_ginDrawAndTakeFakeCard = nullptr;
    this->_ginDrawAndTakeAnimEnable = false;
    this->resetDrawAndTakeAnimationCachedData();
    this->_ginDrawAndTakeAnimEnable = DRAW_AND_TAKE_ANIM_ENABLE;
}

void GPGinControlCardLayer::setGinDrawAndTakeCardProperty(GinCard *card)
{
    if (card == nullptr)
    {
        _ginDrawAndTakeFakeCardHandIndex = -1;
    }

    _ginDrawAndTakeCardProperty = card->getProperty();
    _ginDrawAndTakeCardProperty._positionOnHand = card->getPosition();
    _ginDrawAndTakeCardProperty._zOder = card->getLocalZOrder();

    _ginDrawAndTakeFakeCardHandIndex = this->getIndexCardOnHand(card);
}

void GPGinControlCardLayer::enableDrawAndTakeButton(bool enable)
{
    this->_buttonDraw->setEnabled(enable);
    this->_buttonTake->setEnabled(enable);

    this->_buttonDraw->setTouchEnabled(enable);
    this->_buttonTake->setTouchEnabled(enable);
}

bool GPGinControlCardLayer::onTouchBeganLayoutTouchTake(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{

    this->_ginDrawAndTakeFirstTouch = ptouch;
    this->_ginDrawAndTakeEvent = pEvent;
    this->_ginDrawAndTakeCurrentTouch = ptouch;
    this->_ginDrawAndTakeAnimationRunning = true;

    this->_ginDrawAndTakeType = FakeCardType::TAKE;

    this->_ginDrawAndTakeOriginGlobalPos = this->_layoutTouchTake->getParent()->convertToWorldSpace(
            this->_layoutTouchTake->getPosition());

    return true;

}

void GPGinControlCardLayer::onTouchMovedLayoutTouchTake(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{

}

void GPGinControlCardLayer::onTouchEndedLayoutTouchTake(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    removeEffectHighlight("EffectDragToHand");
    if (this->_ginDrawAndTakeFakeCard == nullptr)
    {
        this->resetDrawAndTakeAnimationCachedData();
        this->_ginDrawAndTakeFakeCardHandIndex = -1;
        return;
    }

    Rect layoutCardBoundingBox = Rect(Vec2::ZERO,
                                      this->_layoutCards->getContentSize() * this->_layoutCards->getScale());

    if (layoutCardBoundingBox.containsPoint(this->_ginDrawAndTakeFakeCard->getPosition()) == true)
    {
        this->setGinDrawAndTakeCardProperty(this->_ginDrawAndTakeFakeCard);

        this->returnFakeCardToOriginPositionAndRemove(false, false);

        if (_status == ControlStatus::CONTROL_STATUS_PASSING)
        {
            this->hintPassCard(false, false, false);
            MessageSender::requestPass(MAIN_USER_SLOT_INDEX, _gameGin->getUpCardValue());
            this->resetDrawAndTakeAnimationCachedData();
        } else if (_status == ControlStatus::CONTROL_STATUS_GET)
        {
            this->hintGetCard(false, MAIN_USER_SLOT_INDEX, -1, false, false);
            int previousSlotIdx = GPGinSlotManager::getInstance()->getPreviousSlotIdx(
                    GPGinSlotManager::getInstance()->getSlotIdx(_gameGin->getTurnActionOrderIdx()));
            MessageSender::requestGetCard(previousSlotIdx);
            this->resetDrawAndTakeAnimationCachedData();
        }
    } else
    {
        this->returnFakeCardToOriginPositionAndRemove();
    }
}

void GPGinControlCardLayer::onTouchCancelledLayoutTouchTake(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    removeEffectHighlight("EffectDragToHand");
    this->returnFakeCardToOriginPositionAndRemove();
}

bool GPGinControlCardLayer::onTouchBeganLayoutTouchDraw(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
    this->_ginDrawAndTakeFirstTouch = ptouch;
    this->_ginDrawAndTakeEvent = pEvent;
    this->_ginDrawAndTakeCurrentTouch = ptouch;
    this->_ginDrawAndTakeAnimationRunning = true;

    this->_ginDrawAndTakeType = FakeCardType::DRAW;

    this->_ginDrawAndTakeOriginGlobalPos = this->_layoutTouchDraw->getParent()->convertToWorldSpace(
            this->_layoutTouchDraw->getPosition());

    return true;
}

void GPGinControlCardLayer::onTouchMovedLayoutTouchDraw(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
}

void GPGinControlCardLayer::onTouchEndedLayoutTouchDraw(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    removeEffectHighlight("EffectDragToHand");
    if (this->_ginDrawAndTakeFakeCard == nullptr)
    {
        this->resetDrawAndTakeAnimationCachedData();
        this->_ginDrawAndTakeFakeCardHandIndex = -1;
        return;
    }

    Rect layoutCardBoundingBox = Rect(Vec2::ZERO,
                                      this->_layoutCards->getContentSize() * this->_layoutCards->getScale());

    if (layoutCardBoundingBox.containsPoint(this->_ginDrawAndTakeFakeCard->getPosition()) == true)
    {
        this->setGinDrawAndTakeCardProperty(this->_ginDrawAndTakeFakeCard);
        this->returnFakeCardToOriginPositionAndRemove(false, false);
        if (_status == ControlStatus::CONTROL_STATUS_GET)
        {
            this->hintGetCard(false, 0, 0, false, false);
            MessageSender::requestGetCard();
            this->resetDrawAndTakeAnimationCachedData();
        }
    } else
    {
        this->returnFakeCardToOriginPositionAndRemove();
    }
}

void GPGinControlCardLayer::onTouchCancelledLayoutTouchDraw(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
//    removeEffectHighlight("EffectDragToHand");
    this->returnFakeCardToOriginPositionAndRemove();
}

bool GPGinControlCardLayer::onTouchBeganCard(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
    if (_cardsIsOnHand && this->enableTouchCard() && _status != ControlStatus::CONTROL_STATUS_MATCH_END &&
        _status != ControlStatus::CONTROL_STATUS_FIGHT_CHALLENGE_FOLD)
    {
        for (int i = _listHandCardsMainUser.size() - 1; i >= 0; i--)
        {
            auto const &card = _listHandCardsMainUser[i];
            if (card->containPoint(card->convertToNodeSpace(ptouch->getLocation())))
            {
                card->onTouchBegan(ptouch, pEvent);
                pEvent->stopPropagation();
                return true;
            }
        }
    }
    return false;
}

void GPGinControlCardLayer::onTouchMovedCard(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
    if (_cardsIsOnHand)
    {
        for (int i = _listHandCardsMainUser.size() - 1; i >= 0; i--)
        {
            auto const &card = _listHandCardsMainUser[i];
            if (card->getTouch() != nullptr)
            {
                this->checkUserMovingCardsInSpecificArea(ptouch, card);
                card->onTouchMoved(ptouch, pEvent);

                if(card == this->_ginDrawAndTakeFakeCard &&
                this->_ginDrawAndTakeType == FakeCardType::DRAW)
                {
                    Rect layoutCardBoundingBox = Rect(Vec2::ZERO,
                                      this->_layoutCards->getContentSize() * this->_layoutCards->getScale());
                    if (layoutCardBoundingBox.containsPoint(this->_ginDrawAndTakeFakeCard->getPosition()) == true)
                    {
                        this->setGinDrawAndTakeCardProperty(this->_ginDrawAndTakeFakeCard);
//                        this->returnFakeCardToOriginPositionAndRemove(false, false);
                        if (_status == ControlStatus::CONTROL_STATUS_GET)
                        {
                            this->hintGetCard(false, 0, 0, false, false);
                            MessageSender::requestGetCard();
//                            this->resetDrawAndTakeAnimationCachedData();
                        }
                    }
                }

                break;
            }
        }
    }
}

void GPGinControlCardLayer::onTouchEndedCard(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
    if (_cardsIsOnHand)
    {
        for (int i = _listHandCardsMainUser.size() - 1; i >= 0; i--)
        {
            auto const &card = _listHandCardsMainUser[i];
            if (card->getTouch() != nullptr)
            {
                HintDropPlaceType hintDropPlaceType = this->checkUserDropCardsInSpecificArea();
                if (hintDropPlaceType != HintDropPlaceType::HintDropPlaceNone)
                {
                    if (hintDropPlaceType == HintDropPlaceType::HintDropPlaceAtThrowCardsArea)
                    {
                        this->onRequestDump(false, ActionType::K_NONE, _listHandCardsMainUser[i], ptouch, pEvent);
                    }
                    this->clearAllEffectHintDropArea();
                } else
                {
                    card->onTouchEnded(ptouch, pEvent);
                    bool newMelds = this->updateListSoftGroupHandCardMainUser();
                    this->calculateScoreCardOnHandBySoftGroup();

                    MessageSender::requestCustomMelds(MAIN_USER_SLOT_INDEX, getListMeldOnHandMainUserBySoftGroup());
                }
                break;
            }
        }
        //        this->checkStateOfGroupAndUngroupButton();
    }
}

void GPGinControlCardLayer::onTouchCancelledCard(cocos2d::Touch *ptouch, cocos2d::Event *pEvent)
{
    this->onTouchEndedCard(ptouch, pEvent);
}

void GPGinControlCardLayer::getAngleBetweenTwoCards(int numberCard)
{
    float lengthOpposite = _layoutCards->getContentSize().width / 2 - GIN_WIDTH_CARD * CARD_IN_HAND_SCALE / 2;

    Vec2 nodeAnchorCardsWorldPos = this->_nodeAnchorCards->getParent()->convertToWorldSpace(_nodeAnchorCards->getPosition());
    Vec2 positionNodeAnchorCards = _layoutCards->convertToNodeSpace(nodeAnchorCardsWorldPos);

    //    Vec2 positionNodeAnchorCards = _layoutCards->convertToNodeSpace(_nodeAnchorCards->getPosition());

    float lenghtHypotenuse = std::abs(positionNodeAnchorCards.y) + _layoutCards->getContentSize().height / 2;
    float angle = (asin(lengthOpposite / lenghtHypotenuse) * ANGLE_PI / PI) * 2 / (numberCard - 1);
    GPGinControlCardLayer::_r = lenghtHypotenuse;
    GPGinControlCardLayer::_angleBetweenTwoCards = angle * 8 / 10;
}

std::vector<GinCardProperty> GPGinControlCardLayer::listPositionCardsOnHandAccordingNumberCards(int numberCard)
{
    if (STYLE_CARDS_ON_HAND == STYLE_CARDS_ON_HAND_STRAIGHT)
        return this->listPositionCardsOnHandAccordingNumberCards_Straight(numberCard);
    return this->listPositionCardsOnHandAccordingNumberCards_Curve(numberCard);
}

std::vector<GinCardProperty> GPGinControlCardLayer::listPositionCardsOnHandAccordingNumberCards_Straight(int numberCard,
                                                                                                         bool isConvertWorldSpace/* = false*/)
{
    std::vector<GinCardProperty> listPropertyCard;
    bool allCardOnHandInGroup = this->checkAllCadsOnHandInGroup();
    std::vector<int> listGroupCardIndex;
    int cardIndex = 0;
    for (auto const &group: _listGroupHandCardsMainUser)
    {
        cardIndex += group._listCards.size();
        listGroupCardIndex.push_back(cardIndex);
    }
    Vec2 lastPosCard = Vec2(GIN_WIDTH_CARD * 0.5f * CARD_IN_HAND_SCALE, _layoutCards->getContentSize().height * 0.5f);
    float widthOfLayoutCard = GIN_WIDTH_CARD * 1.0f * CARD_IN_HAND_SCALE +
                              (numberCard - 1 -
                               ((allCardOnHandInGroup && listGroupCardIndex.size() > 0) ? listGroupCardIndex.size() - 1
                                                                                        : listGroupCardIndex.size())) *
                              _distanceBetweenTwoCards +
                              ((allCardOnHandInGroup && listGroupCardIndex.size() > 0) ? listGroupCardIndex.size() - 1
                                                                                       : listGroupCardIndex.size()) *
                              _distanceGapBetweenTwoMelds;
    for (int i = 0; i < numberCard; i++)
    {
        bool isGap = false;
        for (const int &j: listGroupCardIndex)
        {
            if (i == j)
            {
                isGap = true;
                break;
            }
        }
        float x = lastPosCard.x;
        if (i > 0)
            x = lastPosCard.x + (isGap ? _distanceGapBetweenTwoMelds : _distanceBetweenTwoCards);
        float y = lastPosCard.y;
        lastPosCard = Vec2(x, y);
        x = x + (_layoutCards->getContentSize().width - widthOfLayoutCard) * 0.5f;
        Point position = Point(x, y);
        if (isConvertWorldSpace)
            position = _layoutCards->convertToWorldSpace(position);
        float angle = 0;
        GinCardProperty tlCardProperty = {position, position + Vec2(0, DISTANCE_WAS_SELECTED_CARD), angle, i,
                                          CARD_IN_HAND_SCALE};
        listPropertyCard.push_back(tlCardProperty);
    }
    return listPropertyCard;
}

std::vector<GinCardProperty> GPGinControlCardLayer::listPositionCardsOnHandAccordingNumberCards_Curve(int numberCard,
                                                                                                      bool isConvertWorldSpace/* = false*/)
{
    std::vector<GinCardProperty> listPropertyCard;
    if (numberCard % 2 == 0)
    {
        for (int i = 0; i < numberCard; i++)
        {
            if (i < numberCard / 2)
            {
                float angle = 0 - (_angleBetweenTwoCards / 2 + (numberCard / 2 - i - 1) * _angleBetweenTwoCards);
                Point position = _layoutCards->convertToNodeSpace(
                        _nodeAnchorCards->convertToWorldSpace(GameUtils::getPositionAccordingAngle(angle, _r)));
                if (isConvertWorldSpace)
//                    position = this->convertWorldPosToLayoutRootPos(_layoutCards->convertToWorldSpace(position));
                    position = _layoutCards->convertToWorldSpace(position);

                position += CURVE_CARD_LAYOUT_POSITION_OFFSET;

                listPropertyCard.push_back(
                        {position, position + GameUtils::getPositionAccordingAngle(angle, DISTANCE_WAS_SELECTED_CARD),
                         angle, i, CARD_IN_HAND_SCALE});
            } else
            {
                float angle = 0 + (_angleBetweenTwoCards / 2 + (i - numberCard / 2) * _angleBetweenTwoCards);
                Point position = _layoutCards->convertToNodeSpace(
                        _nodeAnchorCards->convertToWorldSpace(GameUtils::getPositionAccordingAngle(angle, _r)));
                if (isConvertWorldSpace)
//                    position = this->convertWorldPosToLayoutRootPos(_layoutCards->convertToWorldSpace(position));
                    position = _layoutCards->convertToWorldSpace(position);

                position += CURVE_CARD_LAYOUT_POSITION_OFFSET;

                listPropertyCard.push_back(
                        {position, position + GameUtils::getPositionAccordingAngle(angle, DISTANCE_WAS_SELECTED_CARD),
                         angle, i, CARD_IN_HAND_SCALE});
            }
        }
    } else
    {
        for (int i = 0; i < numberCard; i++)
        {
            if (i < numberCard / 2)
            {
                float angle = 0 - ((numberCard / 2 - i) * _angleBetweenTwoCards);
                Point position = _layoutCards->convertToNodeSpace(
                        _nodeAnchorCards->convertToWorldSpace(GameUtils::getPositionAccordingAngle(angle, _r)));
                if (isConvertWorldSpace)
//                    position = this->convertWorldPosToLayoutRootPos(_layoutCards->convertToWorldSpace(position));
                    position = _layoutCards->convertToWorldSpace(position);

                position += CURVE_CARD_LAYOUT_POSITION_OFFSET;

                listPropertyCard.push_back(
                        {position, position + GameUtils::getPositionAccordingAngle(angle, DISTANCE_WAS_SELECTED_CARD),
                         angle, i, CARD_IN_HAND_SCALE});
            } else
            {
                float angle = 0 + ((i - numberCard / 2) * _angleBetweenTwoCards);
                Point position = _layoutCards->convertToNodeSpace(
                        _nodeAnchorCards->convertToWorldSpace(GameUtils::getPositionAccordingAngle(angle, _r)));
                if (isConvertWorldSpace)
//                    position = this->convertWorldPosToLayoutRootPos(_layoutCards->convertToWorldSpace(position));
                    position = _layoutCards->convertToWorldSpace(position);

                position += CURVE_CARD_LAYOUT_POSITION_OFFSET;

                listPropertyCard.push_back(
                        {position, position + GameUtils::getPositionAccordingAngle(angle, DISTANCE_WAS_SELECTED_CARD),
                         angle, i, CARD_IN_HAND_SCALE});
            }
        }
    }
    return listPropertyCard;
}

std::vector<std::vector<GinCardProperty>>
GPGinControlCardLayer::listPositionCardsOnHandAccordingNumberCardsOfAllPlayers(
        const std::vector<std::vector<int>> &listDealCard)
{
    std::vector<std::vector<GinCardProperty>> listGinCardPropertyAllPlayer;
    int i = 0;
    for (auto const &list: listDealCard)
    {
        if (list.size() != 0)
        {
            if (i == MAIN_USER_SLOT_INDEX)
            {
                if (STYLE_CARDS_ON_HAND == STYLE_CARDS_ON_HAND_STRAIGHT)
                    listGinCardPropertyAllPlayer.push_back(
                            this->listPositionCardsOnHandAccordingNumberCards_Straight(list.size(), true));
                else
                    listGinCardPropertyAllPlayer.push_back(
                            this->listPositionCardsOnHandAccordingNumberCards_Curve(list.size(), true));
            } else
            {
                if (i == 1)
                    listGinCardPropertyAllPlayer.push_back(
                            this->listPositionCardsOnHandAccordingNumberCardsOtherPlayer_Straight(i,
                                                                                                  list.size(),
                                                                                                  false,
                                                                                                  _listDealCardPos[i],
                                                                                                  TypeArrangeCardOnHandOtherPlayer::TYPE_ARRANGE_ON_HAND));
                else if (i == 2)
                    listGinCardPropertyAllPlayer.push_back(
                            this->listPositionCardsOnHandAccordingNumberCardsOtherPlayer_Straight(i,
                                                                                                  list.size(),
                                                                                                  true,
                                                                                                  _listDealCardPos[i],
                                                                                                  TypeArrangeCardOnHandOtherPlayer::TYPE_ARRANGE_ON_HAND));
            }
        } else
        {
            listGinCardPropertyAllPlayer.push_back(std::vector<GinCardProperty>());
        }
        i++;
    }

    return listGinCardPropertyAllPlayer;
}


std::vector<GinCardProperty>
GPGinControlCardLayer::listPositionCardsOnHandAccordingNumberCardsOtherPlayer_Straight(int orderIdx,
                                                                                       int numberCard,
                                                                                       bool directionRight,
                                                                                       const cocos2d::Vec2 &position,
                                                                                       TypeArrangeCardOnHandOtherPlayer type)
{
    if (type == TypeArrangeCardOnHandOtherPlayer::TYPE_ARRANGE_ON_HAND)
    {
        std::vector<GinCardProperty> listPropertyCard;
        float width = GIN_WIDTH_CARD * CARD_IN_HAND_OTHER_PLAYER_SCALE +
                      (numberCard - 1) * DISTANCE_BETWEEN_TWO_CARD_OTHER_PLAYER;
        for (int i = 0; i < numberCard; i++)
        {
            float x = position.x + i * DISTANCE_BETWEEN_TWO_CARD_OTHER_PLAYER +
                      (-width * 0.5f + GIN_WIDTH_CARD * CARD_IN_HAND_OTHER_PLAYER_SCALE * 0.5f);
            float y = position.y;
            Point position2 = Point(x, y);
            float angle = 0;
            float zOder = (int) ZorderControlLayer::CARD_IN_HAND + i;
            GinCardProperty tlCardProperty = GinCardProperty{position2, Vec2::ZERO, angle, (int) zOder,
                                                             CARD_IN_HAND_OTHER_PLAYER_SCALE};
            listPropertyCard.push_back(tlCardProperty);
        }
        return listPropertyCard;
    } else if (type == TypeArrangeCardOnHandOtherPlayer::TYPE_ARRANGE_ON_TABLE)
    {
        std::vector<GinCardProperty> listPropertyCard;
        for (int i = 0; i < numberCard; i++)
        {
            float x = (directionRight ? position.x + i * DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 : position.x - i *
                                                                                                             DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1);
            float y = position.y;
            Point position2 = Point(x, y);
            float angle = 0;
            float zOder = directionRight ? (int) ZorderControlLayer::CARD_IN_HAND + i :
                          (int) ZorderControlLayer::CARD_IN_HAND - i;
            GinCardProperty tlCardProperty = GinCardProperty{position2, Vec2::ZERO, angle, (int) zOder,
                                                             CARD_ON_TABLE_SCALE_1};
            listPropertyCard.push_back(tlCardProperty);
        }
        return listPropertyCard;
    }
    return std::vector<GinCardProperty>();
}

std::vector<GinCardProperty>
GPGinControlCardLayer::listPositionCardsOnHandAccordingNumberCardsOtherPlayer(int numberCard, int orderIdx,
                                                                              TypeArrangeCardOnHandOtherPlayer type /*= 0*/)
{
    if (orderIdx >= 1 && orderIdx <= 2)
    {
        if (orderIdx == 1)
            return this->listPositionCardsOnHandAccordingNumberCardsOtherPlayer_Straight(orderIdx, numberCard, false,
                                                                                         _gameGin->getHandCardPos()[orderIdx],
                                                                                         type);
        else if (orderIdx == 2)
            return this->listPositionCardsOnHandAccordingNumberCardsOtherPlayer_Straight(orderIdx, numberCard, true,
                                                                                         _gameGin->getHandCardPos()[orderIdx],
                                                                                         type);
    }
    return std::vector<GinCardProperty>();
}

void GPGinControlCardLayer::showCardsOnHand(const std::vector<GinCard *> &listHandCard, int orderIdx)
{
    if (orderIdx == MAIN_USER_SLOT_INDEX)
    {
        this->calculateScoreCardOnHandBySoftGroup();
        const std::vector<GinCardProperty> &listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
                listHandCard.size());
        for (int i = 0; i < listHandCard.size() && i < listPropertyCard.size(); i++)
        {
            const GinCardProperty &properties = listPropertyCard.at(i);

            GinCard *tlCard = listHandCard[i];
            tlCard->setProperty(properties);
            tlCard->setLocalZOrder(properties._zOder);
            tlCard->setPosition(properties._positionOnHand);
            tlCard->setRotation(properties._angle);
            tlCard->setScale(properties._scale);
        }
        std::vector<GinCard *> cards = listHandCard;
        this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([=]()
                                                                                   {
                                                                                       for (int i = 0;
                                                                                            i < cards.size(); i++)
                                                                                       {
                                                                                           GinCard *card = cards[i];
                                                                                           if (checkValidCardsOnHand(
                                                                                                   card))
                                                                                               card->cardShowOnHand(i *
                                                                                                                    TIME_MOVE_BETWEEEN_TWO_CARDS *
                                                                                                                    0.4f);
                                                                                       }
                                                                                   }), DelayTime::create(0.8f),
                                         CallFunc::create([=]()
                                                          {
                                                              this->onRequestSortCard(
                                                                      _listSortMethod[_currentSortMethodIndex].first);
                                                          }), DelayTime::create(0.4f), CallFunc::create([=]()
                                                                                                        {
                                                                                                            _cardsIsOnHand = true;
                                                                                                            //            highlightPhom(this->getPhom(_listHandCardsMainUser)._phom, false);
                                                                                                        }), nullptr));
    } else
    {
        const std::vector<GinCardProperty> &listPropertyCard = this->listPositionCardsOnHandAccordingNumberCardsOtherPlayer(
                listHandCard.size(),
                orderIdx,
                TypeArrangeCardOnHandOtherPlayer::TYPE_ARRANGE_ON_HAND);
        for (int i = 0; i < listHandCard.size() && i < listPropertyCard.size(); i++)
        {
            GinCard *tlCard = listHandCard[i];
            if (checkValidCards(listHandCard, tlCard))
            {
                const GinCardProperty &properties = listPropertyCard.at(i);
                tlCard->setProperty(properties);
                tlCard->setLocalZOrder(properties._zOder);
                tlCard->setPosition(properties._positionOnHand);
                tlCard->setRotation(properties._angle);
                tlCard->setScale(properties._scale);
                tlCard->setOpacity(0);
            }
        }
        if (orderIdx >= 0 && orderIdx < _listUserHandNode.size())
            _listUserHandNode.at(orderIdx)->showHand(listHandCard.size());
    }
}

void GPGinControlCardLayer::arrangeCardOnHand(TypeArrangeCardOnHand type, int posStart/*= -1*/, int posEnd/*= -1*/,
                                              int numberCardGap /*= 0*/)
{
    switch (type)
    {
        case TypeArrangeCardOnHand::TYPE_ARRANGE_IN_START_END:
        {
            if (posStart >= 0 && posStart < _listHandCardsMainUser.size() && posEnd >= 0 &&
                posEnd < _listHandCardsMainUser.size())
            {
                std::vector<GinCard *> listCardTemp;
                GinCard *startCard = _listHandCardsMainUser[posStart];
                GinCard *endCard = _listHandCardsMainUser[posEnd];
                if (posStart < posEnd)
                {
                    for (int i = 0; i <= numberCardGap; i++)
                        listCardTemp.push_back(_listHandCardsMainUser[posEnd - i]);

                    for (int i = posEnd; i > posStart + numberCardGap; i--)
                        _listHandCardsMainUser[i] = _listHandCardsMainUser[i - 1 - numberCardGap];

                    for (int i = 0; i < listCardTemp.size(); i++)
                        _listHandCardsMainUser[posStart + numberCardGap - i] = listCardTemp[i];
                } else
                {
                    for (int i = 0; i <= numberCardGap; i++)
                        listCardTemp.push_back(_listHandCardsMainUser[posEnd + i]);

                    for (int i = posEnd; i < posStart - numberCardGap; i++)
                        _listHandCardsMainUser[i] = _listHandCardsMainUser[i + 1 + numberCardGap];

                    for (int i = 0; i < listCardTemp.size(); i++)
                        _listHandCardsMainUser[posStart - numberCardGap + i] = listCardTemp[i];
                }
                this->checkUpdateGroup(startCard, endCard, numberCardGap);
                this->playEffectSortCardsOnHand();
            }
        }
            break;
        case TypeArrangeCardOnHand::TYPE_ARRANGE_IN_START_END_MULTI:
        {
            std::vector<GinCard *> listCardDrag;
            int i, j;
            int saveI = -1;
            int saveJ;
            if (posStart < posEnd)
            {
                for (i = posEnd; i > posStart; i--)
                    if (_listHandCardsMainUser[i]->getState() == GinCardState::GinCARDSTATE_DRAG)
                        listCardDrag.push_back(_listHandCardsMainUser[i]);
                saveJ = posEnd - 1;
                for (i = posEnd; i > posStart + listCardDrag.size(); i--)
                {
                    for (j = saveJ; j > posStart; j--)
                    {
                        if (_listHandCardsMainUser[j]->getState() == GinCardState::GinCARDSTATE_DRAG)
                            continue;
                        saveI = i;
                        saveJ = j - 1;
                        _listHandCardsMainUser[i] = _listHandCardsMainUser[j];
                        break;
                    }
                }
                if (saveI != -1)
                    for (i = 0; i < listCardDrag.size(); i++)
                        _listHandCardsMainUser[saveI - 1 - i] = listCardDrag[i];
            } else
            {
                for (i = posEnd; i < posStart; i++)
                    if (_listHandCardsMainUser[i]->getState() == GinCardState::GinCARDSTATE_DRAG)
                        listCardDrag.push_back(_listHandCardsMainUser[i]);
                saveJ = posEnd + 1;
                for (i = posEnd; i < posStart - listCardDrag.size(); i++)
                {
                    for (j = saveJ; j < posStart; j++)
                    {
                        if (_listHandCardsMainUser[j]->getState() == GinCardState::GinCARDSTATE_DRAG)
                            continue;
                        saveI = i;
                        saveJ = j + 1;
                        _listHandCardsMainUser[i] = _listHandCardsMainUser[j];
                        break;
                    }
                }
                if (saveI != -1)
                    for (i = 0; i < listCardDrag.size(); i++)
                        _listHandCardsMainUser[saveI + 1 + i] = listCardDrag[i];
            }
            this->playEffectSortCardsOnHand();
            break;
        }
        case TypeArrangeCardOnHand::TYPE_ARRANGE_INCREASE:
        case TypeArrangeCardOnHand::TYPE_ARRANGE_SUIT:
        case TypeArrangeCardOnHand::TYPE_ARRANGE_COMBINATION:
        {
            //            this->arrangeCardOnHandBySortingGroupAndTrashCard();
            this->softArrangeCardOnHandPhom();
            this->playEffectSortCardsOnHand();
            break;
        }
        case TypeArrangeCardOnHand::TYPE_ARRANGE_MELD_CARD:
        {
            //            this->arrangeCardOnHandPhom();
            this->softArrangeCardOnHandPhom();
            this->playEffectSortCardsOnHand();
            for (auto const &group: _listGroupHandCardsMainUser)
                this->animationShowGroup(group);
            break;
        }
        case TypeArrangeCardOnHand::TYPE_ARRANGE_GROUP_CARD:
        {
            this->arrangeCardOnHandBySortingGroupAtTheBeginning();
            this->playEffectSortCardsOnHand();
            break;
        }
        case TypeArrangeCardOnHand::TYPE_ARRANGE_GROUP_CARD_MULTI:
        {
            GroupCard currentGroup = _listGroupHandCardsMainUser[posEnd];
            if (posStart < posEnd)
            {
                for (int i = posEnd; i > posStart; i--)
                    _listGroupHandCardsMainUser[i] = _listGroupHandCardsMainUser[i - 1];
                _listGroupHandCardsMainUser[posStart] = currentGroup;
            } else
            {
                for (int i = posEnd; i < posStart; i++)
                    _listGroupHandCardsMainUser[i] = _listGroupHandCardsMainUser[i + 1];
                _listGroupHandCardsMainUser[posStart] = currentGroup;
            }
            this->arrangeCardOnHandBySortingGroupAtTheBeginning();
            this->playEffectSortCardsOnHand();
            break;
        }
        default:
            break;
    }
}

int GPGinControlCardLayer::getIndexCardOnHand(GinBaseCard *card)
{
    for (int i = 0; i < _listHandCardsMainUser.size(); i++)
        if (_listHandCardsMainUser[i] == card)
            return i;
    return -1;
}

bool GPGinControlCardLayer::enableTouchCard()
{
    for (auto const &card: _listHandCardsMainUser)
        if (card->getTouch())
            return false;
    return _canTouchCard;
}

void GPGinControlCardLayer::changeLanguage(Language language)
{
    ((Text *) _buttonSort->getChildByName("TextBMButton"))->setString(GET_TEXT_BY_ID(TXT_BUTTON_ARRANGE_CARD));
    ((Text *) _buttonFindMeld->getChildByName("TextBMButton"))->setString(GET_TEXT_BY_ID(TXT_BUTTON_AUTO_MELD));
    ((Text *) _buttonGroup->getChildByName("TextBMButton"))->setString(GET_TEXT_BY_ID(TXT_BUTTON_GROUP_CARD));
    ((Text *) _buttonUngroup->getChildByName("TextBMButton"))->setString(GET_TEXT_BY_ID(TXT_BUTTON_UNGROUP_CARD));
}

void GPGinControlCardLayer::setVisibleButtonXep(bool isVisible)
{
    _buttonFindMeld->setVisible(isVisible);
    //_buttonSort->setVisible(isVisible);
    _buttonSort->setVisible(false);
}

void GPGinControlCardLayer::setVisibleButtonShowThrowCards(bool isVisible)
{
    if (isVisible)
    {
        if (!_buttonShowThrowCards->isVisible())
        {
            _buttonShowThrowCards->setVisible(true);
            _buttonShowThrowCards->setOpacity(0);
            _buttonShowThrowCards->runAction(
                    Sequence::create(DelayTime::create(TIME_CARD_THROW), FadeTo::create(0.01f, 0.1f * 255), nullptr));
        }
    } else
    {
        _buttonShowThrowCards->stopAllActions();
        _buttonShowThrowCards->setVisible(false);
    }
}

void GPGinControlCardLayer::showNotifyWrong(const std::string &content)
{
    _layoutNotify->retain();
    _layoutNotify->setPosition(_gameGin->convertToNodeSpace(
            _layoutNotify->getParent()->convertToWorldSpace(_layoutNotify->getPosition())));
    _layoutNotify->removeFromParent();
    _layoutNotify->stopAllActions();
    _layoutNotify->setVisible(true);
    _layoutNotify->setOpacity(0);

    Text *text = (Text *) Helper::seekWidgetByName(_layoutNotify, "TextNotify");
    text->setString(content);

    _layoutNotify->setContentSize(Size(text->getContentSize().width + 200, _layoutNotify->getContentSize().height));
    Helper::doLayout(_layoutNotify);

    _layoutNotify->runAction(
            Sequence::create(FadeIn::create(0.05f), DelayTime::create(2.0f), FadeOut::create(0.05f), Hide::create(),
                             nullptr));
    _gameGin->addChild(_layoutNotify, kForeground);
    _layoutNotify->release();
}

bool GPGinControlCardLayer::Doi(int clientId1, int clientId2)
{
    return clientId1 / 4 == clientId2 / 4;
}

bool GPGinControlCardLayer::Ca(int clientId1, int clientId2)
{
    int dis = abs(clientId1 - clientId2);
    return dis == 4 || dis == 8;
}

bool GPGinControlCardLayer::LienTuc(int clientId1, int clientId2)
{
    return abs(clientId1 - clientId2) == 4;
}

int GPGinControlCardLayer::CalScore(const CardInHand &cih)
{
    int score = 0;
    for (const int &card: cih._trash)
        score += (int) card / 4 + 1;
    return score;
}

bool GPGinControlCardLayer::PhomIsValid(const CardInHand &cih)
{
    int count_in_take = 0;
    for (auto const &phom: cih._phom)
    {
        int count = 0;
        for (auto card: phom)
            if (checkCardInListTakedCard(card))
                count += 1;
        if (count == 2 && phom.size() < 6)
            return false;
        else if (count == 3 && phom.size() < 9)
            return false;
        count_in_take += count;
    }
    if (count_in_take < _listTakeCard.at(0).size())
        return false;
    return true;
}

bool GPGinControlCardLayer::MeldIsValid(const std::vector<GinCard *> &listCard, bool isStrictMode)
{
    std::vector<int> listCardInt;
    for (auto const &card: listCard)
        if (card != nullptr)
            listCardInt.push_back(card->getCardIndex());
    return this->MeldIsValid(listCardInt, isStrictMode);
}

bool GPGinControlCardLayer::MeldIsValid(std::vector<int> listCard, bool isStrictMode)
{
    if (listCard.size() < 3)
        return false;

    if (isStrictMode == false)
    {
        std::sort(listCard.begin(), listCard.end());
    }

    int v1 = listCard.at(0);
    int v2 = listCard.at(1);
    if (v1 / MAXIMUM_NUMBER_SUIT == v2 / MAXIMUM_NUMBER_SUIT)
    {
        for (int i = 2; i < listCard.size(); i++)
            if (listCard.at(i) / MAXIMUM_NUMBER_SUIT != v1 / MAXIMUM_NUMBER_SUIT)
                return false;
        return true;
    } else
    {
        for (int i = 1; i < listCard.size(); i++)
            if ((listCard.at(i) / MAXIMUM_NUMBER_SUIT != (v1 / MAXIMUM_NUMBER_SUIT + i)) ||
                ((listCard.at(i) % MAXIMUM_NUMBER_SUIT) != (v1 % MAXIMUM_NUMBER_SUIT)))
                return false;
        return true;
    }
}

CardInHand GPGinControlCardLayer::getPhom(const std::vector<GinCard *> &list)
{
    std::vector<int> list_hand;
    std::vector<int> list_take;
    std::vector<int> list_int;
    for (auto card: list)
    {
        if (card->getHighLight())
            list_take.push_back(card->getCardIndex());
        else
            list_hand.push_back(card->getCardIndex());
        list_int.push_back(card->getCardIndex());
    }
    std::vector<std::vector<int>> list_phom = RuleShowCard::suggest(list_hand, list_take);
    for (int i = 0; i < list_phom.size(); i++)
    {
        for (int j = 0; j < list_phom[i].size(); j++)
        {
            auto it = std::find(list_int.begin(), list_int.end(), list_phom[i][j]);
            if (it != list_int.end())
                list_int.erase(std::remove(list_int.begin(), list_int.end(), list_phom[i][j]), list_int.end());
        }
    }
    CardInHand cih = getPhom(list_int);
    for (int i = 0; i < list_phom.size(); i++)
    {
        sortIncrease(list_phom[i]);
        cih._phom.push_back(list_phom[i]);
    }
    return cih;
}

CardInHand GPGinControlCardLayer::getPhom(const std::vector<int> &list_int)
{
    ListOverlap overlap = getPhomOverlap(list_int);
    if (overlap._overlap == false)
    {
        CardInHand cih;
        for (auto phom: overlap._phom)
            cih._phom.push_back(phom);
        cih._trash = getAllTrash(list_int, cih._phom);
        return cih;
    } else
    {
        vector<CardInHand> list_case;
        for (auto phom: overlap._phom)
        {
            CardInHand cih;
            cih._phom.push_back(phom);

            vector<vector<int>> list_phom;
            list_phom.push_back(phom);
            cih._trash = getAllTrash(list_int, list_phom);

            list_case.push_back(cih);
        }

        for (int t = 0; t < list_case.size(); t++)
        {
            ListOverlap olap = getPhomOverlap(list_case.at(t)._trash);
            if (olap._overlap == false)
            {
                for (auto phom: olap._phom)
                    list_case.at(t)._phom.push_back(phom);
                list_case.at(t)._trash = getAllTrash(list_int, list_case.at(t)._phom);
            } else
            {
                // tao them case moi.
                for (int i = 1; i < olap._phom.size(); i++)
                {
                    CardInHand cih;
                    cih._phom = list_case.at(t)._phom;
                    cih._phom.push_back(olap._phom.at(i));
                    cih._trash = getAllTrash(list_int, cih._phom);

                    list_case.push_back(cih);
                    int y;
                }

                list_case.at(t)._phom.push_back(olap._phom.at(0));
                list_case.at(t)._trash = getAllTrash(list_int, list_case.at(t)._phom);
                int t;
            }
        }

        int min = 10000;
        CardInHand cihMin;
        for (auto Case: list_case)
        {
            int score = CalScore(Case);
            if (score < min && PhomIsValid(Case))
            {
                cihMin = Case;
                min = score;
            } else if (score == min && PhomIsValid(Case))
            {
                int count_1 = 0;
                for (auto phom: Case._phom)
                {
                    if (getPhomType(phom) == MeldType::MELD_SANH)
                        count_1 += 1;
                }
                int count_2 = 0;
                for (auto phom: cihMin._phom)
                {
                    if (getPhomType(phom) == MeldType::MELD_SANH)
                        count_2 += 1;
                }
                if (count_1 > count_2)
                    cihMin = Case;
            }
        }

        return cihMin;
    }
}

ListOverlap GPGinControlCardLayer::getPhomOverlap(const std::vector<int> &list)
{
    std::vector<std::vector<int>> phomOverLap;
    int a[13][4];
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
            a[i][j] = 0;
    for (int i = 0; i < list.size(); i++)
    {
        int value = list.at(i);
        int r = value / 4;
        int c = value % 4;
        a[r][c] = 1;
    }
    vector<vector<int>> listPhomSanh;
    for (int i = 0; i < 4; i++)
    {
        if (calCol(a, 13, 4, i) >= 3)
        {
            vector<int> phom;
            for (int j = 0; j < 13; j++)
            {
                if (a[j][i] == 1)
                {
                    int value = j * 4 + i;
                    if (phom.empty())
                    {
                        phom.push_back(value);
                    } else
                    {
                        if (LienTuc(value, phom.at(phom.size() - 1)))
                        {
                            phom.push_back(value);
                        } else
                        {
                            if (phom.size() >= 3)
                            {
                                listPhomSanh.push_back(phom);
                                phom.clear();
                                phom.push_back(value);
                            } else
                            {
                                phom.clear();
                                phom.push_back(value);
                            }
                        }
                    }
                }
            }
            if (phom.size() >= 3)
                listPhomSanh.push_back(phom);
        }
    }
    vector<int> list_over_lap;
    vector<vector<int>> listPhomBaLa;
    for (int i = 0; i < 13; i++)
    {
        if (calRow(a, 13, 4, i) >= 3)
        {
            vector<int> phom;
            bool overLap = false;
            for (int j = 0; j < 4; j++)
            {
                if (a[i][j] == 1)
                {
                    int value = i * 4 + j;
                    phom.push_back(value);
                    Point pt = checkIn(listPhomSanh, value);
                    if (pt.x != -1)
                    {
                        list_over_lap.push_back(pt.x);
                        overLap = true;
                    }
                }
            }
            if (overLap)
                phomOverLap.push_back(phom);
            else
                listPhomBaLa.push_back(phom);
        }
    }
    ListOverlap result;
    if (phomOverLap.empty())
    {
        result._overlap = false;
        for (auto const &phom: listPhomBaLa)
            listPhomSanh.push_back(phom);
        result._phom = listPhomSanh;
        return result;
    } else
    {
        vector<vector<int>> temp = listPhomSanh;
        for (int i = 0; i < list_over_lap.size(); i++)
        {
            if (temp.at(list_over_lap.at(i)).empty() == false)
            {
                phomOverLap.push_back(temp.at(list_over_lap.at(i)));
                temp.at(list_over_lap.at(i)).clear();
            }
        }

        result._overlap = true;
        result._phom = phomOverLap;
        return result;
    }
}

std::vector<int>
GPGinControlCardLayer::getAllTrash(const std::vector<int> &allCard, const std::vector<std::vector<int>> &list_phom)
{
    std::vector<int> trash;
    for (auto const &card: allCard)
    {
        bool have = false;
        for (auto const &phom: list_phom)
        {
            for (auto const &index: phom)
            {
                if (index == card)
                {
                    have = true;
                    break;
                }
            }
            if (have)
                break;
        }
        if (have == false)
            trash.push_back(card);
    }
    return trash;
}

std::vector<GinCard *> GPGinControlCardLayer::getTrashCards()
{
    std::vector<GinCard *> listCards;
    for (auto const &card: _listHandCardsMainUser)
        if (this->checkCardInAGroup(card->getCardIndex()) == -1)
            listCards.push_back(card);
    return listCards;
}

std::vector<std::vector<int>>
GPGinControlCardLayer::getPhomContainCard(const std::vector<GinCard *> &listCard, int card)
{
    std::vector<std::vector<int>> listPhom;
    int a[13][4];
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
            a[i][j] = 0;
    for (auto const &i: listCard)
    {
        int value = i->getCardIndex();
        int r = value / 4;
        int c = value % 4;
        a[r][c] = 1;
    }
    int rowCard = card / 4;
    int colCard = card % 4;
    if (calCol(a, 13, 4, colCard) >= 3)
    {
        vector<int> phom;
        for (int i = 0; i < 13; i++)
        {
            if (a[i][colCard] == 1)
            {
                int value = i * 4 + colCard;
                if (phom.empty())
                {
                    phom.push_back(value);
                } else
                {
                    if (LienTuc(value, phom.at(phom.size() - 1)))
                    {
                        phom.push_back(value);
                    } else
                    {
                        if (phom.size() >= 3)
                        {
                            // Check phom whether contain card
                            for (const int &j: phom)
                            {
                                if (j == card)
                                {
                                    listPhom.push_back(phom);
                                    break;
                                }
                            }
                            phom.clear();
                            phom.push_back(value);
                        } else
                        {
                            phom.clear();
                            phom.push_back(value);
                        }
                    }
                }
            }
        }
        if (phom.size() >= 3)
        {
            for (const int &j: phom)
            {
                if (j == card)
                {
                    listPhom.push_back(phom);
                    break;
                }
            }
        }
    }
    if (calRow(a, 13, 4, rowCard) >= 3)
    {
        vector<int> phom;
        for (int i = 0; i < 4; i++)
        {
            if (a[rowCard][i] == 1)
            {
                int value = rowCard * 4 + i;
                phom.push_back(value);
            }
        }
        listPhom.push_back(phom);
    }
    return listPhom;
}

std::vector<GinCard *> GPGinControlCardLayer::getListSelectedCardOnHand()
{
    std::vector<GinCard *> listSelectedCards;
    for (int i = 0; i < _listHandCardsMainUser.size(); i++)
    {
        if (_listHandCardsMainUser[i]->getState() == GinCardState::GinCARDSTATE_SELECTED)
        {
            listSelectedCards.push_back(_listHandCardsMainUser[i]);
        }
    }
    return listSelectedCards;
}

std::vector<int> GPGinControlCardLayer::getListSelectedCardValueOnHand()
{
    std::vector<int> listSelectedCards;
    for (int i = 0; i < _listHandCardsMainUser.size(); i++)
    {
        if (_listHandCardsMainUser[i]->getState() == GinCardState::GinCARDSTATE_SELECTED)
        {
            listSelectedCards.push_back(_listHandCardsMainUser[i]->getCardValue());
        }
    }
    return listSelectedCards;
}

std::vector<GinCard *> GPGinControlCardLayer::getListSelectedCardOnHandInGroup()
{
    std::vector<GinCard *> listSelectedCards;
    for (int i = 0; i < _listGroupHandCardsMainUser.size(); i++)
    {
        for (int j = 0; j < _listGroupHandCardsMainUser.at(i)._listCards.size(); j++)
        {
            if (_listGroupHandCardsMainUser.at(i)._listCards.at(j)->getState() == GinCardState::GinCARDSTATE_SELECTED)
            {
                listSelectedCards.push_back(_listGroupHandCardsMainUser.at(i)._listCards.at(j));
            }
        }
    }
    return listSelectedCards;
}

// Get list trash cards on hand which aren't belong to any group (valid and invalid group)
std::vector<GinCard *> GPGinControlCardLayer::getListTrashCardsOnHand()
{
    std::vector<GinCard *> listTrashCards;
    for (auto const &card: _listHandCardsMainUser)
    {
        bool isInGroup = false;
        for (auto const &group: _listGroupHandCardsMainUser)
        {
            for (auto const &k: group._listCards)
            {
                if (card->getCardIndex() == k->getCardIndex())
                {
                    isInGroup = true;
                    break;
                }
            }
            if (isInGroup)
                break;
        }
        if (!isInGroup)
            listTrashCards.push_back(card);
    }
    return listTrashCards;
}

// Check whether card is selected on list card hand
bool GPGinControlCardLayer::checkHaveSelectedCardOnListHandCards(const std::vector<GinCard *> &listHandCards)
{
    for (int i = 0; i < listHandCards.size(); i++)
    {
        if (listHandCards[i]->getState() == GinCardState::GinCARDSTATE_SELECTED)
        {
            return true;
        }
    }
    return false;
}

// Check wheter this card is selected on list card hand
bool GPGinControlCardLayer::checkCardIsSlectedOnListHandCards(int valueCard)
{
    for (int i = 0; i < _listHandCardsMainUser.size(); i++)
    {
        if (_listHandCardsMainUser[i]->getCardValue() == valueCard &&
            _listHandCardsMainUser[i]->getState() == GinCardState::GinCARDSTATE_SELECTED)
        {
            return true;
        }
    }
    return false;
}

void GPGinControlCardLayer::checkStateOfGroupAndUngroupButton()
{
    if (_status != ControlStatus::CONTROL_STATUS_MATCH_END && _listHandCardsMainUser.size() > 0)
    {
        std::vector<GinCard *> listSelectedCard = this->getListSelectedCardOnHand();
        if (listSelectedCard.size() >= 2)
        {
            bool allSelectedCardInGroup = this->checkListCardInAGroup(listSelectedCard) >= 0;
            bool allSelectedCardOutGroup = this->checkEachCardDontInAnyGroup(listSelectedCard);
            if (allSelectedCardInGroup)
                showUngroupButton(true, listSelectedCard);
            else if (allSelectedCardOutGroup)
                showGroupButton(true, listSelectedCard);
            else
                showGroupButton(true, listSelectedCard);
        } else
        {
            showGroupButton(false, listSelectedCard);
            showUngroupButton(false, listSelectedCard);
        }
    } else
    {
        showGroupButton(false, _listHandCardsMainUser);
        showUngroupButton(false, _listHandCardsMainUser);
    }
}

bool GPGinControlCardLayer::checkStateDumpCardButton()
{
    if (_status == ControlStatus::CONTROL_STATUS_DUMP)
    {
        std::vector<GinCard *> listSelectedCard = this->getListSelectedCardOnHand();
        if (listSelectedCard.size() == 1)
        {
            _buttonDump->setVisible(true);
            if (true)
            {
//                Size                  sizeHintDump;
//                Vec2                  posHintDump;
//                int                   zOrderHintDump;
//                std::vector<GinCard*> listThrowCards = this->getListThrowCards();
//                if (listThrowCards.size() == 0)
//                {
//                    sizeHintDump   = _buttonTake->getContentSize() + Size(5, 5);
//                    posHintDump    = _buttonTake->getPosition() + Vec2(1, 0);
//                    zOrderHintDump = 0;
//                }
//                else
//                {
//                    auto cardEnd = listThrowCards.back();
//                    sizeHintDump   = cardEnd->getContentSize() * cardEnd->getScale() + Size(5, 5);
//                    posHintDump    = cardEnd->getPosition() + Vec2(1, 0);
//                    zOrderHintDump = cardEnd->getLocalZOrder() - 1;
//                }
//                showEffectHighlight("DumpCard", _layoutRoot, sizeHintDump, _layoutRoot->convertToWorldSpace(posHintDump), zOrderHintDump + 2);

                HintDropPlaceType type = HintDropPlaceType::HintDropPlaceAtThrowCardsArea;
                std::string name = GameUtils::getHintDropPlaceTypeName(type);

                showEffectHighlight("DumpCard",
                                    _layoutRoot,
                                    _listHintDropArea[type].first.size,
                                    this->_layoutRoot->convertToNodeSpace(_listHintDropArea[type].first.origin) +
                                    _listHintDropArea[type].first.size / 2,
                                    (int) ZorderControlLayer::EFFECT_HINT);
            }
            return true;
        } else
        {
            _buttonDump->setVisible(false);
            removeEffectHighlight("DumpCard");
            return false;
        }
    }
    return false;
}

void GPGinControlCardLayer::checkAutoSelectCardIngroup(int cardValue)
{
    int groupIndex = this->checkCardInAGroup(cardValue);
    if (groupIndex >= 0 && groupIndex < _listGroupHandCardsMainUser.size() &&
        _listGroupHandCardsMainUser.at(groupIndex)._listCards.size() > 0)
    {
        const std::vector<GinCard *> &groupCard = _listGroupHandCardsMainUser.at(groupIndex)._listCards;
        for (auto const &card: groupCard)
            if (card->getCardIndex() != cardValue)
                card->selected(true);
        for (auto const &card: _listHandCardsMainUser)
            if (std::find(groupCard.begin(), groupCard.end(), card) == groupCard.end())
                card->unSelected(true);
    } else
    {
        for (auto const &group: _listGroupHandCardsMainUser)
            for (auto const &card: group._listCards)
                card->unSelected(true);
    }
    this->checkStateDumpCardButton();
}

void GPGinControlCardLayer::checkAutoUnselectCardInGroup(int cardValue)
{
    int groupIndex = this->checkCardInAGroup(cardValue);
    if (groupIndex >= 0 && groupIndex < _listGroupHandCardsMainUser.size() &&
        _listGroupHandCardsMainUser.at(groupIndex)._listCards.size() > 0)
    {
        const std::vector<GinCard *> &groupCard = _listGroupHandCardsMainUser.at(groupIndex)._listCards;
        for (auto const &card: groupCard)
            if (card->getCardIndex() != cardValue)
                card->unSelected(true);
    }
    this->checkStateDumpCardButton();
}

// Check whether the card in specfic group
int GPGinControlCardLayer::checkCardInAGroup(int cardValue)
{
    int i = 0;
    for (auto const &group: _listGroupHandCardsMainUser)
    {
        const std::vector<GinCard *> &groupCard = group._listCards;
        for (auto const &card: groupCard)
            if (card->getCardIndex() == cardValue)
                return i;
        i++;
    }
    return -1;
}

// Check whether the card in specific soft group
int GPGinControlCardLayer::checkCardInASoftGroup(int cardValue)
{
    int i = 0;
    for (auto const &group: _listSoftGroupHandCardsMainUser)
    {
        const std::vector<GinCard *> &groupCard = group._listCard;
        for (auto const &card: groupCard)
            if (card->getCardIndex() == cardValue)
                return i;
        i++;
    }
    return -1;
}

std::vector<GinCard *> GPGinControlCardLayer::checkCardInAGroup(GinCard *card)
{
    int indexGroup = this->checkCardInAGroup(card->getCardIndex());
    if (indexGroup >= 0)
        return _listGroupHandCardsMainUser.at(indexGroup)._listCards;
    return std::vector<GinCard *>();
}

std::vector<GinCard *> GPGinControlCardLayer::checkCardInASoftGroup(GinCard *card)
{
    int indexGroup = this->checkCardInASoftGroup(card->getCardIndex());
    if (indexGroup >= 0)
        return _listSoftGroupHandCardsMainUser.at(indexGroup)._listCard;
    return std::vector<GinCard *>();
}

int GPGinControlCardLayer::checkListCardInAGroup(const std::vector<GinCard *> &listCards)
{
    return this->checkListCardInAGroup(this->convertListCardInt(listCards));
}

int GPGinControlCardLayer::checkListCardInAGroup(const std::vector<int> &listCards)
{
    if (listCards.size() >= 1)
    {
        int groupIndex = this->checkCardInAGroup(listCards.at(0));
        if (groupIndex >= 0 && groupIndex < _listGroupHandCardsMainUser.size())
        {
            for (int i = 1; i < listCards.size(); i++)
            {
                int groupIndextemp = this->checkCardInAGroup(listCards.at(i));
                if (groupIndextemp != groupIndex)
                    return -1;
            }
            return groupIndex;
        }
    }
    return -1;
}

bool GPGinControlCardLayer::checkEachCardDontInAnyGroup(const std::vector<GinCard *> &listCards)
{
    return this->checkEachCardDontInAnyGroup(this->convertListCardInt(listCards));
}

bool GPGinControlCardLayer::checkEachCardDontInAnyGroup(const std::vector<int> &listCards)
{
    for (const int &card: listCards)
    {
        int groupIndex = this->checkCardInAGroup(card);
        if (groupIndex >= 0)
            return false;
    }
    return true;
}

bool GPGinControlCardLayer::checkAllCadsOnHandInGroup()
{
    if (_listGroupHandCardsMainUser.size() == 0 || _listHandCardsMainUser.size() == 0)
        return false;
    for (auto const &card: _listHandCardsMainUser)
        if (this->checkCardInAGroup(card->getCardIndex()) < 0)
            return false;
    return true;
}

void GPGinControlCardLayer::setEnableButton(cocos2d::ui::Button *button, bool enable)
{
    button->setEnabled(enable);
    if (enable)
        button->setColor(Color3B::WHITE);
    else
        button->setColor(BUTTON_COLOR_DISABLE);
}

cocos2d::Vec2 GPGinControlCardLayer::getButtonPos(cocos2d::ui::Button *button)
{
    return button->getPosition();
}

GroupCard
GPGinControlCardLayer::addGroup(std::vector<GinCard *> listCard, bool animShowGroup, bool sortGroup/* = false*/,
                                int index/* = -1*/)
{
    GroupCard groupCard;
    if (listCard.size() >= 2)
    {
        sortIncrease(listCard);
        groupCard._listCards = listCard;
        updateGroup(groupCard);
        if (animShowGroup && groupCard._layoutMeld)
            groupCard._layoutMeld->setOpacity(0);
        if (sortGroup)
        {
            if (index >= 0)
            {
                while (index > _listGroupHandCardsMainUser.size())
                    index--;
                _listGroupHandCardsMainUser.insert(_listGroupHandCardsMainUser.begin() + index, groupCard);
            } else
            {
                int indexInsert = 0;
                for (int i = 0; i < _listGroupHandCardsMainUser.size(); i++)
                    if (MeldIsValid(_listGroupHandCardsMainUser.at(i)._listCards))
                        indexInsert = i + 1;
                _listGroupHandCardsMainUser.insert(_listGroupHandCardsMainUser.begin() + indexInsert, groupCard);
            }
        } else
        {
            _listGroupHandCardsMainUser.push_back(groupCard);
        }
    }
    return groupCard;
}

void GPGinControlCardLayer::animationShowGroup(const GroupCard &groupCard)
{
    if (groupCard._listCards.size() > 0 && checkValidMeldGroupLayout(groupCard._layoutMeld))
    {
        std::vector<GinCardProperty> listPropertyCard = this->listPositionCardsOnHandAccordingNumberCards(
                _listHandCardsMainUser.size());
        float maximumTimeMoving = 0;
        for (auto const &card1: groupCard._listCards)
        {
            for (int j = 0; j < _listHandCardsMainUser.size(); j++)
            {
                if (card1->getCardIndex() == _listHandCardsMainUser.at(j)->getCardIndex() &&
                    j < listPropertyCard.size())
                {
                    float timeMoving = card1->getPosition().distance(listPropertyCard.at(j)._positionOnHand) *
                                       TIME_ACTION_CARD_MOVE_ON_HAND_1PIXEL;
                    if (timeMoving > maximumTimeMoving)
                        maximumTimeMoving = timeMoving;
                    break;
                }
            }
        }
        if (groupCard._layoutMeld)
        {
            if (maximumTimeMoving >= 0.02f)
            {
                groupCard._layoutMeld->setOpacity(0);
                groupCard._layoutMeld->runAction(
                        Sequence::create(DelayTime::create(maximumTimeMoving), FadeIn::create(0.1f), nullptr));
            } else
            {
                groupCard._layoutMeld->setOpacity(255);
            }
        }
    }
}

void GPGinControlCardLayer::updateGroup(GroupCard &groupCard)
{
    int indexCard = 0;
    for (int i = 0; i < groupCard._listCards.size(); i++)
    {
        if (groupCard._listCards.at(i)->getState() != GinCardState::GinCARDSTATE_DRAG)
        {
            indexCard = i;
            break;
        }
    }
    if (groupCard._layoutMeld)
        groupCard._layoutMeld->removeFromParent();
    for (int i = 0; i < _listGroupTitleNode.size(); i++)
        if (_listGroupTitleNode[i].first.first == groupCard._layoutMeld)
        {
            _listGroupTitleNode.erase(_listGroupTitleNode.begin() + i);
            break;
        }

    MeldType meldType = this->getPhomType(groupCard._listCards);
    groupCard._type = meldType;
    std::string meldTitle = "";
    if (meldType == MeldType::MELD_BALA)
        meldTitle = groupCard._listCards.size() >= SECRET_MELD_SIZE ? GET_TEXT_BY_ID(TXT_GROUP_MELD_SECRET)
                                                                    : GET_TEXT_BY_ID(TXT_GROUP_MELD_THREE_OF_A_KIND);
    else if (meldType == MeldType::MELD_SANH)
        meldTitle = groupCard._listCards.size() >= SECRET_MELD_SIZE ? GET_TEXT_BY_ID(TXT_GROUP_MELD_SECRET)
                                                                    : GET_TEXT_BY_ID(TXT_GROUP_MELD_STRAIGHT_FLUSH);

    Layout *layoutGroupCard = (Layout *) _layoutGroupCardTittle;

    auto validBg = (Text *) layoutGroupCard->getChildByName("ImageBG");
    auto noneBg = (Text *) layoutGroupCard->getChildByName("ImageBGNone");
    auto specBg = (Text *) layoutGroupCard->getChildByName("ImageBGSpecial");

    float width = GIN_WIDTH_CARD + (groupCard._listCards.size() - 1) * _distanceBetweenTwoCards / CARD_IN_HAND_SCALE;
    float posX = -indexCard * _distanceBetweenTwoCards - GIN_WIDTH_CARD * 0.5 + width / 2;
    float posY = (+GIN_HEIGHT_CARD * CARD_IN_HAND_SCALE * 0.5f) + layoutGroupCard->getContentSize().height * 0.5f;

    layoutGroupCard->setContentSize(Size(width, layoutGroupCard->getContentSize().height));
    layoutGroupCard->setPosition(Vec2(posX, posY));

    Text *textTitle = (Text *) layoutGroupCard->getChildByName("TextGroupType");
    textTitle->setString(meldTitle);

    validBg->setVisible(false);
    noneBg->setVisible(false);
    specBg->setVisible(false);

    if (meldType != MeldType::MELD_NONE)
    {
        if (groupCard._listCards.size() >= SECRET_MELD_SIZE)
            specBg->setVisible(true);
        else
            validBg->setVisible(true);
    } else
        noneBg->setVisible(true);

    Helper::doLayout(layoutGroupCard);
    _layoutGroupCardTittle->setScale(CARD_IN_HAND_SCALE);

    layoutGroupCard = (Layout *) _layoutGroupCardTittle->clone();
    _listGroupTitleNode.push_back(std::make_pair(std::make_pair(layoutGroupCard, groupCard._listCards.at(indexCard)),
                                                 Vec2((int) (posX * CARD_IN_HAND_SCALE),
                                                      (int) (posY * CARD_IN_HAND_SCALE))));
    groupCard._listCards.at(indexCard)->getParent()->addChild(layoutGroupCard, -1);
    groupCard._layoutMeld = layoutGroupCard;
}

void GPGinControlCardLayer::removeGroup(int groupIndex)
{
    if (groupIndex >= 0 && groupIndex < _listGroupHandCardsMainUser.size())
    {
        GroupCard &groupCard = _listGroupHandCardsMainUser.at(groupIndex);
        if (groupCard._layoutMeld)
            groupCard._layoutMeld->removeFromParent();
        for (int i = 0; i < _listGroupTitleNode.size(); i++)
            if (_listGroupTitleNode[i].first.first == groupCard._layoutMeld)
            {
                _listGroupTitleNode.erase(_listGroupTitleNode.begin() + i);
                break;
            }
        _listGroupHandCardsMainUser.erase(_listGroupHandCardsMainUser.begin() + groupIndex);
    }
}

void GPGinControlCardLayer::clearGroup(bool needRemoveLayout)
{
    if (needRemoveLayout)
    {
        for (auto const &groupCard: _listGroupHandCardsMainUser)
            if (groupCard._layoutMeld)
                groupCard._layoutMeld->removeFromParent();
    }
    _listGroupTitleNode.clear();
    _listGroupHandCardsMainUser.clear();
}

void GPGinControlCardLayer::clearSoftGroup()
{
    _listSoftGroupHandCardsMainUser.clear();
}

void GPGinControlCardLayer::checkUpdateGroup(GinCard *startCard, GinCard *endCard, int numberGap)
{
    if (checkValidCardsOnHand(startCard) && checkValidCardsOnHand(endCard))
    {
        int groupIndexOfCard1 = this->checkCardInAGroup(startCard->getCardIndex());
        int groupIndexOfCard2 = this->checkCardInAGroup(endCard->getCardIndex());
        if (numberGap == 0)
        {
            if (groupIndexOfCard1 >= 0 && groupIndexOfCard2 == -1 &&
                groupIndexOfCard1 < _listGroupHandCardsMainUser.size())
            {
                int indexStartCard = this->getIndexCardOnHand(startCard);
                int indexEndCard = this->getIndexCardOnHand(endCard);
                GroupCard &groupCard = _listGroupHandCardsMainUser[groupIndexOfCard1];
                auto posStartCard = std::find(groupCard._listCards.begin(), groupCard._listCards.end(), startCard);
                if (posStartCard != groupCard._listCards.end())
                    groupCard._listCards.insert(posStartCard +
                                                ((indexEndCard - indexStartCard) > 0 ? (indexEndCard - indexStartCard)
                                                                                     : (indexEndCard - indexStartCard +
                                                                                        1)), endCard);
                this->updateGroup(groupCard);
            } else if (groupIndexOfCard1 >= 0 && groupIndexOfCard1 == groupIndexOfCard2 &&
                       groupIndexOfCard1 < _listGroupHandCardsMainUser.size())
            {
                int indexEndCard = this->getIndexCardOnHand(endCard);
                GroupCard &groupCard = _listGroupHandCardsMainUser[groupIndexOfCard1];
                int countCardOfPreviousGroup = 0;
                for (int i = 0; i < groupIndexOfCard1; i++)
                    countCardOfPreviousGroup += _listGroupHandCardsMainUser.at(i)._listCards.size();
                indexEndCard = indexEndCard - countCardOfPreviousGroup;
                auto positionEndCard = std::find(groupCard._listCards.begin(), groupCard._listCards.end(), endCard);
                if (positionEndCard != groupCard._listCards.end())
                {
                    groupCard._listCards.erase(positionEndCard);
                    groupCard._listCards.insert(groupCard._listCards.begin() + indexEndCard, endCard);
                }
                this->updateGroup(groupCard);
            } else if (groupIndexOfCard1 >= 0 && groupIndexOfCard2 >= 0 && groupIndexOfCard1 != groupIndexOfCard2)
            {
                // Remove card out of group
                if (groupIndexOfCard2 < _listGroupHandCardsMainUser.size())
                {
                    GroupCard &groupCard = _listGroupHandCardsMainUser[groupIndexOfCard2];
                    auto pos = std::find(groupCard._listCards.begin(), groupCard._listCards.end(), endCard);
                    if (pos != groupCard._listCards.end())
                        groupCard._listCards.erase(pos);
                    this->updateGroup(groupCard);
                }
                // Add card to another group
                if (groupIndexOfCard1 < _listGroupHandCardsMainUser.size())
                {
                    int indexStartCard = this->getIndexCardOnHand(startCard);
                    int indexEndCard = this->getIndexCardOnHand(endCard);
                    GroupCard &groupCard = _listGroupHandCardsMainUser[groupIndexOfCard1];
                    auto posStartCard = std::find(groupCard._listCards.begin(), groupCard._listCards.end(), startCard);
                    if (posStartCard != groupCard._listCards.end())
                        groupCard._listCards.insert(posStartCard +
                                                    ((indexEndCard - indexStartCard) > 0 ? (indexEndCard -
                                                                                            indexStartCard) : (
                                                             indexEndCard - indexStartCard + 1)), endCard);
                    this->updateGroup(groupCard);
                }
            } else if (groupIndexOfCard1 == -1 && groupIndexOfCard2 >= 0)
            {
                this->removeCardOutOfGroup(groupIndexOfCard2, endCard->getCardIndex());
            }
        }
    }
}

void GPGinControlCardLayer::removeCardOutOfGroup(int groupIndex, int cardValue)
{
    GroupCard &groupCard = _listGroupHandCardsMainUser[groupIndex];
    int indexCard = -1;
    for (int i = 0; i < groupCard._listCards.size(); i++)
    {
        if (groupCard._listCards[i]->getCardIndex() == cardValue)
        {
            indexCard = i;
            break;
        }
    }
    if (indexCard >= 0)
    {
        groupCard._listCards.erase(groupCard._listCards.begin() + indexCard);
        if (groupCard._listCards.size() > 1)
            this->updateGroup(groupCard);
        else
            this->removeGroup(groupIndex);
    }
}

std::vector<int> GPGinControlCardLayer::convertListCardInt(const std::vector<GinCard *> &listCards)
{
    std::vector<int> listCardsInt;
    for (auto const &card: listCards)
        listCardsInt.push_back(card->getCardIndex());
    return listCardsInt;
}

int GPGinControlCardLayer::getScore(int cardValue)
{
    int rank = GinBaseCard::convertRank(cardValue);
    if (rank >= (int) Rank::RANK_J && rank <= (int) Rank::RANK_K)
        return 10;
    else
        return (rank + 1);
    return rank;
}

void GPGinControlCardLayer::calculateScoreCardOnHand()
{
    int score = 0;
    for (const auto &i: _listHandCardsMainUser)
        if (this->checkCardInAGroup(i->getCardValue()) < 0)
            score += this->getScore(i->getCardValue());
    for (const auto &groupCard: _listGroupHandCardsMainUser)
        if (!this->MeldIsValid(groupCard._listCards))
            for (const auto &j: groupCard._listCards)
                score += this->getScore(j->getCardValue());
    _currentScore = score;
    _gameGin->updateDeadwoodBox(score, _currentKnockScore, MAIN_USER_SLOT_INDEX);
}

void GPGinControlCardLayer::calculateScoreCardOnHandBySoftGroup()
{
    int score = 0;
    for (const auto &i: _listHandCardsMainUser)
        if (this->checkCardInASoftGroup(i->getCardValue()) < 0)
            score += this->getScore(i->getCardValue());
    for (const auto &groupCard: _listSoftGroupHandCardsMainUser)
        if (!this->MeldIsValid(groupCard._listCard))
            for (const auto &j: groupCard._listCard)
                score += this->getScore(j->getCardValue());
    _currentScore = score;
    _gameGin->updateDeadwoodBox(score, _currentKnockScore, MAIN_USER_SLOT_INDEX);
}


void GPGinControlCardLayer::checkStatusOfShowThrowCardButton()
{
    int sizeListThrowCards = this->getListThrowCards().size();
    //    bool visible            = sizeListThrowCards > 0;
    bool visible = false;
    this->setVisibleButtonShowThrowCards(visible);
}

void GPGinControlCardLayer::updateNumberOfRemainCards(int orderIdx, bool anim/* = false*/)
{
    _listUserHandNode.at(orderIdx)->relignHand(_listHandCardsAllUser.at(orderIdx).size());
}

void GPGinControlCardLayer::resetDataByDrapDropCards(bool success)
{
    if (success)
    {
        _ginCardDrapCached = nullptr;
        _ptouchDrapCached = nullptr;
        _pEventDrapCached = nullptr;
    } else
    {
        if (_ginCardDrapCached && _ptouchDrapCached && _pEventDrapCached)
            _ginCardDrapCached->onTouchEnded(_ptouchDrapCached, _pEventDrapCached);
        _ginCardDrapCached = nullptr;
        _ptouchDrapCached = nullptr;
        _pEventDrapCached = nullptr;
    }
}

void GPGinControlCardLayer::update(float dt)
{
    for (auto const &pair: _listGroupTitleNode)
        pair.first.first->setPosition(pair.first.second->getPosition() + pair.second);

    if (this->_ginDrawAndTakeAnimEnable == true && this->_ginDrawAndTakeAnimationRunning == true &&
            this->_ginDrawAndTakeTouchHoldCountDown > 0 && this->_ginDrawAndTakeType != FakeCardType::NONE)
    {
        this->_ginDrawAndTakeTouchHoldCountDown -= dt;

        Vec2 touchLocation = this->_layoutRoot->convertToNodeSpace(_ginDrawAndTakeCurrentTouch->getLocationInView());
        if((_ginDrawAndTakeType == FakeCardType::DRAW &&
                _layoutTouchDraw->getBoundingBox().containsPoint(touchLocation) == false) ||
            (_ginDrawAndTakeType == FakeCardType::TAKE &&
                _layoutTouchTake->getBoundingBox().containsPoint(touchLocation) == false))
        {
            this->_ginDrawAndTakeTouchHoldCountDown = 0;
        }

        if (this->_ginDrawAndTakeTouchHoldCountDown <= 0)
        {
            this->_ginDrawAndTakeTouchHoldCountDown = 0;
            this->createDrawAndTakeFakeCard();
            this->enableDrawAndTakeButton(false);

//            showEffectHighlight("EffectDragToHand",
//                                this->_layoutCards->getParent(),
//                                this->_layoutCards->getContentSize() * this->_layoutCards->getScale() + Size(10,10),
//                                this->_layoutCards->getPosition(),
//                                this->_layoutCards->getLocalZOrder());
        }
    }
}

bool GPGinControlCardLayer::isVisibleEffectHintAtDropPlace(HintDropPlaceType type)
{
    std::string name = GameUtils::getHintDropPlaceTypeName(type);
    if (_mapEffectHighlight.find(name) != _mapEffectHighlight.end())
        if (_mapEffectHighlight[name].size() > 0)
            return true;
    return false;
}

void GPGinControlCardLayer::setVisibleEffectHintAtDropPlace(bool visible, HintDropPlaceType type)
{
    if (visible)
    {
        string name = GameUtils::getHintDropPlaceTypeName(type);
        showEffectHighlight(name,
                            _layoutRoot,
                            _listHintDropArea[type].first.size,
                            this->_layoutRoot->convertToNodeSpace(_listHintDropArea[type].first.origin) +
                            _listHintDropArea[type].first.size / 2,
                            (int) ZorderControlLayer::EFFECT_HINT);


//        showEffectHighlight(GameUtils::getHintDropPlaceTypeName(type),
//                            _layoutRoot,
//                            GameUtils::convertToNodeBoundingBox(_layoutRoot, _listHintDropArea[type].first).size,
//                            _listHintDropArea[type].first.origin + Size(_listHintDropArea[type].first.size.width * 0.5f, _listHintDropArea[type].first.size.height * 0.5f),
//                            (int) ZorderControlLayer::EFFECT_HINT);
    } else
    {
        removeEffectHighlight(GameUtils::getHintDropPlaceTypeName(type));
    }
}

void GPGinControlCardLayer::hideAllEffectHintAtDropPlace()
{
    for (const auto &hintDropPlace: _listHintDropArea)
        setVisibleEffectHintAtDropPlace(false, hintDropPlace.first);
}

Node *
GPGinControlCardLayer::showEffectHighlight(const std::string &name, cocos2d::Node *parent, const cocos2d::Size &size,
                                           const cocos2d::Vec2 &position, int zorder)
{
    Layout *layout = _gameGin->getInterfaceLayer()->getLayoutEffectHint();
    layout->setContentSize(size);
    Helper::doLayout(layout);

    layout = (Layout *) layout->clone();
    layout->setPosition(position);
    layout->setVisible(true);
    layout->setLocalZOrder(zorder);
    parent->addChild(layout);

    Helper::seekWidgetByName(layout, "LayoutImage")->runAction(
            RepeatForever::create(Sequence::create(EaseSineIn::create(FadeOut::create(0.75f)),
                                                   DelayTime::create(0.1f),
                                                   EaseSineIn::create(FadeIn::create(0.75f)),
                                                   DelayTime::create(0.1f),
                                                   nullptr)));

    if (name != "")
    {
        if (_mapEffectHighlight.find(name) == _mapEffectHighlight.end())
            _mapEffectHighlight[name] = std::vector<cocos2d::Node *>();
        _mapEffectHighlight[name].push_back(layout);
    }
    return layout;
}

void GPGinControlCardLayer::removeEffectHighlight(const std::string &name)
{
    if (_mapEffectHighlight.find(name) != _mapEffectHighlight.end())
    {
        for (Node *node: _mapEffectHighlight[name])
            node->removeFromParent();
        _mapEffectHighlight[name].clear();
    }
}

void GPGinControlCardLayer::removeAllEffectHighlight()
{
    for (auto const &listHightLight: _mapEffectHighlight)
        for (auto const &highlight: listHightLight.second)
            highlight->removeFromParent();
    _mapEffectHighlight.clear();
}

void GPGinControlCardLayer::showGroupButton(bool show, const std::vector<GinCard *> &listCard)
{
    if (!show)
    {
        _buttonGroup->setVisible(false);
        _buttonGroup->setEnabled(false);
    } else if (listCard.size() > 0)
    {
        bool isVisible = _buttonGroup->isVisible();
        showUngroupButton(false, std::vector<GinCard *>());
        _buttonGroup->setVisible(true);
        _buttonGroup->setEnabled(true);
        _buttonGroup->setPosition(_buttonGroup->getParent()
                                          ->convertToNodeSpace(_layoutCards->convertToWorldSpace(Vec2(
                                                  listCard.at(0)->getPositionX() + (abs(listCard.at(0)->getPositionX() -
                                                                                        listCard.at(listCard.size() -
                                                                                                    1)->getPositionX()) /
                                                                                    2),
                                                  0 + (listCard.at(0)->getContentSize().height) *
                                                      listCard.at(0)->getScale() + 25))));

        if (!isVisible)
        {
            _buttonGroup->setOpacity(0);
            _buttonGroup->stopAllActions();
            _buttonGroup->runAction(FadeIn::create(0.1f));
        }
    }
}

void GPGinControlCardLayer::showUngroupButton(bool show, const std::vector<GinCard *> &listCard)
{
    if (!show)
    {
        _buttonUngroup->setVisible(false);
        _buttonUngroup->setEnabled(false);
    } else if (listCard.size() > 0)
    {
        bool isVisible = _buttonUngroup->isVisible();
        showGroupButton(false, std::vector<GinCard *>());
        _buttonUngroup->setVisible(true);
        _buttonUngroup->setEnabled(true);

        Vec2 pos = _layoutCards->convertToWorldSpace(Vec2(listCard.at(0)->getPositionX() +
                                                          (abs(listCard.at(0)->getPositionX() -
                                                               listCard.at(listCard.size() - 1)->getPositionX()) / 2),
                                                          0 + (listCard.at(0)->getContentSize().height) *
                                                              listCard.at(0)->getScale() + 30));
        _buttonUngroup->setPosition(_buttonUngroup->getParent()->convertToNodeSpace(pos));
        if (!isVisible)
        {
            _buttonUngroup->setOpacity(0);
            _buttonUngroup->stopAllActions();
            _buttonUngroup->runAction(FadeIn::create(0.1f));
        }
    }
}

void GPGinControlCardLayer::checkUserMovingCardsInSpecificArea(cocos2d::Touch *ptouch, GinCard *card)
{
    if (_status == ControlStatus::CONTROL_STATUS_DUMP)
    {
        Vec2 location = card->getParent()->convertToWorldSpace(card->getPosition());
        for (auto &hintDropArea: _listHintDropArea)
        {
            if (hintDropArea.second.first.containsPoint(location))
            {
                std::vector<GinCard *> listMovingCard = card->getListMovingCards();
                if (hintDropArea.first == HintDropPlaceType::HintDropPlaceAtThrowCardsArea)
                {
                    if (listMovingCard.size() == 1)
                    {
                        if (!this->isVisibleEffectHintAtDropPlace(hintDropArea.first))
                        {
                            cocos2d::ValueMapIntKey data;
                            data[(int) HintDropPlaceData::VISIBLE] = Value(true);
                            hintDropArea.second.second = cocos2d::Value(data);
                            this->setVisibleEffectHintAtDropPlace(true, hintDropArea.first);
                        }
                    }
                    return;
                }
            } else
            {
                if (hintDropArea.second.second.getType() != Value::Type::NONE)
                    hintDropArea.second.second = cocos2d::Value();
                this->setVisibleEffectHintAtDropPlace(false, hintDropArea.first);
            }
        }
    }
}

HintDropPlaceType GPGinControlCardLayer::checkUserDropCardsInSpecificArea()
{
    if (_status == ControlStatus::CONTROL_STATUS_DUMP)
        for (auto hintDropArea: _listHintDropArea)
            if (hintDropArea.second.second.getType() != Value::Type::NONE &&
                hintDropArea.second.second.asIntKeyMap()[(int) HintDropPlaceData::VISIBLE].asBool())
                return hintDropArea.first;
    return HintDropPlaceType::HintDropPlaceNone;
}

void GPGinControlCardLayer::clearAllEffectHintDropArea()
{
    for (auto it = _listHintDropArea.begin(); it != _listHintDropArea.end(); it++)
        if (it->second.second.getType() != Value::Type::NONE)
            it->second.second = cocos2d::Value();
    this->hideAllEffectHintAtDropPlace();
}

void GPGinControlCardLayer::startDrap(GinCard *card)
{
    if (card->getListMovingCards().size() > 1)
        _currentDragCardsType = GinCardDragType::GinDRAGCARD_GROUP_CARD;
    else
        _currentDragCardsType = GinCardDragType::GinDRAGCARD_SINGLE_CARD;
    this->checkUndoActionHintTakeCard();
    float unselectedTime = unselectAllCardsOnHand();
    showUngroupButton(false, std::vector<GinCard *>());
    showGroupButton(false, std::vector<GinCard *>());
    this->checkStateDumpCardButton();
}

void GPGinControlCardLayer::showStepHint(bool show)
{
    if (show)
    {
        ((Text *) Helper::seekWidgetByName(_layoutStepHint, "Text"))->setString("");
        _layoutStepHint->setVisible(true);
        _layoutStepHint->setOpacity(0);
        _layoutStepHint->stopAllActions();
        _layoutStepHint->runAction(RepeatForever::create(Sequence::create(EaseSineIn::create(FadeIn::create(0.5f)),
                                                                          DelayTime::create(0.2f),
                                                                          EaseSineIn::create(FadeOut::create(0.5f)),
                                                                          DelayTime::create(0.2f),
                                                                          nullptr)));
        this->checkStepHint();
    } else
    {
        _layoutStepHint->setVisible(false);
        _layoutStepHint->stopAllActions();
    }
}

void GPGinControlCardLayer::checkStepHint()
{
    if (_layoutStepHint->isVisible())
    {
        string strText = "";
        if (_status == ControlStatus::CONTROL_STATUS_GET)
        {
            strText.append(GET_TEXT_BY_ID(TXT_STEP_HINT_DRAW_CARD));
//            strText.append(" - ");
//            strText.append(GET_TEXT_BY_ID(TXT_STEP_HINT_TAKE_CARD));
        } else if (_status == ControlStatus::CONTROL_STATUS_DUMP)
        {
            strText.append(GET_TEXT_BY_ID(TXT_STEP_HINT_DUMP_CARD));
        }
        ((Text *) Helper::seekWidgetByName(_layoutStepHint, "Text"))->setString(strText);
    }
}

float GPGinControlCardLayer::unselectAllCardsOnHand(bool anim)
{
    float unSelectedTime = 0;
    for (GinCard *card: _listHandCardsMainUser)
    {
        if (card->getState() == GinCardState::GinCARDSTATE_SELECTED)
        {
            unSelectedTime = TIME_ACTION_SELECTED + 0.05f;
            card->unSelected(anim);
        }
    }
    return unSelectedTime;
}

float GPGinControlCardLayer::unselectAllCardsExclude(int excludeCardValue, bool anim)
{
    float unSelectedTime = 0;
    for (GinCard *card: _listHandCardsMainUser)
    {
        if (card->getState() == GinCardState::GinCARDSTATE_SELECTED && card->getCardValue() != excludeCardValue)
        {
            unSelectedTime = TIME_ACTION_SELECTED + 0.05f;
            card->unSelected(anim);
        }
    }
    return unSelectedTime;
}

void GPGinControlCardLayer::removeHandCardsByIndex(int index)
{
    for (int i = 0; i < _listHandCardsMainUser.size(); i++)
    {
        GinCard *card = _listHandCardsMainUser.at(i);
        if (card->getCardIndex() == index)
        {
            int groupIndex = checkCardInAGroup(index);
            if (groupIndex >= 0)
                this->removeCardOutOfGroup(groupIndex, index);

            _layoutCards->removeChild(card);
            _listHandCardsMainUser.erase(_listHandCardsMainUser.begin() + i);
            break;
        }
    }
}

GinCard *GPGinControlCardLayer::getHandCardsByIndex(int orderId, int index)
{
    if (orderId == MAIN_USER_SLOT_INDEX)
    {
        for (auto const &card: _listHandCardsMainUser)
            if (card->getCardIndex() == index)
                return card;
    } else
    {
        for (auto const &card: _listHandCardsAllUser[orderId])
            if (card->getCardIndex() == index)
                return card;
    }
    return nullptr;
}

GinCard *GPGinControlCardLayer::getHandCardsOnUser(int orderId)
{
    GinCard *tlCardTake = nullptr;
    if (orderId != MAIN_USER_SLOT_INDEX && orderId < _listHandCardsAllUser.size())
    {
        auto &listHandCardsUser = _listHandCardsAllUser[orderId];
        if (listHandCardsUser.size() > 0)
        {
            tlCardTake = listHandCardsUser.back();
            listHandCardsUser.pop_back();
            tlCardTake->stopAllActions();
        } else
        {
            tlCardTake = createCard(0, CARD_IN_HAND_OTHER_PLAYER_SCALE, false);
            _layoutRoot->addChild(tlCardTake);
        }
    }
    return tlCardTake;
}

bool GPGinControlCardLayer::shouldGroupCardsByHintTakeCard(const std::vector<GinCard *> &listCard)
{
    if (listCard.size() > 0)
    {
        for (auto const &i: listCard)
        {
            bool isOnHand = false;
            for (auto const &card: _listHandCardsMainUser)
            {
                if (i == card)
                {
                    isOnHand = true;
                    break;
                }
            }
            if (!isOnHand)
                return false;
            if (this->checkCardInAGroup(i->getCardIndex()) >= 0)
                return false;
        }
        return true;
    }
    return false;
}

cocos2d::Vec2 GPGinControlCardLayer::getPosShowCard(int orderId, float width)
{
    if (orderId == MAIN_USER_SLOT_INDEX)
        return this->convertWorldPosToLayoutRootPos(_listShowCardPos.at(orderId));
    return this->convertWorldPosToLayoutRootPos(_listShowCardPos.at(orderId));
}

cocos2d::Vec2 GPGinControlCardLayer::convertWorldPosToLayoutRootPos(const cocos2d::Vec2 &posWorld)
{
    return _layoutRoot->convertToNodeSpace(posWorld);
}

void GPGinControlCardLayer::forceUndrapAllCards()
{
    for (auto card: _listHandCardsMainUser)
    {
        if (card->getState() == GinCardState::GinCARDSTATE_DRAG && card->getTouch() != nullptr)
        {
            card->setTouch(nullptr);
            card->unDrag(card);
            break;
        }
    }
}

void GPGinControlCardLayer::checkUndoActionHintTakeCard(bool drawCard/* = false*/)
{
    if (_status == ControlStatus::CONTROL_STATUS_GET || drawCard)
    {
        int count = 0;
        for (auto const &i: _listCardsWereAutoUngroupForHintTake)
            if (this->shouldGroupCardsByHintTakeCard(i.second))
                count++;
        if (count > 0 && count == _listCardsWereAutoUngroupForHintTake.size())
        {
            for (auto const &i: _listCardsWereAutoUngroupForHintTake)
                this->addGroup(i.second, false, true, i.first);
            _listCardsWereAutoUngroupForHintTake.clear();
        }
    }
}

bool GPGinControlCardLayer::checkValidCardsOnHand(GinCard *card)
{
    if (card != nullptr)
        for (auto const &cardOnHand: _listHandCardsMainUser)
            if (cardOnHand && cardOnHand == card)
                return true;
    return false;
}

bool GPGinControlCardLayer::checkValidCards(const std::vector<GinCard *> &listCard, GinCard *card)
{
    if (card != nullptr && listCard.size() > 0)
        for (auto const &cardOnHand: listCard)
            if (cardOnHand && cardOnHand == card)
                return true;
    return false;
}

void GPGinControlCardLayer::showHintKnock(bool show)
{
    _hintDrawCard->stopAllActions();
    _hintDrawCard->setVisible(show);
    if (show)
    {
        _hintDrawCard->setPosition(
                this->getButtonPos(_buttonKnock) + Vec2(0, _buttonKnock->getContentSize().height * 0.5f) +
                Vec2(0, D_POS_HINT_THROW));
        _hintDrawCard->runAction(actionHint(Vec2(0, HINT_DISTANCE)));
    }
}

void GPGinControlCardLayer::showHintDump(bool show)
{
    _hintTakeCard->stopAllActions();
    _hintTakeCard->setVisible(show);
    if (show)
    {
        _hintTakeCard->setPosition(
                this->getButtonPos(_buttonTake) + Vec2(0, _buttonTake->getContentSize().height * 0.5f) +
                Vec2(0, D_POS_HINT_THROW));
        _hintTakeCard->runAction(actionHint(Vec2(0, HINT_DISTANCE)));
    } else
    {
        removeEffectHighlight("DumpCard");
        removeEffectHighlight("TakeCard");
    }
}

void GPGinControlCardLayer::showHintPass(bool show)
{
    _hintDrawCard->stopAllActions();
    _hintDrawCard->setVisible(show);
    if (show)
    {
        _hintDrawCard->setPosition(
                this->getButtonPos(_buttonPass) + Vec2(0, _buttonPass->getContentSize().height * 0.5f) +
                Vec2(0, D_POS_HINT_THROW));
        _hintDrawCard->runAction(actionHint(Vec2(0, HINT_DISTANCE)));
    }
}

void GPGinControlCardLayer::showHintDraw(bool show)
{
    _hintDrawCard->stopAllActions();
    _hintDrawCard->setVisible(show);
    if (show)
    {
        auto a = Vec2(0, 0.3f * (MAXIMUM_NUMBER_CARD - 1 - this->_gameGin->getInterfaceLayer()->getNumDrawCard()));
        _hintDrawCard->setPosition(
                this->getButtonPos(_buttonDraw) + Vec2(0, _buttonDraw->getContentSize().height * 0.5f) +
                Vec2(0, D_POS_HINT_THROW) + Vec2(-0.3f * (MAXIMUM_NUMBER_CARD - 1 - this->_gameGin->getInterfaceLayer()->getNumDrawCard()), 0));
        _hintDrawCard->runAction(actionHint(Vec2(0, HINT_DISTANCE)));
    }
}

void GPGinControlCardLayer::showHintTake(bool show)
{
    _hintTakeCard->stopAllActions();
    _hintTakeCard->setVisible(show);
    if (show)
    {
        _hintTakeCard->setPosition(
                this->getButtonPos(_buttonTake) + Vec2(0, _buttonTake->getContentSize().height * 0.5f) +
                Vec2(0, D_POS_HINT_THROW) /*+ Vec2(-(this->_listThrowCards.size()-1)*0.3f, 0)*/);
        _hintTakeCard->runAction(actionHint(Vec2(0, HINT_DISTANCE)));
    }
}

void GPGinControlCardLayer::showHintGin(bool show)
{
    _hintDrawCard->stopAllActions();
    _hintDrawCard->setVisible(show);
    if (show)
    {
        _hintDrawCard->setPosition(
                this->getButtonPos(_buttonGin) + Vec2(0, _buttonGin->getContentSize().height * 0.5f) +
                Vec2(0, D_POS_HINT_THROW));
        _hintDrawCard->runAction(actionHint(Vec2(0, HINT_DISTANCE)));
    }
}

void GPGinControlCardLayer::showHintBigGin(bool show)
{
    _hintDrawCard->stopAllActions();
    _hintDrawCard->setVisible(show);
    if (show)
    {
        _hintDrawCard->setPosition(
                this->getButtonPos(_buttonBigGin) + Vec2(0, _buttonBigGin->getContentSize().height * 0.5f) +
                Vec2(0, D_POS_HINT_THROW));
        _hintDrawCard->runAction(actionHint(Vec2(0, HINT_DISTANCE)));
    }
}

bool GPGinControlCardLayer::checkValidMeldGroupLayout(cocos2d::Node *node)
{
    if (node != nullptr)
        for (const GroupCard &group: _listGroupHandCardsMainUser)
            if (group._layoutMeld == node)
                return true;
    return false;
}

// Insert card and arrange showing cards
bool GPGinControlCardLayer::removeCardFromRemainCards(int orderIdx, GinCard *card)
{
    auto &listRemainCards = _listRemainCards[orderIdx];
    const auto &itorCard = std::find(listRemainCards.begin(), listRemainCards.end(), card);
    if (itorCard != listRemainCards.end())
    {
        listRemainCards.erase(itorCard);
        return true;
    }
    return false;
}

bool GPGinControlCardLayer::removeCardFromShowingCards(int orderIdx, GinCard *card)
{
    auto &listShowCards = _listShowCards[orderIdx];
    const auto &itorCard = std::find(listShowCards.begin(), listShowCards.end(), card);
    if (itorCard != listShowCards.end())
    {
        listShowCards.erase(itorCard);
        return true;
    }
    return false;
}

int GPGinControlCardLayer::insertCardToShowingCards(int orderIdx, GinCard *ginCard)
{
    int idx = -1;
    int countCard = 0;
    const auto &listMeldUser = _listMeldsAllUser[orderIdx];
    for (const auto &i: listMeldUser)
    {
        for (const auto &j: i)
        {
            if (j == ginCard)
                idx = countCard;
            countCard++;
        }
    }
    if (idx != -1)
        _listShowCards[orderIdx].insert(_listShowCards[orderIdx].begin() + idx, ginCard);
    return idx;
}

std::vector<GinCardProperty> GPGinControlCardLayer::calculatePositionShowingCards(int orderIdx)
{
    std::vector<GinCardProperty> listGinCardProperty;
    const auto &listMeldUser = _listMeldsAllUser[orderIdx];
    int countCard = _listShowCards[orderIdx].size();
    int numMeld = listMeldUser.size();
    std::vector<int> thresholdMeld;
    int thresholdTemp = 0;
    for (const auto &i: listMeldUser)
    {
        thresholdTemp += i.size();
        thresholdMeld.push_back(thresholdTemp);
    }
    int numRemainCard = _listRemainCards[orderIdx].size() > 0 ? 1 : 0;
    float width = countCard > 0 ? GIN_WIDTH_CARD * CARD_ON_TABLE_SCALE_1 +
                                  (countCard - 1 - (numMeld + numRemainCard - 1)) *
                                  DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 +
                                  (numMeld + numRemainCard - 1) * WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1 : 0;
    Vec2 posShowCard = this->getPosShowCard(orderIdx, width);
    Vec2 posAdd = Vec2(-width * 0.5f + 0.5 * GIN_WIDTH_CARD * CARD_ON_TABLE_SCALE_1, 0);
    for (int i = 0; i < countCard; i++)
    {
        bool isThresholdMeld = std::find(thresholdMeld.begin(), thresholdMeld.end(), i) != thresholdMeld.end();
        if (i > 0)
        {
            if (isThresholdMeld)
                posAdd.x += WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1;
            else
                posAdd.x += DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1;
        }
        GinCardProperty ginCardProperty;
        Vec2 posTarget = posShowCard + posAdd;
        ginCardProperty._positionOnHand = posTarget;
        ginCardProperty._angle = 0;
        ginCardProperty._scale = CARD_ON_TABLE_SCALE_1;
        ginCardProperty._zOder = i;
        listGinCardProperty.push_back(ginCardProperty);
    }
    return listGinCardProperty;
}

cocos2d::Vec2 GPGinControlCardLayer::calculateGlobalPositionScoreBox(int orderIdx)
{
    int countCard = _listShowCards[orderIdx].size();
    int numMeld = _listMeldsAllUser[orderIdx].size();
    int numRemainCard = _listRemainCards[orderIdx].size() > 0 ? 1 : 0;
    float width = countCard > 0 ? GIN_WIDTH_CARD * CARD_ON_TABLE_SCALE_1 +
                                  (countCard - 1 - (numMeld + numRemainCard - 1)) *
                                  DISTANCE_BETWEEN_TWO_CARD_ON_TABLE_1 +
                                  (numMeld + numRemainCard - 1) * WIDTH_OF_GAP_BETWEEN_TWO_MELD_PIXEL_1 : 0;

//    return this->getPosShowCard(orderIdx, width) + Vec2(width * 0.5f + 60.0f, 0.0f);
    Vec2 pos = this->_gameGin->getInterfaceLayer()->getShowCardGlobalPos().at(orderIdx);
    float offsetX = 60.0f;
    float offsetY = 0.0f;

    if (s_resolution_ratio <= RESOLUTION_1_6)
    {
        offsetX = 20.0f;
//        if (orderIdx == INDEX_BOT)
//        {
//            offsetY = 15.0f;
//        }
    }

    pos += Vec2(width * 0.5f + offsetX, offsetY);
    return pos;
}

void GPGinControlCardLayer::arrageShowingCards(int orderIdx, const std::vector<int> &excludeCardsValue)
{
    std::vector<GinCard *> excludeCards;
    const auto &listShowCards = _listShowCards[orderIdx];
    for (const auto &i: excludeCardsValue)
        for (const auto &j: listShowCards)
            if (i == j->getCardValue())
            {
                excludeCards.push_back(j);
                break;
            }

    for (int i = 0; i < listShowCards.size(); i++)
    {
        GinCard *card = listShowCards[i];
        bool ignore = false;
        for (const auto &j: excludeCards)
        {
            if (card == j)
            {
                ignore = true;
                break;
            }
        }
        if (!ignore)
        {
            std::vector<GinCardProperty> listShowingCardProperty = this->calculatePositionShowingCards(orderIdx);
            const GinCardProperty &cardProperty = listShowingCardProperty[i];
            card->setLocalZOrder((int) ZorderControlLayer::CARD_IN_HAND + cardProperty._zOder);
            card->runAction(Sequence::create(Spawn::create(
                    EaseCubicActionOut::create(MoveTo::create(TIME_CARD_SORT, cardProperty._positionOnHand)),
                    ScaleTo::create(TIME_CARD_SORT, cardProperty._scale),
                    RotateTo::create(TIME_CARD_SORT, cardProperty._angle),
                    nullptr), CallFunc::create([=]()
                                               {
                                               }), nullptr));
        }
    }
}

int GPGinControlCardLayer::calculateScoreCards(const std::vector<GinCard *> &listCard)
{
    int score = 0;
    for (GinCard *card: listCard)
        score += SvrCard::getPoints(card->getCardIndex());
    return score;
}

int GPGinControlCardLayer::calculateScoreCards(const std::vector<int> &listCard)
{
    int score = 0;
    for (int card: listCard)
        score += SvrCard::getPoints(card);
    return score;
}

void GPGinControlCardLayer::resetListSoftGroupHandCardMainUser()
{
    for (const auto &softGroup: _listSoftGroupHandCardsMainUser)
        for (const auto &card: softGroup._listCard)
            if (checkValidCardsOnHand(card) && card != nullptr)
                card->setMeldHighLightVisible(false);
    _listSoftGroupHandCardsMainUser.clear();
}

bool GPGinControlCardLayer::updateListSoftGroupHandCardMainUser()
{
    this->resetListSoftGroupHandCardMainUser();
    if (this->_listHandCardsMainUser.size() < 3)
        return false;

    int startIndex = 0;
    /*** Check and find all possible 3 cards meld(s) ***/
    for (int index = startIndex; index < this->_listHandCardsMainUser.size();)
    {
        bool checkResult = true;
        vector<GinCard *> cards;
        int checkIndex = index;

        for (int i = 0; i < 3; ++i)
        {
            checkIndex = index + i;
            if (0 <= checkIndex && checkIndex < this->_listHandCardsMainUser.size())
                cards.push_back(this->_listHandCardsMainUser[checkIndex]);
        }
        if (this->MeldIsValid(cards))
        {
            SoftGroupCard sgc;
            sgc._listCard = cards;
            sgc._startIndexInHand = index;
            sgc._endIndexInHand = index + cards.size() - 1;
            sgc._type = getPhomType(cards);
            _listSoftGroupHandCardsMainUser.push_back(sgc);
            index += cards.size();
        } else
        {
            index += 1;
        }
    }

    /*** Check and expand valid soft group if possible ***/
    for (int i = 0; i < this->_listSoftGroupHandCardsMainUser.size(); ++i)
    {
        SoftGroupCard group = this->_listSoftGroupHandCardsMainUser[i];

        if (group._type == MeldType::MELD_BALA)
        {
            vector<GinCard *> cards = group._listCard;
            /*** Try to expand forward from the end of meld ***/
            int startIndex = group._endIndexInHand + 1;
            for (int ci = startIndex; ci < this->_listHandCardsMainUser.size();)
            {
                GinCard *card = this->_listHandCardsMainUser[ci];
                int cigIndex = this->isCardInSoftGroup(card);
                if (cigIndex == -1)
                {
                    cards.push_back(card);
                    if (MeldIsValid(cards))
                    {
                        group._endIndexInHand = ci;
                        group._listCard = cards;
                        this->_listSoftGroupHandCardsMainUser[i] = group;
                        ci++;
                    } else
                    {
                        cards.pop_back();
                        break;
                    }
                } else
                {
                    break;
                }
            }
        } else if (group._type == MeldType::MELD_SANH)
        {
            vector<GinCard *> cards = group._listCard;

            bool noMoreCard = false;

            while (noMoreCard == false)
            {
                bool hasCardFromTheEnd = false;
                bool hasCardFromTheBeginning = false;

                int startIndex = group._endIndexInHand + 1;
                /*** Try to expand forward from the end***/
                for (int ci = startIndex; ci < this->_listHandCardsMainUser.size();)
                {
                    GinCard *card = this->_listHandCardsMainUser[ci];
                    int cigIndex = this->isCardInSoftGroup(card);
                    if (cigIndex == -1)
                    {
                        cards.push_back(card);
                        if (MeldIsValid(cards))
                        {
                            group._endIndexInHand = ci;
                            group._listCard = cards;
                            this->_listSoftGroupHandCardsMainUser[i] = group;

                            if (hasCardFromTheEnd == false)
                            {
                                hasCardFromTheEnd = true;
                            }

                            ci++;
                        } else
                        {
                            cards.pop_back();
                            break;
                        }
                    } else
                    {
                        break;
                    }
                }
                /*** Try to expand backward from the beginning ***/
                startIndex = group._startIndexInHand - 1;
                for (int ci = startIndex; ci >= 0;)
                {
                    GinCard *card = this->_listHandCardsMainUser[ci];
                    int cigIndex = this->isCardInSoftGroup(card);
                    if (cigIndex == -1)
                    {
                        cards.insert(cards.begin(), card);
                        if (MeldIsValid(cards))
                        {
                            group._startIndexInHand = ci;
                            group._listCard = cards;
                            this->_listSoftGroupHandCardsMainUser[i] = group;

                            if (hasCardFromTheBeginning == false)
                            {
                                hasCardFromTheBeginning = true;
                            }

                            ci--;
                        } else
                        {
                            cards.erase(cards.begin());
                            break;
                        }
                    } else
                    {
                        break;
                    }
                }

                /*** Check if loop could be continued ***/
                if (hasCardFromTheBeginning == false && hasCardFromTheEnd == false)
                {
                    noMoreCard = true;
                } else
                {
                    noMoreCard = false;
                }
            }
        }
    }

    /*** Merge straight melds if possible ***/
    if (this->_listSoftGroupHandCardsMainUser.size() >= 2)
    {
        bool shouldStop = false;
        int index = 0;
        while (!shouldStop)
        {
            SoftGroupCard group = this->_listSoftGroupHandCardsMainUser[index];
            bool isMerged = false;
            if (group._type == MeldType::MELD_SANH)
            {
                int nIndex = index + 1;
                if (nIndex < this->_listSoftGroupHandCardsMainUser.size())
                {
                    SoftGroupCard nGroup = this->_listSoftGroupHandCardsMainUser[nIndex];
                    if ((nGroup._type == MeldType::MELD_SANH) &&
                        (group._endIndexInHand + 1 == nGroup._startIndexInHand))
                    {
                        vector<GinCard *> cards = group._listCard;
                        cards.insert(cards.end(), nGroup._listCard.begin(), nGroup._listCard.end());
                        if (MeldIsValid(cards))
                        {
                            this->_listSoftGroupHandCardsMainUser[index]._listCard = cards;
                            this->_listSoftGroupHandCardsMainUser[index]._endIndexInHand = nGroup._endIndexInHand;
                            this->_listSoftGroupHandCardsMainUser.erase(
                                    this->_listSoftGroupHandCardsMainUser.begin() + nIndex);
                            isMerged = true;
                        }
                    }
                }
            }
            if (!isMerged)
                index++;
            if (index >= this->_listSoftGroupHandCardsMainUser.size())
                shouldStop = true;
        }
    }

    /*** Update group card's colors***/
    for (int i = 0; i < _listSoftGroupHandCardsMainUser.size(); ++i)
        for (const auto &card: _listSoftGroupHandCardsMainUser[i]._listCard)
            card->setMeldHighLightVisible(true, _listMeldHighLightColor[i]);

    /*** Check if soft group is changed ***/
    const std::vector<std::vector<int>> &oldMelds = this->getListMeldOnHandMainUserBySoftGroup();
    const std::vector<std::vector<int>> &newMelds = this->getListMeldOnHandMainUserBySoftGroup();
    if (oldMelds.size() != newMelds.size())
    {
        return true;
    } else
    {
        for (int i = 0; i < oldMelds.size(); ++i)
        {
            const std::vector<int> &oldMeld = oldMelds[i];
            const std::vector<int> &newMeld = newMelds[i];
            if (oldMeld.size() != newMelds.size())
            {
                return true;
            } else
            {
                for (int j = 0; j < oldMeld.size(); ++j)
                    if (oldMeld[j] != newMeld[j])
                        return true;
            }
        }
    }

    return false;
}

bool GPGinControlCardLayer::updateListSoftGroupHandCardMainUserOnThrowCard(GinCard *card)
{
    if (card == nullptr)
        return false;
    int gIndex = this->isCardInSoftGroup(card);
    if (gIndex != -1)
    {
        for (int i = 0; i < this->_listSoftGroupHandCardsMainUser[gIndex]._listCard.size(); ++i)
        {
            if (this->_listSoftGroupHandCardsMainUser[gIndex]._listCard[i] == card)
                this->_listSoftGroupHandCardsMainUser[gIndex]._listCard[i] = nullptr;
            else
                this->_listSoftGroupHandCardsMainUser[gIndex]._listCard[i]->setMeldHighLightVisible(false);
        }
    }
    return this->updateListSoftGroupHandCardMainUser();
}

int GPGinControlCardLayer::isCardInSoftGroup(GinCard *card)
{
    if (card == nullptr)
        return -1;
    for (int i = 0; i < this->_listSoftGroupHandCardsMainUser.size(); ++i)
        for (const auto &gCard: this->_listSoftGroupHandCardsMainUser[i]._listCard)
            if (gCard->getCardValue() == card->getCardValue())
                return i;
    return -1;
}

bool GPGinControlCardLayer::isCardInSoftGroup(int cardValue, SoftGroupCard group)
{
    for (const auto &gCard: group._listCard)
        if (gCard->getCardValue() == cardValue)
            return true;
    return false;
}

void GPGinControlCardLayer::handleManualMeldResponse(int orderIdx, int actionType, int knockDeadwood)
{
    if (orderIdx == MAIN_USER_SLOT_INDEX && actionType == ActionType::K_KNOCK)
        this->setKnockDeadwood(knockDeadwood);

    if (orderIdx == MAIN_USER_SLOT_INDEX && this->_status == ControlStatus::CONTROL_STATUS_DUMP)
        this->updateThrowActionUI((ActionType) actionType);
}

void GPGinControlCardLayer::updateThrowActionUI(ActionType actionType)
{
    this->checkStepHint();
    this->checkStateDumpCardButton();
    if (actionType == ActionType::K_KNOCK)
    {
        _buttonKnock->setVisible(true);
        _buttonPass->setVisible(false);
        _buttonGin->setVisible(false);
        _buttonBigGin->setVisible(false);
        showHintKnock(true);
        visibleParticleKnock();

        Text *textKnock = (Text *) _buttonKnock->getChildByName("TextBMButton");
        textKnock->setString(fmt::format(GET_TEXT_BY_ID(TXT_KNOCK_BUTTON), this->_knockDeadwood));
    } else if (actionType == ActionType::K_GIN)
    {
        _buttonGin->setVisible(true);
        _buttonPass->setVisible(false);
        _buttonKnock->setVisible(false);
        _buttonBigGin->setVisible(false);
        showHintGin(true);
        visibleParticleGin();

    } else if (actionType == ActionType::K_BIG_GIN)
    {
        _buttonBigGin->setVisible(true);
        _buttonPass->setVisible(false);
        _buttonKnock->setVisible(false);
        _buttonGin->setVisible(false);
        showHintBigGin(true);
        visibleParticleBigGin();
    } else
    {
        _buttonPass->setVisible(false);
        _buttonKnock->setVisible(false);
        _buttonGin->setVisible(false);
        _buttonBigGin->setVisible(false);
        showHintKnock(false);
        showHintGin(false);
        showHintBigGin(false);
        hideAllParticle();
    }
}

void GPGinControlCardLayer::setThrowCardsVisible(bool visible)
{
    for (auto &card: _listThrowCards)
        if (card)
            card->setVisible(visible);
}

void GPGinControlCardLayer::setShowCardsVisible(bool visible)
{
    for (auto &cards: _listShowCards)
        for (auto &card: cards)
            if (card)
                card->setVisible(visible);
}

void GPGinControlCardLayer::hideAllEffectHint()
{
    showHintTake(false);
    showHintDraw(false);
    showHintDump(false);
    showHintKnock(false);
    showHintGin(false);
    showHintBigGin(false);
    showStepHint(false);
}

float GPGinControlCardLayer::calculateMatchEndShowCardDuration(const PlayerResultData &win,
                                                               const std::vector<PlayerResultData> &listLose)
{
    float duration = calculateMatchEndEachUserShowCardDuration(win);
    for (const auto &data: listLose)
    {
        float dr = calculateMatchEndEachUserShowCardDuration(data);
        if (dr > duration)
        {
            duration = dr;
        }
    }
    return duration;
}

float GPGinControlCardLayer::calculateMatchEndEachUserShowCardDuration(const PlayerResultData &res)
{
    float duration = 0;
    int orderId = GPGinSlotManager::getInstance()->getSlotOrderIdx(res._playerIndex);

    if (orderId == MAIN_USER_SLOT_INDEX)
    {
        duration = DELAY_SHOW_CARD_ON_TABLE * (res._listPhom.size() + res._remainCards.size()) + TIME_CARD_THROW_END_MATCH_USER;
    } else
    {
        duration = DELAY_SHOW_CARD_ON_TABLE * (res._listPhom.size() + res._remainCards.size()) + TIME_CARD_THROW_END_MATCH_BOT;
    }

    return duration;
}

int GPGinControlCardLayer::getPossibleNewCardIndexBySoftGroup(int cardValue)
{
    int possibleIndex = -1;
    bool found = false;

    //Check if card is already in hand
    for (int i = 0; i < this->_listHandCardsMainUser.size() - 1; ++i)
    {
        GinCard *card = this->_listHandCardsMainUser[i];
        if (card->getCardIndex() == cardValue)
        {
            return i;
        }
    }

    // Check if new card can form a new soft group
    for (int i = 0; i < this->_listHandCardsMainUser.size() - 1; ++i)
    {
        GinCard *card = this->_listHandCardsMainUser[i];
        if (this->checkCardInASoftGroup(card->getCardIndex()) != -1)
        {
            continue;
        }

        int nextI = i + 1;
        GinCard *nextCard = this->_listHandCardsMainUser[nextI];
        if (this->checkCardInASoftGroup(nextCard->getCardIndex()) != -1)
        {
            continue;
        }

        vector<int> possibleNewGroup;
        possibleNewGroup.push_back(card->getCardIndex());
        possibleNewGroup.push_back(nextCard->getCardIndex());
        possibleNewGroup.push_back(cardValue);

        sortIncrease(possibleNewGroup);

        if (this->MeldIsValid(possibleNewGroup) == true)
        {
            int lI = std::find(possibleNewGroup.begin(), possibleNewGroup.end(), cardValue) - possibleNewGroup.begin();

//            possibleIndex = nextI + 1;
            possibleIndex = i + lI;

            if (possibleIndex >= this->_listHandCardsMainUser.size())
            {
                possibleIndex = -1;
            }
            found = true;
            break;
        }
    }

    // Check if new card can be in a soft group
    if (found == false)
    {
        for (int i = 0; i < this->_listSoftGroupHandCardsMainUser.size(); ++i)
        {
            SoftGroupCard group = this->_listSoftGroupHandCardsMainUser[i];

            vector<int> cards;
            for (int j = 0; j < group._listCard.size(); ++j)
            {
                cards.push_back(group._listCard[j]->getCardIndex());
            }
            cards.push_back(cardValue);

            sortIncrease(cards);

            if (MeldIsValid(cards, false) == true)
            {
                int lI = std::find(cards.begin(), cards.end(), cardValue) - cards.begin();

//                possibleIndex = group._endIndexInHand + 1;
                possibleIndex = group._startIndexInHand + lI;

                if (possibleIndex >= this->_listHandCardsMainUser.size())
                {
                    possibleIndex = -1;
                }
                found = true;
                break;
            }
        }
    }
    return possibleIndex;
}

void GPGinControlCardLayer::sortSoftGroups()
{
    /*** Sort melds ***/
    bool isSorted = false;
    for (int i = 0; i < this->_listSoftGroupHandCardsMainUser.size(); ++i)
    {
        SoftGroupCard group = this->_listSoftGroupHandCardsMainUser[i];
        for (int i = 0; i < group._listCard.size(); ++i)
        {
            for (int j = i; j < group._listCard.size(); ++j)
            {
                GinCard *cardI = group._listCard[i];
                GinCard *cardJ = group._listCard[j];
                if (cardI->getCardIndex() > cardJ->getCardIndex())
                {
                    GinCard *cardTemp = group._listCard[i];
                    group._listCard[i] = group._listCard[j];
                    group._listCard[j] = cardTemp;

                    if (isSorted == false)
                    {
                        isSorted = true;
                    }
                }
            }
        }

        int groupIndex = 0;
        for (int i = group._startIndexInHand; i <= group._endIndexInHand; ++i)
        {
            this->_listHandCardsMainUser[i] = group._listCard[groupIndex];
            ++groupIndex;
        }
    }

    if (isSorted == true)
    {
        this->playEffectSortCardsOnHand();
    }
}

void GPGinControlCardLayer::sortSoftGroupContainCards(std::vector<int> cards, bool sortSanhOnly /*=true*/)
{
    /*** Sort melds ***/
    bool isSorted = false;

    for (int card: cards)
    {
        for (int i = 0; i < this->_listSoftGroupHandCardsMainUser.size(); ++i)
        {
            SoftGroupCard group = this->_listSoftGroupHandCardsMainUser[i];

            if (this->isCardInSoftGroup(card, group) == true)
            {
                if(group._type == MeldType::MELD_BALA && sortSanhOnly == true)
                {
                    break;
                }

                for (int i = 0; i < group._listCard.size(); ++i)
                {
                    for (int j = i; j < group._listCard.size(); ++j)
                    {
                        GinCard *cardI = group._listCard[i];
                        GinCard *cardJ = group._listCard[j];
                        if (cardI->getCardIndex() > cardJ->getCardIndex())
                        {
                            GinCard *cardTemp = group._listCard[i];
                            group._listCard[i] = group._listCard[j];
                            group._listCard[j] = cardTemp;

                            if (isSorted == false)
                            {
                                isSorted = true;
                            }
                        }
                    }
                }
            }

            int groupIndex = 0;
            for (int i = group._startIndexInHand; i <= group._endIndexInHand; ++i)
            {
                this->_listHandCardsMainUser[i] = group._listCard[groupIndex];
                ++groupIndex;
            }
        }
    }

    if (isSorted == true)
    {
        this->playEffectSortCardsOnHandSpeedControl(3.0f);
    }
}

cocos2d::BezierTo *
GPGinControlCardLayer::getBezierAnimationVertical(cocos2d::Point srcPos, cocos2d::Point dstPos, bool isLeft, float duration)
{
    ccBezierConfig bezierConfig;
    bezierConfig.endPosition = dstPos;
    if (isLeft == false)
    {
        bezierConfig.controlPoint_1 = Point(srcPos.x, (srcPos.y + dstPos.y) / 2);
        bezierConfig.controlPoint_2 = Point((srcPos.x + dstPos.x) / 2, dstPos.y);
    } else
    {
        bezierConfig.controlPoint_1 = Point((srcPos.x + dstPos.y) / 2, srcPos.y);
        bezierConfig.controlPoint_2 = Point(dstPos.x, (dstPos.y + srcPos.y) / 2);
    }

    BezierTo *bezierTo = BezierTo::create(duration, bezierConfig);

    return bezierTo;
}

cocos2d::BezierTo* GPGinControlCardLayer::getBezierAnimationHorizontal(cocos2d::Point srcPos, cocos2d::Point dstPos, bool isUp, float duration)
{
    ccBezierConfig bezierConfig;
    bezierConfig.endPosition = dstPos;
    float offsetY = 100;
    if (isUp == false)
    {
        bezierConfig.controlPoint_1 = Point(srcPos.x + (dstPos.x - srcPos.x) / 4, srcPos.y - offsetY);
        bezierConfig.controlPoint_2 = Point(srcPos.x + (dstPos.x - srcPos.x) / 4 * 3, srcPos.y - offsetY);
    } else
    {
        bezierConfig.controlPoint_1 = Point(srcPos.x + (dstPos.x - srcPos.x) / 4, srcPos.y + offsetY);
        bezierConfig.controlPoint_2 = Point(srcPos.x + (dstPos.x - srcPos.x) / 4 * 3, srcPos.y + offsetY);
    }

    BezierTo *bezierTo = BezierTo::create(duration, bezierConfig);

    return bezierTo;
}

void GPGinControlCardLayer::shakeTable()
{
    auto nodeTable = this->_gameGin;// _interfaceLayer->getTable()->getChildByName("Table");
    if (nodeTable)
    {
        nodeTable->stopActionByTag(9202);
        Vector<FiniteTimeAction *> list;
        nodeTable->setPosition(Point(0, 0));
        Point pos = nodeTable->getPosition();
        for (int i = 5; i >= 0; i--)
        {
            if (i % 2 == 0)
                list.pushBack(EaseExponentialOut::create(MoveTo::create(0.035f, Point(pos.x - i * 2.0f, pos.y +
                                                                                                        GameUtils::randomIntInRange(
                                                                                                                -1, 1) *
                                                                                                        2.0f))));
            else
                list.pushBack(EaseExponentialOut::create(MoveTo::create(0.035f, Point(pos.x + i * 2.0f, pos.y +
                                                                                                        GameUtils::randomIntInRange(
                                                                                                                -1, 1) *
                                                                                                        2.0f))));
        }
        list.pushBack(EaseExponentialOut::create(MoveTo::create(0.055f, Point(0, 0))));

        auto seq = Sequence::create(list);
        seq->setTag(9202);
        nodeTable->runAction(seq);
    }
}

void GPGinControlCardLayer::shakeTableWithDelay(float delay)
{
    auto nodeTable = this->_gameGin;// _interfaceLayer->getTable()->getChildByName("Table");
    if (nodeTable)
    {
        nodeTable->stopActionByTag(9202);
        Vector<FiniteTimeAction *> list;
        nodeTable->setPosition(Point(0, 0));
        Point pos = nodeTable->getPosition();

        list.pushBack(DelayTime::create(delay));
        for (int i = 5; i >= 0; i--)
        {
            if (i % 2 == 0)
                list.pushBack(EaseExponentialOut::create(MoveTo::create(0.035f, Point(pos.x - i * 2.0f, pos.y +
                                                                                                        GameUtils::randomIntInRange(
                                                                                                                -1, 1) *
                                                                                                        2.0f))));
            else
                list.pushBack(EaseExponentialOut::create(MoveTo::create(0.035f, Point(pos.x + i * 2.0f, pos.y +
                                                                                                        GameUtils::randomIntInRange(
                                                                                                                -1, 1) *
                                                                                                        2.0f))));
        }
        list.pushBack(EaseExponentialOut::create(MoveTo::create(0.055f, Point(0, 0))));

        auto seq = Sequence::create(list);
        seq->setTag(9202);
        nodeTable->runAction(seq);
    }
}

cocos2d::Point GPGinControlCardLayer::getDumpPlacePosInLayoutCard()
{
    return this->_dumpPlacePosInLayoutCard;
}

cocos2d::Size GPGinControlCardLayer::getDumpPlaceSize()
{
    return this->_buttonDump->getContentSize()*this->_buttonDump->getScale();
}


cocos2d::Point GPGinControlCardLayer::getDrawPlacePosInLayoutCard()
{
    return this->_drawPlacePosInLayoutCard;
}

cocos2d::Size GPGinControlCardLayer::getDrawPlaceSize()
{
    return this->_buttonDraw->getContentSize()*this->_buttonDraw->getScale();
}

bool GPGinControlCardLayer::canDumpCard()
{
    return (_status == ControlStatus::CONTROL_STATUS_DUMP);
}

cocos2d::Point GPGinControlCardLayer::correctOutOfBoundDraggingCardPosition(cocos2d::Point cardPos)
{
    if(STYLE_CARDS_ON_HAND == STYLE_CARDS_ON_HAND_STRAIGHT)
    {
        return cardPos;
    }
    else
    {
        float distance = cardPos.distance(this->_anchorCardInLayoutCardPosition);
        float boundLength = (_r + _layoutCards->getContentSize().height / 4);
        if (distance <= boundLength)
        {
            return cardPos;
        }

        Vec2 dVec = (this->_anchorCardInLayoutCardPosition - cardPos);
        dVec.normalize();

        Vec2 rVec = dVec * boundLength;

        Vec2 pos = this->_anchorCardInLayoutCardPosition - rVec;

        return pos;
    }
}

float GPGinControlCardLayer::calculateDraggingCardRotation(cocos2d::Point cardPos)
{
    if(STYLE_CARDS_ON_HAND == STYLE_CARDS_ON_HAND_STRAIGHT)
    {
        return 0;
    }

   Vec2 anchorVec = this->_anchorCardInLayoutCardPosition - (this->_anchorCardInLayoutCardPosition + Vec2(0, _r));
   anchorVec.normalize();
   Vec2 cardVec = this->_anchorCardInLayoutCardPosition - cardPos;
   cardVec.normalize();

   float angle = acos((anchorVec.dot(cardVec)/((anchorVec.length()*cardVec.length()))));
   angle = angle*180/PI;

   if(cardPos.x < this->_anchorCardInLayoutCardPosition.x)
   {
       angle = -angle;
   }

   return angle;
}

void GPGinControlCardLayer::initParticles()
{
    this->_particleKnock = ParticleSystemQuad::create("ui/particle/burn_still.plist");
    this->_particleKnock->setPosition(Size(this->_layoutParticle->getContentSize().width / 2,
                               this->_layoutParticle->getContentSize().height / 2));
    this->_particleKnock->setScale(0.7f);
    this->_particleKnock->resetSystem();
    this->_particleKnock->setVisible(true);

    this->_layoutParticle->addChild(this->_particleKnock, -1);


    this->_particleGin = ParticleSystemQuad::create("ui/particle/star_burn_still.plist");
    this->_particleGin->setPosition(Size(this->_layoutParticle->getContentSize().width / 2,
                                           this->_layoutParticle->getContentSize().height / 2));
    this->_particleGin->setScale(1.0f);
    this->_particleGin->resetSystem();
    this->_particleGin->setVisible(true);

    this->_layoutParticle->addChild(this->_particleGin, -1);


    this->_particleBigGin = ParticleSystemQuad::create("ui/particle/star_burn_still.plist");
    this->_particleBigGin->setPosition(Size(this->_layoutParticle->getContentSize().width / 2,
                                           this->_layoutParticle->getContentSize().height / 2));
    this->_particleBigGin->setScale(1.0f);
    this->_particleBigGin->resetSystem();
    this->_particleBigGin->setVisible(true);

    this->_layoutParticle->addChild(this->_particleBigGin, -1);

    this->hideAllParticle();
}

void GPGinControlCardLayer::visibleParticleKnock()
{
    this->_layoutParticle->setVisible(true);

    this->_particleKnock->setVisible(true);
    this->_particleGin->setVisible(false);
    this->_particleBigGin->setVisible(false);

    this->_particleGin->pauseEmissions();
    this->_particleBigGin->pauseEmissions();

//    this->_particleKnock->resetSystem();
    this->_particleKnock->resumeEmissions();
}

void GPGinControlCardLayer::visibleParticleGin()
{
    this->_layoutParticle->setVisible(true);

    this->_particleKnock->setVisible(false);
    this->_particleGin->setVisible(true);
    this->_particleBigGin->setVisible(false);

    this->_particleKnock->pauseEmissions();
    this->_particleBigGin->pauseEmissions();

//    this->_particleGin->resetSystem();
    this->_particleGin->resumeEmissions();
}

void GPGinControlCardLayer::visibleParticleBigGin()
{
    this->_layoutParticle->setVisible(true);

    this->_particleKnock->setVisible(false);
    this->_particleGin->setVisible(false);
    this->_particleBigGin->setVisible(true);

    this->_particleKnock->pauseEmissions();
    this->_particleGin->pauseEmissions();

//    this->_particleBigGin->resetSystem();
    this->_particleBigGin->resumeEmissions();
}

void GPGinControlCardLayer::hideAllParticle()
{
    this->_layoutParticle->setVisible(false);

    this->_particleKnock->setVisible(false);
    this->_particleGin->setVisible(false);
    this->_particleBigGin->setVisible(false);

    this->_particleKnock->pauseEmissions();
    this->_particleGin->pauseEmissions();
    this->_particleBigGin->pauseEmissions();
}

void GPGinControlCardLayer::visibleTurnLayout(bool visible)
{
   this->_layoutTurn->setVisible(visible);
}

void GPGinControlCardLayer::setTurn(int index)
{
    string text = "";
    float orgTextScale = this->_textTurn->getScale();

    if(index == MAIN_USER_SLOT_INDEX)
    {
        text = GET_TEXT_BY_ID(TXT_YOUR_TURN);

        this->_imageTurnBot->setVisible(false);
        this->_imageTurnUser->setVisible(true);
    }
    else
    {
        text = GET_TEXT_BY_ID(TXT_RIVAL_TURN);

        this->_imageTurnBot->setVisible(true);
        this->_imageTurnUser->setVisible(false);
    }

    this->_textTurn->runAction(Sequence::create(ScaleTo::create(0.1f, orgTextScale*1.2f),
                                                   CallFunc::create([=]{
                                                       this->_textTurn->setString(text);
                                                   }),
                                                   ScaleTo::create(0.1f, orgTextScale),
                                                   nullptr));

    if(this->_layoutTurn->isVisible() == false)
    {
//        this->visibleTurnLayout(true);
        this->visibleTurnLayout(false);
    }
}
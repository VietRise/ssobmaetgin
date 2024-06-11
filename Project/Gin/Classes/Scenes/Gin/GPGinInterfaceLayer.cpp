#include "GPGinInterfaceLayer.h"
#include "UserData/UserData.h"
#include "Helper/GameUtils.h"
#include "Define/ResourcesDefine.h"
#include "Define/GameDefine.h"
#include "Manager/SoundManager.h"
#include "Manager/TextManager.h"
#include "Manager/BoardManager.h"
#include "Helper/CardIndexConverter.h"
#include "GPGinScene.h"
#include "Scenes/CheatCardLayer.h"
#include "GPGinControlCardLayer.h"
#include "GinCard.h"
#include "Boards/VideoRewardsPopup.h"
#include "Native/CallToOtherLanguage.h"
#include "FirebaseGG/Firebase.h"
#include "Network/MessageSender.h"
#include "Manager/MyActionsManager.h"
#include "Helper/fmt/format.h"
#include "Manager/AdsManager.h"
#include "FirebaseGG/RemoteConfig.h"

#define TAG_ACTION_COUNTDOWN_HOURLY_BONUS 0
#define TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS 1
#define TAG_ACTION_DECOR_LAYOUT_TIMELINE 100

#define TABLE_SCALE 0.86f

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace std::placeholders;

float GPGinInterfaceLayer::_scaleDrawCard = 1.0f;
float GPGinInterfaceLayer::_scaleTakeCard = 1.0f;

GPGinInterfaceLayer::GPGinInterfaceLayer()
{
}

GPGinInterfaceLayer::~GPGinInterfaceLayer()
{
    GameUtils::releaseTemplate(_mapTemplate);
}

bool GPGinInterfaceLayer::init()
{
    if (!Layer::init())
        return false;
    _listTableMappingFilename.insert(pair<int, string>((int) TableType::TABLE_1, "ui/GameplayScene/TableLayout/TableLayout1.csb"));
    _numDrawCard = 52; //full card
    return true;
}

void GPGinInterfaceLayer::initLayout(cocos2d::Size screenSize, float scale)
{
    _screenSize = screenSize;
    Node* node  = CSLoader::createNode("ui/GameplayScene/Gin/GPGinInterfaceLayer.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();

    this->_layoutInterfaceRoot = static_cast<Layout*>(child);
    this->_layoutInterfaceRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->addChild(_layoutInterfaceRoot, kMiddleground);

    if (s_enableSafeArea)
    {
        float padding = SAFE_AREA_PADDING;

        LayoutComponent* layoutComponent = (LayoutComponent*) static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutContent"))->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + padding);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + padding);
        layoutComponent->refreshLayout();
    }

    if (s_resolution_ratio <= RESOLUTION_1_6)
    {
        Helper::doLayout(this);
        auto layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutContent"));

        float paddingWidth  = -1 * (screenSize.width - layoutContent->getContentSize().width) / 2;
        float paddingHeight = -1 * (screenSize.height - layoutContent->getContentSize().height) / 2;

        LayoutComponent* layoutComponent = (LayoutComponent*) layoutContent->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + paddingWidth);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + paddingWidth);
        layoutComponent->setTopMargin(layoutComponent->getTopMargin() + paddingHeight);
        layoutComponent->setBottomMargin(layoutComponent->getBottomMargin() + paddingHeight);
        layoutComponent->refreshLayout();

        layoutContent->setScale(scale);
    }

    Helper::doLayout(this);

    GameUtils::setLayoutTemplate(_mapTemplate, "LayoutEffectHint", node);
    GameUtils::setLayoutTemplate(_mapTemplate, "LayoutUserStatus", node);
    GameUtils::setLayoutTemplate(_mapTemplate, "LayoutStartRound", node);
    GameUtils::setLayoutTemplate(_mapTemplate, "LayoutEndMatchStatus", node);

    _layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutContent"));
    _nodeTable     = static_cast<Node*>(this->_layoutContent->getChildByName("NodeTable"));

    _buttonSetting = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonSettings"));
    _buttonSetting->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onSettingsButtonClick, this));

    this->_buttonTutorial = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonTutorial"));
    this->_buttonTutorial->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onTutorialButtonClick, this));

    this->_layoutButtonQuit    = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutButtonQuit"));
    this->_buttonQuitOpenState = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonOpenState"));
    this->_buttonQuitOpenState->setPressedActionEnabled(true);
    this->_buttonQuitOpenState->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onQuitButtonClick, this));

    this->_buttonQuitCloseState = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonCloseState"));
    this->_buttonQuitCloseState->setPressedActionEnabled(true);
    this->_buttonQuitCloseState->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onQuitButtonClick, this));
    this->setQuitState(false);

    _layoutRoomInfo       = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "RoomInfoLayout"));
    _textBetTitle         = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextBetTitle"));
    _textBet              = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextBet"));
    _textBonusTitle       = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "TextBonusTitle"));
    _textBonus            = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "TextBonus"));
    _textRoundTitle       = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextRoundTitle"));
    _textRound            = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextRound"));
    _textMaxMultiplyTitle = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextMultiplyTitle"));
    _textMaxMultiply      = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextMultiply"));

    _textRoomTitle = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextRoomTitle"));
    _textRoom      = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoomInfo, "TextRoom"));

    _layoutContinue = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutContinue"));
    _layoutContinue->setVisible(false);
    _buttonContinue = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutContinue, "ButtonContinue"));
    _buttonContinue->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onContinueButtonClick, this));
    _buttonChooseBet = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutContinue, "ButtonChooseBet"));
    _buttonChooseBet->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onChooseBetButtonClick, this));

    _layoutContinuePos = _layoutContinue->getPosition();
    _buttonShowResult  = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonShowResult"));
    _buttonShowResult->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onButtonClick, this));
    _buttonShowResult->setVisible(false);

    // Layout cards on table

    _layoutCardsOnTable     = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutCardsOnTable"));
    _layoutDrawAndTakeCards = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutDrawAndTake"));
    //    _layoutDrawAndTakeCards->setVisible(false);


    _imageViewDrawCard = static_cast<ImageView*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ImageViewDrawCard"));
    _imageViewDrawCard->setOpacity(0);
    _imageViewDrawCard->setCascadeOpacityEnabled(false);
    _imageViewDrawCard->getChildByName("FakeDrawCard")->setVisible(false);
    GPGinInterfaceLayer::_scaleDrawCard = _imageViewDrawCard->getContentSize().width / GIN_WIDTH_CARD * _imageViewDrawCard->getScale();

    _imageViewTakeCard = static_cast<ImageView*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ImageViewTakeCard"));
    _imageViewTakeCard->setCascadeOpacityEnabled(false);
    _imageViewTakeCard->getChildByName("FakeTakeCard")->setVisible(false);
    GPGinInterfaceLayer::_scaleTakeCard = _imageViewTakeCard->getContentSize().width / GIN_WIDTH_CARD * _imageViewTakeCard->getScale();

    ImageView* imageviewBG = static_cast<ImageView*>(_imageViewTakeCard->getChildByName("ImageBG"));
    Rect throwCardArea;
    throwCardArea.origin = this->convertWorldPosToLayoutRootPos(imageviewBG->getParent()->convertToWorldSpace(imageviewBG->getBoundingBox().origin));
    throwCardArea.size   = imageviewBG->getBoundingBox().size;

//    throwCardArea.origin = this->convertWorldPosToLayoutRootPos(imageviewBG->getParent()->convertToWorldSpace(imageviewBG->getPosition()));
//    throwCardArea.size   = imageviewBG->getContentSize();

    _listHintDropArea.insert(std::pair<HintDropPlaceType, Rect>(HintDropPlaceType::HintDropPlaceAtThrowCardsArea, throwCardArea));

    _buttonShowThrowCard = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonShowThrowCard"));
    _buttonShowThrowCard->setVisible(false);

    _nodeFakeDrawCard = this->_imageViewDrawCard->getChildByName("NodeCard");

    _layoutNumCardInDraw          = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutTextNumCardInDraw"));
    _posOriginalTextNumCardInDraw = _layoutNumCardInDraw->getPosition();

    _layoutSlot    = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutUserSlots"));
    _layoutRewards = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutRewards"));

    _buttonFreeGold = static_cast<Button*>(Helper::seekWidgetByName(_layoutRewards, "ButtonFreeGold"));
    _buttonFreeGold->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onButtonClick, this));
    _textBMFreeGold     = static_cast<TextBMFont*>(Helper::seekWidgetByName(_buttonFreeGold, "TextBMFreeGold"));
    _textVideoBonusHint = static_cast<Text*>(Helper::seekWidgetByName(_buttonFreeGold, "TextVideoHint"));

    _buttonVideoBonusReward = static_cast<Button*>(Helper::seekWidgetByName(_layoutRewards, "ButtonVideoBonusReward"));
    _buttonVideoBonusReward->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onButtonClick, this));
    _buttonVideoBonusReward->setVisible(false);
    _textBMVideoBonusReward      = static_cast<Text*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "TextBMMesage"));
    _imageVideoBonusReward       = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageBox"));
    _imageVideoBonusRewardGlow0  = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageGlow0"));
    _imageVideoBonusRewardGlow1  = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageGlow1"));
    _imageVideoBonusRewardScaleX = _imageVideoBonusReward->getScaleX();
    _imageVideoBonusRewardScaleY = _imageVideoBonusReward->getScaleY();

    _layoutEventDecor = static_cast<Layout*>(Helper::seekWidgetByName(_layoutInterfaceRoot, "PanelEventBackground"));
    checkAndEnableEventDecorLayer();

    _buttonHourlyBonus        = static_cast<Button*>(Helper::seekWidgetByName(_layoutRewards, "ButtonHourlyBonus"));
    _textHourlyBonusMoney     = static_cast<TextBMFont*>(Helper::seekWidgetByName(_buttonHourlyBonus, "BMLabelMoney"));
    _textHourlyBonusCountDown = static_cast<TextBMFont*>(Helper::seekWidgetByName(_buttonHourlyBonus, "BMLabelCountDown"));
    _textHourlyBonusTitle     = static_cast<Text*>(Helper::seekWidgetByName(_buttonHourlyBonus, "BMLabelTitle"));
    _imgHourlyBonusGet        = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonHourlyBonus, "ImgGet"));
    _textHourlyBonusGetNow    = static_cast<TextBMFont*>(Helper::seekWidgetByName(_buttonHourlyBonus, "BMLabelGetNow"));
    _buttonHourlyBonus->setPressedActionEnabled(false);
    _textHourlyBonusCountDown->setString("");
    _buttonHourlyBonus->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onButtonClick, this));
    _posButtonHourlyBonus = _buttonHourlyBonus->getPosition();

    _textHourlyBonusTitle->setString(GET_TEXT_BY_ID(TXT_HOURLY_BONUS_TITLE));
    _textHourlyBonusGetNow->setString(GET_TEXT_BY_ID(TXT_GET_NOW));
    _textBMVideoBonusReward->setString(GET_TEXT_BY_ID(TXT_GET_REWARD));

    this->_listDealCardPos.clear();
    this->_listShowCardPos.clear();
    this->_listThrowCardPos.clear();
    this->_listShowStatusPos.clear();
    this->_listHandCardPos.clear();
    this->_listShowCardGlobalPos.clear();
    this->_listHandCardGlobalPos.clear();

    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        Layout* layoutSlot          = nullptr;
        Layout* layoutSlotForMarker = nullptr;
        if (i == MAIN_USER_SLOT_INDEX)
        {
            layoutSlot            = static_cast<Layout*>(_layoutSlot->getChildByName("LayoutSlotMain"));
            layoutSlotForMarker   = static_cast<Layout*>(_layoutCardsOnTable->getChildByName("LayoutCardsSlotMain"));
            _posScoreNodeMainUser = layoutSlot->convertToWorldSpace(layoutSlot->getChildByName("ScoreNodeMain")->getPosition());
        }
        else
        {
            layoutSlot          = static_cast<Layout*>(_layoutSlot->getChildByName(StringUtils::format("LayoutSlot%d", i)));
            layoutSlotForMarker = static_cast<Layout*>(_layoutCardsOnTable->getChildByName(StringUtils::format("LayoutCardsSlot%d", i)));
        }

        Node* dealCardMarker = static_cast<Node*>(layoutSlot->getChildByName("MarkerDealCardPos"));
        this->_listDealCardPos.push_back(layoutSlot->convertToWorldSpace(dealCardMarker->getPosition()));
        this->_listThrowCardPos.push_back(this->getTakeCardPos());

        Node* showStatusMaker = static_cast<Node*>(layoutSlotForMarker->getChildByName("MarkerShowStatusPos"));
        this->_listShowStatusPos.push_back(this->convertWorldPosToLayoutRootPos(layoutSlotForMarker->convertToWorldSpace(showStatusMaker->getPosition())));

        Node* handCardMaker = static_cast<Node*>(layoutSlot->getChildByName("MarkerHandCard"));
        this->_listHandCardPos.push_back(this->convertWorldPosToLayoutRootPos(layoutSlot->convertToWorldSpace(handCardMaker->getPosition())));

        this->_listHandCardGlobalPos.push_back(this->convertWorldPosToLayoutRootPos(layoutSlot->convertToWorldSpace(handCardMaker->getPosition())));

        Node* showCardMarker = static_cast<Node*>(layoutSlotForMarker->getChildByName("MarkerShowCardPos"));
        this->_listShowCardPos.push_back(this->convertWorldPosToLayoutRootPos(layoutSlotForMarker->convertToWorldSpace(showCardMarker->getPosition())));

        this->_listShowCardGlobalPos.push_back(layoutSlotForMarker->convertToWorldSpace(showCardMarker->getPosition()));

        ScoreBoxNode* scoreBoxNode = ScoreBoxNode::create();
        scoreBoxNode->show(false, 0, GinResultType::GIN_RESULT_TYPE_NONE);
        scoreBoxNode->retain();
        DeadwoodBoxNode* deadwoodBoxNode = nullptr;
        if (i == MAIN_USER_SLOT_INDEX)
        {
            deadwoodBoxNode = DeadwoodBoxNode::create();
            deadwoodBoxNode->hide();
            deadwoodBoxNode->retain();
        }
        auto imageviewBgCards     = static_cast<Layout*>(layoutSlotForMarker->getChildByName("ImageBG"));
        auto imageviewBgCardsAnim = static_cast<Layout*>(layoutSlotForMarker->getChildByName("ImageBGAnim"));
        auto layoutNotify         = static_cast<Layout*>(layoutSlotForMarker->getChildByName("LayoutNotify"));

        GPGinUserSlotNode* slot = GPGinUserSlotNode::create();
        slot->setgpGinInterfaceLayer(this);
        slot->setOriginalPosition(layoutSlot->convertToWorldSpace(layoutSlot->getContentSize() / 2));
        slot->initLayout(i, scoreBoxNode, deadwoodBoxNode, imageviewBgCards, imageviewBgCardsAnim, layoutNotify);

        this->_listSlotNode.pushBack(slot);

        if (i == MAIN_USER_SLOT_INDEX)
        {
            Rect dropCardMainUserArea;
            dropCardMainUserArea.origin = this->convertWorldPosToLayoutRootPos(layoutSlotForMarker->getParent()->convertToWorldSpace(layoutSlotForMarker->getBoundingBox().origin));
            dropCardMainUserArea.size   = layoutSlotForMarker->getBoundingBox().size;
        }
        else
        {
            Rect area;
            area.origin = this->convertWorldPosToLayoutRootPos(layoutSlotForMarker->getParent()->convertToWorldSpace(layoutSlotForMarker->getBoundingBox().origin));
            area.size   = layoutSlotForMarker->getBoundingBox().size;
        }
    }
    _layoutSlot->removeFromParent();
    showNotifyBack(false);
    showLayoutRewards(false);

    _ginScoreTable = GinScoreTable::create();
    _ginScoreTable->initLayout(this->_layoutInterfaceRoot, static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutTargetScoreTable")));
    _ginScoreTable->setShow(false);

    _resultScoreBox = ResultScoreBoxNode::create();
    this->_layoutContent->addChild(_resultScoreBox);
    _resultScoreBox->setPosition(this->_nodeTable->getPosition());
    _resultScoreBox->initLayout();
    _resultScoreBox->setVisible(false);

    _jackpotLayout = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutHitPot"));
    _textJackpotValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_jackpotLayout, "TextJackpotValue"));
    _textJackpotTitle = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_jackpotLayout, "TextJackpotTitle"));
    _buttonJackpotHelp = static_cast<Button*>(Helper::seekWidgetByName(this->_jackpotLayout, "ButtonHelp"));
    _buttonJackpotHelp->addClickEventListener(CC_CALLBACK_1(GPGinInterfaceLayer::onButtonClick, this));
    this->setJackpotValue(0);
    this->runJackpotBackgroundAnimation(true);
    _jackpotLayout->setVisible(false);

    _endGameResultNode = EndGameResultNode::create();
    _endGameResultNode->initLayout(this->_gPScene->getRule(), this->_gPScene->getCityType(), this->_gPScene->getTargetScore(), this->_gPScene);
    _endGameResultNode->setVisible(false);
    this->_layoutContent->addChild(_endGameResultNode, END_GAME_LAYOUT_Z_ORDER);
    _endGameResultNode->setPosition(this->_nodeTable->getPosition());

    _extraRoomInforLayout = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutContent, "ExtraRoomInfoLayout"));
    _textBMExtraRoomInfor = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_extraRoomInforLayout, "TextContent"));
    _firstDumpCardLayout = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutContent, "LayoutCard"));
    _firstDumpCard = GinCard::create(nullptr);
    _firstDumpCard->setBack(1);
    _firstDumpCard->setPosition(_firstDumpCardLayout->getContentSize()/2);
    _firstDumpCard->setScale(_firstDumpCardLayout->getContentSize().height/_firstDumpCard->getContentSize().height*1.2f);
    _firstDumpCardLayout->addChild(_firstDumpCard);
}

cocos2d::ui::Button* GPGinInterfaceLayer::getButtonShowThrowCards()
{
    return _buttonShowThrowCard;
}

void GPGinInterfaceLayer::initBackground(CityType cityType)
{
    auto background = _layoutInterfaceRoot->getChildByName("Background");
    if (background)
        background->setColor(Color3B::WHITE);
}

void GPGinInterfaceLayer::changeLanguage(Language language)
{
    _textRoomTitle->setString(GET_TEXT_BY_ID(TXT_ROOM_TITLE));
    _textBetTitle->setString(GET_TEXT_BY_ID(TXT_BET_IN_GAME_TITLE));
    _textRoundTitle->setString(GET_TEXT_BY_ID(TXT_ROUND));
    _textBonusTitle->setString(GET_TEXT_BY_ID(TXT_BONUS_TITLE));
    _textMaxMultiplyTitle->setString(GET_TEXT_BY_ID(TXT_MAX_MULTIPLY_TITLE));
    _textBMVideoBonusReward->setString(GET_TEXT_BY_ID(TXT_GET_REWARD));

    ((Text*) _buttonContinue->getChildByName("Label"))->setString(GET_TEXT_BY_ID(TXT_CONTINUE));
    ((Text*) _buttonChooseBet->getChildByName("Label"))->setString(GET_TEXT_BY_ID(TXT_CHOOSE_BET));
    _textVideoBonusHint->setString(GET_TEXT_BY_ID(TXT_VIDEO_BONUS_TITLE));
    ((Text*) _buttonShowResult->getChildByName("TextBMButton"))->setString(GET_TEXT_BY_ID(TXT_BUTTON_HISTORY));
    _ginScoreTable->changeLanguage(language);
    _resultScoreBox->changeLanguage(language);

    _textJackpotTitle->setString(GET_TEXT_BY_ID(TXT_JACKPOT));
}

vector<Point> GPGinInterfaceLayer::getListDealCardPosition()
{
    return this->_listDealCardPos;
}

std::vector<cocos2d::Point> GPGinInterfaceLayer::getListDealCardPosition(const std::vector<int>& orderIdx)
{
    vector<Point> listPos;
    for (int      i = 0; i < orderIdx.size(); i++)
        listPos.push_back(getDealHandPosition(orderIdx[i]));
    return listPos;
}

const std::map<HintDropPlaceType, cocos2d::Rect>& GPGinInterfaceLayer::getHintDropPlaceArea()
{
    return _listHintDropArea;
}

GPGinUserSlotNode* GPGinInterfaceLayer::getMainUserSlot()
{
    if (this->_listSlotNode.size() > 1)
        return this->_listSlotNode.at(MAIN_USER_SLOT_INDEX);
    return nullptr;
}

GPGinUserSlotNode* GPGinInterfaceLayer::getUserSlot(int index)
{
    if (this->_listSlotNode.size() > 1)
        return this->_listSlotNode.at(index);
    return nullptr;
}

int GPGinInterfaceLayer::getNumDrawCard()
{
    return _numDrawCard;
}

void GPGinInterfaceLayer::setNumDrawCard(int numTakeCard, bool drawCard, bool showFakeCard/* = true*/)
{
    _numDrawCard = numTakeCard;
    ((TextBMFont*) _layoutNumCardInDraw->getChildByName("Text"))->setString(__String::createWithFormat("%d", _numDrawCard)->getCString());
    _posRealDrawCardAtBase = this->getDrawCardPos() + Vec2(-0.2f * (_numDrawCard - 1), 0.3f * (_numDrawCard - 1));
    _layoutNumCardInDraw->setPosition(_posOriginalTextNumCardInDraw + Vec2(-0.2f * (_numDrawCard - 1), 0.3f * (_numDrawCard - 1)));
    // Meaning reset cards and move it to center of table
    if (_numDrawCard == MAXIMUM_NUMBER_CARD)
    {
        initFakeCards();
        for (int i = 0; i < _numDrawCard; i++)
            _listFakeCardDrawCard.at(i)->setVisible(showFakeCard);
    }
    else
    {
        if (_numDrawCard == 0)
        {
            this->setVisibleDrawCard(false);
        }
        else
        {
            initFakeCards();
            if (drawCard)
                _listFakeCardDrawCard.at(MAXIMUM_NUMBER_CARD - 1 - numTakeCard)->setVisible(false);
            else
            {
                for (int i = 0; i < MAXIMUM_NUMBER_CARD; i++)
                {
                    if (showFakeCard)
                    {
                        if (i < MAXIMUM_NUMBER_CARD - numTakeCard)
                            _listFakeCardDrawCard.at(i)->setVisible(false);
                        else
                            _listFakeCardDrawCard.at(i)->setVisible(true);
                    }
                    else
                        _listFakeCardDrawCard.at(i)->setVisible(false);
                }
            }
        }
    }
}

Sprite* GPGinInterfaceLayer::getLastDrawCard()
{
    if (_numDrawCard > 0)
        return _listFakeCardDrawCard.at(MAXIMUM_NUMBER_CARD - _numDrawCard);
    return nullptr;
}

void GPGinInterfaceLayer::addSlot(GinSlotInfo* slotInfo)
{
    int slotIdx = slotInfo->getSlotIdx();
    if (slotIdx != -1)
    {
        GPGinUserSlotNode* slotNode = getSlotNode(slotIdx);
        slotNode->reset();
        slotNode->setActive(true);
        slotNode->setSlotInfo(slotInfo);
        slotNode->updateInfo();
        slotNode->stopAllActions();
        slotNode->setPosition(this->_layoutContent->convertToNodeSpace(slotNode->getOriginalPosition()));
        if (slotNode->getParent() == nullptr)
        {
            this->_layoutContent->addChild(slotNode, USER_INFO_LAYOUT_Z_ORDER);
            this->_layoutContent->addChild(slotNode->getScoreBoxNode(), USER_INFO_LAYOUT_Z_ORDER);
            if (slotNode->getDeadwoodBoxNode() != nullptr)
                this->_layoutContent->addChild(slotNode->getDeadwoodBoxNode(), USER_INFO_LAYOUT_Z_ORDER);
        }
        runUserJoinRoomAnimation(slotIdx);
        if (slotInfo->isMain())
            _ginScoreTable->setUserAvatarLink(slotInfo->getAvatarLink());
        else
            _ginScoreTable->setBotAvatarLink(slotInfo->getAvatarLink());
    }
}

void GPGinInterfaceLayer::setMainSlot(GinSlotInfo* slotInfo, cocos2d::Node* parent)
{
    GPGinUserSlotNode* slotNode = getMainUserSlot();
    slotNode->reset();
    slotNode->setActive(true);
    slotNode->setSlotInfo(slotInfo);
    slotNode->updateInfo();
    slotNode->stopAllActions();
    if (slotNode->getParent() == nullptr)
    {
        slotNode->setPosition(parent->convertToNodeSpace(slotNode->getOriginalPosition()));
        parent->addChild(slotNode, (int) ZorderControlLayer::CARD_ON_HAND_USER + 1);
        parent->addChild(slotNode->getScoreBoxNode(), (int) ZorderControlLayer::CARD_ON_HAND_USER + 1);
        if (slotNode->getDeadwoodBoxNode() != nullptr)
            parent->addChild(slotNode->getDeadwoodBoxNode(), (int) ZorderControlLayer::CARD_ON_HAND_USER + 1);
    }
    if (slotInfo->isMain())
        _ginScoreTable->setUserAvatarLink(slotInfo->getAvatarLink());
    else
        _ginScoreTable->setBotAvatarLink(slotInfo->getAvatarLink());
}

int GPGinInterfaceLayer::getEmptySlotIdx()
{
    if (!this->_listSlotNode.at(2)->isActive())
        return 2;
    else
    {
        for (int i = 1; i < this->_listSlotNode.size(); i++)
            if (!this->_listSlotNode.at(i)->isActive())
                return i;
    }
    return -1;
}

cocos2d::Point GPGinInterfaceLayer::getSlotPos(int order)
{
    return _listSlotNode.at(order)->convertToWorldSpace(Vec2(0, 0));
}

void GPGinInterfaceLayer::updateMoney()
{
    for (int i = 0; i < this->_listSlotNode.size(); i++)
        updateMoney(i);
}

void GPGinInterfaceLayer::updateMoney(int orderIdx)
{
    if (orderIdx >= 0 && orderIdx < this->_listSlotNode.size())
        if (this->_listSlotNode.at(orderIdx)->isActive())
            this->_listSlotNode.at(orderIdx)->updateMoney();
}

void GPGinInterfaceLayer::updateLevel()
{
    for (int i = 0; i < this->_listSlotNode.size(); i++)
        updateLevel(i);
}

void GPGinInterfaceLayer::updateLevel(int orderIdx)
{
    if (orderIdx >= 0 && orderIdx < this->_listSlotNode.size())
        if (this->_listSlotNode.at(orderIdx)->isActive())
            this->_listSlotNode.at(orderIdx)->updateLevel();
}

void GPGinInterfaceLayer::reset()
{
    this->setLayoutDrawAndTakeVisible(true);
    this->setNumDrawCard(MAXIMUM_NUMBER_CARD, false, true);
    for (int i = 0; i < _listSlotNode.size(); i++)
    {
        GPGinUserSlotNode* userSlotNode = _listSlotNode.at(i);
        if (userSlotNode->isActive())
        {
            this->setVisibleScoreBox(false, 0, 0, userSlotNode->getSlotIdx(), Vec2::ZERO, GinResultType::GIN_RESULT_TYPE_NONE);
            userSlotNode->showNotify(false, "");
            userSlotNode->showTextDropArea(true);
            if (userSlotNode->getSlotInfo()->isMain())
                _ginScoreTable->setUserAvatarLink(userSlotNode->getSlotInfo()->getAvatarLink());
            else
                _ginScoreTable->setBotAvatarLink(userSlotNode->getSlotInfo()->getAvatarLink());
        }
    }
    this->_extraRoomInforLayout->setVisible(false);
}

void GPGinInterfaceLayer::setRoomInfo(CityType cityType, TableType tableType, double betMoney, int round, float multiplyValue)
{
    this->initTable(tableType);
    this->setBetMoney(betMoney);
    this->initBackground(cityType);
    this->setRoundNumber(round);
    this->setMaxMultiplyNumber(multiplyValue);
}

void GPGinInterfaceLayer::setRoomName(const std::string& name)
{
    _textRoom->setString(name);
}

void GPGinInterfaceLayer::setBetMoney(long long money)
{
    this->_textBet->setString(GameUtils::MoneyFormat(money, "$"));
}

void GPGinInterfaceLayer::setRoundNumber(int round)
{
    this->_textRound->setString(fmt::format("{0}", round));
}

void GPGinInterfaceLayer::setMaxMultiplyNumber(float value)
{
    this->_textMaxMultiply->setString(StringUtils::format(" x%.0f", value));
}

GPGinUserSlotNode* GPGinInterfaceLayer::getEmptySlotNode()
{
    int idx = getEmptySlotIdx();
    if (idx == -1)
        return nullptr;
    return this->_listSlotNode.at(idx);
}

GPGinUserSlotNode* GPGinInterfaceLayer::getSlotNode(int slotIdx)
{
    int orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx);
    if (orderIdx >= 0 && orderIdx < this->_listSlotNode.size())
        return this->_listSlotNode.at(orderIdx);
    return nullptr;
}

void GPGinInterfaceLayer::initTable(TableType type)
{
    Layout* layout = getTableLayout(type);
    if (layout != nullptr)
    {
        layout->setPosition(Point(0, 0));
        this->_nodeTable->removeChildByName("Table");
        this->_nodeTable->addChild(layout);
    }
}

Layout* GPGinInterfaceLayer::getTableLayout(TableType type)
{
    Layout   * layout    = loadTableLayout(_listTableMappingFilename[(int) TableType::TABLE_1]);
    Node     * nodeTable = static_cast<Node*>(layout->getChildByName("NodeTable"));
    ImageView* imageView = ImageView::create();
    if (imageView != nullptr)
        imageView->loadTexture(Resources::PNG::Img_InGameTable1);

    nodeTable->removeChildByName("Table");
    nodeTable->addChild(imageView);
    switch (type)
    {
        case TableType::TABLE_1:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable1);
            break;
        }
        case TableType::TABLE_2:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable2);
            break;
        }
        case TableType::TABLE_3:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable3);
            break;
        }
        case TableType::TABLE_4:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable4);
            break;
        }
        case TableType::TABLE_5:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable5);
            break;
        }
        case TableType::TABLE_6:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable6);
            break;
        }
        case TableType::TABLE_7:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable7);
            break;
        }
        case TableType::TABLE_8:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable8);
            break;
        }
        case TableType::TABLE_9:
        {
            imageView->loadTexture(Resources::PNG::Img_InGameTable9);
            break;
        }
        default:
            break;
    }
    layout->setName("Table");


    if(s_resolution_ratio <= RESOLUTION_1_6)
    {
        layout->setScale(TABLE_SCALE/RESOLUTION_1_6_SCALE);
    }
    else if (s_resolution_ratio <= RESOLUTION_1_4)
    {
        layout->setScale(TABLE_SCALE/RESOLUTION_1_4_SCALE);
    }
    else
    {
        layout->setScale(TABLE_SCALE);
    }

    return layout;
}

Layout* GPGinInterfaceLayer::loadTableLayout(const std::string& fileName)
{
    Node* node  = CSLoader::createNode(fileName);
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    return static_cast<Layout*>(child);
}

void GPGinInterfaceLayer::setQuitState(bool quit)
{
    this->_quitState = quit;
    if (quit)
    {
        this->_buttonQuitOpenState->setVisible(true);
        this->_buttonQuitCloseState->setVisible(false);
        onQuit(true);
    }
    else
    {
        this->_buttonQuitOpenState->setVisible(false);
        this->_buttonQuitCloseState->setVisible(true);
    }
}

bool GPGinInterfaceLayer::getQuitState()
{
    return this->_quitState;
}

void GPGinInterfaceLayer::runUserJoinRoomAnimation(int slotIdx)
{
    int orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx);
    if (orderIdx >= 0 && orderIdx < this->_listSlotNode.size())
    {
        GPGinUserSlotNode* slotNode = getSlotNode(slotIdx);
        if (slotNode != nullptr)
        {
            Point desPos = slotNode->getPosition();
            slotNode->setOpacity(0);
            switch (orderIdx)
            {
                case 1:
                {
                    Point srcPos = desPos;
                    srcPos.y = srcPos.y + 150;
                    slotNode->setPosition(srcPos);
                    break;
                }
                default:
                    break;
            }
            slotNode->runAction(Spawn::create(EaseExponentialOut::create(MoveTo::create(0.6f, desPos)), FadeIn::create(0.6f), nullptr));
        }
    }
}

Point GPGinInterfaceLayer::getDealHandPosition(int orderIdx)
{
    return this->_listDealCardPos[orderIdx];
}

cocos2d::Vec2 GPGinInterfaceLayer::getRealCardOnDrawCardPosition()
{
    return this->_posRealDrawCardAtBase;
}

void GPGinInterfaceLayer::onButtonClick(cocos2d::Ref* sender)
{
    if (sender == _buttonTutorial)
    {
    }
    else if (sender == _buttonFreeGold)
    {
        bool adsIsAvailable = false;
        adsIsAvailable = AdsManager::getInstance()->isRewardedAdsAvailable();
        if (adsIsAvailable)
        {
            Firebase::logOpenWatchVideo("available");
            AdsManager::getInstance()->showRewardedAds(VideoAdsRequestSourceEnum::OLD_VIDEO_REWARD);
        }
        else
        {
            if (!CallToOtherLanguage::getInstance()->checkInternet())
            {
                Firebase::logOpenWatchVideo("no internet");
                CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_1));
            }
            else
            {
                AdsManager::getInstance()->fetchRewardedAds();
                Firebase::logOpenWatchVideo("not available");
                CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_2));
            }
        }
    }
    else if (sender == _buttonVideoBonusReward)
    {
        MessageSender::requestVideoRewardInfo();
    }
    else if (sender == _buttonHourlyBonus)
    {
        MessageSender::requestHourlyBonus();
    }
    else if (sender == _buttonShowResult)
    {
        _gPScene->showRoundPointPopup();
        Firebase::logClickViewResult();
    }
    else if (sender == _buttonJackpotHelp)
    {
        NotifyPopup* notifyPopup = BoardManager::createNotifyPopup();
        notifyPopup->initValue(GET_TEXT_BY_ID(TXT_JACKPOT_HELP), nullptr);
        notifyPopup->attach(_gPScene, kAboveForeground);
    }
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void GPGinInterfaceLayer::onSettingsButtonClick(cocos2d::Ref* sender)
{
    LobbySettingBoard* lobbySettingBoard = BoardManager::createLobbySettingBoard();
    lobbySettingBoard->setTag((int) BoardID::LOBBY_SETTING_BOARD);
    lobbySettingBoard->initValue(1);
    lobbySettingBoard->attach(_gPScene, kAboveForeground);
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void GPGinInterfaceLayer::onTutorialButtonClick(cocos2d::Ref* sender)
{
    Firebase::logOpenTutorial();
    _gPScene->showTutorial(true);
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void GPGinInterfaceLayer::onQuitButtonClick(cocos2d::Ref* sender)
{
    if (sender == this->_buttonQuitOpenState)
    {
        setQuitState(false);
    }
    else if (sender == this->_buttonQuitCloseState)
    {
        if (_gPScene->inRound())
        {
            _gPScene->removePopup((int) BoardID::YESNO_CONFIRM_POPUP);
            auto quitConfirm = BoardManager::createYesNoConfirmPopup();
            quitConfirm->setContent(GET_TEXT_BY_ID(TXT_CAN_NOT_QUIT_ROUND));
            quitConfirm->setCallback([=](YesNoConfirmResult result) {
                if (result == YesNoConfirmResult::CONFIRM_YES)
                    setQuitState(true);
                quitConfirm->close();
            });
            quitConfirm->attach(_gPScene, kAboveForeground);
        }
        else
        {
            setQuitState(true);
        }
    }
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void GPGinInterfaceLayer::onContinueButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    _gPScene->onContinue2();
}

void GPGinInterfaceLayer::onChooseBetButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    _gPScene->onChooseBet();
}

void GPGinInterfaceLayer::onQuit(bool quit)
{
    _gPScene->onQuit();
}

void GPGinInterfaceLayer::addUserHandNode(cocos2d::Vector<GinUserHandNode*> listUserHandNode)
{
    for (int i = 0; i < listUserHandNode.size(); i++)
    {
        listUserHandNode.at(i)->setPosition(this->_layoutContent->convertToNodeSpace(_listHandCardPos.at(i)));
        this->_layoutContent->addChild(listUserHandNode.at(i), USER_INFO_LAYOUT_Z_ORDER - 1);
//        this->_layoutContent->addChild(listUserHandNode.at(i), USER_INFO_LAYOUT_Z_ORDER + 1);
    }
}

void GPGinInterfaceLayer::setGPScene(GPGinScene* gPScene)
{
    _gPScene = gPScene;
}

void GPGinInterfaceLayer::removeSlot(int orderIdx)
{
    if (orderIdx >= 0 && orderIdx < _listSlotNode.size())
    {
        _listSlotNode.at(orderIdx)->reset();
        _listSlotNode.at(orderIdx)->removeFromParent();
    }
}

vector<Point> GPGinInterfaceLayer::getListSlotDealCardPosition(std::vector<int> orderIdx)
{
    vector<Point> listPos;
    std::sort(orderIdx.begin(), orderIdx.end(), [](const int& i, const int& j) { return (i < j); });
    for (int      i = 0; i < orderIdx.size(); i++)
        listPos.push_back(this->_listDealCardPos[orderIdx[i]]);
    return listPos;
}

const std::vector<cocos2d::Point>& GPGinInterfaceLayer::getThrowPos()
{
    return _listThrowCardPos;
}

const std::vector<cocos2d::Point>& GPGinInterfaceLayer::getDealPos()
{
    return _listDealCardPos;
}

const std::vector<cocos2d::Point>& GPGinInterfaceLayer::getShowCardPos()
{
    return _listShowCardPos;
}

const std::vector<cocos2d::Point>& GPGinInterfaceLayer::getShowCardGlobalPos()
{
    return _listShowCardGlobalPos;
}

const std::vector<cocos2d::Point>& GPGinInterfaceLayer::getShowStatusPos()
{
    return _listShowStatusPos;
}

const std::vector<cocos2d::Point>& GPGinInterfaceLayer::getHandCardPos()
{
    return _listHandCardPos;
}

const std::vector<cocos2d::Point>& GPGinInterfaceLayer::getHandCardGlobalPos()
{
    return _listHandCardGlobalPos;
}

cocos2d::Vec2 GPGinInterfaceLayer::getDrawCardPos()
{
    return this->convertWorldPosToLayoutRootPos(_layoutDrawAndTakeCards->convertToWorldSpace(_imageViewDrawCard->getPosition()));
}

cocos2d::Vec2 GPGinInterfaceLayer::getTakeCardPos()
{
    return this->convertWorldPosToLayoutRootPos(_layoutDrawAndTakeCards->convertToWorldSpace(_imageViewTakeCard->getPosition()));
}

cocos2d::Vec2 GPGinInterfaceLayer::getScoreBoxMainUserPos()
{
    return _posScoreNodeMainUser;
}

void GPGinInterfaceLayer::setVisibleDrawCard(bool isVisible)
{
    _nodeFakeDrawCard->setVisible(isVisible);
    _layoutNumCardInDraw->setVisible(isVisible);
}

void GPGinInterfaceLayer::setVisibleLayoutDrawCard(bool isVisible)
{
    _imageViewDrawCard->setVisible(isVisible);
}

void GPGinInterfaceLayer::setVisibleLayoutTakeCard(bool isVisible)
{
    _imageViewTakeCard->setVisible(isVisible);
}

void GPGinInterfaceLayer::setAllUserTurnOff()
{
    for (int i = 0; i < _listSlotNode.size(); i++)
        this->setUserTurnOff(i);
}

void GPGinInterfaceLayer::setUserTurnOff(int orderIdx)
{
    if (orderIdx >= 0 && orderIdx < _listSlotNode.size())
    {
        if (_listSlotNode.at(orderIdx)->isActive())
        {
            _listSlotNode.at(orderIdx)->setOnTurn(false);
            _listSlotNode.at(orderIdx)->setPlaying(false);
        }
    }
}

void GPGinInterfaceLayer::setUserOnTurn(int orderIdx)
{
    if (orderIdx >= 0 && orderIdx < _listSlotNode.size())
    {
        for (int i = 0; i < _listSlotNode.size(); i++)
        {
            if (_listSlotNode.at(i)->isActive())
            {
                if (i == orderIdx)
                {
                    _listSlotNode.at(i)->setOnTurn(true);
                    _listSlotNode.at(i)->setPlaying(true);
                }
                else
                {
                    _listSlotNode.at(i)->setOnTurn(false);
                    _listSlotNode.at(i)->setPlaying(false);
                }
            }
        }
    }
}

void GPGinInterfaceLayer::updateStatusUser(GinMatchStatus status, int idInTurn, bool isResetName/* = false*/)
{
    for (int i = 0; i < (int) _listSlotNode.size(); i++)
    {
        if (idInTurn != i)
        {
            if (_listSlotNode.at(i)->isActive())
            {
                _listSlotNode.at(i)->getTextUserName()->setString(_listSlotNode.at(i)->getSlotInfo()->getUserName());
                if (isResetName)
                    this->hideStatus(i);
            }
        }
    }
}

void GPGinInterfaceLayer::onAvatarClick(GPGinUserSlotNode* userSlotNode)
{
    bool mainUser = userSlotNode->getSlotInfo()->isMain();
    if (!mainUser)
    {
#if(USE_CHEAT_CARD == 1)
        CheatCardLayer::getInstance()->showCheatLayer(userSlotNode->getSlotInfo()->getSlotIdx());
        CheatCardLayer::getInstance()->requestCheatCard(-1);
#endif
    }
}

void GPGinInterfaceLayer::finishDealCard(int knockScore)
{
    this->setNumDrawCard(GPGinSlotManager::getInstance()->getNumberRemainCardsAfterDealCardFinish(), false, true);
    this->setVisibleDeadwoodBox(true, 0, knockScore, MAIN_USER_SLOT_INDEX, _posScoreNodeMainUser);
}

void GPGinInterfaceLayer::showStatus(int orderIdx, FightStatus fightStatus)
{
    _gPScene->getGinEffect()->showEffectStatus(GameUtils::getLayoutTemplate(_mapTemplate, "LayoutUserStatus"), orderIdx, _listShowStatusPos[orderIdx], fightStatus);
}

void GPGinInterfaceLayer::showMatchEndStatus(GinMatchEndStatus status, cocos2d::Point positionOffset)
{
//    _gPScene->getGinEffect()->showEffectStatus(GameUtils::getLayoutTemplate(_mapTemplate, "LayoutUserStatus"), 0, this->_layoutContent->convertToWorldSpace(this->_nodeTable->getPosition()), FightStatus::KNOCK);
    _gPScene->getGinEffect()->showMatchEndStatusEffect(GameUtils::getLayoutTemplate(_mapTemplate, "LayoutEndMatchStatus"), status, this->_layoutContent->convertToWorldSpace(this->_nodeTable->getPosition()) + positionOffset, nullptr);
}

void GPGinInterfaceLayer::hideStatus(int orderIdx)
{
    _gPScene->getGinEffect()->hideEffectStatus(orderIdx);
}

void GPGinInterfaceLayer::setVisibleScoreBox(bool visible, int value, int score, int slotIdx, const cocos2d::Vec2& position, GinResultType type)
{
    ScoreBoxNode* scoreBoxNode = _listSlotNode.at(GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx))->getScoreBoxNode();
    scoreBoxNode->setPosition(scoreBoxNode->getParent()->convertToNodeSpace(position));
    scoreBoxNode->setScore(score);
    scoreBoxNode->show(visible, value, type);
}

void GPGinInterfaceLayer::setVisibleScoreBox(bool show, int slotIdx)
{
    ScoreBoxNode* scoreBoxNode = _listSlotNode.at(GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx))->getScoreBoxNode();
    scoreBoxNode->setVisible(show);
}


void GPGinInterfaceLayer::updateScoreBox(int value, int score, int slotIdx, const cocos2d::Vec2& position, GinResultType type)
{
    ScoreBoxNode* scoreBoxNode = _listSlotNode.at(GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx))->getScoreBoxNode();
    scoreBoxNode->runAction(Sequence::create(MoveTo::create(position.distance(scoreBoxNode->getPosition()) * TIME_MOVE_ANIMATION_PER_PIXEL, scoreBoxNode->getParent()->convertToNodeSpace(position)), CallFunc::create([=]() {
        scoreBoxNode->setScore(score);
        scoreBoxNode->show(true, value, type);
    }), nullptr));
}

int GPGinInterfaceLayer::getScoreBoxValue(int slotIdx)
{
    int orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx);
    ScoreBoxNode* scoreBoxNode = _listSlotNode.at(orderIdx)->getScoreBoxNode();
    return scoreBoxNode->getScore();
}

void GPGinInterfaceLayer::setVisibleDeadwoodBox(bool show, int score, int knockScore, int slotIdx, cocos2d::Vec2 position)
{
    int orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx);
    DeadwoodBoxNode* deadwoodBoxNode = _listSlotNode.at(orderIdx)->getDeadwoodBoxNode();
    if (deadwoodBoxNode != nullptr)
    {
        Vec2 pos = deadwoodBoxNode->getParent()->convertToNodeSpace(position);
        deadwoodBoxNode->setPosition(pos);
        if (show == true)
            deadwoodBoxNode->show(score, knockScore);
        else
            deadwoodBoxNode->hide();
    }
}

void GPGinInterfaceLayer::updateDeadwoodBox(int score, int knockScore, int slotIdx)
{
    int orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx);
    DeadwoodBoxNode* deadwoodBox = _listSlotNode.at(orderIdx)->getDeadwoodBoxNode();
    if (deadwoodBox != nullptr)
        deadwoodBox->setScoreAndKnockScore(score, knockScore);
}

int GPGinInterfaceLayer::getDeadwoodScore(int slotIdx)
{
    int result = 0;
    int orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx);
    DeadwoodBoxNode* deadwoodBox = _listSlotNode.at(orderIdx)->getDeadwoodBoxNode();
    if (deadwoodBox != nullptr)
        result = deadwoodBox->getScore();
    return result;
}

cocos2d::Size GPGinInterfaceLayer::getLayoutCardSize(int orderIdx)
{
    return _listSlotNode.at(orderIdx)->getLayoutCardOnTableSize();
}

void GPGinInterfaceLayer::showEffectScoreWinLose(bool win, long long money, bool finalRes, int orderIdx, bool fast /*= false*/)
{
    _gPScene->getGinEffect()->showEffectScoreWinLose(win, money, finalRes, this->getSlotPos(orderIdx), this->getPositionTextMoney(orderIdx), orderIdx, [=]() {
        this->updateMoney(orderIdx);
    }, fast);
}

void GPGinInterfaceLayer::showEffectRank(int orderIdx, GinResultType type, int rank)
{
    Vec2 offset = Vec2::ZERO;
    float heightMultiplier = 1.8f; //0.82f
    if(orderIdx == 0)
    {
        heightMultiplier = 2.3f;
        offset = Vec2(0, this->getUserSlot(orderIdx)->getLayoutRoot()->getContentSize().height * heightMultiplier);
    }
    else
    {
        heightMultiplier = 2.1f;
        offset = Vec2(0, -1 * this->getUserSlot(orderIdx)->getLayoutRoot()->getContentSize().height * heightMultiplier);
    }
    _gPScene->getGinEffect()->showEffectRank(this->getSlotPos(orderIdx) + offset, orderIdx, type, rank);
}

void GPGinInterfaceLayer::showLayoutContinue(bool show)
{
    _layoutContinue->setVisible(show);
    if (show)
    {
        _layoutContinue->setPosition(_layoutContinuePos + Vec2(300, 0));
        _layoutContinue->setOpacity(0);
        _layoutContinue->stopAllActions();
        _layoutContinue->runAction(Spawn::create(EaseBackOut::create(MoveTo::create(0.5f, _layoutContinuePos)), FadeIn::create(0.25f), nullptr));
//        if (_gPScene->getCityType() == CityType::CLASSIC_GIN)
//        {
//            _buttonChooseBet->setEnabled(true);
//            _buttonChooseBet->setColor(Color3B::WHITE);
//        }
//        else
//        {
//            _buttonChooseBet->setEnabled(false);
//            _buttonChooseBet->setColor(BUTTON_COLOR_DISABLE);
//        }
        _buttonChooseBet->setEnabled(true);
        _buttonChooseBet->setColor(Color3B::WHITE);
    }
}

void GPGinInterfaceLayer::showButtonResult(bool show)
{
    _buttonShowResult->setVisible(show);
}

void GPGinInterfaceLayer::showLayoutRewards(bool show)
{
    _layoutRewards->setVisible(show);
}

Vec2 GPGinInterfaceLayer::getPositionTextBonus()
{
    return this->_textBonus->convertToWorldSpaceAR(Vec2(0, 0));
}

cocos2d::Vec2 GPGinInterfaceLayer::getPositionTextMoney(int orderIdx)
{
    return _listSlotNode.at(orderIdx)->getPositionTextMoneyOnScreen();
}

void GPGinInterfaceLayer::showNotifyBack(bool show)
{
    auto notify = _layoutButtonQuit->getChildByName("ImageNotify");
    if (notify)
    {
        if (show)
        {
            notify->stopAllActions();
            notify->setVisible(true);
            notify->runAction(Sequence::create(Repeat::create(Sequence::create(EaseElasticOut::create(ScaleTo::create(1.0f, 0.8f)), ScaleTo::create(0.1f, 0.4f), nullptr), 3),
                                               ScaleTo::create(0.1f, 0.8f),
                                               nullptr));
        }
        else
        {
            notify->setVisible(false);
        }
    }
}

void GPGinInterfaceLayer::updateLayoutRewards(const rapidjson::Document& data)
{
    bool   result      = JsonHelper::getBool(data, KEY_RESULT, false);
    double availableAt = JsonHelper::getDouble(data, KEY_VIDEO_REWARD_REWARD_AVAILABLE_AT, 0);
    if (result)
    {
        _buttonFreeGold->setVisible(false);
        _buttonVideoBonusReward->setVisible(true);
        _imageVideoBonusReward->stopAllActions();
        _imageVideoBonusRewardGlow0->stopAllActions();
        _imageVideoBonusRewardGlow1->stopAllActions();

        _imageVideoBonusReward->setScale(this->_imageVideoBonusRewardScaleX, this->_imageVideoBonusRewardScaleY);

        _imageVideoBonusRewardGlow0->setVisible(false);
        _imageVideoBonusRewardGlow1->setVisible(false);

        bool showButton = (availableAt >= 0);
        if (showButton == true)
        {
            double countDown = std::max((double) 0, (availableAt - GameUtils::getCurrentTime()));
            this->runAction(Sequence::create(DelayTime::create(countDown), CallFunc::create([=] {
                _imageVideoBonusReward->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.4f, this->_imageVideoBonusRewardScaleX * 1.1f, this->_imageVideoBonusRewardScaleY * 0.9f),
                                                                                         ScaleTo::create(0.4f, this->_imageVideoBonusRewardScaleX * 0.9f, this->_imageVideoBonusRewardScaleY * 1.1f),
                                                                                         nullptr)));
                _imageVideoBonusRewardGlow0->setVisible(true);
                _imageVideoBonusRewardGlow1->setVisible(true);
                _imageVideoBonusRewardGlow0->runAction(RepeatForever::create(RotateBy::create(20.0f, 360)));
                _imageVideoBonusRewardGlow1->runAction(RepeatForever::create(RotateBy::create(20.0f, 360)));
            }), nullptr));
        }
    }
    else
    {
        _buttonFreeGold->setVisible(true);
        _buttonVideoBonusReward->setVisible(false);
        _imageVideoBonusReward->stopAllActions();
        _imageVideoBonusRewardGlow0->stopAllActions();
        _imageVideoBonusRewardGlow1->stopAllActions();
    }
    showLayoutRewards(true);
}

void GPGinInterfaceLayer::onVideoAdsMoney(const rapidjson::Document& data)
{
    double money = data[KEY_ADS_VIEW_MONEY].GetDouble();
    _textBMFreeGold->setString(StringUtils::format("+%s", GameUtils::MoneyFormat(money, "").c_str()));
}

void GPGinInterfaceLayer::onHourlyBonusData(const rapidjson::Document& data)
{
    double money      = data[KEY_MONEY].GetDouble();
    double bonusMoney = data[KEY_FREE_MONEY].GetDouble();
    double countDown  = data[KEY_COUNT_DOWN_TIME].GetDouble();
    int    result     = data[KEY_RESULT].GetInt();

    this->_hourlyBonusCountDown = countDown;
    this->_hourlyBonusMoney     = bonusMoney;
    if (result == 1)
    {
        float  scaleOrigin = _buttonHourlyBonus->getScale();
        double oldMoney    = UserData::getInstance()->getMoney();

        RewardPopup* notifyPopup = BoardManager::createRewardPopup();
        notifyPopup->attach(_gPScene, kAboveForeground);
        notifyPopup->setContent(GET_TEXT_BY_ID(TXT_GET_BONUS_MONEY));
        notifyPopup->setRewardContent(GameUtils::MoneyFormat(money - oldMoney, "+"));
        notifyPopup->setClaimRewardCallback([=]() {
            Action* action1 = Sequence::create(CallFunc::create(CC_CALLBACK_0(GPGinInterfaceLayer::setHourlyBonusButtonState,
                                                                              this,
                                                                              2,
                                                                              this->_hourlyBonusCountDown - GameUtils::getCurrentTime(),
                                                                              this->_hourlyBonusMoney)), DelayTime::create(2.0f), (FiniteTimeAction*) createMoveHourlyBonusButton(1), nullptr);
            action1->setTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
            this->_buttonHourlyBonus->stopActionByTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
            this->_buttonHourlyBonus->runAction(action1);

            GPGinSlotManager::getInstance()->getSlotInfo().at(0)->setMoney(money);
            UserData::getInstance()->setMoney(money);
            updateMoney();
            if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_GET_REWARDS_ENABLE, true))
                AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), false, "");
            if (UserData::getInstance()->isEnableLocalNotification())
                MessageSender::requestLocalNotificationData();
        });

        Action* action = Sequence::create((FiniteTimeAction*) createMoveHourlyBonusButton(0), nullptr);
        action->setTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
        this->_buttonHourlyBonus->stopActionByTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
        this->_buttonHourlyBonus->runAction(action);
    }
    else
    {
        if (this->_hourlyBonusCountDown - GameUtils::getCurrentTime() <= 0)
            setHourlyBonusButtonState(1, this->_hourlyBonusCountDown - GameUtils::getCurrentTime(), bonusMoney);
        else
            setHourlyBonusButtonState(2, this->_hourlyBonusCountDown - GameUtils::getCurrentTime(), bonusMoney);
    }
}

void GPGinInterfaceLayer::setHourlyBonusButtonState(int state, double countdown, double money)
{
    this->stopActionByTag(TAG_ACTION_COUNTDOWN_HOURLY_BONUS);
    _buttonHourlyBonus->getChildByName("PanelParticle")->setVisible(false);
    if (state == 0) //wait time
    {
        _buttonHourlyBonus->setVisible(false);
        _buttonHourlyBonus->setCascadeColorEnabled(false);
        _buttonHourlyBonus->setEnabled(false);

        _textHourlyBonusGetNow->setVisible(false);
        _textHourlyBonusMoney->setVisible(false);
        _textHourlyBonusTitle->setVisible(true);
        _imgHourlyBonusGet->setVisible(false);
        _textHourlyBonusCountDown->setVisible(true);

        _textHourlyBonusCountDown->setString(GameUtils::getTimeTextFromNumber(countdown));
        _textHourlyBonusMoney->setString(GameUtils::MoneyFormat(money, "+"));
    }
    else if (state == 1) //get now
    {
        _buttonHourlyBonus->setVisible(true);
        _buttonHourlyBonus->getChildByName("PanelParticle")->setVisible(true);
        _buttonHourlyBonus->setCascadeColorEnabled(true);
        _buttonHourlyBonus->setEnabled(true);

        _textHourlyBonusGetNow->setVisible(true);
        _textHourlyBonusMoney->setVisible(true);
        _textHourlyBonusTitle->setVisible(false);
        _imgHourlyBonusGet->setVisible(true);
        _textHourlyBonusCountDown->setVisible(false);

        _textHourlyBonusMoney->setString(GameUtils::MoneyFormat(money, "+"));

        auto scaleBy = ScaleBy::create(0.2f, 1.1f);
        _buttonHourlyBonus->runAction(Sequence::create(EaseBackOut::create(scaleBy), scaleBy->reverse(), nullptr));
    }
    else if (state == 2) //wait time
    {
        _buttonHourlyBonus->setVisible(true);
        _buttonHourlyBonus->setCascadeColorEnabled(true);
        _buttonHourlyBonus->setEnabled(false);

        _textHourlyBonusGetNow->setVisible(false);
        _textHourlyBonusMoney->setVisible(true);
        _textHourlyBonusTitle->setVisible(true);
        _imgHourlyBonusGet->setVisible(false);
        _textHourlyBonusCountDown->setVisible(true);

        _textHourlyBonusCountDown->setString(GameUtils::getTimeTextFromNumber(countdown));
        _textHourlyBonusMoney->setString(GameUtils::MoneyFormat(money, "+"));

        RepeatForever* repeatAction = RepeatForever::create(Sequence::create(DelayTime::create(1), CallFunc::create([=]() {
            if (this->_hourlyBonusCountDown - GameUtils::getCurrentTime() >= 0)
                _textHourlyBonusCountDown->setString(GameUtils::getTimeTextFromNumber(this->_hourlyBonusCountDown - GameUtils::getCurrentTime()));
            else
                setHourlyBonusButtonState(1, 0, money);
        }), nullptr));
        repeatAction->setTag(TAG_ACTION_COUNTDOWN_HOURLY_BONUS);
        this->runAction(repeatAction);
    }
}

Action* GPGinInterfaceLayer::createMoveHourlyBonusButton(int state)
{
    Point dstPos;
    int   opacity = 0;
    if (state == 0)
    {
        dstPos = this->_posButtonHourlyBonus + Point(this->_buttonHourlyBonus->getContentSize().width + 100, 0);
        this->_buttonHourlyBonus->setPosition(this->_posButtonHourlyBonus);
        opacity = 0;
    }
    else if (state == 1)
    {
        dstPos  = this->_posButtonHourlyBonus;
        opacity = 255;
    }
    return Sequence::create(DelayTime::create(0.2f), Spawn::create(EaseBackInOut::create(MoveTo::create(0.5f, dstPos)), FadeTo::create(0.5f, opacity), nullptr), nullptr);
}

void GPGinInterfaceLayer::checkAndEnableEventDecorLayer()
{

}

cocos2d::ui::Layout* GPGinInterfaceLayer::getLayoutEffectHint()
{
    return GameUtils::getLayoutTemplate(_mapTemplate, "LayoutEffectHint");
}

cocos2d::ui::Layout* GPGinInterfaceLayer::getLayoutStartRound()
{
    return GameUtils::getLayoutTemplate(_mapTemplate, "LayoutStartRound");
}

void GPGinInterfaceLayer::showNotify(bool show, int orderIdx, std::string text/*= ""*/)
{
    _listSlotNode.at(orderIdx)->showNotify(show, text);
}

void GPGinInterfaceLayer::showTextDropArea(bool show, int orderIdx)
{
    _listSlotNode.at(orderIdx)->showTextDropArea(show);
}

cocos2d::Vec2 GPGinInterfaceLayer::convertWorldPosToLayoutRootPos(const cocos2d::Vec2& posWorld)
{
    return _layoutInterfaceRoot->convertToNodeSpace(posWorld);
}

void GPGinInterfaceLayer::onQuitKeyBackClick()
{
    this->onQuitButtonClick(_quitState ? this->_buttonQuitOpenState : this->_buttonQuitCloseState);
}

void GPGinInterfaceLayer::resetLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData)
{
    this->_ginScoreTable->resetLayoutTargetScore(targetScore, listBetData);
}

void GPGinInterfaceLayer::updateLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData)
{
    this->_ginScoreTable->updateLayoutTargetScore(targetScore, listBetData);
}

void GPGinInterfaceLayer::hideLayerForQuit()
{
    auto      children   = _layoutInterfaceRoot->getChildren();
    for (auto child : children)
    {
        child->stopAllActions();
        child->setVisible(false);
    }
    auto      background = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutInterfaceRoot, "Background"));
    background->setVisible(true);
}

void GPGinInterfaceLayer::initFakeCards()
{
    if (_listFakeCardDrawCard.size() == 0)
    {
        std::string cardSpriteName = GinCardStore::getCardBackSpriteNameFromID(0);
        for (int    i              = 0; i < MAXIMUM_NUMBER_CARD; i++)
        {
            Sprite* fakeCard = Sprite::createWithSpriteFrameName(cardSpriteName);
            fakeCard->setScale(GPGinInterfaceLayer::_scaleDrawCard);
            fakeCard->setLocalZOrder(MAXIMUM_NUMBER_CARD - i);
            fakeCard->setOpacity(255.0f);
            fakeCard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            fakeCard->setPosition(Vec2(-0.2f * (MAXIMUM_NUMBER_CARD - 1 - i), 0.3f * (MAXIMUM_NUMBER_CARD - 1 - i)));
            _nodeFakeDrawCard->addChild(fakeCard);
            _listFakeCardDrawCard.push_back(fakeCard);
        }
    }
}

void GPGinInterfaceLayer::runShowScoreTableAnimation()
{
    this->_ginScoreTable->runShowAnimation(0, nullptr);
}

void GPGinInterfaceLayer::runHideScoreTableAnimation()
{
    this->_ginScoreTable->runHideAnimation(0, nullptr);
}

Point GPGinInterfaceLayer::getLastFakeDrawCardWorldPosition()
{
    for (int i = _listFakeCardDrawCard.size() - 1; i >= 0; --i)
    {
        int nextI = i - 1;
        if (nextI >= 0)
        {
            if (_listFakeCardDrawCard[i]->isVisible() == false && _listFakeCardDrawCard[nextI]->isVisible() == true)
            {
//                return _nodeFakeDrawCard->convertToWorldSpace(_listFakeCardDrawCard[i]->getPosition());
                return _listFakeCardDrawCard[i]->getParent()->convertToWorldSpace(_listFakeCardDrawCard[i]->getPosition());
            }
        }
    }

    return _nodeFakeDrawCard->convertToWorldSpace(Vec2::ZERO);
}

void GPGinInterfaceLayer::setLayoutDrawAndTakeVisible(bool visible)
{
    this->_layoutDrawAndTakeCards->setVisible(visible);
}

void GPGinInterfaceLayer::setScoreBoxesVisible(bool visible)
{
    for (int i = 0; i < _listSlotNode.size(); i++)
    {
        GPGinUserSlotNode* userSlotNode = _listSlotNode.at(i);
        if (userSlotNode->isActive())
        {
            this->setVisibleScoreBox(visible, 0, 0, userSlotNode->getSlotIdx(), Vec2::ZERO, GinResultType::GIN_RESULT_TYPE_NONE);
        }
    }
}

void GPGinInterfaceLayer::setScoreBoxColor(int value, int slotIdx)
{
    ScoreBoxNode* scoreBoxNode = _listSlotNode.at(GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx))->getScoreBoxNode();
    scoreBoxNode->setLayoutColor(value);
}

ResultScoreBoxNode* GPGinInterfaceLayer::getResultScoreBox()
{
    return this->_resultScoreBox;
}

void GPGinInterfaceLayer::setJackpotValue(long value)
{
    std::string money = GameUtils::MoneyFormat(value);
    _textJackpotValue->setString(money);
}

EndGameResultNode* GPGinInterfaceLayer::getEndGameResultNode()
{
    return this->_endGameResultNode;
}

GinScoreTable* GPGinInterfaceLayer::getScoreTable()
{
   return this->_ginScoreTable;
}

void GPGinInterfaceLayer::setVisibleUIOnEndGame()
{
    this->setScoreBoxesVisible(false);
}

void GPGinInterfaceLayer::playShowExtraRoomInformation(int cardValue, bool isSpadesDouble)
{
    _firstDumpCard->setCardIndex(cardValue, true);
    if(isSpadesDouble == true)
    {
        _textBMExtraRoomInfor->setString(GET_TEXT_BY_ID(TXT_SPADES_DOUBLE));
    }
    else
    {
        _textBMExtraRoomInfor->setString(GET_TEXT_BY_ID(TXT_NORMAL));
    }

    this->_extraRoomInforLayout->setVisible(true);

    Point orgExtraRoomInforPosition = this->_extraRoomInforLayout->getPosition();
    Size orgExtraRoomInforSize = this->_extraRoomInforLayout->getContentSize();
    float orgExtraRoomInforOpacity = this->_extraRoomInforLayout->getOpacity();

    Point newExtraRoomInforPosition = orgExtraRoomInforPosition;
    newExtraRoomInforPosition.x += orgExtraRoomInforSize.width;

    this->_extraRoomInforLayout->setOpacity(0);
    this->_extraRoomInforLayout->setPosition(newExtraRoomInforPosition);

    this->_extraRoomInforLayout->runAction(Spawn::create(FadeTo::create(0.5f, orgExtraRoomInforOpacity),
                                                         EaseBackOut::create(MoveTo::create(0.5f, orgExtraRoomInforPosition)),
                                                         nullptr));
}

void GPGinInterfaceLayer::playHideExtraRoomInformation()
{
    this->_extraRoomInforLayout->setVisible(true);

    Point orgExtraRoomInforPosition = this->_extraRoomInforLayout->getPosition();
    Size orgExtraRoomInforSize = this->_extraRoomInforLayout->getContentSize();
    float orgExtraRoomInforOpacity = this->_extraRoomInforLayout->getOpacity();

    Point newExtraRoomInforPosition = orgExtraRoomInforPosition;
    newExtraRoomInforPosition.x += orgExtraRoomInforSize.width;

    this->_extraRoomInforLayout->runAction(Sequence::create(
            Spawn::create(FadeTo::create(0.5f, 0),
                          EaseBackOut::create(MoveTo::create(0.5f, newExtraRoomInforPosition)),
                          nullptr),
            CallFunc::create([=]
                             {
                                 this->_extraRoomInforLayout->setOpacity(orgExtraRoomInforOpacity);
                                 this->_extraRoomInforLayout->setPosition(orgExtraRoomInforPosition);
                                 this->_extraRoomInforLayout->setVisible(false);
                             }),
            nullptr));
}

void GPGinInterfaceLayer::runJackpotBackgroundAnimation(bool animation)
{
    Sprite* bg = (Sprite*) _jackpotLayout->getChildByName("bg");
    if (bg)
    {
        if (animation)
        {
            _jackpotLayout->setColor(Color3B::WHITE);

            _jackpotLayout->getChildByName("PanelParticle")->setVisible(true);

            if (bg->getActionByTag(1) == nullptr)
            {
                bg->stopAllActions();
                Vector<SpriteFrame*> animFrames;
                animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg1.png"));
                animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg2.png"));
                Action* action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.6f)));
                action->setTag(1);
                bg->runAction(action);
            }
        }
        else
        {
            _jackpotLayout->setColor(Color3B(175,175,175));

            _jackpotLayout->getChildByName("PanelParticle")->setVisible(false);

            bg->stopAllActions();
            bg->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg.png"));
        }
    }
}

void GPGinInterfaceLayer::speedUpJackpotBackgroundAnimation(bool speed)
{
    Sprite* bg = (Sprite*) _jackpotLayout->getChildByName("bg");
    if (bg && bg->getActionByTag(1) != nullptr)
    {
        bg->stopAllActions();
        Vector<SpriteFrame*> animFrames;
        animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg1.png"));
        animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg2.png"));
        Action* action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, speed ? 0.12f : 0.6f)));
        action->setTag(1);
        bg->runAction(action);
    }
}

void GPGinInterfaceLayer::playShowJackpot()
{
    this->_jackpotLayout->setVisible(true);

    Point orgJackpotPosition = this->_jackpotLayout->getPosition();
    Size orgJackpotSize = this->_jackpotLayout->getContentSize();
    float orgJackpotOpacity = this->_jackpotLayout->getOpacity();

    Point newJackpotPosition = orgJackpotPosition;
    newJackpotPosition.x -= orgJackpotSize.width;

    this->_jackpotLayout->setOpacity(0);
    this->_jackpotLayout->setPosition(newJackpotPosition);

    this->_jackpotLayout->runAction(Spawn::create(FadeTo::create(0.5f, orgJackpotOpacity),
                                                         EaseBackOut::create(MoveTo::create(0.5f, orgJackpotPosition)),
                                                         nullptr));
}

void GPGinInterfaceLayer::playHideJackpot()
{
    this->_jackpotLayout->setVisible(true);

    Point orgJackpotPosition = this->_jackpotLayout->getPosition();
    Size orgJackpotSize = this->_jackpotLayout->getContentSize();
    float orgJackpotOpacity = this->_jackpotLayout->getOpacity();

    Point newJackpotPosition = orgJackpotPosition;
    newJackpotPosition.x -= orgJackpotSize.width;

    this->_jackpotLayout->runAction(Sequence::create(
            Spawn::create(FadeTo::create(0.5f, 0),
                          EaseBackOut::create(MoveTo::create(0.5f, newJackpotPosition)),
                          nullptr),
            CallFunc::create([=]
                             {
                                 this->_jackpotLayout->setOpacity(orgJackpotOpacity);
                                 this->_jackpotLayout->setPosition(orgJackpotPosition);
                                 this->_jackpotLayout->setVisible(false);
                             }),
            nullptr));
}

cocos2d::Point GPGinInterfaceLayer::getJackpotGlobalPosition()
{
    Point pos = this->_jackpotLayout->getParent()->convertToWorldSpace(this->_jackpotLayout->getPosition());

    pos.x = this->_jackpotLayout->getContentSize().width/2;

    return pos;
}

void GPGinInterfaceLayer::setJackpotVisible(bool visible)
{
    this->_jackpotLayout->setVisible(visible);
}

cocos2d::Size GPGinInterfaceLayer::getJackpotSizeAfterScale()
{
    Size size = this->_jackpotLayout->getContentSize();
    size.width *= this->_jackpotLayout->getScaleX();
    size.height *= this->_jackpotLayout->getScaleY();

    return size;
}

void GPGinInterfaceLayer::playMoveAndHideScoreBoxAnimation(int slotIdx, cocos2d::Point globalPosition, float delay)
{
    ScoreBoxNode* scoreBoxNode = _listSlotNode.at(GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx))->getScoreBoxNode();

    Point orgPos = scoreBoxNode->getPosition();
    float orgOpacity = scoreBoxNode->getOpacity();
    float orgScale = scoreBoxNode->getScale();

    Point pos = scoreBoxNode->getParent()->convertToNodeSpace(globalPosition);

    float duration = 0.5f;

    Vector<FiniteTimeAction*> listActions;
    listActions.pushBack(DelayTime::create(delay));
    listActions.pushBack(Spawn::create(EaseSineIn::create(MoveTo::create(duration, pos)),
                                        FadeTo::create(duration, 0),
                                        ScaleTo::create(duration, orgScale*0.5f),
                                        nullptr));
    listActions.pushBack(CallFunc::create([=]{
        scoreBoxNode->setVisible(false);
        scoreBoxNode->setPosition(orgPos);
        scoreBoxNode->setOpacity(orgOpacity);
        scoreBoxNode->setScale(orgScale);
    }));

    scoreBoxNode->runAction(Sequence::create(listActions));
}

void GPGinInterfaceLayer::playShowTableAnimation()
{
   this->playTableFadeAnimation(255,0.5f);
}

void GPGinInterfaceLayer::playHideTableAnimation()
{
    this->playTableFadeAnimation(0,0.5f);
}

void GPGinInterfaceLayer::playTableFadeAnimation(float duration, float opacity)
{
    Node* node = this->_nodeTable->getChildByName("Table");

    node->runAction(FadeTo::create(duration, opacity));
}

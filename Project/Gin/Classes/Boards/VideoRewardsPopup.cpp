#include "VideoRewardsPopup.h"
#include "Helper/GameUtils.h"
#include "Network/MessageHandler.h"
#include "Network/MessageSender.h"
#include "UserData/UserData.h"
#include "Native/CallToOtherLanguage.h"
#include "FirebaseGG/Firebase.h"
#include "Manager/BoardManager.h"
#include "Helper/fmt/format.h"
#include "Manager/MyActionsManager.h"
#include "Boards/NotifyPopup.h"
#include "AdapterClientServer/AdapterClientServer.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Manager/AdsManager.h"

#define PATH                        "ui/Common/LobbyVideoRewardInforPopup.csb"
#define ID_ROOT                        "Root"
#define BUTTON_CLOSE                "ButtonClose"
#define PANEL_CONTENT                "PanelContent"
#define PANEL_INFO                    "PanelRewardInfo"
#define BUTTON_CLOSE                "ButtonClose"
#define PANEL_MONEY                    "PanelMoney"
#define TEXT_USER_MONEY                "TextBMMoneyUser"
#define PANEL_ITEM                    "PanelItem"
#define PANEL_ITEM_LOTTERY            "PanelLottery"
#define PANEL_ITEM_IMAGE_BACKGROUND    "ImageBackground"
#define PANEL_ITEM_IMAGE_ICON        "ImageIcon"
#define PANEL_ITEM_IMAGE_CHECK        "ImageCheck"
#define PANEL_ITEM_IMAGE_ARROW        "ImageArrow"
#define PANEL_ITEM_LABEL_TEXT_1        "LabelText1"
#define PANEL_ITEM_LABEL_TEXT_2        "LabelText2"
#define PANEL_ITEM_LABEL_TEXT_3        "LabelText3"
#define PANEL_ITEM_BUTTON_VIDEO        "ButtonVideo"
#define PANEL_ITEM_TEXT_VIDEO        "LabelText"
#define PANEL_ITEM_TEXT_COUNTDOWN    "LabelTextCountDown"
#define PANEL_ITEM_TEXT_VIDEO_NOTICE "LabelTextNotice"
#define PANEL_ITEM_BUTTON_GET        "ButtonGet"
#define PANEL_ITEM_TEXT_GET            "LabelText"
#define    LIST_VIEW_ITEM                "ListViewContent"
#define TEXT_NOTICE_1                "LabelNotice1"
#define TEXT_NOTICE_2                "LabelNotice2"
#define TEXT_NOTICE_3                "LabelNotice3"
#define TEXT_NOTICE_4                "LabelNotice4"
#define TEXT_NOTICE_5                "LabelNotice5"
#define TEXT_NOTICE_6                "LabelNotice6"
#define PANEL_RESET_INFO            "PanelResetInfo"
#define TEXT_RESET_NOTICE            "LabelResetNotice"
#define TEXT_RESET_COUNTDOWN        "LabelResetCountdown"
#define PANEL_LOTTERY                "PanelLotteryAnim"
#define IMAGE_BOX                    "ImageBox"
#define PANEL_REWARD                "PanelReward"
#define IMAGE_BACKGROUND            "ImageBackground"
#define TEXT_VALUE                    "LabelValue"
#define BUTTON_OPEN                    "ButtonOpen"
#define TEXT_BUTTON_OPEN            "LabelText"
#define PANEL_EVENT_BACKGROUND        "PanelEventBackground"

#define TAG_ACTION_COUNTDOWN_CLAIM_BUTTON 2
#define TAG_ACTION_OPEN 3
#define TAG_ACTION_IMAGE_BOX_WAIT_OPEN 4

#define TAG_ACTION_DECOR_LAYOUT_TIMELINE 100

#define TIME_DELAY_OPEN 0.1f
#define TIME_ANIM_OPEN 0.2f
#define TIME_ANIM_INFO_ITEM 0.1f
#define TIME_ANIM_DELAY_UNIT_INFO_ITEM 0.05f

#define URGENT_COUNTDOWN_VALUE 1800
#define COLOR_RESET_COUNTDOWN_NORMAL cocos2d::Color3B(229,229,229)
#define COLOR_RESET_COUNTDOWN_URGENT cocos2d::Color3B(255,0,0)

USING_NS_CC;
using namespace ui;
using namespace std;

std::map<int, std::string> RewardItem::_mapRewardImages = {{ 0, "ui/ImagesCommon/timebonus_1.png" },
                                                           { 1, "ui/ImagesCommon/timebonus_2.png" },
                                                           { 2, "ui/ImagesCommon/timebonus_3.png" },
                                                           { 3, "ui/ImagesCommon/timebonus_4.png" }, };

RewardItem::RewardItem()
{
    _videoRewardPopup           = nullptr;
    _availableAt                = 0;
    _value                      = 0;
    _isLottery                  = false;
    _countdown                  = 0;
    _countdownLoadVideo         = 0;
    _countdownAfterRequestVideo = 0;
}

RewardItem::~RewardItem()
{
}

bool RewardItem::init()
{
    if (!Layout::init())
        return false;
    this->_countdownLoadVideo         = 0;
    this->_countdown                  = 0;
    this->_countdownAfterRequestVideo = 0;
    return true;
}

RewardItem* RewardItem::createRewardItem(VideoRewardsPopup* popup, cocos2d::ui::Layout* item)
{
    RewardItem* node = RewardItem::create();
    node->_rootNode         = item;
    node->_videoRewardPopup = popup;
    node->addChild(node->_rootNode);
    node->setAnchorPoint(Vec2(0.5f, 0.5f));
    node->setCascadeOpacityEnabled(true);
    node->_rootNode->setCascadeOpacityEnabled(true);

    node->_imageBackground = static_cast<ImageView*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_IMAGE_BACKGROUND));
    node->_imageBackground->setCascadeColorEnabled(true);
    node->_imageBackground->setCascadeOpacityEnabled(true);
    node->_imageIcon  = static_cast<ImageView*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_IMAGE_ICON));
    node->_imageArrow = static_cast<ImageView*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_IMAGE_ARROW));
    node->_imageCheck = static_cast<ImageView*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_IMAGE_CHECK));
    node->_text1      = static_cast<Text*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_LABEL_TEXT_1));
    node->_text2      = static_cast<TextBMFont*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_LABEL_TEXT_2));
    node->_text3      = static_cast<Text*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_LABEL_TEXT_3));
    if (node->_text3 != nullptr)
        node->_text3->setString(GET_TEXT_BY_ID(TXT_UP_TO));
    node->_buttonVideo     = static_cast<Button*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_BUTTON_VIDEO));
    node->_textButtonVideo = static_cast<Text*>(Helper::seekWidgetByName(node->_buttonVideo, PANEL_ITEM_TEXT_VIDEO));
    node->_textButtonVideo->setString(GET_TEXT_BY_ID(TXT_CLAIM_NOW));
    node->_textCountDown         = static_cast<TextBMFont*>(Helper::seekWidgetByName(node->_buttonVideo, PANEL_ITEM_TEXT_COUNTDOWN));
    node->_textButtonVideoNotice = static_cast<Text*>(Helper::seekWidgetByName(node->_buttonVideo, PANEL_ITEM_TEXT_VIDEO_NOTICE));
    node->_textButtonVideoNotice->setString(GET_TEXT_BY_ID(TXT_CLAIM_VIDEO_NOTICE));
    node->_buttonClaim     = static_cast<Button*>(Helper::seekWidgetByName(node->_rootNode, PANEL_ITEM_BUTTON_GET));
    node->_textButtonClaim = static_cast<Text*>(Helper::seekWidgetByName(node->_buttonClaim, PANEL_ITEM_TEXT_GET));

    node->_buttonVideo->addClickEventListener(CC_CALLBACK_1(RewardItem::onButtonClick, node));
    node->_buttonClaim->addClickEventListener(CC_CALLBACK_1(RewardItem::onButtonClick, node));

    Size size = Size(node->_imageBackground->getContentSize().width, item->getContentSize().height);
    node->setContentSize(size);
    node->_rootNode->setPosition(Vec2(size.width / 2, size.height / 2));

    node->_buttonVideo->setVisible(false);
    node->_buttonClaim->setVisible(false);
    ui::Helper::doLayout(node);
    return node;
}

void RewardItem::onButtonClick(cocos2d::Ref* sender)
{
    if (_videoRewardPopup->getState() == InterfacePopupBoard::State::OPENED)
    {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        if (sender == this->_buttonVideo)
        {
            bool adsIsAvailable = false;
            adsIsAvailable = AdsManager::getInstance()->isRewardedAdsAvailable();
            if (adsIsAvailable)
            {
                string text = "";
                if (!_isLottery)
                    text    = fmt::format("{0}\n{1}",
                                          fmt::format(GET_TEXT_BY_ID(TXT_CONFIRM_WATCH_VIDEO_POPUP_LINE1)),
                                          fmt::format(GET_TEXT_BY_ID(TXT_CONFIRM_WATCH_VIDEO_POPUP_1_LINE2), GameUtils::MoneyFormat(_value, "$")));
                else
                    text = fmt::format("{0}\n{1}",
                                       fmt::format(GET_TEXT_BY_ID(TXT_CONFIRM_WATCH_VIDEO_POPUP_LINE1)),
                                       fmt::format(GET_TEXT_BY_ID(TXT_CONFIRM_WATCH_VIDEO_POPUP_2_LINE2), GameUtils::MoneyFormat(_value, "$")));
                ActionConfirmPopup* confirmPopup = BoardManager::createActionConfirmPopup();
                confirmPopup->initValue(text, GET_TEXT_BY_ID(TXT_CONFIRM_WATCH_VIDEO_BUTTON_NO), GET_TEXT_BY_ID(TXT_CONFIRM_WATCH_VIDEO_BUTTON_YES), [=](ActionConfirmResult result) {
                    if (result == ActionConfirmResult::CONFIRM_ACTION)
                    {
                        Firebase::logOpenWatchVideo("available");
                        AdsManager::getInstance()->showRewardedAds(VideoAdsRequestSourceEnum::VIDEO_REWARD_SKIP_WAITING);
                    }
                    confirmPopup->close();
                });
                confirmPopup->attach(_videoRewardPopup, kAboveForeground);
            }
            else
            {
                if (!CallToOtherLanguage::getInstance()->checkInternet())
                {
                    Firebase::logOpenWatchVideo("no internet");
                    CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_3));
                }
                else
                {
                    CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_2));
                    AdsManager::getInstance()->fetchRewardedAds();
                    Firebase::logOpenWatchVideo("not available");
                }
            }
        }
        else if (sender == this->_buttonClaim)
        {
            this->_buttonClaim->setEnabled(false);
            MessageSender::requestVideoRewardClaim(false);
        }
    }
}

void RewardItem::setButtonVideoEnable(bool enable)
{
    this->_buttonVideo->setEnabled(enable);
}

void RewardItem::setButtonClaimEnable(bool enable)
{
    this->_buttonClaim->setEnabled(enable);
}

void RewardItem::setButtonVideoVisible(bool visible)
{
    this->_buttonVideo->setVisible(visible);
}

void RewardItem::setButtonClaimVisible(bool visible)
{
    this->_buttonClaim->setVisible(visible);
}

void RewardItem::setData(double value, int step, int currentStep, double availableAt, bool skippable, bool isLottery)
{
    this->_availableAt = availableAt;
    this->_value       = value;
    this->_isLottery   = isLottery;
    int countdown = availableAt - GameUtils::getCurrentTime();
    if (isLottery == false)
    {
        this->_imageIcon->setVisible(true);
        if (_mapRewardImages.find(step) != _mapRewardImages.end())
            this->_imageIcon->loadTexture(_mapRewardImages[step], Widget::TextureResType::PLIST);
        this->_text1->setString(fmt::format(GET_TEXT_BY_ID(TXT_STEP), step + 1));
        this->_text2->setString((step >= currentStep) ? GameUtils::MoneyFormat(value, "+") : GET_TEXT_BY_ID(TXT_CLAIMED));
    }
    else
    {
        this->_imageIcon->setVisible(true);
        this->_text1->setString(GET_TEXT_BY_ID(TXT_LUCKY_DRAW));
        this->_text2->setString(fmt::format(GET_TEXT_BY_ID(TXT_UP_TO), GameUtils::MoneyFormatShortCut(value, "")));
        this->_text2->setString(GameUtils::MoneyFormat(value, ""));

        float cScaleX  = _imageIcon->getScaleX();
        float cScaleY  = _imageIcon->getScaleY();
        float animTime = (step == currentStep) ? 0.5f : 5.0f;

        ScaleTo      * scale1 = ScaleTo::create(animTime, cScaleX * 1.1f, cScaleY * 0.9f);
        ScaleTo      * scale2 = ScaleTo::create(animTime, cScaleX * 0.9f, cScaleY * 1.1f);
        Sequence     * seq    = Sequence::create(scale1, scale2, nullptr);
        RepeatForever* action = RepeatForever::create(seq);
        _imageIcon->runAction(action);
    }
    this->_imageCheck->setVisible(false);
    if (step < currentStep)
    {
        Color3B dimColor = Color3B(100, 100, 100);
        this->_imageBackground->setColor(dimColor);
        this->_imageIcon->setColor(dimColor);
        this->_imageCheck->setColor(dimColor);
        this->_imageCheck->setVisible(true);
        if (this->_imageArrow != nullptr)
            this->_imageArrow->setColor(dimColor);
        this->_text1->setOpacity(120);
        this->_text2->setOpacity(120);
        if (this->_text3 != nullptr)
            this->_text3->setOpacity(170);
        this->_buttonVideo->setVisible(false);
        this->_buttonClaim->setVisible(false);
    }
    else if (step == currentStep)
    {
        this->_imageBackground->setColor(Color3B(255, 255, 255));
        this->_imageIcon->setColor(Color3B(255, 255, 255));
        if (this->_imageArrow != nullptr)
            this->_imageArrow->setColor(Color3B(255, 255, 255));
        this->_text1->setOpacity(255);
        this->_text2->setOpacity(255);
        if (this->_text3 != nullptr)
            this->_text3->setOpacity(255);
        if (countdown <= 0)
        {
            this->_buttonVideo->setVisible(false);
            this->_buttonClaim->setVisible(true);
            this->_textButtonClaim->setString(GET_TEXT_BY_ID(TXT_CLAIM_NOW));
        }
        else if (skippable)
        {
            this->_countdown = countdown;
            this->schedule(schedule_selector(RewardItem::tick), 1.0f, this->_countdown, 0);
        }
        else
        {
            this->_countdown = countdown;
            this->_buttonVideo->setVisible(false);
            this->_buttonClaim->setVisible(true);
            this->_textButtonClaim->setString(GameUtils::getTimeTextFromNumber(this->_countdown));

            DelayTime* delay       = DelayTime::create(1.0f);
            CallFunc * callback    = CallFunc::create([=]() {
                this->_countdown--;
                this->_textButtonClaim->setString(GameUtils::getTimeTextFromNumber(this->_countdown));
            });
            CallFunc * callbackEnd = CallFunc::create([=]() {
                this->_textButtonClaim->setString(GET_TEXT_BY_ID(TXT_CLAIM_NOW));
            });
            Sequence * action      = Sequence::create(Repeat::create(Sequence::create(delay, callback, nullptr), countdown), callbackEnd, nullptr);
            action->setTag(TAG_ACTION_COUNTDOWN_CLAIM_BUTTON);
            this->runAction(action);
        }
    }
    else
    {
        this->_imageBackground->setColor(Color3B(255, 255, 255));
        this->_imageIcon->setColor(Color3B(255, 255, 255));
        if (this->_imageArrow != nullptr)
            this->_imageArrow->setColor(Color3B(255, 255, 255));
        this->_text1->setOpacity(255);
        this->_text2->setOpacity(255);
        if (this->_text3 != nullptr)
            this->_text3->setOpacity(255);
        this->_buttonVideo->setVisible(false);
        this->_buttonClaim->setVisible(false);
    }
}

void RewardItem::tick(float dt)
{
    int oldCountDown = this->_countdown;
    this->_countdown = this->_availableAt - GameUtils::getCurrentTime();
    if (this->_countdown > 0)
    {
        if (this->_countdownAfterRequestVideo > 0)
            this->_countdownAfterRequestVideo -= (oldCountDown - this->_countdown);
        else if (this->_countdownLoadVideo > 0)
            this->_countdownLoadVideo -= (oldCountDown - this->_countdown);
        std::string text = GameUtils::getTimeTextFromNumber(this->_countdown);
        if (this->_countdownAfterRequestVideo > 0)
        {
            this->_buttonVideo->setVisible(false);
            this->_buttonClaim->setVisible(false);
        }
        else if (AdsManager::getInstance()->isRewardedAdsAvailable() && this->_countdownLoadVideo <= 0)
        {
            this->_buttonVideo->setVisible(true);
            this->_buttonClaim->setVisible(false);
            this->_textCountDown->setString(text);
        }
        else
        {
            this->_buttonVideo->setVisible(true);
            this->_buttonClaim->setVisible(false);
            this->_textButtonClaim->setString(text);
            this->_textCountDown->setString(text);
        }
    }
    else
    {
        this->unschedule(schedule_selector(RewardItem::tick));
        this->_buttonVideo->setVisible(false);
        this->_buttonClaim->setVisible(true);
        this->_textButtonClaim->setString(GET_TEXT_BY_ID(TXT_CLAIM_NOW));
    }
}

VideoRewardsPopup::VideoRewardsPopup()
{
    _applyTouchOutClose = false;
}

VideoRewardsPopup::~VideoRewardsPopup()
{
    CC_SAFE_RELEASE_NULL(this->_panel_item);
    CC_SAFE_RELEASE_NULL(this->_panel_item_lottery);
    this->_list_view_item = nullptr;
}

bool VideoRewardsPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    initScreen();
    this->_currentStep = -1;
    this->_rewardLevel = 0;
    _loterryAnimCallback = nullptr;
    _isPlayingAnimation  = false;
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_SKIP_RESPONSE, MESSAGEHANDLE_SELECTOR(VideoRewardsPopup::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_CLAIM_RESPONSE, MESSAGEHANDLE_SELECTOR(VideoRewardsPopup::onServerResponse));
    return true;
}

void VideoRewardsPopup::onEnter()
{
    InterfacePopupBoard::onEnter();
    if (!CallToOtherLanguage::getInstance()->checkInternet())
        this->checkToRemindUserTurnOnNetwork();
    else
    {
        AdsManager::getInstance()->loadInterstitialAds();
        AdsManager::getInstance()->fetchRewardedAds();
    }
}

void VideoRewardsPopup::onExit()
{
    InterfacePopupBoard::onExit();
    MessageHandler::getInstance()->unregisterHandleMessage(this);
}

void VideoRewardsPopup::changeLanguage(Language language)
{
    this->_text_notice_1->setString(GET_TEXT_BY_ID(TXT_NOTICE_1));
    this->_text_notice_2->setString(GET_TEXT_BY_ID(TXT_NOTICE_2));
    this->_text_notice_3->setString(GET_TEXT_BY_ID(TXT_NOTICE_3));
    this->_text_notice_4->setString(GET_TEXT_BY_ID(TXT_NOTICE_4));
    this->_text_notice_5->setString(GET_TEXT_BY_ID(TXT_NOTICE_5));
    this->_text_notice_6->setString(GET_TEXT_BY_ID(TXT_NOTICE_6));
    this->_text_reset_notice->setString(GET_TEXT_BY_ID(TXT_NOTICE_RESET));
    this->_text_reset_coundown->setString("");
    this->_text_button_open->setString(GET_TEXT_BY_ID(TXT_OPEN_REWARD));
}

void VideoRewardsPopup::initScreen()
{
    initWithColor(cocos2d::Color4B(0, 0, 0, 255));
    Node* node = CSLoader::createNode(PATH);
    Node* root = node->getChildByName(ID_ROOT);
    root->removeFromParent();

    this->_layoutRoot = static_cast<Layout*>(root);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_layoutRoot, kAboveForeground);
    this->setCascadeOpacityEnabled(true);
    this->_layoutRoot->setCascadeOpacityEnabled(true);
    ui::Helper::doLayout(this);

    Size  rootSize     = this->_layoutRoot->getContentSize();
    float contentScale = 1.0f;
    if (rootSize.width < 960)
        contentScale = rootSize.width / 960;
    this->_panel_info    = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_INFO));
    this->_panel_content = static_cast<Layout*>(Helper::seekWidgetByName(_panel_info, PANEL_CONTENT));
    this->_panel_content->setScale(contentScale);

    this->_buttonClose = static_cast<Button*>(Helper::seekWidgetByName(_panel_info, BUTTON_CLOSE));
    this->_buttonClose->setPressedActionEnabled(true);
    this->_buttonClose->addClickEventListener(CC_CALLBACK_1(VideoRewardsPopup::onButtonClick, this));

    this->_panel_item = static_cast<Layout*>(Helper::seekWidgetByName(_panel_content, PANEL_ITEM));
    this->_panel_item->removeFromParent();
    this->_panel_item->retain();

    this->_panel_money     = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_MONEY));
    this->_text_user_money = static_cast<TextBMFont*>(Helper::seekWidgetByName(_panel_money, TEXT_USER_MONEY));
    this->_text_user_money->setString(GameUtils::MoneyFormat(UserData::getInstance()->getMoney()));

    this->_panel_item_lottery = static_cast<Layout*>(Helper::seekWidgetByName(_panel_content, PANEL_ITEM_LOTTERY));
    this->_panel_item_lottery->removeFromParent();
    this->_panel_item_lottery->retain();

    this->_list_view_item = static_cast<ListView*>(Helper::seekWidgetByName(_panel_content, LIST_VIEW_ITEM));
    this->_list_view_item->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->_list_view_item->setMagneticType(ListView::MagneticType::BOTH_END);
    this->_list_view_item->setMagneticAllowedOutOfBoundary(false);
    this->_list_view_item->setItemsMargin(10);
    this->_list_view_item->setScrollBarEnabled(false);

    this->_text_notice_1 = static_cast<Text*>(Helper::seekWidgetByName(_panel_info, TEXT_NOTICE_1));
    this->_text_notice_2 = static_cast<Text*>(Helper::seekWidgetByName(_panel_info, TEXT_NOTICE_2));
    this->_text_notice_3 = static_cast<Text*>(Helper::seekWidgetByName(_panel_info, TEXT_NOTICE_3));
    this->_text_notice_4 = static_cast<Text*>(Helper::seekWidgetByName(_panel_info, TEXT_NOTICE_4));
    this->_text_notice_5 = static_cast<Text*>(Helper::seekWidgetByName(_panel_info, TEXT_NOTICE_5));
    this->_text_notice_6 = static_cast<Text*>(Helper::seekWidgetByName(_panel_info, TEXT_NOTICE_6));

    this->_panel_reset_info    = static_cast<Layout*>(Helper::seekWidgetByName(_panel_info, PANEL_RESET_INFO));
    this->_text_reset_notice   = static_cast<TextBMFont*>(Helper::seekWidgetByName(_panel_reset_info, TEXT_RESET_NOTICE));
    this->_text_reset_coundown = static_cast<TextBMFont*>(Helper::seekWidgetByName(_panel_reset_info, TEXT_RESET_COUNTDOWN));

    this->_panel_lottery      = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_LOTTERY));
    this->_image_box          = static_cast<ImageView*>(Helper::seekWidgetByName(_panel_lottery, IMAGE_BOX));
    this->_panel_reward       = static_cast<Layout*>(Helper::seekWidgetByName(_panel_lottery, PANEL_REWARD));
    this->_image_background   = static_cast<ImageView*>(Helper::seekWidgetByName(_panel_lottery, IMAGE_BACKGROUND));
    this->_text_value         = static_cast<TextBMFont*>(Helper::seekWidgetByName(_panel_lottery, TEXT_VALUE));
    this->_button_open_reward = static_cast<Button*>(Helper::seekWidgetByName(_panel_lottery, BUTTON_OPEN));
    this->_text_button_open   = static_cast<Text*>(Helper::seekWidgetByName(_panel_lottery, TEXT_BUTTON_OPEN));
    this->_button_open_reward->addClickEventListener(CC_CALLBACK_1(VideoRewardsPopup::onButtonClick, this));

    this->_panel_lottery->setVisible(false);
    this->_text_notice_3->setVisible(false);
    this->_text_notice_4->setVisible(false);
    this->_text_notice_5->setVisible(false);
    this->_text_notice_6->setVisible(false);
    this->_panel_reset_info->setVisible(false);

    this->_text_notice_1->setString(GET_TEXT_BY_ID(TXT_NOTICE_1));
    this->_text_notice_2->setString(GET_TEXT_BY_ID(TXT_NOTICE_2));
    this->_text_notice_3->setString(GET_TEXT_BY_ID(TXT_NOTICE_3));
    this->_text_notice_4->setString(GET_TEXT_BY_ID(TXT_NOTICE_4));
    this->_text_notice_5->setString(GET_TEXT_BY_ID(TXT_NOTICE_5));
    this->_text_notice_6->setString(GET_TEXT_BY_ID(TXT_NOTICE_6));
    this->_text_reset_notice->setString(GET_TEXT_BY_ID(TXT_NOTICE_RESET));
    this->_text_reset_coundown->setString("");
    this->_text_button_open->setString(GET_TEXT_BY_ID(TXT_OPEN_REWARD));

    _layoutEventDecor = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_EVENT_BACKGROUND));
    checkAndEnableEventDecorLayer();
}

cocos2d::FiniteTimeAction* VideoRewardsPopup::createShowAction(float delay)
{
    FadeTo* action = FadeTo::create(TIME_ANIM_OPEN, 255);
    return Sequence::create(DelayTime::create(delay), action, nullptr);
}

void VideoRewardsPopup::open()
{
    this->setOpacity(0);
    FiniteTimeAction* popupAction = createShowAction(TIME_DELAY_OPEN);
    popupAction->setTag(TAG_ACTION_OPEN);
    this->runAction(popupAction);

    _state = State::OPENED;
}

void VideoRewardsPopup::close(bool forceClose /*= false*/)
{
    if (forceClose == false)
    {
        if (_state == State::OPENED)
        {
            _state = State::CLOSED;
            _layoutRoot->stopAllActions();

            CallFunc* endAnimFunc = CallFunc::create([=] {
                _state = State::CLOSED;
                this->removeFromParent();
            });
            _layoutRoot->runAction(Sequence::create(Spawn::create(Sequence::create(DelayTime::create(0.1f), FadeOut::create(0.1f), nullptr), nullptr), endAnimFunc, nullptr));
        }
    }
    else
    {
        _state = State::CLOSED;
        _layoutRoot->stopAllActions();
        this->removeFromParent();
    }
}


void VideoRewardsPopup::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
    {
        if (_isPlayingAnimation == false)
        {
            this->close();
            MessageSender::requestVideoRewardEnable();
            this->unschedule(schedule_selector(VideoRewardsPopup::tick));
        }
    }
    else if (sender == _button_open_reward)
    {
        this->playLotteryOpenAnimation();
    }
}

void VideoRewardsPopup::setData(const rapidjson::Document& data, bool playAnimation)
{
    this->unschedule(schedule_selector(VideoRewardsPopup::tick));
    this->_list_view_item->removeAllChildren();
    bool isEnable = data[KEY_VIDEO_REWARD_ENABLE].GetBool();
    this->_panel_item->setVisible(isEnable);
    this->_panel_item_lottery->setVisible(isEnable);
    this->_text_notice_1->setVisible(isEnable);
    this->_text_notice_2->setVisible(isEnable);
    this->_text_notice_5->setVisible(isEnable);
    this->_text_notice_6->setVisible(isEnable);
    this->_panel_reset_info->setVisible(isEnable);
    this->_text_notice_3->setVisible(!isEnable);
    this->_text_notice_4->setVisible(!isEnable);
    if (isEnable)
    {
        int                 currentStep = data[KEY_VIDEO_REWARD_CURRENT_STEP].GetInt();
        double              countDown   = data[KEY_VIDEO_REWARD_COUNT_DOWN].GetDouble();
        bool                skippable   = data[KEY_VIDEO_REWARD_SKIPPABLE].GetBool();
        double              resetAt     = data[KEY_VIDEO_REWARD_REWARD_RESET_AT].GetDouble();
        int                 lv          = data[KEY_VIDEO_REWARD_REWARD_LV].GetInt();
        std::vector<double> rewards;
        for (int            i           = 0; i < data[KEY_VIDEO_REWARD_REWARD_DATA].Size(); i++)
            rewards.push_back(data[KEY_VIDEO_REWARD_REWARD_DATA][i].GetDouble());
        Size                itemSize    = static_cast<Layout*>(Helper::seekWidgetByName(_panel_item, PANEL_ITEM_IMAGE_BACKGROUND))->getContentSize();
        Size                scrollSize  = this->_list_view_item->getInnerContainerSize();
        for (int            i           = 0; i < rewards.size(); i++)
        {
            bool isLottery = (i == rewards.size() - 1);
            RewardItem* item = RewardItem::createRewardItem(this, static_cast<Layout*>((isLottery) ? (this->_panel_item_lottery->clone()) : (this->_panel_item->clone())));
            item->setData(rewards[i], i, currentStep, countDown, (currentStep == i) ? skippable : false, isLottery);
            item->setTag(i);
            this->_list_view_item->pushBackCustomItem(item);
        }
        this->_list_view_item->requestDoLayout();
        if (itemSize.width * 5 < scrollSize.width)
        {
            Size size = Size(itemSize.width * 5 + 10 * 4, scrollSize.height);
            this->_list_view_item->setContentSize(size);
        }
        this->_currentStep = currentStep;
        this->_resetAt     = resetAt;
        this->_rewardLevel = lv;
        tick(0);
        this->schedule(schedule_selector(VideoRewardsPopup::tick), 1.0f, CC_REPEAT_FOREVER, 0);
        if (playAnimation == true)
            playInfoListAppearAnimation(TIME_DELAY_OPEN + TIME_ANIM_OPEN, TIME_ANIM_DELAY_UNIT_INFO_ITEM);
    }
}

void VideoRewardsPopup::onServerResponse(MessageID messageId, const rapidjson::Document& data)
{
    if (this->_list_view_item == nullptr)
        return;
    RewardItem* item = static_cast<RewardItem*>(this->_list_view_item->getChildByTag(this->_currentStep));
    switch (messageId)
    {
        case MessageID::MESSAGE_VIDEO_REWARD_SKIP_RESPONSE:
        {
            if (item != nullptr)
            {
                item->setButtonVideoEnable(true);
                double countDown = data[KEY_VIDEO_REWARD_COUNT_DOWN].GetDouble() - GameUtils::getCurrentTime();
                if (countDown == 0)
                    MessageSender::requestVideoRewardClaim(true);
            }
            break;
        }
        case MessageID::MESSAGE_VIDEO_REWARD_CLAIM_RESPONSE:
        {
            if (item != nullptr)
            {
                item->setButtonClaimEnable(true);
                bool result = data[KEY_RESULT].GetBool();
                if (result == true)
                {
                    item->unschedule(schedule_selector(RewardItem::tick));
                    item->setButtonVideoVisible(false);
                    item->setButtonClaimVisible(false);
                    double bonus = JsonHelper::getDouble(data, KEY_BONUS_MONEY, 0);
                    this->_data = JsonHelper::copyDocument(data);
                    bool isLottery          = JsonHelper::getBool(data, KEY_VIDEO_REWARD_IS_LOTTERY, false);
                    bool isSkipWaitingPhase = JsonHelper::getBool(data, KEY_VIDEO_REWARD_IS_SKIP_WAITING_PHASE, false);
                    MessageSender::requestUserData();
                    _isPlayingAnimation = true;
                    if (isLottery == true)
                    {
                        playLotteryShowAnimation(bonus, [=] {
                            MyActionsManager::getInstance()->createActionMoneyChange(this->_text_user_money,
                                                                                     1.0f,
                                                                                     100,
                                                                                     UserData::getInstance()->getMoney() - bonus,
                                                                                     UserData::getInstance()->getMoney(),
                                                                                     "",
                                                                                     "");
                            this->setData(this->_data);
                            this->_isPlayingAnimation = false;
                            SoundManager::playSoundEffect(SoundEvent::CLAIM_MONEY);
                        });
                    }
                    else
                    {
                        RewardPopup* notifyPopup = BoardManager::createRewardPopup();
                        notifyPopup->attach(this, kAboveForeground);
                        notifyPopup->setContent(GET_TEXT_BY_ID(TXT_VIDEO_REWARD_MONEY));
                        notifyPopup->setRewardContent(GameUtils::MoneyFormat(bonus, "+"));
                        notifyPopup->setClaimRewardCallback([=]() {
                            MyActionsManager::getInstance()->createActionMoneyChange(this->_text_user_money,
                                                                                     1.0f,
                                                                                     100,
                                                                                     UserData::getInstance()->getMoney() - bonus,
                                                                                     UserData::getInstance()->getMoney(),
                                                                                     "",
                                                                                     "");
                            this->setData(this->_data);
                            this->_isPlayingAnimation = false;
                            if (!isSkipWaitingPhase)
                            {
                                Sequence* seq = Sequence::create(DelayTime::create(1.0), CallFunc::create([=] {
                                    if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_GET_REWARDS_ENABLE, true))
                                        AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), false, INTERSTITIAL_ADMOB_GET_MONEY_TIME_BONUS);
                                }), DelayTime::create(0.5f), CallFunc::create([=]() {
                                    this->checkToRemindUserTurnOnNetwork();
                                }), nullptr);
                                this->getParent()->runAction(seq);
                            }
                            else
                            {
                                this->checkToRemindUserTurnOnNetwork();
                            }
                        });
                    }
                    Firebase::logClaimTimeBonus(_currentStep, bonus, isSkipWaitingPhase, _rewardLevel);
                    if (UserData::getInstance()->isEnableLocalNotification())
                        MessageSender::requestLocalNotificationData();
                }
            }
            break;
        }
    }
}

void VideoRewardsPopup::playLotteryShowAnimation(double value, const ActionLotteryAnimCallback& callback)
{
    this->_loterryAnimCallback = callback;
    this->_button_open_reward->setVisible(false);
    this->_image_box->loadTexture("ui/ImagesCommon/gift_box.png", Widget::TextureResType::PLIST);
    this->_image_box->setOpacity(0);
    this->_panel_reward->setOpacity(0);
    this->_panel_reward->setVisible(false);
    this->_text_value->setString(GameUtils::MoneyFormat(value, "+"));
    this->_text_value->setOpacity(0);

    Size textSize       = this->_text_value->getContentSize();
    Size backgroundSize = this->_image_background->getContentSize();
    backgroundSize.width = textSize.width * 2.0f;
    this->_image_background->setContentSize(backgroundSize);

    this->_panel_info->setVisible(true);
    this->_panel_lottery->setOpacity(255);
    this->_panel_lottery->setVisible(false);

    Sequence* inSequence = Sequence::create(FadeOut::create(0.25f), CallFunc::create([=] {
        this->_panel_info->setVisible(false);
        this->_panel_lottery->setVisible(true);
    }), nullptr);
    this->_panel_info->runAction(inSequence);

    Vec2 originalPos = this->_image_box->getPosition();
    Vec2 startPos    = Vec2(originalPos);
    startPos.y *= 5.5f;
    float scaleX = this->_image_box->getScaleX();
    float scaleY = this->_image_box->getScaleY();
    this->_image_box->setPosition(startPos);
    Spawn   * boxSpawn    = Spawn::create(FadeTo::create(0.25f, 255), EaseBounceOut::create(MoveTo::create(0.75f, originalPos)), nullptr);
    Sequence* boxSequence = Sequence::create(DelayTime::create(0.5f),
                                             ScaleTo::create(0.1f, scaleX * 0.75f, scaleY * 1.25f),
                                             ScaleTo::create(0.1f, scaleX * 1.25f, scaleY * 0.75f),
                                             ScaleTo::create(0.05f, scaleX, scaleY),
                                             CallFunc::create([=] {
                                                 this->_button_open_reward->setOpacity(0);
                                                 this->_button_open_reward->setEnabled(false);
                                                 this->_button_open_reward->setVisible(true);
                                                 Sequence* action = Sequence::create(FadeIn::create(0.1f), CallFunc::create([=] {
                                                     this->_button_open_reward->setEnabled(true);
                                                 }), nullptr);
                                                 this->_button_open_reward->runAction(action);
                                                 //
                                                 ScaleTo      * scale1       = ScaleTo::create(0.5f, this->_image_box->getScaleX() * 1.1f, this->_image_box->getScaleY() * 0.9f);
                                                 ScaleTo      * scale2       = ScaleTo::create(0.5f, this->_image_box->getScaleX() * 0.9f, this->_image_box->getScaleY() * 1.1f);
                                                 Sequence     * seq          = Sequence::create(scale1, scale2, nullptr);
                                                 RepeatForever* repeatAction = RepeatForever::create(seq);
                                                 repeatAction->setTag(TAG_ACTION_IMAGE_BOX_WAIT_OPEN);
                                                 this->_image_box->runAction(repeatAction);
                                             }),
                                             nullptr);
    this->_image_box->runAction(boxSpawn);
    this->_image_box->runAction(boxSequence);
}

void VideoRewardsPopup::playLotteryOpenAnimation()
{
    this->_image_box->stopActionByTag(TAG_ACTION_IMAGE_BOX_WAIT_OPEN);
    this->_button_open_reward->setVisible(false);

    float scaleX = this->_image_box->getScaleX();
    float scaleY = this->_image_box->getScaleX();
    Sequence* boxSequence = Sequence::create(Repeat::create(Sequence::create(ScaleTo::create(0.25f, scaleX * 1.25f, scaleY * 0.75f), ScaleTo::create(0.15f, scaleX * 0.75f, scaleY * 1.25f), nullptr),
                                                            3), ScaleTo::create(0.5f, scaleX * 2.25f, scaleY * 0.5f), ScaleTo::create(0.1f, scaleX * 0.5f, scaleY * 2.25f), CallFunc::create([=] {
        this->_image_box->loadTexture("ui/ImagesCommon/gift_box_open.png", Widget::TextureResType::PLIST);
        {
            float scale            = this->_panel_reward->getScale();
            Point originalPanelPos = this->_panel_reward->getPosition();
            Point startPanelPos    = Vec2(this->_image_box->getPosition());
            this->_panel_reward->setPosition(startPanelPos);
            this->_panel_reward->setScale(0);
            this->_panel_reward->setVisible(true);
            this->_panel_reward
                ->runAction(Spawn::create(FadeTo::create(0.5f, 255), EaseSineIn::create(MoveTo::create(0.5f, originalPanelPos)), EaseSineIn::create(ScaleTo::create(0.5f, scale)), nullptr));
        }
        {
            float scale = this->_text_value->getScale();
            this->_text_value->setScale(scale * 2.0f);
            this->_text_value->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(EaseBackOut::create(ScaleTo::create(0.2f, scale)), FadeTo::create(0.1f, 255), nullptr), nullptr));
            SoundManager::playSoundEffect(SoundEvent::VIDEO_REWARD_LOTTERY);
        }
    }), ScaleTo::create(0.1f, scaleX * 1.25f, scaleY * 0.75f), ScaleTo::create(0.1f, scaleX * 0.75f, scaleY * 1.25f), ScaleTo::create(0.1f, scaleX, scaleY), nullptr);
    this->_image_box->runAction(boxSequence);
    Sequence* outSequence = Sequence::create(DelayTime::create(5.0f), FadeOut::create(0.5f), CallFunc::create([=] {
        this->_panel_info->setOpacity(0);
        this->_panel_info->setVisible(true);
        this->_panel_lottery->setVisible(false);
        this->_panel_info->runAction(FadeIn::create(0.5f));
        if (this->_loterryAnimCallback != nullptr)
            _loterryAnimCallback();
    }), nullptr);
    this->_panel_lottery->runAction(outSequence);
}

void VideoRewardsPopup::playInfoListAppearAnimation(float delayStart, float delayUnit)
{
    cocos2d::Vector<cocos2d::Node*> children = this->_list_view_item->getChildren();
    for (int                        i        = 0; i < children.size(); i++)
    {
        Node* node = children.at(i);
        node->setOpacity(0);
        this->_list_view_item->requestDoLayout();
        Sequence* action = Sequence::create(DelayTime::create(delayStart + delayUnit * i),
                                            FadeIn::create(TIME_ANIM_INFO_ITEM),
                                            CallFunc::create([=] { this->_list_view_item->requestDoLayout(); }),
                                            nullptr);
        node->runAction(action);
    }
}

void VideoRewardsPopup::tick(float dt)
{
    double countdown = this->_resetAt - GameUtils::getCurrentTime();
    countdown = (countdown >= 0) ? countdown : 0;
    std::string text = GameUtils::getTimeTextFromNumber(countdown);
    this->_text_reset_coundown->setString(text);
    this->_text_reset_coundown->setColor((countdown >= URGENT_COUNTDOWN_VALUE) ? COLOR_RESET_COUNTDOWN_NORMAL : COLOR_RESET_COUNTDOWN_URGENT);
    if (countdown <= 0)
        this->unschedule(schedule_selector(VideoRewardsPopup::tick));
}

void VideoRewardsPopup::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            if (_applyBackKey && _state == InterfacePopupBoard::State::OPENED)
            {
                if (_isPlayingAnimation == false)
                {
                    close();
                    MessageSender::requestVideoRewardEnable();
                    this->unschedule(schedule_selector(VideoRewardsPopup::tick));
                }
            }
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}

void VideoRewardsPopup::checkAndEnableEventDecorLayer()
{
    bool isEnabled = UserData::getInstance()->isEventDecorEnabled();
    _layoutEventDecor->setVisible(isEnabled);
    _layoutRoot->stopActionByTag(TAG_ACTION_DECOR_LAYOUT_TIMELINE);
    if (isEnabled)
    {
        cocostudio::timeline::ActionTimeline* pActionTimeline = cocos2d::CSLoader::createTimeline(PATH);
        if (pActionTimeline != nullptr)
        {
            pActionTimeline->setTag(TAG_ACTION_DECOR_LAYOUT_TIMELINE);
            pActionTimeline->setTarget(_layoutRoot);
            _layoutRoot->runAction(pActionTimeline);
            pActionTimeline->gotoFrameAndPlay(0, true);
        }
    }
}

void VideoRewardsPopup::checkToRemindUserTurnOnNetwork()
{
    if (!CallToOtherLanguage::getInstance()->checkInternet())
    {
        CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_3));
        AdsManager::getInstance()->scheduleFetchVideo();
    }
}

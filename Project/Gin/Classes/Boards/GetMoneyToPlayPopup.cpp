#include "GetMoneyToPlayPopup.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Manager/MyActionsManager.h"
#include "Network/MessageSender.h"
#include "Helper/GameUtils.h"
#include "Native/CallToOtherLanguage.h"
#include "FirebaseGG/Firebase.h"
#include "Manager/AdsManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

GetMoneyToPlayPopup::GetMoneyToPlayPopup()
{
    this->_applyTouchOutClose = false;
    this->_applyBackKey       = false;
}

GetMoneyToPlayPopup::~GetMoneyToPlayPopup()
{
}

bool GetMoneyToPlayPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void GetMoneyToPlayPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/GetMoneyPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));

    _buttonViewAds = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonViewAds"));
    _buttonGetGold = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonGetGold"));
    _buttonClose   = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));

    this->_buttonGetGold->addClickEventListener(CC_CALLBACK_1(GetMoneyToPlayPopup::onGetGoldButtonClick, this));
    this->_buttonViewAds->addClickEventListener(CC_CALLBACK_1(GetMoneyToPlayPopup::onViewAdsButtonClick, this));
    this->_buttonClose->addClickEventListener(CC_CALLBACK_1(GetMoneyToPlayPopup::onButtonClick, this));

    _buttonVideoBonusReward = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonVideoBonusReward"));
    _buttonVideoBonusReward->addClickEventListener(CC_CALLBACK_1(GetMoneyToPlayPopup::onVideoRewardButtonClick, this));
    _buttonVideoBonusReward->setVisible(false);
    _textBMVideoBonusReward = static_cast<Text*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "TextBMMesage"));
    _textBMVideoBonusReward->setString(GET_TEXT_BY_ID(TXT_GET_REWARD));
    _imageVideoBonusReward       = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageBox"));
    _imageVideoBonusRewardGlow0  = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageGlow0"));
    _imageVideoBonusRewardGlow1  = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageGlow1"));
    _imageVideoBonusRewardScaleX = _imageVideoBonusReward->getScaleX();
    _imageVideoBonusRewardScaleY = _imageVideoBonusReward->getScaleY();

    _labelGold        = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_buttonGetGold, "LabelGold"));
    _labelViewAds     = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_buttonViewAds, "LabelViewAds"));
    _labelViewAdsHint = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonViewAds, "TextVideoHint"));

    this->changeLanguage(TextManager::getInstance()->getLanguageType());
}

void GetMoneyToPlayPopup::onViewAdsButtonClick(Ref* sender)
{
    bool adsIsAvailable = AdsManager::getInstance()->isRewardedAdsAvailable();
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
            CCLOG("No Network");
            CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_1));
        }
        else
        {
            Firebase::logOpenWatchVideo("not available");
            CCLOG("No Available");
            CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_2));
            AdsManager::getInstance()->fetchRewardedAds();
        }
    }
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void GetMoneyToPlayPopup::onGetGoldButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    MessageSender::requestGoldBonus();
    close();
}

void GetMoneyToPlayPopup::onButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
    {
        MessageSender::requestGoldBonus();
        close();
    }
}

void GetMoneyToPlayPopup::onVideoRewardButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonVideoBonusReward)
        MessageSender::requestVideoRewardInfo();
}

void GetMoneyToPlayPopup::setContent(const std::string& content)
{
    this->_textContent->setString(content);
}

void GetMoneyToPlayPopup::setEnableViewAds(bool visible)
{
    this->_buttonViewAds->setEnabled(visible);
    if (!visible)
    {
        this->_buttonViewAds->setColor(Color3B(100, 100, 100));
        this->_labelViewAds->setColor(Color3B(100, 100, 100));
    }
    else
    {
        this->_buttonViewAds->setOpacity(255.0f);
        this->_labelViewAds->setOpacity(255.0f);
        this->_buttonViewAds->setColor(Color3B::WHITE);
    }
}

void GetMoneyToPlayPopup::setEnableFreeGold(bool visible)
{
    this->_buttonGetGold->setEnabled(visible);
    this->_labelGold->setString(defaultFreeGoldText);
    auto textGold = static_cast<TextBMFont*>(_buttonClose->getChildByName("LabelGold"));
    textGold->setString(defaultFreeGoldText);
}

void GetMoneyToPlayPopup::setTextOfGetGold(const std::string& text)
{
    this->_labelGold->setString(text);
    this->defaultFreeGoldText = text;
    auto textGold = static_cast<TextBMFont*>(_buttonClose->getChildByName("LabelGold"));
    textGold->setString(defaultFreeGoldText);
}

void GetMoneyToPlayPopup::setTextOfViewAds(const std::string& text)
{
    this->_labelViewAds->setString(text);
}

void GetMoneyToPlayPopup::changeLanguage(Language)
{
    this->_textContent->setString(GET_TEXT_BY_ID(TXT_GET_MONEY_POPUP_TITLE));
    this->_labelViewAdsHint->setString(GET_TEXT_BY_ID(TXT_VIDEO_BONUS_TITLE));
    this->_textBMVideoBonusReward->setString(GET_TEXT_BY_ID(TXT_GET_REWARD));
}

void GetMoneyToPlayPopup::onVideoRewardEnable(bool enable, double availableAt)
{
    if (enable)
    {
        _buttonViewAds->setVisible(false);
        _buttonVideoBonusReward->setVisible(true);
        _imageVideoBonusReward->stopAllActions();
        _imageVideoBonusRewardGlow0->stopAllActions();
        _imageVideoBonusRewardGlow1->stopAllActions();
        _imageVideoBonusReward->setScale(_imageVideoBonusRewardScaleX, _imageVideoBonusRewardScaleY);
        _imageVideoBonusRewardGlow0->setVisible(false);
        _imageVideoBonusRewardGlow1->setVisible(false);
        if (availableAt >= 0)
        {
            double countDown = std::max((double) 0, availableAt - GameUtils::getCurrentTime());
            this->runAction(Sequence::create(DelayTime::create(countDown), CallFunc::create([=] {
                ScaleTo      * scale1 = ScaleTo::create(0.4f, _imageVideoBonusRewardScaleX * 1.1f, _imageVideoBonusRewardScaleY * 0.9f);
                ScaleTo      * scale2 = ScaleTo::create(0.4f, _imageVideoBonusRewardScaleX * 0.9f, _imageVideoBonusRewardScaleY * 1.1f);
                Sequence     * seq    = Sequence::create(scale1, scale2, nullptr);
                RepeatForever* action = RepeatForever::create(seq);
                _imageVideoBonusReward->runAction(action);

                _imageVideoBonusRewardGlow0->setVisible(true);
                _imageVideoBonusRewardGlow1->setVisible(true);
                _imageVideoBonusRewardGlow0->runAction(RepeatForever::create(RotateBy::create(20.0f, 360)));
                _imageVideoBonusRewardGlow1->runAction(RepeatForever::create(RotateBy::create(20.0f, 360)));
            }), nullptr));
        }
    }
    else
    {
        _buttonViewAds->setVisible(true);
        _buttonVideoBonusReward->setVisible(false);
        _imageVideoBonusReward->stopAllActions();
        _imageVideoBonusRewardGlow0->stopAllActions();
        _imageVideoBonusRewardGlow1->stopAllActions();
    }
}

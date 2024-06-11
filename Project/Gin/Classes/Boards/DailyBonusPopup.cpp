#include "DailyBonusPopup.h"
#include "Manager/MyActionsManager.h"
#include "Helper/GameUtils.h"
#include "Manager/TextManager.h"
#include "UserData/UserData.h"

#define PATH                        "ui/Common/DailyBonusPopup.csb"
#define ID_ROOT                     "Root"
#define ID_TEXT_MONEY               "text_money"
#define ID_TEXT_NEXT_MONEY          "text_next_money"
#define ID_BUTTON_PANEL             "panel_button"
#define ID_IMAGE_TITLE              "daily_bonus_title"
#define PANEL_EVENT_BACKGROUND      "PanelEventBackground"

#define TAG_ACTION_DECOR_LAYOUT_TIMELINE 100

USING_NS_CC;
using namespace ui;
using namespace std;

DailyBonusPopup::DailyBonusPopup()
{
    this->_buttonCallback     = nullptr;
    this->_applyTouchOutClose = false;
    this->_applyBackKey       = false;
}

DailyBonusPopup::~DailyBonusPopup()
{
}

bool DailyBonusPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    initScreen();
    return true;
}

void DailyBonusPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node = CSLoader::createNode(PATH);
    Node* root = node->getChildByName(ID_ROOT);
    root->removeFromParent();

    this->_layoutRoot = static_cast<Layout*>(root);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_layoutRoot, kAboveForeground);
    ui::Helper::doLayout(this);

    this->_textContent    = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, ID_TEXT_MONEY));
    this->_textSubContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, ID_TEXT_NEXT_MONEY));
    this->_imgTitle       = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutRoot, ID_IMAGE_TITLE));
    this->_layoutButton   = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, ID_BUTTON_PANEL));

    _layoutEventDecor = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_EVENT_BACKGROUND));
    checkAndEnableEventDecorLayer();
    changeLanguage(TextManager::getInstance()->getLanguageType());

    auto buttonReceive = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonReceive"));
    buttonReceive->addTouchEventListener(CC_CALLBACK_2(DailyBonusPopup::onButtonTouch, this));
}

void DailyBonusPopup::show(cocos2d::Node* parent)
{
    parent->addChild(this, kAboveForeground);
    this->setScale(0);
    this->_layoutButton->setScale(0);
    this->_imgTitle->setScale(0);

    FiniteTimeAction* popupAction  = createShowAction(0.75f);
    FiniteTimeAction* titleAction  = createShowAction(1.0f);
    FiniteTimeAction* buttonAction = createShowAction(1.25f);

    this->runAction(popupAction);
    this->_imgTitle->runAction(titleAction);
    this->_layoutButton->runAction(buttonAction);

    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
        SoundManager::playSoundEffect(SoundEvent::DAILY_BONUS);
    }), nullptr));
}

void DailyBonusPopup::hide()
{
    FiniteTimeAction* animation = createHideAction();
    Sequence        * action    = Sequence::create(animation, CallFunc::create([=]() {
        removeFromParent();
    }), nullptr);
    _layoutRoot->runAction(action);
}

cocos2d::FiniteTimeAction* DailyBonusPopup::createShowAction(float delay)
{
    ScaleTo    * scaleUp = ScaleTo::create(0.25f, 1.0f);
    EaseBackOut* action  = EaseBackOut::create(scaleUp);
    return Sequence::create(DelayTime::create(delay), action, nullptr);
}

cocos2d::FiniteTimeAction* DailyBonusPopup::createHideAction(float delay)
{
    ScaleTo   * scaleDown = ScaleTo::create(0.25f, 0.0f);
    EaseBackIn* action    = EaseBackIn::create(scaleDown);
    return Sequence::create(DelayTime::create(delay), action, nullptr);
}

void DailyBonusPopup::changeLanguage(Language language)
{
}

void DailyBonusPopup::setContent(double money, double bonusMoney, double nextBonusMoney, const GetMoneyButtonCallback& callback)
{
    this->_money      = money;
    this->_bonusMoney = bonusMoney;
    this->_textContent->setString(GameUtils::MoneyFormat(bonusMoney, "+"));
    this->_textSubContent->setString(StringUtils::format("%s %s", GET_TEXT_BY_ID(TXT_NEXT_DAY_BONUS), GameUtils::MoneyFormat(nextBonusMoney, "+").c_str()));
    this->_buttonCallback = callback;
}

void DailyBonusPopup::onButtonTouch(cocos2d::Ref* pSender, Widget::TouchEventType type)
{
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        {
            SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            CallFunc* callback2 = CallFunc::create([=]() {
                if (_buttonCallback != nullptr)
                {
                    _buttonCallback(this->_money, this->_bonusMoney);
                    SoundManager::playSoundEffect(SoundEvent::CLAIM_MONEY);
                }
                hide();
            });
            runAction(Sequence::create(DelayTime::create(0.2f), DelayTime::create(0.2f), callback2, nullptr));
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
        default:
            break;
    }
}

void DailyBonusPopup::checkAndEnableEventDecorLayer()
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

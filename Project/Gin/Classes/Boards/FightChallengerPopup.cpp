#include "FightChallengerPopup.h"
#include "Helper/GameUtils.h"
#include "Manager/TextManager.h"
#include "Manager/BoardManager.h"
#include "Helper/fmt/format.h"
#include "Scenes/Gin/GPGinControlCardLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

FightChallengerPopup::~FightChallengerPopup()
{
}

FightChallengerPopup::FightChallengerPopup()
{
    _fightChallengerCallback           = nullptr;
    _gpGinControlCardLayerCallback = nullptr;
    _isChallenge                       = false;
    _isRequested                       = false;
}

bool FightChallengerPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void FightChallengerPopup::initScreen()
{
    this->setContentSize(_screenSize);
    Node* node  = CSLoader::createNode("ui/Common/FightChallengerPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _layoutFight     = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "LayoutFight"));
    _layoutChallenge = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "LayoutChallenge"));

    this->_textContent = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoot, "TextContent"));
    this->_buttonFold  = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonFold"));
    this->_buttonFold->addClickEventListener(CC_CALLBACK_1(FightChallengerPopup::onButtonClick, this));
    this->_buttonChallenge = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonChallange"));
    this->_buttonChallenge->addClickEventListener(CC_CALLBACK_1(FightChallengerPopup::onButtonClick, this));

    this->_buttonCancel = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonCancel"));
    this->_buttonCancel->addClickEventListener(CC_CALLBACK_1(FightChallengerPopup::onButtonClick, this));
    this->_buttonFight = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonFight"));
    this->_buttonFight->addClickEventListener(CC_CALLBACK_1(FightChallengerPopup::onButtonClick, this));

    changeLanguage(TextManager::getInstance()->getLanguageType());
}

void FightChallengerPopup::setChallenge(int currentScore)
{
    _isChallenge = true;
    _layoutFight->setVisible(false);
    _layoutChallenge->setVisible(true);
    _textContent->setString(fmt::format(GET_TEXT_BY_ID(TXT_FIGHT_CHALLENGE_CONTENT), currentScore));
    static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "Panel_11"))->setVisible(true);

    _applyTouchOutClose = false;
    _applyBackKey       = false;
}

void FightChallengerPopup::setFight(int currentScore)
{
    _isChallenge = false;
    _layoutFight->setVisible(true);
    _layoutChallenge->setVisible(false);
    _textContent->setString(fmt::format(GET_TEXT_BY_ID(TXT_FIGHT_CONFIRM), currentScore));
    static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "Panel_11"))->setVisible(false);

    _applyTouchOutClose = true;
    _applyBackKey       = true;
}

void FightChallengerPopup::changeLanguage(Language language)
{
    ((Text*) _buttonFold->getChildByName("Label"))->setString(GET_TEXT_BY_ID(TXT_FOLD));
    ((Text*) _buttonChallenge->getChildByName("Label"))->setString(GET_TEXT_BY_ID(TXT_CHALLENGE));
    ((Text*) _buttonCancel->getChildByName("Label"))->setString(GET_TEXT_BY_ID(TXT_FIGHT_CANCEL));
    ((Text*) _buttonFight->getChildByName("Label"))->setString(GET_TEXT_BY_ID(TXT_FIGHT));
}

void FightChallengerPopup::onButtonClick(cocos2d::Ref* sender)
{
    if (!_isRequested)
    {
        _isRequested = true;
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        if (sender == _buttonFold)
        {
            if (_fightChallengerCallback != nullptr)
                _fightChallengerCallback(TAG_BUTTON_FOLD);
            this->close();
        }
        else if (sender == _buttonChallenge)
        {
            if (_fightChallengerCallback != nullptr && _gpGinControlCardLayerCallback != nullptr)
            {
                if (false)
                {
                    _fightChallengerCallback(TAG_BUTTON_CHALLENGE);
                    this->close();
                }
                else
                {
                    _isRequested = false;
                    NotifyPopup* notifyPopup = BoardManager::createNotifyPopup();
                    notifyPopup->initValue(GET_TEXT_BY_ID(TXT_NOTIFY_USER_CHALLENGE_NO_SECRET_MELD), nullptr);
                    notifyPopup->attach(this, kAboveForeground);
                }
            }
            else
            {
                this->close();
            }
        }
        else if (sender == _buttonCancel)
        {
            this->close();
        }
        else if (sender == _buttonFight)
        {
            if (_fightChallengerCallback != nullptr)
                _fightChallengerCallback(TAG_BUTTON_FIGHT);
            this->close();
        }
    }
}

void FightChallengerPopup::close(bool forceClose)
{
    hideParticle();
    InterfacePopupBoard::close(forceClose);
}

void FightChallengerPopup::hideParticle()
{
    Node* node = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "PanelContent"))->getChildByName("Particle");
    if (node)
        node->setVisible(false);
}

bool FightChallengerPopup::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    if (_layoutFight->isVisible())
        return true;
    return false;
}
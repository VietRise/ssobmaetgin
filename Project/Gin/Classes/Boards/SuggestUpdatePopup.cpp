#include "SuggestUpdatePopup.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"
#include "Native/CallToOtherLanguage.h"

USING_NS_CC;
using namespace ui;
using namespace std;

SuggestUpdatePopup::SuggestUpdatePopup()
{
}

SuggestUpdatePopup::~SuggestUpdatePopup()
{
}

bool SuggestUpdatePopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void SuggestUpdatePopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/SuggestUpdatePopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _buttonUpdate = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonUpdate"));
    _buttonUpdate->addClickEventListener(CC_CALLBACK_1(SuggestUpdatePopup::onButtonClick, this));

    _buttonLater = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonLater"));
    _buttonLater->addClickEventListener(CC_CALLBACK_1(SuggestUpdatePopup::onButtonClick, this));

    _textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));

    this->changeLanguage(TextManager::getInstance()->getLanguageType());
}

void SuggestUpdatePopup::initValue(const std::string& content)
{
    if (content != "")
        _textContent->setString(content);
}

void SuggestUpdatePopup::changeLanguage(Language)
{
    _textContent->setString(GET_TEXT_BY_ID(TXT_BUTTON_SUGGEST_CONTENT));

    auto labelLater  = (TextBMFont*) _buttonLater->getChildByName("Label");
    auto labelUpdate = (TextBMFont*) _buttonUpdate->getChildByName("Label");

    labelLater->setString(GET_TEXT_BY_ID(TXT_BUTTON_SUGGEST_LATER));
    labelUpdate->setString(GET_TEXT_BY_ID(TXT_BUTTON_SUGGEST_UPDATE));
}

void SuggestUpdatePopup::onButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);

    if (button == _buttonLater)
        this->close();
    else if (button == _buttonUpdate)
    {
        CallToOtherLanguage::getInstance()->openAppInStore();
        this->close();
    }
}


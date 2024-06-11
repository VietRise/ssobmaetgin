#include "MailboxContentPopup.h"
#include  "Define/GameDefine.h"
#include "Manager/SoundManager.h"
#include "Component/MailboxItem.h"

USING_NS_CC;
using namespace ui;
using namespace std;

MailboxContentPopup::~MailboxContentPopup()
{
}

MailboxContentPopup::MailboxContentPopup() : InterfacePopupBoard()
{
}

bool MailboxContentPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void MailboxContentPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/MailboxContentPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _textTitle   = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextTitle"));
    _textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(MailboxContentPopup::onButtonClick, this));
}

void MailboxContentPopup::initValue(const std::string& title, const std::string& content)
{
    _textTitle->setString(title);
    _textContent->setString(content);
}

void MailboxContentPopup::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
        close();
}

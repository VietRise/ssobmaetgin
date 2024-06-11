#include "ChangeAvatarPopup.h"
#include "Manager/TextManager.h"
#include "Manager/MyActionsManager.h"
#include "UserData/UserData.h"
#include "Network/MessageSender.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/BoardManager.h"
#include "FirebaseGG/Firebase.h"

USING_NS_CC;
using namespace ui;
using namespace std;

ChangeAvatarPopup::~ChangeAvatarPopup()
{
}

ChangeAvatarPopup::ChangeAvatarPopup()
{
    _listAvatar.push_back("BotAvatar/54.png");
    _listAvatar.push_back("BotAvatar/2.png");
    _listAvatar.push_back("BotAvatar/1.png");
    _listAvatar.push_back("BotAvatar/6.png");
    _listAvatar.push_back("BotAvatar/7.png");
    _listAvatar.push_back("BotAvatar/8.png");
    _listAvatar.push_back("BotAvatar/9.png");
    _listAvatar.push_back("BotAvatar/10.png");
    _listAvatar.push_back("BotAvatar/12.png");
    _listAvatar.push_back("BotAvatar/13.png");
    _listAvatar.push_back("BotAvatar/14.png");
    _listAvatar.push_back("BotAvatar/15.png");
    _listAvatar.push_back("BotAvatar/16.png");
    _listAvatar.push_back("BotAvatar/17.png");
    _listAvatar.push_back("BotAvatar/18.png");
    _listAvatar.push_back("BotAvatar/19.png");
    _listAvatar.push_back("BotAvatar/20.png");
    _listAvatar.push_back("BotAvatar/21.png");
    _listAvatar.push_back("BotAvatar/22.png");
    _listAvatar.push_back("BotAvatar/23.png");
    _listAvatar.push_back("BotAvatar/24.png");
    _listAvatar.push_back("BotAvatar/25.png");
    _listAvatar.push_back("BotAvatar/26.png");
    _listAvatar.push_back("BotAvatar/27.png");
    _listAvatar.push_back("BotAvatar/28.png");
    _listAvatar.push_back("BotAvatar/3.png");
    _listAvatar.push_back("BotAvatar/33.png");
    _listAvatar.push_back("BotAvatar/34.png");
    _listAvatar.push_back("BotAvatar/35.png");
    _listAvatar.push_back("BotAvatar/36.png");
    _listAvatar.push_back("BotAvatar/37.png");
    _listAvatar.push_back("BotAvatar/38.png");
    _listAvatar.push_back("BotAvatar/39.png");
    _listAvatar.push_back("BotAvatar/4.png");
    _listAvatar.push_back("BotAvatar/40.png");
    _listAvatar.push_back("BotAvatar/42.png");
    _listAvatar.push_back("BotAvatar/43.png");
    _listAvatar.push_back("BotAvatar/45.png");
    _listAvatar.push_back("BotAvatar/46.png");
    _listAvatar.push_back("BotAvatar/47.png");
    _listAvatar.push_back("BotAvatar/48.png");
    _listAvatar.push_back("BotAvatar/49.png");
    _listAvatar.push_back("BotAvatar/5.png");
    _listAvatar.push_back("BotAvatar/50.png");
    _listAvatar.push_back("BotAvatar/52.png");
    _listAvatar.push_back("BotAvatar/53.png");
    _listAvatar.push_back("BotAvatar/55.png");
    _listAvatar.push_back("BotAvatar/56.png");
    _listAvatar.push_back("BotAvatar/58.png");
    _imgPath = "";
}

bool ChangeAvatarPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void ChangeAvatarPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node  = CSLoader::createNode("ui/Common/ChangeAvatarPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);
    _layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelContent"));
    _layoutAvatar  = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "PanelAvatar"));
    _buttonSave    = static_cast<Button*>(Helper::seekWidgetByName(_layoutContent, "ButtonSave"));
    _buttonSave->setPressedActionEnabled(false);
    _buttonSave->addClickEventListener(CC_CALLBACK_1(ChangeAvatarPopup::onButtonClick, this));
    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(ChangeAvatarPopup::onButtonClick, this));
    _buttonDevice = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonDevice"));
    _buttonDevice->addClickEventListener(CC_CALLBACK_1(ChangeAvatarPopup::onButtonClick, this));
    _textfieldName = static_cast<TextField*>(Helper::seekWidgetByName(_layoutContent, "TextFieldName"));
    _textfieldName->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _textfieldName->setTextVerticalAlignment(TextVAlignment::CENTER);
    _layoutRowTemplate = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "LayoutRowTemplate"));
    _layoutRowTemplate->setVisible(false);
    _layoutAvatarTemplate = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "LayoutAvatarTemplate"));
    _layoutAvatarTemplate->setVisible(false);
    _listViewContent = static_cast<ListView*>(Helper::seekWidgetByName(_layoutRoot, "ListViewContent"));
    changeLanguage(TextManager::getInstance()->getLanguageType());
}

void ChangeAvatarPopup::initValue()
{
    _changeName = UserData::getInstance()->getUserName();
    _imgPath    = UserData::getInstance()->getAvatarLink();
    _textfieldName->setString(_changeName);
    changeAvatar(UserData::getInstance()->getAvatarLink());
    initListAvatar();

    for (AvatarItem* iter : _listAvatarItem)
    {
        iter->setChecked(false);
        if (iter->_image == UserData::getInstance()->getAvatarLink())
            iter->setChecked(true);
    }
}

void ChangeAvatarPopup::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    if (_applyTouchOutClose && _state == InterfacePopupBoard::State::OPENED)
        if (!_layoutRoot->getBoundingBox().containsPoint(touch->getLocation()))
            onButtonClick(_buttonClose);
}

void ChangeAvatarPopup::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    Button* button = (Button*) sender;
    if (button == _buttonSave)
    {
        saveChange();
        this->close();
    }
    else if (button == _buttonClose)
    {
        if (_textfieldName->getString() != UserData::getInstance()->getUserName() || _imgPath != UserData::getInstance()->getAvatarLink())
        {
            auto confirm = BoardManager::createYesNoConfirmPopup();
            confirm->setContent(GET_TEXT_BY_ID(TXT_CHANGE_AVATAR_CONFIRM_SAVE));
            confirm->setCallback([=](YesNoConfirmResult result) {
                confirm->close();
                if (result == YesNoConfirmResult::CONFIRM_YES)
                    saveChange();
                this->close();
            });
            confirm->attach(this, kAboveForeground);
        }
        else
        {
            this->close();
        }
    }
    else if (button == _buttonDevice)
    {
    }
}

void ChangeAvatarPopup::changeLanguage(Language language)
{
    auto labelTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTitle"));
    labelTitle->setString(GET_TEXT_BY_ID(TXT_CHANGE_AVATAR_POPUP_TITLE));
    auto labelSave = static_cast<Text*>(Helper::seekWidgetByName(_buttonSave, "Label"));
    labelSave->setString(GET_TEXT_BY_ID(TXT_CHANGE_AVATAR_BUTTON_SAVE));
    auto labelDevice = static_cast<Text*>(Helper::seekWidgetByName(_buttonDevice, "Label"));
    labelDevice->setString(GET_TEXT_BY_ID(TXT_CHANGE_AVATAR_BUTTON_DEVICE));
}

void ChangeAvatarPopup::initListAvatar()
{
    int numSlot = 1;
    Layout* row = nullptr;
    for (int i = 0; i < _listAvatar.size(); i++)
    {
        const std::string& name = _listAvatar[i];
        if (!row)
        {
            numSlot = 1;
            row     = (Layout*) _layoutRowTemplate->clone();
            row->setVisible(true);
            _listViewContent->pushBackCustomItem(row);
        }
        if (row)
        {
            std::string nameSlot = StringUtils::format("LayoutSlot%d", numSlot);
            Layout* layoutSlot = static_cast<Layout*>(Helper::seekWidgetByName(row, nameSlot));
            layoutSlot->setBackGroundColorType(Layout::BackGroundColorType::NONE);

            Layout* layout = (Layout*) _layoutAvatarTemplate->clone();
            layout->setVisible(true);
            layout->setPosition(layoutSlot->getContentSize() / 2);
            layout->setBackGroundColorType(Layout::BackGroundColorType::NONE);
            layout->setScale(layoutSlot->getContentSize().width / layout->getContentSize().width);

            AvatarItem* item = new AvatarItem(layout);
            item->initValue(CC_CALLBACK_1(ChangeAvatarPopup::onAvatarAction, this));
            item->setAvatar(name);
            item->setChecked(false);
            _listAvatarItem.pushBack(item);
            layoutSlot->addChild(layout);

            numSlot++;
            if (Helper::seekWidgetByName(row, StringUtils::format("LayoutSlot%d", numSlot)) == nullptr)
                row = nullptr;
        }
    }
}

void ChangeAvatarPopup::onAvatarAction(AvatarItem* item)
{
    if (item)
    {
        changeAvatar(item->_image);
        selectAvatar(item);
        Firebase::logChangeAvatar(item->_image);
    }
}

void ChangeAvatarPopup::changeAvatar(const std::string& image)
{
    _layoutAvatar->removeChildByTag(AVATAR_TAG);
    auto avatar = Avatar::create();
    avatar->setFrameUrl(image, _layoutAvatar->getContentSize());
    avatar->setPosition(Point(_layoutAvatar->getContentSize().width / 2, _layoutAvatar->getContentSize().height / 2));
    avatar->setTag(AVATAR_TAG);
    _layoutAvatar->addChild(avatar, -1);
}

void ChangeAvatarPopup::selectAvatar(AvatarItem* item)
{
    _imgPath = item->_image;
    for (AvatarItem* iter : _listAvatarItem)
    {
        iter->setChecked(false);
        if (iter == item)
            iter->setChecked(true);
    }
}

void ChangeAvatarPopup::saveChange()
{
    if (_textfieldName->getString() != UserData::getInstance()->getUserName() && _textfieldName->getString() != "")
        MessageSender::requestChangeName(_textfieldName->getString());
    if (_imgPath != UserData::getInstance()->getAvatarLink())
        MessageSender::requestChangeProfilePicture(_imgPath);
}

void ChangeAvatarPopup::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            if (_applyBackKey && _state == InterfacePopupBoard::State::OPENED)
                onButtonClick(_buttonClose);
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}

//AVATAR
AvatarItem::~AvatarItem()
{
    CC_SAFE_RELEASE_NULL(_layout);
}

AvatarItem::AvatarItem(cocos2d::ui::Layout* layout) : Ref()
{
    _callback = nullptr;
    this->_layout = layout;
    this->_layout->retain();
    _scale    = _layout->getScale();
    _checkBox = static_cast<CheckBox*>(Helper::seekWidgetByName(_layout, "CheckBox"));
    autorelease();
}

void AvatarItem::initValue(const AvatarActionCallback& callback)
{
    _layout->addTouchEventListener(CC_CALLBACK_2(AvatarItem::onClick, this));
    _callback = callback;
}

void AvatarItem::setChecked(bool checked)
{
    if (_checkBox)
        _checkBox->setSelected(checked);
}

void AvatarItem::setAvatar(const std::string& image)
{
    _image = image;
    auto layoutAvatar = static_cast<Layout*>(Helper::seekWidgetByName(_layout, "LayoutAvatar"));
    layoutAvatar->removeChildByTag(AVATAR_TAG);
    layoutAvatar->setBackGroundColorType(Layout::BackGroundColorType::NONE);
    Avatar* avatar = Avatar::create();
    avatar->setFrameUrl(_image, layoutAvatar->getContentSize());
    avatar->setPosition(Point(layoutAvatar->getContentSize().width / 2, layoutAvatar->getContentSize().height / 2));
    avatar->setTag(AVATAR_TAG);
    avatar->setMask(false);
    layoutAvatar->addChild(avatar, -1);
}

void AvatarItem::onClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
    Layout* layout = (Layout*) sender;
    if (layout == _layout)
    {
        layout->stopAllActions();
        if (cocos2d::ui::Widget::TouchEventType::BEGAN == eventType)
        {

        }
        else if (cocos2d::ui::Widget::TouchEventType::ENDED == eventType)
        {
            layout->runAction(Sequence::create(ScaleTo::create(0.1f, _scale - 0.1f), EaseExponentialOut::create(ScaleTo::create(0.1f, _scale)), CallFunc::create([=]() {
                SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
                if (_callback)
                    _callback(this);
            }), nullptr));
        }
        else if (cocos2d::ui::Widget::TouchEventType::CANCELED == eventType)
        {
            auto scaleTo = ScaleTo::create(0.1f, _scale);
            layout->runAction(EaseExponentialOut::create(scaleTo));
        }
    }
}

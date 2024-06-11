#include "MailboxPopup.h"
#include "UserData/UserData.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"
#include "Define/GameDefine.h"
#include "Manager/SoundManager.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/NotificationManager.h"
#include "Manager/BoardManager.h"
#include "Component/MailboxItem.h"
#include "Scenes/LobbyScene.h"
#include "Network/MessageSender.h"
#include "Helper/fmt/format.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Helper/GameUtils.h"
#include "Boards/MailboxContentPopup.h"

USING_NS_CC;
using namespace ui;
using namespace std;

MailboxPopup::~MailboxPopup()
{
    NotificationManager::getInstance()->unregisterNotification(this);
    CC_SAFE_RELEASE_NULL(_layoutMailboxItemTemplate);
}
MailboxPopup::MailboxPopup() : InterfacePopupBoard()
{
}

bool MailboxPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void MailboxPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/MailboxPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _textTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextTitle"));
    _textEmpty = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelEmpty"));

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(MailboxPopup::onButtonClick, this));

    _layoutMailboxItemTemplate = static_cast<Layout*>(node->getChildByName("LayoutMailBoxItem"));
    _layoutMailboxItemTemplate->retain();
    _layoutMailboxItemTemplate->removeFromParent();

    _listViewContent = static_cast<ListView*>(Helper::seekWidgetByName(_layoutRoot, "ListViewContent"));

    changeLanguage(TextManager::getInstance()->getLanguageType());
}

void MailboxPopup::changeLanguage(Language language)
{
    _textTitle->setString(GET_TEXT_BY_ID(TXT_MAILBOX_TITLE));
    _textEmpty->setString(GET_TEXT_BY_ID(TXT_MAILBOX_EMPTY));
}

void MailboxPopup::onExit()
{
    InterfacePopupBoard::onExit();
    setMailboxRead();
    _lobbyScene->checkNewMail();
}

void MailboxPopup::initValue(const rapidjson::Value& data, LobbyScene* lobbyScene)
{
    _lobbyScene = lobbyScene;
    if (data.IsArray() && data.Size() > 0)
    {
        for (int i = 0; i < data.Size(); i++)
        {
            const rapidjson::Value& mail = data[i];
            if (isValidMailboxItem(mail))
            {
                Layout* layout = (Layout*) _layoutMailboxItemTemplate->clone();
                _listViewContent->pushBackCustomItem(layout);

                MailboxItem* mailboxItem = new MailboxItem(layout);
                mailboxItem->initValue(mail, CC_CALLBACK_1(MailboxPopup::onMailboxAction, this));
                _listMailboxItem.pushBack(mailboxItem);
            }
        }
    }
    _textEmpty->setVisible(false);
    if (_listMailboxItem.size() == 0)
        _textEmpty->setVisible(true);
}

bool MailboxPopup::isValidMailboxItem(const rapidjson::Value& data)
{
    return true;
}

void MailboxPopup::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
        close();
}

void MailboxPopup::removeMailboxItems(const std::vector<std::pair<std::string, std::string>>& removeMailboxItems)
{
    for (int i = 0; i < removeMailboxItems.size(); i++)
    {
        const std::pair<std::string, std::string>& removeItem = removeMailboxItems.at(i);
        for (int j = 0; j < _listMailboxItem.size(); j++)
        {
            MailboxItem* item = _listMailboxItem.at(j);
            if (item->getType() == removeItem.first && item->getId() == removeItem.second)
            {
                _listViewContent->removeChild(item->getLayout(), true);
                _listViewContent->requestDoLayout();
                _listMailboxItem.eraseObject(item);
                break;
            }
        }
    }
}

void MailboxPopup::onMailboxAction(MailboxItem* item)
{
    const std::string& type = item->_type;
    const std::string& id   = item->_id;
    const std::string& data = item->_data;
    if (type == MAILBOX_TYPE_UPDATE)
    {
        close(true);
        CallToOtherLanguage::getInstance()->openAppInStore();
    }
    else if (type == MAILBOX_TYPE_READ)
    {
        MailboxContentPopup* popup = BoardManager::createMailboxContentPopup();
        popup->initValue(item->_title, item->_fullContent);
        popup->attach(this, kAboveForeground);
    }
    else if (type == MAILBOX_TYPE_FUNCTION && data != "")
    {
        if (data == MAILBOX_FUNCTION_SHOP_TABLE)
            _lobbyScene->openShopTable();
        else if (data == MAILBOX_FUNCTION_GAMECENTER)
            _lobbyScene->openGameCenter();
        else if (data == MAILBOX_FUNCTION_PLAY)
            MessageSender::requestOpenTicket(UserData::getInstance()->getIdPlaceSelected(), UserData::getInstance()->getIdTableSelected());
        else if (data == MAILBOX_FUNCTION_QUICK_PLAY)
            MessageSender::requestOpenTicket(UserData::getInstance()->getIdPlaceSelected(), UserData::getInstance()->getIdTableSelected());
        else if (data == MAILBOX_FUNCTION_LEADER_BOARD)
            _lobbyScene->openLeaderboard();
        close(true);
    }
    else if (type == MAILBOX_TYPE_SHARE)
    {
        std::string data2 = RemoteConfig::getString(REMOTECONFIG_FUNCTION_SHARE_GAME_LINK, SHARE_GAME_URL_JSON_DATA);
        if (data2 != "")
        {
            const rapidjson::Document& doc = JsonHelper::parseStringToJson(data2);
            std::string urlApp  = JsonHelper::getString(doc, "data", "");
            std::string message = fmt::format(GET_TEXT_BY_ID(TXT_SHARE_APP_MESSAGE), GameUtils::getGameName().c_str(), urlApp.c_str());
            CallToOtherLanguage::getInstance()->shareDataToMessenger(GameUtils::getGameName(), message);
        }
    }
    else
    {
        //do nothing
    }
}

void MailboxPopup::setMailboxRead()
{
    std::vector<std::string> list;
    for (int                 i = 0; i < _listMailboxItem.size(); i++)
    {
        MailboxItem* item = _listMailboxItem.at(i);
        if (item)
        {
            std::string id = item->_type + "_" + item->_id;
            list.push_back(id);
        }
    }
    UserData::getInstance()->setMailRead(list);
}
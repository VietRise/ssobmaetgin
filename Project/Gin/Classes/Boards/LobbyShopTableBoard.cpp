#include "LobbyShopTableBoard.h"
#include "Scenes/LobbyScene.h"
#include "Helper/JsonHelper.h"
#include "Helper/GameUtils.h"
#include "UserData/UserData.h"
#include "Manager/MyActionsManager.h"
#include "Manager/NotificationManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

LobbyShopTableBoard::~LobbyShopTableBoard()
{
    LobbyTableNew::_listTable.clear();
}

LobbyShopTableBoard::LobbyShopTableBoard()
{
    _lobbyScene = nullptr;
}

bool LobbyShopTableBoard::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void LobbyShopTableBoard::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node  = CSLoader::createNode("ui/Common/LobbyShopTableBoard.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _textBMTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTitle"));
    _textBMTitle->setString(GET_TEXT_BY_ID(TXT_LOBBY_SHOP_TABLE_TITLE));

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(LobbyShopTableBoard::onButtonClick, this));

    _listViewContent = static_cast<ListView*>(Helper::seekWidgetByName(_layoutRoot, "ListViewContent"));
    _listViewContent->setDirection(ScrollView::Direction::HORIZONTAL);
    _listViewContent->setTouchEnabled(true);
    _listViewContent->setBounceEnabled(true);
    _listViewContent->setClippingEnabled(true);
    _listViewContent->addEventListener((ui::ListView::ccListViewCallback) CC_CALLBACK_2(LobbyShopTableBoard::selectedItemListViewEvent, this));
}

void LobbyShopTableBoard::initValue(const ValueType& listDataTable)
{
    for (unsigned int i = 0; i < listDataTable.Size(); i++)
    {
        const ValueType& dataTable = listDataTable[i];
        Node           * node      = CSLoader::createNode("ui/Common/LobbyShopTableItem.csb");
        Node           * child     = node->getChildByName("Root");
        child->removeFromParent();
        Layout* layoutTable = static_cast<Layout*>(child);
        _listViewContent->pushBackCustomItem(layoutTable);
        LobbyTableNew* table = new LobbyTableNew(_lobbyScene, this, layoutTable);
        table->initValue(JsonHelper::getInt(dataTable, KEY_TABLE_ID, 0),
                         "",
                         JsonHelper::getInt(dataTable, KEY_TABLE_PRICE, 0),
                         (LobbyTableNew::State) JsonHelper::getInt(dataTable, KEY_TABLE_STATUS, 0),
                         JsonHelper::getDouble(dataTable, KEY_TABLE_BONUS, 0) * 100);

        _listTable.pushBack(table);
    }
    LobbyTableNew::setListTable(_listTable);
    // Select first table
    for (unsigned int i = 0; i < _listTable.size(); i++)
    {
        if (_listTable.at(i)->getId() == UserData::getInstance()->getIdTableSelected())
        {
            _listTable.at(i)->select();
            break;
        }
    }
}

void LobbyShopTableBoard::setLobbyScene(LobbyScene* lobbyScene)
{
    _lobbyScene = lobbyScene;
}

void LobbyShopTableBoard::onExit()
{
    InterfacePopupBoard::onExit();
    NotificationManager::getInstance()->postNotification(EVENT_CHANGE_TABLE);
}

void LobbyShopTableBoard::selectedItemListViewEvent(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type)
{
    ListView* listView = (ListView*) sender;
    switch (type)
    {
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
            break;
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
        {
            SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            int index = (int) listView->getCurSelectedIndex();

            if (_listTable.at(index)->getState() == LobbyTableNew::State::NORMAL)
            {
                _listTable.at(index)->select();
                if (_listTable.at(index)->getState() == LobbyTableNew::State::SELECTED)
                {
                    // Diselect
                    for (int i = 0; i < _listTable.size(); i++)
                    {
                        if (i != index && _listTable.at(i)->getState() == LobbyTableNew::State::SELECTED)
                        {
                            _listTable.at(i)->select();
                            break;
                        }
                    }
                }
            }
            else if (_listTable.at(index)->getState() == LobbyTableNew::State::LOCK_CANNOT_UNLOCK)
                _listTable.at(index)->select();
            else if (_listTable.at(index)->getState() == LobbyTableNew::State::LOCK_CAN_UNLOCK)
                _listTable.at(index)->select();
        }
            break;
        default:
            break;
    }
}

void LobbyShopTableBoard::changeLanguage(Language language)
{
    _textBMTitle->setString(GET_TEXT_BY_ID(TXT_LOBBY_SHOP_TABLE_TITLE));
    if (_lobbyScene)
        for (int i = 0; i < _listTable.size(); i++)
            _listTable.at(i)->changeLanguage(language);
}

void LobbyShopTableBoard::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
        close();
}
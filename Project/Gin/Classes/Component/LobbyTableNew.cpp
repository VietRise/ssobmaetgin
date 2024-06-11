#include "LobbyTableNew.h"
#include "Scenes/LobbyScene.h"
#include "Manager/BoardManager.h"
#include "Manager/MyActionsManager.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"
#include "Helper/GameUtils.h"
#include "Network/MessageSender.h"
#include "Helper/JsonHelper.h"
#include "UserData/UserData.h"
#include "Helper/fmt/format.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace std;

cocos2d::Vector<LobbyTableNew*> LobbyTableNew::_listTable;

LobbyTableNew::~LobbyTableNew()
{
    CC_SAFE_RELEASE_NULL(_layoutTable);
}

LobbyTableNew::LobbyTableNew(LobbyScene* lobbyScene, LobbyShopTableBoard* lobbyShopTableBoard, cocos2d::ui::Layout* layoutTable) : Ref()
{
    _lobbyScene                       = lobbyScene;
    _lobbyShopTableBoard              = lobbyShopTableBoard;
    _layoutTable                      = layoutTable;
    _imageViewTable                   = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutTable, "ImageTable"));
    _textBMLuckyPercent               = static_cast<Text*>(Helper::seekWidgetByName(_layoutTable, "TextBMLucky"));
    _imageViewCheckBox                = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutTable, "ImageCheck"));
    _imageViewCheck                   = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutTable, "ImageBoxCheck"));
    _buttonListView                   = static_cast<Button*>(Helper::seekWidgetByName(_layoutTable, "ButtonListView"));
    _buttonBuy                        = static_cast<Button*>(Helper::seekWidgetByName(_layoutTable, "ButtonBuy"));
    _textBMPrice                      = static_cast<TextBMFont*>(Helper::seekWidgetByName(_buttonBuy, "TextBMPrice"));
    _textBMDescrible                  = static_cast<Text*>(Helper::seekWidgetByName(_layoutTable, "TextBMDescrible"));
    _textBMConditionOpenPreviousTable = static_cast<Text*>(Helper::seekWidgetByName(_layoutTable, "TextBMDescribleCannotBuy"));
    _textBMConditionOpenPreviousTable->setString(GET_TEXT_BY_ID(TXT_LOBBY_SHOP_TABLE_CONDITION_OPEN_TABLE));
    _id           = 0;
    _name         = "";
    _price        = 0;
    _state        = State::NORMAL;
    _percentLucky = 0;
    _layoutTable->retain();
    autorelease();
}

void LobbyTableNew::initValue(int id, const std::string& name, long price, LobbyTableNew::State state, float percentLucky)
{
    _id = id;
    _imageViewTable->loadTexture(StringUtils::format("ui/ImageLobbyScene/image_mini_table_%i.png", _id), Widget::TextureResType::PLIST);

    this->setName(name);
    this->setPrice(price);
    this->setState(state);
    this->setPercentLucky(percentLucky);
    this->setTextDescrible();
    if ((_state == State::LOCK_CANNOT_UNLOCK || _state == State::LOCK_CAN_UNLOCK) && UserData::getInstance()->getMoney() >= _price)
        showBuyNotify(true);
    else
        showBuyNotify(false);
}

void LobbyTableNew::setListTable(const cocos2d::Vector<LobbyTableNew*>& listTable)
{
    _listTable = listTable;
}

int LobbyTableNew::getId()
{
    return _id;
}

const std::string& LobbyTableNew::getName()
{
    return _name;
}

void LobbyTableNew::setName(const std::string& name)
{
    _name = name;
}

void LobbyTableNew::setPrice(long price)
{
    _price = price;
    _textBMPrice->setString(GameUtils::MoneyFormat(price, "$"));
}

LobbyTableNew::State LobbyTableNew::getState()
{
    return _state;
}

void LobbyTableNew::setState(LobbyTableNew::State state)
{
    _state = state;
    switch (_state)
    {
        case LobbyTableNew::State::NORMAL:
            _buttonListView->setEnabled(true);
            _imageViewCheckBox->setVisible(false);
            _imageViewCheck->setVisible(true);
            _buttonBuy->setVisible(false);
            _textBMConditionOpenPreviousTable->setVisible(false);
            showBuyNotify(false);
            break;
        case LobbyTableNew::State::SELECTED:
            _buttonListView->setEnabled(false);
            _imageViewCheckBox->setVisible(true);
            _imageViewCheck->setVisible(true);
            _buttonBuy->setVisible(false);
            _buttonBuy->setColor(Color3B::WHITE);
            _textBMConditionOpenPreviousTable->setVisible(false);
            showBuyNotify(false);
            break;
        case LobbyTableNew::State::LOCK_CAN_UNLOCK:
            _buttonListView->setEnabled(true);
            _imageViewCheckBox->setVisible(false);
            _imageViewCheck->setVisible(false);
            _buttonBuy->setVisible(true);
            _buttonBuy->setEnabled(true);
            _buttonBuy->setColor(Color3B::WHITE);
            _textBMConditionOpenPreviousTable->setVisible(false);
            break;
        case LobbyTableNew::State::LOCK_CANNOT_UNLOCK:
            _buttonListView->setEnabled(true);
            _imageViewCheckBox->setVisible(false);
            _imageViewCheck->setVisible(false);
            _buttonBuy->setVisible(true);
            _buttonBuy->setEnabled(false);
            _buttonBuy->setColor(Color3B::GRAY);
            _textBMConditionOpenPreviousTable->setVisible(true);
            break;
        default:
            break;
    }
}

void LobbyTableNew::setPercentLucky(float percentLucky)
{
    _percentLucky = percentLucky;
    _textBMLuckyPercent->setString(StringUtils::format("%.1f %%", _percentLucky));
}

void LobbyTableNew::setTextDescrible()
{
    _textBMDescrible->setString(GET_TEXT_BY_ID(TXT_CHOOSE_BET_TABLE_HINT));
}

void LobbyTableNew::select()
{
    if (_state == State::NORMAL)
    {
        _lobbyScene->setLobbyTableSelected(this);
        this->setState(State::SELECTED);
        UserData::getInstance()->setIdTableSelected(_id);
        UserData::getInstance()->setLuckyValue(_percentLucky);
    }
    else if (_state == State::SELECTED)
    {
        this->setState(State::NORMAL);
    }
    else if (_state == State::LOCK_CANNOT_UNLOCK)
    {
        NotifyPopup* notifyPopup = BoardManager::createNotifyPopup();
        notifyPopup->initValue(GET_TEXT_BY_ID(TXT_LOBBY_SHOP_TABLE_CONDITION_OPEN_TABLE), nullptr);
        notifyPopup->attach(_lobbyScene, kAboveForeground);
    }
    else if (_state == State::LOCK_CAN_UNLOCK)
    {
        if (UserData::getInstance()->getMoney() >= _price)
        {
            ActionConfirmPopup* confirmPopup = BoardManager::createActionConfirmPopup();
            confirmPopup->initValue(GET_TEXT_BY_ID(TXT_BUY_TABLE_CONTENT), GET_TEXT_BY_ID(TXT_YESNOCONFIRM_NO_BUTTON), GET_TEXT_BY_ID(TXT_BUY_TABLE_CONFIRM), [=](ActionConfirmResult result) {
                if (result == ActionConfirmResult::CONFIRM_ACTION)
                    MessageSender::requestUnlockTable(_id);
                confirmPopup->close();
            });
            confirmPopup->attach(_lobbyScene, kAboveForeground);
        }
        else
        {
            MessageSender::requestUnlockTable(_id);
        }
    }
}

void LobbyTableNew::unlock()
{
    if (_state == State::NORMAL)
        this->select();
}

void LobbyTableNew::updateDataFromUnlockCityOrTable(const rapidjson::Value& data)
{
    State state = (State) JsonHelper::getInt(data, KEY_TABLE_STATUS, 0);
    this->setState(state);
    if ((_state == State::LOCK_CANNOT_UNLOCK || _state == State::LOCK_CAN_UNLOCK) && UserData::getInstance()->getMoney() >= _price)
        showBuyNotify(true);
    else
        showBuyNotify(false);
}

void LobbyTableNew::changeLanguage(Language language)
{
    this->setTextDescrible();
}

LobbyTableNew* LobbyTableNew::getLobbyTableByID(int id)
{
    for (int i = 0; i < _listTable.size(); i++)
        if (_listTable.at(i)->getId() == id)
            return _listTable.at(i);
    return nullptr;
}

void LobbyTableNew::showBuyNotify(bool show)
{
    auto notify = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutTable, "ImageNotify"));
    if (show)
    {
        notify->setVisible(true);
        ScaleTo* scaleUp   = ScaleTo::create(1.0f, 0.8f);
        ScaleTo* scaleDown = ScaleTo::create(0.1f, 0.4f);
        notify->runAction(Sequence::create(Repeat::create(Sequence::create(EaseElasticOut::create(scaleUp), scaleDown, nullptr), 1), ScaleTo::create(0.1f, 0.8f), nullptr));
    }
    else
    {
        notify->setVisible(false);
    }
}
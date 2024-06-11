#include "LobbyPlace.h"
#include "Scenes/LobbyScene.h"
#include "Boards/LobbyTicketFlightBoard.h"
#include "Manager/BoardManager.h"
#include "Manager/TextManager.h"
#include "Network/MessageSender.h"
#include "UserData/UserData.h"
#include "Define/TextDefine.h"
#include "Helper/GameUtils.h"
#include "FirebaseGG/Firebase.h"
#include "Helper/fmt/format.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace std;

#define LOCK_CITY_DIM_COLOR cocos2d::Color3B(255,255,255)
std::vector<LobbyPlace*> LobbyPlace::_listPlace;

LobbyPlace::~LobbyPlace()
{
    NotificationManager::getInstance()->unregisterNotification(this);
}

LobbyPlace::LobbyPlace(LobbyScene* lobbyScene, cocos2d::ui::Layout* layoutEntireCity)
{
    _lobbyScene       = lobbyScene;
    _layoutEntireCity = layoutEntireCity;
    _layoutCity       = static_cast<Layout*>(Helper::seekWidgetByName(layoutEntireCity, "PanelCity"));

    _panelUnlock = static_cast<Layout*>(Helper::seekWidgetByName(layoutEntireCity, "PanelUnlock"));
    _panelLock   = static_cast<Layout*>(Helper::seekWidgetByName(layoutEntireCity, "PanelLock"));

    _panelUnlock->setVisible(false);
    _panelLock->setVisible(false);

    auto buttonUnlock = static_cast<Button*>(Helper::seekWidgetByName(_panelLock, "ButtonUnlock"));
    buttonUnlock->addClickEventListener([=](cocos2d::Ref* sender) {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        MessageSender::requestUnlockCity(_id);
    });
    auto buttonAction = static_cast<Button*>(Helper::seekWidgetByName(_panelUnlock, "ButtonAction"));
    buttonAction->addClickEventListener([=](cocos2d::Ref* sender) {
        touchEnd();
    });

    auto buttonGuide = static_cast<Button*>(Helper::seekWidgetByName(layoutEntireCity, "ButtonGuide"));
    if (buttonGuide)
        buttonGuide->addClickEventListener([=](cocos2d::Ref* sender) {
            
        });
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_CHANGE_LANGUAGE, NOTIFYCALLBACK_SELECTOR(this, LobbyPlace::onNotify));
}

void LobbyPlace::initValue(int id, LobbyPlace::State state, const std::string& name, long price, float jackpotRatioBet, double requiredMoney)
{
    this->setID(id);
    this->setState(state);
    _name          = name;
    _price         = price;
    _betJackpot    = jackpotRatioBet;
    _requiredMoney = requiredMoney;

    auto textPrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(_panelLock, "Label"));
    textPrice->setString(GameUtils::MoneyFormat(_price, "$"));
    if (UserData::getInstance()->getMoney() >= _price)
        showBuyNotify(true);
    else
        showBuyNotify(false);
    this->changeLanguage(TextManager::getInstance()->getLanguageType());
}

void LobbyPlace::setListPlace(const std::vector<LobbyPlace*>& listPlace)
{
    _listPlace = listPlace;
}

int LobbyPlace::getID()
{
    return _id;
}

void LobbyPlace::setID(int id)
{
    _id = id;
}

LobbyPlace::State LobbyPlace::getState()
{
    return _state;
}

void LobbyPlace::setState(LobbyPlace::State state)
{
    _state = state;
    switch (_state)
    {
        case State::NORMAL:
        {
            _panelLock->setVisible(false);
            _panelUnlock->setVisible(true);
            _layoutCity->setColor(Color3B::WHITE);
            break;
        }
        case State::LOCK_CAN_UNLOCK:
        {
            _panelLock->setVisible(true);
            _panelUnlock->setVisible(false);
            _layoutCity->setColor(LOCK_CITY_DIM_COLOR);
            break;
        }
        case State::LOCK_CANNOT_UNLOCK:
        {
            _panelLock->setVisible(true);
            _panelUnlock->setVisible(false);
            _layoutCity->setColor(LOCK_CITY_DIM_COLOR);
            break;
        }
        default:
            break;
    }
}

const std::string& LobbyPlace::getName()
{
    return _name;
}

void LobbyPlace::setName(const std::string& name)
{
    _name = name;
}

long LobbyPlace::getPrice()
{
    return _price;
}

void LobbyPlace::setPrice(long price)
{
    _price = price;
}

void LobbyPlace::showBuyNotify(bool show)
{
    auto notify = static_cast<ImageView*>(Helper::seekWidgetByName(_panelLock, "ImageNotify"));
    notify->setVisible(false);
    if (show)
    {
        notify->setVisible(true);
        ScaleTo* scaleUp   = ScaleTo::create(1.0f, 0.8f);
        ScaleTo* scaleDown = ScaleTo::create(0.1f, 0.4f);
        notify->runAction(Sequence::create(Repeat::create(Sequence::create(EaseElasticOut::create(scaleUp), scaleDown, nullptr), 2), ScaleTo::create(0.1f, 0.8f), nullptr));
    }
}

void LobbyPlace::select()
{
    if (_state == LobbyPlace::State::NORMAL)
    {
        UserData::getInstance()->setIdPlaceSelected(_id);
        MessageSender::requestOpenTicket(_id, UserData::getInstance()->getIdTableSelected());
    }
    else
    {
        playActionVibration();
    }
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void LobbyPlace::unlock()
{
    this->setState(State::NORMAL);
}

void LobbyPlace::touchBegin()
{
    this->playActionTouchBegin();
}

void LobbyPlace::touchEnd()
{
    this->playActionTouchEnd();
    _layoutEntireCity->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]() {
        this->select();
    }), nullptr));
}

void LobbyPlace::reopenTicket()
{
    _layoutCity->stopAllActions();
    RotateBy * rotate1     = RotateBy::create(0.5f, Vec3(0, 120 + 360 * 2, 0));
    CallFuncN* functionMid = CallFuncN::create([=](Node* node) {
        node->setRotation3D(Vec3(0, 300, 0));
        UserData::getInstance()->setIdPlaceSelected(_id);
    });
    RotateTo * rotate2     = RotateTo::create(0.1f, Vec3(0, 360, 0));
    CallFuncN* functionEnd = CallFuncN::create([=](Node* node) {
        auto buttonCity = (Button*) Helper::seekWidgetByName(_layoutEntireCity, "Background");
        auto cloneLayer = buttonCity->clone();
        cloneLayer->setPosition(buttonCity->getPosition());
        cloneLayer->setScale(1.0f);
        buttonCity->getParent()->addChild(cloneLayer, buttonCity->getLocalZOrder() - 1);
        cloneLayer->setCascadeOpacityEnabled(true);
        cloneLayer->setOpacity(90);
        cloneLayer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.75f, 1.5f), FadeTo::create(0.75f, 0), nullptr), RemoveSelf::create(), nullptr));
    });
    Sequence * sequence    = Sequence::create(EaseCubicActionIn::create(rotate1), functionMid, EaseCubicActionIn::create(rotate2), functionEnd, DelayTime::create(0.5f), nullptr);
    _layoutCity->runAction(sequence);
}

bool LobbyPlace::checkTouchCity(cocos2d::ui::Layout* layout)
{
    if (_layoutEntireCity == layout)
        return true;
    return false;
}

void LobbyPlace::fadeOut()
{
    FadeOut  * fadeOut = FadeOut::create(0.4f);
    CallFuncN* funcEnd = CallFuncN::create([=](Node* object) {
        object->setVisible(false);
    });
    _layoutEntireCity->runAction(Sequence::create(fadeOut, funcEnd, nullptr));
}

void LobbyPlace::fadeIn()
{
    CallFuncN* funcStart = CallFuncN::create([=](Node* object) {
        object->setVisible(true);
    });
    FadeIn   * fadeIn    = FadeIn::create(0.4f);
    _layoutEntireCity->runAction(Sequence::create(funcStart, fadeIn, nullptr));
}

void LobbyPlace::playActionTouchBegin()
{
    auto scaleTo = ScaleTo::create(0.1f, 0.95f);
    _layoutEntireCity->runAction(EaseExponentialOut::create(scaleTo));
}

void LobbyPlace::playActionTouchEnd()
{
    auto scaleTo = ScaleTo::create(0.1f, 1.0f);
    _layoutEntireCity->runAction(EaseExponentialOut::create(scaleTo));
}

void LobbyPlace::playActionVibration()
{
    stopActionVibration();
    Vector<FiniteTimeAction*> list;
    Point                     pos = _layoutCity->getPosition();
    for (int                  i   = 5; i >= 0; i--)
    {
        if (i % 2 == 0)
            list.pushBack(EaseExponentialInOut::create(MoveTo::create(0.075f, Point(pos.x - i * 0.75f, pos.y))));
        else
            list.pushBack(EaseExponentialInOut::create(MoveTo::create(0.075f, Point(pos.x + i * 0.75f, pos.y))));
    }
    _layoutCity->runAction(Sequence::create(list));
}

void LobbyPlace::stopActionVibration()
{
    _layoutCity->stopAllActions();
    _layoutCity->setPosition(Vec2(_layoutEntireCity->getContentSize().width / 2, _layoutEntireCity->getContentSize().height / 2));
}

LobbyPlace* LobbyPlace::getLobbyPlaceByID(int id)
{
    for (int i = 0; i < _listPlace.size(); i++)
        if (_listPlace[i]->getID() == id)
            return _listPlace[i];
    return nullptr;
}

void LobbyPlace::changeLanguage(Language language)
{
    Text* textTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutEntireCity, "TextTitle"));
	if (_id == (int)CityType::CLASSIC_GIN)
		textTitle->setString(GET_TEXT_BY_ID(TXT_ROOM_3_LOBBY_1));
	else if (_id == (int)CityType::STRAIGHT_GIN)
		textTitle->setString(GET_TEXT_BY_ID(TXT_ROOM_3_LOBBY_2));
	else if (_id == (int)CityType::OKLAHOMA_GIN)
		textTitle->setString(GET_TEXT_BY_ID(TXT_ROOM_3_LOBBY_3));

    auto label2 = static_cast<Text*>(Helper::seekWidgetByName(_panelUnlock, "ButtonAction")->getChildByName("Label"));
    label2->setString(GET_TEXT_BY_ID(TXT_SELECT_ROOM_TITLE));
}

void LobbyPlace::onNotify(void* sender, int id, const cocos2d::Value& data)
{
    switch (id)
    {
        case EVENT_CHANGE_LANGUAGE:
        {
            ValueMap arr  = data.asValueMap();
            Language lang = (Language) arr[LANGUAGE_TYPE_KEY].asInt();
            changeLanguage(lang);
            break;
        }
        default:
            break;
    }
}

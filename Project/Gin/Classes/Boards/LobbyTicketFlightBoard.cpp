#include "LobbyTicketFlightBoard.h"
#include "Component/LobbyPlace.h"
#include "Scenes/LobbyScene.h"
#include "Network/MessageSender.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"
#include "Helper/GameUtils.h"
#include "UserData/UserData.h"
#include "Manager/NotificationManager.h"
#include "Manager/BoardManager.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace ui;

#define MARGIN_BET 40.0f
#define MARGIN_TARGET_SCORE 40.0f

#define TARGET_SCORE_SELECTED_SCALE 1.2f
#define TARGET_SCORE_UNSELECTED_SCALE 1.0f

#define TARGET_SCORE_SELECTED_OPACITY 255.0f
#define TARGET_SCORE_UNSELECTED_OPACITY 155.0f

#define TAG_TARGET_SCORE_LAYOUT_ANIM 11

LobbyTicketFlightBoard::~LobbyTicketFlightBoard()
{
    CC_SAFE_RELEASE_NULL(_layoutBetPoint);
    NotificationManager::getInstance()->unregisterNotification(this);
}

LobbyTicketFlightBoard::LobbyTicketFlightBoard() : InterfacePopupBoard()
{
    _rule                      = 0;
    _draggingSlider            = false;
    _draggingTargetScoreSlider = false;
}

bool LobbyTicketFlightBoard::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void LobbyTicketFlightBoard::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/LobbyTicketFlightBoard.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);
    _layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "LayoutContent"));

    _sliderBet = static_cast<Slider*>(Helper::seekWidgetByName(_layoutContent, "SliderBet"));
    _sliderBet->setMaxPercent(100);
    _sliderBet->addEventListener(CC_CALLBACK_2(LobbyTicketFlightBoard::sliderEvent, this));
    _sliderBet->addTouchEventListener(CC_CALLBACK_2(LobbyTicketFlightBoard::onSliderTouch, this));

    _progressBall = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutContent, "ImageInnerBar"));
    _progressBall->setLocalZOrder(2);

    _buttonFlightNow = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonFlightNow"));
    _buttonFlightNow->addTouchEventListener(CC_CALLBACK_2(LobbyTicketFlightBoard::onButtonTouch, this));

    _textBMFlightNow = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextBMFilghtNow"));
    _textBMFlightNow->setLocalZOrder(2);
    _textLuckyValue = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextLuckyPercent"));

    _textBMTitle    = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTitle"));
    _textTableTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextTableTitle"));
    _buttonClose    = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(LobbyTicketFlightBoard::onButtonClick, this));

    _layoutBetPoint = static_cast<Layout*>(Helper::seekWidgetByName((Layout*) _sliderBet->getParent(), "PanelBetPoint"));
    _layoutBetPoint->retain();
    _layoutBetPoint->removeFromParent();

    _layoutTable = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelTouchTable"));
    _layoutTable->addTouchEventListener(CC_CALLBACK_2(LobbyTicketFlightBoard::onButtonTouch, this));

    _imageTable    = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutTable, "ImageTable"));
    _textTableHint = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextLuckyHint"));
    _layoutLucky   = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelLucky"));
    _layoutLucky->addTouchEventListener(CC_CALLBACK_2(LobbyTicketFlightBoard::onButtonTouch, this));

    _sliderOriginalSize = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelSliderBet"))->getContentSize();

    _sliderTargetScore = static_cast<Slider*>(Helper::seekWidgetByName(_layoutContent, "SliderTargetScore"));
    _sliderTargetScore->setMaxPercent(100);
    _sliderTargetScore->addEventListener(CC_CALLBACK_2(LobbyTicketFlightBoard::targetScoreSliderEvent, this));
    _sliderTargetScore->addTouchEventListener(CC_CALLBACK_2(LobbyTicketFlightBoard::onTargetScoreSliderTouch, this));

    _layoutScore = static_cast<Layout*>(Helper::seekWidgetByName((Layout*) _sliderTargetScore->getParent(), "PanelScore"));
    _layoutScore->retain();
    _layoutScore->removeFromParent();

    _imageTargetScoreSliderControl = static_cast<ImageView*>(Helper::seekWidgetByName((Layout*) _sliderTargetScore->getParent(), "ImageSliderControl"));
    _imageTargetScoreSliderControl->setLocalZOrder(2);

    Layout* layoutBetPointSoFar = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelBetPointSoFar"));
    layoutBetPointSoFar->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        Node* node = (Node*) sender;
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            if (node->getActionByTag(11) == nullptr)
            {
                node->setScale(1.0f);
                node->runAction(getZoomInActionBetPoint());
                if (_betRange.size() > 0)
                    selectBetRange(_betRange.size() - 1);
                SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            }
        }
    });
    this->changeLanguage(TextManager::getInstance()->getLanguageType());

    _layoutExtra    = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelExtra"));
    _imageJackpot   = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutExtra, "ImageJackpot"));
    _imageMultiply  = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutExtra, "ImageMultiply"));
    _textBMMultiply = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutExtra, "TextBMMultiply"));

    //    _imageJackpot->setVisible(false);
    //    _imageMultiply->setVisible(false);
    _layoutExtra->setVisible(false);

    _layoutLucky->setVisible(false);
}

void LobbyTicketFlightBoard::initValue(CityType city, double bet, float luckyPercent)
{
    _cityType     = city;
    _bet          = bet;
    _luckyPercent = luckyPercent;
    this->setLuckyValue(luckyPercent);
    updateBetRange();
    updateTargetScoreRange();
    setTable(UserData::getInstance()->getIdTableSelected());
}

std::vector<double> LobbyTicketFlightBoard::getBetRange(double money)
{
    if (s_betRange.find((int) _rule) != s_betRange.end())
    {
        const std::map<int, std::map<double, std::vector<double>>>& map = s_betRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        const std::map<double, std::vector<double>>& mapCities = map.at(cityType);
        for (auto it = mapCities.rbegin(); it != mapCities.rend(); ++it)
            if (money >= it->first)
                return it->second;
    }
    return std::vector<double>();
}

std::pair<double, std::vector<double>> LobbyTicketFlightBoard::getBetRangeSoFar(double money)
{
    if (s_betRange.find((int) _rule) != s_betRange.end())
    {
        const std::map<int, std::map<double, std::vector<double>>>& map = s_betRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        std::vector<double> betRange = getBetRange(money);
        if (betRange.size() > 0)
        {
            const std::map<double, std::vector<double>>& mapCities = map.at(cityType);
            for (auto it = mapCities.begin(); it != mapCities.end(); it++)
            {
                if (money < it->first && it->second.size() > 0 && betRange[betRange.size() - 1] != it->second[it->second.size() - 1])
                    return make_pair(it->first, it->second);
            }
        }
    }
    return make_pair(-1, std::vector<double>());
}

void LobbyTicketFlightBoard::selectBetRange(int index)
{
    if (index >= 0 && index < _betRange.size())
    {
        float percent = (index * 100.0f) / (_betRange.size() - 1);
        _sliderBet->setPercent(percent);
        _bet = _betRange[index];
        updateUIAcordingPercent(_sliderBet);
        updateBetMoney(index);
        UserData::getInstance()->setBet(_bet);
    }
}

void LobbyTicketFlightBoard::updateBetMoney(int index)
{
    if (index >= 0 && index < _listLayoutBetPoint.size())
    {
        Layout* betPoint = _listLayoutBetPoint.at(index);
        Layout* betmoney = (Layout*) betPoint->getChildByName("ImageBetMoney");
        if (!betmoney->isVisible() && _draggingSlider)
            SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);

        for (int i = 0; i < _listLayoutBetPoint.size(); i++)
        {
            auto bP = (Layout*) _listLayoutBetPoint.at(i)->getChildByName("ImageBetMoney");
            bP->setVisible(false);

            auto label = (TextBMFont*) _listLayoutBetPoint.at(i)->getChildByName("Label");
            label->setVisible(true);
        }
        betmoney->setVisible(true);

        auto textMoney = (TextBMFont*) _listLayoutBetPoint.at(index)->getChildByName("Label");
        textMoney->setVisible(false);

        if (_listLayoutBetPoint.at(index)->getActionByTag(11) == nullptr)
        {
            _listLayoutBetPoint.at(index)->setScale(1.0f);
            _listLayoutBetPoint.at(index)->runAction(getZoomInActionBetPoint());
        }
    }
}

int LobbyTicketFlightBoard::getBetRangeIndex(double betMoney)
{
    for (int i = 0; i < _betRange.size(); i++)
        if (_betRange[i] == betMoney)
            return i;
    return -1;
}

void LobbyTicketFlightBoard::updateBetRange()
{
    updateBetRangeSoFar();
    for (int i = 0; i < _listLayoutBetPoint.size(); i++)
        _listLayoutBetPoint.at(i)->removeFromParent();
    _listLayoutBetPoint.clear();
    _listLayoutBetPointPos.clear();
    _betRange = getBetRange(UserData::getInstance()->getMoney());
    for (int i = 0; i < _betRange.size(); i++)
    {
        Layout* betPoint = (Layout*) _layoutBetPoint->clone();
        betPoint->setCascadeOpacityEnabled(true);
        betPoint->setLocalZOrder(1);
        betPoint->setSwallowTouches(false);
        TextBMFont* label = (TextBMFont*) betPoint->getChildByName("Label");
        label->setString(GameUtils::MoneyFormatShortCut(_betRange[i], ""));

        Layout* betmoney = (Layout*) betPoint->getChildByName("ImageBetMoney");
        label = (TextBMFont*) betmoney->getChildByName("TextBMBetMoney");
        label->setString(GameUtils::MoneyFormatShortCut(_betRange[i], ""));

        Text* labelNote = (Text*) betPoint->getChildByName("TextNote");
        labelNote->setVisible(i == _betRange.size() - 1 ? true : false);
        labelNote->setString(GET_TEXT_BY_ID(TXT_BET_MAX));

        betmoney->setVisible(false);

        float posY = _sliderBet->getScaleY() * _sliderBet->getContentSize().height / 2;
        float posX = (1.0f * i * _sliderBet->getScaleX() * (_sliderBet->getContentSize().width - MARGIN_BET)) / (_betRange.size() - 1) + MARGIN_BET / 2;

        betPoint->setPosition(_sliderBet->getParent()->convertToNodeSpace(_sliderBet->convertToWorldSpace(Point(posX, posY))));
        _sliderBet->getParent()->addChild(betPoint);

        betPoint->setTag(i);
        betPoint->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
            Node* node = (Node*) sender;
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                _draggingSlider = true;
                selectBetRange(node->getTag());
                _progressBall->stopAllActions();
                _progressBall->runAction(Sequence::create(ScaleTo::create(0.1f, 1.0f), ScaleTo::create(0.1f, 0.8f), nullptr));
                if (node->getActionByTag(11) == nullptr)
                {
                    node->setScale(1.0f);
                    node->runAction(getZoomInActionBetPoint());
                }
            }
        });
        _listLayoutBetPoint.pushBack(betPoint);
        _listLayoutBetPointPos.push_back(betPoint->getParent()->convertToWorldSpace(betPoint->getPosition()));
    }

    int index = getBetRangeIndex(UserData::getInstance()->getBet());
    if (index != -1)
    {
        selectBetRange(index);
    }
    else
    {
        if (_betRange.size() >= 2 && _betRange.size() <= 5)
            selectBetRange(1);
        else
            selectBetRange(2);
    }
}

void LobbyTicketFlightBoard::updateBetRangeSoFar()
{
    std::vector<double>                    betRange      = getBetRange(UserData::getInstance()->getMoney());
    std::pair<double, std::vector<double>> betRangeSoFar = getBetRangeSoFar(UserData::getInstance()->getMoney());
    Layout* _layoutBetRangeSoFar = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelSliderBetSoFar"));

    if (betRangeSoFar.first != -1 && betRangeSoFar.second.size() > 0)
    {
        float width = (_sliderOriginalSize.width - MARGIN_BET) / (betRange.size());

        Layout* layoutBetRange = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelSliderBet"));
        layoutBetRange->setContentSize(Size(_sliderOriginalSize.width - width, _sliderOriginalSize.height));
        Helper::doLayout(layoutBetRange);
        _layoutBetRangeSoFar->setVisible(true);

        Layout    * layoutBetPointSoFar = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelBetPointSoFar"));
        TextBMFont* textBetSofar        = static_cast<TextBMFont*>(Helper::seekWidgetByName(layoutBetPointSoFar, "Label"));
        Text      * textBetSofarRequire = static_cast<Text*>(Helper::seekWidgetByName(layoutBetPointSoFar, "TextRequire"));

        textBetSofar->setString(GameUtils::MoneyFormatShortCut(betRangeSoFar.second[betRangeSoFar.second.size() - 1], ""));
        textBetSofarRequire->setString(fmt::format(GET_TEXT_BY_ID(TXT_BET_SO_FAR_REQUIRE), GameUtils::MoneyFormatShortCut(betRangeSoFar.first, "$")));
    }
    else
    {
        _layoutBetRangeSoFar->setVisible(false);
        Layout* layoutBetRange = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelSliderBet"));
        layoutBetRange->setContentSize(_sliderOriginalSize);
        Helper::doLayout(layoutBetRange);
    }
}

int LobbyTicketFlightBoard::getBetRangeFromPercent(float percent)
{
    Point point;
    point.x = (percent / 100.0f) * _sliderBet->getContentSize().width;
    point.y = 0;
    float    min   = 999999999;
    int      index = 0;
    for (int i     = 0; i < _listLayoutBetPoint.size(); i++)
    {
        Point worldPos1 = _listLayoutBetPointPos.at(i);
        Point worldPos2 = _sliderBet->convertToWorldSpace(point);
        float dist      = worldPos1.distance(worldPos2);
        if (dist <= min)
        {
            min   = dist;
            index = i;
        }
    }
    return index;
}

int LobbyTicketFlightBoard::getTargetScoreRangeFromPercent(float percent)
{
    Point point;
    point.x = (percent / 100.0f) * _sliderTargetScore->getContentSize().width;
    point.y = 0;
    float    min   = 999999999;
    int      index = 0;
    for (int i     = 0; i < _listLayoutTargetScore.size(); i++)
    {
        Point worldPos1 = _listLayoutTargetScorePos.at(i);
        Point worldPos2 = _sliderTargetScore->convertToWorldSpace(point);
        float dist      = worldPos1.distance(worldPos2);
        if (dist <= min)
        {
            min   = dist;
            index = i;
        }
    }
    return index;
}

void LobbyTicketFlightBoard::onEnter()
{
    InterfacePopupBoard::onEnter();
    NotificationManager::getInstance()->registerNotification(this, EVENT_CHANGE_TABLE, NOTIFYCALLBACK_SELECTOR(this, LobbyTicketFlightBoard::onNotifyEvent));
}

void LobbyTicketFlightBoard::onExit()
{
    InterfacePopupBoard::onExit();
    _state = State::CLOSED;
    NotificationManager::getInstance()->unregisterNotification(this, EVENT_CHANGE_TABLE);
}

void LobbyTicketFlightBoard::onButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        {
            if (pSender == _layoutTable)
            {
                auto scaleTo = ScaleTo::create(0.1f, 0.95f);
                _layoutTable->runAction(EaseExponentialOut::create(scaleTo));

            }
        }
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            if (pSender == _buttonFlightNow)
            {
                MessageSender::requestFlightNow(UserData::getInstance()->getIdPlaceSelected(), UserData::getInstance()->getIdTableSelected(), _bet, _targetScore);
                if (_state == InterfacePopupBoard::State::OPENED)
                    this->close();
                UserData::getInstance()->setBet(_bet);
                SoundManager::playSoundEffect(SoundEvent::TAKE_OFF);
            }
            else if (pSender == _layoutTable)
            {
                auto scaleTo = ScaleTo::create(0.1f, 1.0f);
                _layoutTable->runAction(Sequence::create(EaseExponentialOut::create(scaleTo), CallFunc::create([=]() {
                    _lobbyScene->openShopTable();
                }), nullptr));
                SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            }
            else if (pSender == _layoutLucky)
            {
                SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            }
        }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
        {
            if (pSender == _buttonFlightNow)
            {
            }
            else if (pSender == _layoutTable)
            {
                auto scaleTo = ScaleTo::create(0.1f, 1.0f);
                _layoutTable->runAction(EaseExponentialOut::create(scaleTo));
            }
        }
            break;
        default:
            break;
    }
}

void LobbyTicketFlightBoard::sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event)
{
    Slider* slider = (Slider*) sender;
    if (event == Slider::EventType::ON_PERCENTAGE_CHANGED)
        updateBetMoney(getBetRangeFromPercent(slider->getPercent()));
    else if (event == Slider::EventType::ON_SLIDEBALL_UP || event == Slider::EventType::ON_SLIDEBALL_DOWN || event == Slider::EventType::ON_SLIDEBALL_CANCEL)
        selectBetRange(getBetRangeFromPercent(slider->getPercent()));
    float width = slider->getContentSize().width - MARGIN_BET;
    _progressBall->setPositionX((slider->getPositionX() - width / 2) + width * ((float) slider->getPercent() * 1.0f / _sliderBet->getMaxPercent()));
}

void LobbyTicketFlightBoard::onSliderTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        {
            _draggingSlider = true;
            _progressBall->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.0f);
            _progressBall->runAction(scaleTo);
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
        {
            _draggingSlider = false;
            _progressBall->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 0.8f);
            _progressBall->runAction(scaleTo);
            break;
        }
    }
}

void LobbyTicketFlightBoard::onTargetScoreSliderTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        {
            _draggingTargetScoreSlider = true;
            _imageTargetScoreSliderControl->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.0f);
            _imageTargetScoreSliderControl->runAction(scaleTo);
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
        {
            _draggingTargetScoreSlider = false;
            _imageTargetScoreSliderControl->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 0.8f);
            _imageTargetScoreSliderControl->runAction(scaleTo);
            break;
        }
    }
}

void LobbyTicketFlightBoard::updateTargetScore(int index)
{
    if (index >= 0 && index < _listLayoutTargetScore.size())
    {
        if (_draggingTargetScoreSlider)
            SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);

        for (int i = 0; i < _listLayoutTargetScore.size(); i++)
        {
            auto layout = _listLayoutTargetScore.at(i);

            Layout* layoutExtra = (Layout*)  layout->getChildByName("PanelExtra");
            Layout* layoutJP = (Layout*) layoutExtra->getChildByName("PanelJackpot");
            Layout* layoutMulti = (Layout*) layoutExtra->getChildByName("PanelMultiply");

            ImageView* imageJPBg = (ImageView*) layoutJP->getChildByName("ImageBg");
            ImageView* imageMultiBg = (ImageView*) layoutMulti->getChildByName("ImageBg");

            if(i == index)
            {
                this->playTargetScoreAnim(layout, TARGET_SCORE_SELECTED_SCALE, TARGET_SCORE_SELECTED_OPACITY);
            }
            else
            {
                this->playTargetScoreAnim(layout, TARGET_SCORE_UNSELECTED_SCALE, TARGET_SCORE_UNSELECTED_OPACITY);
            }
        }
    }
}

void LobbyTicketFlightBoard::selectTargetScoreRange(int index)
{
    if (index >= 0 && index < _targetScoreRange.size())
    {
        float percent = (index * 100.0f) / (_targetScoreRange.size() - 1);
        _sliderTargetScore->setPercent(percent);
        _targetScore = _targetScoreRange[index];
        updateTargetScoreUIAcordingPercent(_sliderTargetScore);
        updateTargetScore(index);
        UserData::getInstance()->setTargetScore(_targetScore);
    }
}

void LobbyTicketFlightBoard::targetScoreSliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event)
{
    Slider* slider = (Slider*) sender;
    if (event == Slider::EventType::ON_PERCENTAGE_CHANGED)
        updateTargetScore(getTargetScoreRangeFromPercent(slider->getPercent()));
    else if (event == Slider::EventType::ON_SLIDEBALL_UP || event == Slider::EventType::ON_SLIDEBALL_DOWN || event == Slider::EventType::ON_SLIDEBALL_CANCEL)
        selectTargetScoreRange(getTargetScoreRangeFromPercent(slider->getPercent()));
    float width = slider->getContentSize().width - MARGIN_BET;
    _imageTargetScoreSliderControl->setPositionX((slider->getPositionX() - width / 2) + width * ((float) slider->getPercent() * 1.0f / _sliderBet->getMaxPercent()));
}

void LobbyTicketFlightBoard::updateUIAcordingPercent(cocos2d::ui::Slider* slider)
{
    float width = slider->getContentSize().width - MARGIN_BET;
    _progressBall->setPositionX((slider->getPositionX() - width / 2) + (width) * ((float) slider->getPercent() / _sliderBet->getMaxPercent()));
}

void LobbyTicketFlightBoard::updateTargetScoreUIAcordingPercent(cocos2d::ui::Slider* slider)
{
    float width = slider->getContentSize().width - MARGIN_TARGET_SCORE;
    _imageTargetScoreSliderControl->setPositionX((slider->getPositionX() - width / 2) + (width) * ((float) slider->getPercent() / _sliderTargetScore->getMaxPercent()));
}

int LobbyTicketFlightBoard::getTargetScoreRangeIndex(int targetScore)
{
    for (int i = 0; i < _targetScoreRange.size(); i++)
        if (_targetScoreRange[i] == targetScore)
            return i;
    return -1;
}

void LobbyTicketFlightBoard::updateTargetScoreRange()
{
    for (int i = 0; i < _listLayoutTargetScore.size(); i++)
        _listLayoutTargetScore.at(i)->removeFromParent();
    _listLayoutTargetScore.clear();
    _listLayoutTargetScorePos.clear();
    _targetScoreRange = getTargetScoreRange();
    for (int i = 0; i < _targetScoreRange.size(); i++)
    {
        Layout* tScore = (Layout*) _layoutScore->clone();
        tScore->setCascadeOpacityEnabled(true);
        tScore->setLocalZOrder(1);
        tScore->setSwallowTouches(false);

        Layout* layoutTargetScore = (Layout*)tScore->getChildByName("PanelTargetScore");
        TextBMFont* label = (TextBMFont*) layoutTargetScore->getChildByName("Label");
        label->setString(fmt::format(GET_TEXT_BY_ID(TXT_TARGET_SCORE_TEMPLATE), GameUtils::MoneyFormatShortCut(_targetScoreRange[i], "")));

        TextBMFont* labelTitle = (TextBMFont*) layoutTargetScore->getChildByName("LabelTitle");
        labelTitle->setString(GET_TEXT_BY_ID(TXT_TARGET_SCORE));

        Layout* layoutExtra = (Layout*) tScore->getChildByName("PanelExtra");
        Layout* layoutJP = (Layout*) layoutExtra->getChildByName("PanelJackpot");
        Layout* layoutMulti = (Layout*) layoutExtra->getChildByName("PanelMultiply");
        TextBMFont* textMultiply = (TextBMFont*) layoutMulti->getChildByName("TextMultiply");
        textMultiply->setString(fmt::format("X{0}", (int)this->getMaxMultiplyValue(_targetScoreRange[i])));

        float jpmulti = this->getJackpotBetMultiply(_targetScoreRange[i]);
        if(jpmulti <= 0)
        {
            layoutJP->setVisible(false);
        }
        else
        {
            layoutJP->setVisible(true);
        }

        float posY = _sliderTargetScore->getScaleY() * _sliderTargetScore->getContentSize().height / 2;
        float posX = (1.0f * i * _sliderTargetScore->getScaleX() * (_sliderTargetScore->getContentSize().width - MARGIN_TARGET_SCORE)) / (_targetScoreRange.size() - 1) + MARGIN_TARGET_SCORE / 2;

        tScore->setPosition(_sliderTargetScore->getParent()->convertToNodeSpace(_sliderTargetScore->convertToWorldSpace(Point(posX, posY))));
        _sliderTargetScore->getParent()->addChild(tScore);

        tScore->setTag(i);
        tScore->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
            Node* node = (Node*) sender;
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                _draggingTargetScoreSlider = true;
                selectTargetScoreRange(node->getTag());
                _imageTargetScoreSliderControl->stopAllActions();
                _imageTargetScoreSliderControl->runAction(Sequence::create(ScaleTo::create(0.1f, 1.0f), ScaleTo::create(0.1f, 0.8f), nullptr));

//                this->updateTargetScore(node->getTag());
            }
        });
        _listLayoutTargetScore.pushBack(tScore);
        _listLayoutTargetScorePos.push_back(tScore->getParent()->convertToWorldSpace(tScore->getPosition()));
    }

    int index = getTargetScoreRangeIndex(UserData::getInstance()->getTargetScore());
    if (index != -1)
    {
        selectTargetScoreRange(index);
    }
    else
    {
        selectTargetScoreRange(0);
    }
}

std::vector<int> LobbyTicketFlightBoard::getTargetScoreRange()
{
    if (s_targetScoreDataRange.find((int) _rule) != s_targetScoreDataRange.end())
    {
        const std::map<int, std::vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        const std::vector<TargetScoreData>& listTargetScoreData = map.at(cityType);
        std::vector<int> listTargetScore;
        for (const auto& scoreData : listTargetScoreData)
            listTargetScore.push_back(scoreData._targetScore);
        return listTargetScore;
    }
    return std::vector<int>();
}

void LobbyTicketFlightBoard::setLuckyValue(float luckyVal)
{
    _textLuckyValue->setString(StringUtils::format("%.1f %%", luckyVal * 100));
}

void LobbyTicketFlightBoard::changeLanguage(Language language)
{
    _textBMTitle->setString(GET_TEXT_BY_ID(TXT_LOBBY_TICKETFLIGHT_TITLE));
    _textBMFlightNow->setString(GET_TEXT_BY_ID(TXT_LOBBY_TICKETFLIGHT_BUTTON_PLAY));

    _textTableTitle->setString(GET_TEXT_BY_ID(TXT_CHOOSE_BET_TABLE_TITLE));
    _textTableHint->setString(GET_TEXT_BY_ID(TXT_CHOOSE_BET_TABLE_HINT));
}

void LobbyTicketFlightBoard::setTable(int tableId)
{
    _imageTable->loadTexture(StringUtils::format("ui/ImageLobbyScene/image_mini_table_%i.png", tableId), Widget::TextureResType::PLIST);
}

void LobbyTicketFlightBoard::onNotifyEvent(void* sender, int id, cocos2d::Ref* data)
{
    switch (id)
    {
        case EVENT_CHANGE_TABLE:
        {
            MessageSender::requestOpenTicket(UserData::getInstance()->getIdPlaceSelected(), UserData::getInstance()->getIdTableSelected());
            break;
        }
        default:
            break;
    }
}

void LobbyTicketFlightBoard::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
        close();
}

cocos2d::FiniteTimeAction* LobbyTicketFlightBoard::getZoomInActionBetPoint()
{
    auto seq = Sequence::create(ScaleTo::create(0.1f, 1.1f), ScaleTo::create(0.1f, 1.0f), DelayTime::create(0.2f), nullptr);
    seq->setTag(11);
    return seq;
}

cocos2d::FiniteTimeAction* LobbyTicketFlightBoard::getActionTargetScore(float scale, float opacity, float duration, bool isScaleUp)
{
    if(isScaleUp == true)
    {
        auto seq = Spawn::create(Sequence::create(ScaleTo::create(duration/2, scale*1.2f),
                                                  ScaleTo::create(duration/2, scale),
                                                  nullptr),
                                 FadeTo::create(duration, opacity),
                                 nullptr);
        seq->setTag(TAG_TARGET_SCORE_LAYOUT_ANIM);
        return seq;
    }
    else
    {
        auto seq = Spawn::create(EaseBackOut::create(ScaleTo::create(duration, scale)),
                                 FadeTo::create(duration, opacity),
                                 nullptr);
        seq->setTag(TAG_TARGET_SCORE_LAYOUT_ANIM);
        return seq;
    }
}

void LobbyTicketFlightBoard::playTargetScoreAnim(cocos2d::ui::Layout* layout, float scale, float opacity)
{
   if(layout == nullptr)
   {
       return;
   }

   layout->stopActionByTag(TAG_TARGET_SCORE_LAYOUT_ANIM);

   float duration = 0.2f;
   duration *= (abs(layout->getScale()-scale)/(TARGET_SCORE_SELECTED_SCALE - TARGET_SCORE_UNSELECTED_SCALE));

   float isScaleUp = (layout->getScale() < scale);

   layout->runAction(getActionTargetScore(scale, opacity, duration, isScaleUp));
}

float LobbyTicketFlightBoard::getJackpotBetMultiply()
{
    if (s_targetScoreDataRange.find((int) _rule) != s_targetScoreDataRange.end())
    {
        const std::map<int, std::vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        const std::vector<TargetScoreData>& listTargetScoreData = map.at(cityType);
        for (const auto                   & scoreData : listTargetScoreData)
            if (scoreData._targetScore == _targetScore)
                return scoreData._jackpotBetMultiply;
    }
    return 0;
}

float LobbyTicketFlightBoard::getJackpotBetMultiply(int targetScore)
{
    if (s_targetScoreDataRange.find((int) _rule) != s_targetScoreDataRange.end())
    {
        const std::map<int, std::vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        const std::vector<TargetScoreData>& listTargetScoreData = map.at(cityType);
        for (const auto                   & scoreData : listTargetScoreData)
            if (scoreData._targetScore == targetScore)
                return scoreData._jackpotBetMultiply;
    }
    return 0;
}

float LobbyTicketFlightBoard::getMaxMultiplyValue()
{
    if (s_targetScoreDataRange.find((int) _rule) != s_targetScoreDataRange.end())
    {
        const std::map<int, std::vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        const std::vector<TargetScoreData>& listTargetScoreData = map.at(cityType);
        float maxValue = 1;
        for (const auto& scoreData : listTargetScoreData)
            if (scoreData._targetScore == _targetScore)
                for (const auto& it : scoreData._multiplyDataMap)
                    if (maxValue < it.second)
                        maxValue = it.second;
        return maxValue;
    }
    return 1;
}

float LobbyTicketFlightBoard::getMaxMultiplyValue(int targetScore)
{
    if (s_targetScoreDataRange.find((int) _rule) != s_targetScoreDataRange.end())
    {
        const std::map<int, std::vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        const std::vector<TargetScoreData>& listTargetScoreData = map.at(cityType);
        float maxValue = 1;
        for (const auto& scoreData : listTargetScoreData)
            if (scoreData._targetScore == targetScore)
                for (const auto& it : scoreData._multiplyDataMap)
                    if (maxValue < it.second)
                        maxValue = it.second;
        return maxValue;
    }
    return 1;
}

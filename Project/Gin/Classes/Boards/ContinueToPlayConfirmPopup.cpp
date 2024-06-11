#include "ContinueToPlayConfirmPopup.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"
#include "UserData/UserData.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace ui;
using namespace std;

#define MARGIN_BET 40.0f
#define MARGIN_TARGET_SCORE 40.0f

#define TARGET_SCORE_SELECTED_SCALE 1.2f
#define TARGET_SCORE_UNSELECTED_SCALE 1.0f

#define TARGET_SCORE_SELECTED_OPACITY 255.0f
#define TARGET_SCORE_UNSELECTED_OPACITY 200.0f

#define TAG_TARGET_SCORE_LAYOUT_ANIM 11

ContinueToPlayConfirmPopup::ContinueToPlayConfirmPopup()
{
    _quitCallback              = nullptr;
    _continueCallback          = nullptr;
    _rule                      = 0;
    _targetScore               = 0;
    _draggingSlider            = false;
    _draggingTargetScoreSlider = false;
}

ContinueToPlayConfirmPopup::~ContinueToPlayConfirmPopup()
{
    CC_SAFE_RELEASE_NULL(_layoutBetPoint);
}

bool ContinueToPlayConfirmPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void ContinueToPlayConfirmPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node  = CSLoader::createNode("ui/Common/ContinueToPlayConfirmPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "LayoutContent"));
    _sliderBet     = static_cast<Slider*>(Helper::seekWidgetByName(_layoutContent, "SliderBet"));
    _sliderBet->addEventListener(CC_CALLBACK_2(ContinueToPlayConfirmPopup::sliderEvent, this));
    _sliderBet->addTouchEventListener(CC_CALLBACK_2(ContinueToPlayConfirmPopup::onSliderTouch, this));
    _sliderBet->setMaxPercent(100);
    _buttonContinue = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonContinue"));
    _buttonContinue->addClickEventListener(CC_CALLBACK_1(ContinueToPlayConfirmPopup::onButtonClick, this));

    _textContinueButton = static_cast<Text*>(this->_buttonContinue->getChildByName("Label"));
    _textContinueButton->setString(GET_TEXT_BY_ID(TXT_CONTINUE_TO_PLAY_POPUP_CONTINUE));

    _buttonQuit = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonQuit"));
    //_buttonQuit->setPressedActionEnabled(true);
    _buttonQuit->addClickEventListener(CC_CALLBACK_1(ContinueToPlayConfirmPopup::onButtonClick, this));

    _textQuitButton = static_cast<Text*>(this->_buttonQuit->getChildByName("Label"));
    _textQuitButton->setString(GET_TEXT_BY_ID(TXT_QUIT_POPUP_QUIT));

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(ContinueToPlayConfirmPopup::onButtonClick, this));

    _progressBall = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutContent, "ImageInnerBar"));
    _progressBall->setLocalZOrder(2);


    _layoutBetPoint = static_cast<Layout*>(Helper::seekWidgetByName((Layout*) _sliderBet->getParent(), "PanelBetPoint"));
    _layoutBetPoint->retain();
    _layoutBetPoint->removeFromParent();

    _sliderOriginalSize = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelSliderBet"))->getContentSize();

    _sliderTargetScore                = static_cast<Slider*>(Helper::seekWidgetByName(_layoutRoot, "SliderTargetScore"));
    _imageTargetScoreSliderController = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutRoot, "ImageTargetScoreSliderController"));
    _layoutScore                      = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelScore"));
    _layoutScore                      = static_cast<Layout*>(Helper::seekWidgetByName((Layout*) _sliderTargetScore->getParent(), "PanelScore"));
    _layoutScore->retain();
    _layoutScore->removeFromParent();

    _sliderTargetScore->addEventListener(CC_CALLBACK_2(ContinueToPlayConfirmPopup::sliderTargetScoreEvent, this));
    _sliderTargetScore->addTouchEventListener(CC_CALLBACK_2(ContinueToPlayConfirmPopup::onSliderTargetScoreTouch, this));
    _sliderTargetScore->setMaxPercent(100);

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
}

void ContinueToPlayConfirmPopup::initValue(CityType city, bool force, int bet, int targetScore)
{
    _cityType    = city;
    _bet         = bet;
    _targetScore = targetScore;

    _buttonContinue->setEnabled(true);
    _buttonQuit->setEnabled(true);
    _buttonClose->setVisible(true);

    if (force)
    {
        _buttonClose->setVisible(false);
        _applyBackKey       = false;
        _applyTouchOutClose = false;
    }
    updateBetRange();
    updateTargetScoreRange();
}

std::vector<double> ContinueToPlayConfirmPopup::getBetRange(double money)
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

void ContinueToPlayConfirmPopup::selectBetRange(int index)
{
    if (index >= 0 && index < _betRange.size())
    {
        float percent = (index * 100.0f) / (_betRange.size() - 1);
        _sliderBet->setPercent(percent);
        _bet = _betRange[index];
        updateUIAcordingPercent(_sliderBet);
        updateBetMoney(index);
    }
}

void ContinueToPlayConfirmPopup::updateBetMoney(int index)
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

int ContinueToPlayConfirmPopup::getBetRangeFromPercent(float percent)
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

        float dist = worldPos1.distance(worldPos2);
        if (dist <= min)
        {
            min   = dist;
            index = i;
        }
    }
    return index;
}

void ContinueToPlayConfirmPopup::changeLanguage(Language)
{
    _textContinueButton->setString(GET_TEXT_BY_ID(TXT_CONTINUE_TO_PLAY_POPUP_CONTINUE));
    _textQuitButton->setString(GET_TEXT_BY_ID(TXT_QUIT_POPUP_QUIT));

}

void ContinueToPlayConfirmPopup::onButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    if (button == _buttonContinue)
    {
        _buttonContinue->setEnabled(false);
        if (_continueCallback != nullptr)
            _continueCallback(_bet, _targetScore);
        UserData::getInstance()->setBet(_bet);
        UserData::getInstance()->setTargetScore(_targetScore);
        this->close();
    }
    else if (button == _buttonQuit)
    {
        _buttonQuit->setEnabled(false);
        if (_quitCallback != nullptr)
            _quitCallback();
        this->close();
    }
    else if (sender == _buttonClose)
    {
        this->close();
    }
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void ContinueToPlayConfirmPopup::sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event)
{

    Slider* slider = (Slider*) sender;
    if (event == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        updateBetMoney(getBetRangeFromPercent(slider->getPercent()));
        //selectBetRange(getBetRangeFromPercent(slider->getPercent()));
    }
    else if (event == Slider::EventType::ON_SLIDEBALL_UP || event == Slider::EventType::ON_SLIDEBALL_DOWN || event == Slider::EventType::ON_SLIDEBALL_CANCEL)
    {
        selectBetRange(getBetRangeFromPercent(slider->getPercent()));
    }
    float width = slider->getContentSize().width - MARGIN_BET;
    _progressBall->setPositionX((slider->getPositionX() - width / 2) + width * ((float) slider->getPercent() * 1.0f / _sliderBet->getMaxPercent()));
}

void ContinueToPlayConfirmPopup::onSliderTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
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

void ContinueToPlayConfirmPopup::updateUIAcordingPercent(cocos2d::ui::Slider* slider)
{
    float width = slider->getContentSize().width - MARGIN_BET;
    _progressBall->setPositionX((slider->getPositionX() - width / 2) + (width) * ((float) slider->getPercent() / _sliderBet->getMaxPercent()));
}

void ContinueToPlayConfirmPopup::setQuitCallback(const QuitCallback& quitCallback)
{
    _quitCallback = quitCallback;
}

void ContinueToPlayConfirmPopup::setContinueCallback(const ContinueCallback& continueCallback)
{
    _continueCallback = continueCallback;
}


int ContinueToPlayConfirmPopup::getBetRangeIndex(double betMoney)
{
    for (int i = 0; i < _betRange.size(); i++)
        if (_betRange[i] == betMoney)
            return i;
    return -1;
}

void ContinueToPlayConfirmPopup::updateBetRange()
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

void ContinueToPlayConfirmPopup::updateBetRangeSoFar()
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

std::pair<double, std::vector<double>> ContinueToPlayConfirmPopup::getBetRangeSoFar(double money)
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

cocos2d::FiniteTimeAction* ContinueToPlayConfirmPopup::getZoomInActionBetPoint()
{
    auto seq = Sequence::create(ScaleTo::create(0.1f, 1.1f), ScaleTo::create(0.1f, 1.0f), DelayTime::create(0.2f), nullptr);
    seq->setTag(11);
    return seq;
}

cocos2d::FiniteTimeAction* ContinueToPlayConfirmPopup::getActionTargetScore(float scale, float opacity, float duration, bool isScaleUp)
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

void ContinueToPlayConfirmPopup::playTargetScoreAnim(cocos2d::ui::Layout* layout, float scale, float opacity)
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

void ContinueToPlayConfirmPopup::updateTargetScoreUIAcordingPercent(cocos2d::ui::Slider* slider)
{
    float width = slider->getContentSize().width - MARGIN_TARGET_SCORE;
    _imageTargetScoreSliderController->setPositionX((slider->getPositionX() - width / 2) + (width) * ((float) slider->getPercent() / _sliderTargetScore->getMaxPercent()));
}

std::vector<int> ContinueToPlayConfirmPopup::getTargetScoreRange()
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

void ContinueToPlayConfirmPopup::selectTargetScoreRange(int index)
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

void ContinueToPlayConfirmPopup::updateTargetScore(int index)
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

int ContinueToPlayConfirmPopup::getTargetScoreRangeFromPercent(float percent)
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

        float dist = worldPos1.distance(worldPos2);
        if (dist <= min)
        {
            min   = dist;
            index = i;
        }
    }
    return index;
}

int ContinueToPlayConfirmPopup::getTargetScoreRangeIndex(int score)
{
    for (int i = 0; i < _targetScoreRange.size(); i++)
        if (_targetScoreRange[i] == score)
            return i;
    return -1;
}

void ContinueToPlayConfirmPopup::updateTargetScoreRange()
{
    for (int i = 0; i < _listLayoutTargetScore.size(); i++)
        _listLayoutTargetScore.at(i)->removeFromParent();
    _listLayoutTargetScore.clear();
    _listLayoutTargetScorePos.clear();
    _targetScoreRange = getTargetScoreRange();
    for (int i = 0; i < _targetScoreRange.size(); i++)
    {
        Layout* targetScore = (Layout*) _layoutScore->clone();
        targetScore->setLocalZOrder(1);
        targetScore->setSwallowTouches(false);

        Layout* layoutTargetScore = (Layout*)targetScore->getChildByName("PanelTargetScore");
        TextBMFont* label = (TextBMFont*) layoutTargetScore->getChildByName("Label");
        label->setString(fmt::format(GET_TEXT_BY_ID(TXT_TARGET_SCORE_TEMPLATE), GameUtils::MoneyFormatShortCut(_targetScoreRange[i], "")));

        TextBMFont* labelTitle = (TextBMFont*) layoutTargetScore->getChildByName("LabelTitle");
        labelTitle->setString(GET_TEXT_BY_ID(TXT_TARGET_SCORE));

        Layout* layoutExtra = (Layout*) targetScore->getChildByName("PanelExtra");
        Layout* layoutJP = (Layout*) layoutExtra->getChildByName("PanelJackpot");
        Layout* layoutMulti = (Layout*) layoutExtra->getChildByName("PanelMultiply");
        TextBMFont* textMultiply = (TextBMFont*) layoutMulti->getChildByName("TextMultiply");
        textMultiply->setString(fmt::format("X{0}", (int)this->getMaxMultiplyValue(_targetScoreRange[i])));

        float posY = _sliderTargetScore->getScaleY() * _sliderTargetScore->getContentSize().height / 2;
        float posX = (1.0f * i * _sliderTargetScore->getScaleX() * (_sliderTargetScore->getContentSize().width - MARGIN_BET)) / (_targetScoreRange.size() - 1) + MARGIN_BET / 2;

        targetScore->setPosition(_sliderBet->getParent()->convertToNodeSpace(_sliderBet->convertToWorldSpace(Point(posX, posY))));
        _sliderTargetScore->getParent()->addChild(targetScore);

        targetScore->setTag(i);
        targetScore->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
            Node* node = (Node*) sender;
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                _draggingTargetScoreSlider = true;
                selectTargetScoreRange(node->getTag());
                _imageTargetScoreSliderController->stopAllActions();
                _imageTargetScoreSliderController->runAction(Sequence::create(ScaleTo::create(0.1f, 1.0f), ScaleTo::create(0.1f, 0.8f), nullptr));
                if (node->getActionByTag(11) == nullptr)
                {
                    node->setScale(1.0f);
                    node->runAction(getZoomInActionBetPoint());
                }
            }
        });
        _listLayoutTargetScore.pushBack(targetScore);
        _listLayoutTargetScorePos.push_back(targetScore->getParent()->convertToWorldSpace(targetScore->getPosition()));
    }

    int index = getTargetScoreRangeIndex(UserData::getInstance()->getTargetScore());
    if (index != -1)
    {
        selectTargetScoreRange(index);
    }
    else
    {
        if (_betRange.size() >= 2 && _betRange.size() <= 5)
            selectTargetScoreRange(1);
        else
            selectTargetScoreRange(2);
    }
}

void ContinueToPlayConfirmPopup::sliderTargetScoreEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event)
{

    Slider* slider = (Slider*) sender;
    if (event == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        updateTargetScore(getTargetScoreRangeFromPercent(slider->getPercent()));
        //selectBetRange(getBetRangeFromPercent(slider->getPercent()));
    }
    else if (event == Slider::EventType::ON_SLIDEBALL_UP || event == Slider::EventType::ON_SLIDEBALL_DOWN || event == Slider::EventType::ON_SLIDEBALL_CANCEL)
    {
        selectTargetScoreRange(getTargetScoreRangeFromPercent(slider->getPercent()));
    }
    float width = slider->getContentSize().width - MARGIN_BET;
    _imageTargetScoreSliderController->setPositionX((slider->getPositionX() - width / 2) + width * ((float) slider->getPercent() * 1.0f / _sliderTargetScore->getMaxPercent()));
}

void ContinueToPlayConfirmPopup::onSliderTargetScoreTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
        {
            _draggingTargetScoreSlider = true;
            _imageTargetScoreSliderController->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 1.0f);
            _imageTargetScoreSliderController->runAction(scaleTo);
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
        {
            _draggingTargetScoreSlider = false;
            _imageTargetScoreSliderController->stopAllActions();
            ScaleTo* scaleTo = ScaleTo::create(0.1f, 0.8f);
            _imageTargetScoreSliderController->runAction(scaleTo);
            break;
        }
    }
}

float ContinueToPlayConfirmPopup::getJackpotBetMultiply()
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

float ContinueToPlayConfirmPopup::getMaxMultiplyValue(int targetScore)
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

#include "GPGinUserSlotNode.h"
#include "Helper/GameUtils.h"
#include "GPGinInterfaceLayer.h"
#include "Manager/SoundManager.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"

USING_NS_CC;
using namespace ui;
using namespace std;

#define TAG_ACTION_USER_THINKING 100
#define TAG_ACTION_ZOOM_IN_OUT 102
#define TAG_ACTION_FADE_IN_OUT 103

#define CHANGE_MONEY_ANIMATION_TIME 1.0f

#define TL_USERINFO_ONTURN cocos2d::Color3B(255,255,255)
#define TL_USERINFO_NOTONTURN cocos2d::Color3B(255,255,255)

GPGinUserSlotNode::GPGinUserSlotNode()
{
    this->_textUsername                = nullptr;
    this->_textMoney                   = nullptr;
    this->_layoutRoot                  = nullptr;
    this->_layoutAvatar                = nullptr;
    this->_layoutInfo                  = nullptr;
    this->_imageviewBGOnTable          = nullptr;
    this->_imageviewBGOnTableAnim      = nullptr;
    this->_layoutNotify                = nullptr;
    this->_scoreBoxNode                = nullptr;
    this->_active                      = false;
    this->_currentMoney                = 0;
    this->_currentMoneyForAnim         = 0;
    this->_slotInfo                    = nullptr;
    this->_layoutDot                   = nullptr;
    this->_countDot                    = 0;
    this->_currentLevel                = 0;
    this->_currentLevelExp             = 0;
    this->_currentLevelExpForAnimation = 0;
    this->_currentLevelTotalExp        = 0;
}

GPGinUserSlotNode::~GPGinUserSlotNode()
{
    CC_SAFE_RELEASE(_scoreBoxNode);
    CC_SAFE_RELEASE(_deadwoodBoxNode);
}

bool GPGinUserSlotNode::init()
{
    if (!Node::init())
        return false;
    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);
    return true;
}

void GPGinUserSlotNode::initLayout(int idx,
                                   ScoreBoxNode* scoreBoxNode,
                                   DeadwoodBoxNode* deadwoodBoxNode,
                                   cocos2d::ui::Layout* imageviewBGTable,
                                   cocos2d::ui::Layout* imageviewBGTableAnim,
                                   cocos2d::ui::Layout* layoutNotify)
{
    Node* node = nullptr;
    if (idx == MAIN_USER_SLOT_INDEX)
        node = CSLoader::createNode("ui/Common/MainUserInfoLayout.csb");
    else
        node = CSLoader::createNode("ui/Common/UserInfoLayout.csb");

    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setPosition(Point(0, 0));
#if USE_CHEAT_CARD == 1
    if (idx == MAIN_USER_SLOT_INDEX)
    {
        this->_layoutRoot->setTouchEnabled(false);
    }
    else
    {
        this->_layoutRoot->setTouchEnabled(true);
        this->_layoutRoot->addClickEventListener(CC_CALLBACK_1(GPGinUserSlotNode::onAvatarClick, this));
    }
#else
    this->_layoutRoot->setTouchEnabled(false);
#endif
    _imageTurnBorder = static_cast<ImageView*>(Helper::seekWidgetByName(this->_layoutRoot, "BorderTurn"));
    _imageTurnBorder->setVisible(false);

    this->_layoutAvatar = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "LayoutAvatar"));
    this->_layoutAvatar->setTouchEnabled(false);
    this->_layoutInfo = static_cast<Layout*>(this->_layoutRoot->getChildByName("LayoutInfo"));
    this->_layoutInfo->setTouchEnabled(false);

    this->_textUsername           = static_cast<Text*>(this->_layoutInfo->getChildByName("TextUsername"));
    this->_textMoney              = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutInfo, "TextMoney"));
    this->_scaleOriginalTextMoney = this->_textMoney->getScale();
    this->_textUserLevel          = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutRoot, "TextLevel"));
    this->_sliderUserLevel        = static_cast<Slider*>(Helper::seekWidgetByName(this->_layoutRoot, "LevelSlider"));
    this->_avatar                 = Avatar::create();
    this->_avatar->setPosition(this->_layoutAvatar->getContentSize() / 2);
    this->_layoutAvatar->addChild(this->_avatar, -2);

    this->_layoutDot = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "LayoutDot"));
    if (this->_layoutDot != nullptr)
        this->_layoutDot->setVisible(false);
    addChild(this->_layoutRoot);
    Helper::doLayout(this->_layoutRoot);

    // Jackpot icon
    _jackpotIconNode = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "Rank1"));
    _jackpotIconNode->setVisible(false);

    _scoreBoxNode       = scoreBoxNode;
    _deadwoodBoxNode    = deadwoodBoxNode;
    _imageviewBGOnTable = imageviewBGTable;
//    _imageviewBGOnTable->setVisible(true);
    _imageviewBGOnTable->setVisible(false);
    _imageviewBGOnTableAnim = imageviewBGTableAnim;
    _imageviewBGOnTableAnim->setVisible(false);
    _layoutNotify = layoutNotify;
    _layoutNotify->setVisible(false);
    _scaleOriginal = this->_layoutRoot->getScale();
}

void GPGinUserSlotNode::setUsername(const std::string& username)
{
    this->_textUsername->setString(username);
}

void GPGinUserSlotNode::setMoney(double money)
{
    _currentMoney = money;
    this->_textMoney->setString(GameUtils::MoneyFormat(money, "$"));
}

void GPGinUserSlotNode::setMoney(double money, bool animation)
{
    this->_currentMoneyForAnim = _currentMoney;
    float step = (1 / 30.0f * (money - _currentMoney)) / CHANGE_MONEY_ANIMATION_TIME;
    unschedule("changeMoney");
    if (step != 0)
    {
        schedule([=](float dt) {
            if (((money - _currentMoney) <= 0 && this->_currentMoneyForAnim <= money) || ((money - _currentMoney) >= 0 && this->_currentMoneyForAnim >= money))
            {
                unschedule("changeMoney");
                this->setMoney(money);
                return;
            }
            this->_currentMoneyForAnim += step;
            if ((_currentMoney <= money && _currentMoneyForAnim <= money) || (_currentMoney >= money && _currentMoneyForAnim >= money))
                this->_textMoney->setString(GameUtils::MoneyFormat(this->_currentMoneyForAnim, "$"));
        }, 1 / 30.0f, "changeMoney");
    }
    this->_textMoney->stopAllActions();
    this->_textMoney->setScale(this->_scaleOriginalTextMoney);
    ScaleBy* scaleBy = ScaleBy::create(CHANGE_MONEY_ANIMATION_TIME / 2, 1.1f);
    this->_textMoney->runAction(Sequence::create(EaseElasticOut::create(scaleBy), scaleBy->reverse(), nullptr));
}

void GPGinUserSlotNode::setAvatarLink(const std::string& link)
{
    this->_avatar->setFrameUrl(link, this->_layoutAvatar->getContentSize());
}

cocos2d::ui::Text* GPGinUserSlotNode::getTextUserName()
{
    return _textUsername;
}

cocos2d::ui::Layout* GPGinUserSlotNode::getLayoutRoot()
{
    return _layoutRoot;
}

void GPGinUserSlotNode::onAvatarClick(cocos2d::Ref* pSender)
{
#if USE_CHEAT_CARD == 1
    if (_gpGinInterfaceLayer)
        _gpGinInterfaceLayer->onAvatarClick(this);
#endif
}

void GPGinUserSlotNode::setLevel(int level)
{
    if (_textUserLevel != nullptr)
        _textUserLevel->setString(StringUtils::format("Lv. %d", level));
}

void GPGinUserSlotNode::setLevelExp(int newLevel, int currentExp, int totalExp, bool animation)
{
    if (_sliderUserLevel != nullptr)
    {
        if (animation == false)
        {
            _sliderUserLevel->setMaxPercent(1000);
            _currentLevelExp             = currentExp;
            _currentLevelExpForAnimation = currentExp * _sliderUserLevel->getMaxPercent() * 1.0f / totalExp;
            _currentLevel                = newLevel;
            _currentLevelTotalExp        = totalExp;
            _sliderUserLevel->setPercent(_currentLevelExpForAnimation);
            setLevel(_currentLevel);
        }
        else
        {
            _sliderUserLevel->setMaxPercent(1000);
            _currentLevelExpForAnimation = _currentLevelExp * _sliderUserLevel->getMaxPercent() * 1.0f / _currentLevelTotalExp;
            float step = 7;// (1 / 60.0f * (currentExp - _currentLevelExp)) / CHANGE_LEVEL_EXP_ANIMATION_TIME;
            unschedule("changeLevelExp");
            if (step != 0)
            {
                schedule([=](float dt) {
                    if (newLevel != _currentLevel && _currentLevelExpForAnimation >= _sliderUserLevel->getMaxPercent())
                    {
                        _currentLevel++;
                        _currentLevelExpForAnimation = 0;
                        _currentLevelTotalExp        = totalExp;
                        _currentLevelExp             = currentExp;

                        setLevel(_currentLevel);
                        _textUserLevel->stopAllActions();
                        auto ScaleBy = ScaleBy::create(0.2f, 1.3f);
                        _textUserLevel->runAction(Sequence::create(ScaleBy, ScaleBy->reverse(), nullptr));
                    }
                    if (newLevel == _currentLevel && _currentLevelExpForAnimation + step > currentExp * _sliderUserLevel->getMaxPercent() * 1.0f / _currentLevelTotalExp)
                    {
                        unschedule("changeLevelExp");
                        this->setLevelExp(newLevel, currentExp, totalExp);
                        return;
                    }
                    this->_currentLevelExpForAnimation += step;
                    _sliderUserLevel->setPercent(_currentLevelExpForAnimation);
                }, 1 / 60.0f, "changeLevelExp");
            }
        }
    }
}

void GPGinUserSlotNode::setPlaying(bool thinking)
{
    if (this->_layoutDot != nullptr)
    {
        if (thinking)
        {
            this->_countDot = 0;
            if (this->_layoutDot->getActionByTag(TAG_ACTION_USER_THINKING) == nullptr)
            {
                this->_layoutDot->getChildByName("ImageDone")->setVisible(false);
                this->_layoutDot->setVisible(true);
                stopActionByTag(TAG_ACTION_USER_THINKING);
                ImageView* dot1 = static_cast<ImageView*>(this->_layoutDot->getChildByName("ImageDot1"));
                ImageView* dot2 = static_cast<ImageView*>(this->_layoutDot->getChildByName("ImageDot2"));
                ImageView* dot3 = static_cast<ImageView*>(this->_layoutDot->getChildByName("ImageDot3"));
                dot1->setVisible(false);
                dot2->setVisible(false);
                dot3->setVisible(false);
                CallFunc     * showDot = CallFunc::create([=] {
                    this->_countDot++;

                    if (_countDot > 3)
                    {
                        _countDot = 0;
                        dot1->setVisible(false);
                        dot2->setVisible(false);
                        dot3->setVisible(false);
                    }
                    else
                    {
                        ImageView* dot = nullptr;
                        switch (_countDot)
                        {
                            case 1:
                                dot = dot1;
                                break;
                            case 2:
                                dot = dot2;
                                break;
                            case 3:
                                dot = dot3;
                                break;
                            default:
                                break;
                        }
                        if (dot != nullptr)
                            dot->setVisible(true);
                    }
                });
                RepeatForever* repeat  = RepeatForever::create(Sequence::create(DelayTime::create(0.3f), showDot, nullptr));
                repeat->setTag(TAG_ACTION_USER_THINKING);
                runAction(repeat);
            }
        }
        else
        {
            stopActionByTag(TAG_ACTION_USER_THINKING);
            this->_layoutDot->getChildByName("ImageDone")->setVisible(false);
            ImageView* dot1 = static_cast<ImageView*>(this->_layoutDot->getChildByName("ImageDot1"));
            ImageView* dot2 = static_cast<ImageView*>(this->_layoutDot->getChildByName("ImageDot2"));
            ImageView* dot3 = static_cast<ImageView*>(this->_layoutDot->getChildByName("ImageDot3"));
            dot1->setVisible(false);
            dot2->setVisible(false);
            dot3->setVisible(false);
            this->_layoutDot->setVisible(false);
        }
    }
}

void GPGinUserSlotNode::setSlotIdx(int slotIdx)
{
    this->_slotIdx = slotIdx;
}

int GPGinUserSlotNode::getSlotIdx()
{
    return this->_slotIdx;
}

void GPGinUserSlotNode::setSlotInfo(GinSlotInfo* slotInfo)
{
    this->_slotInfo = slotInfo;
    this->_slotInfo->setNeedUpdate(true);
}

GinSlotInfo* GPGinUserSlotNode::getSlotInfo()
{
    return this->_slotInfo;
}

void GPGinUserSlotNode::setActive(bool active)
{
    this->_active = active;
    setVisible(active);
}

bool GPGinUserSlotNode::isActive()
{
    return this->_active;
}

void GPGinUserSlotNode::updateInfo()
{
    setSlotIdx(this->_slotInfo->getSlotIdx());
    setUsername(this->_slotInfo->getUserName());
    setAvatarLink(this->_slotInfo->getAvatarLink());
    setMoney(this->_slotInfo->getMoney());
    setLevelExp(this->_slotInfo->getLevel(), this->_slotInfo->getLevelCurrentExp(), this->_slotInfo->getLevelTotalExp());
    this->_slotInfo->setNeedUpdate(false);
}

void GPGinUserSlotNode::updateMoney()
{
    if (this->_slotInfo != nullptr && this->_slotInfo->needUpdate() == true)
        setMoney(this->_slotInfo->getMoney(), true);
}

void GPGinUserSlotNode::updateLevel()
{
    if (this->_slotInfo != nullptr)
        setLevelExp(this->_slotInfo->getLevel(), this->_slotInfo->getLevelCurrentExp(), this->_slotInfo->getLevelTotalExp(), true);
}

void GPGinUserSlotNode::setOriginalPosition(const cocos2d::Point& pos)
{
    this->_originalPosition = pos;
}

Point GPGinUserSlotNode::getOriginalPosition()
{
    return this->_originalPosition;
}

void GPGinUserSlotNode::reset()
{
    unschedule("changeMoney");
    stopAllActions();
    setActive(false);
    _textUsername->setVisible(true);
//    _imageviewBGOnTable->setVisible(true);
    _imageviewBGOnTable->setVisible(false);
    _imageviewBGOnTableAnim->setVisible(false);
    _layoutNotify->setVisible(false);
}

void GPGinUserSlotNode::setOnTurn(bool onTurn, bool anim /*= true*/)
{
//    _imageviewBGOnTable->setVisible(true);
    _imageviewBGOnTable->setVisible(false);
    _imageTurnBorder->setVisible(onTurn);
//    _imageTurnBorder->setVisible(false);
    if (onTurn == true)
    {
        setColor(TL_USERINFO_ONTURN);
        if (anim)
        {
//            _imageviewBGOnTableAnim->setVisible(true);
//            _imageviewBGOnTableAnim->setOpacity(0);
//            _imageviewBGOnTableAnim->stopActionByTag(TAG_ACTION_FADE_IN_OUT);
//            auto action = FadeIn::create(0.2f);
//            action->setTag(TAG_ACTION_FADE_IN_OUT);
//            _imageviewBGOnTableAnim->runAction(action);
            auto actSequence = Sequence::create(DelayTime::create(0),
                                                EaseSineOut::create(ScaleTo::create(0.2f, _scaleOriginal + 0.2f)),
                                                EaseSineOut::create(ScaleTo::create(0.2f, _scaleOriginal)),
                                                nullptr);
            actSequence->setTag(TAG_ACTION_ZOOM_IN_OUT);
            _layoutRoot->stopActionByTag(TAG_ACTION_ZOOM_IN_OUT);
            _layoutRoot->runAction(actSequence);
        }
    }
    else
    {
        setColor(TL_USERINFO_NOTONTURN);
        _imageviewBGOnTableAnim->stopActionByTag(TAG_ACTION_FADE_IN_OUT);
        if (_imageviewBGOnTableAnim->isVisible())
        {
            auto action = Sequence::create(FadeOut::create(0.1f), Hide::create(), nullptr);
            action->setTag(TAG_ACTION_FADE_IN_OUT);
            _imageviewBGOnTableAnim->runAction(action);
        }
    }
}

Vec2 GPGinUserSlotNode::getPositionTextMoneyOnScreen()
{
    return this->_textMoney->getParent()->convertToWorldSpace(this->_textMoney->getPosition());
}

Vec2 GPGinUserSlotNode::getPositionJackpotIconOnScreen()
{
    return this->_jackpotIconNode->getParent()->convertToWorldSpace(this->_jackpotIconNode->getPosition());
}

ScoreBoxNode* GPGinUserSlotNode::getScoreBoxNode()
{
    return _scoreBoxNode;
}

DeadwoodBoxNode* GPGinUserSlotNode::getDeadwoodBoxNode()
{
    return _deadwoodBoxNode;
}

cocos2d::Size GPGinUserSlotNode::getLayoutCardOnTableSize()
{
    return _imageviewBGOnTable->getContentSize();
}

void GPGinUserSlotNode::showJackpotIcon(bool visible, bool anim)
{
    if (anim)
    {
        if (visible)
        {
            _jackpotIconNode->setScale(0);
            _jackpotIconNode->setOpacity(0);
            _jackpotIconNode->setVisible(true);
            _jackpotIconNode->runAction(Spawn::create(FadeIn::create(0.1f), EaseBackOut::create(ScaleTo::create(0.2f, 1.0f)), nullptr));
        }
        else
        {
            _jackpotIconNode->runAction(Sequence::create(Spawn::create(FadeOut::create(0.2f), EaseBackIn::create(ScaleTo::create(0.1f, 0.0f)), nullptr),
                                                         DelayTime::create(0.2f),
                                                         Hide::create(),
                                                         nullptr));
        }
    }
    else
    {
        _jackpotIconNode->setVisible(visible);
    }
}

void GPGinUserSlotNode::showNotify(bool visible, const std::string& text)
{
    ((Text*) _layoutNotify->getChildByName("TextNotify"))->setString(text);
    _layoutNotify->setVisible(visible);
}

void GPGinUserSlotNode::showTextDropArea(bool visible)
{
    _imageviewBGOnTable->getChildByName("Text")->setVisible(visible);
}

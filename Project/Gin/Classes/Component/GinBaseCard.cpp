#include "GinBaseCard.h"
#include "Helper/JsonHelper.h"
#include "Helper/GameUtils.h"

#define TAG_ACTION_FLIP_CARD 100
#define FLIP_CARD_DELAY_TIME 0.2f

USING_NS_CC;
using namespace ui;
using namespace std;

#define CARD_DIM_COLOR Color3B(170,170,170)

#define ZORDER_CARD_BACK 0
#define ZORDER_CARD_FRONT 0
#define ZORDER_CARD_HIGHLIGHT 1
#define ZORDER_CARD_FRONT_HIGHT_LIGHT 0

GinBaseCard::~GinBaseCard()
{
    if (_frontSprite)
    {
        _frontSprite->removeFromParent();
        CC_SAFE_RELEASE_NULL(_frontSprite);
    }

    if (_backSprite)
    {
        _backSprite->removeFromParent();
        CC_SAFE_RELEASE_NULL(_backSprite);
    }
    _eventDispatcher->removeEventListenersForTarget(this);

    if (_meldCircleSprite)
    {
        _meldCircleSprite->removeFromParent();
        CC_SAFE_RELEASE_NULL(_meldCircleSprite);
    }
}

GinBaseCard::GinBaseCard()
{
    _index    = -1;
    _value    = -1;
    _isOpen   = false;
    _isMoving = false;
    this->_isFlipping = false;
    _isHighLightGive = false;
    _isDimColor      = false;

    _frontSprite         = nullptr;
    _backSprite          = nullptr;
    _highLight           = nullptr;
    _highLightTake       = nullptr;
    _effectHighLightPhom = nullptr;
    _backBlackSprite     = nullptr;
    _bonusCard           = nullptr;
    _touchEnable         = false;
    _touchListener       = nullptr;
    _meldCircleSprite    = nullptr;
}

bool GinBaseCard::init()
{
    if (!Node::init())
        return false;
    this->setCascadeOpacityEnabled(true);
    this->setCascadeColorEnabled(true);
    return true;
}

void GinBaseCard::setCardIndex(int index, bool isSmall/*= false*/)
{
    _index   = index;
    _isSmall = isSmall;
    this->setFront(_index);
}

int GinBaseCard::getCardIndex()
{
    return _index;
}

void GinBaseCard::setCardValue(int value)
{
    _value = value;
}

int GinBaseCard::getCardValue()
{
    return _value;
}

void GinBaseCard::setFront(cocos2d::Sprite* frontSprite)
{
    if (_frontSprite != nullptr)
    {
        _frontSprite->removeFromParent();
        _frontSprite->release();
        _frontSprite = nullptr;
        _bonusCard   = nullptr;
    }
    _frontSprite = frontSprite;
    _frontSprite->retain();
    this->addChild(_frontSprite, ZORDER_CARD_FRONT);

}

void GinBaseCard::setFront(int index)
{
    if (index >= 0)
    {
        if (_isSmall)
            this->setFront(GinCardStore::getInstance()->getCardFront1(index));
        else
            this->setFront(GinCardStore::getInstance()->getCardFront(index));
    }
}

void GinBaseCard::setFrontBackground(const std::string& spriteFrameName)
{
    SpriteFrame* spriteFrame = GinCardStore::getInstance()->getFrame(spriteFrameName);
    if (spriteFrame != nullptr && _frontSprite != nullptr)
        _frontSprite->setSpriteFrame(spriteFrame);
}

void GinBaseCard::setBack(cocos2d::Sprite* backSprite)
{
    if (_backSprite != nullptr)
    {
        _backSprite->removeFromParent();
        _backSprite->release();
        _backSprite = nullptr;
    }
    _backSprite = backSprite;
    _backSprite->retain();
    this->addChild(_backSprite, ZORDER_CARD_BACK);
}

void GinBaseCard::setBack(int backId)
{
    setBack(Sprite::createWithSpriteFrameName(GinCardStore::getCardBackSpriteNameFromID(backId)));
}

void GinBaseCard::setHighLight(bool highLight)
{
    if (!_highLight)
        this->initHighLight();
    this->setFrontBackground(getFrontBackground(highLight));
    _highLight->setVisible(highLight);
}

bool GinBaseCard::getHighLight()
{
    if (!_highLight)
        this->initHighLight();
    return _highLight->isVisible();
}

void GinBaseCard::initHighLight()
{
    _highLight = Sprite::createWithSpriteFrameName("ui/ImageGPGin/highlight_card_taked.png");
    //_highLight = Sprite::createWithSpriteFrameName(GIN_CARD_FRONT_SPECIAL_SPRITEFRAME_NAME);
    //_highLight->setAnchorPoint(Vec2(0, 0));
    _highLight->setVisible(false);
    _highLight->setOpacity(0);
    //this->_frontSprite->addChild(_highLight, ZORDER_CARD_FRONT_HIGHT_LIGHT);
    this->addChild(_highLight, ZORDER_CARD_HIGHLIGHT);
}

void GinBaseCard::setHighLightGive(bool highLightGive)
{
    _isHighLightGive = highLightGive;
    if (_isHighLightGive)
        this->setFrontBackground(GIN_CARD_FRONT_SPECIAL_2_SMALL_SPRITEFRAME_NAME);
    else
        this->setHighLight(this->getHighLight());
}

bool GinBaseCard::getHighLightGive()
{
    return _isHighLightGive;
}

void GinBaseCard::setHide(bool hide)
{
    if (!_backBlackSprite)
        this->initHide();
    _backBlackSprite->setVisible(hide);
}

bool GinBaseCard::getHide()
{
    if (!_backBlackSprite)
        this->initHide();
    return _backBlackSprite->isVisible();
}

void GinBaseCard::initHide()
{
    _backBlackSprite = Sprite::createWithSpriteFrameName(GinCardStore::getCardBackSpriteNameFromID(1));
    _backBlackSprite->setPosition(_backSprite->getPosition());
    _backBlackSprite->setScale(_backSprite->getScaleX(), _backSprite->getScaleY());
    _backBlackSprite->setRotation(_backSprite->getRotation());
    _backBlackSprite->setColor(Color3B::BLACK);
    _backBlackSprite->setOpacity(130);
    _backBlackSprite->setVisible(false);
    this->addChild(_backBlackSprite, 2);
}

void GinBaseCard::setEffectHightLightPhom(bool highLight, cocos2d::Color3B color /*= cocos2d::Color3B::YELLOW*/)
{
    if (highLight)
    {
        if (!_effectHighLightPhom)
            this->initEffectHightLightPhom();
        _effectHighLightPhom->setColor(color);
    }
    else
    {
        if (_effectHighLightPhom)
        {
            _effectHighLightPhom->removeFromParent();
            _effectHighLightPhom = nullptr;
        }
    }
}

void GinBaseCard::initEffectHightLightPhom()
{
    _effectHighLightPhom = Sprite::createWithSpriteFrameName("ui/ImageGPGin/boder_00000.png");
    _effectHighLightPhom->setPosition(Vec2(0, 0));
    _effectHighLightPhom->setOpacity(100);
    this->addChild(_effectHighLightPhom, 3);
    cocos2d::Vector<SpriteFrame*> animFrames;
    for (int                      i = 0; i <= 10; i++)
        animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(cocos2d::StringUtils::format("ui/ImageGPGin/boder_000%02d.png", i)));
    _effectHighLightPhom->runAction(Sequence::create(Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 1 / 15.0f)), 2), CallFunc::create([=]() {
        _effectHighLightPhom->removeFromParent();
        _effectHighLightPhom = nullptr;
    }), nullptr));
}

void GinBaseCard::setDimColor(bool dimColor)
{
    if (_isDimColor != dimColor)
    {
        _isDimColor = dimColor;
        if (_isDimColor)
            this->setColor(CARD_DIM_COLOR);
        else
            this->setColor(Color3B::WHITE);
    }
}

bool GinBaseCard::getDimColor()
{
    return _isDimColor;
}

void GinBaseCard::open(bool anim)
{
    if (!this->_isOpen)
    {
        if (!anim)
        {
            if (this->_frontSprite != nullptr)
                this->_frontSprite->setVisible(true);
            if (this->_backSprite != nullptr)
                this->_backSprite->setVisible(false);
            this->_isFlipping = true;
        }
        else
        {
            if (!this->_isFlipping)
            {
                this->_isFlipping = true;
                //MoveBy* moveBy1 = MoveBy::create(0.1f,Vec2(10,0));
                //	ScaleTo* scaleTo1 = ScaleTo::create(FLIP_CARD_DELAY_TIME, 0, this->getScaleY());
                //MoveBy* moveBy2 = MoveBy::create(0.1f, Vec2(-10, 0));
                //ScaleTo* scaleTo2 = ScaleTo::create(FLIP_CARD_DELAY_TIME, this->getScaleX(), this->getScaleY());

                // reduce the width to zero and then expand back to initial width
                Action* scale = Sequence::create(ScaleTo::create(FLIP_CARD_DELAY_TIME / 2, 0, getScaleY()), CallFunc::create([=] {
                    if (this->_frontSprite != nullptr)
                        this->_frontSprite->setVisible(true);
                    if (this->_backSprite != nullptr)
                        this->_backSprite->setVisible(false);
                }), ScaleTo::create(FLIP_CARD_DELAY_TIME / 2, getScaleX(), getScaleY()), nullptr);
                scale->setTag(TAG_ACTION_FLIP_CARD);
                runAction(scale);
                // simulate the rotational 3d effect by playing with the skew
                // flip the skew value after the initial half of the animation for a more credible 3d effect
                float skewAmount = -6;
                Action* skew = Sequence::create(SkewBy::create(FLIP_CARD_DELAY_TIME / 2, 0, skewAmount),
                                                SkewTo::create(0, 0, -skewAmount),
                                                SkewTo::create(FLIP_CARD_DELAY_TIME / 2, 0, 0),
                                                CallFunc::create([=] {
                                                    this->_isFlipping = false;
                                                }),
                                                nullptr);
                skew->setTag(TAG_ACTION_FLIP_CARD);
                runAction(skew);
            }
        }
        this->_isOpen = true;
    }
}

void GinBaseCard::close(bool anim)
{
    if (this->_isOpen)
    {
        if (!anim)
        {
            this->_frontSprite->setVisible(false);
            this->_backSprite->setVisible(true);
            this->_isFlipping = false;
        }
        else
        {
            if (!this->_isFlipping)
                this->_isFlipping = true;
        }
        this->_isOpen = false;
    }
}

void GinBaseCard::reset()
{
    this->stopAllActions();
    _index    = -1;
    _value    = -1;
    _isOpen   = false;
    _isMoving = false;
    this->_isFlipping = false;

    if (this->_frontSprite != nullptr)
    {
        this->_frontSprite->removeFromParent();
        CC_SAFE_RELEASE_NULL(this->_frontSprite);
        _bonusCard = nullptr;
    }
    if (this->_backSprite != nullptr)
    {
        this->_backSprite->removeFromParent();
        CC_SAFE_RELEASE_NULL(this->_backSprite);
    }
    if (this->_meldCircleSprite != nullptr)
    {
        this->_meldCircleSprite->removeFromParent();
        CC_SAFE_RELEASE_NULL(this->_meldCircleSprite);
    }

    this->setColor(GIN_CARD_NORMAL_COLOR);
    this->setTouchEnable(false);
    this->setOpacity(255);
    this->setRotation(0);
}

bool GinBaseCard::containPoint(cocos2d::Point point)
{
    if (this->_frontSprite != nullptr && this->_frontSprite->isVisible() && this->_frontSprite->getBoundingBox().containsPoint(point))
        return true;
    if (this->_backSprite != nullptr && this->_backSprite->isVisible() && this->_backSprite->getBoundingBox().containsPoint(point))
        return true;
    return false;
}

void GinBaseCard::setTouchEnable(bool enable)
{
    this->_touchEnable = enable;
    if (_touchEnable)
    {
        if (this->_touchListener == nullptr)
        {
            this->_touchListener                   = EventListenerTouchOneByOne::create();
            this->_touchListener->onTouchBegan     = CC_CALLBACK_2(GinBaseCard::onTouchBegan, this);
            this->_touchListener->onTouchMoved     = CC_CALLBACK_2(GinBaseCard::onTouchMoved, this);
            this->_touchListener->onTouchEnded     = CC_CALLBACK_2(GinBaseCard::onTouchEnded, this);
            this->_touchListener->onTouchCancelled = CC_CALLBACK_2(GinBaseCard::onTouchCancelled, this);
            this->_touchListener->setSwallowTouches(true);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(this->_touchListener, this);
        }
    }
    else
    {
        _eventDispatcher->removeEventListener(_touchListener);
        this->_touchListener = nullptr;
    }
}

bool GinBaseCard::onTouchBegan(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
    return false;
}

void GinBaseCard::onTouchMoved(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
}

void GinBaseCard::onTouchEnded(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
}

void GinBaseCard::onTouchCancelled(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
}

const cocos2d::Size& GinBaseCard::getContentSize()
{
    if (this->_frontSprite != nullptr && this->_frontSprite->isVisible())
        return _frontSprite->getContentSize();
    if (this->_backSprite != nullptr && this->_backSprite->isVisible())
        return _backSprite->getContentSize();
    return Node::getContentSize();
}

int GinBaseCard::convertRank(int value)
{

    return value / MAXIMUM_NUMBER_SUIT;
}

int GinBaseCard::convertSuit(int value)
{
    return value % MAXIMUM_NUMBER_SUIT;
}

std::string GinBaseCard::getFrontBackground(bool highlight)
{
    if (!highlight)
    {
        if (!_isSmall)
            return GIN_CARD_FRONT_SPRITEFRAME_NAME;
        return GIN_CARD_FRONT_SMALL_SPRITEFRAME_NAME;
    }
    else
    {
        if (!_isSmall)
            return GIN_CARD_FRONT_SPECIAL_SPRITEFRAME_NAME;
        return GIN_CARD_FRONT_SPECIAL_SMALL_SPRITEFRAME_NAME;
    }
    return GIN_CARD_FRONT_SPRITEFRAME_NAME;
}

void GinBaseCard::setBonusCard(bool bonusCard)
{
    if (!_bonusCard)
        this->initBonusCard();
    _bonusCard->setVisible(bonusCard);
}

bool GinBaseCard::getBonusCard()
{
    if (!_bonusCard)
        this->initBonusCard();
    return _bonusCard->isVisible();
}

void GinBaseCard::initBonusCard()
{
    _bonusCard = Sprite::createWithSpriteFrameName(!_isSmall ? GIN_CARD_FRONT_BONUS_CARD_SPRITEFRAME_NAME : GIN_CARD_FRONT_BONUS_CARD_SMALL_SPRITEFRAME_NAME);
    _bonusCard->setAnchorPoint(Vec2(0, 0));
    _frontSprite->addChild(_bonusCard, 0);
}

void GinBaseCard::setMeldHighLightVisible(bool visible, MeldColorType meldColor)
{
    if (visible)
        this->setFrontBackground(GinCardStore::getMeldColorSpriteFrameName(meldColor));
    else
        this->setFrontBackground(GIN_CARD_FRONT_SPRITEFRAME_NAME);
    setMeldHighlightCircle(visible, meldColor);
}

void GinBaseCard::initMeldHighlightCircle(MeldColorType meldColor)
{
    _meldCircleSprite = Sprite::createWithSpriteFrameName(GinCardStore::getMeldColorCircleSpriteFrameName(meldColor));
    _meldCircleSprite->setAnchorPoint(Vec2(0, 0));
    _meldCircleSprite->retain();
    _frontSprite->addChild(_meldCircleSprite, 0);
}

void GinBaseCard::setMeldHighlightCircle(bool visible, MeldColorType meldColor)
{
    if (visible)
    {
        if (_meldCircleSprite == nullptr)
            initMeldHighlightCircle(meldColor);
        else
            _meldCircleSprite->setSpriteFrame(GinCardStore::getMeldColorCircleSpriteFrameName(meldColor));
    }
    else
    {
        if (this->_meldCircleSprite != nullptr)
        {
            this->_meldCircleSprite->removeFromParent();
            CC_SAFE_RELEASE_NULL(this->_meldCircleSprite);
        }
    }
}
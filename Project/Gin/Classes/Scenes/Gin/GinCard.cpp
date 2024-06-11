#include "GinCard.h"
#include "Scenes/Gin/GPGinControlCardLayer.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"
#include "Manager/SoundManager.h"
#include <cmath>
#include "ui/CocosGUI.h"
#include "Scenes/Gin/GPGinInterfaceLayer.h"

using namespace std;
USING_NS_CC;
using namespace ui;

GinCard::~GinCard()
{
}

GinCard::GinCard()
{
    _state              = GinCardState::GinCARDSTATE_ONHAND;
    _touch              = nullptr;
    _touchBeganPosition = Vec2::ZERO;
    _isSmall            = false;
}

GinCard* GinCard::create(GPGinControlCardLayer* layerControlCard)
{
    GinCard* tlCard = new GinCard();
    if (tlCard && tlCard->init())
    {
        tlCard->setLayerControlCard(layerControlCard);
        tlCard->autorelease();
        return tlCard;
    }
    CC_SAFE_RELEASE(tlCard);
    return nullptr;
}

void GinCard::openNoAnim()
{
    if (_frontSprite != nullptr)
        _frontSprite->setVisible(true);
    if (_backSprite != nullptr)
        _backSprite->setVisible(false);
}

void GinCard::openAnim()
{
    this->runAction(Sequence::create(RotateBy::create(0.06f, Vec3(0, 90, 0)), cocos2d::CallFunc::create([=]() {
        const Vec3& v = this->getRotation3D();
        this->setRotation3D(Vec3(v.x, -90, v.z));
        _frontSprite->setVisible(true);
        _backSprite->setVisible(false);
    }), RotateBy::create(0.06f, Vec3(0, 90, 0)), nullptr));
}

void GinCard::closeNoAnim()
{
    if (_frontSprite != nullptr)
        _frontSprite->setVisible(false);
    if (_backSprite != nullptr)
        _backSprite->setVisible(true);
}

void GinCard::closeAnim()
{
    this->runAction(Sequence::create(EaseCubicActionIn::create(RotateTo::create(0.06f, Vec3(0, 90, 0))), CallFuncN::create([=](Node* node) {
        node->setRotation3D(Vec3(0, 270, 0));
        _frontSprite->setVisible(false);
        _backSprite->setVisible(true);
    }), EaseCubicActionIn::create(RotateTo::create(0.06f, Vec3(0, 360, 0))), nullptr));
}

GinCardProperty GinCard::getProperty()
{
    return _property;
}

void GinCard::setProperty(const GinCardProperty& property)
{
    _property = property;
    _property._positionOnHandWasSelected = _property._positionOnHand + GameUtils::getPositionAccordingAngle(_property._angle, DISTANCE_WAS_SELECTED_CARD);
}

GinCardState GinCard::getState()
{
    return _state;
}

void GinCard::setState(GinCardState state)
{
    _state = state;
}

std::vector<GinCard*> GinCard::getListMovingCards()
{
    return _listMovingCards;
}

void GinCard::setLayerControlCard(GPGinControlCardLayer* layerControlCard)
{
    _layerControlCard = layerControlCard;
}

cocos2d::Touch* GinCard::getTouch()
{
    return _touch;
}

void GinCard::setTouch(cocos2d::Touch* touch)
{
    _touch = touch;
}

void GinCard::reset()
{
    GinBaseCard::reset();
}

void GinCard::open(bool anim)
{
    if (!_isOpen)
    {
        _isOpen = true;
        if (anim)
            this->openAnim();
        else
            this->openNoAnim();
    }
}

void GinCard::close(bool anim)
{
    if (_isOpen)
    {
        _isOpen = false;
        if (anim)
            this->closeAnim();
        else
            this->closeNoAnim();
    }
}

void GinCard::selected(bool playAnim)
{
    if (this->getState() == GinCardState::GinCARDSTATE_ONHAND)
    {
        this->setState(GinCardState::GinCARDSTATE_SELECTED);
        if (playAnim)
        {
            Spawn* actSequence = Spawn::create(MoveTo::create(TIME_ACTION_SELECTED, _property._positionOnHandWasSelected),
                                               Sequence::create(ScaleTo::create(TIME_ACTION_SELECTED, CARD_IN_HAND_MAX_SCALE),
                                                                ScaleTo::create(TIME_ACTION_SELECTED, CARD_IN_HAND_SCALE),
                                                                nullptr),
                                               nullptr);
            actSequence->setTag(TAG_ACTION_SELECTED_CARD);
            this->stopActionByTag(TAG_ACTION_UNSELECTED_CARD);
            this->stopActionByTag(TAG_ACTION_SELECTED_CARD);
            this->setScale(1.0f * CARD_IN_HAND_SCALE);
            this->runAction(actSequence);
        }
        else
        {
            this->stopActionByTag(TAG_ACTION_UNSELECTED_CARD);
            this->stopActionByTag(TAG_ACTION_SELECTED_CARD);
            this->setScale(1.0f * CARD_IN_HAND_SCALE);
            this->setPosition(_property._positionOnHandWasSelected);
        }
    }
}

void GinCard::unSelected(bool playAnim)
{
    if (this->getState() == GinCardState::GinCARDSTATE_SELECTED)
    {
        this->setState(GinCardState::GinCARDSTATE_ONHAND);
        if (playAnim)
        {
            MoveTo* actSequence = MoveTo::create(TIME_ACTION_SELECTED, _property._positionOnHand);
            actSequence->setTag(TAG_ACTION_UNSELECTED_CARD);
            this->stopActionByTag(TAG_ACTION_SELECTED_CARD);
            this->stopActionByTag(TAG_ACTION_UNSELECTED_CARD);
            this->setScale(1.0f * CARD_IN_HAND_SCALE);
            this->runAction(actSequence);
        }
        else
        {
            this->stopActionByTag(TAG_ACTION_SELECTED_CARD);
            this->stopActionByTag(TAG_ACTION_UNSELECTED_CARD);
            this->setScale(1.0f * CARD_IN_HAND_SCALE);
            this->setPosition(_property._positionOnHand);
        }
    }
}

void GinCard::startDrag(GinCard* cardIsSelected, const cocos2d::Vec2& posWorldTouch)
{
    if (this->getState() == GinCardState::GinCARDSTATE_ONHAND || this->getState() == GinCardState::GinCARDSTATE_SELECTED)
    {
        unSelected(false);
        if (this == cardIsSelected)
        {
            for (auto const& card : _listMovingCards)
            {
                if (card == cardIsSelected)
                {
                    this->setState(GinCardState::GinCARDSTATE_DRAG);
                    this->setLocalZOrder(TOTAL_NUMBER_CARD + 1);
                    this->setOpacity(OPACITY_CARD_DRAG);
                }
                else
                {
                    card->startDrag(cardIsSelected, posWorldTouch);
                }
            }
        }
        else
        {
            this->setState(GinCardState::GinCARDSTATE_DRAG);
            this->setLocalZOrder(TOTAL_NUMBER_CARD + 1);
            this->setOpacity(OPACITY_CARD_DRAG);
        }
        this->setScale(this->getDragScaleRatioOnDragging());
    }
}

void GinCard::drag(GinCard* cardIsSelected, const cocos2d::Vec2& posWorldTouch, float angleRotate /*= 0*/, const cocos2d::Vec2& positonCard /*= cocos2d::Vec2::ZERO*/)
{
    Layout* layoutCard = _layerControlCard->getLayoutCard();
    if (this->getState() == GinCardState::GinCARDSTATE_DRAG)
    {
        float boundY = this->getBoundY();
        if (this == cardIsSelected)
        {
            Vec2 position = layoutCard->convertToNodeSpace(posWorldTouch) - _touchBeganPosition;
            position.y += DRAG_CARD_POSITION_Y_OFFSET;
            if((this->_layerControlCard->canDumpCard() == false) &&
                    (this->getParent()->convertToNodeSpace(_touch->getStartLocation()).y < boundY))
            {
                position = this->_layerControlCard->correctOutOfBoundDraggingCardPosition(position);
            }

            if(position.y < boundY)
            {
                angleRotate = this->_layerControlCard->calculateDraggingCardRotation(position);
            }
            else
            {
                angleRotate = 0;
            }

            this->setPosition(position);
//            this->setRotation(0);
            this->setRotation(angleRotate);

            int      index = -1;
            for (int i     = 0; i < _listMovingCards.size(); i++)
            {
                if (_listMovingCards[i] == cardIsSelected)
                {
                    index = i;
                    break;
                }
            }
            for (int i     = 0; i < _listMovingCards.size(); i++)
            {
                if (_listMovingCards[i] == cardIsSelected)
                    continue;
                if (index != -1)
                    _listMovingCards[i]->drag(cardIsSelected, posWorldTouch, 0.0f, position + Vec2((i - index) * GPGinControlCardLayer::_distanceBetweenTwoCards, 0.0f));
            }
            if (!_isMoving)
            {
                this->checkMoveInPositionOfAnotherCard();
            }
        }
        else
        {
            if(positonCard.y < boundY)
            {
                angleRotate = this->_layerControlCard->calculateDraggingCardRotation(positonCard);
            }

            this->setRotation(angleRotate);
            this->setPosition(positonCard);
        }
        this->setScale(this->getDragScaleRatioOnDragging());
    }
}

void GinCard::unDrag(GinCard* cardIsSelected)
{
    if (this->getState() == GinCardState::GinCARDSTATE_DRAG)
    {
        if (this == cardIsSelected)
        {
            // Stop action drag
            this->setOpacity(255);
            this->actUnDragTo(this);
            for (auto const& card : _listMovingCards)
            {
                if (card == cardIsSelected)
                    continue;
                card->unDrag(cardIsSelected);
            }
        }
        else
        {
            this->setOpacity(255);
            this->actUnDragTo(this);
        }
        this->setScale(CARD_IN_HAND_SCALE);
    }
}

void GinCard::actUnDragTo(GinCard* card)
{
    this->stopActionByTag(TAG_ACTION_UNDRAG_CARD);
    float time = this->getPosition().distance(card->getProperty()._positionOnHand) * TIME_ACTION_CARD_MOVE_ON_HAND_1PIXEL;
    Sequence* sequence = Sequence::create(CallFunc::create([=]() {
        this->setLocalZOrder(card->getProperty()._zOder);
    }), Spawn::create(EaseExponentialOut::create(MoveTo::create(time, card->getProperty()._positionOnHand)), RotateTo::create(time, card->getProperty()._angle), nullptr), CallFunc::create([=]() {
        this->setState(GinCardState::GinCARDSTATE_ONHAND);
    }), nullptr);
    sequence->setTag(TAG_ACTION_UNDRAG_CARD);
    this->runAction(sequence);
}

void GinCard::checkMoveInPositionOfAnotherCard()
{
    if (_listMovingCards.size() > 1)
    {
        const std::vector<GroupCard>& listGroupCard = _layerControlCard->getListGroupCardsMainUser();
        int currentGroupIndex = _layerControlCard->checkListCardInAGroup(_listMovingCards);
        if (currentGroupIndex >= 0)
        {
            for (int i = 0; i < listGroupCard.size(); i++)
            {
                if (i != currentGroupIndex)
                {
                    GinCard* cardInGroupOnHand   = nullptr;
                    GinCard* cardInGroupDragging = nullptr;
                    if (i < currentGroupIndex)
                    {
                        cardInGroupOnHand   = listGroupCard.at(i)._listCards.front();
                        cardInGroupDragging = _listMovingCards.front();
                    }
                    else
                    {
                        cardInGroupOnHand   = listGroupCard.at(i)._listCards.back();
                        cardInGroupDragging = _listMovingCards.back();
                    }
                    if (checkTwoCardsIsOverlap(cardInGroupOnHand, cardInGroupDragging))
                    {
                        _layerControlCard->arrangeCardOnHand(TypeArrangeCardOnHand::TYPE_ARRANGE_GROUP_CARD_MULTI, i, currentGroupIndex);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        const std::vector<GinCard*>& listHandCards = _layerControlCard->getListHandCardsMainUser();
        for (GinCard               * card : listHandCards)
        {
            bool cardNotOnHand = false;
            for (GinCard* cardMoving : _listMovingCards)
            {
                if (card == cardMoving)
                {
                    cardNotOnHand = true;
                    break;
                }
            }
            if (cardNotOnHand)
                continue;
            if (checkTwoCardsIsOverlap(card, this))
            {
                _layerControlCard->arrangeCardOnHand(TypeArrangeCardOnHand::TYPE_ARRANGE_IN_START_END, _layerControlCard->getIndexCardOnHand(card), _layerControlCard->getIndexCardOnHand(this));
                break;
            }
        }
    }
}

bool GinCard::checkTwoCardsIsOverlap(GinCard* cardOnHand, GinCard* cardDrag)
{
    Vec2 positionForChecking;
    if (cardOnHand->getState() == GinCardState::GinCARDSTATE_ONHAND)
        positionForChecking = cardOnHand->getProperty()._positionOnHand;
    else if (cardOnHand->getState() == GinCardState::GinCARDSTATE_SELECTED)
        positionForChecking = cardOnHand->getProperty()._positionOnHandWasSelected;
    if (_layerControlCard->getIndexCardOnHand(cardOnHand) > _layerControlCard->getIndexCardOnHand(cardDrag))
        positionForChecking.x = positionForChecking.x + GPGinControlCardLayer::_distanceBetweenTwoCards / 2;
    else
        positionForChecking.x = positionForChecking.x - GPGinControlCardLayer::_distanceBetweenTwoCards / 2;
    float distanceX = abs(cardDrag->getPositionX() - positionForChecking.x);
    float distanceY = abs(cardDrag->getPositionY() - positionForChecking.y);
    if (distanceX <= DISTANCE_MOVE_IN_ANOTHER_CARD_X && distanceY <= DISTANCE_MOVE_IN_ANOTHER_CARD_Y)
        return true;
    return false;
}

bool GinCard::onTouchBegan(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
    if (!_isMoving && !_touch)
    {
        _touch = ptouch;
        Vec2 location = ptouch->getLocation();
        _touchBeganPosition = this->convertToNodeSpace(location);
        if (this->getState() == GinCardState::GinCARDSTATE_ONHAND)
        {
            DelayTime* actDelay    = DelayTime::create(TIME_DELAY_TO_START_DRAG_CARD);
            CallFunc * actFunc     = CallFunc::create([=]() {
                if (this->getState() == GinCardState::GinCARDSTATE_ONHAND)
                {
                    _listMovingCards.clear();
                    _listMovingCards = _layerControlCard->checkCardInAGroup(this);
                    if (_listMovingCards.size() == 0)
                        _listMovingCards.push_back(this);
                    this->startDrag(this, location);
                    _layerControlCard->startDrap(this);
                }
            });
            Sequence * actSequence = Sequence::create(actDelay, actFunc, nullptr);
            actSequence->setTag(TAG_ACTION_CHECK_START_DRAG_CARD);
            this->runAction(actSequence);
        }
    }
    return true;
}

void GinCard::onTouchMoved(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
    if (_touch == ptouch)
    {
        Vec2 location = ptouch->getLocation();
        if (this->getState() == GinCardState::GinCARDSTATE_ONHAND || this->getState() == GinCardState::GinCARDSTATE_SELECTED)
        {
            float distance = this->convertToNodeSpace(location).distance(_touchBeganPosition);
            if (distance >= DISTANCE_EFFECT_DRAG)
            {
                this->stopActionByTag(TAG_ACTION_CHECK_START_DRAG_CARD);
                _listMovingCards.clear();
                _listMovingCards = _layerControlCard->checkCardInAGroup(this);
                if (_listMovingCards.size() == 0)
                    _listMovingCards.push_back(this);
                this->startDrag(this, location);
                _layerControlCard->startDrap(this);
            }
        }
        else if (this->getState() == GinCardState::GinCARDSTATE_DRAG)
        {
            this->drag(this, location);
        }
    }
}

void GinCard::onTouchEnded(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
    if (_touch == ptouch)
    {
        if (this->getState() == GinCardState::GinCARDSTATE_ONHAND)
        {
            SoundManager::playSoundEffect(SoundEvent::TL_CLICK_CARD);
            this->stopActionByTag(TAG_ACTION_CHECK_START_DRAG_CARD);
            this->selected(true);

            _layerControlCard->unselectAllCardsExclude(this->getCardValue(), true);

            _layerControlCard->checkAutoSelectCardIngroup(_value);
        }
        else if (this->getState() == GinCardState::GinCARDSTATE_SELECTED)
        {
            SoundManager::playSoundEffect(SoundEvent::TL_CLICK_CARD);
            this->unSelected();
            _layerControlCard->checkAutoUnselectCardInGroup(_value);
        }
        else if (this->getState() == GinCardState::GinCARDSTATE_DRAG)
        {
            this->unDrag(this);
        }
        _touch = nullptr;
    }
}

void GinCard::onTouchCancelled(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
    if (this->getState() == GinCardState::GinCARDSTATE_DRAG)
        this->unDrag(this);
    _touch = nullptr;
}


void GinCard::cardShowOnHand(float time, const cocos2d::Vec2& midPos)
{
    if (_state == GinCardState::GinCARDSTATE_UNKNOWN || _state == GinCardState::GinCARDSTATE_ONHAND)
    {
        _isMoving = true;
        // Move
        ccBezierConfig bezier;
        bezier.controlPoint_1 = this->getPosition();
        bezier.controlPoint_2 = midPos;
        bezier.endPosition    = _property._positionOnHand;
        this->runAction(Sequence::create(Spawn::create(EaseExponentialOut::create(BezierTo::create(time, bezier)), EaseExponentialOut::create(RotateTo::create(time, _property._angle)), nullptr),
                                         CallFunc::create([=]() {
                                             _isMoving = false;
                                             this->setState(GinCardState::GinCARDSTATE_ONHAND);
                                         }),
                                         nullptr));
    }
}

void GinCard::cardShowOnHand(float time)
{
    if (_state == GinCardState::GinCARDSTATE_UNKNOWN || _state == GinCardState::GinCARDSTATE_ONHAND)
    {
        _isMoving = true;
        this->runAction(Sequence::create(DelayTime::create(time), Show::create(), CallFunc::create([=]() {
            this->open();
            _isMoving = false;
            this->setState(GinCardState::GinCARDSTATE_ONHAND);
        }), nullptr));
    }
}

void GinCard::cardCollectToMid(float time, const cocos2d::Vec2& midPos, const cocos2d::Vec2& endPos)
{
    _isMoving = true;
    // Move
    ccBezierConfig bezier;
    bezier.controlPoint_1 = this->getPosition();
    bezier.controlPoint_2 = midPos;
    bezier.endPosition    = endPos;
    this->runAction(Sequence::create(Spawn::create(EaseExponentialOut::create(BezierTo::create(time, bezier)), EaseExponentialOut::create(RotateTo::create(time, 0)), nullptr), CallFunc::create([=]() {
        _isMoving = false;
    }), nullptr));
}

void GinCard::cardCloseAndRemove(bool fadeOut /*= false*/)
{
    this->runAction(Sequence::create(fadeOut ? FadeOut::create(0.25f) : FadeOut::create(0.01f), CallFunc::create([=]() {
        this->removeFromParent();
    }), nullptr));
}

void GinCard::playEffectHighLightMeldCard()
{
    Sequence* actSequence = Sequence::create(EaseBackIn::create(ScaleTo::create(TIME_ACTION_HIGHLIGHT_MELD_CARD, 1.2f * CARD_ON_TABLE_SCALE_1)),
                                             EaseBackOut::create(ScaleTo::create(TIME_ACTION_HIGHLIGHT_MELD_CARD, 1.0f * CARD_ON_TABLE_SCALE_1)),
                                             nullptr);
    actSequence->setTag(TAG_ACTION_HIGHLIGHT_MELD_CARD);
    this->stopActionByTag(TAG_ACTION_HIGHLIGHT_MELD_CARD);
    this->setScale(1.0f * CARD_ON_TABLE_SCALE_1);
    this->runAction(actSequence);
}

void GinCard::playEffectSpinCard()
{
    Vector<FiniteTimeAction*> listAction;

    listAction.pushBack(RotateBy::create(ACTION_SPIN_CARD_DURATION, Vec3(0,90.0f,0)));
    listAction.pushBack(CallFunc::create([=]{
        if(this->_isOpen == true)
        {
            this->close(false);
        }
        else
        {
            this->open(false);
        }
    }));
    listAction.pushBack(RotateBy::create(ACTION_SPIN_CARD_DURATION, Vec3(0,90.0f,0)));

    Sequence* actSequence = Sequence::create(listAction);
//    actSequence->setTag(TAG_ACTION_SPIN_CARD);
    Repeat* actRepeat = Repeat::create(actSequence,ACTION_SPIN_CARD_TIME);
    actRepeat->setTag(TAG_ACTION_SPIN_CARD);

    this->stopActionByTag(TAG_ACTION_SPIN_CARD);
    this->runAction(actRepeat);
}

float GinCard::getSpinEffectDuration()
{
   return ACTION_SPIN_CARD_DURATION*2*ACTION_SPIN_CARD_TIME;
}

float GinCard::getDragScaleRatioOnDragging()
{
    float result  = CARD_IN_HAND_SCALE;

    Point cardPos = this->getPosition();


    Point anchorPos = Point::ZERO;
    float radius = 1.0f;

    if(this->_isOpen == true)
    {
        anchorPos = this->_layerControlCard->getDumpPlacePosInLayoutCard();
        radius = this->_layerControlCard->getDumpPlaceSize().height/2;
    }
    else
    {
        anchorPos = this->_layerControlCard->getDrawPlacePosInLayoutCard();
        radius = this->_layerControlCard->getDrawPlaceSize().height/2;
    }

    float distance = cardPos.distance(anchorPos);

    if(distance >= radius)
    {
        result = CARD_IN_HAND_MAX_SCALE;
    }
    else
    {
        float dis = abs(radius - distance);

        result = (CARD_IN_HAND_MAX_SCALE - GPGinInterfaceLayer::_scaleTakeCard)*(1-dis/radius) + GPGinInterfaceLayer::_scaleTakeCard;
    }

    return result;
}

float GinCard::getBoundY()
{
    float boundY = this->_layerControlCard->getLayoutCard()->getContentSize().height/2;
    boundY += this->getCardSize().height/2;
    boundY += DISTANCE_WAS_SELECTED_CARD;
    return boundY;
}

cocos2d::Size GinCard::getCardSize()
{
   if(this->_isOpen)
   {
       return this->_frontSprite->getContentSize()*this->getScale();
   }
   else
   {
       return this->_backSprite->getContentSize()*this->getScale();
   }
}

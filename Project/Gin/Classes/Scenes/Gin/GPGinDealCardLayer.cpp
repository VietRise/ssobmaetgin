#include "GPGinDealCardLayer.h"
#include "Define/GameDefine.h"
#include "ServerOffline/Rules/RuleConstant.h"
#include "GPGinInterfaceLayer.h"
#include "GPGinControlCardLayer.h"

#include "GPGinScene.h"

#define GIN_CARD_SPEED 0.3f
#define GIN_DELAY_PER_ROUND 0.0f
#define GIN_DELAY_PER_SLOT 0.04f
#define DEAL_CARD_DEFAULT_CARD_SIZE cocos2d::Size(60.0f,90.0f)
#define GIN_CARD_DEFAULT_DISTANCE (this->getContentSize().height/2)

USING_NS_CC;
using namespace cocos2d::ui;

GPGinDealCardLayer::~GPGinDealCardLayer()
{
}

GPGinDealCardLayer::GPGinDealCardLayer(const cocos2d::Size& layoutRootSize, float layoutRootScale)
{
    _layoutRootSize  = layoutRootSize;
    _layoutRootScale = layoutRootScale;
}

bool GPGinDealCardLayer::init()
{
    if (!Layer::init())
        return false;
    Size winSize = Director::getInstance()->getWinSize();
    _layoutRoot = Layout::create();
    _layoutRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _layoutRoot->setContentSize(_layoutRootSize);
    _layoutRoot->setScale(_layoutRootScale);
    _layoutRoot->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));

//    this->_layoutRoot->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
//    this->_layoutRoot->setColor(Color3B::GREEN);

    this->addChild(_layoutRoot);
    for (int i = 0; i < MAXIMUM_NUMBER_CARD; i++)
    {
        Sprite* fakeCard = Sprite::create();
        fakeCard->setScale(GPGinInterfaceLayer::_scaleDrawCard);
        fakeCard->setLocalZOrder(MAXIMUM_NUMBER_CARD - i);
        fakeCard->setOpacity(0);
        _layoutRoot->addChild(fakeCard);
        this->_listFakeCard.pushBack(fakeCard);
    }
    return true;
}

GPGinDealCardLayer* GPGinDealCardLayer::createLayerForGame(const cocos2d::Size& layerSize,
                                                           float layerScale,
                                                           const cocos2d::Point& layerPosition,
                                                           const std::vector<Point>& listSlot,
                                                           int playerPos,
                                                           const cocos2d::Point& dealCardCenter,
                                                           int cardBackId)
{
    GPGinDealCardLayer* layer = new GPGinDealCardLayer(layerSize, layerScale);
    if (layer && layer->init())
    {
        layer->setListSlotPos(listSlot, playerPos);
        layer->setDealCardCenter(dealCardCenter);
        layer->setCardBackId(cardBackId);
        layer->setPosition(layerPosition);
        layer->moveFakeCardToCenter();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

void GPGinDealCardLayer::setListSlotPos(const std::vector<Point>& listSlotPos, int playerPos)
{
    this->_listSlotPos = listSlotPos;
    this->_playerPos   = playerPos;
}

void GPGinDealCardLayer::setDealCardCenter(const cocos2d::Point& center)
{
    this->_dealCardCenter = center;
}

void GPGinDealCardLayer::setCardBackId(int id)
{
    this->_cardBackId = id;
    updateFakeCardSprite();
}

void GPGinDealCardLayer::updateFakeCardSprite()
{
    std::string cardSpriteName = GinCardStore::getCardBackSpriteNameFromID(this->_cardBackId);
    for (int    i              = 0; i < this->_listFakeCard.size(); i++)
    {
        Sprite     * sprite = (Sprite*) (this->_listFakeCard.at(i));
        SpriteFrame* frame  = SpriteFrameCache::getInstance()->getSpriteFrameByName(cardSpriteName);
        sprite->setSpriteFrame(frame);
    }
}

void GPGinDealCardLayer::moveFakeCardToCenter()
{
    for (int i = 0; i < this->_listFakeCard.size(); i++)
    {
        Sprite* sprite = (Sprite*) (this->_listFakeCard.at(i));
        sprite->setPosition(this->_layoutRoot->convertToNodeSpace(this->_dealCardCenter) - Point(0.20f * (this->_listFakeCard.size() - 1 - i), -0.30f * (this->_listFakeCard.size() - 1 - i)));
    }
}

int GPGinDealCardLayer::getNumCardForGame()
{
    return RuleConstant::Common::MAX_HAND_CARD;
}

void GPGinDealCardLayer::setDealCardCallback(const PreDealCardCallback& preDealCardCallback, const OnDealCardCallback& onDealCardCallBack, const PosDealCardCallback& posDealCardCallback)
{
    this->_preDealCardCallback = preDealCardCallback;
    this->_onDealCardCallback  = onDealCardCallBack;
    this->_posDealCardCallback = posDealCardCallback;
}

void GPGinDealCardLayer::setPosDealCardLayer(const std::vector<std::vector<GinCardProperty>>& listHandCardProperty, int firstOderIndexDeal)
{
    _listHandCardProperty = listHandCardProperty;
    for (int i = 0; i < _listHandCardProperty.size(); i++)
    {
        if (i != MAIN_USER_SLOT_INDEX)
        {
            for (int j = 0; j < _listHandCardProperty.at(i).size(); j++)
            {
                _listHandCardProperty.at(i).at(j)._positionOnHand            = _layoutRoot->convertToNodeSpace(_listSlotPos.at(i));
                _listHandCardProperty.at(i).at(j)._positionOnHandWasSelected = _layoutRoot->convertToNodeSpace(_listSlotPos.at(i));
            }
        }
    }
    _oderIndexFirst = firstOderIndexDeal;
}

void GPGinDealCardLayer::playDealCardAnimation()
{
    reset();
    if (this->_preDealCardCallback != nullptr)
        this->_preDealCardCallback();
    onPlayAnimation(CallFunc::create(this->_onDealCardCallback), CallFunc::create(this->_posDealCardCallback));
}

void GPGinDealCardLayer::onPlayAnimation(cocos2d::CallFunc* onDealCardCallBack, cocos2d::CallFunc* posDealCardCallback)
{
    int      numCard        = getNumCardForGame();
    int      realSizeOfUser = 0;
    for (int i              = 0; i < _listHandCardProperty.size(); i++)
        if (_listHandCardProperty.at(i).size() > 0)
            realSizeOfUser++;
    float    delayRoundPlus = 0.5f;
    float    timeDelay      = GIN_DELAY_PER_ROUND;
    int      i              = _oderIndexFirst;
    int      count          = 0;
    do
    {
        for (int j = 0; j < _listHandCardProperty.at(i).size(); j++)
        {
            int   fakeCardIndex = j * realSizeOfUser + ((i - _oderIndexFirst) + _listHandCardProperty.size()) % _listHandCardProperty.size();
            Point moveToPoint;
            if (i != _playerPos)
                moveToPoint = this->_layoutRoot->convertToNodeSpace(_listSlotPos[i]);
            else
                moveToPoint = this->_layoutRoot->convertToNodeSpace(_listHandCardProperty.at(i).at(j)._positionOnHand);
            int multiFactor = (moveToPoint.distance(_dealCardCenter) / GIN_CARD_DEFAULT_DISTANCE + 1);
            Sprite* sprite = _listFakeCard.at(fakeCardIndex);
            cocos2d::Vector<FiniteTimeAction*> listSpawn;
            listSpawn.pushBack(EaseCubicActionOut::create(MoveTo::create(GIN_CARD_SPEED * multiFactor, moveToPoint)));
            listSpawn.pushBack(EaseCubicActionOut::create(RotateBy::create(GIN_CARD_SPEED * multiFactor, _listHandCardProperty.at(i).at(j)._angle - (-180))));
            if (i != _playerPos)
            {
                listSpawn.pushBack(ScaleTo::create(0.1f, DEAL_CARD_DEFAULT_CARD_SIZE.width / sprite->getContentSize().width));
                listSpawn.pushBack(Sequence::create(DelayTime::create(GIN_CARD_SPEED * multiFactor * 0.4f),
                                                    CallFunc::create(CC_CALLBACK_0(GPGinDealCardLayer::onCardMoveToPlace, this, fakeCardIndex, j, i)),
                                                    nullptr));
            }
            else
            {
                listSpawn.pushBack(ScaleTo::create(0.1f, CARD_IN_HAND_SCALE));
            }
            if (i == _playerPos)
            {
                listSpawn.pushBack(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
                    sprite->setLocalZOrder(i);
                }), nullptr));
            }
            Spawn* spawn = Spawn::create(listSpawn);
            cocos2d::Vector<FiniteTimeAction*> listAction;
            listAction.pushBack(DelayTime::create((fakeCardIndex + 1) * GIN_DELAY_PER_SLOT));
            listAction.pushBack(RotateTo::create(0, -180));
            if (onDealCardCallBack != nullptr)
                listAction.pushBack(onDealCardCallBack);
            listAction.pushBack(spawn);
            count++;
            if (count == getNumCardForGame() * realSizeOfUser)
            {
                listAction.pushBack(CallFunc::create(CC_CALLBACK_0(GPGinDealCardLayer::onFinishDealCard, this)));
                if (posDealCardCallback != nullptr)
                    listAction.pushBack(posDealCardCallback);
            }
            sprite->runAction(Sequence::create(listAction));
        }
        i          = (i + 1) % _listHandCardProperty.size();
    } while (i != _oderIndexFirst);
}

void GPGinDealCardLayer::onCardMoveToPlace(int fakeCardIndex, int handIndex, int slotIndex)
{
    _listFakeCard.at(fakeCardIndex)->setLocalZOrder(0);
    _listFakeCard.at(fakeCardIndex)->runAction(FadeOut::create(0.2f));
}

void GPGinDealCardLayer::onFinishDealCard()
{
    int      numCard = getNumCardForGame();
    for (int i       = 0; i <= _listSlotPos.size() * numCard; i++)
        _listFakeCard.at(i)->setVisible(false);
}

void GPGinDealCardLayer::reset()
{
    stopAllActions();
    moveFakeCardToCenter();
    for (int i = 0; i < MAXIMUM_NUMBER_CARD; i++)
    {
        Sprite* fakeCard = this->_listFakeCard.at(i);
        fakeCard->stopAllActions();
        fakeCard->setLocalZOrder(MAXIMUM_NUMBER_CARD - i);
        fakeCard->setVisible(true);
        fakeCard->setRotation(0);
        fakeCard->setOpacity(255.0f);
        fakeCard->setScale(GPGinInterfaceLayer::_scaleDrawCard);
    }
}

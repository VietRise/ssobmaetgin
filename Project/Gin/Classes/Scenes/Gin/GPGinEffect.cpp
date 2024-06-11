#include "GPGinEffect.h"
#include "GPGinControlCardLayer.h"
#include "Define/GameDefine.h"
#include "Define/ResourcesDefine.h"
#include "Manager/TextManager.h"
#include "Manager/SoundManager.h"
#include "Helper/GameUtils.h"
#include "Component/JackpotIconNode.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace ui;
using namespace std;

#define Z_ORDER_EFFECT_RANK_ORDER        10
#define Z_ORDER_EFFECT_MONEY            20
#define RESULT_WIN_EN_IMAGE_NAME "ui/ImageGPGin/win_E.png"
#define RESULT_GIN_EN_IMAGE_NAME "ui/ImageGPGin/gin_E.png"
#define RANK_EFFECT_SPRITE_NAME "rank"
#define SHOW_MONEY_CHANGE_DELAYTIME 0.5f

GPGinEffect::~GPGinEffect()
{
    CC_SAFE_RELEASE_NULL(_particleShowCard);
    CC_SAFE_RELEASE_NULL(_particleShowStatus);
}

GPGinEffect::GPGinEffect()
{
    _layerEffect = nullptr;
}

void GPGinEffect::initLayout(const cocos2d::Size& layoutSize, float layoutScale, cocos2d::Node* parentScene, cocos2d::Node* parentControlLayer)
{
    _layerEffect = Layout::create();
    _layerEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _layerEffect->setContentSize(layoutSize);
    _layerEffect->setScale(layoutScale);
    _layerEffect->setPosition(Director::getInstance()->getWinSize() * 0.5f);
    _layerEffect->setName("Layer Effect");
//    _layerEffect->setColor(Color3B::RED);
    parentScene->addChild(_layerEffect, kForeground);

    _layerEffectControlLayer = Layer::create();
    parentControlLayer->addChild(_layerEffectControlLayer, (int) ZorderControlLayer::EFFECT_HINT);

    _particleShowCard = ParticleSystemQuad::create("ui/particle/test3.plist");
    _particleShowCard->setPosition(Point(0, 0));
    _particleShowCard->setVisible(false);
    _particleShowCard->retain();

    _particleShowStatus = ParticleSystemQuad::create("ui/particle/test3.plist");
    _particleShowStatus->setPosition(Point(0, 0));
    _particleShowStatus->setVisible(false);
    _particleShowStatus->retain();

    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listNodeRankEffect.pushBack(Node::create());
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        Layout* rankEffectNode = Layout::create();
        Sprite* sprite         = Sprite::createWithSpriteFrameName(RESULT_GIN_EN_IMAGE_NAME);
        rankEffectNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rankEffectNode->setContentSize(sprite->getContentSize());
        rankEffectNode->addChild(sprite);
        sprite->setPosition(rankEffectNode->getContentSize() * 0.5f);
        _listNodeRankEffectGinOnTable.pushBack(rankEffectNode);
    }
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        MoneyBoxNode* moneyBoxNode = MoneyBoxNode::create();
        moneyBoxNode->reset();
        _listMoneyBoxNodeGatherEffect.pushBack(moneyBoxNode);
    }
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        _listTextBMStatus.push_back(nullptr);
    }
}

void GPGinEffect::reset()
{
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        if (_listTextBMStatus[i])
        {
            _listTextBMStatus[i]->removeFromParent();
            _listTextBMStatus[i] = nullptr;
        }
    }
    _moneyBox = nullptr;
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listMoneyBoxNodeGatherEffect.at(i)->removeFromParent();
    if (_layerEffect)
        _layerEffect->removeAllChildren();
    if (_layerEffectControlLayer)
        _layerEffectControlLayer->removeAllChildren();
}

void GPGinEffect::showEffectScoreWinLose(bool win,
                                         long long money,
                                         bool finalRes,
                                         const cocos2d::Vec2& position,
                                         const cocos2d::Vec2& positionMoney,
                                         int orderIdx,
                                         const std::function<void()>& callbackUpdateMoney,
                                         bool fast /*= false*/)
{
    if (orderIdx == MAIN_USER_SLOT_INDEX)
    {
        _moneyBox = MoneyBoxNode::create();
        _moneyBox->setPosition(_layerEffect->convertToNodeSpace(position + Vec2(0, 29.0f)));
        _layerEffect->addChild(_moneyBox, 100);

        _moneyBox->setOpacity(0);
        _moneyBox->setScale(1.0f);
        _moneyBox->setMoney(money);

        float time  = 0.3f;
        float time2 = 0.5f;
        float time3 = 0.4f;
        if (fast)
        {
            time  = 0.2f;
            time2 = 0.25f;
            time3 = 0.3f;
        }
        Point          srcPos = _moneyBox->getPosition();
        Point          desPos = _layerEffect->convertToNodeSpace(positionMoney);
        ccBezierConfig bezierConfig;
        bezierConfig.controlPoint_1 = Point(srcPos.x + 70, srcPos.y);
        bezierConfig.controlPoint_2 = Point(srcPos.x + 70, desPos.y + (srcPos.y - desPos.y) * 0.2f);
        bezierConfig.endPosition    = desPos;
        _moneyBox->runAction(Sequence::create(Show::create(),
                                              Spawn::create(FadeIn::create(time2), EaseExponentialOut::create(MoveTo::create(time2, Point(_moneyBox->getPositionX(), srcPos.y + 33.0f))), nullptr),
                                              DelayTime::create(time2),
                                              Spawn::create(BezierTo::create(time, bezierConfig),
                                                            Sequence::create(EaseCubicActionOut::create(ScaleTo::create(time * 0.5f, 1.4f)),
                                                                             EaseCubicActionIn::create(ScaleTo::create(time * 0.5f, 1.0f)),
                                                                             nullptr),
                                                            nullptr),
                                              Sequence::create(Repeat::create(Sequence::create(RotateTo::create(0.1f, 5), RotateTo::create(0.15f, -5), RotateTo::create(0.1f, 0), nullptr), 1),
                                                               EaseQuinticActionOut::create(ScaleTo::create(time3, 1.1f)),
                                                               EaseQuinticActionOut::create(ScaleTo::create(time3, 0.0f)),
                                                               nullptr),
                                              Hide::create(),
                                              CallFuncN::create([=](Node* node) {
                                                  node->setPosition(_layerEffect->convertToNodeSpace(positionMoney));
                                                  if (callbackUpdateMoney != nullptr)
                                                      callbackUpdateMoney();
                                                  SoundManager::playSoundEffect(SoundEvent::TL_SHOW_FINAL_RESULT);
                                              }),
                                              RemoveSelf::create(),
                                              nullptr));
    }
    else
    {
        _layerEffect->runAction(Sequence::create(CallFunc::create([=] {
            if (money != 0)
                showUserMoneyEffect(orderIdx, money, position);
        }), DelayTime::create(SHOW_MONEY_CHANGE_DELAYTIME), CallFunc::create([=] {
            if (money != 0 && callbackUpdateMoney != nullptr)
                callbackUpdateMoney();
        }), nullptr));
    }
}

void GPGinEffect::moveBonusToMoney(double money, double bonusRate)
{
    if (money > 0 && bonusRate > 1.0f)
    {
        TextBMFont* textBonusRate = TextBMFont::create(StringUtils::format("x%.0f", bonusRate), Resources::BMFont::BMFont_Normal);
        textBonusRate->setAnchorPoint(Vec2(0.5f, 0.5f));
        textBonusRate->setPosition(_layerEffect->convertToNodeSpace(_positionTextBonus));
        textBonusRate->setVisible(false);
        textBonusRate->setColor(Color3B(255, 195, 81));
        textBonusRate->setScale(1.3f);
        _layerEffect->addChild(textBonusRate, Z_ORDER_EFFECT_MONEY);
        Point target = _moneyBox->getPosition();
        textBonusRate->setVisible(true);
        textBonusRate->runAction(Sequence::create(JumpTo::create(0.5f, target, 200.0f, 1), CallFunc::create([=] {
            _moneyBox->setMoney(money);
            _moneyBox->playBonusRate(bonusRate);
        }), RemoveSelf::create(), nullptr));
    }
}

void GPGinEffect::showEffectRank(const cocos2d::Vec2& position, int orderIdx, GinResultType type, int rank)
{
    // Play sound effect
    if (orderIdx == MAIN_USER_SLOT_INDEX)
    {
        if (type == GinResultType::GIN_RESULT_TYPE_SCORE || type == GinResultType::GIN_RESULT_TYPE_WIN_KNOCK
        || type == GinResultType::GIN_RESULT_TYPE_WIN_GIN || type == GinResultType::GIN_RESULT_TYPE_WIN_UNDERCUT
        || type == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
            if (rank == 1)
                SoundManager::playSoundEffect(SoundEvent::TL_WIN);
    }
    else
    {
        if (type == GinResultType::GIN_RESULT_TYPE_SCORE || type == GinResultType::GIN_RESULT_TYPE_WIN_KNOCK
            || type == GinResultType::GIN_RESULT_TYPE_WIN_GIN || type == GinResultType::GIN_RESULT_TYPE_WIN_UNDERCUT
            || type == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
            if (rank == 1)
                SoundManager::playSoundEffect(SoundEvent::TL_SCORE);
    }
    Node* nodeRankEffect = _listNodeRankEffect.at(orderIdx);
    nodeRankEffect->removeAllChildren();
    nodeRankEffect->setTag(rank);
    if (nodeRankEffect->getParent() == nullptr)
    {
        nodeRankEffect->setPosition(_layerEffect->convertToNodeSpace(position));
        _layerEffect->addChild(nodeRankEffect, Z_ORDER_EFFECT_RANK_ORDER);
    }
    else
    {
        nodeRankEffect->setPosition(nodeRankEffect->getParent()->convertToNodeSpace(position));
    }
    Sprite* sprite = Sprite::create();
    sprite->setTag(rank);
    sprite->setName(RANK_EFFECT_SPRITE_NAME);
    SpriteFrame* spriteFrame = getRankEffectSpriteFrame(type, rank);
    if (spriteFrame != nullptr)
        sprite->setSpriteFrame(spriteFrame);
    sprite->setScale(0.9f);
    nodeRankEffect->addChild(sprite);
    sprite->runAction(Sequence::create(CallFunc::create([=] {
        Sprite* spriteEff = Sprite::createWithSpriteFrame(sprite->getSpriteFrame()->clone());
        spriteEff->setScale(sprite->getScale());
        spriteEff->runAction(Sequence::create(Spawn::create(FadeOut::create(0.5f), ScaleBy::create(0.5f, 1.4f), nullptr), RemoveSelf::create(), nullptr));
        nodeRankEffect->addChild(spriteEff, spriteEff->getLocalZOrder() - 1);
    }), DelayTime::create(1.5f), FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
}

void GPGinEffect::showEffectRankGinOnTable(const cocos2d::Vec2& position, int orderIdx)
{
    Layout* layoutGinEffect = _listNodeRankEffectGinOnTable.at(orderIdx);
    layoutGinEffect->setPosition(position);
    layoutGinEffect->removeFromParent();
    _layerEffect->addChild(layoutGinEffect, Z_ORDER_EFFECT_RANK_ORDER);
    this->playEffectStatus(layoutGinEffect);
    SoundManager::playSoundEffect(SoundEvent::TL_GIN);
}

void GPGinEffect::showEffectStatus(cocos2d::ui::Layout* layoutTemp, int orderIdx, const cocos2d::Vec2& position, FightStatus fightStatus)
{
    if (fightStatus == FightStatus::PASS ||
        fightStatus == FightStatus::KNOCK ||
        fightStatus == FightStatus::UNDERCUT)
    {
        Layout* layoutUserStatus = _listTextBMStatus[orderIdx];
        if (layoutUserStatus == nullptr)
        {
            layoutUserStatus = (Layout*) layoutTemp->clone();
            layoutUserStatus->setCascadeColorEnabled(true);
            layoutUserStatus->setCascadeOpacityEnabled(true);
            _listTextBMStatus[orderIdx] = layoutUserStatus;
            _layerEffect->addChild(layoutUserStatus);
        }
        auto layoutPass      = Helper::seekWidgetByName(layoutUserStatus, "LayoutPass");
        auto layoutKnock      = Helper::seekWidgetByName(layoutUserStatus, "LayoutKnock");
        auto layoutUndercut      = Helper::seekWidgetByName(layoutUserStatus, "LayoutUndercut");

        layoutPass->setVisible(false);
        layoutKnock->setVisible(false);
        layoutUndercut->setVisible(false);

        layoutPass->setPositionY(layoutUserStatus->getContentSize().height / 2);
        layoutKnock->setPositionY(layoutUserStatus->getContentSize().height / 2);
        layoutUndercut->setPositionY(layoutUserStatus->getContentSize().height / 2);


        if (fightStatus == FightStatus::KNOCK)
        {
            layoutKnock->setVisible(true);
            SoundManager::playSoundEffect(SoundEvent::TL_CHALLENGE);
        }
        else if (fightStatus == FightStatus::UNDERCUT)
        {
            layoutUndercut->setVisible(true);
            SoundManager::playSoundEffect(SoundEvent::TL_FIGHT);
        }
        else if (fightStatus == FightStatus::PASS)
        {
            layoutPass->setVisible(true);
            SoundManager::playSoundEffect(SoundEvent::TL_FOLD);
        }

        layoutUserStatus->setPosition(_layerEffect->convertToNodeSpace(position));
        layoutUserStatus->setVisible(true);
        if (fightStatus == FightStatus::KNOCK || fightStatus == FightStatus::UNDERCUT)
        {
            this->playEffectStatus(layoutUserStatus);
            layoutUserStatus->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]() {
                if (fightStatus == FightStatus::KNOCK)
                {
                    auto particle = ParticleSystemQuad::create("ui/particle/fight.plist");
                    particle->setPosition(Size(layoutUserStatus->getContentSize().width / 2, layoutUserStatus->getContentSize().height / 2 - 2));
                    particle->setScale(0.8f);
                    particle->resetSystem();
                    particle->setVisible(true);
                    particle->setAutoRemoveOnFinish(true);
                    layoutUserStatus->addChild(particle, -1);
                }
                else if (fightStatus == FightStatus::UNDERCUT)
                {
                    auto particle = ParticleSystemQuad::create("ui/particle/fight2.plist");
                    particle->setPosition(Size(layoutUserStatus->getContentSize().width / 2, layoutUserStatus->getContentSize().height / 2 - 2));
                    particle->setScale(0.8f);
                    particle->resetSystem();
                    particle->setVisible(true);
                    particle->setAutoRemoveOnFinish(true);
                    layoutUserStatus->addChild(particle, -1);
                }
            }), nullptr));
        }
        else
        {
            layoutUserStatus->setOpacity(0);
            layoutUserStatus->setScale(0.5f);
            layoutUserStatus->runAction(Spawn::create(FadeIn::create(0.2f), EaseBackOut::create(ScaleTo::create(0.2f, 1.0f)), nullptr));
        }
    }
}

void GPGinEffect::hideEffectStatus(int orderIdx)
{
    if (_listTextBMStatus[orderIdx])
        _listTextBMStatus[orderIdx]->setVisible(false);
}

void GPGinEffect::playEffectStatus(cocos2d::ui::Layout* layout)
{
    Layout* layoutEffect = (Layout*) layout->clone();
    layoutEffect->retain();
    layout->setOpacity(0);
    layout->setScale(3.0f);
    float time = 0.4f;
    layout->runAction(Sequence::create(
            Spawn::create(Sequence::create(EaseExponentialIn::create(ScaleTo::create(0.88f * time, 0.7f)),
                                           EaseCubicActionIn::create(ScaleTo::create(0.12f * time, 1.0f)),
                                           nullptr),
                          EaseExponentialIn::create(FadeIn::create(0.4f * time)),
                          nullptr),
            DelayTime::create(0.45f * time),
            CallFuncN::create([=](Node *object)
                              {
                                  _particleShowStatus->removeFromParent();
                                  _particleShowStatus->resetSystem();
                                  _particleShowStatus->setVisible(true);
                                  _particleShowStatus->setScale(0.6f);
                                  _particleShowStatus->setPosition(Vec2(object->getContentSize().width * 0.5f,
                                                                        object->getContentSize().height * 0.5f));
                                  object->addChild(_particleShowStatus);
                              }),
            DelayTime::create(0.23f * time),
            CallFuncN::create([=](Node *object)
                              {
                                  Layout *spriteEff = layoutEffect;
                                  spriteEff->setPosition(Vec2(object->getContentSize().width * 0.5f,
                                                              object->getContentSize().height * 0.5f));
                                  spriteEff->setScale(object->getScale());
//                                  spriteEff->setOpacity(70);
                                  spriteEff->setOpacity(125);
                                  spriteEff->runAction(Sequence::create(
                                          Spawn::create(FadeOut::create(0.7f), ScaleBy::create(0.7f, 1.7f), nullptr),
                                          CallFunc::create([=]
                                                           {
                                                               spriteEff->removeFromParent();
                                                               spriteEff->release();
                                                           }), nullptr));
                                  object->addChild(spriteEff, -1);
                              }),
            nullptr));
}

void GPGinEffect::showEffectDrawCard(const GinCardProperty& cardPropertyStart, const GinCardProperty& cardPropertyEnd, const std::function<void()>& callbackFinish)
{
    GinCard* tlCard = GinCard::create(nullptr);
    tlCard->setCardIndex(-1);
    tlCard->setCardValue(-1);
    tlCard->setBack(1);
    tlCard->setScale(cardPropertyStart._scale);
    tlCard->setRotation(cardPropertyStart._angle);
    tlCard->setPosition(_layerEffect->convertToNodeSpace(cardPropertyStart._positionOnHand));
    _layerEffect->addChild(tlCard);
    tlCard->runAction(Sequence::create(MoveBy::create(0.05f, Point(0, 10)),
                                       DelayTime::create(0.1f),
                                       Spawn::create(EaseCubicActionOut::create(MoveTo::create(TIME_CARD_DRAW, cardPropertyEnd._positionOnHand)),
                                                     EaseBackOut::create(ScaleTo::create(TIME_CARD_DRAW, cardPropertyEnd._scale)),
                                                     RotateTo::create(TIME_CARD_DRAW, cardPropertyEnd._angle),
                                                     nullptr),
                                       DelayTime::create(0.1f),
                                       CallFunc::create([=]() {
                                           if (callbackFinish)
                                               callbackFinish();
                                       }),
                                       RemoveSelf::create(),
                                       nullptr));
}

void GPGinEffect::showUserMoneyEffect(int orderIdx, double money, const cocos2d::Vec2& position)
{
    MoneyBoxNode* moneyBoxNode = MoneyBoxNode::create();
    moneyBoxNode->reset();
    moneyBoxNode->setMoney(money);
    moneyBoxNode->show(true);
    moneyBoxNode->setScale(0.8f);
    moneyBoxNode->setPosition(_layerEffect->convertToNodeSpace(position - Point(0, 12)));
    moneyBoxNode->setOpacity(0);
    _layerEffect->addChild(moneyBoxNode);
    moneyBoxNode->runAction(Sequence::create(Spawn::create(EaseExponentialOut::create(MoveBy::create(0.25f, Point(0, 40))), FadeIn::create(0.25f), nullptr),
                                             DelayTime::create(2.0f),
                                             FadeOut::create(0.1f),
                                             RemoveSelf::create(),
                                             nullptr));
}

cocos2d::SpriteFrame* GPGinEffect::getRankEffectSpriteFrame(GinResultType type, int rank)
{
    Language language = TextManager::getInstance()->getLanguageType();
    if (type == GinResultType::GIN_RESULT_TYPE_SCORE ||
        type == GinResultType::GIN_RESULT_TYPE_WIN_KNOCK ||
        type == GinResultType::GIN_RESULT_TYPE_WIN_GIN ||
        type == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN ||
        type == GinResultType::GIN_RESULT_TYPE_WIN_UNDERCUT)
    {
        switch (language)
        {
            case Language::LANGUAGE_ENGLISH:
            case Language::LANGUAGE_INDONESIAN:
                switch (rank)
                {
                    case 1:
                        return SpriteFrameCache::getInstance()->getSpriteFrameByName(RESULT_WIN_EN_IMAGE_NAME);
                    default:
                        return nullptr;
                }
                break;
            default:
                break;
        }
    }
//    else if (type == GinResultType::GIN_RESULT_TYPE_WIN_GIN || type == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
//    {
//        switch (language)
//        {
//            case Language::LANGUAGE_ENGLISH:
//            case Language::LANGUAGE_INDONESIAN:
//                return SpriteFrameCache::getInstance()->getSpriteFrameByName(RESULT_WIN_EN_IMAGE_NAME);
//                break;
//            default:
//                return SpriteFrameCache::getInstance()->getSpriteFrameByName(RESULT_WIN_EN_IMAGE_NAME);
//                break;
//        }
//    }
    // Lose instant and Folded
    else
    {
        switch (language)
        {
            case Language::LANGUAGE_ENGLISH:
            case Language::LANGUAGE_INDONESIAN:
                return nullptr;
            default:
                return nullptr;
        }
    }
    return nullptr;
}

void GPGinEffect::setPositionOfTextBonus(const cocos2d::Point& position)
{
    this->_positionTextBonus = position;
}

void GPGinEffect::showEffectGatherMoneyToPot(const cocos2d::Vec2& jackpotPos, const std::vector<cocos2d::Vec2>& listPlayerPos, double money, const std::function<void()>& callback)
{
    float moneyBoxScale = 0.9f;
    _layerEffect->runAction(Sequence::create(CallFunc::create([=]() {
        for (int i = 0; i < listPlayerPos.size(); i++)
        {
            MoneyBoxNode* moneyBoxNode = _listMoneyBoxNodeGatherEffect.at(i);
            moneyBoxNode->setPosition(_layerEffect->convertToNodeSpace(listPlayerPos.at(i)));
            moneyBoxNode->reset();
            moneyBoxNode->setMinus(false);
            moneyBoxNode->setMoney(-1 * money);
            moneyBoxNode->setOpacity(150);
            moneyBoxNode->setScale(0);
            moneyBoxNode->show(true);
            _layerEffect->addChild(moneyBoxNode, Z_ORDER_EFFECT_MONEY);
            moneyBoxNode->runAction(Sequence::create(Spawn::create(FadeIn::create(0.3f),
                                                                   EaseExponentialOut::create(ScaleTo::create(0.3f, moneyBoxScale)),
                                                                   EaseExponentialOut::create(MoveTo::create(0.3f, _layerEffect->convertToNodeSpace(listPlayerPos.at(i) + Vec2(0, 30)))),
                                                                   nullptr),
                                                     DelayTime::create(1.0f),
                                                     EaseExponentialOut::create(MoveTo::create(0.75f, _layerEffect->convertToNodeSpace(jackpotPos))),
                                                     FadeOut::create(0.1f),
                                                     RemoveSelf::create(),
                                                     nullptr));
        }
    }), DelayTime::create(2.1f), CallFunc::create([=] {
        MoneyBoxNode* moneySumBox = MoneyBoxNode::create();
        moneySumBox->reset();
        moneySumBox->setPosition(_layerEffect->convertToNodeSpace(jackpotPos));
        moneySumBox->setVisible(true);
        moneySumBox->setMoney(money * listPlayerPos.size());
        moneySumBox->setScale(0.95f * moneyBoxScale);
        moneySumBox->setOpacity(255);
        _layerEffect->addChild(moneySumBox, Z_ORDER_EFFECT_MONEY);
        moneySumBox->runAction(Sequence::create(Sequence::create(Spawn::create(EaseBackOut::create(ScaleTo::create(0.3f, moneyBoxScale)), FadeIn::create(0.3f), nullptr), nullptr),
                                                DelayTime::create(0.5f),
                                                ScaleTo::create(0.2f, 0.0f),
                                                RemoveSelf::create(),
                                                nullptr));
    }), DelayTime::create(1.0f), CallFunc::create([=]() {
        SoundManager::playSoundEffect(SoundEvent::TL_SHOW_FINAL_RESULT);
        if (callback != nullptr)
            callback();
    }), nullptr));
}

void GPGinEffect::showEffectMoveCrownToPot(const cocos2d::Vec2& jackpotPos, const cocos2d::Vec2& posStart, int round, const std::function<void()>& callback)
{
    JackpotIconNode* jackpotIcon = JackpotIconNode::create();
    jackpotIcon->reset();
    jackpotIcon->enableJackpot(true);
    jackpotIcon->show(true, false, 0);
    jackpotIcon->setPosition(_layerEffect->convertToNodeSpace(posStart));
    _layerEffect->addChild(jackpotIcon, Z_ORDER_EFFECT_MONEY);
    jackpotIcon->setScale(0);
    jackpotIcon->runAction(Sequence::create(CallFunc::create([=]() {
                                                SoundManager::playSoundEffect(SoundEvent::TL_HITPOT_1);
                                            }),
                                            EaseBackOut::create(ScaleTo::create(0.25f, 1.5f)),
                                            DelayTime::create(0.75f),
                                            EaseCircleActionOut::create(MoveTo::create(0.7f, _layerEffect->convertToNodeSpace(jackpotPos))),
                                            DelayTime::create(0.1f),
                                            CallFunc::create([=]() {
                                                SoundManager::playSoundEffect(SoundEvent::TL_HITPOT_2);
                                                if (callback != nullptr)
                                                    callback();
                                            }),
                                            Hide::create(),
                                            RemoveSelf::create(),
                                            nullptr));
}

void GPGinEffect::showEffectWinJackpot(int slotIdx,
                                       const cocos2d::Vec2& jackpotPos,
                                       const cocos2d::Vec2& userPos,
                                       double totalMoney,
                                       const std::function<void()>& callbackResetJackpot,
                                       const std::function<void()>& cbUpdateMoney,
                                       const std::function<void()>& cbUpdateLevel,
                                       const std::function<void()>& callbackFinish)
{
    SoundManager::playSoundEffect(SoundEvent::TL_WIN_JACKPOT);
    auto moneyBox = MoneyBoxNode::create();
    moneyBox->reset();
    moneyBox->setMoney(totalMoney);
    moneyBox->setPosition(_layerEffect->convertToNodeSpace(jackpotPos));
    _layerEffect->addChild(moneyBox, Z_ORDER_EFFECT_MONEY);
    moneyBox->setScale(0);
    moneyBox->setOpacity(0);
    float scale = 1.2f;
    moneyBox->runAction(Sequence::create(DelayTime::create(1.0f),
                                         CallFunc::create([=] {
                                             _particleShowCard->removeFromParent();
                                             _particleShowCard->resetSystem();
                                             _particleShowCard->setVisible(true);
                                             _particleShowCard->setPosition(Vec2::ZERO);
                                             moneyBox->addChild(_particleShowCard);
                                             if (callbackResetJackpot != nullptr)
                                                 callbackResetJackpot();
                                         }),
                                         Show::create(),
                                         Spawn::create(FadeIn::create(0.3f), EaseExponentialOut::create(ScaleTo::create(0.3f, scale)), nullptr),
                                         DelayTime::create(1.0f),
                                         EaseExponentialOut::create(MoveTo::create(1.0f, _layerEffect->convertToNodeSpace(userPos))),
                                         Sequence::create(Repeat::create(Sequence::create(RotateTo::create(0.1f, 5), RotateTo::create(0.15f, -5), RotateTo::create(0.1f, 0), nullptr), 1),
                                                          EaseQuinticActionOut::create(ScaleTo::create(0.4f, scale + 0.1f)),
                                                          EaseQuinticActionOut::create(ScaleTo::create(0.4f, 0.0f)),
                                                          nullptr),
                                         Hide::create(),
                                         CallFunc::create([=] {
                                             SoundManager::playSoundEffect(SoundEvent::TL_SHOW_FINAL_RESULT);
                                             if (cbUpdateMoney != nullptr)
                                                 cbUpdateMoney();
                                         }),
                                         DelayTime::create(0.2f),
                                         CallFunc::create([=] {
                                             if (cbUpdateLevel != nullptr)
                                                 cbUpdateLevel();
                                         }),
                                         CallFunc::create([=] {
                                             if (callbackFinish != nullptr)
                                                 callbackFinish();
                                         }),
                                         RemoveSelf::create(),
                                         nullptr));
}

void GPGinEffect::showStartRoundEffect(cocos2d::ui::Layout* layoutTemp, const cocos2d::Vec2& position, int currentRound)
{
    auto layout = layoutTemp->clone();
    layout->setScale(0.5f);
    layout->setOpacity(1);
    Point desPos = position;
    layout->setPosition(_layerEffect->convertToNodeSpace(desPos));
    _layerEffect->addChild(layout, kForeground);
    layout->setVisible(false);
    auto label = (Text*) Helper::seekWidgetByName(layout, "Label");
    if (currentRound == 1)
        label->setString(fmt::format(GET_TEXT_BY_ID(TXT_ROUND_START_FIRST_TITLE), currentRound));
    else
        label->setString(fmt::format(GET_TEXT_BY_ID(TXT_ROUND_START_TITLE), currentRound));
    layout->runAction(Sequence::create(DelayTime::create(0.1f),
                                       Show::create(),
                                       Spawn::create(FadeIn::create(0.2f), ScaleTo::create(0.2f, 1.0f), nullptr),
                                       DelayTime::create(0.5f),
                                       CallFunc::create([=]() {
                                           SoundManager::playSoundEffect(SoundEvent::TL_READY);
                                       }),
                                       DelayTime::create(2.5f),
                                       FadeOut::create(0.2f),
                                       Hide::create(),
                                       RemoveSelf::create(),
                                       nullptr));
}

void GPGinEffect::showMatchEndStatusEffect(cocos2d::ui::Layout* layoutTemp,GinMatchEndStatus status, const cocos2d::Vec2& position, MatchEndStatusFinishCallback callback)
{
    if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_NONE)
    {
        if(callback != nullptr)
        {
            callback();
        }
        return;
    }

    Layout *layoutStatus = (Layout *) layoutTemp->clone();
    layoutStatus->setCascadeColorEnabled(true);
    layoutStatus->setCascadeOpacityEnabled(true);
    _layerEffect->addChild(layoutStatus);

    layoutStatus->setVisible(true);
    layoutStatus->setPosition(_layerEffect->convertToNodeSpace(position));

    auto layoutKnock = Helper::seekWidgetByName(layoutStatus, "LayoutKnock");
    auto layoutUndercut = Helper::seekWidgetByName(layoutStatus, "LayoutUndercut");
    auto layoutGin = Helper::seekWidgetByName(layoutStatus, "LayoutGin");
    auto layoutBigGin = Helper::seekWidgetByName(layoutStatus, "LayoutBigGin");
    auto layoutGin25 = Helper::seekWidgetByName(layoutStatus, "LayoutGin25");
    auto layoutBigGin50 = Helper::seekWidgetByName(layoutStatus, "LayoutBigGin50");
    auto layoutDraw = Helper::seekWidgetByName(layoutStatus, "LayoutDraw");
    auto layoutSpadesDouble = Helper::seekWidgetByName(layoutStatus, "LayoutSpadesDouble");

    layoutKnock->setVisible(false);
    layoutUndercut->setVisible(false);
    layoutGin->setVisible(false);
    layoutBigGin->setVisible(false);
    layoutDraw->setVisible(false);
    layoutSpadesDouble->setVisible(false);
    layoutGin25->setVisible(false);
    layoutBigGin50->setVisible(false);

    layoutKnock->setPosition(layoutStatus->getContentSize() / 2);
    layoutUndercut->setPosition(layoutStatus->getContentSize() / 2);
    layoutGin->setPosition(layoutStatus->getContentSize() / 2);
    layoutBigGin->setPosition(layoutStatus->getContentSize() / 2);
    layoutDraw->setPosition(layoutStatus->getContentSize() / 2);
    layoutSpadesDouble->setPosition(layoutStatus->getContentSize() / 2);
    layoutGin25->setPosition(layoutStatus->getContentSize() / 2);
    layoutBigGin50->setPosition(layoutStatus->getContentSize() / 2);

    if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_KNOCK)
    {
        layoutKnock->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_FIGHT);
    }
    else if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_UNDERCUT)
    {
        layoutUndercut->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_UNDERCUT_25);
    }
    else if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_GIN)
    {
        layoutGin->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_GIN);
    }
    else if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_BIGGIN)
    {
        layoutBigGin->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_GIN);
    }
    else if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_DRAW)
    {
        layoutDraw->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_FOLD);
    }
    else if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_SPADES_DOUBLE)
    {
        layoutSpadesDouble->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_FIGHT);
    }
    else if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_GIN_25)
    {
        layoutGin25->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_GIN_25);
    }
    else if (status == GinMatchEndStatus::GIN_MATCH_END_STATUS_BIGGIN_50)
    {
        layoutBigGin50->setVisible(true);
        SoundManager::playSoundEffect(SoundEvent::TL_BIGGIN_50);
    }

   this->playEffectStatus(layoutStatus);
   layoutStatus->runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));
   if(status == GinMatchEndStatus::GIN_MATCH_END_STATUS_KNOCK)
   {
       layoutStatus->runAction(Sequence::create(DelayTime::create(0.352f), CallFunc::create([=]()
                                                                                          {

                                                                                              auto particle = ParticleSystemQuad::create(
                                                                                                      "ui/particle/fight.plist");
                                                                                              particle->setPosition(
                                                                                                      Size(layoutStatus->getContentSize().width /
                                                                                                           2,
                                                                                                           layoutStatus->getContentSize().height /
                                                                                                           2 -
                                                                                                           2));
                                                                                              particle->setScale(
                                                                                                      0.8f);
                                                                                              particle->resetSystem();
                                                                                              particle->setVisible(
                                                                                                      true);
                                                                                              particle->setAutoRemoveOnFinish(
                                                                                                      true);
                                                                                              layoutStatus->addChild(
                                                                                                      particle,
                                                                                                      -1);
                                                                                          }), nullptr));
   }
   else if(status == GinMatchEndStatus::GIN_MATCH_END_STATUS_UNDERCUT)
   {
       layoutStatus->runAction(Sequence::create(DelayTime::create(0.352f), CallFunc::create([=]()
                                                                                          {

                                                                                              auto particle = ParticleSystemQuad::create(
                                                                                                      "ui/particle/fight2.plist");
                                                                                              particle->setPosition(
                                                                                                      Size(layoutStatus->getContentSize().width /
                                                                                                           2,
                                                                                                           layoutStatus->getContentSize().height /
                                                                                                           2 -
                                                                                                           2));
                                                                                              particle->setScale(
                                                                                                      0.8f);
                                                                                              particle->resetSystem();
                                                                                              particle->setVisible(
                                                                                                      true);
                                                                                              particle->setAutoRemoveOnFinish(
                                                                                                      true);
                                                                                              layoutStatus->addChild(
                                                                                                      particle,
                                                                                                      -1);
                                                                                          }), nullptr));
   }
   else if(status == GinMatchEndStatus::GIN_MATCH_END_STATUS_GIN)
   {
       layoutStatus->runAction(Sequence::create(DelayTime::create(0.352f), CallFunc::create([=]()
                                                                                          {

                                                                                              auto particle = ParticleSystemQuad::create(
                                                                                                      "ui/particle/star_special.plist");
                                                                                              particle->setPosition(
                                                                                                      Size(layoutStatus->getContentSize().width /
                                                                                                           2,
                                                                                                           layoutStatus->getContentSize().height /
                                                                                                           2 -
                                                                                                           2));
                                                                                              particle->setScale(
                                                                                                      0.8f);
                                                                                              particle->resetSystem();
                                                                                              particle->setVisible(
                                                                                                      true);
                                                                                              particle->setAutoRemoveOnFinish(
                                                                                                      true);
                                                                                              layoutStatus->addChild(
                                                                                                      particle,
                                                                                                      -1);
                                                                                          }), nullptr));
   }
   else if(status == GinMatchEndStatus::GIN_MATCH_END_STATUS_BIGGIN)
   {
       layoutStatus->runAction(Sequence::create(DelayTime::create(0.352f), CallFunc::create([=]()
                                                                                          {

                                                                                              auto particle = ParticleSystemQuad::create(
                                                                                                      "ui/particle/star_special.plist");
                                                                                              particle->setPosition(
                                                                                                      Size(layoutStatus->getContentSize().width /
                                                                                                           2,
                                                                                                           layoutStatus->getContentSize().height /
                                                                                                           2 -
                                                                                                           2));
                                                                                              particle->setScale(
                                                                                                      0.8f);
                                                                                              particle->resetSystem();
                                                                                              particle->setVisible(
                                                                                                      true);
                                                                                              particle->setAutoRemoveOnFinish(
                                                                                                      true);
                                                                                              layoutStatus->addChild(
                                                                                                      particle,
                                                                                                      -1);
                                                                                          }), nullptr));
   }
}
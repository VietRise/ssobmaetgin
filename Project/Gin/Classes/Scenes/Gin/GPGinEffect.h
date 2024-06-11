#ifndef __GP_GIN_EFFECT_H__
#define __GP_GIN_EFFECT_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GinCard.h"
#include "Component/MoneyBoxNode.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"


typedef std::function<void()> MatchEndStatusFinishCallback;

class GPGinEffect : public cocos2d::Node
{
private:
    cocos2d::ui::Layout        * _layerEffect;
    cocos2d::Layer             * _layerEffectControlLayer;
    cocos2d::ParticleSystemQuad* _particleShowCard;
    cocos2d::ParticleSystemQuad* _particleShowStatus;
    std::vector<cocos2d::ui::Layout*>     _listHintGiveCards;
    cocos2d::Vector<cocos2d::Node*>       _listNodeRankEffect;
    cocos2d::Vector<cocos2d::ui::Layout*> _listNodeRankEffectGinOnTable;
    std::vector<cocos2d::ui::Layout*>     _listTextBMStatus;
    MoneyBoxNode* _moneyBox;
    cocos2d::Vector<MoneyBoxNode*> _listMoneyBoxNodeGatherEffect;
    cocos2d::Vec2                  _positionTextBonus;

    void showUserMoneyEffect(int orderIdx, double money, const cocos2d::Vec2& position);
    cocos2d::SpriteFrame* getRankEffectSpriteFrame(GinResultType type, int rank);
    void playEffectStatus(cocos2d::ui::Layout* node);
public:
    ~GPGinEffect();
    GPGinEffect();

    void initLayout(const cocos2d::Size& layoutSize, float layoutScale, cocos2d::Node* parentScene, cocos2d::Node* parentControlLayer);
    void reset();
    void showEffectScoreWinLose(bool win,
                                long long money,
                                bool finalRes,
                                const cocos2d::Vec2& position,
                                const cocos2d::Vec2& positionMoney,
                                int orderIdx,
                                const std::function<void()>& callbackUpdateMoney,
                                bool fast = false);
    void moveBonusToMoney(double money, double bonusRate);
    void showEffectRank(const cocos2d::Vec2& position, int orderIdx, GinResultType type, int rank);
    void showEffectRankGinOnTable(const cocos2d::Vec2& position, int orderIdx);
    void showEffectStatus(cocos2d::ui::Layout* layoutTemp, int orderIdx, const cocos2d::Vec2& position, FightStatus fightStatus);
    void hideEffectStatus(int orderIdx);
    void showEffectDrawCard(const GinCardProperty& cardPropertyStart, const GinCardProperty& cardPropertyEnd, const std::function<void()>& callbackFinish);
    void setPositionOfTextBonus(const cocos2d::Point& position);
    void showEffectGatherMoneyToPot(const cocos2d::Vec2& jackpotPos, const std::vector<cocos2d::Vec2>& listPlayerPos, double money, const std::function<void()>& callback);
    void showEffectMoveCrownToPot(const cocos2d::Vec2& jackpotPos, const cocos2d::Vec2& posStart, int round, const std::function<void()>& callback);
    void showEffectWinJackpot(int slotIdx,
                              const cocos2d::Vec2& jackpotPos,
                              const cocos2d::Vec2& userPos,
                              double totalMoney,
                              const std::function<void()>& callbackResetJackpot,
                              const std::function<void()>& cbUpdateMoney,
                              const std::function<void()>& cbUpdateLevel,
                              const std::function<void()>& callbackFinish);
    void showStartRoundEffect(cocos2d::ui::Layout* layoutTemp, const cocos2d::Vec2& postion, int currentRound);

    void showMatchEndStatusEffect(cocos2d::ui::Layout* layoutTemp,GinMatchEndStatus status, const cocos2d::Vec2& position, MatchEndStatusFinishCallback callback);

    CREATE_FUNC(GPGinEffect);
};

#endif 

#ifndef __GIN_BASE_CARD_H__
#define __GIN_BASE_CARD_H__

#include "cocos2d.h"
#include "GinCardStore.h"

class GinBaseCard;

#define GIN_CARD_DIM_COLOR cocos2d::Color3B(100,100,100)
#define GIN_CARD_NORMAL_COLOR cocos2d::Color3B(255,255,255)

#define GIN_WIDTH_CARD 102
#define GIN_HEIGHT_CARD 142

class GinBaseCard : public cocos2d::Node
{
protected:
    int  _index;
    int  _value;
    bool _isOpen;
    bool _isMoving;
    bool _isFlipping;
    bool _isSmall;
    bool _isDimColor;

    cocos2d::Sprite* _frontSprite;
    cocos2d::Sprite* _backSprite;
    cocos2d::Sprite* _backBlackSprite;
    cocos2d::Sprite* _meldCircleSprite;

    cocos2d::Sprite* _highLight;
    cocos2d::Sprite* _highLightTake;
    cocos2d::Sprite* _effectHighLightPhom;
    cocos2d::Sprite* _bonusCard;

    bool _isHighLightGive;
    bool _touchEnable;
    cocos2d::EventListenerTouchOneByOne* _touchListener;
public:

    virtual ~GinBaseCard();
    GinBaseCard();

    virtual bool init();

    void setCardIndex(int index, bool isSmall = false);
    int getCardIndex();

    void setCardValue(int value);
    int getCardValue();

    void setFront(cocos2d::Sprite* frontSprite);
    void setFront(int index);
    void setFrontBackground(const std::string& spriteFrameName);

    void setBack(cocos2d::Sprite* backSprite);
    void setBack(int backId);

    void setHighLight(bool highLight);
    bool getHighLight();
    void initHighLight();

    void setBonusCard(bool bonusCard);
    bool getBonusCard();
    void initBonusCard();

    void setHighLightGive(bool highLightGive);
    bool getHighLightGive();


    void setHide(bool hide);
    bool getHide();
    void initHide();

    void setDimColor(bool dimColor);
    bool getDimColor();

    void setEffectHightLightPhom(bool highLight, cocos2d::Color3B color);
    void initEffectHightLightPhom();

    virtual void open(bool anim = false);
    virtual void close(bool anim = false);

    virtual void reset();

    bool containPoint(cocos2d::Point point);
    void setTouchEnable(bool enable);
    virtual bool onTouchBegan(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    virtual void onTouchMoved(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);
    virtual void onTouchCancelled(cocos2d::Touch* ptouch, cocos2d::Event* pEvent);

    const cocos2d::Size& getContentSize();

    static int convertRank(int value);
    static int convertSuit(int value);

    std::string getFrontBackground(bool highlight);

    void setMeldHighLightVisible(bool visible, MeldColorType meldColor = MeldColorType::UNKNOWN);

    void initMeldHighlightCircle(MeldColorType meldColor = MeldColorType::UNKNOWN);
    void setMeldHighlightCircle(bool visible, MeldColorType meldColor = MeldColorType::UNKNOWN);

    CREATE_FUNC(GinBaseCard);
};

#endif // __GIN_BASE_CARD_H__

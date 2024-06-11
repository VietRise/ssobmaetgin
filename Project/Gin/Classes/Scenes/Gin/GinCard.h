#ifndef __GIN_CARD_H__
#define __GIN_CARD_H__

#include "Component/GinBaseCard.h"

#define TOTAL_NUMBER_CARD 13
#define DISTANCE_WAS_SELECTED_CARD 18
//#define DISTANCE_EFFECT_DRAG 50
#define DISTANCE_EFFECT_DRAG 10
#define DISTANCE_MOVE_IN_ANOTHER_CARD_X 25
#define DISTANCE_MOVE_IN_ANOTHER_CARD_Y 120
#define OPACITY_CARD_DRAG 255
//#define DRAG_SCALE_RATIO 1.2f
//#define UNDRAG_SCALE_RATIO 1.0f

#define TIME_ACTION_SELECTED 0.08f
#define TIME_ACTION_HIGHLIGHT_MELD_CARD 0.16f
#define TIME_ACTION_CARD_MOVE_ON_HAND_1PIXEL 0.5f/1000
#define TIME_MOVE_BETWEEEN_TWO_CARDS 0.06f
#define TIME_DELAY_TO_START_DRAG_CARD 2.0f
#define ACTION_SPIN_CARD_DURATION 0.2f
#define ACTION_SPIN_CARD_TIME 2

#define TAG_ACTION_MOVEANDROTATE_CARD 1
#define TAG_ACTION_CHECK_START_DRAG_CARD 2
#define TAG_ACTION_SELECTED_CARD 4
#define TAG_ACTION_UNSELECTED_CARD 5
#define TAG_ACTION_UNDRAG_CARD 6
#define TAG_ACTION_HIGHLIGHT_MELD_CARD 10
#define TAG_ACTION_SPIN_CARD 11

typedef struct
{
    cocos2d::Vec2 _positionOnHand;
    cocos2d::Vec2 _positionOnHandWasSelected;
    float         _angle;
    int           _zOder;
    float         _scale;
} GinCardProperty;

enum class GinCardState
{
    GinCARDSTATE_UNKNOWN,
    GinCARDSTATE_ONHAND,
    GinCARDSTATE_SELECTED,
    GinCARDSTATE_DRAG
};

enum class GinCardDragType
{
    GinDRAGCARD_UNKNOW,
    GinDRAGCARD_SINGLE_CARD,
    GinDRAGCARD_GROUP_CARD
};

class GPGinControlCardLayer;

class GinCard : public GinBaseCard
{
private:
    GinCardProperty _property;
    GinCardState    _state;

    GPGinControlCardLayer* _layerControlCard;
    cocos2d::Touch       * _touch;
    cocos2d::Vec2         _touchBeganPosition;
    std::vector<GinCard*> _listMovingCards;

    void openNoAnim();
    void openAnim();
    void closeNoAnim();
    void closeAnim();
    void actUnDragTo(GinCard* card);
    bool checkTwoCardsIsOverlap(GinCard* cardOnHand, GinCard* cardDrag);
public:
    ~GinCard();
    GinCard();

    static GinCard* create(GPGinControlCardLayer* layerControlCard);

    GinCardProperty getProperty();
    void setProperty(const GinCardProperty& positionOriginal);

    GinCardState getState();
    void setState(GinCardState state);

    std::vector<GinCard*> getListMovingCards();
    void setLayerControlCard(GPGinControlCardLayer* layerControlCard);

    cocos2d::Touch* getTouch();
    void setTouch(cocos2d::Touch* touch);

    virtual void reset() override;
    virtual void open(bool anim = false) override;
    virtual void close(bool anim = false) override;

    void selected(bool playAnim = true);
    void unSelected(bool playAnim = true);

    void startDrag(GinCard* cardIsSelected, const cocos2d::Vec2& posWorldTouch);
    void drag(GinCard* cardIsSelected, const cocos2d::Vec2& posWorldTouch, float angleRotate = 0, const cocos2d::Vec2& positonCard = cocos2d::Vec2::ZERO);
    void unDrag(GinCard* cardIsSelected);

    void checkMoveInPositionOfAnotherCard();

    virtual bool onTouchBegan(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchMoved(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchEnded(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchCancelled(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;

    void cardShowOnHand(float time, const cocos2d::Vec2& midPos);
    void cardShowOnHand(float time);
    void cardCollectToMid(float time, const cocos2d::Vec2& midPos, const cocos2d::Vec2& endPos);
    void cardCloseAndRemove(bool fadeOut = false);
    void playEffectHighLightMeldCard();
    void playEffectSpinCard();
    static float getSpinEffectDuration();

    float getDragScaleRatioOnDragging();
    float getBoundY();
    cocos2d::Size getCardSize();
};

#endif // __GIN_CARD_H__

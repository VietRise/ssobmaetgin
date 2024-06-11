#ifndef __GPGinDealCardLayer__
#define __GPGinDealCardLayer__

#include "cocos2d.h"
#include <vector>
#include "ui/CocosGUI.h"
#include "Define/GameDefine.h"
#include "GinCard.h"

typedef std::function<void()> PreDealCardCallback;
typedef std::function<void()> OnDealCardCallback;
typedef std::function<void()> PosDealCardCallback;

class GPGinScene;

class GPGinDealCardLayer : public cocos2d::Layer
{
private:
    cocos2d::ui::Layout* _layoutRoot;
    cocos2d::Size                                 _layoutRootSize;
    float                                         _layoutRootScale;
    cocos2d::Vector<cocos2d::Sprite*>             _listFakeCard;
    std::vector<cocos2d::Point>                   _listSlotPos;
    cocos2d::Point                                _dealCardCenter;
    std::vector<std::vector<GinCardProperty>> _listHandCardProperty;
    int                                           _oderIndexFirst;
    int                                           _cardBackId;
    int                                           _playerPos;

    PreDealCardCallback _preDealCardCallback;
    OnDealCardCallback  _onDealCardCallback;
    PosDealCardCallback _posDealCardCallback;

    void onPlayAnimation(cocos2d::CallFunc* onDealCardCallBack, cocos2d::CallFunc* posDealCardCallback);
    void onCardMoveToPlace(int fakeCardIndex, int handIndex, int slotIndex);
    void onFinishDealCard();
protected:
public:
    CC_SYNTHESIZE(GPGinScene*, _gameGin, GameGin);

    GPGinDealCardLayer(const cocos2d::Size& layoutRootsize, float layoutRootScale);
    ~GPGinDealCardLayer();

    int getNumCardForGame();
    void reset();

    void setPosDealCardLayer(const std::vector<std::vector<GinCardProperty>>& listHandCardProperty, int firstOderIndexDeal);
    void setListSlotPos(const std::vector<cocos2d::Point>& listSlotPos, int playerPos);
    void setDealCardCenter(const cocos2d::Point& center);
    void setCardBackId(int id);

    void updateFakeCardSprite();
    void moveFakeCardToCenter();

    void playDealCardAnimation();
    void setDealCardCallback(const PreDealCardCallback& preDealCardCallback, const OnDealCardCallback& onDealCardCallBack, const PosDealCardCallback& posDealCardCallback);

    virtual bool init() override;
    static GPGinDealCardLayer* createLayerForGame(const cocos2d::Size& layerSize,
                                                      float layerScale,
                                                      const cocos2d::Point& layerPosition,
                                                      const std::vector<cocos2d::Point>& listSlot,
                                                      int playerPos,
                                                      const cocos2d::Point& dealCardCenter,
                                                      int cardBackId);
};

#endif // __GPGinDealCardLayer__

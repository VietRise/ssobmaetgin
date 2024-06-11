#ifndef __GIN_USER_HAND_NODE__
#define __GIN_USER_HAND_NODE__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "cocostudio/CocoStudio.h"

#define GIN_ON_USER_HAND_SIZE cocos2d::Size(35.8f,56.8f)
#define CURVE_R 100
#define CURVE_A -40
#define CURVE_A_COLLAPSE -20

#define HAND_CARD_COLOR_ONTURN cocos2d::Color3B(255,255,255)
#define HAND_CARD_COLOR_NOTONTURN cocos2d::Color3B(255,255,255)

class GinUserHandNode : public cocos2d::Node
{
private:
	bool _onTurn;
	int _numberRemainCard;
	cocos2d::Node* _nodeHandCard;
	std::vector<cocos2d::Sprite*> _listCardOnHand;
	std::vector<cocos2d::Sprite*> _listCardCached;
	cocos2d::ui::Layout* _layoutRemainCard;
	std::vector<cocos2d::Vec2> _listCardExpandDesPos;
	std::vector<float> _listCardExpandDesRotate;
	std::vector<cocos2d::Vec2> _listCardCollapseDesPos;
	std::vector<float> _listCardCollapseDesRotate;

public:
	GinUserHandNode();
	~GinUserHandNode();

	virtual bool init();
	void setLayoutRemainCard(cocos2d::ui::Layout* layoutRemainCard);
	void playShowHandAnimation();
	void playClearHandAnimation();
	void showHand(int remainCardNum, bool anim = true);
	void clearHand();
	void setOnTurn(bool onTurn);
	void collapseHand();
	void expandHand();
	void reset();
	void setRemainCardNum(int remainCard);
	void relignHand(int remainCardNum);
	int getRemainCardNum();
	void pushAllCardOnHandBackToCachedList();

	CREATE_FUNC(GinUserHandNode);
};

#endif //__GIN_USER_HAND_NODE__
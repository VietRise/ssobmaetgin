#include "GinUserHandNode.h"
#include "Scenes/Gin/GinCard.h"
#include "Helper/GameUtils.h"
#include "Helper/fmt/format.h"
#include "Manager/SoundManager.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

GinUserHandNode::~GinUserHandNode()
{
	CC_SAFE_RELEASE_NULL(_nodeHandCard);
	for (int i = 0; i < _listCardCached.size(); i++)
	{
		CC_SAFE_RELEASE_NULL(_listCardCached.at(i));
	}
	_listCardCached.clear();
}

GinUserHandNode::GinUserHandNode()
{
	_onTurn = false;
	_numberRemainCard = 0;
	_layoutRemainCard = nullptr;
	_nodeHandCard = nullptr;
}

bool GinUserHandNode::init()
{
	if (!Node::init())
	{
		return false;
	}

	_nodeHandCard = Node::create();
	_nodeHandCard->retain();
	_nodeHandCard->setCascadeOpacityEnabled(true);
	_nodeHandCard->setCascadeColorEnabled(true);
	_nodeHandCard->setColor(HAND_CARD_COLOR_ONTURN);

//    _nodeHandCard->setVisible(false);

	int size = TOTAL_NUMBER_CARD;

	float rExpand = CURVE_R;
	float angleExpand = CURVE_A;

	float rCollapse = CURVE_R;
	float angleCollapse = CURVE_A_COLLAPSE;

	for (int i = 0; i < size; i++)
	{
		Sprite* card = Sprite::createWithSpriteFrameName(GinCardStore::getInstance()->getCardBackSpriteNameFromID(0));
		card->setVisible(false);
		card->setLocalZOrder(i);
		card->setScale(GIN_ON_USER_HAND_SIZE.width / card->getContentSize().width);
		card->retain();
		_listCardCached.push_back(card);

		//_nodeHandCard->addChild(card);

		//expand Pos & Rotate
		Point exDesPos = GameUtils::getPositionOnCircle(size, i, rExpand, angleExpand);
		float exDesRota = GameUtils::getAngleForFacingAPoint(exDesPos, Point(0, -rExpand));
		_listCardExpandDesPos.push_back(exDesPos);
		_listCardExpandDesRotate.push_back(exDesRota);

		//collapse Pos & Rotate
		Point coDesPos = GameUtils::getPositionOnCircle(size, i, rCollapse, angleCollapse);
		float coDesRota = GameUtils::getAngleForFacingAPoint(coDesPos, Point(0, -rCollapse));
		_listCardCollapseDesPos.push_back(coDesPos);
		_listCardCollapseDesRotate.push_back(coDesRota);
	}

	return true;
}

void GinUserHandNode::setLayoutRemainCard(cocos2d::ui::Layout* layoutRemainCard)
{
	_layoutRemainCard = (Layout*)layoutRemainCard->clone();
	_layoutRemainCard->setPosition(Vec2(0, -4));
	_layoutRemainCard->setLocalZOrder(50);
	_nodeHandCard->addChild(_layoutRemainCard);
}

void GinUserHandNode::playShowHandAnimation()
{
	_nodeHandCard->setColor(HAND_CARD_COLOR_NOTONTURN);
	float r = CURVE_R;
	float angle = CURVE_A_COLLAPSE;
	Point startPos = GameUtils::getPositionOnCircle(_numberRemainCard, 0, r, angle);
	float startRotate = GameUtils::getAngleForFacingAPoint(startPos, Point(0, -r));

	float time = 0.6f / (_listCardOnHand.size() - 1);
	for (int i = 0; i < _listCardOnHand.size(); i++)
	{
		Sprite* card = _listCardOnHand.at(i);
		card->setVisible(true);
//        card->setVisible(false);
		card->stopAllActions();

		Point desPos = _listCardCollapseDesPos[i];
		float desRota = _listCardCollapseDesRotate[i];

		card->setPosition(startPos);
		card->setRotation(startRotate);

		RotateTo* rotate = RotateTo::create(i * time, desRota);

		Point midPoint = GameUtils::getCenterPositionOnCircleArc(startPos, desPos, r, Point(0, -r));

		ccBezierConfig bezierConfig;
		bezierConfig.controlPoint_1 = midPoint;
		bezierConfig.controlPoint_2 = midPoint;
		bezierConfig.endPosition = desPos;

		BezierTo* bezier = BezierTo::create(i * time, bezierConfig);
		card->runAction(Spawn::create(EaseExponentialOut::create(bezier), EaseExponentialOut::create(rotate), nullptr));
	}
}

void GinUserHandNode::playClearHandAnimation()
{

}

void GinUserHandNode::showHand(int remainCardNum, bool anim/* = true*/)
{
	setRemainCardNum(remainCardNum);
	_nodeHandCard->stopAllActions();
	if (_nodeHandCard->getParent() == nullptr)
	{
		addChild(_nodeHandCard, 1);
	}
	playShowHandAnimation();
}

void GinUserHandNode::clearHand()
{
	CallFunc* remove = CallFunc::create([=]
	{
		_nodeHandCard->setOpacity(255);
		_nodeHandCard->removeFromParent();
	});

	Sequence* seq = Sequence::create(/*DelayTime::create(0.03f), */FadeOut::create(0.1f), remove, nullptr);
	_nodeHandCard->runAction(seq);
}

void GinUserHandNode::setOnTurn(bool onTurn)
{
	if (onTurn != _onTurn)
	{
		_onTurn = onTurn;
		if (onTurn == true)
		{
			expandHand();
		}
		else
		{

			collapseHand();
		}
	}
}

void GinUserHandNode::collapseHand()
{
	//_nodeHandCard->setColor(HAND_CARD_COLOR_NOTONTURN);
	int size = _listCardOnHand.size();
	float r = CURVE_R;
	float angle = CURVE_A_COLLAPSE;

	vector<Point> cardCollapseDesPos;
	float time = 0.3f / ((_listCardOnHand.size() - 1) / 2);
	for (int i = 0; i < _listCardOnHand.size(); i++)
	{
		Sprite* card = _listCardOnHand.at(i);
		card->setVisible(true);
//        card->setVisible(false);
		card->stopAllActions();

		/*	Point desPos = _listCardCollapseDesPos[i];
			float desRota = _listCardCollapseDesRotate[i];*/
		float rExpand = CURVE_R;
		float angleExpand = CURVE_A;

		float rCollapse = CURVE_R;
		float angleCollapse = CURVE_A_COLLAPSE;

		Point desPos = GameUtils::getPositionOnCircle(size, i, rCollapse, _listCardOnHand.size() * angleCollapse / TOTAL_NUMBER_CARD);
		float desRota = GameUtils::getAngleForFacingAPoint(desPos, Point(0, -rCollapse));

		Point midPoint = GameUtils::getCenterPositionOnCircleArc(desPos, card->getPosition(), r, Point(0, -r));

		ccBezierConfig bezierConfig;
		bezierConfig.controlPoint_1 = midPoint;
		bezierConfig.controlPoint_2 = midPoint;
		bezierConfig.endPosition = desPos;

		int k = abs((size - 1) / 2);
		RotateTo* rotate = RotateTo::create(k * time, desRota);

		BezierTo* bezier = BezierTo::create(k * time, bezierConfig);

		MoveTo* moveTo = MoveTo::create(k * time, desPos);
		card->setLocalZOrder(i);
		card->runAction(Spawn::create(EaseExponentialOut::create(bezier), EaseExponentialOut::create(rotate), nullptr));
	}
}

void GinUserHandNode::expandHand()
{
	_nodeHandCard->setColor(HAND_CARD_COLOR_ONTURN);
#if USE_PLAY_BOT == 1
	_nodeHandCard->setColor(Color3B(100, 255, 100));
#endif
	int size = _listCardOnHand.size();
	float r = CURVE_R;
	float angle = CURVE_A;
	float rExpand = CURVE_R;
	float angleExpand = CURVE_A;

	float rCollapse = CURVE_R;
	float angleCollapse = CURVE_A_COLLAPSE;

	vector<Point> cardCollapseDesPos;
	float time = 0.3f / ((_listCardOnHand.size() - 1) / 2);

	for (int i = 0; i < _listCardOnHand.size(); i++)
	{
		Sprite* card = _listCardOnHand.at(i);
		if (card->isVisible())
		{
			//card->setVisible(true);
			card->stopAllActions();

			//Point desPos = _listCardExpandDesPos[i];
			//float desRota = _listCardExpandDesRotate[i];

			//expand Pos & Rotate

			Point desPos = GameUtils::getPositionOnCircle(size, i, rExpand, _listCardOnHand.size() * angleExpand / TOTAL_NUMBER_CARD);
			float desRota = GameUtils::getAngleForFacingAPoint(desPos, Point(0, -rExpand));

			Point midPoint = GameUtils::getCenterPositionOnCircleArc(desPos, card->getPosition(), r, Point(0, -r));

			ccBezierConfig bezierConfig;
			bezierConfig.controlPoint_1 = midPoint;
			bezierConfig.controlPoint_2 = midPoint;
			bezierConfig.endPosition = desPos;

			int k = abs((size - 1) / 2);
			RotateTo* rotate = RotateTo::create(k * time, desRota);

			BezierTo* bezier = BezierTo::create(k * time, bezierConfig);

			MoveTo* moveTo = MoveTo::create(k * time, desPos);
			card->runAction(Spawn::create(EaseExponentialOut::create(bezier), EaseExponentialOut::create(rotate), nullptr));

		}
	}
}

void GinUserHandNode::reset()
{
	_onTurn = false;
	_numberRemainCard = 0;
	pushAllCardOnHandBackToCachedList();
	_nodeHandCard->removeFromParent();
	_nodeHandCard->stopAllActions();
}

void GinUserHandNode::relignHand(int remainCardNum)
{
	setRemainCardNum(remainCardNum);
	if (_onTurn == true)
	{
		expandHand();
	}
	else
	{

		collapseHand();
	}
}

void GinUserHandNode::setRemainCardNum(int remainCard)
{
	_numberRemainCard = remainCard;
	if (_numberRemainCard <= 0)
	{
		_layoutRemainCard->setVisible(false);
	}
	else
	{
//		_layoutRemainCard->setVisible(true);
        _layoutRemainCard->setVisible(false);

		auto label = ((TextBMFont*)_layoutRemainCard->getChildByName("Label"));
		label->setString(fmt::format("{0}", _numberRemainCard));
		label->setColor(Color3B(255, 255, 255));
		if (_numberRemainCard <= 2)
		{
			label->setColor(Color3B(221, 20, 20));
			// SoundManager::getInstance()->playSoundEffect(SoundEvent::BIG2_WARNING);
		}
	}
	
	int size = _listCardOnHand.size();
	if (size < _numberRemainCard)
	{
		for (int i = 0; i < _numberRemainCard - size; i++)
		{
			// Popback card from cached list
			Sprite* card = _listCardCached.back();
			_listCardCached.pop_back();
            card->setVisible(true);

			// Add card to list card on hand
			_listCardOnHand.push_back(card);
			_nodeHandCard->addChild(card);
		}
	}
	else if (size > _numberRemainCard)
	{
		for (int i = 0; i < size - _numberRemainCard; i++)
		{
			if (_listCardOnHand.size() > 0)
			{
				// Remove card on hand
				Sprite* card = _listCardOnHand.back();
				_listCardOnHand.pop_back();
				card->removeFromParent();
                card->setVisible(false);
				// Add it to back list cached card
				_listCardCached.push_back(card);
			}
		}
	}

    for(int i=0; i<_listCardOnHand.size(); ++i)
    {
        _listCardOnHand[i]->setLocalZOrder(i);
    }
	
}

int GinUserHandNode::getRemainCardNum()
{
	return _numberRemainCard;
}

void GinUserHandNode::pushAllCardOnHandBackToCachedList()
{
	for (Sprite* sprite : _listCardOnHand)
	{
		sprite->removeFromParent();
		_listCardCached.push_back(sprite);
	}
	_listCardOnHand.clear();
}
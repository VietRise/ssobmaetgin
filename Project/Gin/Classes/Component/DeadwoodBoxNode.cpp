#include "DeadwoodBoxNode.h"
#include "Helper/GameUtils.h"
#include "Define/GameDefine.h"
#include "Define/ResourcesDefine.h"
#include "Manager/SoundManager.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"

USING_NS_CC;
using namespace ui;
using namespace std;

DeadwoodBoxNode::~DeadwoodBoxNode()
{

}

DeadwoodBoxNode::DeadwoodBoxNode()
{
	_rootLayout = nullptr;
	_textNumberScore = nullptr;
    _textTitle = nullptr;
	_score      = 0;
    _knockScore = 0;
}

bool DeadwoodBoxNode::init()
{
	if (!Node::init())
		return false;

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    Node* node = CSLoader::createNode("ui/Common/DeadwoodBox.csb");
    this->_rootLayout = static_cast<Layout*> (node->getChildByName("Root"));
    this->_rootLayout->removeFromParent();
    this->_rootLayout->setPosition(Vec2(0.0f, 0.0f));

    addChild(this->_rootLayout);

	this->_textNumberScore = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_rootLayout, "Label"));
    this->_textTitle = static_cast<Text*>(Helper::seekWidgetByName(this->_rootLayout, "TextTitle"));

    this->_textTitle->setText(GET_TEXT_BY_ID(TXT_DEADWOOD));
	return true;
}

void DeadwoodBoxNode::setScore(int score)
{
	_score = score;
    this->updateText();
}

int DeadwoodBoxNode::getScore()
{
	return _score;
}

void DeadwoodBoxNode::setKnockScore(int knockScore)
{
    _knockScore = knockScore;
    this->updateText();
}

int DeadwoodBoxNode::getKnockScore()
{
    return _knockScore;
}

void DeadwoodBoxNode::show(int score, int knockScore)
{
    _score      = score;
    _knockScore = knockScore;
    this->updateText();
	this->setVisible(true);
}

void DeadwoodBoxNode::hide()
{
    this->setVisible(false);
}

void DeadwoodBoxNode::setScoreAndKnockScore(int score, int knockScore)
{
    this->_score = score;
    this->_knockScore = knockScore;
    this->updateText();
}
void DeadwoodBoxNode::updateText()
{
    if(this->_knockScore >= 0)
    {
        _textNumberScore->setString(StringUtils::format("%d/%d", _score, _knockScore));
    }
    else
    {
        _textNumberScore->setString(StringUtils::format("%d/-", _score));
    }
}

#include "ScoreBoxNode.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace std;

ScoreBoxNode::~ScoreBoxNode()
{
}

ScoreBoxNode::ScoreBoxNode()
{
    _rootLayout      = nullptr;
    _textNumberScore = nullptr;
    _score           = 0;
}

bool ScoreBoxNode::init()
{
    if (!Node::init())
        return false;

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    Node* node = CSLoader::createNode("ui/Common/ScoreBox.csb");
    this->_rootLayout = static_cast<Layout*> (node->getChildByName("Root"));
    this->_rootLayout->removeFromParent();
    this->_rootLayout->setPosition(Vec2(0.0f, 0.0f));

    addChild(this->_rootLayout);

    this->_textNumberScore = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_rootLayout, "Label"));
    this->_textTitleScore = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_rootLayout, "Title"));
    this->_layoutNegative  = static_cast<Layout*>(Helper::seekWidgetByName(this->_rootLayout, "Negative"));
    this->_layoutNone      = static_cast<Layout*>(Helper::seekWidgetByName(this->_rootLayout, "None"));
    this->_layoutPositive  = static_cast<Layout*>(Helper::seekWidgetByName(this->_rootLayout, "Positive"));
    this->_layoutScoreUser = static_cast<Layout*>(Helper::seekWidgetByName(this->_rootLayout, "User"));
    return true;
}

void ScoreBoxNode::setScore(int score)
{
    score = (score>=0)?score:0;

    _score = score;
    _textNumberScore->setString(StringUtils::toString(score));
}

int ScoreBoxNode::getScore()
{
    return _score;
}

void ScoreBoxNode::show(bool show, int value, GinResultType type)
{
    this->_layoutNegative->setVisible(false);
    this->_layoutNone->setVisible(false);
    this->_layoutPositive->setVisible(false);
    this->_layoutScoreUser->setVisible(false);

	if (type == GinResultType::GIN_RESULT_TYPE_NONE 
		|| type == GinResultType::GIN_RESULT_TYPE_LOSE_GIN)
    {
        this->_layoutNone->setVisible(true);
    }
    else
    {
        if (value > 0)
            this->_layoutPositive->setVisible(true);
        else if (value < 0)
            this->_layoutNegative->setVisible(true);
        else
            this->_layoutNone->setVisible(true);
    }
    this->setVisible(show);
}

void ScoreBoxNode::showUserScore(bool show, int value)
{
    this->_layoutNegative->setVisible(false);
    this->_layoutNone->setVisible(false);
    this->_layoutPositive->setVisible(false);
    this->_layoutScoreUser->setVisible(true);
    this->setVisible(show);
}

void ScoreBoxNode::setTitle(std::string text)
{
    this->_textTitleScore->setString(text);
}

void ScoreBoxNode::setLayoutColor(int value)
{
    if (value > 0)
        this->_layoutPositive->setVisible(true);
    else if (value < 0)
        this->_layoutNegative->setVisible(true);
    else
        this->_layoutNone->setVisible(true);
}
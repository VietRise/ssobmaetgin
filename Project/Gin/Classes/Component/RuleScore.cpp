#include "RuleScore.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace ui;
using namespace std;

std::vector<std::pair<std::string, std::string>>                              RuleScore::_listTextCardPoint;
std::vector<std::pair<std::pair<std::string, std::string>, cocos2d::Color3B>> RuleScore::_listTextScoring;

RuleScore::RuleScore()
{
//    _listTextCardPoint.clear();
//    _listTextCardPoint.push_back({ GET_TEXT_BY_ID(TXT_RULE_SCORING_CARD_POINTS_NAME_1), GET_TEXT_BY_ID(TXT_RULE_SCORING_CARD_POINTS_VALUE_1) });
//    _listTextCardPoint.push_back({ GET_TEXT_BY_ID(TXT_RULE_SCORING_CARD_POINTS_NAME_2), GET_TEXT_BY_ID(TXT_RULE_SCORING_CARD_POINTS_VALUE_2) });
//
//    _listTextScoring.clear();
//    _listTextScoring.push_back({{ GET_TEXT_BY_ID(TXT_RULE_SCORING_NORMAL_WIN_TITLE), GET_TEXT_BY_ID(TXT_RULE_SCORING_NORMAL_WIN_VALUE) }, COLOR_POSITIVE_NUMBER_3 });
//	_listTextScoring.push_back({ {GET_TEXT_BY_ID(TXT_RULE_SCORING_GIN_TITLE), GET_TEXT_BY_ID(TXT_RULE_SCORING_GIN_VALUE) }, COLOR_POSITIVE_NUMBER_3 });
//    _listTextScoring.push_back({{ GET_TEXT_BY_ID(TXT_RULE_SCORING_WIN_CHALLENGE_TITLE), GET_TEXT_BY_ID(TXT_RULE_SCORING_WIN_CHALLENGE_VALUE) }, COLOR_POSITIVE_NUMBER_3 });
//    _listTextScoring.push_back({{ GET_TEXT_BY_ID(TXT_RULE_SCORING_SECRET_MELDS_TITLE), GET_TEXT_BY_ID(TXT_RULE_SCORING_SECRET_MELDS_VALUE) }, COLOR_POSITIVE_NUMBER_3 });
//    _listTextScoring.push_back({{ GET_TEXT_BY_ID(TXT_RULE_SCORING_BONUS_CARDS_TITLE), GET_TEXT_BY_ID(TXT_RULE_SCORING_BONUS_CARDS_VALUE) }, COLOR_POSITIVE_NUMBER_3 });
//    _listTextScoring.push_back({{ GET_TEXT_BY_ID(TXT_RULE_SCORING_FOLD_TITLE), GET_TEXT_BY_ID(TXT_RULE_SCORING_FOLD_VALUE) }, COLOR_NAGATIVE_NUMBER_3 });
//    _listTextScoring.push_back({{ GET_TEXT_BY_ID(TXT_RULE_SCORING_BURNED_TITLE), GET_TEXT_BY_ID(TXT_RULE_SCORING_BURNED_VALUE) }, COLOR_NAGATIVE_NUMBER_3 });
}

RuleScore::~RuleScore()
{
}

bool RuleScore::init()
{
    if (!Layout::init())
        return false;
    initLayout();
    setCascadeOpacityEnabled(true);
    setContent();
    return true;
}

void RuleScore::initLayout()
{
//    screenSize = Director::getInstance()->getWinSize();
//    this->setContentSize(screenSize);
//    Node* node = CSLoader::createNode("ui/Common/RuleScore.csb");
//
//    Node* child = node->getChildByName("Root");
//    child->removeFromParent();
//
//    this->_layoutRoot = static_cast<Layout*>(child);
//    this->addChild(_layoutRoot);
//    Helper::doLayout(this);
//
//    this->_labelTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelScoringTitle"));
//    setCascadeColorEnabled(true);
//    setCascadeOpacityEnabled(true);
//    setName(GET_TEXT_BY_ID(TXT_RULE_SCORING_TITLE));
}

void RuleScore::setContent()
{
//    auto listView = (ListView*) Helper::seekWidgetByName(_layoutRoot, "ListViewContent");
//    listView->setClippingEnabled(true);
//    this->_labelTitle->setString(GET_TEXT_BY_ID(TXT_RULE_SCORING_TITLE));
//    // Layout Card Points
//    auto layoutCardPoints = static_cast<Layout*>(Helper::seekWidgetByName(listView, "PanelCardPoints"));
//    auto textCardPoints   = static_cast<Text*>(Helper::seekWidgetByName(layoutCardPoints, "LabelName"));
//    textCardPoints->setString(GET_TEXT_BY_ID(TXT_RULE_SCORING_CARD_POINTS_TITLE));
//    for (int i             = 0; i < _listTextCardPoint.size(); i++)
//    {
//        auto layoutCardPointsLine = static_cast<Layout*>(Helper::seekWidgetByName(layoutCardPoints, fmt::format("PanelCardPoints{0}", i + 1)));
//        ((Text*) layoutCardPointsLine->getChildByName("TextName"))->setString(_listTextCardPoint.at(i).first);
//        ((Text*) layoutCardPointsLine->getChildByName("TextValue"))->setString(_listTextCardPoint.at(i).second);
//    }
//    // Layout Scoring
//    auto     layoutScoring = static_cast<Layout*>(Helper::seekWidgetByName(listView, "PanelScoring"));
//    auto     textScoring   = static_cast<Text*>(Helper::seekWidgetByName(layoutScoring, "LabelName"));
//    textScoring->setString(GET_TEXT_BY_ID(TXT_RULE_SCORING_SCORE_TITLE));
//    for (int i = 0; i < _listTextScoring.size(); i++)
//    {
//        auto layoutScoringLine = static_cast<Layout*>(Helper::seekWidgetByName(layoutScoring, fmt::format("Panel{0}", i + 1)));
//        ((Text*) layoutScoringLine->getChildByName("TextName"))->setString(_listTextScoring.at(i).first.first);
//        ((TextBMFont*) layoutScoringLine->getChildByName("TextValue"))->setString(_listTextScoring.at(i).first.second);
//        ((TextBMFont*) layoutScoringLine->getChildByName("TextValue"))->setColor(_listTextScoring.at(i).second);
//    }
}

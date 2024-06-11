#include "RuleDetails.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"

USING_NS_CC;
using namespace ui;
using namespace std;

RuleDetails::RuleDetails()
{
}

RuleDetails::~RuleDetails()
{
}

bool RuleDetails::init()
{
    if (!Layout::init())
        return false;
    initLayout();
    setCascadeOpacityEnabled(true);
    setContent();
    return true;
}

void RuleDetails::initLayout()
{
    screenSize = Director::getInstance()->getWinSize();
    this->setContentSize(screenSize);
    Node* node = CSLoader::createNode("ui/Common/RuleDetails.csb");

    Node* child = node->getChildByName("Root");
    child->removeFromParent();

    this->_layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot);
    Helper::doLayout(this);

    this->_labelTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelDetails"));

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    setName(GET_TEXT_BY_ID(TXT_RULE_DETAILS_TITLE));
}

void RuleDetails::setContent()
{
    auto listView = (ListView*) Helper::seekWidgetByName(_layoutRoot, "ListViewContent");
    listView->setClippingEnabled(true);
    this->_labelTitle->setString(GET_TEXT_BY_ID(TXT_RULE_DETAILS_TITLE));

    this->setSectionData(listView, "PanelGinDeck", TXT_RULE_GIN_DECK_TITLE, TXT_RULE_GIN_DECK);
    this->setSectionData(listView, "PanelGinObjective", TXT_RULE_GIN_OBJECTIVE_TITLE, TXT_RULE_GIN_OBJECTIVE);
    this->setSectionData(listView, "PanelGinDeadWood", TXT_RULE_GIN_DEADWOOD_TITLE, TXT_RULE_GIN_DEADWOOD);
    this->setSectionData(listView, "PanelGinDeal", TXT_RULE_GIN_DEAL_TITLE, TXT_RULE_GIN_DEAL);
    this->setSectionData(listView, "PanelGinKnocking", TXT_RULE_GIN_KNOCKING_TITLE, TXT_RULE_GIN_KNOCKING);
    this->setSectionData(listView, "PanelGinGin", TXT_RULE_GIN_GIN_TITLE, TXT_RULE_GIN_GIN);
    this->setSectionData(listView, "PanelGinBigGin", TXT_RULE_GIN_BIGGIN_TITLE, TXT_RULE_GIN_BIGGIN);
    this->setSectionData(listView, "PanelGinUndercut", TXT_RULE_GIN_UNDERCUT_TITLE, TXT_RULE_GIN_UNDERCUT);
    this->setSectionData(listView, "PanelGinEndingRound", TXT_RULE_GIN_ENDING_ROUND_TITLE, TXT_RULE_GIN_ENDING_ROUND);
    this->setSectionData(listView, "PanelGinEndingGame", TXT_RULE_GIN_ENDING_GAME_TITLE, TXT_RULE_GIN_ENDING_GAME);
    this->setSectionData(listView, "PanelGinStraightGin", TXT_RULE_STRAIGHT_GIN_TITLE, TXT_RULE_STRAIGHT_GIN);
    this->setSectionData(listView, "PanelGinOklahomaGin", TXT_RULE_OKLAHOMA_GIN_TITLE, TXT_RULE_OKLAHOMA_GIN);
}

void RuleDetails::setSectionData(cocos2d::ui::Layout* root, std::string layoutName, int titleTextId, int descriptionTextId)
{
    auto layout = static_cast<Layout*>(Helper::seekWidgetByName(root, layoutName));

    auto titleText = static_cast<Text*>(Helper::seekWidgetByName(layout, "LabelTitle"));
    auto descriptionText = static_cast<Text*>(Helper::seekWidgetByName(layout, "LabelDescription"));

    titleText->setString(GET_TEXT_BY_ID(titleTextId));
    descriptionText->setString(GET_TEXT_BY_ID(descriptionTextId));
}

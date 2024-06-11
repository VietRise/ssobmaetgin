#include "MoneyBoxNode.h"
#include "Helper/GameUtils.h"
#include "Define/GameDefine.h"
#include "Define/ResourcesDefine.h"
#include "Manager/SoundManager.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace std;

MoneyBoxNode::MoneyBoxNode()
{
    _minus = true;
}

MoneyBoxNode::~MoneyBoxNode()
{
}

bool MoneyBoxNode::init()
{
    if (!Node::init())
        return false;
    Node* node = CSLoader::createNode("ui/Common/MoneyBox.csb");
    this->_rootLayout = static_cast<Layout*> (node->getChildByName("Root"));
    this->_rootLayout->removeFromParent();
    this->_rootLayout->setPosition(Vec2(0.0f, 0.0f));
    addChild(this->_rootLayout);
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    return true;
}

void MoneyBoxNode::setMoney(double money)
{
    auto positive = (Layout*) Helper::seekWidgetByName(_rootLayout, "Positive");
    auto negative = (Layout*) Helper::seekWidgetByName(_rootLayout, "Negative");

    positive->setVisible(false);
    negative->setVisible(false);
    Layout* layout = nullptr;

    if (money > 0)
    {
        positive->setVisible(true);
        negative->setVisible(false);
        layout = positive;
    }
    else
    {
        positive->setVisible(false);
        negative->setVisible(true);
        layout = negative;
    }

    auto        textMoney   = (TextBMFont*) Helper::seekWidgetByName(layout, "TextMoney");
    std::string moneyString = GameUtils::MoneyFormat(money);
    if (!_minus && money < 0)
        moneyString = GameUtils::MoneyFormat(-1 * money);
    if (money > 0)
        moneyString = "+" + moneyString;
    textMoney->setString(moneyString);
    this->_rootLayout->setContentSize(Size(textMoney->getContentSize().width * textMoney->getScale() + 50, this->_rootLayout->getContentSize().height));
    Helper::doLayout(this->_rootLayout);
}

void MoneyBoxNode::show(bool show)
{
    setVisible(show);
}

void MoneyBoxNode::reset()
{
    setScale(1.0f);
}

void MoneyBoxNode::playBonusRate(float bonusRate)
{
    ScaleBy* scaleBy = ScaleBy::create(0.15f, 1.3f);
    this->runAction(Sequence::create(EaseElasticOut::create(scaleBy), scaleBy->reverse(), nullptr));
    std::string strScore = StringUtils::format("x%.0f", bonusRate);
    Size        sizeBg   = this->_rootLayout->getContentSize();
    TextBMFont* textEffectMultiplicationScore = TextBMFont::create(strScore, Resources::BMFont::BMFont_Normal);
    textEffectMultiplicationScore->setAnchorPoint(Vec2(0.5f, 0.5f));
    textEffectMultiplicationScore->setPosition(Vec2(sizeBg.width / 2.0f, sizeBg.height / 2.5f));
    textEffectMultiplicationScore->setOpacity(255);
    textEffectMultiplicationScore->setVisible(true);
    textEffectMultiplicationScore->setColor(Color3B(255, 195, 81));
    this->_rootLayout->addChild(textEffectMultiplicationScore);
    MoveBy  * moveBy  = MoveBy::create(1.0f, Vec3(0, 75, 0));
    FadeOut * fadeOut = FadeOut::create(1.0f);
    CallFunc* finish  = CallFunc::create([=] {
        textEffectMultiplicationScore->removeFromParent();
    });
    SoundManager::playSoundEffect(SoundEvent::DOUBLE_MONEY);
    textEffectMultiplicationScore->runAction(Sequence::create(Spawn::create(moveBy, fadeOut, nullptr), finish, nullptr));
}

void MoneyBoxNode::setMinus(bool minus)
{
    _minus = minus;
}
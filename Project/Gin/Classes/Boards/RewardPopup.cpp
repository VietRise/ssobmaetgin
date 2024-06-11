#include "RewardPopup.h"
#include "Define/GameDefine.h"
#include "Manager/MyActionsManager.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace ui;
using namespace std;

RewardPopup::RewardPopup()
{
    _claimRewardCallback = nullptr;
    _applyTouchOutClose  = false;
    _applyBackKey        = false;
}

RewardPopup::~RewardPopup()
{
}

bool RewardPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void RewardPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/RewardPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));

    this->_buttonGetReward = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonGetGold"));
    this->_buttonGetReward->addClickEventListener(CC_CALLBACK_1(RewardPopup::onGetRewardButtonClick, this));

    this->_textRewardContent = static_cast<TextBMFont*>(this->_buttonGetReward->getChildByName("Label"));

    SoundManager::playSoundEffect(SoundEvent::REWARD_MONEY);
}

void RewardPopup::setClaimRewardCallback(const ClaimRewardCallback& claimRewardCallback)
{
    _claimRewardCallback = claimRewardCallback;
}

void RewardPopup::setRewardContent(const std::string& rewardContent)
{
    this->_textRewardContent->setString(rewardContent);
    this->_buttonGetReward->setEnabled(true);
}

void RewardPopup::setContent(const std::string& content)
{
    this->_textContent->setString(content);
}

void RewardPopup::onGetRewardButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    SoundManager::playSoundEffect(SoundEvent::CLAIM_MONEY);
    if (_claimRewardCallback != nullptr)
        _claimRewardCallback();
    this->_buttonGetReward->setEnabled(false);
    close();
}

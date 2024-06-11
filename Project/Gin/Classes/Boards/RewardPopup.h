#ifndef __REWARD_POPUP_H__
#define __REWARD_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>

typedef std::function<void()> ClaimRewardCallback;

class RewardPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text      * _textContent;
    cocos2d::ui::Button    * _buttonGetReward;
    cocos2d::ui::TextBMFont* _textRewardContent;

    ClaimRewardCallback _claimRewardCallback;

    void onGetRewardButtonClick(Ref* sender);
protected:
public:
    RewardPopup();
    ~RewardPopup();

    virtual bool init() override;
    void initScreen();

    void setRewardContent(const std::string& rewardContent);
    void setContent(const std::string& content);
    void setClaimRewardCallback(const ClaimRewardCallback& claimMoneyCallback);

    CREATE_FUNC(RewardPopup);
};

#endif //__REWARD_POPUP_H__
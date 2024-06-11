#ifndef __VIDEO_REWARDS_POPUP_H__
#define __VIDEO_REWARDS_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>
#include "Helper/JsonHelper.h"
#include "Define/ServerClientMessageDefine.h"

typedef std::function<void()> ActionLotteryAnimCallback;

class VideoRewardsPopup;

class RewardItem : public cocos2d::ui::Layout
{
private:
    double _availableAt;
    double _value;
    bool   _isLottery;
    int    _countdown;
    int    _countdownLoadVideo;
    int    _countdownAfterRequestVideo;

    VideoRewardsPopup  * _videoRewardPopup;
    cocos2d::ui::Layout* _rootNode;

    cocos2d::ui::ImageView * _imageBackground;
    cocos2d::ui::ImageView * _imageIcon;
    cocos2d::ui::ImageView * _imageCheck;
    cocos2d::ui::ImageView * _imageArrow;
    cocos2d::ui::Text      * _text1;
    cocos2d::ui::TextBMFont* _text2;
    cocos2d::ui::Text      * _text3;
    cocos2d::ui::Button    * _buttonVideo;
    cocos2d::ui::Text      * _textButtonVideo;
    cocos2d::ui::Text      * _textButtonVideoNotice;
    cocos2d::ui::TextBMFont* _textCountDown;
    cocos2d::ui::Button    * _buttonClaim;
    cocos2d::ui::Text      * _textButtonClaim;

    static std::map<int, std::string> _mapRewardImages;
protected:
public:
    RewardItem();
    virtual ~RewardItem();

    virtual bool init() override;
    CREATE_FUNC(RewardItem);

    void setData(double value, int step, int currentStep, double availableAt, bool skippable, bool isLottery);
    void onButtonClick(cocos2d::Ref* sender);
    void setButtonVideoEnable(bool enable);
    void setButtonClaimEnable(bool enable);
    void setButtonVideoVisible(bool visible);
    void setButtonClaimVisible(bool visible);
    void tick(float dt);

    static RewardItem* createRewardItem(VideoRewardsPopup* popup, cocos2d::ui::Layout* item);
};

class VideoRewardsPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout    * _root;
    cocos2d::ui::Button    * _buttonClose;
    cocos2d::ui::Layout    * _panel_content;
    cocos2d::ui::Layout    * _panel_money;
    cocos2d::ui::TextBMFont* _text_user_money;
    cocos2d::ui::Layout    * _panel_info;
    cocos2d::ui::Layout    * _panel_item;
    cocos2d::ui::Layout    * _panel_item_lottery;
    cocos2d::ui::ListView  * _list_view_item;
    cocos2d::ui::Text      * _text_notice_1;
    cocos2d::ui::Text      * _text_notice_2;
    cocos2d::ui::Text      * _text_notice_3;
    cocos2d::ui::Text      * _text_notice_4;
    cocos2d::ui::Text      * _text_notice_5;
    cocos2d::ui::Text      * _text_notice_6;
    cocos2d::ui::Layout    * _panel_reset_info;
    cocos2d::ui::TextBMFont* _text_reset_notice;
    cocos2d::ui::TextBMFont* _text_reset_coundown;

    cocos2d::ui::Layout    * _panel_lottery;
    cocos2d::ui::ImageView * _image_box;
    cocos2d::ui::Layout    * _panel_reward;
    cocos2d::ui::ImageView * _image_background;
    cocos2d::ui::TextBMFont* _text_value;
    cocos2d::ui::Button    * _button_open_reward;
    cocos2d::ui::Text      * _text_button_open;
    cocos2d::ui::Layout    * _layoutEventDecor;

    double                    _resetAt;
    int                       _currentStep;
    int                       _rewardLevel;
    rapidjson::Document       _data;
    ActionLotteryAnimCallback _loterryAnimCallback;
    bool                      _isPlayingAnimation = false;

    cocos2d::FiniteTimeAction* createShowAction(float delay = 0);
    void onButtonClick(cocos2d::Ref* sender);
    virtual void changeLanguage(Language language) override;
protected:
public:
    VideoRewardsPopup();
    ~VideoRewardsPopup();

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    void initScreen();
    virtual void open() override;
    virtual void close(bool forceClose = false) override;

    void setData(const rapidjson::Document& data, bool playAnimation = false);
    void onServerResponse(MessageID messageId, const rapidjson::Document& data);

    void playLotteryShowAnimation(double value, const ActionLotteryAnimCallback& callback);
    void playLotteryOpenAnimation();
    void playInfoListAppearAnimation(float delayStart, float delayUnit);
    void tick(float dt);
    void checkAndEnableEventDecorLayer();
    void checkToRemindUserTurnOnNetwork();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

    CREATE_FUNC(VideoRewardsPopup);
};

#endif

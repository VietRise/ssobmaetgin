#ifndef __GET_MONEY_TO_PLAY_POPUP_H__
#define __GET_MONEY_TO_PLAY_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>

class GetMoneyToPlayPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text      * _textContent;
    cocos2d::ui::Button    * _buttonViewAds;
    cocos2d::ui::Button    * _buttonGetGold;
    cocos2d::ui::Button    * _buttonClose;
    cocos2d::ui::TextBMFont* _labelGold;
    cocos2d::ui::TextBMFont* _labelViewAds;
    cocos2d::ui::Text      * _labelViewAdsHint;
    cocos2d::ui::Button    * _buttonVideoBonusReward;
    cocos2d::ui::Text      * _textBMVideoBonusReward;
    cocos2d::ui::ImageView * _imageVideoBonusReward;
    cocos2d::ui::ImageView * _imageVideoBonusRewardGlow0;
    cocos2d::ui::ImageView * _imageVideoBonusRewardGlow1;

    float _imageVideoBonusRewardScaleX;
    float _imageVideoBonusRewardScaleY;

    void onViewAdsButtonClick(Ref* sender);
    void onGetGoldButtonClick(Ref* sender);
    void onVideoRewardButtonClick(Ref* sender);

    void onButtonClick(Ref* sender);

    std::string defaultFreeGoldText;
protected:
public:
    GetMoneyToPlayPopup();
    ~GetMoneyToPlayPopup();

    virtual bool init() override;
    void initScreen();
    void setContent(const std::string& content);
    void setEnableViewAds(bool visible);
    void setEnableFreeGold(bool visible);
    void setTextOfGetGold(const std::string& text);
    void setTextOfViewAds(const std::string& text);
    virtual void changeLanguage(Language language) override;
    void onVideoRewardEnable(bool enable, double availableAt);

    CREATE_FUNC(GetMoneyToPlayPopup);
};

#endif //__GET_MONEY_TO_PLAY_POPUP_H__
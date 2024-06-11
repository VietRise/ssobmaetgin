#ifndef __RATING_POPUP_H__
#define __RATING_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include "Define/ServerClientMessageDefine.h"

class RatingPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text  * _textTitle;
    cocos2d::ui::Text  * _textContent;
    cocos2d::ui::Button* _buttonSubmit;
    cocos2d::ui::Text  * _textSubmit;
    cocos2d::ui::Button* _buttonLater;
    std::vector<cocos2d::ui::ImageView*> _listImageViewStar;

    void onSubmitButtonClick(Ref* sender);
    void onLaterButtonClick(Ref* sender);
    void onStarImageViewClick(Ref* sender);
    void onSubmitResponse(const rapidjson::Document& data);

    int         _chooseStar;
    bool        _isPlayAnimation;
    std::string _conditionName;
protected:
public:
CC_SYNTHESIZE(int, _stateRating, StateRating);

    ~RatingPopup();
    RatingPopup();

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void close(bool forceClose = false) override;
    void closeWithAction(cocos2d::CallFunc* actionBeforeEnd);
    void setConditionOpen(const std::string& conditionName);
    void initScreen();
    void startAnimationStar();
    void stopAnimationStar();
    void onServerResponse(MessageID messageId, const rapidjson::Document& data);
    CREATE_FUNC(RatingPopup);
};

#endif //__RATING_POPUP_H__

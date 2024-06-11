#ifndef __CONTINUE_TO_PLAY_CONFIRM_POPUP_H__
#define __CONTINUE_TO_PLAY_CONFIRM_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>
#include "Define/InGameDefine.h"

#define CONTINUE_TO_PLAY_QUIT_CALLBACK_SELECTOR(_SELECTOR)  std::bind(&_SELECTOR, this)
typedef std::function<void()>                     QuitCallback;

#define CONTINUE_TO_PLAY_CONTINUE_CALLBACK_SELECTOR(_SELECTOR)  std::bind(&_SELECTOR, this, std::placeholders::_1, std::placeholders::_2)
typedef std::function<void(double bet, int targetScore)> ContinueCallback;

class ContinueToPlayConfirmPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout   * _layoutContent;
    cocos2d::ui::Slider   * _sliderBet;
    cocos2d::ui::Button   * _buttonContinue;
    cocos2d::ui::Button   * _buttonQuit;
    cocos2d::ui::Button   * _buttonClose;
    cocos2d::ui::Text     * _textContinueButton;
    cocos2d::ui::Text     * _textQuitButton;
    cocos2d::ui::ImageView* _progressBall;
    cocos2d::ui::Layout   * _layoutBetPoint;

    cocos2d::ui::Slider     * _sliderTargetScore;
    cocos2d::ui::Layout     * _layoutScore;
    cocos2d::ui::ImageView  * _imageTargetScoreSliderController;

    double                                _bet;
    int                                   _targetScore;
    bool                                  _draggingSlider;
    bool                                  _draggingTargetScoreSlider;
    CityType                              _cityType;
    cocos2d::Vector<cocos2d::ui::Layout*> _listLayoutBetPoint;
    std::vector<cocos2d::Point>           _listLayoutBetPointPos;
    std::vector<double>                   _betRange;
    std::vector<int>                      _targetScoreRange;
    cocos2d::Vector<cocos2d::ui::Layout*> _listLayoutTargetScore;
    std::vector<cocos2d::Point>           _listLayoutTargetScorePos;

    QuitCallback     _quitCallback;
    ContinueCallback _continueCallback;
    cocos2d::Size    _sliderOriginalSize;
    void onButtonClick(cocos2d::Ref* sender);
    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event);
    void onSliderTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void updateUIAcordingPercent(cocos2d::ui::Slider* slider);
    std::vector<double> getBetRange(double money);
    std::pair<double, std::vector<double>> getBetRangeSoFar(double money);
    void selectBetRange(int index);
    void updateBetMoney(int index);
    int getBetRangeFromPercent(float percent);
    virtual void changeLanguage(Language language) override;
    int getBetRangeIndex(double betMoney);
    void updateBetRange();
    void updateBetRangeSoFar();
    cocos2d::FiniteTimeAction* getZoomInActionBetPoint();
    cocos2d::FiniteTimeAction* getActionTargetScore(float scale, float opacity, float duration, bool isScaleUp);
    void playTargetScoreAnim(cocos2d::ui::Layout* layout, float scale, float opacity);

    void updateTargetScoreUIAcordingPercent(cocos2d::ui::Slider* slider);
    std::vector<int> getTargetScoreRange();
    void selectTargetScoreRange(int index);
    void updateTargetScore(int index);
    int getTargetScoreRangeFromPercent(float percent);
    int getTargetScoreRangeIndex(int targetScore);
    void updateTargetScoreRange();

    float getJackpotBetMultiply();
    float getMaxMultiplyValue(int targetScore);

    void sliderTargetScoreEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event);
    void onSliderTargetScoreTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

protected:
public:
CC_SYNTHESIZE(int, _rule, _rule);
    ContinueToPlayConfirmPopup();
    ~ContinueToPlayConfirmPopup();

    virtual bool init() override;
    void initScreen();
    void initValue(CityType city, bool force, int bet, int targetScore);
    void setQuitCallback(const QuitCallback& quitCallback);
    void setContinueCallback(const ContinueCallback& continueCallback);

    CREATE_FUNC(ContinueToPlayConfirmPopup);
};

#endif //__CONTINUE_TO_PLAY_CONFIRM_POPUP_H__
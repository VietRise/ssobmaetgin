#ifndef __LOBBY_TICKET_FLIGHT_BOARD_H__
#define __LOBBY_TICKET_FLIGHT_BOARD_H__

#include "InterfacePopupBoard.h"
#include "Define/InGameDefine.h"

class LobbyScene;

class LobbyPlace;

class LobbyTicketFlightBoard : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout   * _layoutContent;
    cocos2d::ui::Slider   * _sliderBet;
    cocos2d::ui::Button   * _buttonFlightNow;
    cocos2d::ui::Text     * _textBMFlightNow;
    cocos2d::ui::Button   * _buttonClose;
    cocos2d::ui::ImageView* _progressBall;
    cocos2d::ui::Text     * _textLuckyValue;
    cocos2d::ui::Text     * _textBMTitle;
    cocos2d::ui::Text     * _textTableTitle;
    cocos2d::ui::Text     * _textTableHint;
    cocos2d::ui::Layout   * _layoutBetPoint;
    cocos2d::ui::Layout   * _layoutTable;
    cocos2d::ui::ImageView* _imageTable;
    cocos2d::ui::Layout   * _layoutLucky;
    cocos2d::Size _sliderOriginalSize;
    cocos2d::ui::Slider   * _sliderTargetScore;
    cocos2d::ui::ImageView* _imageTargetScoreSliderControl;
    cocos2d::ui::Layout   * _layoutScore;

    cocos2d::ui::Layout     * _layoutExtra;
    cocos2d::ui::ImageView  * _imageJackpot;
    cocos2d::ui::ImageView  * _imageMultiply;
    cocos2d::ui::TextBMFont * _textBMMultiply;

    cocos2d::Vector<cocos2d::ui::Layout*> _listLayoutBetPoint;
    std::vector<cocos2d::Point>           _listLayoutBetPointPos;
    cocos2d::Vector<cocos2d::ui::Layout*> _listLayoutTargetScore;
    std::vector<cocos2d::Point>           _listLayoutTargetScorePos;
    std::vector<double>                   _betRange;
    bool                                  _draggingSlider;
    bool                                  _draggingTargetScoreSlider;
    std::vector<int>                      _targetScoreRange;

    void setLuckyValue(float luckyVal);
    virtual void changeLanguage(Language language) override;

    float getJackpotBetMultiply();
    float getJackpotBetMultiply(int targetScore);
    float getMaxMultiplyValue();
    float getMaxMultiplyValue(int targetScore);
protected:
public:
CC_SYNTHESIZE(LobbyScene*, _lobbyScene, LobbyScene);
CC_SYNTHESIZE(double, _bet, Bet);
CC_SYNTHESIZE(CityType, _cityType, CityType);
CC_SYNTHESIZE(int, _rule, _rule);
CC_SYNTHESIZE_READONLY(float, _luckyPercent, luckyPercent);
CC_SYNTHESIZE(int, _targetScore, TargetScore);

    ~LobbyTicketFlightBoard();
    LobbyTicketFlightBoard();

    virtual bool init() override;
    void initScreen();
    void initValue(CityType city, double bet, float luckyPercent);
    std::vector<double> getBetRange(double money);
    std::pair<double, std::vector<double>> getBetRangeSoFar(double money);
    void selectBetRange(int index);
    int getBetRangeFromPercent(float percent);
    void updateBetMoney(int index);
    int getBetRangeIndex(double betMoney);
    void updateBetRange();
    void updateBetRangeSoFar();
    cocos2d::FiniteTimeAction* getZoomInActionBetPoint();
    cocos2d::FiniteTimeAction* getActionTargetScore(float scale, float opacity, float duration, bool isScaleUp);
    void playTargetScoreAnim(cocos2d::ui::Layout* layout, float scale, float opacity);
    void setTable(int tableId);

    std::vector<int> getTargetScoreRange();
    int getTargetScoreRangeFromPercent(float percent);
    void updateTargetScore(int index);
    int getTargetScoreRangeIndex(int score);
    void selectTargetScoreRange(int index);
    void updateTargetScoreRange();

    virtual void onEnter() override;
    virtual void onExit() override;

    void onButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event);
    void onSliderTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void targetScoreSliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType event);
    void onTargetScoreSliderTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void updateUIAcordingPercent(cocos2d::ui::Slider* slider);
    void updateTargetScoreUIAcordingPercent(cocos2d::ui::Slider* slider);
    void onButtonClick(cocos2d::Ref* sender);

    void onNotifyEvent(void* sender, int id, cocos2d::Ref* data);

    CREATE_FUNC(LobbyTicketFlightBoard);
};

#endif // ! __LOBBY_TICKET_FLIGHT_BOARD_H__
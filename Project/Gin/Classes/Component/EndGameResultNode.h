#ifndef GIN_ENDGAMERESULTNODE_H
#define GIN_ENDGAMERESULTNODE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Define/InGameDefine.h"
#include "Define/GameDefine.h"
#include "Component/Avatar.h"
#include "Manager/TextManager.h"

class GPGinScene;

class EndGameResultNode : public cocos2d::Node
{
private:
    GPGinScene* _gPScene;

    cocos2d::ui::Layout    * _rootLayout;

    cocos2d::ui::Layout     *_layoutUserRoot;
    cocos2d::ui::Layout     *_layoutUserDecor;
    cocos2d::ui::Layout     *_layoutUserAvatar;
    cocos2d::ui::ImageView  *_imageUserBorder;
    cocos2d::ui::ImageView  *_imageUserBorderHighlight;
    cocos2d::ui::Layout     *_layoutUserMoney;
    cocos2d::ui::ImageView  *_imageLayoutUserMoneyBg;
    cocos2d::ui::TextBMFont *_textBMUserMoneyValue;
    cocos2d::ui::Layout     *_layoutUserScore;
    cocos2d::ui::Layout     *_layoutUserTargetScore;
    cocos2d::ui::Text       *_textUserScoreTitle;
    cocos2d::ui::TextBMFont *_textBMUserScoreValue;
    cocos2d::ui::Layout     *_layoutUserScoreGap;
    cocos2d::ui::TextBMFont *_textBMUserScoreGapValue;
    cocos2d::ui::ImageView  *_layoutStarUser;
    cocos2d::ui::TextBMFont *_textBMStarUser;
    cocos2d::Node           *_nodeUserScoreAnim;

    cocos2d::ui::Layout     *_layoutBotRoot;
    cocos2d::ui::Layout     *_layoutBotDecor;
    cocos2d::ui::Layout     *_layoutBotAvatar;
    cocos2d::ui::ImageView  *_imageBotBorder;
    cocos2d::ui::ImageView  *_imageBotBorderHighlight;
    cocos2d::ui::Layout     *_layoutBotMoney;
    cocos2d::ui::ImageView  *_imageLayoutBotMoneyBg;
    cocos2d::ui::TextBMFont *_textBMBotMoneyValue;
    cocos2d::ui::Layout     *_layoutBotScore;
    cocos2d::ui::Layout     *_layoutBotTargetScore;
    cocos2d::ui::Text       *_textBotScoreTitle;
    cocos2d::ui::TextBMFont *_textBMBotScoreValue;
    cocos2d::ui::Layout     *_layoutBotScoreGap;
    cocos2d::ui::TextBMFont *_textBMBotScoreGapValue;
    cocos2d::ui::ImageView  *_layoutStarBot;
    cocos2d::ui::TextBMFont *_textBMStarBot;
    cocos2d::Node           *_nodeBotScoreAnim;

    cocos2d::ui::Layout     *_layoutBonusRoot;

    cocos2d::ui::Layout     *_layoutBonusMultiplyRoot;
    cocos2d::ui::TextBMFont *_textBMBonusMultiplyTitle;
    cocos2d::ui::Layout     *_layoutBonusMultiplyProgressRoot;
    cocos2d::ui::LoadingBar *_progressBarMultiply;
    cocos2d::ui::Layout     *_layoutStarPivotTemplate;

    cocos2d::ui::Layout     *_layoutScoreGap;
    cocos2d::ui::TextBMFont *_textBMScoreGap;

    cocos2d::ui::Layout     *_layoutBonusJackpotRoot;
    cocos2d::ui::TextBMFont *_textBMJackpotTitle;
    cocos2d::ui::TextBMFont *_textBMJackpotMoney;
    cocos2d::ui::Layout     *_layoutJackpotValue;
    cocos2d::Sprite         *_imageJackpotBg;
    cocos2d::Sprite         *_imageJackpotBoardBg;
    cocos2d::Node          *_nodeJackpotUser;
    cocos2d::Node          *_nodeJackpotBot;

    Avatar* _userAvatar;
    Avatar* _botAvatar;

    std::string _userAvatarLink;
    std::string _botAvatarLink;
    int         _userScore;
    int         _botScore;
    float       _scoreGap;
    float       _multiplyProgressScore;
    float       _multiplyProgressMaxScore;
    double      _userMoney;
    double      _botMoney;
    double      _userFinalMoney;
    double      _botFinalMoney;
    int         _multiplyValue;
    double      _normalWinMoney;
    double      _multiplyMoney;
    double      _jackpotMoney;
    std::map<int, float>  _mapMultiplyProgress;
    std::vector<int>    _listMultiplyProgressKey;
    double      _bet;
    bool        _isUserWin;
    int         _rule;
    CityType         _cityType;
    int         _targetScore;
    int         _nextStarIndex;

    cocos2d::Point  _orgLayoutProgressCurrentPosition;
    cocos2d::Size  _layoutProgressSize;

    cocos2d::Vector<cocos2d::ui::Layout*> _listStarPivotLayout;


    cocos2d::Point _multiplyStarGlobalPos ;
    cocos2d::Size _multiplyStarGlobalSize;

    void setFinalMultiplyBonus(float multiValue);
    void updateMultiplyProgress(int currentGapScore, int maxGapScore, float percent);

    void updateMoneyAnimation(cocos2d::ui::TextBMFont* text, double value, float delay, float duration, bool isUser);

    void playStarShowAnimation(cocos2d::ui::Layout* layout, float duration, float delay, bool isLeft, bool isWin);
    void playStarHideAnimation(cocos2d::ui::Layout* layout, float duration, float delay);

    void initStarProgress();

    void runJackpotBackgroundAnimation(bool animation);
    void speedUpJackpotBackgroundAnimation(bool speed);

    cocos2d::BezierTo* getBezierAnimation(cocos2d::Point srcPos, cocos2d::Point dstPos, bool isLeft, float duration);
public:
    EndGameResultNode();
    ~EndGameResultNode();

    virtual bool init() override;

    void updateMapMultiplyProgress();

    void reset();

    void changeLanguage(Language language);

    void setUserAvatarLink(std::string link);
    void setBotAvatarLink(std::string link);

    void initLayout(int rule, CityType cityType, int targetScore, GPGinScene* scene);

    void playEndGameAnimation(std::string userAvatar, std::string botAvatar, int userScore, int botScore,  double userMoney, double botMoney,
                              double normalWinMoney, double multiplyMoney, double jackpotMoney, bool isUserWin, double bet,
                              int rule, CityType cityType, int targetScore);

    void setUserMoney(double money);
    void setBotMoney(double money);

    float getEstimateEndGameAnimationTime(int scoreGap, float multiplyValue, bool hasJackpot);



    CREATE_FUNC(EndGameResultNode);

};


#endif //GIN_ENDGAMERESULTNODE_H

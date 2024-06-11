#ifndef __ResultScoreBoxNode__
#define __ResultScoreBoxNode__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Define/InGameDefine.h"
#include "Component/Avatar.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"

typedef std::function<void()> AnimationFinishCallback;

class ResultScoreBoxNode: public cocos2d::Node
{
private:
    cocos2d::ui::Layout *_rootLayout;

    cocos2d::ui::Layout *_layoutFinal;
    cocos2d::ui::TextBMFont *_textNumberScore;
    cocos2d::ui::Text *_textTitleScore;
    cocos2d::ui::Layout *_layoutScoreUser;
    cocos2d::ui::Layout *_layoutAvatar;

    cocos2d::ui::Layout *_layoutGap;
    cocos2d::ui::TextBMFont *_textNumberScoreGap;
    cocos2d::ui::Text *_textTitleScoreGap;

    cocos2d::ui::Layout *_layoutFinalScore;
    cocos2d::ui::ImageView *_imageFinalScoreBg;
    cocos2d::ui::TextBMFont *_textFinalScoreValue;

    Avatar *_avatar;

    int _score;
    float _orgLayoutFinalOpacity;
    float _orgLayoutGapOpacity;
    float _orgLayoutFinalScoreOpacity;
public:
    ResultScoreBoxNode();
    ~ResultScoreBoxNode();
    virtual bool init() override;
    void setScore(int score);
    int getScore();

    void initLayout();

    void setAvatarLink(const std::string& link);

    void playShowGapAnimation(AnimationFinishCallback finishCallback, int score, float delay);
    void playShowFinalAnimation(AnimationFinishCallback finishCallback, const std::string& newAvatarLink, int score, int userScore, int targetScore);
    void playShowFinalAnimation1(AnimationFinishCallback finishCallback, const std::string& newAvatarLink, int score, int userScore, int targetScore);
    void playUndercutAnimation(AnimationFinishCallback finishCallback, int newScore);
    void playScoreGapUpdateAnimation(AnimationFinishCallback finishCallback, int newScore);
    void playHideAnimation(AnimationFinishCallback finishCallback, float delay);
    void playMoveHideAnimation(AnimationFinishCallback finishCallback, cocos2d::Point endPos, float delay);

    void changeLanguage(Language language);

    CREATE_FUNC(ResultScoreBoxNode);
};

#endif __ResultScoreBoxNode__

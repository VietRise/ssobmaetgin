#ifndef __SCORE_BOX_NODE_H__
#define __SCORE_BOX_NODE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Define/InGameDefine.h"

class ScoreBoxNode : public cocos2d::Node
{
private:
    cocos2d::ui::Layout    * _rootLayout;
    cocos2d::ui::TextBMFont* _textNumberScore;
    cocos2d::ui::TextBMFont* _textTitleScore;
    cocos2d::ui::Layout    * _layoutNegative;
    cocos2d::ui::Layout    * _layoutNone;
    cocos2d::ui::Layout    * _layoutPositive;
    cocos2d::ui::Layout    * _layoutScoreUser;

    int _score;
public:
    ScoreBoxNode();
    ~ScoreBoxNode();

    virtual bool init() override;
    void setScore(int score);
    int getScore();
    void show(bool show, int value, GinResultType type);
    void showUserScore(bool show, int value);
    void setLayoutColor(int value);

    void setTitle(std::string text);

    CREATE_FUNC(ScoreBoxNode);
};

#endif //__SCORE_BOX_NODE_H__
#ifndef __DEADWOOD_BOX_NODE_H__
#define __DEADWOOD_BOX_NODE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class DeadwoodBoxNode : public cocos2d::Node
{
private:
    cocos2d::ui::Layout* _rootLayout;
    cocos2d::ui::Text* _textTitle;
    cocos2d::ui::TextBMFont* _textNumberScore;

    int _score;
    int _knockScore;

    void updateText();
public:
    DeadwoodBoxNode();
    ~DeadwoodBoxNode();

    virtual bool init();
    void setScore(int score);
    int getScore();
    void setKnockScore(int knockScore);
    int getKnockScore();
    void setScoreAndKnockScore(int score, int knockScore);
    void show(int score, int knockScore);
    void hide();

    CREATE_FUNC(DeadwoodBoxNode);
};

#endif //__DEADWOOD_BOX_NODE_H__

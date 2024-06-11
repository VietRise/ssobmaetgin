#ifndef __RULE_SCORE_H__
#define __RULE_SCORE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class RuleScore : public cocos2d::ui::Layout
{
private:
    cocos2d::Size screenSize;
    cocos2d::ui::Layout* _layoutRoot;
    cocos2d::ui::Text  * _labelTitle;
protected:
public:
    static std::vector<std::pair<std::string, std::string>>                              _listTextCardPoint;
    static std::vector<std::pair<std::pair<std::string, std::string>, cocos2d::Color3B>> _listTextScoring;

    RuleScore();
    ~RuleScore();

    virtual bool init() override;
    void initLayout();
    void setContent();

    CREATE_FUNC(RuleScore);
};

#endif //__RULE_SCORE_H__

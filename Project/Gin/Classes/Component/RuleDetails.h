#ifndef __RULE_DETAILS_H__
#define __RULE_DETAILS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class RuleDetails : public cocos2d::ui::Layout
{
private:
    cocos2d::Size screenSize;
    cocos2d::ui::Layout* _layoutRoot;
    cocos2d::ui::Text  * _labelTitle;
protected:
public:
    RuleDetails();
    ~RuleDetails();

    virtual bool init() override;
    void initLayout();
    void setContent();
    CREATE_FUNC(RuleDetails);

    void setSectionData(cocos2d::ui::Layout* root, std::string layoutName, int titleTextId, int descriptionTextId);
};

#endif //__RULE_DETAILS_H__

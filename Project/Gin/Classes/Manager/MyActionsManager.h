#ifndef __MY_ACTION_MANAGER_H__
#define __MY_ACTION_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MyActionsManager
{
private:
    std::vector<std::pair<cocos2d::Node*, int>> _mapMoneyAction;
    MyActionsManager();
public:
    ~MyActionsManager();

    static MyActionsManager* getInstance();
    static void destroyInstance();

    // Create action money
    cocos2d::Action* createActionMoneyChange(cocos2d::ui::TextBMFont* moneyLabel, float duration, int step, double moneyFrom, double moneyTo, std::string preFix, std::string posFix);

    double getNodeMoneyAction(cocos2d::Node* node);
    void setNodeMoneyAction(cocos2d::Node* node, double money);
    void removeNodeAction(cocos2d::Node* node);
};

#endif // !__MY_ACTION_MANAGER_H__
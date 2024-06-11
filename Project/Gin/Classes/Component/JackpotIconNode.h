#ifndef __JACKPOT_ICON_NODE_H__
#define __JACKPOT_ICON_NODE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class JackpotIconNode : public cocos2d::Node
{
private:
    cocos2d::ui::Layout   * _rootLayout;
    cocos2d::ui::ImageView* _imageJackpotNone;
    cocos2d::ui::ImageView* _imageJackpot;
    float _imageJackpotScale;
public:
CC_SYNTHESIZE_READONLY(int, _consecutiveWin, ConsecutiveWin);
    ~JackpotIconNode();
    JackpotIconNode();

    static JackpotIconNode* create();
    virtual bool init() override;
    void reset();
    void show(bool show, bool animation = false, float scaleTo = 1.0f);
    void enableJackpot(bool enable, bool animation = false);
};

#endif //__JACKPOT_ICON_NODE_H__

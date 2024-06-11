#ifndef __DEBUG_LAYER_H__
#define __DEBUG_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class DebugLayer : public cocos2d::Layer
{
private:
    cocos2d::ui::Layout  * _layoutInterfaceRoot;
    cocos2d::ui::Layout  * _layoutDebug;
    cocos2d::ui::Button  * _buttonLog;
    cocos2d::ui::Button  * _buttonClose;
    cocos2d::ui::Button  * _buttonCopy;
    cocos2d::ui::Button  * _buttonClear;
    cocos2d::ui::ListView* _listView;
    std::vector<std::string> _logString;
protected:
public:
    DebugLayer();
    ~DebugLayer();

    static DebugLayer* getInstance();
    static void destroyInstance();

    virtual bool init() override;

    void addText(const std::string& text, const cocos2d::Color3B& color);
    void onOpenAndCloseClick(cocos2d::Ref* sender);
    void onClearClink(cocos2d::Ref* sender);
    void onCopyClick(cocos2d::Ref* sender);
    void attach(cocos2d::Node* node);
    std::string getStringLog();
    CREATE_FUNC(DebugLayer);
};

#endif //__DEBUG_LAYER_H_

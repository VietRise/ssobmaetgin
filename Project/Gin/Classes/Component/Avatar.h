#ifndef __AVATAR_H__
#define __AVATAR_H__

#include "cocos2d.h"

class Avatar : public cocos2d::Node
{
private:
    cocos2d::Sprite* _sprite;
    cocos2d::Sprite* _mask;
protected:
public:
    Avatar();
    ~Avatar();

    virtual bool init() override;
    void setFrame(const std::string& frameId, const cocos2d::Size& size);
    void setFrameUrl(const std::string& url, const cocos2d::Size& size);
    void setMask(bool mask);

    CREATE_FUNC(Avatar);
};

#endif //__AVATAR_H__
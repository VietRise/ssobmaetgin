#include "Avatar.h"
#include "Manager/DownloadManager.h"
#include "Helper/GameUtils.h"

USING_NS_CC;

Avatar::Avatar()
{
    this->_mask   = nullptr;
    this->_sprite = nullptr;
}

Avatar::~Avatar()
{
}

bool Avatar::init()
{
    if (!Node::init())
        return false;
    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);
    this->_sprite = Sprite::create();
    setMask(true);
    return true;
}

void Avatar::setFrame(const std::string& frameId, const cocos2d::Size& size)
{
    if (frameId != "")
    {
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameId);
        if (spriteFrame == nullptr)
        {
            Sprite* sprite = Sprite::create(frameId);
            if (sprite)
                spriteFrame = Sprite::create(frameId)->getSpriteFrame();
        }
        if (spriteFrame)
        {
            this->_sprite->setSpriteFrame(spriteFrame);
            float scaleX = size.width / this->_sprite->getContentSize().width;
            float scaleY = size.height / this->_sprite->getContentSize().height;
            this->_sprite->setScaleX(scaleX);
            this->_sprite->setScaleY(scaleY);
            if (this->_mask)
            {
                float maskScaleX = (this->_sprite->getContentSize().width * scaleX) / this->_mask->getContentSize().width;
                float maskScaleY = (this->_sprite->getContentSize().height * scaleY) / this->_mask->getContentSize().height;
                this->_mask->setScaleX(maskScaleX);
                this->_mask->setScaleY(maskScaleY);
            }
        }
    }
}

void Avatar::setFrameUrl(const std::string& url, const cocos2d::Size& size)
{
    if (GameUtils::validateURL(url))
        DownloadManager::getInstance()->downloadImage(this->_sprite, url, "", size);
    else
        setFrame(url, size);
}

void Avatar::setMask(bool mask)
{
    if (mask)
    {
        _mask = Sprite::createWithSpriteFrameName("ui/ImagesCommon/avatar_alpha.png");
        //settings up the masks
        ClippingNode* holseClipper = ClippingNode::create();
        holseClipper->setInverted(true);
        holseClipper->setAlphaThreshold(0);
        holseClipper->addChild(this->_sprite);
        holseClipper->setCascadeOpacityEnabled(true);
        holseClipper->setCascadeColorEnabled(true);
        holseClipper->setTag(1);

        Node* node = Node::create();
        node->setCascadeOpacityEnabled(true);
        node->setCascadeColorEnabled(true);
        node->addChild(_mask);
        holseClipper->setStencil(node);
        //set the mask position to the heroCard
        addChild(holseClipper);
    }
    else
    {
        this->_sprite->retain();
        this->_sprite->removeFromParent();
        removeChildByTag(1);
        addChild(this->_sprite);
        this->_sprite->release();
    }
}

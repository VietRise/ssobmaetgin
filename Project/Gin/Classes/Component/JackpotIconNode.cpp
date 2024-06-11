#include "JackpotIconNode.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

JackpotIconNode::~JackpotIconNode()
{
}

JackpotIconNode::JackpotIconNode()
{
    _rootLayout        = nullptr;
    _imageJackpotNone  = nullptr;
    _imageJackpot      = nullptr;
    _imageJackpotScale = 1.0f;
    _consecutiveWin    = 0;
}

JackpotIconNode* JackpotIconNode::create()
{
    JackpotIconNode* jackpotIconNode = new JackpotIconNode();
    if (jackpotIconNode->init())
    {
        jackpotIconNode->autorelease();
        return jackpotIconNode;
    }
    CC_SAFE_RELEASE_NULL(jackpotIconNode);
    return nullptr;
}

bool JackpotIconNode::init()
{
    if (!Node::init())
        return false;

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    Node* node = CSLoader::createNode("ui/Common/JackpotIcon.csb");
    this->_rootLayout = static_cast<Layout*> (node->getChildByName("Root"));
    this->_rootLayout->removeFromParent();
    this->_rootLayout->setPosition(Vec2(0.0f, 0.0f));

    addChild(this->_rootLayout);

    _imageJackpotNone  = static_cast<ImageView*>(Helper::seekWidgetByName(_rootLayout, "ImageJackpotNone"));
    _imageJackpot      = static_cast<ImageView*>(Helper::seekWidgetByName(_rootLayout, "ImageJackpot"));
    _imageJackpotScale = _imageJackpot->getScale();
    return true;
}

void JackpotIconNode::reset()
{
    _imageJackpotNone->setVisible(false);
    _imageJackpot->setVisible(false);
}

void JackpotIconNode::show(bool show, bool animation/* = false*/, float scaleTo/* = 1.0f*/)
{
    if (animation)
    {
        if (show)
        {
            this->setScale(0);
            this->setOpacity(0);
            this->setVisible(true);
            this->runAction(Spawn::create(FadeIn::create(0.1f), EaseBackOut::create(ScaleTo::create(0.2f, scaleTo)), nullptr));
        }
        else
        {
            this->runAction(Sequence::create(Spawn::create(FadeOut::create(0.2f), EaseBackIn::create(ScaleTo::create(0.1f, 0.0f)), nullptr), DelayTime::create(0.2f), Hide::create(), nullptr));
        }
    }
    else
    {
        this->setVisible(show);
    }
}

void JackpotIconNode::enableJackpot(bool enable, bool animation/* = false*/)
{
    if (animation)
    {
        if (enable)
        {
            ParticleSystemQuad* particle = (ParticleSystemQuad*) this->getChildByTag(2);
            if (particle == nullptr)
            {
                particle = ParticleSystemQuad::create("ui/particle/test3.plist");
                particle->setTag(2);
                this->addChild(particle);
            }
            particle->setPosition(Point(0, 0));
            particle->setVisible(false);
            particle->setScale(0.75f);

            _imageJackpotNone->setVisible(true);
            _imageJackpot->setVisible(true);
            // Run animation
            _imageJackpot->setScale(1.5f);
            _imageJackpot->setOpacity(255.0f);
            auto actShow = Spawn::create(CallFunc::create([=]() {
                particle->resetSystem();
                particle->setVisible(true);
            }), EaseBackIn::create(ScaleTo::create(0.2f, _imageJackpotScale)), nullptr);
            _imageJackpot->stopAllActions();
            _imageJackpot->runAction(actShow);
        }
        else
        {
            _imageJackpotNone->setVisible(true);
            _imageJackpot->setVisible(true);
            // Run animation
            auto actHide = Sequence::create(Spawn::create(FadeOut::create(0.2f), EaseBackOut::create(ScaleTo::create(0.1f, 0.0f)), nullptr),
                                            DelayTime::create(0.2f),
                                            CallFuncN::create([=](Node* node) {
                                                node->setVisible(false);
                                            }),
                                            nullptr);
            _imageJackpot->stopAllActions();
            _imageJackpot->runAction(actHide);
        }
    }
    else
    {
        if (enable)
        {
            _imageJackpotNone->setVisible(true);
            _imageJackpot->setVisible(true);
        }
        else
        {
            _imageJackpotNone->setVisible(true);
            _imageJackpot->setVisible(false);
        }
    }
}
#include "GinScoreTable.h"
#include "Manager/SoundManager.h"
#include "Manager/TextManager.h"
#include "Helper/CardIndexConverter.h"
#include "FirebaseGG/Firebase.h"
#include "Network/MessageSender.h"
#include "Manager/MyActionsManager.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace std::placeholders;

#define ROTATION_TURN_USER 129
#define ROTATION_TURN_BOT 51
#define ROTATE_ANIMATION_DURATION 0.2f

#define ACTION_ARROW_TAG 1

GinScoreTable::GinScoreTable()
{
}

GinScoreTable::~GinScoreTable()
{
}

bool GinScoreTable::init()
{
    if (!Layer::init())
        return false;
    _isShown = true;
    return true;
}

void GinScoreTable::initLayout(cocos2d::ui::Layout* root, cocos2d::ui::Layout* parent)
{
    _root = static_cast<Layout*>(Helper::seekWidgetByName(root, "LayoutScore"));
    Vec2 position = _root->getPosition();
    _root->setPosition(Vec2::ZERO);
    _root->removeFromParent();
    this->addChild(_root);
    this->setPosition(position);
    parent->addChild(this);
    this->setCascadeOpacityEnabled(true);

    _layoutTargetScoreTitle = static_cast<Layout*>(Helper::seekWidgetByName(_root, "PanelTitle"));
    _textTargetScoreTitle   = static_cast<Text*>(Helper::seekWidgetByName(_layoutTargetScoreTitle, "TextScoreTitle"));
    _layoutScoreUser        = static_cast<Layout*>(Helper::seekWidgetByName(_root, "LayoutScoreSlot0"));
    _imageAvatarUser        = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutScoreUser, "LayoutAvatar"));
    _textScoreUser          = static_cast<Text*>(Helper::seekWidgetByName(_layoutScoreUser, "TextScoreValue"));
    _textFinalScoreUser     = static_cast<Text*>(Helper::seekWidgetByName(_layoutScoreUser, "TextFinalScoreValue"));
    _layoutScoreBot         = static_cast<Layout*>(Helper::seekWidgetByName(_root, "LayoutScoreSlot1"));
    _imageAvatarBot         = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutScoreBot, "LayoutAvatar"));
    _textScoreBot           = static_cast<Text*>(Helper::seekWidgetByName(_layoutScoreBot, "TextScoreValue"));
    _textFinalScoreBot      = static_cast<Text*>(Helper::seekWidgetByName(_layoutScoreBot, "TextFinalScoreValue"));

    _textTargetScoreTitle->setString(GET_TEXT_BY_ID(TXT_TARGET_SCORE));

    this->_avatarUser = Avatar::create();
    this->_avatarUser->setPosition(this->_imageAvatarUser->getContentSize() / 2);
    this->_imageAvatarUser->addChild(this->_avatarUser, -2);

    this->_avatarBot = Avatar::create();
    this->_avatarBot->setPosition(this->_imageAvatarBot->getContentSize() / 2);
    this->_imageAvatarBot->addChild(this->_avatarBot, -2);

    this->_listPlayerScoreLayout.push_back(_layoutScoreUser);
    this->_listPlayerScoreLayout.push_back(_layoutScoreBot);

    this->_listPlayerScoreLayoutPosition.push_back(_layoutScoreUser->getPosition());
    this->_listPlayerScoreLayoutPosition.push_back(_layoutScoreBot->getPosition());

    this->_textScoreUser->setString(fmt::format(GET_TEXT_BY_ID(TXT_CURRENT_SLOT_SCORE), "-", "-"));
    this->_textScoreBot->setString(fmt::format(GET_TEXT_BY_ID(TXT_CURRENT_SLOT_SCORE), "-", "-"));

    this->_listPosition.push_back(this->_layoutScoreUser->getPosition());
    this->_listPosition.push_back(this->_layoutScoreBot->getPosition());

    this->_layoutScoreBot->setPosition(this->_listPosition[0]);
    this->_layoutScoreUser->setPosition(this->_listPosition[1]);
}

void GinScoreTable::changeLanguage(Language language)
{
    _textTargetScoreTitle->setString(GET_TEXT_BY_ID(TXT_TARGET_SCORE));
}

void GinScoreTable::resetLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData)
{
    this->updateLayoutTargetScore(targetScore, listBetData);
}

void GinScoreTable::updateLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData)
{
    std::vector<BetData> oldBetData = this->_listBetData;

    bool targetScoreChange = (this->_targetScore != targetScore);

    this->_targetScore = targetScore;
    this->_listBetData = listBetData;

    for (int i = 0; i < _listBetData.size(); ++i)
    {
        BetData data      = _listBetData[i];
        bool    hasChange = ((oldBetData.size() != listBetData.size()) || (oldBetData[i]._score != data._score));

        if (targetScoreChange == true)
        {
            //            int scoreIncreasedVal = data._score - oldBetData[i]._score;

            ImageView* image     = nullptr;
            Text     * text      = nullptr;
            Text     * finalText = nullptr;
            if (data._isUser == true)
            {
                image     = this->_imageAvatarUser;
                text      = this->_textScoreUser;
                finalText = this->_textFinalScoreUser;
            }
            else
            {
                image     = this->_imageAvatarBot;
                text      = this->_textScoreBot;
                finalText = this->_textFinalScoreBot;
            }

            float orgTextScale = finalText->getScale();
            Sequence* textSequence = Sequence::create(ScaleTo::create(0.1f, orgTextScale * 1.5f), CallFunc::create([=] {
                text->setString(fmt::format("{0}", data._score));
                finalText->setString(fmt::format("/{0}", this->_targetScore));
                SoundManager::playSoundEffect(SoundEvent::TL_SCORE);
            }), ScaleTo::create(0.1f, orgTextScale), nullptr);

            finalText->runAction(textSequence);
        }

        if (hasChange == true)
        {
            ImageView* image     = nullptr;
            Text     * text      = nullptr;
            Text     * finalText = nullptr;
            if (data._isUser == true)
            {
                image     = this->_imageAvatarUser;
                text      = this->_textScoreUser;
                finalText = this->_textFinalScoreUser;
            }
            else
            {
                image     = this->_imageAvatarBot;
                text      = this->_textScoreBot;
                finalText = this->_textFinalScoreBot;
            }

            float orgTextScale = text->getScale();
            Sequence* textSequence = Sequence::create(ScaleTo::create(0.1f, orgTextScale * 1.5f), CallFunc::create([=] {
                text->setString(fmt::format("{0}", data._score));
                finalText->setString(fmt::format("/{0}", this->_targetScore));
                SoundManager::playSoundEffect(SoundEvent::TL_SCORE);
            }), ScaleTo::create(0.1f, orgTextScale), nullptr);

            float orgImageScale = image->getScale();
            Sequence* imageSequence = Sequence::create(ScaleTo::create(0.1f, orgImageScale * 1.5f), ScaleTo::create(0.1f, orgImageScale), nullptr);

            text->runAction(textSequence);
            image->runAction(imageSequence);
        }
    }
}

void GinScoreTable::setAvatarLinks(const std::string& linkUser, const std::string& linkBot)
{
    this->_avatarUser->setFrameUrl(linkUser, this->_imageAvatarUser->getContentSize());
    this->_avatarBot->setFrameUrl(linkBot, this->_imageAvatarBot->getContentSize());
}
void GinScoreTable::setUserAvatarLink(const std::string& linkUser)
{
    this->_avatarUser->setFrameUrl(linkUser, this->_imageAvatarUser->getContentSize());
}

void GinScoreTable::setBotAvatarLink(const std::string& linkBot)
{
    this->_avatarBot->setFrameUrl(linkBot, this->_imageAvatarBot->getContentSize());
}

void GinScoreTable::runShowAnimation(float delay, ActionCallback callback)
{
    if (this->_isShown == true)
    {
        if (callback != nullptr)
        {
            callback();
        }
        return;
    }
    Point desPos = this->getPosition();
    Point srcPos = desPos;
    srcPos.x = srcPos.x - 150;
    this->setPosition(srcPos);
    this->setOpacity(0);
    this->setVisible(true);

    this->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(EaseExponentialOut::create(MoveTo::create(0.6f, desPos)), FadeIn::create(0.6f), nullptr), CallFunc::create([=]() {
        this->_isShown = true;

        if (callback != nullptr)
        {
            callback();
        }
    }), nullptr));
}

void GinScoreTable::runHideAnimation(float delay, ActionCallback callback)
{
    if (this->_isShown == false)
    {
        if (callback != nullptr)
        {
            callback();
        }
        return;
    }
    Point srcPos = this->getPosition();
    Point desPos = srcPos;
    desPos.x = srcPos.x - 150;

    this->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(EaseExponentialIn::create(MoveTo::create(0.6f, desPos)), FadeOut::create(0.6f), nullptr), CallFunc::create([=]() {
        this->setPosition(srcPos);
        this->_isShown = false;

        if (callback != nullptr)
        {
            callback();
        }
    }), nullptr));
}

void GinScoreTable::setShow(bool visible)
{
    this->_isShown = visible;
    this->setVisible(visible);
}

bool GinScoreTable::getShow()
{
    return this->_isShown;
}

cocos2d::Point GinScoreTable::getUserScoreGlobalPos()
{
    Point pos = this->_textScoreUser->getParent()->convertToWorldSpace(this->_textScoreUser->getPosition());
    pos.x -= this->_textScoreUser->getContentSize().width / 2;
    return pos;
}

cocos2d::Point GinScoreTable::getBotScoreGlobalPos()
{
    Point pos = this->_textScoreBot->getParent()->convertToWorldSpace(this->_textScoreBot->getPosition());
    pos.x -= this->_textScoreBot->getContentSize().width / 2;
    return pos;
}
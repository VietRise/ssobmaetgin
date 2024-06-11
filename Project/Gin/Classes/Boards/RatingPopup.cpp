#include "RatingPopup.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Manager/MyActionsManager.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/BoardManager.h"
#include "Helper/GameUtils.h"
#include "FirebaseGG/Firebase.h"
#include "Helper/JsonHelper.h"
#include "Network/MessageSender.h"
#include "Network/MessageHandler.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

RatingPopup::~RatingPopup()
{
}

RatingPopup::RatingPopup()
{
    _chooseStar         = 0;
    _stateRating        = 0;
    _conditionName      = "";
    _applyTouchOutClose = false;
    _applyBackKey       = false;
}

bool RatingPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void RatingPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/RatingPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_textTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextTitle"));
    this->_textTitle->setString(fmt::format(GET_TEXT_BY_ID(TXT_RATING_POPUP_TITLE), GameUtils::getGameName().c_str()));

    this->_textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));
    this->_textContent->setString(GET_TEXT_BY_ID(TXT_RATING_POPUP_CONTENT));

    this->_buttonSubmit = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonSubmit"));
    this->_buttonSubmit->setEnabled(true);
    this->_buttonSubmit->setColor(Color3B::WHITE);
    this->_buttonSubmit->addClickEventListener(CC_CALLBACK_1(RatingPopup::onSubmitButtonClick, this));
    _textSubmit = (Text*) _buttonSubmit->getChildByName("Label");
    _textSubmit->setString(GET_TEXT_BY_ID(TXT_BUTTON_SUBMIT));

    this->_buttonLater = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonLater"));
    this->_buttonLater->addClickEventListener(CC_CALLBACK_1(RatingPopup::onLaterButtonClick, this));
    auto textBtnLater = (Text*) _buttonLater->getChildByName("Label");
    textBtnLater->setString(GET_TEXT_BY_ID(TXT_BUTTON_LATER));

    for (int i = 1; i <= 5; i++)
    {
        std::string nameStar      = StringUtils::format("Star%d", i);
        auto        imageViewStar = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutRoot, nameStar));
        imageViewStar->setTag(i);
        imageViewStar->setTouchEnabled(true);
        imageViewStar->addClickEventListener(CC_CALLBACK_1(RatingPopup::onStarImageViewClick, this));
        this->_listImageViewStar.push_back(imageViewStar);

        Sprite* spriteStarFill = (Sprite*) imageViewStar->getChildByName("SpriteFill");
        spriteStarFill->setOpacity(0);
    }
    this->startAnimationStar();
}

void RatingPopup::onEnter()
{
    InterfacePopupBoard::onEnter();
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_RATING_POPUP_SUBMIT_RESPONSE, MESSAGEHANDLE_SELECTOR(RatingPopup::onServerResponse));
}

void RatingPopup::onExit()
{
    InterfacePopupBoard::onExit();
    MessageHandler::getInstance()->unregisterHandleMessage(this);
}

void RatingPopup::close(bool forceClose /*= false*/)
{
    this->closeWithAction(nullptr);
}

void RatingPopup::closeWithAction(cocos2d::CallFunc* actionBeforeEnd)
{
    _state = InterfacePopupBoard::State::CLOSED;
    _layoutRoot->stopAllActions();

    CallFunc* endAnimFunc = CallFunc::create([=] {
        _state = InterfacePopupBoard::State::CLOSED;
        this->removeFromParent();
    });

    if (actionBeforeEnd != nullptr)
        _layoutRoot->runAction(Sequence::create(Spawn::create(EaseBackIn::create(MoveTo::create(0.4f, Point(_screenSize.width / 2, _screenSize.height / 2) + Point(0, 80))),
                                                              Sequence::create(DelayTime::create(0.3f), FadeOut::create(0.1f), nullptr),
                                                              nullptr), actionBeforeEnd, endAnimFunc, nullptr));
    else
        _layoutRoot->runAction(Sequence::create(Spawn::create(EaseBackIn::create(MoveTo::create(0.4f, Point(_screenSize.width / 2, _screenSize.height / 2) + Point(0, 80))),
                                                              Sequence::create(DelayTime::create(0.3f), FadeOut::create(0.1f), nullptr),
                                                              nullptr), endAnimFunc, nullptr));
}

void RatingPopup::setConditionOpen(const std::string& conditionName)
{
    _conditionName = conditionName;
    Firebase::logOpenRatingPopup(_stateRating, conditionName);
}

void RatingPopup::onSubmitButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (_chooseStar > 0)
    {
        MessageSender::requestRatingSubmit(_chooseStar);
        Firebase::logClickStarPopup(_stateRating, _conditionName, _chooseStar);
        Firebase::logClickButtonRatingPopup(_stateRating, _conditionName, _chooseStar, "Submit", "RatingPopup");
    }
    else
    {
        for (int i = 0; i < _listImageViewStar.size(); i++)
        {
            auto imageViewStar = _listImageViewStar.at(i);
            imageViewStar->stopAllActions();
            imageViewStar->runAction(Sequence::create(ScaleTo::create(0.2f, 1.3f), EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)), nullptr));
        }
    }
}

void RatingPopup::onLaterButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    this->close();
    Firebase::logClickButtonRatingPopup(_stateRating, _conditionName, _chooseStar, "Later", "RatingPopup");
}

void RatingPopup::onStarImageViewClick(Ref* sender)
{
    ImageView* star = (ImageView*) sender;
    if (_isPlayAnimation)
    {
        this->_buttonSubmit->setEnabled(true);
        this->_buttonSubmit->setColor(Color3B::WHITE);
        this->stopAnimationStar();
    }
    this->_buttonSubmit->runAction(Sequence::create(EaseCubicActionOut::create(ScaleTo::create(0.1f, 1.2f)), EaseCubicActionOut::create(ScaleTo::create(0.1f, 1.0f)), nullptr));
    int tagStar = star->getTag();
    _chooseStar = tagStar;
    for (int i = 1; i <= 5; i++)
    {
        Sprite* spriteStarFill = (Sprite*) _listImageViewStar.at(i - 1)->getChildByName("SpriteFill");
        if (i <= tagStar)
            spriteStarFill->setOpacity(255);
        else
            spriteStarFill->setOpacity(0);
    }
}

void RatingPopup::startAnimationStar()
{
    _isPlayAnimation = true;
    for (int i = 0; i < 5; i++)
    {
        Sprite* spriteStarFill = (Sprite*) _listImageViewStar.at(i)->getChildByName("SpriteFill");
        spriteStarFill->runAction(Repeat::create(Sequence::create(DelayTime::create(0.08f * (i + 10)),
                                                                  Spawn::create(ScaleTo::create(0.16f, 1.8f), FadeIn::create(0.05f), nullptr),
                                                                  ScaleTo::create(0.2f, 1.37f),
                                                                  DelayTime::create(1.8f - 0.08f * (i + 10)),
                                                                  ScaleTo::create(0.22f, 1.8f),
                                                                  Spawn::create(FadeOut::create(0.1f), ScaleTo::create(0.1f, 1.37f), nullptr),
                                                                  nullptr), 1));
    }
}

void RatingPopup::stopAnimationStar()
{
    _isPlayAnimation = false;
    for (int i = 0; i < 5; i++)
    {
        Sprite* spriteStarFill = (Sprite*) _listImageViewStar.at(i)->getChildByName("SpriteFill");
        spriteStarFill->stopAllActions();
        spriteStarFill->setScale(1.37f);
    }
}

void RatingPopup::onServerResponse(MessageID messageId, const rapidjson::Document& data)
{
    switch (messageId)
    {
        case MessageID::MESSAGE_RATING_POPUP_SUBMIT_RESPONSE:
        {
            this->onSubmitResponse(data);
            break;
        }
        default:
            break;
    }
}

void RatingPopup::onSubmitResponse(const rapidjson::Document& data)
{
    int result = JsonHelper::getInt(data, KEY_RESULT, 0);
    if (result > 0)
    {
        int chooseStar = JsonHelper::getInt(data, KEY_RATING_STAR, 0);
        if (chooseStar >= 4)
        {
            CallToOtherLanguage::getInstance()->openAppInStore();
            CallFunc* actBeforeEnd = CallFunc::create([=]() {
                RatingPopupThank* ratingThankPopup = BoardManager::createRatingThankPopup();
                ratingThankPopup->setType(RatingPopupThank::Type::THANK_HIGH_STAR);
                ratingThankPopup->setCallbackOK(nullptr);
                ratingThankPopup->attach(this->getParent(), kAboveForeground);
            });
            this->closeWithAction(actBeforeEnd);
        }
        else
        {
            CallFunc* actBeforeEnd = CallFunc::create([=]() {
                RatingPopupThank* ratingThankPopup = BoardManager::createRatingThankPopup();
                ratingThankPopup->setType(RatingPopupThank::Type::THANK_LOW_STAR);
                ratingThankPopup->setCallbackOK(nullptr);
                ratingThankPopup->attach(this->getParent(), kAboveForeground);
            });
            this->closeWithAction(actBeforeEnd);
        }
    }
}
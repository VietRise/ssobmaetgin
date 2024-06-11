#include "ResultScoreBoxNode.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"
#include "Manager/SoundManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

#define SHOW_ANIMATION_STEP_DURATION 0.1f
#define HIDE_ANIMATION_STEP_DURATION 0.1f
#define UNDERCUT_ANIMATION_STEP_DURATION 0.1f
#define MOVE_HIDE_ANIMATION_STEP_DURATION 0.5f
#define MOVE_HIDE_TRANSITION_ANIMATION_STEP_DURATION 0.25f
#define MOVE_FADE_ANIMATION_STEP_DURATION 0.25f
#define DELAY_HIDE 0.25f

ResultScoreBoxNode::ResultScoreBoxNode()
{

}

ResultScoreBoxNode::~ResultScoreBoxNode()
{

}
bool ResultScoreBoxNode::init()
{
    if (!Node::init())
        return false;

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    Node* node = CSLoader::createNode("ui/Common/ResultScoreBox.csb");
    this->_rootLayout = static_cast<Layout*> (node->getChildByName("Root"));
    this->_rootLayout->removeFromParent();
    this->_rootLayout->setPosition(Vec2(0.0f, 0.0f));

    addChild(this->_rootLayout);

    this->_layoutFinal = static_cast<Layout*>(Helper::seekWidgetByName(this->_rootLayout, "FinalLayout"));
    this->_textNumberScore = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutFinal, "Label"));
    this->_textTitleScore = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutFinal, "Title"));
    this->_layoutScoreUser = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutFinal, "User"));
    this->_layoutAvatar = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutFinal, "LayoutAvatar"));

    this->_avatar                 = Avatar::create();
    this->_avatar->setPosition(this->_layoutAvatar->getContentSize() / 2);
    this->_layoutAvatar->addChild(this->_avatar, -2);

    this->_layoutGap = static_cast<Layout*>(Helper::seekWidgetByName(this->_rootLayout, "GapLayout"));
    this->_textNumberScoreGap = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutGap, "Label"));
    this->_textTitleScoreGap = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutGap, "Title"));

    this->_layoutFinalScore = static_cast<Layout*>(Helper::seekWidgetByName(this->_rootLayout, "FinalScoreLayout"));
    this->_imageFinalScoreBg = static_cast<ImageView*>(Helper::seekWidgetByName(this->_layoutFinalScore, "ImageBG"));
    this->_textFinalScoreValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutFinalScore, "Label"));


    this->_orgLayoutFinalOpacity = this->_layoutFinal->getOpacity();
    this->_orgLayoutGapOpacity = this->_layoutGap->getOpacity();
    this->_orgLayoutFinalScoreOpacity = this->_layoutFinalScore->getOpacity();

    changeLanguage(TextManager::getInstance()->getLanguageType());

    return true;
}

void ResultScoreBoxNode::initLayout()
{
    this->_layoutGap->setVisible(false);
    this->_layoutFinal->setVisible(false);
    this->_layoutFinalScore->setVisible(false);

    changeLanguage(TextManager::getInstance()->getLanguageType());
}

void ResultScoreBoxNode::setScore(int score)
{
   this->_score = score;

   _textNumberScoreGap->setString(StringUtils::toString(StringUtils::format("%d",score)));
   _textNumberScore->setString(StringUtils::toString(StringUtils::format("+%d",score)));
   _textFinalScoreValue->setString(StringUtils::toString(StringUtils::format("+%d",score)));
}

int ResultScoreBoxNode::getScore()
{
   return this->_score;
}

void ResultScoreBoxNode::setAvatarLink(const std::string& link)
{
    this->_avatar->setFrameUrl(link, this->_layoutAvatar->getContentSize());
}

void ResultScoreBoxNode::playShowGapAnimation(AnimationFinishCallback finishCallback, int score, float delay)
{
    this->setVisible(true);
    this->_layoutGap->setVisible(true);
    this->_layoutFinal->setVisible(false);

    this->setScore(score);

    float orgScale = this->getScale();
    float orgOpacity = this->getOpacity();

    this->setScale(0);
    this->setOpacity(0);

    Spawn *spawn = Spawn::create(FadeTo::create(SHOW_ANIMATION_STEP_DURATION, orgOpacity),
                                 ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, orgScale),
                                 nullptr);

    Sequence *sequence = Sequence::create(DelayTime::create(delay),
                                          spawn,
                                          CallFunc::create([=]
                                                           {
                                                               SoundManager::playSoundEffect(SoundEvent::TL_SCORE);
                                                               if (finishCallback != nullptr)
                                                               {
                                                                   finishCallback();
                                                               }
                                                           }),
                                          nullptr);

    this->runAction(sequence);
}

void ResultScoreBoxNode::playShowFinalAnimation(AnimationFinishCallback finishCallback, const std::string& newAvatarLink, int score, int userScore, int targetScore)
{
    this->setScore(score);
    this->setAvatarLink(newAvatarLink);

    float orgScaleX = this->getScaleX();
    float orgScaleY = this->getScaleY();
    float orgScale = this->_layoutGap->getScale();
    float orgOpacity = this->_layoutGap->getOpacity();

    float textOrgScale = this->_textNumberScore->getScale();
    float textOrgOpacity = this->_textNumberScore->getOpacity();

    ScaleTo * scale1 = ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, 0, orgScaleY);

    ScaleTo * scale2 = ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, orgScaleX, orgScaleY);

    Sequence *sequence = Sequence::create(scale1,
                                             CallFunc::create([=]{
                                                 this->_layoutGap->setVisible(false);
                                                 this->_layoutFinal->setVisible(true);
                                             }),
                                             scale2,
                                            CallFunc::create([=]{
                                               SoundManager::playSoundEffect(SoundEvent::TL_COMPLETE);
                                            }),
                                             nullptr);

//    Spawn * spawn5 = Spawn::create(ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, textOrgScale*1.5f),
//                                   nullptr);
//
//    Spawn * spawn6 = Spawn::create(FadeTo::create(SHOW_ANIMATION_STEP_DURATION, textOrgOpacity),
//                                   ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, textOrgScale),
//                                   nullptr);
//    Sequence * textSequence = Sequence::create(DelayTime::create(SHOW_ANIMATION_STEP_DURATION*10),
//                                             spawn5,
//                                             CallFunc::create([=]{
//                                                  SoundManager::playSoundEffect(SoundEvent::TL_SECRET_MELD);
//                                                  this->_textNumberScore->setString(StringUtils::format("%d/%d",userScore, targetScore));
//                                             }),
//                                             spawn6,
//                                             nullptr);
//    this->_textNumberScore->runAction(textSequence);


    this->runAction(sequence);
}


void ResultScoreBoxNode::playShowFinalAnimation1(AnimationFinishCallback finishCallback, const std::string& newAvatarLink, int score, int userScore, int targetScore)
{
    this->setScore(score);
    this->setAvatarLink(newAvatarLink);

    float orgScaleX = this->getScaleX();
    float orgScaleY = this->getScaleY();
    float orgScale = this->_layoutGap->getScale();
    float orgOpacity = this->_layoutGap->getOpacity();

    float textOrgScale = this->_textNumberScore->getScale();
    float textOrgOpacity = this->_textNumberScore->getOpacity();

    ScaleTo * scale1 = ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, 0, orgScaleY);

    ScaleTo * scale2 = ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, orgScaleX, orgScaleY);

    Sequence *sequence = Sequence::create(scale1,
                                          CallFunc::create([=]{
                                              this->_layoutGap->setVisible(false);
                                              this->_layoutFinal->setVisible(true);
                                          }),
                                          scale2,
                                          CallFunc::create([=]{
                                              SoundManager::playSoundEffect(SoundEvent::TL_COMPLETE);
                                          }),
                                          nullptr);

    Spawn * spawn5 = Spawn::create(ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, textOrgScale*1.5f),
                                   nullptr);

    Spawn * spawn6 = Spawn::create(FadeTo::create(SHOW_ANIMATION_STEP_DURATION, textOrgOpacity),
                                   ScaleTo::create(SHOW_ANIMATION_STEP_DURATION, textOrgScale),
                                   nullptr);
    Sequence * textSequence = Sequence::create(DelayTime::create(SHOW_ANIMATION_STEP_DURATION*10),
                                             spawn5,
                                             CallFunc::create([=]{
                                                  SoundManager::playSoundEffect(SoundEvent::TL_SECRET_MELD);
                                                  this->_textNumberScore->setString(StringUtils::format("%d/%d",userScore, targetScore));
                                             }),
                                             spawn6,
                                             nullptr);
    this->_textNumberScore->runAction(textSequence);


    this->runAction(sequence);
}

void ResultScoreBoxNode::playScoreGapUpdateAnimation(AnimationFinishCallback finishCallback, int newScore)
{
    float orgScale = this->_textNumberScoreGap->getScale();

    Sequence *sequence = Sequence::create(ScaleTo::create(UNDERCUT_ANIMATION_STEP_DURATION/2, orgScale*1.5f),
                                          CallFunc::create([=]{
                                              this->setScore(newScore);
                                              SoundManager::playSoundEffect(SoundEvent::TL_HITPOT_1);
                                          }),
                                          ScaleTo::create(UNDERCUT_ANIMATION_STEP_DURATION/2, orgScale),
                                          CallFunc::create([=]{
                                              if(finishCallback != nullptr)
                                              {
                                                  finishCallback();
                                              }
                                          }),
                                          nullptr);

    this->_textNumberScoreGap->runAction(sequence);
}

void ResultScoreBoxNode::playUndercutAnimation(AnimationFinishCallback finishCallback, int newScore)
{
    float orgScale = this->_textNumberScoreGap->getScale();

    Sequence *sequence = Sequence::create(ScaleTo::create(UNDERCUT_ANIMATION_STEP_DURATION, orgScale*1.5f),
                                          CallFunc::create([=]{
                                              this->setScore(newScore);
                                              SoundManager::playSoundEffect(SoundEvent::TL_HITPOT_1);
                                          }),
                                          ScaleTo::create(UNDERCUT_ANIMATION_STEP_DURATION, orgScale),
                                          CallFunc::create([=]{
                                             if(finishCallback != nullptr)
                                             {
                                                 finishCallback();
                                             }
                                          }),
                                          nullptr);

    this->_textNumberScoreGap->runAction(sequence);
}

void ResultScoreBoxNode::playHideAnimation(AnimationFinishCallback finishCallback, float delay)
{
    float orgScale = this->getScale();
    float orgOpacity = this->getOpacity();

    Spawn *spawn = Spawn::create(FadeTo::create(HIDE_ANIMATION_STEP_DURATION, 0),
                                 ScaleTo::create(HIDE_ANIMATION_STEP_DURATION, orgScale*1.5f),
                                 nullptr);

    Sequence *sequence = Sequence::create(DelayTime::create(delay),
                                          spawn,
                                          CallFunc::create([=]{
                                              this->setVisible(false);
                                              this->setScale(orgScale);
                                              this->setOpacity(orgOpacity);

                                              this->_layoutGap->setVisible(false);
                                              this->_layoutFinal->setVisible(false);

                                              if(finishCallback != nullptr)
                                              {
                                                  finishCallback();
                                              }
                                          }),
                                          nullptr);

    this->runAction(sequence);
}

void ResultScoreBoxNode::playMoveHideAnimation(AnimationFinishCallback finishCallback, cocos2d::Point endPos, float delay)
{
    float orgScale = this->getScale();
    float orgOpacity = this->getOpacity();
    Point orgPos = this->getPosition();
    //
    this->_layoutFinalScore->setVisible(true);
    this->_layoutFinalScore->setOpacity(0);

    this->_layoutFinal->runAction(FadeOut::create(MOVE_HIDE_TRANSITION_ANIMATION_STEP_DURATION));

    this->_layoutFinalScore->runAction(FadeIn::create(MOVE_HIDE_TRANSITION_ANIMATION_STEP_DURATION));

    //
    Spawn *spawn1 = Spawn::create(ScaleTo::create(MOVE_HIDE_ANIMATION_STEP_DURATION, 0.45f),
                                 EaseSineIn::create(MoveTo::create(MOVE_HIDE_ANIMATION_STEP_DURATION, endPos)),
                                 nullptr);

    Spawn *spawn2 = Spawn::create(FadeTo::create(MOVE_FADE_ANIMATION_STEP_DURATION, 0),
                                 ScaleTo::create(MOVE_FADE_ANIMATION_STEP_DURATION, 0.65f),
                                 nullptr);

    Sequence *sequence = Sequence::create(DelayTime::create(delay),
                                          spawn1,
                                          DelayTime::create(DELAY_HIDE),
                                          spawn2,
                                          CallFunc::create([=]{
                                              this->setVisible(false);
                                              this->setScale(orgScale);
                                              this->setOpacity(orgOpacity);
                                              this->setPosition(orgPos);

                                              this->_layoutGap->setVisible(false);
                                              this->_layoutFinal->setVisible(false);
                                              this->_layoutFinalScore->setVisible(false);

                                              this->_layoutGap->setOpacity(this->_orgLayoutGapOpacity);
                                              this->_layoutFinalScore->setOpacity(this->_orgLayoutFinalScoreOpacity);
                                              this->_layoutFinal->setOpacity(this->_orgLayoutFinalOpacity);

                                              if(finishCallback != nullptr)
                                              {
                                                  finishCallback();
                                              }
                                          }),
                                          nullptr);
    this->runAction(sequence);

}

void ResultScoreBoxNode::changeLanguage(Language language)
{
    this->_textTitleScore->setText(GET_TEXT_BY_ID(TXT_TARGET_SCORE_1));
    this->_textTitleScoreGap->setText(GET_TEXT_BY_ID(TXT_SCORE_GAP));
}

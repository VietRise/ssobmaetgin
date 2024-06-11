#include "EndGameResultNode.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Helper/GameUtils.h"
#include "Define/GameDefine.h"
#include "UserData/UserData.h"
#include "Helper/fmt/format.h"
#include "Manager/SoundManager.h"
#include "Scenes/Gin/GPGinScene.h"

USING_NS_CC;
using namespace ui;
using namespace std;

#define SHOW_AVATAR_ANIMATION_DURATION 0.2f
#define SHOW_SCORE_ANIMATION_DURATION 0.5f
#define SHOW_WINNER_ANIMATION_DURATION 0.5f
#define DELAY_SHOW_SCORE_GAP 0.5f
#define MOVE_SCORE 0.5f
#define SHOW_SCORE_GAP_ANIMATION_DURATION 0.2f
#define UPDATE_SCORE_GAP_ANIMATION_DURATION 0.2f
#define SHOW_MONEY_ANIMATION_DURATION 0.2f
#define UPDATE_MONEY_ANIMATION 0.6f
#define SHOW_MULTIPLY_ANIMATION_DURATION 0.2f
#define DELAY_MULTIPLY_ANIMATION_DURATION 0.5f
#define DELAY_PROGRESS_MULTIPLY 0.0167f
#define HIDE_SCORE_GAP_ANIMATION_DURATION 0.2f
#define DELAY_SHOW_MULTIPLY_FINAL_DURATION 0.2f
#define SHOW_MULTIPLY_FINAL_DURATION 0.5f
#define DELAY_MULTIPLY_FINAL_DURATION 0.2f
#define HIDE_MULTIPLY_FINAL_DURATION 0.2f
#define POP_STAR_ANIMATION_DURATION 0.1f
#define DELAY_HIDE_MULTIPLY_ANIMATION_DURATION 1.0f
#define HIDE_MULTIPLY_ANIMATION_DURATION 0.2f
#define SHOW_JACKPOT_ANIMATION_DURATION 0.2f
#define DELAY_HIDE_JACKPOT_ANIMATION_DURATION 1.0f
#define HIDE_JACKPOT_ANIMATION_DURATION 0.2f
#define DELAY_PROGRESS_JACKPOT 0.0167f
#define HIDE_COIN_GLOW 0.1f
#define MOVE_JACKPOT_ANIMATION_DURATION 0.5f
#define DELAY_MOVE_JACKPOT_ANIMATION 0.25f
#define MOVE_MONEY_TO_SLOT_DURATION 0.5f
#define FADE_MONEY_DURATION 0.1f
#define DELAY_FADE_MONEY 0.5f
#define HIDE_AVATAR_DURATION 0.1f

#define SCORE_GAP_MINUS_STEP 0.5f

#define LAYOUT_TOP_Z_ORDER 8002
#define LAYOUT_MID_Z_ORDER 8001
#define LAYOUT_BOT_Z_ORDER 8000

#define COLOR_POSITIVE_MONEY_BG Color3B(255,180,50)
#define COLOR_NEGATIVE_MONEY_BG Color3B(67,67,67)

#define COLOR_POSITIVE_MONEY_TXT Color3B(66,41,0)
#define COLOR_NEGATIVE_MONEY_TXT Color3B(242,242,242)


EndGameResultNode::~EndGameResultNode()
{
    this->_layoutStarPivotTemplate->release();

    for(int i=0; i<this->_listStarPivotLayout.size(); ++i)
    {
        this->_listStarPivotLayout.at(i)->removeFromParent();
    }
    this->_listStarPivotLayout.clear();
}

EndGameResultNode::EndGameResultNode()
{

}

bool EndGameResultNode::init()
{
    if (!Node::init())
        return false;

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    Node* node = CSLoader::createNode("ui/Common/EndMatchResultPopup.csb");
    this->_rootLayout = static_cast<Layout*> (node->getChildByName("Root"));
    this->_rootLayout->removeFromParent();
    this->_rootLayout->setPosition(Vec2(0.0f, 0.0f));

    this->addChild(this->_rootLayout);

    this->_layoutUserRoot   = static_cast<Layout*> (Helper::seekWidgetByName(this->_rootLayout, "LayoutUser"));
    this->_layoutUserDecor  = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutUserRoot, "LayoutDecor"));
    this->_layoutUserAvatar = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutUserRoot, "LayoutAvatar"));
    this->_imageUserBorder= static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutUserRoot, "Border"));
    this->_imageUserBorderHighlight= static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutUserRoot, "BorderHighlight"));
    this->_layoutUserMoney  = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutUserRoot, "LayoutMoney"));
    this->_imageLayoutUserMoneyBg = static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutUserMoney, "ImageBg"));
    this->_textBMUserMoneyValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutUserMoney, "TextBMReward"));
    this->_layoutUserScore  = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutUserRoot, "LayoutScore"));
    this->_layoutUserTargetScore = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutUserScore, "LayoutTargetScore"));
    this->_textUserScoreTitle = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutUserTargetScore, "Title"));
    this->_textBMUserScoreValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutUserTargetScore, "Label"));
    this->_layoutUserScoreGap = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutUserScore, "LayoutScoreGap"));
    this->_textBMUserScoreGapValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutUserScoreGap, "Label"));

    this->_layoutStarUser = static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutUserMoney, "LayoutStar"));
    this->_textBMStarUser = static_cast<TextBMFont*> (Helper::seekWidgetByName(this->_layoutStarUser, "TextBMMultiplyValue"));
    this->_nodeUserScoreAnim = static_cast<Node*> (this->_layoutUserRoot->getChildByName("NodeScoreAnimPos"));

    this->_userAvatar                 = Avatar::create();
    this->_userAvatar->setPosition(this->_layoutUserAvatar->getContentSize() / 2);
    this->_layoutUserAvatar->addChild(this->_userAvatar);


    this->_layoutBotRoot   = static_cast<Layout*> (Helper::seekWidgetByName(this->_rootLayout, "LayoutBot"));
    this->_layoutBotDecor  = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBotRoot, "LayoutDecor"));
    this->_layoutBotAvatar = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBotRoot, "LayoutAvatar"));
    this->_imageBotBorder= static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutBotAvatar, "Border"));
    this->_imageBotBorderHighlight= static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutBotAvatar, "BorderHighlight"));
    this->_layoutBotMoney  = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBotRoot, "LayoutMoney"));
    this->_imageLayoutBotMoneyBg = static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutBotMoney, "ImageBg"));
    this->_textBMBotMoneyValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutBotMoney, "TextBMReward"));
    this->_layoutBotScore  = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBotRoot, "LayoutScore"));
    this->_layoutBotTargetScore = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBotScore, "LayoutTargetScore"));
    this->_textBotScoreTitle = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutBotTargetScore, "Title"));
    this->_textBMBotScoreValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutBotTargetScore, "Label"));
    this->_layoutBotScoreGap = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBotScore, "LayoutScoreGap"));
    this->_textBMBotScoreGapValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutBotScoreGap, "Label"));
    this->_layoutStarBot = static_cast<ImageView*> (Helper::seekWidgetByName(this->_layoutBotMoney, "LayoutStar"));
    this->_textBMStarBot = static_cast<TextBMFont*> (Helper::seekWidgetByName(this->_layoutStarBot, "TextBMMultiplyValue"));
    this->_nodeBotScoreAnim = static_cast<Node*> (this->_layoutBotRoot->getChildByName("NodeScoreAnimPos"));

    this->_botAvatar                 = Avatar::create();
    this->_botAvatar->setPosition(this->_layoutBotAvatar->getContentSize() / 2);
    this->_layoutBotAvatar->addChild(this->_botAvatar);

    this->_layoutBonusRoot = static_cast<Layout*> (Helper::seekWidgetByName(this->_rootLayout, "LayoutBonus"));

    this->_layoutBonusMultiplyRoot = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBonusRoot, "LayoutMultiplyBonus"));
    this->_textBMBonusMultiplyTitle = static_cast<TextBMFont*> (Helper::seekWidgetByName(this->_layoutBonusMultiplyRoot, "TextBMTitle"));
    this->_layoutBonusMultiplyProgressRoot = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBonusMultiplyRoot, "LayoutProgress"));
    this->_progressBarMultiply = static_cast<LoadingBar*> (Helper::seekWidgetByName(this->_layoutBonusMultiplyProgressRoot, "ProgressBar"));

    this->_layoutScoreGap = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBonusMultiplyRoot, "LayoutScoreGap"));
    this->_textBMScoreGap = static_cast<TextBMFont*> (Helper::seekWidgetByName(this->_layoutScoreGap, "TextBMScoreGap"));

    this->_layoutStarPivotTemplate = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBonusMultiplyProgressRoot, "PanelStarPivot"));
    this->_layoutStarPivotTemplate->removeFromParent();
    this->_layoutStarPivotTemplate->retain();
    this->_layoutStarPivotTemplate->setVisible(false);

    this->_layoutBonusJackpotRoot = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBonusRoot, "LayoutJackpot"));
    this->_imageJackpotBg = static_cast<Sprite*> (this->_layoutBonusJackpotRoot->getChildByName("ImgCoinBG"));
    this->_layoutJackpotValue = static_cast<Layout*> (Helper::seekWidgetByName(this->_layoutBonusRoot, "LayoutJackpotValue"));
    this->_textBMJackpotTitle = static_cast<TextBMFont*> (Helper::seekWidgetByName(this->_layoutJackpotValue, "TextBMTitle"));
    this->_textBMJackpotMoney = static_cast<TextBMFont*> (Helper::seekWidgetByName(this->_layoutJackpotValue, "TextBMValue"));
    this->_imageJackpotBoardBg = static_cast<Sprite*> (this->_layoutJackpotValue->getChildByName("ImageBG"));
    this->_nodeJackpotUser = static_cast<Node*> (this->_layoutBonusJackpotRoot->getChildByName("NodeAnchorUser"));
    this->_nodeJackpotBot = static_cast<Node*> (this->_layoutBonusJackpotRoot->getChildByName("NodeAnchorBot"));

    this->_imageJackpotBg->setVisible(false);


    this->changeLanguage(TextManager::getInstance()->getLanguageType());

    this->_layoutProgressSize = this->_layoutBonusMultiplyProgressRoot->getContentSize();

    return true;
}

void EndGameResultNode::initStarProgress()
{
    for(int i=0; i<this->_listStarPivotLayout.size(); ++i)
    {
        this->_listStarPivotLayout.at(i)->removeFromParent();
    }
    this->_listStarPivotLayout.clear();

    int index = 0;
    for(auto data : this->_mapMultiplyProgress)
    {
        if(data.second < 1)
        {
            continue;
        }

        Layout* layout = (Layout*)this->_layoutStarPivotTemplate->clone();

        ImageView* layoutStar = static_cast<ImageView*>(Helper::seekWidgetByName(layout, "LayoutStar"));
        TextBMFont* textMultiply = static_cast<TextBMFont*>(Helper::seekWidgetByName(layoutStar, "TextBMMultiply"));

        Layout* layoutScore = static_cast<Layout*>(Helper::seekWidgetByName(layout, "PanelScore"));
        TextBMFont* textScore = static_cast<TextBMFont*>(Helper::seekWidgetByName(layoutScore, "TextBMScore"));

        textMultiply->setString(StringUtils::format("x%.0f", data.second));
        textScore->setString(StringUtils::format("%d", data.first));

        layoutStar->loadTexture("ui/ImagesCommon/unfill_star.png", Widget::TextureResType::PLIST);

        this->_layoutBonusMultiplyProgressRoot->addChild(layout);
        this->_listStarPivotLayout.pushBack(layout);

        if(data.second == 1)
        {
            layout->setVisible(false);
        }
        else
        {
            layout->setVisible(true);
        }

        index++;
    }

    Point position = this->_layoutBonusMultiplyProgressRoot->getContentSize()/2;
    float offsetX = this->_layoutBonusMultiplyProgressRoot->getContentSize().width/(this->_listStarPivotLayout.size()-1);
//    position.x = offsetX;
    position.x = 0;
    for(int i=0; i<this->_listStarPivotLayout.size(); ++i)
    {
        Layout* layout = (Layout*)this->_listStarPivotLayout.at(i);
        position.y = layout->getPositionY();
        layout->setPosition(position);
        position.x += offsetX;
    }
}

void EndGameResultNode::changeLanguage(Language language)
{
    this->_textUserScoreTitle->setString(GET_TEXT_BY_ID(TXT_SCORE));
    this->_textBotScoreTitle->setString(GET_TEXT_BY_ID(TXT_SCORE));

    this->_textBMJackpotTitle->setString(GET_TEXT_BY_ID(TXT_JACKPOT));
    this->_textBMBonusMultiplyTitle->setString(GET_TEXT_BY_ID(TXT_SCORE_GAP_MULTIPLY_BONUS_TITLE));
}

void EndGameResultNode::reset()
{
    this->_layoutUserRoot->setVisible(false);
    this->_layoutUserDecor->setVisible(false);
    this->_imageUserBorder->setVisible(true);
    this->_imageUserBorderHighlight->setVisible(false);
    this->_layoutUserMoney->setVisible(false);
    this->_layoutStarUser->setVisible(false);
    this->_layoutUserTargetScore->setVisible(false);
    this->_layoutUserScoreGap->setVisible(false);
    this->_layoutUserScore->setVisible(false);

    this->_layoutBotRoot->setVisible(false);
    this->_layoutBotDecor->setVisible(false);
    this->_imageBotBorder->setVisible(true);
    this->_imageBotBorderHighlight->setVisible(false);
    this->_layoutBotMoney->setVisible(false);
    this->_layoutStarBot->setVisible(false);
    this->_layoutBotTargetScore->setVisible(false);
    this->_layoutBotScoreGap->setVisible(false);
    this->_layoutBotScore->setVisible(false);

    this->_layoutBonusMultiplyRoot->setVisible(false);
    this->_layoutBonusJackpotRoot->setVisible(false);


    this->_multiplyProgressScore = 0;
    this->_multiplyProgressMaxScore = 0;

    this->_multiplyStarGlobalPos = Point::ZERO;
    this->_multiplyStarGlobalSize = Size::ZERO;

    this->updateMapMultiplyProgress();
    this->initStarProgress();
}

void EndGameResultNode::setUserAvatarLink(std::string link)
{
    this->_userAvatarLink = link;
    this->_userAvatar->setFrameUrl(link, this->_layoutUserAvatar->getContentSize());
}

void EndGameResultNode::setBotAvatarLink(std::string link)
{
    this->_botAvatarLink = link;
    this->_botAvatar->setFrameUrl(link, this->_layoutBotAvatar->getContentSize());
}

void EndGameResultNode::initLayout(int rule, CityType cityType, int targetScore, GPGinScene* scene)
{
    _rule = rule;
    _cityType = cityType;
    _targetScore = targetScore;
    _gPScene = scene;
   this->reset();
}

void EndGameResultNode::playEndGameAnimation(std::string userAvatar, std::string botAvatar, int userScore, int botScore,  double userMoney, double botMoney,
                              double normalWinMoney, double multiplyMoney, double jackpotMoney, bool isUserWin, double bet,
                              int rule, CityType cityType, int targetScore)
{
    this->_layoutUserRoot->setLocalZOrder(LAYOUT_TOP_Z_ORDER);
    this->_layoutBotRoot->setLocalZOrder(LAYOUT_TOP_Z_ORDER);
    this->_layoutBonusRoot->setLocalZOrder(LAYOUT_MID_Z_ORDER);

    _rule = rule;
    _cityType = cityType;
    _targetScore = targetScore;

    this->reset();
    float totalDelay = 0.0f;

    _isUserWin = isUserWin;

    _nextStarIndex = 0;

    this->setUserAvatarLink(userAvatar);
    this->setBotAvatarLink(botAvatar);

    this->_userScore = userScore;
    this->_botScore = botScore;
    this->_scoreGap = abs(this->_userScore - this->_botScore);

    this->_userFinalMoney = userMoney;
    this->_botFinalMoney = botMoney;

    this->_normalWinMoney = normalWinMoney;
    this->_multiplyMoney = multiplyMoney;
    this->_jackpotMoney = jackpotMoney;
    this->_bet = bet;
    this->_multiplyValue = this->_multiplyMoney/this->_bet+1;

    this->setBotMoney(0);
    this->setUserMoney(0);

    this->_textBMUserScoreValue->setString(StringUtils::format("%d", this->_userScore));
    this->_textBMBotScoreValue->setString(StringUtils::format("%d", this->_botScore));
    this->_textBMUserScoreGapValue->setString(StringUtils::format("%d", this->_userScore));
    this->_textBMBotScoreGapValue->setString(StringUtils::format("%d", this->_botScore));
    this->_textBMScoreGap->setString(StringUtils::format("%d", 0));

    //Show avatar and score
    this->_layoutUserRoot->setVisible(true);
    this->_layoutUserAvatar->setVisible(true);
    this->_layoutUserScore->setVisible(true);

    this->_layoutUserRoot->setOpacity(0);

    this->_layoutBotRoot->setVisible(true);
    this->_layoutBotAvatar->setVisible(true);
    this->_layoutBotScore->setVisible(false);

    this->_multiplyProgressMaxScore = 0;
    for(auto data : this->_mapMultiplyProgress)
    {
       if(data.first > this->_multiplyProgressMaxScore)
       {
           this->_multiplyProgressMaxScore = data.first;
       }
    }

    this->_multiplyProgressScore = 0;
     this->updateMultiplyProgress(this->_multiplyProgressScore,
                                  this->_multiplyProgressMaxScore,
                                  0);

    this->_layoutBotRoot->setOpacity(0);

    this->_layoutUserRoot->runAction(FadeIn::create(SHOW_AVATAR_ANIMATION_DURATION));
    this->_layoutBotRoot->runAction(FadeIn::create(SHOW_AVATAR_ANIMATION_DURATION));
    totalDelay += SHOW_AVATAR_ANIMATION_DURATION;

    float orgUserScoreOpacity = this->_layoutUserScore->getOpacity();
    float orgUserScoreScale = this->_layoutUserScore->getScale();

    this->_layoutUserScore->setVisible(true);
    this->_layoutUserTargetScore->setVisible(true);
    this->_layoutUserScore->setScale(orgUserScoreScale*1.5);
    this->_layoutUserScore->setOpacity(0);
    this->_layoutUserScore->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                       Spawn::create(FadeTo::create(SHOW_SCORE_ANIMATION_DURATION, orgUserScoreOpacity),
                                                                     ScaleTo::create(SHOW_SCORE_ANIMATION_DURATION, orgUserScoreScale),
                                                                     nullptr),
                                                       nullptr));

    float orgBotScoreOpacity = this->_layoutBotScore->getOpacity();
    float orgBotScoreScale = this->_layoutBotScore->getScale();

    this->_layoutBotScore->setVisible(true);
    this->_layoutBotTargetScore->setVisible(true);
    this->_layoutBotScore->setScale(orgBotScoreScale*1.5);
    this->_layoutBotScore->setOpacity(0);
    this->_layoutBotScore->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                       Spawn::create(FadeTo::create(SHOW_SCORE_ANIMATION_DURATION, orgBotScoreOpacity),
                                                                     ScaleTo::create(SHOW_SCORE_ANIMATION_DURATION, orgBotScoreScale),
                                                                     nullptr),
                                                       nullptr));

    totalDelay += SHOW_SCORE_ANIMATION_DURATION;

    //Show winner
    ImageView* imageWinner = nullptr;
    Layout* layoutWinnerDecor = nullptr;
    if(isUserWin)
    {
        imageWinner = this->_imageUserBorderHighlight;
        layoutWinnerDecor = this->_layoutUserDecor;
    }
    else
    {
        imageWinner = this->_imageBotBorderHighlight;
        layoutWinnerDecor = this->_layoutBotDecor;
    }
    imageWinner->setVisible(true);
    imageWinner->setOpacity(0);

    float orgLayoutWinnerScale = layoutWinnerDecor->getScale();
    layoutWinnerDecor->setVisible(true);
    layoutWinnerDecor->setOpacity(0);

    imageWinner->runAction(Sequence::create(DelayTime::create(totalDelay),
                                            FadeIn::create(SHOW_WINNER_ANIMATION_DURATION),
                                            nullptr));

    layoutWinnerDecor->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                  Spawn::create(FadeIn::create(SHOW_WINNER_ANIMATION_DURATION), ScaleTo::create(SHOW_WINNER_ANIMATION_DURATION, orgLayoutWinnerScale), nullptr),
                                                  nullptr));


    this->runAction(Sequence::create(DelayTime::create(totalDelay),
                                     CallFunc::create([=]{
                                         if(isUserWin == true)
                                         {
                                             SoundManager::playSoundEffect(SoundEvent::TL_WIN_GAME);
                                         }
                                         else
                                         {
                                             SoundManager::playSoundEffect(SoundEvent::TL_FOLD);
                                         }
                                     }),nullptr));

    totalDelay+=SHOW_WINNER_ANIMATION_DURATION;
    totalDelay+=DELAY_SHOW_SCORE_GAP;

    //Show money box
    this->_layoutUserMoney->setVisible(true);
    this->_layoutUserMoney->setOpacity(0);
    float orgUserMoneyLayoutScale = this->_layoutUserMoney->getScale();
    this->_layoutUserMoney->setScale(orgUserMoneyLayoutScale*1.5f);
    this->_layoutUserMoney->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                       Spawn::create(ScaleTo::create(SHOW_MONEY_ANIMATION_DURATION, orgUserMoneyLayoutScale),
                                                                     FadeIn::create(SHOW_MONEY_ANIMATION_DURATION),
                                                                     nullptr),
                                                       nullptr));

    this->_layoutBotMoney->setVisible(true);
    this->_layoutBotMoney->setOpacity(0);
    float orgBotMoneyLayoutScale = this->_layoutBotMoney->getScale();
    this->_layoutBotMoney->setScale(orgBotMoneyLayoutScale*1.5f);
    this->_layoutBotMoney->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                       Spawn::create(ScaleTo::create(SHOW_MONEY_ANIMATION_DURATION, orgBotMoneyLayoutScale),
                                                                     FadeIn::create(SHOW_MONEY_ANIMATION_DURATION),
                                                                     nullptr),
                                                       nullptr));
    totalDelay+=SHOW_MONEY_ANIMATION_DURATION;

    double userNormalWinMoney = 0;
    if (this->_isUserWin)
    {
        userNormalWinMoney = this->_normalWinMoney;
    }
    else
    {
        userNormalWinMoney = -this->_normalWinMoney;
    }
    this->updateMoneyAnimation(this->_textBMUserMoneyValue, userNormalWinMoney, totalDelay, UPDATE_MONEY_ANIMATION, true);

    double botNormalWinMoney = 0;
    if (!this->_isUserWin)
    {
        botNormalWinMoney = this->_normalWinMoney;
    } else
    {
        botNormalWinMoney = -this->_normalWinMoney;
    }
    this->updateMoneyAnimation(this->_textBMBotMoneyValue, botNormalWinMoney, totalDelay, UPDATE_MONEY_ANIMATION, false);
    totalDelay+=UPDATE_MONEY_ANIMATION;

    //Show multiply bonus
    if(this->_listMultiplyProgressKey.size() > 1)
    {
        this->_layoutBonusMultiplyRoot->setVisible(true);
        this->_layoutBonusMultiplyRoot->setOpacity(0);
        float orgLayoutBonusMultiplyRootScale = this->_layoutBonusMultiplyRoot->getScale();
        this->_layoutBonusMultiplyRoot->setScale(orgLayoutBonusMultiplyRootScale * 1.5f);
        this->_layoutBonusMultiplyRoot->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                                   Spawn::create(FadeIn::create(
                                                                                         SHOW_MULTIPLY_ANIMATION_DURATION),
                                                                                 ScaleTo::create(
                                                                                         SHOW_MULTIPLY_ANIMATION_DURATION,
                                                                                         orgLayoutBonusMultiplyRootScale),
                                                                                 nullptr),
                                                                   nullptr));
        totalDelay += SHOW_MULTIPLY_ANIMATION_DURATION;
        totalDelay += DELAY_MULTIPLY_ANIMATION_DURATION;

        //Move score to score gap
        Point scoreGapGlobalPos = this->_layoutScoreGap->getParent()->convertToWorldSpace(this->_layoutScoreGap->getPosition());

        Point orgUserScorePos = this->_layoutUserScore->getPosition();
        Point newUserScorePosition = this->_nodeUserScoreAnim->getPosition();
        BezierTo* bezierUserAnim = this->getBezierAnimation(orgUserScorePos, newUserScorePosition, true, MOVE_SCORE);

        float orgUserTargetScoreLayoutOpacity = this->_layoutUserTargetScore->getOpacity();
        float orgUserScoreGapLayoutOpacity = this->_layoutUserScoreGap->getOpacity();

        this->_layoutUserScore->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                           Spawn::create(
                                                            bezierUserAnim,
                                                           EaseBackIn::create(ScaleTo::create(MOVE_SCORE,0)),
                                                           nullptr),
                                                           CallFunc::create([=]{
                                                               this->_layoutUserScore->setPosition(orgUserScorePos);
                                                               this->_layoutUserScore->setScale(orgUserScoreScale);
                                                               this->_layoutUserScore->setVisible(false);
                                                               this->_layoutUserTargetScore->setVisible(false);
                                                               this->_layoutUserScoreGap->setVisible(false);

                                                               this->_layoutUserTargetScore->setOpacity(orgUserTargetScoreLayoutOpacity);
                                                               this->_layoutUserScoreGap->setOpacity(orgUserScoreGapLayoutOpacity);
                                                           }),
                                                                   nullptr));

        this->_layoutUserTargetScore->setVisible(true);
        this->_layoutUserScoreGap->setVisible(true);
        this->_layoutUserScoreGap->setOpacity(0);
        this->_layoutUserTargetScore->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                                 FadeTo::create(MOVE_SCORE/2,0),
                                                                 nullptr));
        this->_layoutUserScoreGap->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                                 FadeTo::create(MOVE_SCORE/2,orgUserScoreGapLayoutOpacity),
                                                                 nullptr));

        Point orgBotScorePos = this->_layoutBotScore->getPosition();
        Point newBotScorePosition = this->_nodeBotScoreAnim->getPosition();
        BezierTo* bezierBotAnim = this->getBezierAnimation(orgBotScorePos, newBotScorePosition, false, MOVE_SCORE);

        float orgBotTargetScoreLayoutOpacity = this->_layoutBotTargetScore->getOpacity();
        float orgBotScoreGapLayoutOpacity = this->_layoutBotScoreGap->getOpacity();

        this->_layoutBotScore->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                           Spawn::create(
                                                            bezierBotAnim,
                                                           EaseBackIn::create(ScaleTo::create(MOVE_SCORE,0)),
                                                           nullptr),
                                                           CallFunc::create([=]{
                                                               this->_layoutBotScore->setPosition(orgBotScorePos);
                                                               this->_layoutBotScore->setScale(orgBotScoreScale);
                                                               this->_layoutBotScore->setVisible(false);
                                                               this->_layoutBotTargetScore->setVisible(false);
                                                               this->_layoutBotScoreGap->setVisible(false);

                                                               this->_layoutBotTargetScore->setOpacity(orgBotTargetScoreLayoutOpacity);
                                                               this->_layoutBotScoreGap->setOpacity(orgBotScoreGapLayoutOpacity);
                                                           }),
                                                                   nullptr));

        this->_layoutBotTargetScore->setVisible(true);
        this->_layoutBotScoreGap->setVisible(true);
        this->_layoutBotScoreGap->setOpacity(0);
        this->_layoutBotTargetScore->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                                 FadeTo::create(MOVE_SCORE/2,0),
                                                                 nullptr));
        this->_layoutBotScoreGap->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                                 FadeTo::create(MOVE_SCORE/2,orgBotScoreGapLayoutOpacity),
                                                                 nullptr));

        totalDelay+=MOVE_SCORE;

        float orgScoreGapScale = this->_textBMScoreGap->getScale();
        this->_textBMScoreGap->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                          ScaleTo::create(UPDATE_SCORE_GAP_ANIMATION_DURATION, orgScoreGapScale*1.5f),
                                                          CallFunc::create([=]{
                                                              this->_textBMScoreGap->setString(StringUtils::format("%d",(int)this->_scoreGap));
                                                          }),
                                                          ScaleTo::create(UPDATE_SCORE_GAP_ANIMATION_DURATION, orgScoreGapScale),
                                                          nullptr));
        totalDelay+=UPDATE_SCORE_GAP_ANIMATION_DURATION;

        // Run progress animation
        Vector<FiniteTimeAction *> listAction;
        listAction.pushBack(DelayTime::create(totalDelay));
        float scoreGap = min(this->_scoreGap, this->_multiplyProgressMaxScore);

        int nextStarIndex = 0;
        while (scoreGap > 0)
        {
            scoreGap-= SCORE_GAP_MINUS_STEP;
            listAction.pushBack(DelayTime::create(DELAY_PROGRESS_MULTIPLY));
            listAction.pushBack(CallFunc::create([=]
                                                 {
//                                                     this->_scoreGap-=SCORE_GAP_MINUS_STEP;
                                                     this->_multiplyProgressScore+=SCORE_GAP_MINUS_STEP;

                                                     float percent = this->_multiplyProgressScore/this->_multiplyProgressMaxScore;

                                                     this->updateMultiplyProgress(this->_multiplyProgressScore,
                                                                                  this->_multiplyProgressMaxScore,
                                                                                  percent);

                                                     if(this->_nextStarIndex < this->_listMultiplyProgressKey.size() &&
                                                             this->_multiplyProgressScore >= this->_listMultiplyProgressKey[this->_nextStarIndex])
                                                     {

                                                         Node* starPivotNode = this->_listStarPivotLayout.at(this->_nextStarIndex);
                                                         ImageView* starNode = (ImageView*)starPivotNode->getChildByName("LayoutStar");

                                                         starNode->loadTexture("ui/ImagesCommon/fill_star.png", Widget::TextureResType::PLIST);

                                                         float orgStarScale = starNode->getScale();
                                                         starNode->runAction(Sequence::create(ScaleTo::create(0.1f, orgStarScale*2.5f),
                                                                                              ScaleTo::create(0.1f, orgStarScale*1.5f),
                                                                                              nullptr));
//                                                         _multiplyStarGlobalPos = starNode->getParent()->convertToWorldSpace(starNode->getPosition());
//                                                         _multiplyStarGlobalSize = starNode->getContentSize()*orgStarScale*1.5f;

                                                         SoundManager::playSoundEffect(SoundEvent::TL_MULTIPLY);

                                                         this->_nextStarIndex+=1;
                                                     }

                                                 }));
            totalDelay += DELAY_PROGRESS_MULTIPLY;
        }
        this->_layoutBonusMultiplyRoot->runAction(Sequence::create(listAction));

        if (this->_multiplyValue > 1)
        {
            totalDelay += DELAY_SHOW_MULTIPLY_FINAL_DURATION;

            Node* starNode = this->_listStarPivotLayout.at(this->_multiplyValue-1);
            float orgStarScale = starNode->getScale();
            _multiplyStarGlobalPos = starNode->getParent()->convertToWorldSpace(starNode->getPosition());
            _multiplyStarGlobalSize = starNode->getContentSize()*orgStarScale*1.5f;

            // Show multiply final
            this->setFinalMultiplyBonus(this->_multiplyValue);
            this->playStarShowAnimation((Layout *) this->_layoutStarUser, SHOW_MULTIPLY_FINAL_DURATION, totalDelay, true, (isUserWin));
            this->playStarShowAnimation((Layout *) this->_layoutStarBot, SHOW_MULTIPLY_FINAL_DURATION, totalDelay, false, !(isUserWin));
            totalDelay += SHOW_MULTIPLY_FINAL_DURATION;

            totalDelay += DELAY_MULTIPLY_FINAL_DURATION;

            // Update money
            double userMultiplyWinMoney = 0;
            if (this->_isUserWin)
            {
                userMultiplyWinMoney = this->_normalWinMoney + this->_multiplyMoney;
            } else
            {
                userMultiplyWinMoney = -(this->_normalWinMoney + this->_multiplyMoney);
            }
            this->updateMoneyAnimation(this->_textBMUserMoneyValue, userMultiplyWinMoney, totalDelay,
                                       UPDATE_MONEY_ANIMATION, true);

            double botMultiplyWinMoney = 0;
            if (!this->_isUserWin)
            {
                botMultiplyWinMoney = this->_normalWinMoney + this->_multiplyMoney;

                this->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                 CallFunc::create([=]{
                                                     SoundManager::playSoundEffect(SoundEvent::TL_BURNED);
                                                 }),
                                                 nullptr));
            } else
            {
                botMultiplyWinMoney = -(this->_normalWinMoney + this->_multiplyMoney);

                this->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                 CallFunc::create([=]{
                                                     SoundManager::playSoundEffect(SoundEvent::TL_WIN_MULTIPLY);
                                                 }),
                                                 nullptr));
            }
            this->updateMoneyAnimation(this->_textBMBotMoneyValue, botMultiplyWinMoney, totalDelay,
                                       UPDATE_MONEY_ANIMATION,
                                       false);

            totalDelay += UPDATE_MONEY_ANIMATION;

            // Hide multiply final
            this->playStarHideAnimation((Layout *) this->_layoutStarUser, HIDE_MULTIPLY_FINAL_DURATION, totalDelay);
            this->playStarHideAnimation((Layout *) this->_layoutStarBot, HIDE_MULTIPLY_FINAL_DURATION, totalDelay);
            totalDelay += HIDE_MULTIPLY_FINAL_DURATION;
        }

        //Hide multiply bonus
        totalDelay += DELAY_HIDE_MULTIPLY_ANIMATION_DURATION;
        float orgLayoutBonusMultiplyOpacity = this->_layoutBonusMultiplyRoot->getOpacity();
        this->_layoutBonusMultiplyRoot->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                                   Spawn::create(FadeOut::create(
                                                                                         HIDE_MULTIPLY_ANIMATION_DURATION),
                                                                                 ScaleTo::create(
                                                                                         HIDE_MULTIPLY_ANIMATION_DURATION,
                                                                                         orgLayoutBonusMultiplyRootScale *
                                                                                         1.5f),
                                                                                 nullptr),
                                                                   CallFunc::create([=]
                                                                                    {
                                                                                        this->_layoutBonusMultiplyRoot->setScale(
                                                                                                orgLayoutBonusMultiplyRootScale);
                                                                                        this->_layoutBonusMultiplyRoot->setOpacity(
                                                                                                orgLayoutBonusMultiplyOpacity);
                                                                                        this->_layoutBonusMultiplyRoot->setVisible(
                                                                                                false);
                                                                                    }),
                                                                   nullptr));
        totalDelay += HIDE_MULTIPLY_ANIMATION_DURATION;
    }
    if(this->_jackpotMoney > 0)
    {
        this->_layoutBonusJackpotRoot->setVisible(true);
        this->runJackpotBackgroundAnimation(true);

        this->runJackpotBackgroundAnimation(true);

        this->_textBMJackpotMoney->setString(GameUtils::MoneyFormat(this->_jackpotMoney,""));

        //Show Jackpot bonus
        float orgLayoutOpacity = this->_layoutJackpotValue->getOpacity();
        float orgLayoutScale = this->_layoutJackpotValue->getScale();
        Point orgLayoutPosition = this->_layoutJackpotValue->getPosition();

        this->runAction(Sequence::create(DelayTime::create(totalDelay),
                                         CallFunc::create([=]{
                                             SoundManager::playSoundEffect(SoundEvent::TL_WIN_JACKPOT);
                                         }),
                                         nullptr));

        this->_layoutJackpotValue->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                              EaseBackOut::create(ScaleTo::create(SHOW_JACKPOT_ANIMATION_DURATION, orgLayoutScale)),
                                                              nullptr));
        totalDelay += SHOW_JACKPOT_ANIMATION_DURATION;

        totalDelay += DELAY_HIDE_JACKPOT_ANIMATION_DURATION;
        //

        Point newPosition = Point::ZERO;
        if(_isUserWin)
        {
            newPosition = this->_nodeJackpotUser->getPosition();
        }
        else
        {
            newPosition = this->_nodeJackpotBot->getPosition();
        }

        ccBezierConfig bezierConfig;
        bezierConfig.endPosition = newPosition;
        if(_isUserWin == false)
        {
            bezierConfig.controlPoint_1 = Point(orgLayoutPosition.x, (orgLayoutPosition.y + newPosition.y)/2);
            bezierConfig.controlPoint_2 = Point((orgLayoutPosition.x + newPosition.x)/2, newPosition.y);
        }
        else
        {
            bezierConfig.controlPoint_1 = Point((orgLayoutPosition.x + newPosition.y)/2, orgLayoutPosition.y);
            bezierConfig.controlPoint_2 = Point(newPosition.x, (newPosition.y+orgLayoutPosition.y)/2);
        }

        Point startPos = this->_layoutJackpotValue->getParent()->convertToNodeSpace(this->_gPScene->getInterfaceLayer()->getJackpotGlobalPosition());

        this->_layoutJackpotValue->setPosition(startPos);

//        BezierTo* bezierTo = this->getBezierAnimationVertical(startPos, newPosition, _isUserWin, MOVE_JACKPOT_ANIMATION_DURATION);
        EaseSineIn* moveTo = EaseSineIn::create(MoveTo::create(MOVE_JACKPOT_ANIMATION_DURATION, newPosition));

        this->_layoutJackpotValue->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                              CallFunc::create([=]{
                                                                  this->_layoutUserRoot->setLocalZOrder(LAYOUT_MID_Z_ORDER);
                                                                  this->_layoutBotRoot->setLocalZOrder(LAYOUT_MID_Z_ORDER);
                                                                  this->_layoutBonusRoot->setLocalZOrder(LAYOUT_TOP_Z_ORDER);

                                                                  this->_gPScene->getInterfaceLayer()->setJackpotVisible(false);
                                                              }),
//                                                              bezierTo,
                                                                moveTo,
                                                              Spawn::create(
//                                                                DelayTime::create(DELAY_MOVE_JACKPOT_ANIMATION),
                                                                FadeTo::create(MOVE_JACKPOT_ANIMATION_DURATION, 0),
                                                                ScaleTo::create(MOVE_JACKPOT_ANIMATION_DURATION,0),
                                                                nullptr),
                                                              CallFunc::create([=]{
                                                                  this->_layoutBonusJackpotRoot->setVisible(false);
                                                                  this->_layoutJackpotValue->setPosition(orgLayoutPosition);
                                                                  this->_layoutJackpotValue->setScale(orgLayoutScale);
                                                                  this->_layoutJackpotValue->setOpacity(orgLayoutOpacity);
                                                                  this->runJackpotBackgroundAnimation(false);
                                                              }),
                                                              nullptr));

        totalDelay += MOVE_JACKPOT_ANIMATION_DURATION + DELAY_MOVE_JACKPOT_ANIMATION + MOVE_JACKPOT_ANIMATION_DURATION;

        //Update winner money
        if (isUserWin)
        {
            this->updateMoneyAnimation(
                    this->_textBMUserMoneyValue,
                    this->_userFinalMoney, totalDelay,
                    UPDATE_MONEY_ANIMATION, true);
        }
        else
        {
            this->updateMoneyAnimation(
                    this->_textBMBotMoneyValue,
                    this->_botFinalMoney, totalDelay,
                    UPDATE_MONEY_ANIMATION, false);
        }
        totalDelay += UPDATE_MONEY_ANIMATION;
    }
    // Hide user avatar and bot avatar
    float layoutUserDecorOpacity = this->_layoutUserDecor->getOpacity();
    this->_layoutUserDecor->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                       FadeOut::create(HIDE_AVATAR_DURATION),
                                                       CallFunc::create([=]{
                                                           this->_layoutUserDecor->setVisible(false);
                                                           this->_layoutUserDecor->setOpacity(layoutUserDecorOpacity);
                                                       }),
                                                       nullptr));

    float layoutBotDecorOpacity = this->_layoutBotDecor->getOpacity();
    this->_layoutBotDecor->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                      FadeOut::create(HIDE_AVATAR_DURATION),
                                                       CallFunc::create([=]{
                                                           this->_layoutBotDecor->setVisible(false);
                                                           this->_layoutBotDecor->setOpacity(layoutBotDecorOpacity);
                                                       }),
                                                       nullptr));
    float layoutUserAvatarOpacity = this->_layoutUserAvatar->getOpacity();
    this->_layoutUserAvatar->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                       FadeOut::create(HIDE_AVATAR_DURATION),
                                                       CallFunc::create([=]{
                                                           this->_layoutUserAvatar->setVisible(false);
                                                           this->_layoutUserAvatar->setOpacity(layoutUserAvatarOpacity);
                                                       }),
                                                       nullptr));

    float layoutBotAvatarOpacity = this->_layoutBotAvatar->getOpacity();
    this->_layoutBotAvatar->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                      FadeOut::create(HIDE_AVATAR_DURATION),
                                                      CallFunc::create([=]{
                                                          this->_layoutBotAvatar->setVisible(false);
                                                          this->_layoutBotAvatar->setOpacity(layoutBotAvatarOpacity);
                                                      }),
                                                      nullptr));

    totalDelay+=HIDE_AVATAR_DURATION;
    // Move user money to slot
    Point userMoneyOrgPos = this->_layoutUserMoney->getPosition();
    float userMoneyOrgOpacity = this->_layoutUserMoney->getOpacity();
//    Size userSlotSize = this->_gPScene->getInterfaceLayer()->getMainUserSlot()->getContentSize();

//    BezierTo *moveUserMoneyAnimation = this->getBezierAnimationVertical(userMoneyOrgPos,
//                                                                this->_layoutUserMoney->getParent()->convertToNodeSpace(
//                                                                        this->_gPScene->getInterfaceLayer()->getPositionTextMoney(
//                                                                                MAIN_USER_SLOT_INDEX) + Point(0, 70)), false,
//                                                                MOVE_MONEY_TO_SLOT_DURATION);
    EaseSineIn *moveUserMoneyAnimation = EaseSineIn::create(
            MoveTo::create(MOVE_MONEY_TO_SLOT_DURATION, this->_layoutUserMoney->getParent()->convertToNodeSpace(
                    this->_gPScene->getInterfaceLayer()->getPositionTextMoney(
                            MAIN_USER_SLOT_INDEX) + Point(0, 70))));

    this->_layoutUserMoney->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                       Spawn::create(moveUserMoneyAnimation,
                                                                     ScaleTo::create(MOVE_MONEY_TO_SLOT_DURATION, orgUserMoneyLayoutScale*0.7f),
                                                                     nullptr),
                                                       DelayTime::create(DELAY_FADE_MONEY),
                                                       Spawn::create(FadeTo::create(FADE_MONEY_DURATION, 0),
                                                                     ScaleTo::create(FADE_MONEY_DURATION, orgUserMoneyLayoutScale*1.1f),
                                                                     nullptr),
                                                       CallFunc::create([=]{
                                                           this->_layoutUserMoney->setPosition(userMoneyOrgPos);
                                                           this->_layoutUserMoney->setOpacity(userMoneyOrgOpacity);
                                                           this->_layoutUserMoney->setScale(orgUserMoneyLayoutScale);
                                                           this->_layoutUserMoney->setVisible(false);

                                                           this->_gPScene->getInterfaceLayer()->updateMoney(MAIN_USER_SLOT_INDEX);
                                                       }),
                                                       nullptr));

    // Move bot money to slot
    Point botMoneyOrgPos = this->_layoutBotMoney->getPosition();
    float botMoneyOrgOpacity = this->_layoutBotMoney->getOpacity();
//    Size botSlotSize = this->_gPScene->getInterfaceLayer()->getSlotNode(BOT_SLOT_INDEX)->getContentSize();

//    BezierTo *moveBotMoneyAnimation = this->getBezierAnimationVertical(botMoneyOrgPos,
//                                                               this->_layoutBotMoney->getParent()->convertToNodeSpace(
//                                                                       this->_gPScene->getInterfaceLayer()->getPositionTextMoney(
//                                                                               BOT_SLOT_INDEX)+ Point(0, -50)), false,
//                                                               MOVE_MONEY_TO_SLOT_DURATION);
    EaseSineIn *moveBotMoneyAnimation = EaseSineIn::create(
            MoveTo::create(MOVE_MONEY_TO_SLOT_DURATION, this->_layoutBotMoney->getParent()->convertToNodeSpace(
                    this->_gPScene->getInterfaceLayer()->getPositionTextMoney(
                            BOT_SLOT_INDEX) + Point(0, -50))));
    this->_layoutBotMoney->runAction(Sequence::create(DelayTime::create(totalDelay),
                                                      Spawn::create(moveBotMoneyAnimation,
                                                                    ScaleTo::create(MOVE_MONEY_TO_SLOT_DURATION, orgBotMoneyLayoutScale*0.7f),
                                                                    nullptr),
                                                      DelayTime::create(DELAY_FADE_MONEY),
                                                      Spawn::create(FadeTo::create(FADE_MONEY_DURATION, 0),
                                                                     ScaleTo::create(FADE_MONEY_DURATION, orgBotMoneyLayoutScale*1.1f),
                                                                     nullptr),
                                                      CallFunc::create([=]{
                                                          this->_layoutBotMoney->setPosition(botMoneyOrgPos);
                                                          this->_layoutBotMoney->setOpacity(botMoneyOrgOpacity);
                                                          this->_layoutBotMoney->setScale(orgBotMoneyLayoutScale);
                                                          this->_layoutBotMoney->setVisible(false);

                                                          this->_gPScene->getInterfaceLayer()->updateMoney(BOT_SLOT_INDEX);
                                                      }),
                                                      nullptr));
}

void EndGameResultNode::setUserMoney(double money)
{
    this->_userMoney = money;

    if(this->_userMoney >= 0)
    {
        this->_textBMUserMoneyValue->setString(GameUtils::MoneyFormat(this->_userMoney,"+"));
    }
    else
    {
        this->_textBMUserMoneyValue->setString(GameUtils::MoneyFormat(this->_userMoney, ""));
    }

    if(this->_userMoney > 0)
    {
        this->_textBMUserMoneyValue->setColor(COLOR_POSITIVE_MONEY_TXT);
        this->_imageLayoutUserMoneyBg->setColor(COLOR_POSITIVE_MONEY_BG);
    }
    else
    {
        this->_textBMUserMoneyValue->setColor(COLOR_NEGATIVE_MONEY_TXT);
        this->_imageLayoutUserMoneyBg->setColor(COLOR_NEGATIVE_MONEY_BG);
    }
}

void EndGameResultNode::setBotMoney(double money)
{
    this->_botMoney = money;

    if(this->_botMoney >= 0)
    {
        this->_textBMBotMoneyValue->setString(GameUtils::MoneyFormat(this->_botMoney,"+"));
    }
    else
    {
        this->_textBMBotMoneyValue->setString(GameUtils::MoneyFormat(this->_botMoney, ""));
    }

    if(this->_botMoney > 0)
    {
        this->_textBMBotMoneyValue->setColor(COLOR_POSITIVE_MONEY_TXT);
        this->_imageLayoutBotMoneyBg->setColor(COLOR_POSITIVE_MONEY_BG);
    }
    else
    {
        this->_textBMBotMoneyValue->setColor(COLOR_NEGATIVE_MONEY_TXT);
        this->_imageLayoutBotMoneyBg->setColor(COLOR_NEGATIVE_MONEY_BG);
    }
}

void EndGameResultNode::setFinalMultiplyBonus(float multiValue)
{
   this->_textBMStarUser->setString(StringUtils::format("x%.0f", multiValue));
   this->_textBMStarBot->setString(StringUtils::format("x%.0f", multiValue));
}

void EndGameResultNode::updateMapMultiplyProgress()
{
    if (s_targetScoreDataRange.find((int) _rule) != s_targetScoreDataRange.end())
    {
        const std::map<int, std::vector<TargetScoreData>>& map = s_targetScoreDataRange.at((int) _rule);
        int cityType = (int) _cityType;
        if (map.find((int) _cityType) == map.end())
            cityType = (int) CityType::CLASSIC_GIN;
        const std::vector<TargetScoreData>& listTargetScoreData = map.at(cityType);
        for (const auto                   & scoreData : listTargetScoreData)
            if (scoreData._targetScore == _targetScore)
            {
                this->_mapMultiplyProgress = scoreData._multiplyDataMap;
                this->_listMultiplyProgressKey.clear();
                for(std::pair<int, float> pair : this->_mapMultiplyProgress)
                {
                    this->_listMultiplyProgressKey.push_back(pair.first);
                }
                std::sort(this->_listMultiplyProgressKey.begin(), this->_listMultiplyProgressKey.end());
                break;
            }
    }
}

void EndGameResultNode::updateMultiplyProgress(int currentGapScore, int maxGapScore, float percent)
{
    this->_progressBarMultiply->setPercent(percent*100.0f);
//    this->_textBMProgressValue->setString(StringUtils::format("%d/%d",currentGapScore, maxGapScore));
}

void EndGameResultNode::updateMoneyAnimation(cocos2d::ui::TextBMFont* text, double value, float delay, float duration, bool isUser)
{
    float orgTextScale = text->getScale();
    text->runAction(Sequence::create(DelayTime::create(delay),
                                                            ScaleTo::create(duration / 2,
                                                                            orgTextScale * 1.2f),
                                                            CallFunc::create([=]
                                                                             {
                                                                                 if (isUser)
                                                                                 {
                                                                                     this->setUserMoney(value);
                                                                                 }
                                                                                 else
                                                                                 {
                                                                                     this->setBotMoney(value);
                                                                                 }

                                                                                 SoundManager::playSoundEffect(SoundEvent::TL_SCORE);
                                                                             }),
                                                            ScaleTo::create(duration / 2,
                                                                            orgTextScale),
                                                            nullptr));
}

float EndGameResultNode::getEstimateEndGameAnimationTime(int scoreGap, float multiplyValue, bool hasJackpot)
{
    float result = SHOW_AVATAR_ANIMATION_DURATION +
                   SHOW_SCORE_ANIMATION_DURATION +
                   SHOW_WINNER_ANIMATION_DURATION +
                   DELAY_SHOW_SCORE_GAP +
                   SHOW_MONEY_ANIMATION_DURATION +
                   UPDATE_MONEY_ANIMATION;

   if(_listMultiplyProgressKey.size() > 1)
   {
       result += SHOW_MULTIPLY_ANIMATION_DURATION +
                 DELAY_MULTIPLY_ANIMATION_DURATION +
                 MOVE_SCORE +
                 UPDATE_SCORE_GAP_ANIMATION_DURATION +
                 DELAY_PROGRESS_MULTIPLY * scoreGap;

       if(multiplyValue > 1)
       {
           result += DELAY_SHOW_MULTIPLY_FINAL_DURATION +
                     SHOW_MULTIPLY_FINAL_DURATION +
                     DELAY_MULTIPLY_FINAL_DURATION +
                     UPDATE_MONEY_ANIMATION +
                     HIDE_MULTIPLY_FINAL_DURATION;
       }

       result += DELAY_HIDE_MULTIPLY_ANIMATION_DURATION +
                 HIDE_MULTIPLY_ANIMATION_DURATION;

   }


   if(hasJackpot)
   {
       result += SHOW_JACKPOT_ANIMATION_DURATION +
                 DELAY_HIDE_JACKPOT_ANIMATION_DURATION +
//                 HIDE_COIN_GLOW +
                 MOVE_JACKPOT_ANIMATION_DURATION +
                 DELAY_MOVE_JACKPOT_ANIMATION +
                 MOVE_JACKPOT_ANIMATION_DURATION +
                 UPDATE_MONEY_ANIMATION;
   }

   result += MOVE_MONEY_TO_SLOT_DURATION;
   result += FADE_MONEY_DURATION;

   result += 3.0f;

   return result;
}

void EndGameResultNode::playStarShowAnimation(cocos2d::ui:: Layout* layout, float duration, float delay, bool isLeft, bool isWin)
{
    layout->setVisible(true);

    if(isWin)
    {
        ((ImageView*)layout)->loadTexture("ui/ImagesCommon/fill_star.png", Widget::TextureResType::PLIST);
    }
    else
    {
        ((ImageView*)layout)->loadTexture("ui/ImagesCommon/grey_star.png", Widget::TextureResType::PLIST);
    }

    Point srcPos = layout->getParent()->convertToNodeSpace(_multiplyStarGlobalPos);
    Point dstPos = layout->getPosition();

    float scale = layout->getContentSize().width/this->_multiplyStarGlobalSize.width;

//    float orgOpacity = layout->getOpacity();

    float orgOpacity = 255;
    float orgScale = layout->getScale();

    layout->setOpacity(0);
//    layout->setScale(orgScale*1.5f);
    layout->setScale(scale);
    layout->setPosition(srcPos);

    float scaleDuration = 0.1f;
    duration -= scaleDuration*2;

    BezierTo* bezierTo = this->getBezierAnimation(srcPos, dstPos, isLeft, duration);

    layout->runAction(Sequence::create(DelayTime::create(delay),
                                       Spawn::create(ScaleTo::create(duration, orgScale*1.2f),
                                                     FadeTo::create(duration/2, orgOpacity),
                                                     RotateBy::create(duration, 360.0f),
                                                     bezierTo,
                                                     nullptr),
                                       ScaleTo::create(scaleDuration, orgScale*0.8f),
                                       ScaleTo::create(scaleDuration, orgScale),
                                       nullptr));
}

void EndGameResultNode::playStarHideAnimation(cocos2d::ui:: Layout* layout, float duration, float delay)
{
//    float orgOpacity = layout->getOpacity();
    float orgScale = layout->getScale();
    float orgOpacity = 255;

    layout->runAction(Sequence::create(DelayTime::create(delay),
                                       Spawn::create(ScaleTo::create(duration, orgScale*1.5f),
                                                     FadeTo::create(duration, 0),
                                                     nullptr),
                                       CallFunc::create([=]{
                                          layout->setOpacity(orgOpacity);
                                          layout->setScale(orgScale);
                                          layout->setVisible(false);
                                       }),
                                       nullptr));
}

void EndGameResultNode::runJackpotBackgroundAnimation(bool animation)
{
    Sprite* bg = (Sprite*) _imageJackpotBoardBg;
    if (bg)
    {
        if (animation)
        {
            if (bg->getActionByTag(1) == nullptr)
            {
                bg->stopAllActions();
                Vector<SpriteFrame*> animFrames;
                animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg1.png"));
                animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg2.png"));
                Action* action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.6f)));
                action->setTag(1);
                bg->runAction(action);
            }
        }
        else
        {
            bg->stopAllActions();
            bg->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg.png"));
        }
    }
}

void EndGameResultNode::speedUpJackpotBackgroundAnimation(bool speed)
{
    Sprite* bg = (Sprite*) _imageJackpotBoardBg;
    if (bg && bg->getActionByTag(1) != nullptr)
    {
        bg->stopAllActions();
        Vector<SpriteFrame*> animFrames;
        animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg1.png"));
        animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/ImagesCommon/jackpot_bg2.png"));
        Action* action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, speed ? 0.12f : 0.6f)));
        action->setTag(1);
        bg->runAction(action);
    }
}

cocos2d::BezierTo* EndGameResultNode::getBezierAnimation(cocos2d::Point srcPos, cocos2d::Point dstPos, bool isLeft, float duration)
{
    ccBezierConfig bezierConfig;
    bezierConfig.endPosition = dstPos;
    if(isLeft == false)
    {
        bezierConfig.controlPoint_1 = Point(srcPos.x, (srcPos.y + dstPos.y)/2);
        bezierConfig.controlPoint_2 = Point((srcPos.x + dstPos.x)/2, dstPos.y);
    }
    else
    {
        bezierConfig.controlPoint_1 = Point((srcPos.x + dstPos.y)/2, srcPos.y);
        bezierConfig.controlPoint_2 = Point(dstPos.x, (dstPos.y+srcPos.y)/2);
    }

    BezierTo* bezierTo = BezierTo::create(duration,bezierConfig);

    return bezierTo;
}

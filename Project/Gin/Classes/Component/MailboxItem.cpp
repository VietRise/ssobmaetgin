#include "MailboxItem.h"
#include "Manager/BoardManager.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"
#include "Network/MessageSender.h"
#include "Helper/JsonHelper.h"
#include "UserData/UserData.h"
#include "Helper/fmt/format.h"

USING_NS_CC;
using namespace ui;
using namespace std;

MailboxItem::~MailboxItem()
{
    CC_SAFE_RELEASE_NULL(_layout);
}

MailboxItem::MailboxItem(cocos2d::ui::Layout* layout) : Ref()
{
    _id             = "";
    _type           = "";
    _data           = "";
    _actionCallback = nullptr;
    this->_layout = layout;
    this->_layout->retain();
    autorelease();
}

void MailboxItem::initValue(const rapidjson::Value& data, const MailboxActionCallback& callback)
{
    _actionCallback = callback;
    _id             = JsonHelper::getString(data, KEY_MAILBOX_ID, "");
    _type           = JsonHelper::getString(data, KEY_MAILBOX_TYPE, "");
    _data           = JsonHelper::getString(data, KEY_MAILBOX_DATA, "");
    setTitle(GET_TEXT_BY_JSON(JsonHelper::parseStringToJson(JsonHelper::getString(data, KEY_MAILBOX_TITLE, "").c_str())));
    setContent(GET_TEXT_BY_JSON(JsonHelper::parseStringToJson(JsonHelper::getString(data, KEY_MAILBOX_CONTENT, "").c_str())));
    setFullContent(GET_TEXT_BY_JSON(JsonHelper::parseStringToJson(JsonHelper::getString(data, KEY_MAILBOX_FULL_CONTENT, "").c_str())));

    _buttonListView = static_cast<Button*>(Helper::seekWidgetByName(this->_layout, "ButtonListView"));
    //    _buttonListView->addClickEventListener(CC_CALLBACK_1(MailboxItem::onButtonClick, this));
    _buttonListView->setEnabled(false);

    _buttonClaim = static_cast<Button*>(Helper::seekWidgetByName(this->_layout, "ButtonClaim"));
    _buttonClaim->addClickEventListener(CC_CALLBACK_1(MailboxItem::onButtonClick, this));

    _buttonAction = static_cast<Button*>(Helper::seekWidgetByName(this->_layout, "ButtonAction"));
    _buttonAction->addClickEventListener(CC_CALLBACK_1(MailboxItem::onButtonClick, this));

    _buttonClaim->setVisible(false);
    _buttonAction->setVisible(false);

    _imageNotify = static_cast<ImageView*>(Helper::seekWidgetByName(this->_layout, "ImageNotify"));
    _imageNotify->setVisible(false);

    auto labelClaim = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonClaim, "Label"));
    labelClaim->setString(GET_TEXT_BY_ID(TXT_MAILBOX_ACTION_CLAIM_REWARD));

    auto labelAction = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonAction, "Label"));
    labelAction->setString("");

    auto layoutReward = static_cast<Layout*>(Helper::seekWidgetByName(this->_layout, "PanelReward"));
    layoutReward->setVisible(false);

    initIcon();
    if (_type == MAILBOX_TYPE_REWARD)
    {
        _buttonClaim->setVisible(true);
        _buttonAction->setVisible(false);
        setRewards(JsonHelper::getString(data, KEY_MAILBOX_REWARDS, "[]"));
    }
    else if (_type == MAILBOX_TYPE_READ)
    {
        _buttonClaim->setVisible(false);
        _buttonAction->setVisible(false);
        if (_fullContent != "")
            _buttonAction->setVisible(true);
        labelAction->setString(GET_TEXT_BY_ID(TXT_MAILBOX_ACTION_READ));
    }
    else if (_type == MAILBOX_TYPE_UPDATE)
    {
        _buttonClaim->setVisible(false);
        _buttonAction->setVisible(true);
        labelAction->setString(GET_TEXT_BY_ID(TXT_MAILBOX_ACTION_UPDATE));
    }
    else if (_type == MAILBOX_TYPE_FUNCTION)
    {
        _buttonClaim->setVisible(false);
        _buttonAction->setVisible(true);
        labelAction->setString(GET_TEXT_BY_ID(TXT_MAILBOX_ACTION_FUNCTION));
    }
    else if (_type == MAILBOX_TYPE_SHARE)
    {
        _buttonClaim->setVisible(false);
        _buttonAction->setVisible(true);
        labelAction->setString(GET_TEXT_BY_ID(TXT_MAILBOX_ACTION_SHARE));
    }
    else
    {
        _buttonClaim->setVisible(false);
        _buttonAction->setVisible(false);
    }
    setTimeStart(JsonHelper::getDouble(data, KEY_MAILBOX_START_AT, 0));
    setTimeEnd(JsonHelper::getDouble(data, KEY_MAILBOX_END_AT, 0));
    std::vector<std::string> listMailRead = UserData::getInstance()->getMailRead();
    bool                     isNew        = false;
    if (std::find(listMailRead.begin(), listMailRead.end(), (_type + "_" + _id)) == listMailRead.end())
        isNew = true;
    if (isNew || _type == MAILBOX_TYPE_REWARD)
        _imageNotify->setVisible(true);
}

void MailboxItem::onButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    if (button == _buttonClaim)
        MessageSender::requestClaimMailboxReward(_type, _id);
    else if (button == _buttonListView)
        _imageNotify->setVisible(false);
    else if (button == _buttonAction)
        if (_actionCallback)
            _actionCallback(this);
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void MailboxItem::setTitle(const std::string& title)
{
    _title = title;
    Text* textTitle = (Text*) Helper::seekWidgetByName(_layout, "TextTitle");
    textTitle->setString(_title);
}

void MailboxItem::setContent(const std::string& content)
{
    _content = content;
    Text* textContent = (Text*) Helper::seekWidgetByName(_layout, "TextContent");
    textContent->setString(_content);
}

void MailboxItem::setFullContent(const std::string& content)
{
    _fullContent = content;
}

void MailboxItem::setTimeStart(double startAt)
{
    auto textStartAt = static_cast<Text*>(Helper::seekWidgetByName(this->_layout, "TextStartAt"));
    textStartAt->setString("");
    if (startAt > 0)
    {
        int timeLeftSecond = GameUtils::getCurrentTime() - startAt;
        if (timeLeftSecond >= 0)
        {
            int day = timeLeftSecond / 86400;
            timeLeftSecond -= day * 86400;
            int hour = timeLeftSecond / 3600;
            timeLeftSecond -= hour * 3600;
            int minute = timeLeftSecond / 60;
            timeLeftSecond -= minute * 60;

            int second = timeLeftSecond;
            int time   = 0;

            std::string timeText = "";
            if (day > 0)
            {
                time         = day;
                timeText     = GET_TEXT_BY_ID(TXT_PLURAL_DAY);
                if (day <= 1)
                    timeText = GET_TEXT_BY_ID(TXT_DAY);
            }
            else if (hour > 0)
            {
                time         = hour;
                timeText     = GET_TEXT_BY_ID(TXT_PLURAL_HOUR);
                if (hour <= 1)
                    timeText = GET_TEXT_BY_ID(TXT_HOUR);
            }
            else if (minute > 0)
            {
                time         = minute;
                timeText     = GET_TEXT_BY_ID(TXT_PLURAL_MINUTE);
                if (minute <= 1)
                    timeText = GET_TEXT_BY_ID(TXT_MINUTE);
            }
            else if (second > 0)
            {
                time         = second;
                timeText     = GET_TEXT_BY_ID(TXT_PLURAL_SECOND);
                if (second <= 1)
                    timeText = GET_TEXT_BY_ID(TXT_SECOND);
            }
            textStartAt->setString(fmt::format(GET_TEXT_BY_ID(TXT_TIME_AGO), time, timeText));
        }
    }
}

void MailboxItem::setTimeEnd(double endAt)
{
    auto textEndAt = static_cast<Text*>(Helper::seekWidgetByName(this->_layout, "TextEndAt"));
    textEndAt->setString("");
    if (endAt > 0)
    {
        std::string time       = "";
        time_t      time1      = (time_t) endAt;
        tm          localTime1 = *localtime(&time1);
        std::string year       = StringUtils::format("%02d", (int) (localTime1.tm_year + 1900));
        std::string month      = StringUtils::format("%02d", (int) (localTime1.tm_mon + 1));
        std::string day        = StringUtils::format("%02d", (int) (localTime1.tm_mday));
        textEndAt->setString(fmt::format(GET_TEXT_BY_ID(TXT_MAILBOX_REWARD_EXPIRE_TIME), fmt::format(GET_TEXT_BY_ID(TXT_DATE), year.c_str(), month.c_str(), day.c_str())));
    }
}

void MailboxItem::initIcon()
{
    auto iconMail = static_cast<ImageView*>(Helper::seekWidgetByName(this->_layout, "ImageIcon"));
    iconMail->setColor(Color3B::WHITE);
    if (_type == MAILBOX_TYPE_REWARD)
    {
        iconMail->loadTexture("ui/ImagesCommon/icon_rewards.png", Widget::TextureResType::PLIST);
        iconMail->setColor(Color3B(255, 245, 182));
    }
    else
    {
        iconMail->loadTexture("ui/ImagesCommon/icon_mail.png", Widget::TextureResType::PLIST);
    }
}

const std::string& MailboxItem::getId()
{
    return _id;
}

const std::string& MailboxItem::getType()
{
    return _type;
}

cocos2d::ui::Layout* MailboxItem::getLayout()
{
    return _layout;
}

void MailboxItem::setRewards(const std::string& rewardsStr)
{
    const rapidjson::Document& rewards = JsonHelper::parseStringToJson(rewardsStr);
    if (rewards.IsArray() && rewards.Size() > 0)
    {
        for (int i = 0; i < rewards.Size(); i++)
        {
            const rapidjson::Value& reward = rewards[i];
            std::string rewardType  = JsonHelper::getString(reward, "type", "");
            double      rewardValue = JsonHelper::getDouble(reward, "value", 0);
            if (rewardType == "money" && rewardValue > 0)
            {
                auto layoutReward = static_cast<Layout*>(Helper::seekWidgetByName(this->_layout, "PanelReward"));
                layoutReward->setVisible(true);
                auto textMoney = static_cast<TextBMFont*>(Helper::seekWidgetByName(layoutReward, "LabelMoney"));
                textMoney->setVisible(true);
                textMoney->setString(GameUtils::MoneyFormatShortCut(rewardValue, "+"));
                break;
            }
        }
    }
}
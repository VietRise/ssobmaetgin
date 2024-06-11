#include "SvrMailbox.h"
#include "Define/GameDefine.h"
#include "ServerOffline/SvrUserProfile.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Helper/GameUtils.h"
#include "FirebaseGG/RemoteConfig.h"
#include "ServerOffline/ServerOffline.h"
#include "Define/ResourcesDefine.h"

std::vector<std::string> SvrMailbox::_mailBoxChecked;
std::vector<MailboxData> SvrMailbox::_listMailbox;

std::vector<MailboxData> SvrMailbox::mergeMailboxLocal(const std::vector<MailboxData>& mailboxRemote)
{
    std::vector<MailboxData> mailboxList;
    mailboxList.insert(mailboxList.begin(), mailboxRemote.begin(), mailboxRemote.end());
    std::string              json = GameUtils::readStringFromFileDecode(MAILBOX_LOCAL_FILENAME, "[]");
    std::vector<MailboxData> list = SvrMailbox::parseMailboxJson(json);
    mailboxList.insert(mailboxList.begin(), list.begin(), list.end());
    return mailboxList;
}

std::vector<std::string> SvrMailbox::getMailboxCheckedList()
{
    std::vector<std::string> list;
    std::string              json = GameUtils::readStringFromFileDecode(MAILBOX_CHECKED_FILENAME, "[]");
    if (json.empty() != true)
    {
        rapidjson::Document doc = JsonHelper::parseStringToJson(json);
        if (doc.IsArray())
        {
            for (int i = 0; i < doc.Size(); i++)
            {
                if (doc[i].IsString())
                {
                    std::string id = doc[i].GetString();
                    if (id != "")
                        list.push_back(id);
                }
            }
        }
    }
    //backup list
    json = GameUtils::getStringForKeyUserDefault(KEY_LIST_MAILBOX_CHECKED, "[]");
    if (json.empty() != true)
    {
        rapidjson::Document doc = JsonHelper::parseStringToJson(json);
        if (doc.IsArray())
        {
            for (int i = 0; i < doc.Size(); i++)
            {
                if (doc[i].IsString() && std::find(list.begin(), list.end(), doc[i].GetString()) == list.end())
                {
                    std::string id = doc[i].GetString();
                    if (id != "")
                        list.push_back(id);
                }
            }
        }
    }
    return list;
}

void SvrMailbox::setMailboxChecked(const MailboxData& mail)
{
    std::string         json = GameUtils::readStringFromFileDecode(MAILBOX_CHECKED_FILENAME, "[]");
    rapidjson::Document doc  = JsonHelper::parseStringToJson(json);
    if (!doc.IsArray())
    {
        doc = rapidjson::Document();
        doc.SetArray();
    }
    if (doc.IsArray())
    {
        std::string data = mail._type + "_" + mail._id;
        doc.PushBack(rapidjson::Value(rapidjson::kStringType).SetString(data.c_str(), doc.GetAllocator()), doc.GetAllocator());
        GameUtils::writeStringToFileEncode(MAILBOX_CHECKED_FILENAME, JsonHelper::convertDocumentToString(doc));

        //backup in case that file can not save
        rapidjson::Document backupList;
        backupList.SetArray();
        int start = doc.Size() - 20;
        if (start < 0)
            start = 0;
        for (int i = start; i >= 0 && i < doc.Size(); i++)
            backupList.PushBack(rapidjson::Value(rapidjson::kStringType).SetString(doc[i].GetString(), doc.GetAllocator()), doc.GetAllocator());
        GameUtils::setStringForKeyUserDefault(KEY_LIST_MAILBOX_CHECKED, JsonHelper::convertDocumentToString(backupList));
    }
}

bool SvrMailbox::isMailboxChecked(const MailboxData& mail)
{
    for (int i = 0; i < SvrMailbox::_mailBoxChecked.size(); i++)
    {
        auto mailChecked = SvrMailbox::_mailBoxChecked[i];
        if (SvrMailbox::_mailBoxChecked[i] == (mail._type + "_" + mail._id))
            return true;
    }
    return false;
}

bool SvrMailbox::isValidMailbox(const MailboxData& mail, const int& versionCodeApp)
{
    long long currentTime = GameUtils::getCurrentTime();
    if (mail._version <= versionCodeApp && mail._startAt <= currentTime && (currentTime <= mail._endAt || mail._endAt <= 0))
    {
        if (mail._type == MAILBOX_TYPE_REWARD)
        {
            if (isValidMailboxReward(mail) && !isMailboxChecked(mail))
                return true;
            return false;
        }
        else if (mail._type == MAILBOX_TYPE_UPDATE)
        {
            std::string versionCodeStr = mail._data;
            if (versionCodeStr != "")
            {
                int versionCode        = atoi(versionCodeStr.c_str());
                int currentVersionCode = SvrUserProfile::getVersionCode();
                if (currentVersionCode < versionCode)
                    return true;
            }
            return false;
        }
        else if (mail._type == MAILBOX_TYPE_FUNCTION)
        {
            if (mail._data == MAILBOX_FUNCTION_ADS_KILLER)
            {
                return false;
                if (mail._data == MAILBOX_FUNCTION_ADS_KILLER && !SvrUserProfile::getInterferingAdsEnable())
                    return false;
            }
            return true;
        }
        else
        {
            return true;
        }
    }
    return false;
}

void SvrMailbox::removeOldMailboxChecked()
{
    std::vector<std::string> checked = SvrMailbox::getMailboxCheckedList();
    if (checked.size() > 0)
    {
        bool removed = false;
        int  limit   = RemoteConfig::getInteger(REMOTECONFIG_FUNCTION_MAILBOX_CHECKED_LIMIT, 100);
        if (checked.size() > limit)
        {
            for (auto iter = checked.begin(); iter != checked.end();)
            {
                if (checked.size() > limit - 10)
                {
                    iter    = checked.erase(iter);
                    removed = true;
                }
                else
                {
                    break;
                }
            }
        }
        if (removed)
        {
            rapidjson::Document doc;
            doc.SetArray();
            for (auto iter = checked.begin(); iter != checked.end(); iter++)
                doc.PushBack(rapidjson::Value(rapidjson::kStringType).SetString(iter->c_str(), doc.GetAllocator()), doc.GetAllocator());
            GameUtils::writeStringToFileEncode(MAILBOX_CHECKED_FILENAME, JsonHelper::convertDocumentToString(doc));
        }
    }
}

const MailboxData& SvrMailbox::getMailbox(const std::string& type, const std::string& id)
{
    for (int i = 0; i < SvrMailbox::_listMailbox.size(); i++)
    {
        const MailboxData& mailbox = SvrMailbox::_listMailbox.at(i);
        if (mailbox._type == type && mailbox._id == id)
            return mailbox;
    }
    static MailboxData defaultMailboxData;
    return defaultMailboxData;
}

bool SvrMailbox::claimMailboxRewards(const rapidjson::Document& reward)
{
    if (reward.IsArray() && reward.Size() > 0)
    {
        bool     res = false;
        for (int i   = 0; i < reward.Size(); i++)
        {
            std::string rewardType  = JsonHelper::getString(reward[i], KEY_MAIL_REWARD_TYPE, "");
            double      rewardValue = JsonHelper::getDouble(reward[i], KEY_MAIL_REWARD_VALUE, 0);
            if (claimMailboxReward(rewardType, rewardValue) == true)
                res = true;
        }
        return res;
    }
    return false;
}

bool SvrMailbox::claimMailboxReward(const std::string& type, double value)
{
    if (type == "money" && value > 0)
    {
        SvrUserProfile::setMoney("ClaimMailboxReward", value);
        ServerOffline::_getBonusGold = true;
        return true;
    }
    return false;
}

double SvrMailbox::getMailboxRewardValue(const MailboxData& mailboxItem, const std::string& type)
{
    rapidjson::Document rewardJson = JsonHelper::parseStringToJson(mailboxItem._reward);
    if (rewardJson.IsArray() && rewardJson.Size() > 0)
    {
        for (int i = 0; i < rewardJson.Size(); i++)
        {
            std::string rewardType = JsonHelper::getString(rewardJson[i], "type", "");
            if (rewardType == type)
                return JsonHelper::getDouble(rewardJson[i], "value", 0);
        }
    }
    return 0;
}

void SvrMailbox::sortMailboxList()
{
    if (SvrMailbox::_listMailbox.size() > 0)
        std::sort(SvrMailbox::_listMailbox.begin(), SvrMailbox::_listMailbox.end(), [](const MailboxData& a, const MailboxData& b) {
            return a._startAt > b._startAt;
        });
}

bool SvrMailbox::isValidMailboxReward(const MailboxData& mailboxItem)
{
    if (mailboxItem._type == MAILBOX_TYPE_REWARD && mailboxItem._id != "" && mailboxItem._reward != "" && mailboxItem._reward != "[]")
        return true;
    return false;
}

std::vector<MailboxData> SvrMailbox::getMailboxList()
{
    std::vector<MailboxData> list;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_MAILBOX_ENABLE, true))
#else
        if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_MAILBOX_ENABLE, true))
#endif
    {
        std::string              json        = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_MAILBOX_DATA, Resources::JSON::JSON_FILE_LIST_MAILBOXS);
        std::vector<MailboxData> listMailbox = SvrMailbox::mergeMailboxLocal(SvrMailbox::parseMailboxJson(json));
        for (int                 i           = 0; i < listMailbox.size(); i++)
            if (SvrMailbox::isValidMailbox(listMailbox[i], s_versionCode))
                list.push_back(listMailbox[i]);
        SvrMailbox::sortMailboxList();
    }
    return list;
}

void SvrMailbox::addMailLocal(const MailboxData& mail)
{
    std::string         json = GameUtils::readStringFromFileDecode(MAILBOX_LOCAL_FILENAME, "[]");
    rapidjson::Document doc  = JsonHelper::parseStringToJson(json);
    if (!doc.IsArray())
    {
        doc = rapidjson::Document();
        doc.SetArray();
    }
    if (doc.IsArray())
    {
        rapidjson::Value value;
        value.SetObject();
        value.AddMember(KEY_MAIL_ID, rapidjson::Value(rapidjson::kStringType).SetString(mail._id.c_str(), doc.GetAllocator()), doc.GetAllocator());
        value.AddMember(KEY_MAIL_TYPE, rapidjson::Value(rapidjson::kStringType).SetString(mail._type.c_str(), doc.GetAllocator()), doc.GetAllocator());
        value.AddMember(KEY_MAIL_TITLE, rapidjson::Value(rapidjson::kObjectType).CopyFrom(JsonHelper::parseStringToJson(mail._title), doc.GetAllocator()), doc.GetAllocator());
        value.AddMember(KEY_MAIL_CONTENT, rapidjson::Value(rapidjson::kObjectType).CopyFrom(JsonHelper::parseStringToJson(mail._content), doc.GetAllocator()), doc.GetAllocator());
        value.AddMember(KEY_MAIL_FULL_CONTENT, rapidjson::Value(rapidjson::kObjectType).CopyFrom(JsonHelper::parseStringToJson(mail._fullContent), doc.GetAllocator()), doc.GetAllocator());
        value.AddMember(KEY_MAIL_REWARD, rapidjson::Value(rapidjson::kObjectType).CopyFrom(JsonHelper::parseStringToJson(mail._reward), doc.GetAllocator()), doc.GetAllocator());
        value.AddMember(KEY_MAIL_CHECK, mail._check, doc.GetAllocator());
        value.AddMember(KEY_MAIL_DATA, rapidjson::Value(rapidjson::kStringType).SetString(mail._data.c_str(), doc.GetAllocator()), doc.GetAllocator());
        value.AddMember(KEY_MAIL_START_AT, mail._startAt, doc.GetAllocator());
        value.AddMember(KEY_MAIL_END_AT, mail._endAt, doc.GetAllocator());
        value.AddMember(KEY_MAIL_LOCAL, true, doc.GetAllocator());
        doc.PushBack(value, doc.GetAllocator());
        GameUtils::writeStringToFileEncode(MAILBOX_LOCAL_FILENAME, JsonHelper::convertDocumentToString(doc));
    }
}

void SvrMailbox::removeMailLocal(const MailboxData& mailboxItem)
{
    std::string         json = GameUtils::readStringFromFileDecode(MAILBOX_LOCAL_FILENAME, "[]");
    rapidjson::Document doc  = JsonHelper::parseStringToJson(json);
    if (doc.IsArray())
    {
        for (auto iter = doc.Begin(); iter != doc.End();)
        {
            const rapidjson::Value& value = *iter;
            std::string id   = JsonHelper::getString(value, KEY_MAIL_ID, "");
            std::string type = JsonHelper::getString(value, KEY_MAIL_TYPE, "");
            if (id == mailboxItem._id && mailboxItem._type == type)
                iter = doc.Erase(iter);
            else
                iter++;
        }
        GameUtils::writeStringToFileEncode(MAILBOX_LOCAL_FILENAME, JsonHelper::convertDocumentToString(doc));
    }
}

std::vector<MailboxData> SvrMailbox::parseMailboxJson(const std::string& jsonData)
{
    std::vector<MailboxData> result;
    rapidjson::Document      document = JsonHelper::parseStringToJson(jsonData);
    _mailBoxChecked.clear();
    _mailBoxChecked = SvrMailbox::getMailboxCheckedList();
    if (document.IsArray() && document.Size() > 0)
    {
        for (int i = 0; i < document.Size(); i++)
        {
            const rapidjson::Value& object = document[i];
            MailboxData mailbox;
            mailbox._id          = JsonHelper::getString(object, KEY_MAIL_ID, "");
            mailbox._type        = JsonHelper::getString(object, KEY_MAIL_TYPE, "");
            mailbox._version     = JsonHelper::getInt(object, KEY_MAIL_VERSION, 0);
            mailbox._title       = JsonHelper::convertValueToString(JsonHelper::getValue(object, KEY_MAIL_TITLE, rapidjson::Value(rapidjson::kNullType)));
            mailbox._content     = JsonHelper::convertValueToString(JsonHelper::getValue(object, KEY_MAIL_CONTENT, rapidjson::Value(rapidjson::kNullType)));
            mailbox._fullContent = JsonHelper::convertValueToString(JsonHelper::getValue(object, KEY_MAIL_FULL_CONTENT, rapidjson::Value(rapidjson::kNullType)));
            mailbox._reward      = JsonHelper::convertValueToString(JsonHelper::getValue(object, KEY_MAIL_REWARD, rapidjson::Value(rapidjson::kNullType)));
            mailbox._check       = JsonHelper::getBool(object, KEY_MAIL_CHECK, true);
            mailbox._data        = JsonHelper::getString(object, KEY_MAIL_DATA, "");
            mailbox._startAt     = JsonHelper::getDouble(object, KEY_MAIL_START_AT, 0);
            mailbox._endAt       = JsonHelper::getDouble(object, KEY_MAIL_END_AT, 0);
            mailbox._isLocal     = JsonHelper::getBool(object, KEY_MAIL_LOCAL, false);
            result.push_back(mailbox);
        }
    }
    return result;
}
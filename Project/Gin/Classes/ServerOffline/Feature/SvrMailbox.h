#ifndef __SVR_MAILBOX_H__
#define __SVR_MAILBOX_H__

#include <string>
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"

class MailboxData
{
public:
    MailboxData()
    {
        _id      = "";
        _type    = "";
        _version = 0;
        _check   = true;
        _data    = "";
        _startAt = 0;
        _endAt   = 0;
        _isLocal = false;
    }

    std::string _id;
    std::string _type;
    int         _version;
    std::string _title;
    std::string _content;
    std::string _fullContent;
    std::string _reward;
    std::string _data;
    bool        _isLocal;
    double      _startAt;
    double      _endAt;
    bool        _check;
};

class SvrMailbox
{
private:
    static std::vector<std::string> _mailBoxChecked;

    static std::vector<MailboxData> parseMailboxJson(const std::string& jsonData);
    static std::vector<MailboxData> mergeMailboxLocal(const std::vector<MailboxData>& mailboxRemote);
    static std::vector<std::string> getMailboxCheckedList();

    static bool isMailboxChecked(const MailboxData& mail);
    static bool isValidMailbox(const MailboxData& mail, const int& versionCodeApp);
    static bool claimMailboxReward(const std::string& type, double value);
    static void sortMailboxList();
    static void addMailLocal(const MailboxData& mailboxItem);
protected:
public:
    static std::vector<MailboxData> getMailboxList();
    static const MailboxData& getMailbox(const std::string& type, const std::string& id);
    static bool isValidMailboxReward(const MailboxData& mailboxItem);
    static void removeOldMailboxChecked();
    static bool claimMailboxRewards(const rapidjson::Document& reward);
    static void setMailboxChecked(const MailboxData& mail);
    static void removeMailLocal(const MailboxData& mailboxItem);
    static double getMailboxRewardValue(const MailboxData& mailboxItem, const std::string& type);

    static std::vector<MailboxData> _listMailbox;
};

#endif //__SVR_MAILBOX_H__

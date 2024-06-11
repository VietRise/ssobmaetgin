#include "BotConfig.h"
#include "Define/InGameDefine.h"
#include "Define/ResourcesDefine.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Helper/DebugHelper.h"
#include "Helper/JsonHelper.h"
#include "Helper/LogHelper.h"
#include "ServerOffline/SvrUserProfile.h"
#include "ServerOffline/Feature/SvrLog.h"

using namespace std;

string    BotConfig::_confJson;
BotConfig BotConfig::_conf;

int BotConfig::s_currentVersion = -1;

BotConfig BotConfig::loadConf()
{
    if (_confJson.empty())
    {
        _confJson = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_BOT_CONF_LEVEL, Resources::JSON::JSON_BOT_CONF_LEVEL);
        _conf     = BotConfig::deserialize(JsonHelper::parseStringToJson(_confJson));
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        CCASSERT(_confJson == _conf.toJson(true), StringHelper::format("Failed to load a JSON string to an object (remote_config:%s)", REMOTECONFIG_FUNCTION_BOT_CONF_LEVEL).c_str());
#endif
    }
    if (s_currentVersion == -1)
        s_currentVersion = SvrUserProfile::getBotVersion();
    return _conf;
}

BotVersion BotConfig::getVersion(const BotConfig& botConf, const double& userWinRate, const int& userMatchCount, const double& userMoney)
{
    bool      hasChanged = false;
    const int oldVersion = s_currentVersion;
    if (botConf.versions.find(s_currentVersion) != botConf.versions.end())
    {
        // Checking to upgrade or downgrade version
        BotVersion botVersion = botConf.versions.at(s_currentVersion);
        // Upgrade
        if (userWinRate >= botVersion.upVerWinRate && botConf.versions.find(botVersion.upVer) != botConf.versions.end() && botVersion.upVer > s_currentVersion)
        {
            hasChanged       = true;
            s_currentVersion = botVersion.upVer;
#if DEBUG_LOG_BOT
            vector<pair<string, string>> logs;
            DebugHelper::action2Log("Old version", oldVersion, logs);
            DebugHelper::action2Log("New version", s_currentVersion, logs);
            LogHelper::match("BOT UPGRADE VERSION", logs);
#endif
        }

        // Downgrade
        if (userWinRate <= botVersion.downVerWinRate && botConf.versions.find(botVersion.downVer) != botConf.versions.end() && botVersion.downVer < s_currentVersion)
        {
            hasChanged       = true;
            s_currentVersion = botVersion.downVer;
#if DEBUG_LOG_BOT
            vector<pair<string, string>> logs;
            DebugHelper::action2Log("Old version", oldVersion, logs);
            DebugHelper::action2Log("New version", s_currentVersion, logs);
            LogHelper::match("BOT DOWNGRADE VERSION", logs);
#endif
        }
    }
    else
    {
        if (botConf.versionDef != s_currentVersion)
        {
            hasChanged       = true;
            s_currentVersion = botConf.versionDef;
        }
    }
    if (hasChanged)
    {
        SvrUserProfile::setBotVersion(s_currentVersion);
        SvrLog::botUpDownVersion(s_currentVersion, oldVersion, userWinRate, userMatchCount, userMoney);
    }
    return botConf.versions.at(s_currentVersion);
}

BotVersion BotConfig::getCurrentVersion()
{
    return _conf.versions.at(s_currentVersion);
}

int BotConfig::getIndexLevel(const int& level)
{
    const auto& version = getCurrentVersion();
    for (int i = 0; i < version.levels.size(); i++)
        if (version.levels[i] == level)
            return i;
    return -1;
}

int BotConfig::getShuffleRemainCards(const int& level)
{
    const auto& version = getCurrentVersion();
    int index = getIndexLevel(level);
    if (index >= 0 && index < version.data.size())
        return version.data.at(index).shuffle;
    return 0;
}

vector<int> BotConfig::getClassicKnock(const int& level)
{
    const auto& version = getCurrentVersion();
    int index = getIndexLevel(level);
    if (index >= 0 && index < version.data.size())
        return version.data.at(index).classic;
    return {};
}

vector<vector<int>> BotConfig::getOklahomaKnock(const int& level)
{
    const auto& version = getCurrentVersion();
    int index = getIndexLevel(level);
    if (index >= 0 && index < version.data.size())
        return version.data.at(index).oklahoma;
    return {};
}
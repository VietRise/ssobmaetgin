#ifndef __BOT_CONFIG_H__
#define __BOT_CONFIG_H__

#include <vector>
#include <string>
#include <map>

#include "json/rapidjson.h"
#include "json/document.h"

#include "BotConstant.h"
#include "Define/GameDefine.h"
#include "Helper/JsonHelper.h"
#include "Helper/StringHelper.h"

// BotLevel class
#define BOT_JSON_KEY_SHUFFLE "shuffle"
#define BOT_JSON_KEY_CLASSIC_KNOCK "classic_knock"
#define BOT_JSON_KEY_OKLAHOMA_KNOCK "oklahoma_knock"

// BotWinRate class
#define BOT_JSON_KEY_RATIO "ratio"
#define BOT_JSON_KEY_PERCENT "percent"

// BotVersion class
#define BOT_JSON_KEY_VERSION "ver"
#define BOT_JSON_KEY_LEVEL_DEFAULT "leveldef"
#define BOT_JSON_KEY_LEVELS "levels"
#define BOT_JSON_KEY_UP_VERSION "upver"
#define BOT_JSON_KEY_UP_VERSION_WIN_RATE "upver_winrate"
#define BOT_JSON_KEY_DOWN_VERSION "downver"
#define BOT_JSON_KEY_DOWN_VERSION_WIN_RATE "downver_winrate"
#define BOT_JSON_KEY_WIN_RATES "winrates"
#define BOT_JSON_KEY_DATA "data"

// BotConfig class
#define BOT_JSON_KEY_ENABLE "enable"
#define BOT_JSON_KEY_START_MATCH "startmatch"
#define BOT_JSON_KEY_VERSION_DEFAULT "versiondef"
#define BOT_JSON_KEY_VERSIONS "versions"

struct BotLevel
{
    int                           shuffle = 0;
    std::vector<int>              classic;
    std::vector<std::vector<int>> oklahoma;

    static BotLevel deserialize(const rapidjson::Value& val)
    {
        BotLevel data;
        data.shuffle  = JsonHelper::getInt(val, BOT_JSON_KEY_SHUFFLE, 0);
        data.classic  = JsonHelper::getVectorInt(val, BOT_JSON_KEY_CLASSIC_KNOCK);
        data.oklahoma = JsonHelper::getVectorVectorInt(val, BOT_JSON_KEY_OKLAHOMA_KNOCK);
        return data;
    }

    rapidjson::Value serialize(rapidjson::Document& doc) const
    {
        rapidjson::Document::AllocatorType& al = doc.GetAllocator();
        rapidjson::Value val;
        val.SetObject();
        val.AddMember(BOT_JSON_KEY_SHUFFLE, shuffle, al);
        val.AddMember(BOT_JSON_KEY_CLASSIC_KNOCK, JsonHelper::makeValueArrayInt(doc, classic), al);
        val.AddMember(BOT_JSON_KEY_OKLAHOMA_KNOCK, JsonHelper::makeValueArrayArrayInt(doc, oklahoma), al);
        return val;
    }
};

struct BotWinRate
{
    int              ratio = 0;
    std::vector<int> percent;

    static BotWinRate deserialize(const rapidjson::Value& val)
    {
        BotWinRate data;
        data.ratio   = JsonHelper::getInt(val, BOT_JSON_KEY_RATIO, 0);
        data.percent = JsonHelper::getVectorInt(val, BOT_JSON_KEY_PERCENT);
        return data;
    }

    rapidjson::Value serialize(rapidjson::Document& doc) const
    {
        rapidjson::Document::AllocatorType& al = doc.GetAllocator();
        rapidjson::Value val;
        val.SetObject();
        val.AddMember(BOT_JSON_KEY_RATIO, ratio, al);
        val.AddMember(BOT_JSON_KEY_PERCENT, JsonHelper::makeValueArrayInt(doc, percent), al);
        return val;
    }
};

struct BotVersion
{
    int                     version        = (int) BotConstant::Version::VERSION_0;
    std::vector<int>        levels;
    int                     levelDef       = (int) BotConstant::Level::LEVEL_0;
    int                     upVer          = (int) BotConstant::Version::VERSION_0;
    int                     upVerWinRate   = 0;
    int                     downVer        = (int) BotConstant::Version::VERSION_0;
    int                     downVerWinRate = 0;
    std::vector<BotWinRate> winRates;
    std::vector<BotLevel>   data;

    static BotVersion deserialize(const rapidjson::Value& val)
    {
        BotVersion data;
        data.version        = JsonHelper::getInt(val, BOT_JSON_KEY_VERSION, 0);
        data.levels         = JsonHelper::getVectorInt(val, BOT_JSON_KEY_LEVELS);
        data.levelDef       = JsonHelper::getInt(val, BOT_JSON_KEY_LEVEL_DEFAULT, (int) BotConstant::Level::LEVEL_0);
        data.upVer          = JsonHelper::getInt(val, BOT_JSON_KEY_UP_VERSION, 0);
        data.upVerWinRate   = JsonHelper::getInt(val, BOT_JSON_KEY_UP_VERSION_WIN_RATE, 0);
        data.downVer        = JsonHelper::getInt(val, BOT_JSON_KEY_DOWN_VERSION, 0);
        data.downVerWinRate = JsonHelper::getInt(val, BOT_JSON_KEY_DOWN_VERSION_WIN_RATE, 0);

        const rapidjson::Value& winRateArr = JsonHelper::getValue(val, BOT_JSON_KEY_WIN_RATES);
        if (winRateArr.IsArray())
        {
            for (int i = 0; i < winRateArr.Size(); i++)
                if (winRateArr[i].IsObject())
                    data.winRates.push_back(BotWinRate::deserialize(winRateArr[i]));
        }

        const rapidjson::Value& dataArr = JsonHelper::getValue(val, BOT_JSON_KEY_DATA);
        if (dataArr.IsArray())
        {
            for (int i = 0; i < dataArr.Size(); i++)
                if (dataArr[i].IsObject())
                    data.data.push_back(BotLevel::deserialize(dataArr[i]));
        }
        return data;
    }

    rapidjson::Value serialize(rapidjson::Document& doc) const
    {
        rapidjson::Document::AllocatorType& al = doc.GetAllocator();

        rapidjson::Value winRateArr;
        winRateArr.SetArray();
        for (const auto& wr : winRates)
            winRateArr.PushBack(wr.serialize(doc), al);

        rapidjson::Value dataArr;
        dataArr.SetArray();
        for (const auto& wr : data)
            dataArr.PushBack(wr.serialize(doc), al);

        rapidjson::Value val;
        val.SetObject();
        val.AddMember(BOT_JSON_KEY_VERSION, version, al);
        val.AddMember(BOT_JSON_KEY_LEVEL_DEFAULT, levelDef, al);
        val.AddMember(BOT_JSON_KEY_LEVELS, JsonHelper::makeValueArrayInt(doc, levels), al);
        val.AddMember(BOT_JSON_KEY_UP_VERSION, upVer, al);
        val.AddMember(BOT_JSON_KEY_UP_VERSION_WIN_RATE, upVerWinRate, al);
        val.AddMember(BOT_JSON_KEY_DOWN_VERSION, downVer, al);
        val.AddMember(BOT_JSON_KEY_DOWN_VERSION_WIN_RATE, downVerWinRate, al);
        val.AddMember(BOT_JSON_KEY_WIN_RATES, winRateArr, al);
        val.AddMember(BOT_JSON_KEY_DATA, dataArr, al);
        return val;
    }
};

class BotConfig
{
public:
    bool                      enable     = false;
    int                       startMatch = 0;
    int                       versionDef = (int) BotConstant::Version::VERSION_0;
    std::map<int, BotVersion> versions;

    static BotConfig deserialize(const rapidjson::Value& val)
    {
        BotConfig data;
        data.enable     = JsonHelper::getInt(val, BOT_JSON_KEY_VERSION, 0);
        data.enable     = JsonHelper::getBool(val, BOT_JSON_KEY_ENABLE, false);
        data.startMatch = JsonHelper::getInt(val, BOT_JSON_KEY_START_MATCH, 0);
        data.versionDef = JsonHelper::getInt(val, BOT_JSON_KEY_VERSION_DEFAULT, (int) BotConstant::Version::VERSION_0);

        const rapidjson::Value& versionJsonArray = JsonHelper::getValue(val, BOT_JSON_KEY_VERSIONS);
        if (versionJsonArray.IsArray())
        {
            for (int i = 0; i < versionJsonArray.Size(); i++)
            {
                const rapidjson::Value& versionJsonObject = versionJsonArray[i];
                if (versionJsonObject.IsObject())
                {
                    BotVersion botVersion = BotVersion::deserialize(versionJsonObject);
                    data.versions[botVersion.version] = botVersion;
                }
            }
        }

        return data;
    }

    rapidjson::Document serialize() const
    {
        rapidjson::Document doc;
        rapidjson::Document::AllocatorType& al = doc.GetAllocator();

        rapidjson::Value versionArr;
        versionArr.SetArray();
        for (const auto& it : versions)
            versionArr.PushBack(it.second.serialize(doc), al);

        doc.SetObject();
        doc.AddMember(BOT_JSON_KEY_ENABLE, enable, al);
        doc.AddMember(BOT_JSON_KEY_START_MATCH, startMatch, al);
        doc.AddMember(BOT_JSON_KEY_VERSION_DEFAULT, versionDef, al);
        doc.AddMember(BOT_JSON_KEY_VERSIONS, versionArr, al);
        return doc;
    }

    std::string toJson(bool pretty) const
    {
        return JsonHelper::convertDocumentToString(serialize(), pretty);
    }

    static int s_currentVersion;

private:
    static std::string _confJson;
    static BotConfig   _conf;

    static BotVersion getCurrentVersion();
    static int getIndexLevel(const int& level);

public:
    static BotConfig loadConf();
    static BotVersion getVersion(const BotConfig& botConf, const double& userWinRate, const int& userMatchCount, const double& userMoney);

    static int getShuffleRemainCards(const int& level);
    static std::vector<int> getClassicKnock(const int& level);
    static std::vector<std::vector<int>> getOklahomaKnock(const int& level);
};

#endif // __BOT_CONFIG_H__
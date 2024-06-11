#include "SvrWelcomeBackBonus.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "FirebaseGG/RemoteConfig.h"

std::vector<std::pair<int, double>> SvrWelcomeBackBonus::getWelcomeBackBonus(int userlevel)
{
    std::vector<std::pair<int, double>> listData;
    const rapidjson::Document& document = JsonHelper::parseStringToJson(RemoteConfig::getString(REMOTECONFIG_FUNCTION_WELCOME_BACK_DATA, "[]"));
    if (document.IsArray() && document.Size() > 0)
    {
        int      index = -1;
        for (int i     = 0; i < document.Size(); i++)
        {
            if (JsonHelper::getInt(document[i], "lv", 0) > userlevel)
                break;
            index = i;
        }
        if (index != -1)
        {
            const rapidjson::Value& datas = JsonHelper::getValue(document[index], "data");
            if (datas.IsArray())
            {
                for (int i = 0; i < datas.Size(); i++)
                {
                    std::pair<int, double> pair = std::make_pair<int, double>(datas[i][0].GetInt(), (double) datas[i][1].GetInt64());
                    listData.push_back(pair);
                }
            }
        }
    }
    return listData;
}

bool SvrWelcomeBackBonus::isWelcomeBackEnable()
{
    return RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_WELCOME_BACK_ENABLE, false);
}

std::pair<int, double> SvrWelcomeBackBonus::getWelcomeBackBonus(int userlevel, double dayDiff)
{
    if (dayDiff >= WELLCOMEBACK_DAY_DIFF && SvrWelcomeBackBonus::isWelcomeBackEnable())
    {
        const rapidjson::Document& document = JsonHelper::parseStringToJson(RemoteConfig::getString(REMOTECONFIG_FUNCTION_WELCOME_BACK_DATA, "[]"));
        if (document.IsArray() && document.Size() > 0)
        {
            int      index = -1;
            for (int i     = 0; i < document.Size(); i++)
            {
                if (JsonHelper::getInt(document[i], "lv", 0) > userlevel)
                    break;
                index = i;
            }
            if (index != -1)
            {
                const rapidjson::Value& datas = JsonHelper::getValue(document[index], "data");
                if (!datas.IsNull())
                {
                    int      idx = -1;
                    for (int i   = 0; i < datas.Size(); i++)
                    {
                        const rapidjson::Value& data = datas[i];
                        if (data.Size() >= 1 && data[0].GetInt() > dayDiff)
                            break;
                        idx                          = i;
                    }
                    if (idx != -1 && datas.Size() >= idx + 1 && datas[idx].Size() >= 2)
                        return std::make_pair(datas[idx][0].GetInt(), (double) datas[idx][1].GetInt64());
                }
            }
        }
    }
    return std::make_pair(0, 0);
}

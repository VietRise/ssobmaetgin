#include "SvrBotHelper.h"
#include "Helper/JsonHelper.h"
#include "InGameServerOfflineDefines.h"
#include "Helper/GameUtils.h"
#include "Define/ResourcesDefine.h"
#include "FirebaseGG/RemoteConfig.h"
#include "ServerOffline/SvrUserProfile.h"

using namespace std;

vector<string> SvrBotHelper::listName;
vector<string> SvrBotHelper::listNickName;
vector<string> SvrBotHelper::listAvatar;
string         SvrBotHelper::listBotJson;

int SvrBotHelper::randomHelper(int i)
{
    if (i > 0)
        return cocos2d::RandomHelper::random_int(0, i - 1);
    return 0;
}

vector<string> SvrBotHelper::getListRandomName()
{
    if (listName.size() == 0)
    {
        const rapidjson::Document& doc  = JsonHelper::parseStringToJson(SvrBotHelper::loadBotDataJson());
        const rapidjson::Value   & list = doc[JSON_BOT_NAME];
        for (int i = 0; i < list.Size(); i++)
            listName.push_back(list[i].GetString());
    }
    std::random_shuffle(listName.begin(), listName.end(), randomHelper);
    return listName;
}

vector<string> SvrBotHelper::getListRandomNickName()
{
    if (listNickName.size() == 0)
    {
        const rapidjson::Document& doc  = JsonHelper::parseStringToJson(SvrBotHelper::loadBotDataJson());
        const rapidjson::Value   & list = doc[JSON_BOT_NICK_NAME];
        for (int i = 0; i < list.Size(); i++)
            listNickName.push_back(list[i].GetString());
    }
    std::random_shuffle(listNickName.begin(), listNickName.end(), randomHelper);
    return listNickName;
}

vector<string> SvrBotHelper::getListRandomImagePath()
{
    if (listAvatar.size() == 0)
    {
        const rapidjson::Document& doc  = JsonHelper::parseStringToJson(SvrBotHelper::loadBotDataJson());
        const rapidjson::Value   & list = doc[JSON_BOT_AVATAR];
        for (int i = 0; i < list.Size(); i++)
            listAvatar.push_back(list[i].GetString());
    }
    std::random_shuffle(listAvatar.begin(), listAvatar.end(), randomHelper);
    return listAvatar;
}

vector<SvrBotData> SvrBotHelper::getRandomBotInfo(int num, const GameMatchBundle& bundle, const vector<string>& checkedAvatar)
{
    vector<string>     listCheck = checkedAvatar;
    vector<SvrBotData> result;
    getListRandomName();
    getListRandomImagePath();
    getListRandomNickName();
    const int roomBet = bundle.price * BONUS_POINTS_BIG_GIN;
    for (int  i       = 0; i < num; i++)
    {
        SvrBotData data;
        data.name  = cocos2d::StringUtils::format("%s %s", listName.back().c_str(), listNickName.back().c_str());
        data.money = GameUtils::unifyMoneyValue((double) (cocos2d::RandomHelper::random_int(MIN_BOT_MONEY, MAX_BOT_MONEY)) * roomBet);
        string   avatar = "";
        for (int k      = 0; k < 5; k++)
        {
            std::random_shuffle(listAvatar.begin(), listAvatar.end(), randomHelper);
            avatar = listAvatar.back();
            if (std::find(listCheck.begin(), listCheck.end(), avatar) == listCheck.end())
                break;
        }
        data.avatar = avatar;
        listCheck.push_back(avatar);
        result.push_back(data);
        std::random_shuffle(listName.begin(), listName.end(), randomHelper);
        std::random_shuffle(listNickName.begin(), listNickName.end(), randomHelper);
        std::random_shuffle(listAvatar.begin(), listAvatar.end(), randomHelper);
    }
    return result;
}

vector<SvrBotData> SvrBotHelper::getBotInfo(int num, const GameMatchBundle& bundle, const vector<string>& checkedAvatar)
{
    return getRandomBotInfo(num, bundle, checkedAvatar);
}

SvrBotData SvrBotHelper::changeBot(SvrBotData& oldBot, const GameMatchBundle& bundle, const vector<string>& checkedAvatar)
{
    return SvrBotHelper::getRandomBotInfo(1, bundle, checkedAvatar)[0];
}

string SvrBotHelper::loadBotDataJson()
{
    if (SvrBotHelper::listBotJson.empty())
        SvrBotHelper::listBotJson = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_LIST_BOT_DATA, Resources::JSON::JSON_FILE_BOT_DATA);
    return SvrBotHelper::listBotJson;
}

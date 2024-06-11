#ifndef __SVR_BOT_HELPER_H__
#define __SVR_BOT_HELPER_H__

#include <string>
#include <vector>
#include "Define/InGameDefine.h"

class SvrBotData
{
public:
    std::string name;
    std::string avatar;
    double      money;

    void reset()
    {
        name   = "";
        avatar = "";
        money  = 0;
    }
};

class SvrBotHelper
{
private:
    static std::string              listBotJson;
    static std::vector<std::string> listName;
    static std::vector<std::string> listAvatar;
    static std::vector<std::string> listNickName;

    static std::vector<std::string> getListRandomName();
    static std::vector<std::string> getListRandomNickName();
    static std::vector<std::string> getListRandomImagePath();
    static int randomHelper(int i);
    static std::vector<SvrBotData> getRandomBotInfo(int num, const GameMatchBundle& bundle, const std::vector<std::string>& checkedAvatar);
    static std::string loadBotDataJson();
protected:
public:
    static std::vector<SvrBotData> getBotInfo(int num, const GameMatchBundle& bundle, const std::vector<std::string>& checkedAvatar);
    static SvrBotData changeBot(SvrBotData& oldBot, const GameMatchBundle& bundle, const std::vector<std::string>& checkedAvatar);
};

#endif

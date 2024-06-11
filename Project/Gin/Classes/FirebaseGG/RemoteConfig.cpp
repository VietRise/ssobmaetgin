#include "RemoteConfig.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "Helper/GameUtils.h"
#endif

using namespace std;

string RemoteConfig::getStringData(const string& remoteConfigKey, const string& filename)
{
    if (!remoteConfigKey.empty() && !filename.empty())
    {
        string json = RemoteConfig::getString(remoteConfigKey, "");
        if (json.empty() && cocos2d::FileUtils::getInstance()->isFileExist(filename))
            json = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        DLOG_FB(remoteConfigKey, filename);
#endif
        return json;
    }
    return "";
}
#include "RemoteConfig.h"
#include "Define/GameDefine.h"

using namespace std;

string RemoteConfig::getString(const string& key, const string& defaultValue)
{
    DLOG_FB(key, defaultValue);
    return defaultValue;
}

long RemoteConfig::getLong(const string& key, long defaultValue)
{
    DLOG_FB(key, defaultValue);
    return defaultValue;
}

int RemoteConfig::getInteger(const string& key, int defaultValue)
{
    DLOG_FB(key, defaultValue);
    return defaultValue;
}

double RemoteConfig::getDouble(const string& key, double defaultValue)
{
    DLOG_FB(key, defaultValue);
    return defaultValue;
}

bool RemoteConfig::getBoolean(const string& key, bool defaultValue)
{
    DLOG_FB(key, defaultValue);
    return defaultValue;
}

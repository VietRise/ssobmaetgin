#ifndef __REMOTE_CONFIG_H__
#define __REMOTE_CONFIG_H__

#include <string>
#include "Define/RemoteConfigDefine.h"

class RemoteConfig
{
public:
    static std::string getString(const std::string& key, const std::string& defaultValue);
    static long getLong(const std::string& key, long defaultValue);
    static int getInteger(const std::string& key, int defaultValue);
    static double getDouble(const std::string& key, double defaultValue);
    static bool getBoolean(const std::string& key, bool defaultValue);

    static std::string getStringData(const std::string& remoteConfigKey, const std::string& filename);
};

#endif //__REMOTE_CONFIG_H__
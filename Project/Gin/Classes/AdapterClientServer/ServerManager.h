#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include "json/rapidjson.h"
#include "json/document.h"

class ServerManager
{
private:
public:
    ~ServerManager();
    ServerManager();

    static ServerManager* getInstance();
    static void destroyInstance();

    void sendMessage(const rapidjson::Document& data);
    void onMessage(const rapidjson::Document& data);
};

#endif // !__SERVER_MANAGER_H__
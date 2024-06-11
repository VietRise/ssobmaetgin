#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__

#include "json/rapidjson.h"
#include "json/document.h"

class ClientManager
{
private:

public:
    ~ClientManager();
    ClientManager();

    static ClientManager* getInstance();
    static void destroyInstance();

    void sendMessage(const rapidjson::Document& data);
    void onMessage(const rapidjson::Document& data);
};

#endif // !__CLIENT_MANAGER_H__
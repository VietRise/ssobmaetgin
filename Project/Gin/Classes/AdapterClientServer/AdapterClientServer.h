#ifndef __ADAPTER_CLIENT_SERVER_H__
#define __ADAPTER_CLIENT_SERVER_H__

#include "cocos2d.h"
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"

class AdapterClientServer : public cocos2d::Node
{
private:
    std::vector<std::string> _queueMessageClientSender;
    std::vector<std::string> _queueMessageServerSender;
public:
    ~AdapterClientServer();
    AdapterClientServer();

    static AdapterClientServer* getInstance();
    static void destroyInstance();

    virtual bool init() override;
    virtual void update(float dt) override;

    void onProcessMessageFromClient();
    void onProcessMessageFromServer();

    void addMessageClientSender(const rapidjson::Document& message);
    void addMessageServerSender(const rapidjson::Document& message);
};

#endif // !__ADAPTER_CLIENT_SERVER_H__
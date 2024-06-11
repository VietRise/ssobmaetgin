#ifndef __DOWNLOAD_MANAGER_H__
#define __DOWNLOAD_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"

class RequestHandler;

class DownloadImageHandler;

class RequestDataHandler;

#define DATA_RESPONSE_CALLBACK_SELECTOR(_SELECTOR)  std::bind(&_SELECTOR, this, std::placeholders::_1)
typedef std::function<void(std::string data)>                      DataResponseCallback;

#define IMAGE_RESPONSE_CALLBACK_SELECTOR(_SELECTOR)  std::bind(&_SELECTOR, this, std::placeholders::_1, std::placeholders::_2)
typedef std::function<void(cocos2d::Sprite* sprite, bool success)> ImageResponseCallback;

class DownloadManager
{
private:
    DownloadManager();
    cocos2d::Vector<RequestHandler*>                                            _listRequestHandler;
    std::vector<std::pair<std::string, cocos2d::Vector<DownloadImageHandler*>>> _listImageRequest;
protected:
public:
    ~DownloadManager();

    static DownloadManager* getInstance();
    static void destroyInstance();

    void requestData(const std::string& url, const DataResponseCallback& callback);
    void downloadImage(cocos2d::Sprite* sprite, const std::string& url, const std::string& defaulFrameId, cocos2d::Size size, const ImageResponseCallback& callback = nullptr, void* target = nullptr);
    void removeHandler(RequestHandler* handler);
    void removeHandlerForTarget(void* target);

    void requestImageUrl(const std::string& url, DownloadImageHandler* handler);
    void finishRequestImageUrl(const std::string& url, bool success);
    void onImageDownloadCompleted(cocos2d::network::HttpClient* pSender, cocos2d::network::HttpResponse* pResponse);
    void saveImage(cocos2d::Image* image, const std::string& name, const std::string& path);
};

class RequestHandler : public cocos2d::Ref
{
private:
protected:
    void* _target;
    std::string _url;
public:
    RequestHandler();
    ~RequestHandler();

    void setTarget(void* target);
    void* getTarget();
    virtual void finish();
};

class DownloadImageHandler : public RequestHandler
{
private:
    cocos2d::Sprite* _sprite;
    cocos2d::Size         _size;
    ImageResponseCallback _callback;
protected:
public:
    DownloadImageHandler();
    ~DownloadImageHandler();

    void downloadImage(cocos2d::Sprite* sprite, const std::string& url, cocos2d::Size size);
    void setImageResponseCallback(const ImageResponseCallback& callback);
    void onImageDownloadCompleted(cocos2d::network::HttpClient* pSender, cocos2d::network::HttpResponse* pResponse);
    void onDownloadCompleted(const std::string& url, bool success);
    virtual void finish(bool success);
    void saveImage(cocos2d::Image* image, const std::string& name, const std::string& path);
    cocos2d::Image* loadImageFromCache(const std::string& name);
    void setSpriteSize(cocos2d::Size size);
    void setSpriteTexture(cocos2d::Sprite* sprite, cocos2d::Texture2D* texture);
};


class RequestDataHandler : public RequestHandler
{
private:
    std::string _data;

    DataResponseCallback _callback;
protected:
public:
    RequestDataHandler();
    ~RequestDataHandler();

    void requestData(const std::string& url, const DataResponseCallback& callback);
    void onRequestDataCompleted(cocos2d::network::HttpClient* pSender, cocos2d::network::HttpResponse* pResponse);
    virtual void finish();
};

#endif // __DOWNLOAD_MANAGER_H__
